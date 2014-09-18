
#include "domain.h"
using namespace Pegas;

/*********************************************************
		PortableCardStack class implementation
*********************************************************/

PortableCardStack::PortableCardStack(GameBoard& gameBoard):
	Singleton(*this), m_previousColumn(0), m_gameBoard(gameBoard)
{
	m_cardList.reserve(SpiderGame::k_cardsInSuite - 1);

	setWidth(GameBoard::k_playingCardWidth);
	setHeight(0);
}	

void PortableCardStack::reset(CardColumn* previousColumn)
{
	m_cardList.clear();
	m_previousColumn = previousColumn;
}

void PortableCardStack::addCard(PlayingCard* card)
{
	if(m_cardList.empty())
	{
		setLeft(card->getLeft());
		setTop(card->getTop());
	}

	m_cardList.push_back(card);

	CURCOORD height = GameBoard::k_playingCardHeight + 
		CardColumn::k_openCardRelativeTopOffset * (m_cardList.size() - 1);
	setHeight(height);
}

void PortableCardStack::draw(GrafManager& context)
{
	for(CardList::iterator it = m_cardList.begin();
		it != m_cardList.end(); ++it)
	{
		(*it)->draw(context);
	}
}
		
void PortableCardStack::capture()
{
	GUIManager& guiManager = GUIManager::getInstance();
	guiManager.addMovableObject(this);

	SpiderGame& gameHost = SpiderGame::getInstance();
	gameHost.setGameState(SpiderGame::k_gameStateMovingCardStack);

	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.playSound(k_soundTakeCard);
}

void PortableCardStack::move(CURCOORD cx, CURCOORD cy)
{
	CURCOORD left = getLeft() + cx;
	CURCOORD top = getTop() + cy;
	setLeft(left);
	setTop(top);

	alignCards();
}

void PortableCardStack::release(CURCOORD x, CURCOORD y)
{
	setLeft(x);
	setTop(y);

	alignCards();


	MakeTurnCommand* makeTurnCommand = m_gameBoard.putCardsToColumn(*this);
	if(makeTurnCommand != 0)
	{
		ComplexGameCommand* fullCommand = new ComplexGameCommand();
		if(m_previousColumn->getTopStack() == 0)
		{
			OpenClosedCardCommand* openCloseCardCommand =  m_previousColumn->openClosedCard();
			fullCommand->addCommand(openCloseCardCommand);
		}
		fullCommand->addCommand(makeTurnCommand);
		SpiderGame::getInstance().addCommand(fullCommand);
	}else
	{
		for(CardList::iterator it = m_cardList.begin();
			it != m_cardList.end(); ++it)
		{
			m_previousColumn->addCard(*it);
		}
	}
	m_cardList.clear();
	m_previousColumn = 0;

	SpiderGame& gameHost = SpiderGame::getInstance();
	gameHost.setGameState(SpiderGame::k_gameStateUserTurnWaiting);

	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.playSound(k_soundReleaseCard);
}

void PortableCardStack::alignCards()
{
	CURCOORD left = getLeft();
	CURCOORD top = getTop();

	for(CardList::iterator it = m_cardList.begin();
		it != m_cardList.end(); ++it)
	{
		(*it)->setLeft(left);
		(*it)->setTop(top);

		top+= CardColumn::k_openCardRelativeTopOffset;
	}
}