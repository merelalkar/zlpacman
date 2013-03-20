#include "stdafx.h"
#include "game_screen.h"

#include "pacman_game_screens.h"
#include "game_resources.h"
#include "game_events.h"
#include "game_objects.h"
#include "standart_widgets.h"
#include "fader_layer.h"

#include "platform_windows.h"

using namespace Pegas;

GameVerticalLayer::GameVerticalLayer():
	BaseScreenLayer(_text("game screen"), k_layerGameWorld, false), m_numLives(0)
{
		
}

void GameVerticalLayer::create(IPlatformContext* context)
{
	TheEventMgr.addEventListener(this, Event_HUD_LevelChanged::k_type);
	TheEventMgr.addEventListener(this, Event_HUD_ScoresChanged::k_type);
	TheEventMgr.addEventListener(this, Event_HUD_LivesChanged::k_type);

	m_gameWorld.create(context);

	CURCOORD canvasWidth = GrafManager::getInstance().getCanvasWidth();
	CURCOORD canvasHeight = GrafManager::getInstance().getCanvasHeight();

	TextureResource* mazeTexture = Pegas::TextureResourceManager::getInstance().getResource(k_textureMaze);
	assert(mazeTexture && "k_textureMaze not found");

	m_maze._texture = k_textureMaze;
	m_maze._left = k_sideMargin;
	m_maze._top = k_topBarHeight;
	m_maze._width = canvasWidth - (k_sideMargin * 2);
	m_maze._height = (CURCOORD)((mazeTexture->getInnerImageHeight() * m_maze._width) / mazeTexture->getInnerImageWidth());

	tchar buffer[50];
#ifdef _UNICODE
		m_scoresText = _itow(MAX_INT32, buffer, 50);
#else
		m_scoresText = _itoa(MAX_INT32, buffer, 50);
#endif	
	CURCOORD bottomBarHeight, scoresBarWidth;
	GrafManager::getInstance().getTextExtent(buffer, k_fontMenuButton, scoresBarWidth, bottomBarHeight); 

	//scores text sprite
	TextureResource* scoresTextTexture = Pegas::TextureResourceManager::getInstance().getResource(k_textureScoresText);
	assert(scoresTextTexture && "k_textureScoresText not found");

	m_scoresTextSprite._texture = k_textureScoresText;
	m_scoresTextSprite._left = k_sideMargin;
	m_scoresTextSprite._top = k_topBarHeight + m_maze._height + k_sideMargin;
	m_scoresTextSprite._height = k_bottomBarHeight;
	m_scoresTextSprite._width = (scoresTextTexture->getInnerImageWidth() * m_scoresTextSprite._height) / scoresTextTexture->getInnerImageHeight(); 
	
	//lives text sprite
	TextureResource* livesTextTexture = Pegas::TextureResourceManager::getInstance().getResource(k_textureLivesText);
	assert(livesTextTexture && "k_textureLivesText not found");

	m_livesText._texture = k_textureLivesText;
	m_livesText._left = canvasWidth * 0.5;
	m_livesText._top = k_topBarHeight + m_maze._height + k_sideMargin;
	m_livesText._height = k_bottomBarHeight;
	m_livesText._width = (livesTextTexture->getInnerImageWidth() * m_livesText._height) / livesTextTexture->getInnerImageHeight();

	//live icon sprite
	TextureResource* livesIconTexture = Pegas::TextureResourceManager::getInstance().getResource(k_texturePacmanStaticSprite);
	assert(livesIconTexture && "livesIconTexture not found");

	m_liveIcon._texture = k_texturePacmanStaticSprite;
	m_liveIcon._top = k_topBarHeight + m_maze._height + k_sideMargin;
	m_liveIcon._height = k_bottomBarHeight;
	m_liveIcon._width = (livesIconTexture->getInnerImageWidth() * m_liveIcon._height) / livesIconTexture->getInnerImageHeight();

	m_scoresTextParams._font = k_fontMenuButton;
	m_scoresTextParams._color = 0xffffff33;
	m_scoresTextParams._top = m_scoresTextSprite._top + m_scoresTextSprite._height;
	m_scoresTextParams._left = m_scoresTextSprite._left + m_scoresTextSprite._width + k_sideMargin;

	m_levelTextParams._font = k_fontMenuButton;
	m_levelTextParams._color = 0xffffff33;
	m_levelTextParams._top = m_scoresTextSprite._top + m_scoresTextSprite._height;
	m_levelTextParams._left = canvasWidth - k_sideMargin - k_bottomBarHeight;	
}

