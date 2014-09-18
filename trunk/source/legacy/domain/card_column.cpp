
#include "domain.h"
using namespace Pegas;


CardColumn::~CardColumn()
{
	setWidth(GameBoard::k_playingCardWidth);

	destroyCardStacks();
}

void CardColumn::destroyCardStacks()
{
	for(CardStackList::iterator it = m_cardStacks.begin();
		it != m_cardStacks.end(); ++it)
	{
		delete (*it);
	}
	
	m_cardStacks.clear();
}

CardStack* CardColumn::createNewStack()
{
	CardStack* stack = new CardStack(*this);
		
	CURCOORD topOffset;
	if(m_cardStacks.size() > 0)
	{
		CardStack* topStack = m_cardStacks.back();
		CURCOORD bottom = topStack->getTop() + topStack->getHeight();
		topOffset = bottom - GameBoard::k_playingCardHeight + k_openCardRelativeTopOffset; 
	}else
	{
		topOffset = GameBoard::k_borderOffset + m_closedCards.size() * k_closedCardRelativeTopOffset;
	}
	
	stack->setLeft(getLeft());
	stack->setTop(topOffset);
	m_cardStacks.push_back(stack);

	return stack;
}

void CardColumn::setLeft(CURCOORD left)
{
	GameObject::setLeft(left);

	alignCards();
}

void CardColumn::setTop(CURCOORD top)
{
	GameObject::setTop(top);

	alignCards();
}

void CardColumn::alignCards()
{
	CURCOORD left = getLeft();
	CURCOORD topOffset = getTop();

	for(ClosedCardList::iterator it = m_closedCards.begin();
		it != m_closedCards.end(); ++it)
	{
		(*it)->setLeft(left);
		(*it)->setTop(topOffset);

		topOffset+= k_closedCardRelativeTopOffset;
	}

	for(CardStackList::iterator it = m_cardStacks.begin();
		it != m_cardStacks.end(); ++it)
	{
		(*it)->setLeft(left);
		(*it)->setTop(topOffset);

		CURCOORD height = (*it)->getHeight() - GameBoard::k_playingCardHeight + k_openCardRelativeTopOffset;
		topOffset+= height;
	}

	recalkSize();
}

void CardColumn::draw(GrafManager& context)
{
	SpriteParameters params;
	params._left = getLeft();
	params._top = getTop();
	params._width = GameBoard::k_playingCardWidth;
	params._height = GameBoard::k_playingCardHeight;
	params._texture = k_textureColumnGag;

	context.drawSprite(params);

	for(ClosedCardList::iterator it = m_closedCards.begin();
		it != m_closedCards.end(); ++it)
	{
		(*it)->draw(context);
	}

	for(CardStackList::iterator it = m_cardStacks.begin();
		it != m_cardStacks.end(); ++it)
	{
		(*it)->draw(context);
	}
}

void CardColumn::initialize(const CardList& cards)
{
	destroyCardStacks();
	
	m_zIndex = 0;

	m_closedCards.clear();
	m_closedCards.insert(m_closedCards.end(), cards.begin(), cards.end());

	for(ClosedCardList::iterator it = m_closedCards.begin();
		it != m_closedCards.end(); ++it)
	{
		(*it)->setState(PlayingCard::k_stateClosed);
		(*it)->setZIndex(getNextZIndex());
	}

	alignCards();
}

void CardColumn::addCard(PlayingCard* card)
{
	CardStack* topStack = 0;
	PlayingCard* topCard = getTopCard();

	if((topCard != 0) && (card->getRank() != PlayingCard::k_rankKing))
	{
		if((topCard->getSuite() == card->getSuite()) && ((card->getRank() + 1) == topCard->getRank()))
		{
			topStack = m_cardStacks.back();			
		}else
		{
			topStack = createNewStack();
		}
	}else
	{
		topStack = createNewStack();
	}
	
	card->setZIndex(getNextZIndex());
	topStack->addCard(card);

	recalkSize();
}

