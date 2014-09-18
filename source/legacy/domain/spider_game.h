#ifndef PEGAS_FAKESPIDER_SPIDER_GAME_H
#define PEGAS_FAKESPIDER_SPIDER_GAME_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{
	class SpiderGame: public Singleton<SpiderGame>
	{
		typedef std::stack<GameCommand*> GameCommandQueue;

	public:
		enum
		{
			k_numCards = 104,
			k_numSuits = 4,
			k_cardsInSuite = 13,
			k_numStacks = 8, // = k_numCards / k_cardsInSuite
			k_cardsInColumns = 44,
			k_cardsInSeries = 10,
			k_seriesToPass = 6,
			k_maxCommandsInQueue = 1
		};
		
		enum DifficultyLevel
		{
			k_difficultyLevelEasy = 1,
			k_difficultyLevelMedium = 2,
			k_difficultyLevelHeavy = 4			
		};

		enum Scores
		{
			k_scoresBuildStack = 100,
			k_scoresMakeTurn = 1
		};

		//состо€ни€ игры
		enum GameState
		{
			k_gameStateUnknown,
			//ожидание хода игрока
			k_gameStateUserTurnWaiting,
			//игрок перемещает стопку кард из одного столбца в другой
			k_gameStateMovingCardStack,
			//проигрывание любой анимации (раздача нового р€да карт, отправка 
			//полной собранной стопки в "стойло", и т.д.
			k_gameStatePlayingAnimation,
			//пользователь выиграл, проигрываем дурацкий салютик
			//при первом клике предлагаем сыграть новую игру
			k_gameStateUserWin
		};

		SpiderGame();
		virtual ~SpiderGame();

        void setGameState(GameState state);
		GameState getGameState();
		bool playerAlreadyHasMadeTurn(); //Ќјѕ»—ј“№ –≈јЋ»«ј÷»ё ћ≈“ќƒј

		DifficultyLevel getDifficutlyLevel();
		int getRemainingSeries();

		void addCommand(GameCommand* command);
		bool hasCommands();

		void beginNewGame(DifficultyLevel level);
		void restartCurrentGame();
		void undoLastTurn();
		void passNewSeries();
		void showValidTurn();
		void completeGameByUserWin(); //Ќјѕ»—ј“№ –≈јЋ»«ј÷»ё ћ≈“ќƒј

		void resizeGameBoard(CURCOORD newWidth, CURCOORD newHeight);
		GameBoard& getGameBoard() { return m_gameBoard; };

	private:
		void clearCommandQueue();
				
		GameBoard        m_gameBoard;
		PlayingCard      m_playingCards[k_numCards];
        DifficultyLevel  m_difficultyLevel;
		GameState        m_currentGameState;

		int m_remainingSeries;

		GameCommandQueue m_commandQueue;
		YouWinAPrizeAnimator m_userWinAnimator;

		SpiderGame(const SpiderGame& src);
		SpiderGame& operator=(const SpiderGame& src);
	};
}

#endif //PEGAS_FAKESPIDER_SPIDER_GAME_H