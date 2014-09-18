
#include "domain.h"
using namespace Pegas;

/***********************************************************************
       SeriesPassAnimator class implementation
***********************************************************************/

void SeriesPassAnimator::beginAnimation(const CardList& cards)
{
	AnimationManager& animManager = AnimationManager::getInstance();
	AnimationParams params;
    CardAnimationTrek trek;

	m_trekMap.clear();
	m_cardMap.clear();

	params._delay = 0;
	params._duration = k_cardAnimationLength;
	params._loop = false;

	trek._leftFrom = m_cardDeck->getLeft();
	trek._topFrom = m_cardDeck->getTop();

	for(CardList::const_iterator it = cards.begin(); it != cards.end(); ++it)
	{
		PlayingCard* pCard = *it;
		trek._leftTo = pCard->getLeft();
		trek._topTo = pCard->getTop();

		pCard->setLeft(trek._leftFrom);
		pCard->setTop(trek._topFrom);
		pCard->setState(PlayingCard::k_stateInvisible);

		ANIMATIONID animID = animManager.registerAnimation(params, this);
		m_trekMap[animID] = trek;
		m_cardMap[animID] = pCard;

		params._delay+= k_cardAnimationDelayStep;
	}
}

void SeriesPassAnimator::animationStart(const AnimationParams& params)
{
	PlayingCard* pCard = m_cardMap[params._id];
	pCard->setState(PlayingCard::k_stateOpened);

	SoundManager& soundManager = SoundManager::getInstance();
	soundManager.playSound(k_soundCardPassing);
	
}

void SeriesPassAnimator::updateAnimation(const AnimationParams& params, 
										 MILLISECONDS elapsedTime)
{
	PlayingCard* pCard = m_cardMap[params._id];
	CardAnimationTrek& trek = m_trekMap[params._id];

	double k = (elapsedTime * 1.0)  / params._duration;
	CURCOORD left = trek._leftFrom + k * (trek._leftTo - trek._leftFrom);
	CURCOORD top = trek._topFrom + k * (trek._topTo - trek._topFrom);

	pCard->setLeft(left);
	pCard->setTop(top);
}

void SeriesPassAnimator::animationFinish(const AnimationParams& params)
{
	PlayingCard* pCard = m_cardMap[params._id];
	CardAnimationTrek& trek = m_trekMap[params._id];

	pCard->setLeft(trek._leftTo);
	pCard->setTop(trek._topTo);

	m_cardMap.erase(params._id);
	m_trekMap.erase(params._id);

	if(m_cardMap.empty())
	{
		m_cardDeck->decreaseCounter();
	}
}

/*******************************************************************
      ShowValidTurnAnimator class implementation 
********************************************************************/

void ShowValidTurnAnimator::beginAnimation(const CardList& srcCards, PlayingCard* dstCard)
{
	AnimationManager& animManager = AnimationManager::getInstance();
	AnimationParams params;

	m_cardMap.clear();
	
	params._delay = 0;
	params._duration = k_cardHighLightDuration;
	params._loop = false;

	ANIMATIONID animID;
	for(CardList::const_iterator it = srcCards.begin(); it != srcCards.end(); ++it)
	{
		animID = animManager.registerAnimation(params, this);
		m_cardMap[animID] = *it;
	}

	params._delay = k_cardHighLightDuration;
	
	animID = animManager.registerAnimation(params, this);
	m_cardMap[animID] = dstCard;
}


void ShowValidTurnAnimator::animationStart(const AnimationParams& params)
{
	PlayingCard* pCard = m_cardMap[params._id];
	
	pCard->setState(PlayingCard::k_stateNegative);
}

void ShowValidTurnAnimator::updateAnimation(const AnimationParams& params, 
											MILLISECONDS elapsedTime)
{
	//no implementation
}

void ShowValidTurnAnimator::animationFinish(const AnimationParams& params)
{
	PlayingCard* pCard = m_cardMap[params._id];
	
	pCard->setState(PlayingCard::k_stateOpened);
}

/*******************************************************************
      AddBuildedStackAnimator class implementation 
********************************************************************/

void AddBuildedStackAnimator::beginAnimation(CardStack& stack)
{
	m_stack = &stack;

	AnimationManager& animManager = AnimationManager::getInstance();
	CardStackIterator& cardIterator = stack.getCardIterator();
	AnimationParams params;
    CardAnimationTrek trek;

	m_trekMap.clear();
	m_cardMap.clear();

	params._delay = 0;
	params._duration = k_cardAnimationLength;
	params._loop = false;

	trek._leftTo = m_deck->getLeft() + 
		(m_deck->getStackCount() * BuildedStackDeck::k_relativeLeftOffset);
	
	trek._topTo = m_deck->getTop();

	while(!cardIterator.isLast())
	{
		PlayingCard* pCard = cardIterator.getCurrent();
		trek._leftFrom = pCard->getLeft();
		trek._topFrom = pCard->getTop();

		ANIMATIONID animID = animManager.registerAnimation(params, this);
		m_trekMap[animID] = trek;
		m_cardMap[animID] = pCard;

		params._delay+= k_cardAnimationDelayStep;

		cardIterator.moveNext();
	}
}

