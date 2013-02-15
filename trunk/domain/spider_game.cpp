
#include "domain.h"
using namespace Pegas;

SpiderGame::SpiderGame(): 
	Singleton(*this), 
	m_remainingSeries(k_seriesToPass),
	m_difficultyLevel(k_difficultyLevelEasy), 
	m_currentGameState(k_gameStateUnknown) 
{
	m_userWinAnimator.setGameBoard(&m_gameBoard);
	GrafManager::getInstance().addLayout(&m_userWinAnimator);
}

SpiderGame::~SpiderGame()
{

}

void SpiderGame::setGameState(GameState state)
{
	m_currentGameState = state;	
}

SpiderGame::GameState SpiderGame::getGameState()
{
	return m_currentGameState;
}


SpiderGame::DifficultyLevel SpiderGame::getDifficutlyLevel()
{
	return m_difficultyLevel;
}

int SpiderGame::getRemainingSeries()
{
	return m_remainingSeries;
}

void SpiderGame::addCommand(GameCommand* command)
{
	if(m_commandQueue.size() == k_maxCommandsInQueue)
	{
		GameCommand* oldCommand = m_commandQueue.top();
		delete oldCommand;
		m_commandQueue.pop();		
	}

	m_commandQueue.push(command);
}

bool SpiderGame::hasCommands()
{
	return (!m_commandQueue.empty());
}

void SpiderGame::clearCommandQueue()
{
	while(m_commandQueue.size() > 0)
	{
		GameCommand* oldCommand = m_commandQueue.top();
		delete oldCommand;
		m_commandQueue.pop();
	}
}

void SpiderGame::beginNewGame(DifficultyLevel level)
{
	
	m_difficultyLevel = level;
	m_remainingSeries = k_seriesToPass;
	clearCommandQueue();
	
	PlayingCard::CardRank	ranks[k_numCards];
	PlayingCard::CardSuite	suites[k_numCards];
	
	std::vector<int> vIndexes;
	std::vector<PlayingCard::CardSuite> vSuites;

	vIndexes.reserve(k_numCards);
	vSuites.reserve(k_numSuits);
    
	vSuites.push_back(PlayingCard::k_suiteSpade);
    
	if(level == k_difficultyLevelMedium)
	{
		vSuites.push_back(PlayingCard::k_suiteHeart);		
	}

	if(level == k_difficultyLevelHeavy)
	{
		vSuites.push_back(PlayingCard::k_suiteHeart);
		vSuites.push_back(PlayingCard::k_suiteClub);
		vSuites.push_back(PlayingCard::k_suiteDeamond);
	}
	
	int numSuits = vSuites.size();
	int numDecs = (int)(k_numStacks / numSuits);
    int numCardsInDeck = (int)(k_numCards / numDecs); 

	for(int iDeck = 0; iDeck < numDecs; iDeck++)
	{
		for(int iSuite = 0; iSuite < numSuits; iSuite++)
		{
			PlayingCard::CardSuite suite = vSuites[iSuite];

			for(int iRank = 1; iRank <= PlayingCard::CardRank::k_rankKing; iRank++)
			{
				int iIndex = iDeck * numCardsInDeck	+ iSuite * k_cardsInSuite + (iRank - 1);

				ranks[iIndex] = (PlayingCard::CardRank)iRank;
				suites[iIndex] = suite;

				vIndexes.push_back(iIndex);
			}//for(int iRank = 1; iRank <= PlayingCard::CardRank::k_rankKing; iRank++)
		}//for(int iSuite = 0; iSuite < numSuits; iSuite++)
	}//for(int iDeck = 0; iDeck < numDecs; numDecs++)

	random_shuffle(vIndexes.begin(), vIndexes.end());
    
	CardList initialCards;
	initialCards.reserve(k_cardsInColumns);

	for(int iCard = 0; iCard < k_numCards; iCard++)
	{
		int iIndex = vIndexes[iCard];

		m_playingCards[iCard].setZIndex(0);
		m_playingCards[iCard].setOwner(0);
		m_playingCards[iCard].setSuite(suites[iIndex]);
		m_playingCards[iCard].setRank(ranks[iIndex]);

		if(iCard < k_cardsInColumns)
		{
			initialCards.push_back(&m_playingCards[iCard]);
		}
	}

	m_gameBoard.initialize(initialCards);
	setGameState(k_gameStateUserTurnWaiting);

	passNewSeries();
	
	//m_userWinAnimator.startAnimation();
}

void SpiderGame::restartCurrentGame()
{
	m_remainingSeries = k_seriesToPass;
	clearCommandQueue();

	CardList initialCards;
	initialCards.reserve(k_cardsInColumns);

	for(int iCard = 0; iCard < k_numCards; iCard++)
	{
		m_playingCards[iCard].setZIndex(0);
		m_playingCards[iCard].setOwner(0);
		
		if(iCard < k_cardsInColumns)
		{
			initialCards.push_back(&m_playingCards[iCard]);
		}
	}

	m_gameBoard.initialize(initialCards);
	setGameState(k_gameStateUserTurnWaiting);

	passNewSeries();
}

void SpiderGame::undoLastTurn()
{
	if(hasCommands())
	{
		GameCommand* command = m_commandQueue.top();
		command->undo();
		delete command;

		m_commandQueue.pop();
	}
}

void SpiderGame::passNewSeries()
{
	if(m_gameBoard.hasEmptyColumns())
	{
		OSUtils& utils = OSUtils::getInstance();
		utils.showMessage(k_stringSeriesPassImposible, k_stringGameWindowCaption);

		return;
	}

	if(m_remainingSeries == 0)
	{
		return;
	}

	clearCommandQueue();
	
	int iBaseOffset = k_cardsInColumns + (k_seriesToPass - m_remainingSeries) * k_cardsInSeries;

	CardList cardSeries;
	cardSeries.reserve(k_cardsInSeries);

	for(int iCard = 0; iCard < k_cardsInSeries; iCard++)
	{
		cardSeries.push_back(&m_playingCards[iBaseOffset + iCard]);
	}
	
	m_gameBoard.passNewSeries(cardSeries);
	
	m_remainingSeries--;
}

void SpiderGame::showValidTurn()
{
	m_gameBoard.showValidTurn();
}

void SpiderGame::resizeGameBoard(CURCOORD newWidth, CURCOORD newHeight)
{
	m_gameBoard.resize(newWidth, newHeight);
}

void SpiderGame::completeGameByUserWin()
{
   setGameState(k_gameStateUserWin);

   SoundManager::getInstance().playSound(k_soundYouAreWin);

   m_userWinAnimator.startAnimation();
}

bool SpiderGame::playerAlreadyHasMadeTurn()
{
	return false;
}