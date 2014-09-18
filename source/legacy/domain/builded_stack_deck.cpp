
#include "domain.h"
using namespace Pegas;

void BuildedStackDeck::draw(GrafManager& context)
{
	if(m_bottomCards.size() == 0)
		return;

	SpriteParameters params;
	params._width = GameBoard::k_playingCardWidth;
	params._height = GameBoard::k_playingCardHeight;
	params._top = getTop();
	params._left = getLeft();
	
	for(BottomStackCardList::iterator it = m_bottomCards.begin();
		it != m_bottomCards.end(); ++it)
	{
		params._texture = *it;
		context.drawSprite(params);

		params._left+= k_relativeLeftOffset;
	}
}

void BuildedStackDeck::reset()
{
	m_bottomCards.clear();
}

void BuildedStackDeck::addStack(CardStack* pStack)
{
	PlayingCard* pCard = pStack->getBottomCard();
	RESOURCEID texture = PlayingCard::getTexture(*pCard);

	m_bottomCards.push_back(texture);

	if(m_bottomCards.size() == SpiderGame::k_numStacks)
	{
		SpiderGame& gameHost = SpiderGame::getInstance();
		gameHost.completeGameByUserWin();
	}
}

CURCOORD BuildedStackDeck::getWidth()
{
	return m_bottomCards.size() * GameBoard::k_playingCardWidth;
}

CURCOORD BuildedStackDeck::getHeight()
{
	return GameBoard::k_playingCardHeight;
}