void AddBuildedStackAnimator::animationStart(const AnimationParams& params)
{
	//no implementation;
}

void AddBuildedStackAnimator::updateAnimation(const AnimationParams& params, MILLISECONDS elapsedTime)
{
	PlayingCard* pCard = m_cardMap[params._id];
	CardAnimationTrek& trek = m_trekMap[params._id];

	double k = (elapsedTime * 1.0)  / params._duration;
	CURCOORD left = trek._leftFrom + k * (trek._leftTo - trek._leftFrom);
	CURCOORD top = trek._topFrom + k * (trek._topTo - trek._topFrom);

	pCard->setLeft(left);
	pCard->setTop(top);
}

void AddBuildedStackAnimator::animationFinish(const AnimationParams& params)
{
	PlayingCard* pCard = m_cardMap[params._id];
	pCard->setState(PlayingCard::k_stateInvisible);

	m_cardMap.erase(params._id);
	m_trekMap.erase(params._id);

	if(m_cardMap.empty())
	{
		m_deck->addStack(m_stack);
		
		CardColumn& column = m_stack->getColumn();
		column.removeLastStack();
		if(column.getTopStack() == 0)
		{
			OpenClosedCardCommand* command = column.openClosedCard();
			delete command;
		}
	}
}

/*******************************************************************************
		YouWinAPrizeAnimator class implementation
*********************************************************************************/

#define VECTORTORGB(vec) (0xff000000 | (((unsigned char)(vec)._x) << 16) | (((unsigned char)(vec)._y) << 8) | ((unsigned char)(vec)._z))

YouWinAPrizeAnimator::YouWinAPrizeAnimator():
 m_gameBoard(0), m_isActive(false), m_startTime(0)
{
	m_textKeyColors[0] = Vector3(255.0, 0.0, 0.0);
	m_textKeyColors[1] = Vector3(255.0, 255.0, 0.0);
	m_textKeyColors[2] = Vector3(0.0, 255.0, 0.0);
	m_textKeyColors[3] = Vector3(0.0, 255.0, 255.0);
	m_textKeyColors[4] = Vector3(0.0, 0.0, 255.0);
	m_textKeyColors[5] = Vector3(255.0, 0.0, 255.0);
}		

void YouWinAPrizeAnimator::startAnimation()
{
	if(!m_gameBoard) return;

	m_startTime = OSUtils::getInstance().getCurrentTime();
	m_isActive = true;

	CURCOORD width, height;
	GrafManager& grafManager = GrafManager::getInstance();
	grafManager.getTextExtent(k_messageText, k_fontYouAreWinMessage, width, height);

	m_textParams._font = k_fontYouAreWinMessage;
	m_textParams._color = VECTORTORGB(m_textKeyColors[0]);
	m_textParams._left = (m_gameBoard->getWidth() - width) / 2;
	m_textParams._top = (m_gameBoard->getHeight() - height) / 2;
}

void YouWinAPrizeAnimator::stopAnimation()
{
	m_isActive = false;
}

void YouWinAPrizeAnimator::setGameBoard(GameBoard* gameBoard)
{
	m_gameBoard = gameBoard;	
}

void YouWinAPrizeAnimator::draw(GrafManager& context)
{
	if(!m_isActive || !m_gameBoard) return;

	MILLISECONDS currentTime = OSUtils::getInstance().getCurrentTime();
	MILLISECONDS ellapsedTime = currentTime - m_startTime;

	animateText(ellapsedTime, context);

	static const MILLISECONDS k_saluteCommonTime = k_saluteFaseLength_1 + k_saluteFaseLength_2;
	MILLISECONDS deltaTime = ellapsedTime % k_saluteCommonTime;

	if(deltaTime < k_saluteFaseLength_1)
	{
		animateSalute_Fase1(deltaTime, context);
	}else
	{
		animateSalute_Fase2(deltaTime - k_saluteFaseLength_1, context);
	}
}

String YouWinAPrizeAnimator::k_messageText = _text("Не надоело еще?");

void YouWinAPrizeAnimator::animateText(MILLISECONDS ellapsedTime, GrafManager& context)
{
	static double k_step = 1.0 / k_numColors;

	MILLISECONDS deltaTime = ellapsedTime % k_textColorCycle;
	double k = (deltaTime * 1.0) / k_textColorCycle;
	
	int i0 = floor(k / k_step);
	int i1 = i0 + 1;
	if(i1 >= k_numColors) i1 = 0;

	Vector3 color0 = m_textKeyColors[i0];
	Vector3 color1 = m_textKeyColors[i1];
	double k_local = k_step * i0;

	double t = (k - k_local) * k_numColors;
	Vector3 color = color0 + ((color1 - color0) * t);
	m_textParams._color = VECTORTORGB(color);
	
	context.drawText(k_messageText, m_textParams);	
}

void YouWinAPrizeAnimator::animateSalute_Fase1(MILLISECONDS deltaTime, GrafManager& context)
{

}

void YouWinAPrizeAnimator::animateSalute_Fase2(MILLISECONDS deltaTime, GrafManager& context)
{

}