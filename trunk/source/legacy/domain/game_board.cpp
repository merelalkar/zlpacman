
#include "domain.h"
using namespace Pegas;

GameBoard::GameBoard(): m_cardDeck(*this), m_portableCardStack(*this) 
{
	m_gameBoardSprite._left = 0;
	m_gameBoardSprite._top = 0;
	m_gameBoardSprite._texture = k_textureGameBoardFelt;
	m_gameBoardSprite._flags = k_repeatTextureAlongX|k_repeatTextureAlongY;

	GrafManager& grafManager = GrafManager::getInstance();
	alignGameObjects(grafManager.getCanvasWidth(), grafManager.getCanvasHeight());

    grafManager.addLayout(this);
	grafManager.addLayout(&m_portableCardStack);

	m_seriesPassAnimator.setCardDeck(&m_cardDeck);
	m_addBuildedStackAnimator.setBuildedStackDeck(&m_buildedStackDeck);
}

void GameBoard::draw(GrafManager& context)
{
	context.drawSprite(m_gameBoardSprite);

	m_buildedStackDeck.draw(context);
	m_scoreBoard.draw(context);
	m_cardDeck.draw(context);

	for(int i = 0; i < k_cardColumnCount; i++)
	{
		m_cardColumns[i].draw(context);
	}    
}

void GameBoard::resize(CURCOORD newWidth, CURCOORD newHeight)
{
	setWidth(newWidth);
	setHeight(newHeight);

	alignGameObjects(newWidth, newHeight);	
}

void GameBoard::alignGameObjects(CURCOORD boardWidth, CURCOORD boardHeight)
{
	CURCOORD leftOffset, topOffset;

	m_gameBoardSprite._width = boardWidth;
	m_gameBoardSprite._height = boardHeight;
    
	CURCOORD freeSpace = boardWidth - (k_playingCardWidth * k_cardColumnCount);
	CURCOORD barWidth = (CURCOORD)(freeSpace / (k_cardColumnCount + 1));

	leftOffset = barWidth;
	for(int i = 0; i < k_cardColumnCount; i++)
	{
		m_cardColumns[i].setLeft(leftOffset);
		m_cardColumns[i].setTop(k_borderOffset);

        leftOffset+= (barWidth + k_playingCardWidth);
	}

    topOffset = boardHeight - k_borderOffset - k_playingCardHeight;
	m_buildedStackDeck.setLeft(k_borderOffset);
	m_buildedStackDeck.setTop(topOffset);	

    topOffset = boardHeight - k_borderOffset - m_scoreBoard.getHeight();
	leftOffset = (CURCOORD)((boardWidth - m_scoreBoard.getWidth()) / 2);
	m_scoreBoard.setLeft(leftOffset);
	m_scoreBoard.setTop(topOffset);

	topOffset = boardHeight - k_borderOffset - k_playingCardHeight;
	leftOffset = boardWidth - k_borderOffset - m_cardDeck.getWidth();
	m_cardDeck.setLeft(leftOffset);
	m_cardDeck.setTop(topOffset);
}

void GameBoard::initialize(const CardList& cards)
{
	CardList columnCards;
	columnCards.reserve(6);

	const int k_numColumnsInFirstGroup = 4;
	const int k_numColumnsInSecondGroup = 6;
	const int k_numCardsInColumnFisrtGroup = 5;
	const int k_numCardsInColumnSecondGroup = 4;

	for(int i = 0; i < k_numColumnsInFirstGroup; i++)
	{
		CardList::const_iterator it_from = cards.begin() + i * k_numCardsInColumnFisrtGroup;
		CardList::const_iterator it_to = cards.begin() + (i + 1) * k_numCardsInColumnFisrtGroup;

		columnCards.insert(columnCards.begin(), it_from, it_to);
		m_cardColumns[i].initialize(columnCards);

		columnCards.clear();
	}
	
	const int k_totalCardsInFirstGroup = 20; //k_numColumnsInFirstGroup * k_numCardsInColumnFisrtGroup;
	for(int i = 0; i < k_numColumnsInSecondGroup; i++)
	{
		CardList::const_iterator it_from = cards.begin() + k_totalCardsInFirstGroup + i * k_numCardsInColumnSecondGroup;
		CardList::const_iterator it_to = cards.begin() + k_totalCardsInFirstGroup + (i + 1) * k_numCardsInColumnSecondGroup;

		columnCards.insert(columnCards.begin(), it_from, it_to);
		m_cardColumns[k_numColumnsInFirstGroup + i].initialize(columnCards);

		columnCards.clear();
	}

	m_cardDeck.initialize(SpiderGame::k_seriesToPass);
	m_buildedStackDeck.reset();
	m_scoreBoard.reset();	
}

