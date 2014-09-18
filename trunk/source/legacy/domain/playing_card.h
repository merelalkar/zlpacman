#ifndef PEGAS_FAKESPIDER_PLAYING_CARD_H
#define PEGAS_FAKESPIDER_PLAYING_CARD_H 
#pragma once

namespace Pegas
{
	class PlayingCard: public ClickableGameObject
	{
	public:
		enum CardState
		{
			k_stateInvisible,
			k_stateClosed,
			k_stateOpened,
			k_stateNegative
		};

		enum CardRank
		{
			k_rankAce = 1,
			k_rank_2,
			k_rank_3,
			k_rank_4,
			k_rank_5,
			k_rank_6,
			k_rank_7,
			k_rank_8,
			k_rank_9,
			k_rank_10,
			k_rankJack,
			k_rankQueen,
			k_rankKing
		};

		enum CardSuite
		{
			k_suiteDeamond = 1,
			k_suiteClub,
			k_suiteHeart,
			k_suiteSpade
		};

		typedef std::map<int, RESOURCEID> CardTextureMap;

		
		PlayingCard();
		virtual ~PlayingCard() {};

		virtual void draw(GrafManager& context);
		virtual void click(CURCOORD x, CURCOORD y);
		
		void setOwner(CardStack* pOwner) { m_pOwner = pOwner; };
		void setState(CardState state);
		void setSuite(CardSuite suite);
		void setRank(CardRank rank);

		CardStack* getOwner() const { return m_pOwner; };
		CardState getState() const { return m_state; };
		CardSuite getSuite() const { return m_suite; };
		CardRank getRank() const { return m_rank; };

		static RESOURCEID getTexture(const PlayingCard& card);
	private:

		CardState m_state;
		CardSuite m_suite;
		CardRank  m_rank;

		CardStack* m_pOwner;

		SpriteParameters m_cardSpriteParams;

		static void buildTextureMap();
		static CardTextureMap s_textureIDMap;

		PlayingCard(const PlayingCard& src);
		PlayingCard& operator=(const PlayingCard& src);
	};
}
#endif //PEGAS_FAKESPIDER_PLAYING_CARD_H