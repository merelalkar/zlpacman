#ifndef PEGAS_FAKESPIDER_GAME_ANIMATIONS_H
#define PEGAS_FAKESPIDER_GAME_ANIMATIONS_H
#pragma once

namespace Pegas
{
	struct CardAnimationTrek 
	{
			CURCOORD _leftFrom;
			CURCOORD _topFrom;
			CURCOORD _leftTo;
			CURCOORD _topTo;
	};

	typedef std::map<ANIMATIONID, CardAnimationTrek> CardAnimationTrekMap;
	typedef std::map<ANIMATIONID, PlayingCard*> PlayingCardMap;

	class SeriesPassAnimator: public IAnimationEventListener
	{
	public:
		enum
		{
			k_cardAnimationLength = 100,
			k_cardAnimationDelayStep = 120
		};

		SeriesPassAnimator(): m_cardDeck(0) {};
		virtual ~SeriesPassAnimator() {};

		void beginAnimation(const CardList& cards);
		void setCardDeck(CardDeck* cardDeck) { m_cardDeck = cardDeck; };

		virtual void animationStart(const AnimationParams& params);
		virtual void updateAnimation(const AnimationParams& params, MILLISECONDS elapsedTime);
		virtual void animationFinish(const AnimationParams& params);

	private:
		CardDeck*				m_cardDeck;		
		CardAnimationTrekMap	m_trekMap;
		PlayingCardMap			m_cardMap;
	
	};

	class ShowValidTurnAnimator: public IAnimationEventListener
	{
	public:
		enum
		{
			k_cardHighLightDuration = 300 
		};

		ShowValidTurnAnimator() {};
		virtual ~ShowValidTurnAnimator() {};

		void beginAnimation(const CardList& srcCards, PlayingCard* dstCard); 
		
		virtual void animationStart(const AnimationParams& params);
		virtual void updateAnimation(const AnimationParams& params, MILLISECONDS elapsedTime);
		virtual void animationFinish(const AnimationParams& params);
	private:		
		PlayingCardMap		m_cardMap;
	};

	class AddBuildedStackAnimator: public IAnimationEventListener
	{
	public:
		enum
		{
			k_cardAnimationLength = 200,
			k_cardAnimationDelayStep = 100
		};

		AddBuildedStackAnimator(): m_deck(0), m_stack(0) {};
		virtual ~AddBuildedStackAnimator() {};

		void beginAnimation(CardStack& stack);
		void setBuildedStackDeck(BuildedStackDeck* deck) { m_deck = deck; };

		virtual void animationStart(const AnimationParams& params);
		virtual void updateAnimation(const AnimationParams& params, MILLISECONDS elapsedTime);
		virtual void animationFinish(const AnimationParams& params);

	private:
		BuildedStackDeck* m_deck;
		CardStack* m_stack;
		CardAnimationTrekMap	m_trekMap;
		PlayingCardMap			m_cardMap;
	};

	class YouWinAPrizeAnimator: public IDrawable
	{
	public:

		enum
		{
			k_numColors = 6,
			k_textColorCycle = 12000,
			k_saluteFaseLength_1 = 2000,
			k_saluteFaseLength_2 = 4000,
			k_numCircles = 20
		};
		
		static String k_messageText;

		YouWinAPrizeAnimator();
		virtual ~YouWinAPrizeAnimator() {};

		void startAnimation();
		void stopAnimation();
		void setGameBoard(GameBoard* gameBoard);

		virtual void draw(GrafManager& context);
		void animateText(MILLISECONDS ellapsedTime, GrafManager& context);
		void animateSalute_Fase1(MILLISECONDS deltaTime, GrafManager& context);
		void animateSalute_Fase2(MILLISECONDS deltaTime, GrafManager& context);

	private:
		GameBoard* m_gameBoard;
		bool m_isActive;
		MILLISECONDS m_startTime;
		TextParameters m_textParams;
		Vector3 m_textKeyColors[k_numColors];
	};
}

#endif //PEGAS_FAKESPIDER_GAME_ANIMATIONS_H