void GameBoard::passNewSeries(const CardList& series)
{
	for(int i = 0; i < k_cardColumnCount; i++)
	{
		m_cardColumns[i].addCard(series[i]);
	}

	m_seriesPassAnimator.beginAnimation(series);
}

void GameBoard::showValidTurn()
{
	std::map<PlayingCard*, int> srcCards;
	std::map<PlayingCard*, int> dstCards;
	std::vector<std::pair<int, int> > validTurns;
	std::vector<std::pair<int, PlayingCard*> > indexMap;

	for(int iColumn = 0; iColumn < k_cardColumnCount; iColumn++)
	{
		CardStack* topStack = m_cardColumns[iColumn].getTopStack();
		if(topStack == 0) 
		{
			continue;
		}

		dstCards.insert(std::make_pair(topStack->getTopCard(), iColumn));

		CardStackIterator& it = topStack->getCardIterator();
		for(; !it.isLast(); it.moveNext())
		{
			PlayingCard* card = it.getCurrent();
			srcCards.insert(std::make_pair(card, iColumn));
		}
	}

	
	for(std::map<PlayingCard*, int>::iterator src_it = srcCards.begin();
		src_it != srcCards.end(); ++src_it)
	{
		PlayingCard* srcCard = src_it->first;
		int iSrcColumn = src_it->second;

		for(std::map<PlayingCard*, int>::iterator dst_it = dstCards.begin();
			dst_it != dstCards.end(); ++dst_it)
		{
			PlayingCard* dstCard = dst_it->first;
			int iDstColumn = dst_it->second;

			if(iSrcColumn == iDstColumn) continue;

			if((srcCard->getRank() + 1) == dstCard->getRank())
			{
				validTurns.push_back(std::make_pair(src_it->second, dst_it->second));
				
				int index = validTurns.size() - 1;
				indexMap.push_back(std::make_pair(index, srcCard));				
			}
		}//for(std::map<PlayingCard*, int>::iterator dst_it = dstCards.begin();
	}//for(std::map<PlayingCard*, int>::iterator src_it = srcCards.begin();

	SoundManager& sndManager = SoundManager::getInstance(); 
	if(validTurns.size() > 0)
	{
		std::random_shuffle(indexMap.begin(), indexMap.end());
		int index = indexMap[0].first;
		int iSrcColumn = validTurns[index].first;
		int iDstColumn = validTurns[index].second;
		PlayingCard* srcCard = indexMap[0].second;
		PlayingCard* dstCard = m_cardColumns[iDstColumn].getTopCard();

		CardStack* stack = m_cardColumns[iSrcColumn].getTopStack();
		CardStack::PlayingCardsStack& cards = stack->getCards();
		CardStack::PlayingCardsStack::iterator find_it = std::find(cards.begin(), cards.end(), srcCard);		
		CardList srcCardList(find_it, cards.end());
				

		sndManager.playSound(k_soundValidTurnExist);		
		m_showValidTurnAnimator.beginAnimation(srcCardList, dstCard);				
	}else
	{
		sndManager.playSound(k_soundValidTurnAbsent);
	}
}

bool GameBoard::hasEmptyColumns()
{
	for(int i = 0; i < k_cardColumnCount; i++)
	{
		if(m_cardColumns[i].isEmpty())
		{
			return true;
		}
	}

	return false;
}


