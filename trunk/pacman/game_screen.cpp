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

/*************************************************************************************************
	GameVerticalLayer class implementation
**************************************************************************************************/
GameVerticalLayer::GameVerticalLayer():
	BaseScreenLayer(_text("game screen"), k_layerGameWorld, false), m_numLives(0), m_keyDownMutex(0)
{
	m_controlledActor = k_actorPacman;		
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
		m_keyDownMutex++;

		EventPtr evt(new Event_ChangeDirection(m_controlledActor, Character::k_moveTop));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeDOWN &&  !(flags & k_keyFlagRepeat))
	{
		m_keyDownMutex++;

		EventPtr evt(new Event_ChangeDirection(m_controlledActor, Character::k_moveBottom));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeLEFT &&  !(flags & k_keyFlagRepeat))
	{
		m_keyDownMutex++;

		EventPtr evt(new Event_ChangeDirection(m_controlledActor, Character::k_moveLeft));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeRIGHT &&  !(flags & k_keyFlagRepeat))
	{
		m_keyDownMutex++;

		EventPtr evt(new Event_ChangeDirection(m_controlledActor, Character::k_moveRight));
		TheEventMgr.pushEventToQueye(evt);
	}

	if(key == IKeyboardController::k_keyCodeESCAPE)
	{
		EventPtr evt(new Event_Game_Pause());
		TheEventMgr.pushEventToQueye(evt);
	}

	//TODO: временно. Убрать когда будет экран паузы
	if(key == IKeyboardController::k_keyCodeSPACE)
	{
		EventPtr evt(new Event_Game_Resume());
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
		m_keyDownMutex--;
		if(m_keyDownMutex == 0)
		{
			EventPtr evt(new Event_CancelChangingDirection(m_controlledActor));
			TheEventMgr.pushEventToQueye(evt);
		}
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

/*************************************************************************************************
	Debug Layer
***************************************************************************************************/
DebugLayer::DebugLayer()
	:BaseScreenLayer(_text("debug console"), k_layerDebug, false)
{
	
}

void DebugLayer::create(IPlatformContext* context)
{
	m_ellapsedTime = 0;
	m_frames = 0;
	
	m_params._font = k_fontMain;
	m_params._color = 0xffffffff;
	m_params._left = 10;
	m_params._top = 20;
}

void DebugLayer::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
{
	m_ellapsedTime+= deltaTime;
	m_frames++;

	if(m_ellapsedTime >= 1000)
	{
		int32 FPS = (int32)(m_frames / (m_ellapsedTime * 0.001));
		m_frames = m_ellapsedTime = 0;

		tchar text[50];
#ifndef _UNICODE
		sprintf(text, _text("FPS = %d"), FPS);
#else
		wsprintf(text, _text("FPS = %d"), FPS);
#endif
		m_fpsText = text;
	}
}

void DebugLayer::render(IPlatformContext* context)
{
	GrafManager& graf = GrafManager::getInstance();
	graf.drawText(m_fpsText, m_params);
}

/**************************************************************************************************
	
***************************************************************************************************/
enum GamePauseButtons
{
	k_gamePauseExitToMenu = 1,
	k_gamePauseResumeGame,
	k_gamePauseOptionsMenu
};

GameScreen::GameScreen():
	DefaultGameState(k_stateGame)
{

}

void GameScreen::enter(IPlatformContext* context)
{
	DefaultGameState::enter(context);

	TheEventMgr.addEventListener(this, Event_GUI_ButtonClick::k_type);
	TheEventMgr.addEventListener(this, Event_Game_Pause::k_type);
	
	pushLayer(BaseScreenLayerPtr(new GameVerticalLayer()));
	//TODO: pause layer
	//TODO: options layer
	pushLayer(BaseScreenLayerPtr(new FaderLayer(k_layerFader)));
	pushLayer(BaseScreenLayerPtr(new DebugLayer()));

	EventPtr evt(new Event_GUI_StartFadeout());
	TheEventMgr.triggerEvent(evt);	
}

void GameScreen::leave(IPlatformContext* context)
{
	TheEventMgr.removeEventListener(this);

	DefaultGameState::leave(context);
}

void GameScreen::handleEvent(EventPtr evt)
{
	if(evt->getType() == Event_GUI_ButtonClick::k_type)
	{
		Event_GUI_ButtonClick* pEvent = evt->cast<Event_GUI_ButtonClick>();
		if(pEvent->m_button->getID() == k_gamePauseExitToMenu)
		{
			ProcessPtr fadein = new Fadein();
			m_platform->attachProcess(fadein);

			ProcessPtr toMenu = new ChangeStateTask(m_platform, k_stateMainMenu);
			fadein->attachNext(toMenu);
		}
	}
}