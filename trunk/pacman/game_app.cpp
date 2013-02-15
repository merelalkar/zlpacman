#include "stdafx.h"
#include "game_app.h"
#include "resource.h"

using namespace Pegas;

const RESOURCEID k_textureCardBack = 1;
const RESOURCEID k_textureCardKing = 2;
const RESOURCEID k_textureFelt = 3;

const RESOURCEID k_stringYouWinThePrize = 1;
const RESOURCEID k_stringScores = 2;
const RESOURCEID k_stringTurns = 3;
const RESOURCEID k_stringGameWindowTitle = 4;

const RESOURCEID k_fontBig = 1;
const RESOURCEID k_fontSmall = 2;

const RESOURCEID k_soundPassNewSeries = 1;
const RESOURCEID k_soundReleaseCard = 2;
const RESOURCEID k_soundShowValidTurn = 3;
const RESOURCEID k_soundNoValidTurn = 4;
const RESOURCEID k_soundTakeCard = 5;
const RESOURCEID k_soundYouWin = 6;


void GameApplication::init(HWND hWnd)
{
	registerResources();
	playSound();

	m_grafManager.initialize(hWnd);
	//m_grafManager.addLayout(this);
}

void GameApplication::run()
{
	if(!m_isActive)
		return;

	m_grafManager.render();
}

void GameApplication::cleanup()
{
	m_grafManager.destroy();
	m_stringManager.destroyAll();
	m_textureManager.destroyAll();
	m_fontManager.destroyAll();
}

void GameApplication::resize(int width, int height)
{
	m_grafManager.setViewport(0, 0, width, height);
}

void GameApplication::draw(GrafManager& context)
{
	/*
	Pegas::SpriteParameters sprite;
	sprite._left = 0;
	sprite._top = 0;
	sprite._width = context.getCanvasWidth();
	sprite._height = context.getCanvasHeight();
	sprite._texture = k_textureFelt;
	sprite._flags = k_repeatTextureAlongX | k_repeatTextureAlongY;
	
	context.drawSprite(sprite);

	sprite._left = 10;
	sprite._top = 130;
	sprite._width = 71;
	sprite._height = 96;
	sprite._texture = k_textureCardKing;
	sprite._flags = 0;

	context.drawSprite(sprite);

	sprite._left = 110;
	sprite._top = 130;
	sprite._width = 71;
	sprite._height = 96;
	sprite._texture = k_textureCardKing;
	sprite._flags = k_invertTextureColors;

	context.drawSprite(sprite);

	
	context.drawRectangle(10, 10, 100, 100, 0xffff0000, 0xff0000ff);
	context.drawEllipse(110, 10, 100, 100, 0xff000000, 0xff00ff00);
	context.drawLine(10, 120, 210, 120, 0xff0000ff);
	
	CURCOORD width, height;
	context.getTextExtent(k_stringYouWinThePrize, k_fontBig, width, height);
	
	TextParameters tp;
	tp._left = (context.getCanvasWidth() - width) / 2;
	tp._top = (context.getCanvasHeight() - height) / 2;
	tp._color = 0xffffffff;
	tp._font = k_fontBig;

	context.drawText(k_stringYouWinThePrize, tp);*/
	
}

void GameApplication::playSound()
{
	//m_soundPlayer.playSound(k_soundYouWin);	
}

void GameApplication::registerResources()
{
	/*
	m_stringManager.registerResource(k_stringYouWinThePrize, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_YOU_WIN_THE_PRIZE));
	m_stringManager.registerResource(k_stringScores, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_SCORES));
	m_stringManager.registerResource(k_stringTurns, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_TURNS));
	m_stringManager.registerResource(k_stringGameWindowTitle, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_APP_TITLE));
	m_stringManager.loadAll();

	m_textureManager.registerResource(k_textureCardBack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARDBACK));
	m_textureManager.registerResource(k_textureCardKing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARDKING));
	m_textureManager.registerResource(k_textureFelt, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_FELT));

	m_fontManager.registerResource(k_fontBig, MAKE_FONT_RESOURCE_CODE(_T("Verdana"), 72));
	m_fontManager.registerResource(k_fontSmall, MAKE_FONT_RESOURCE_CODE(_T("Verdana"), 12));*/

	/*
	const RESOURCEID k_soundPassNewSeries = 1;
	const RESOURCEID k_soundReleaseCard = 2;
	const RESOURCEID k_soundShowValidTurn = 3;
	const RESOURCEID k_soundNoValidTurn = 4;
	const RESOURCEID k_soundTakeCard = 5;
	const RESOURCEID k_soundYouWin = 6;
	*/

	/*m_soundManager.registerResource(k_soundPassNewSeries, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE1));
	m_soundManager.registerResource(k_soundReleaseCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE2));
	m_soundManager.registerResource(k_soundShowValidTurn, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE3));
	m_soundManager.registerResource(k_soundNoValidTurn, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE4));
	m_soundManager.registerResource(k_soundTakeCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE5));
	m_soundManager.registerResource(k_soundYouWin, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE6));
	m_soundManager.loadAll();*/
}