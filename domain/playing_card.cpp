
#include "domain.h"
using namespace Pegas;

#define MAKE_CARD_TEXTURE_ID(suite, rank) ((suite << 8)|(rank))

PlayingCard::CardTextureMap PlayingCard::s_textureIDMap;

void PlayingCard::buildTextureMap()
{
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rankAce)] = k_textureDeamondAce;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_2)] = k_textureDeamond_2;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_3)] = k_textureDeamond_3;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_4)] = k_textureDeamond_4;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_5)] = k_textureDeamond_5;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_6)] = k_textureDeamond_6;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_7)] = k_textureDeamond_7;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_8)] = k_textureDeamond_8;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_9)] = k_textureDeamond_9;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rank_10)] = k_textureDeamond_10;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rankJack)] = k_textureDeamondJack;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rankQueen)] = k_textureDeamondQueen;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteDeamond, k_rankKing)] = k_textureDeamondKing;

	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rankAce)] = k_textureClubAce;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_2)] = k_textureClub_2;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_3)] = k_textureClub_3;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_4)] = k_textureClub_4;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_5)] = k_textureClub_5;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_6)] = k_textureClub_6;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_7)] = k_textureClub_7;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_8)] = k_textureClub_8;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_9)] = k_textureClub_9;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rank_10)] = k_textureClub_10;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rankJack)] = k_textureClubJack;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rankQueen)] = k_textureClubQueen;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteClub, k_rankKing)] = k_textureClubKing;

	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rankAce)] = k_textureHeartAce;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_2)] = k_textureHeart_2;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_3)] = k_textureHeart_3;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_4)] = k_textureHeart_4;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_5)] = k_textureHeart_5;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_6)] = k_textureHeart_6;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_7)] = k_textureHeart_7;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_8)] = k_textureHeart_8;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_9)] = k_textureHeart_9;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rank_10)] = k_textureHeart_10;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rankJack)] = k_textureHeartJack;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rankQueen)] = k_textureHeartQueen;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteHeart, k_rankKing)] = k_textureHeartKing;

	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rankAce)] = k_textureSpadeAce;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_2)] = k_textureSpade_2;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_3)] = k_textureSpade_3;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_4)] = k_textureSpade_4;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_5)] = k_textureSpade_5;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_6)] = k_textureSpade_6;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_7)] = k_textureSpade_7;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_8)] = k_textureSpade_8;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_9)] = k_textureSpade_9;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rank_10)] = k_textureSpade_10;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rankJack)] = k_textureSpadeJack;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rankQueen)] = k_textureSpadeQueen;
	s_textureIDMap[MAKE_CARD_TEXTURE_ID(k_suiteSpade, k_rankKing)] = k_textureSpadeKing;
	
}

RESOURCEID PlayingCard::getTexture(const PlayingCard& card)
{
	CardSuite suite = card.getSuite();
	CardRank rank = card.getRank();

	return s_textureIDMap[MAKE_CARD_TEXTURE_ID(suite, rank)];
}

PlayingCard::PlayingCard(): m_state(k_stateInvisible), m_pOwner(NULL)
{
	setWidth(GameBoard::k_playingCardWidth);
	setHeight(GameBoard::k_playingCardHeight);

	m_cardSpriteParams._width = GameBoard::k_playingCardWidth;
	m_cardSpriteParams._height = GameBoard::k_playingCardHeight;

	if(s_textureIDMap.size() == 0)
	{
		buildTextureMap();
	}	
}

void PlayingCard::setState(CardState state)
{
	m_state = state;

	m_cardSpriteParams._flags = 0;
	switch(state)
	{
	case k_stateOpened:
	case k_stateNegative:
		
		m_cardSpriteParams._texture = getTexture(*this);
		if(state == k_stateNegative)
		{
			m_cardSpriteParams._flags = k_invertTextureColors;			
		}
		break;
	case k_stateClosed:
		
		m_cardSpriteParams._texture = k_textureCardBack;
		break;
	}	
}

void PlayingCard::setSuite(CardSuite suite)
{
	m_suite = suite;
	
	CardState state;
	if((state == k_stateOpened) || (state == k_stateNegative))
	{
		m_cardSpriteParams._texture = getTexture(*this);
	}
}

void PlayingCard::setRank(CardRank rank)
{
	m_rank = rank;

	CardState state;
	if((state == k_stateOpened) || (state == k_stateNegative))
	{
		m_cardSpriteParams._texture = getTexture(*this);
	}
}

void PlayingCard::draw(GrafManager& context)
{
	CardState currentState = getState();

	if(currentState == k_stateInvisible)
	{
		return;
	}

	m_cardSpriteParams._left = getLeft();
	m_cardSpriteParams._top = getTop();
	
	context.drawSprite(m_cardSpriteParams);
}

void PlayingCard::click(CURCOORD x, CURCOORD y)
{
	if(m_pOwner == NULL)
		return;

	if(getState() != k_stateOpened)
		return;

	SpiderGame& gameHost = SpiderGame::getInstance();
	SpiderGame::GameState gameState = gameHost.getGameState();

	if(gameState != SpiderGame::k_gameStateUserTurnWaiting)
		return;

	m_pOwner->takeStackByCard(this);
}