void GameBoard::addBuildedCardStack(CardStack& stack)
{
	m_scoreBoard.addScores(SpiderGame::k_scoresBuildStack);

	m_addBuildedStackAnimator.beginAnimation(stack);	
}

MakeTurnCommand* GameBoard::putCardsToColumn(PortableCardStack& portableStack)
{
	std::set<CardColumn*> columns;
	CURCOORD stackCoords[4][2];
	stackCoords[0][0] = portableStack.getLeft();
	stackCoords[0][1] = portableStack.getTop();
	stackCoords[1][0] = portableStack.getLeft() + portableStack.getWidth();
	stackCoords[1][1] = portableStack.getTop();
	stackCoords[2][0] = portableStack.getLeft();
	stackCoords[2][1] = portableStack.getTop() + portableStack.getHeight();
	stackCoords[3][0] = portableStack.getLeft() + portableStack.getWidth();
	stackCoords[3][1] = portableStack.getTop() + portableStack.getHeight();

	for(int iColumn = 0; iColumn < k_cardColumnCount; iColumn++)
	{
		CardColumn* pColumn = &m_cardColumns[iColumn];
		if(pColumn == portableStack.getPreviousColumn())
		{
			continue;
		}

		for(int iCorner = 0; iCorner < 4; iCorner++)
		{
			if(pColumn->isIntersects(stackCoords[iCorner][0], stackCoords[iCorner][1]))
			{
				if(columns.count(pColumn) == 0)
					columns.insert(pColumn);
				
				break;
			}//if(pColumn->isIntersects(stackCoords[iCorner][0], stackCoords[iCorner][1]))
		}//for(iCorner = 0; iCorner < 4; iCorner++)
	}//for(int iColumn = 0; iColumn < k_cardColumnCount; iColumn++)

	if(columns.size() == 0)
		return 0;
	
	CardColumn* dstColumn = 0;
	PlayingCard* srcBottomCard = portableStack.getCards().at(0);
	
	for(std::set<CardColumn*>::iterator it = columns.begin(); 
		it != columns.end(); ++it)
	{
		//пропускаем непустые столбцы (то есть имеющие по крайней мере стопку неоткрытых карт)
		//в случае если нижн€€ карта перемещаемой стопки - король любой масти
		if(srcBottomCard->getRank() == PlayingCard::k_rankKing && 
			!(*it)->isEmpty())
			continue;
		
		//искомый столбец пуст - на него можно переместить любую карту
		//столбец подходит
		if((*it)->isEmpty())
		{
			dstColumn = (*it);
			break;
		}
		
		PlayingCard* dstTopCard = (*it)->getTopCard();
		if(dstTopCard->getRank() == (srcBottomCard->getRank() + 1))
		{
			dstColumn = (*it);
			break;
		}
	}

	if(dstColumn != 0)
	{
		CardList& cards = portableStack.getCards();
		for(CardList::iterator it = cards.begin(); 
			it != cards.end(); ++it)
		{
			dstColumn->addCard(*it);			
		}

		
		m_scoreBoard.removeScores(SpiderGame::k_scoresMakeTurn);
		m_scoreBoard.addTurn();

		MakeTurnCommand* pCommand = new MakeTurnCommand(dstColumn, 
			portableStack.getPreviousColumn(), portableStack.getCards(), &m_scoreBoard);
				
		return pCommand;		
	}
	
	return 0;
}

/*********************************************************************
      MakeTurnCommand class implementation  
*********************************************************************/

MakeTurnCommand::MakeTurnCommand(CardColumn* currentColumn, 
								CardColumn* previousColumn, 
								const CardList& cards, 
								ScoreBoard* scoreBoard):
 m_currentColumn(currentColumn), 
 m_previousColumn(previousColumn), 
 m_scoreBoard(scoreBoard),
 m_playingCards(cards)
{
			
}
		
void MakeTurnCommand::undo()
{
	m_scoreBoard->addScores(SpiderGame::k_scoresMakeTurn);
	m_currentColumn->removeCards(m_playingCards);

	for(CardList::iterator it = m_playingCards.begin();
		it != m_playingCards.end(); ++it)
	{
		m_previousColumn->addCard(*it);
	}
}