
#include "domain.h"
using namespace Pegas;

void CardDeck::initialize(int deckCount)
{
	m_deckCounter = deckCount;

	setPlacement();
}

void CardDeck::decreaseCounter()
{
	if(m_deckCounter == 0)
		return;

	m_deckCounter--;

	setPlacement();
}

void CardDeck::setPlacement()
{
	CURCOORD width = 0;
	if(m_deckCounter > 0)
	{
		width = GameBoard::k_playingCardWidth + k_leftRelativeOffset * (m_deckCounter - 1);
	}
	CURCOORD left = m_gameBoard.getWidth() - GameBoard::k_borderOffset - width;
	CURCOORD top =  m_gameBoard.getHeight() - GameBoard::k_borderOffset - GameBoard::k_playingCardHeight;

	setLeft(left);
	setTop(top);
	setWidth(width);
	setHeight(GameBoard::k_playingCardHeight);
}

void CardDeck::draw(GrafManager& context)
{
	SpriteParameters params;
	params._left = m_gameBoard.getWidth() - GameBoard::k_borderOffset - GameBoard::k_playingCardWidth;
	params._top = getTop();
	params._width = GameBoard::k_playingCardWidth;
	params._height = GameBoard::k_playingCardHeight;
	params._texture = k_textureCardBack;
	params._flags = 0;

	for(int i = 0; i < m_deckCounter; i++)
	{
		context.drawSprite(params);

		params._left-= k_leftRelativeOffset;
	}
}

void CardDeck::click(CURCOORD x, CURCOORD y)
{
	SpiderGame& gameHost = SpiderGame::getInstance();
	gameHost.passNewSeries();
}