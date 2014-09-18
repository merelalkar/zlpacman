
#include "domain.h"
using namespace Pegas;

CardStack::CardStack(CardColumn& owner): m_owner(owner)
{
	m_cardIterator = new CardStackIterator();
}

CardStack::~CardStack() 
{
	delete m_cardIterator;
}

void CardStack::takeStackByCard(PlayingCard* pTopCard)
{
	if(!m_owner.isTopStack(this))
		return;
	
	PlayingCardsStack::iterator from_it = std::find(m_cardStack.begin(), 
		m_cardStack.end(), pTopCard);
	
	if(from_it == m_cardStack.end())
		return;

	PortableCardStack& portCS = PortableCardStack::getInstance();
	portCS.reset(&m_owner);

	for(PlayingCardsStack::iterator it = from_it; it != m_cardStack.end(); ++it)
	{
		(*it)->setOwner(0);
		portCS.addCard(*it);
	}
	
	m_cardStack.erase(from_it, m_cardStack.end());
	recalkSize();
	portCS.capture();

	if(m_cardStack.empty())
	{
		m_owner.removeLastStack();
	}
}

void CardStack::addCard(PlayingCard* pCard)
{
	CURCOORD left = getLeft();
	CURCOORD top = getTop();
	CURCOORD height = getHeight();
	
	if(!m_cardStack.empty())
	{
		PlayingCard* topCard = m_cardStack.back();
		top = topCard->getTop() + CardColumn::k_openCardRelativeTopOffset;
		height+= CardColumn::k_openCardRelativeTopOffset;
	}else
	{
		height = GameBoard::k_playingCardHeight;
	}
	setWidth(GameBoard::k_playingCardWidth);
	setHeight(height);
	
	pCard->setLeft(left);
	pCard->setTop(top);
	pCard->setState(PlayingCard::k_stateOpened);
	pCard->setOwner(this);

	m_cardStack.push_back(pCard);

	if(m_cardStack.size() == SpiderGame::k_cardsInSuite)
	{
		SpiderGame& gameHost = SpiderGame::getInstance();
		GameBoard& gameBoard = gameHost.getGameBoard();

		gameBoard.addBuildedCardStack(*this);
	}
}

void CardStack::draw(GrafManager& context)
{
	for(PlayingCardsStack::iterator it = m_cardStack.begin();
		it != m_cardStack.end(); ++it)
	{
		(*it)->draw(context);
	}
}

PlayingCard* CardStack::getTopCard()
{
	if(m_cardStack.empty())
	{
		return 0;
	}

	return m_cardStack.back();
}

PlayingCard* CardStack::getBottomCard()
{
	if(m_cardStack.empty())
	{
		return 0;
	}

	return m_cardStack.front();
}

void CardStack::setLeft(CURCOORD left)
{
	GameObject::setLeft(left);

	alignCards();
}

void CardStack::setTop(CURCOORD top)
{
	GameObject::setTop(top);

	alignCards();
}

void CardStack::alignCards()
{
	CURCOORD left = getLeft();
	CURCOORD top = getTop();

	for(PlayingCardsStack::iterator it = m_cardStack.begin();
		it != m_cardStack.end(); ++it)
	{
		(*it)->setLeft(left);
		(*it)->setTop(top);

		top+= CardColumn::k_openCardRelativeTopOffset;
	}
}

void CardStack::recalkSize()
{
	int numCards = m_cardStack.size();
	if(numCards == 0)
	{
		setHeight(0);
	}

	CURCOORD height = GameBoard::k_playingCardHeight + 
		CardColumn::k_openCardRelativeTopOffset * (numCards - 1);

	setHeight(height);
	setWidth(GameBoard::k_playingCardWidth);
}

CardStackIterator& CardStack::getCardIterator()
{
	m_cardIterator->reset(m_cardStack.rbegin(), m_cardStack.rend());

	return (*m_cardIterator);
}

bool CardStack::hasCard(PlayingCard* card)
{
	PlayingCardsStack::iterator it = std::find(m_cardStack.begin(), m_cardStack.end(), card);

	return (it != m_cardStack.end());
}

void CardStack::removeCardsBeginingOne(PlayingCard* card)
{
	PlayingCardsStack::iterator it = std::find(m_cardStack.begin(), m_cardStack.end(), card);
	
	if(it != m_cardStack.end())
	{
		m_cardStack.erase(it, m_cardStack.end());
	}

	if(m_cardStack.size() != 0)
	{
		recalkSize();
	}
}

/********************************************************************
     CardStackIterator class implementation 
********************************************************************/

void CardStackIterator::reset(const InnerIterator& fisrt_it, const InnerIterator& last_it)
{
	m_current = m_first = fisrt_it;
	m_last = last_it;
}

void CardStackIterator::moveNext()
{
	++m_current;
}

bool CardStackIterator::isLast()
{
	return m_current == m_last;
}

PlayingCard* CardStackIterator::getCurrent()
{
	return *m_current;
}

void CardStackIterator::moveTo(PlayingCard* card)
{
	while(m_current != m_last)
	{
		if(*m_current == card)
			return;

		++m_current;
	}	
}