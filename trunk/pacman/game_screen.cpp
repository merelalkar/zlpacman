#include "stdafx.h"
#include "game_screen.h"

#include "pacman_game_screens.h"
#include "game_resources.h"
#include "game_events.h"
#include "standart_widgets.h"
#include "fader_layer.h"

#include "platform_windows.h"

using namespace Pegas;

GameVerticalLayer::GameVerticalLayer():
	BaseScreenLayer(_text("game screen"), k_layerGameWorld, false), m_numLives(0)
{
	/*
		GameWorld m_gameWorld;

		SpriteParameters m_maze;
		SpriteParameters m_scoresText;
		SpriteParameters m_livesText;
		SpriteParameters m_liveIcon;
		TextParameters m_scoresText;
		int32 m_numLives;
	*/	
}

void GameVerticalLayer::create(IPlatformContext* context)
{
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

	TextureResource* scoresTextTexture = Pegas::TextureResourceManager::getInstance().getResource(k_textureScoresText);
	assert(scoresTextTexture && "k_textureScoresText not found");

	m_scoresText._texture = k_textureScoresText;
	m_scoresText._left = k_sideMargin;
	m_scoresText._top = k_topBarHeight + m_maze._height + k_sideMargin;
	m_scoresText._width = scoresTextTexture->getInnerImageWidth();
	m_scoresText._height = scoresTextTexture->getInnerImageHeight();
	
	TextureResource* livesTextTexture = Pegas::TextureResourceManager::getInstance().getResource(k_textureLivesText);
	assert(livesTextTexture && "k_textureLivesText not found");

	m_livesText._texture = k_textureLivesText;
	m_livesText._left = canvasWidth * 0.5;
	m_livesText._top = k_topBarHeight + m_maze._height + k_sideMargin;
	m_livesText._width = livesTextTexture->getInnerImageWidth();
	m_livesText._height = livesTextTexture->getInnerImageHeight();
}

void GameVerticalLayer::destroy(IPlatformContext* context)
{

}

void GameVerticalLayer::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
{

}

void GameVerticalLayer::render(IPlatformContext* context)
{

}

void GameVerticalLayer::onKeyDown(KeyCode key, KeyFlags flags)
{

}

void GameVerticalLayer::handleEvent(EventPtr evt)
{

}