CardStack* CardColumn::getTopStack()
{
	if(m_cardStacks.size() == 0)
	{
		return 0;
	}

	return m_cardStacks.back();
}

PlayingCard* CardColumn::getTopCard()
{
	CardStack* topStack = getTopStack();
	
	if(topStack != 0)
	{
		return topStack->getTopCard();
	}

	return 0;	
}

bool CardColumn::isEmpty()
{
	return ((m_closedCards.size() == 0) && (m_cardStacks.size() == 0));
}

bool CardColumn::isTopStack(const CardStack* stack)
{
	if(m_cardStacks.size() == 0)
	{
		return false;
	}

	CardStack* topStack = m_cardStacks.back();

	return (stack == topStack);
}

bool CardColumn::isIntersects(CURCOORD x, CURCOORD y)
{
	CURCOORD width = isEmpty() ? GameBoard::k_playingCardWidth : getWidth();
	CURCOORD height = isEmpty() ? GameBoard::k_playingCardHeight : getHeight();

	if(x < getLeft() || x > (getLeft() + width))
		return false;

	if(y < getTop() || y > (getTop() + height))
		return false;

	return true;
}

void CardColumn::removeLastStack()
{
	if(m_cardStacks.size() == 0)
	{
		return;
	}

	CardStack* topStack = m_cardStacks.back();
	m_cardStacks.pop_back();

	delete topStack;

	recalkSize();
}

void CardColumn::recalkSize()
{
	setWidth(GameBoard::k_playingCardWidth);

	CURCOORD top = getTop();
	CURCOORD height = 0;
	
	CardStack* topStack = getTopStack();
	if(topStack != 0)
	{
		height = topStack->getTop() + topStack->getHeight() - top;	
	}else
	{
		if(m_closedCards.size() > 0)
		{
			height = GameBoard::k_playingCardHeight + (m_closedCards.size() - 1) * k_closedCardRelativeTopOffset; 
		}
	}

	setHeight(height);
}

OpenClosedCardCommand* CardColumn::openClosedCard()
{
	if(m_cardStacks.size() == 0 && m_closedCards.size() > 0)
	{
		PlayingCard* card = m_closedCards.back();
		m_closedCards.pop_back();

		addCard(card);
	}

	OpenClosedCardCommand* command = new OpenClosedCardCommand(this);

	return command;
}

void CardColumn::undoOpenClosedCard()
{
	if(m_cardStacks.size() != 1)
		return;
	
	CardStack* topStack = getTopStack();
	if(topStack->getNumCard() != 1) 
		return;

	PlayingCard* card = topStack->getTopCard();
	removeLastStack();

	card->setState(PlayingCard::k_stateClosed);
	card->setZIndex(getNextZIndex());
	card->setLeft(getLeft());
	card->setTop(getTop() + m_closedCards.size() * k_closedCardRelativeTopOffset);
	
	m_closedCards.push_back(card);
}

void CardColumn::removeCards(const CardList& cards)
{
	if(m_cardStacks.size() == 0)
		return;

	PlayingCard* card = cards.front();
	CardStackList::iterator it_from = m_cardStacks.end();
	for(CardStackList::iterator it = m_cardStacks.begin(); it != m_cardStacks.end(); ++it)
	{
		if((*it)->hasCard(card))
		{
			(*it)->removeCardsBeginingOne(card);
			if((*it)->getNumCard() == 0)
			{
				it_from = it;
			}
			else
			{
				it_from = it;
				++it_from;
			}
			
			break;
		}
	}//for(CardStackList::iterator it = m_cardStacks.begin(); it != m_cardStacks.end(); ++it)

	for(CardStackList::iterator it = it_from; it != m_cardStacks.end(); ++it)
	{
		delete (*it);
	}
	
	m_cardStacks.erase(it_from, m_cardStacks.end());
	
	recalkSize();
}

/*******************************************************************************
		OpenClosedCardCommand class implementation	
*******************************************************************************/

void OpenClosedCardCommand::undo()
{
	m_ñolumn->undoOpenClosedCard();
}