void GameVerticalLayer::destroy(IPlatformContext* context)
{
	TheEventMgr.removeEventListener(this);

	m_gameWorld.destroy(context);
}

void GameVerticalLayer::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
{
	m_gameWorld.update(context, deltaTime, timeLimit);
}

void GameVerticalLayer::render(IPlatformContext* context)
{
	GrafManager::getInstance().drawSprite(m_maze);

	m_gameWorld.render(context);

	GrafManager::getInstance().drawSprite(m_scoresTextSprite);
	GrafManager::getInstance().drawSprite(m_livesText);
	GrafManager::getInstance().drawText(m_scoresText, m_scoresTextParams);
	GrafManager::getInstance().drawText(m_levelText, m_levelTextParams);
	
	m_liveIcon._left = m_livesText._left + m_livesText._width + 5;
	for(int32 i = 0; i < m_numLives; i++)
	{
		GrafManager::getInstance().drawSprite(m_liveIcon);
		m_liveIcon._left+= m_liveIcon._width + 5;
	}
}

void GameVerticalLayer::onKeyDown(KeyCode key, KeyFlags flags)
{
	if(key == IKeyboardController::k_keyCodeUP &&  !(flags & k_keyFlagRepeat))
	{
		EventPtr evt(new Event_ChangeDirection(k_actorPacman, Character::k_moveTop));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeDOWN &&  !(flags & k_keyFlagRepeat))
	{
		EventPtr evt(new Event_ChangeDirection(k_actorPacman, Character::k_moveBottom));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeLEFT &&  !(flags & k_keyFlagRepeat))
	{
		EventPtr evt(new Event_ChangeDirection(k_actorPacman, Character::k_moveLeft));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeRIGHT &&  !(flags & k_keyFlagRepeat))
	{
		EventPtr evt(new Event_ChangeDirection(k_actorPacman, Character::k_moveRight));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeESCAPE)
	{
		EventPtr evt(new Event_Game_Pause());
		TheEventMgr.pushEventToQueye(evt);
	}
}

void GameVerticalLayer::onKeyUp(KeyCode key, KeyFlags flags)
{
	if(key == IKeyboardController::k_keyCodeUP
		|| key == IKeyboardController::k_keyCodeDOWN
		|| key == IKeyboardController::k_keyCodeLEFT
		|| key == IKeyboardController::k_keyCodeRIGHT)
	{
		EventPtr evt(new Event_CancelChangingDirection(k_actorPacman));
		TheEventMgr.pushEventToQueye(evt);
	}
}

void GameVerticalLayer::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_HUD_LevelChanged::k_type)
	{
		Event_HUD_LevelChanged* pEvent = evt->cast<Event_HUD_LevelChanged>();
		m_level = pEvent->_level;
		
		tchar buffer[10];
#ifdef _UNICODE
		m_levelText = _itow(m_level, buffer, 10);
#else
		m_levelText = _itoa(m_level, buffer, 10);
#endif	
	}

	if(evt->getType() == Event_HUD_ScoresChanged::k_type)
	{
		Event_HUD_ScoresChanged* pEvent = evt->cast<Event_HUD_ScoresChanged>();
		m_numScore = pEvent->_scores;

		tchar buffer[10];
#ifdef _UNICODE
		m_scoresText = _itow(m_numScore, buffer, 10);
#else
		m_scoresText = _itoa(m_numScore, buffer, 10);
#endif	
	}

	if(evt->getType() == Event_HUD_LivesChanged::k_type)
	{
		Event_HUD_LivesChanged* pEvent = evt->cast<Event_HUD_LivesChanged>();
		m_numLives = pEvent->_lives;		
	}
}