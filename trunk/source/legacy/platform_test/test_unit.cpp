#include "stdafx.h"
#include "test_unit.h"
#include "resource.h"

using namespace Pegas;

const RESOURCEID k_textureCardBack = 1;
const RESOURCEID k_textureCardKing = 2;
const RESOURCEID k_textureFelt = 3;
const RESOURCEID k_textureFish = 4;
const RESOURCEID k_texturePingo = 5;

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

TestUnit::TestUnit(HINSTANCE hInstance): 
	  Singleton(*this)  
{
	m_hInstance = hInstance;
	m_isActive =  false;
	m_animSprite = 0;
	m_lastTime = 0;
}

void TestUnit::testFileIO()
{
	File outFile, inFile;
	
	outFile.open(_text("test.data"), File::WRITE_ONLY);
	
	int8 data_out_int8 = 0xaa;
	uint8 data_out_uint8 = 0xbb;
	int16 data_out_int16 = 0xcccc;
	uint16 data_out_uint16 = 0xdddd;
	int32 data_out_int32 = 0xfafafa;
	uint32 data_out_uint32 = 0xfbfbfb;
	float data_out_float = 1234.56789;
	bool data_out_bool = false;

	std::vector<String> outStrings;
	outStrings.push_back(_text("строка по русски"));
	outStrings.push_back(_text("Shines down upon you"));
	outStrings.push_back(_text("You walk a lonely road"));
	outStrings.push_back(_text("Your heart will be true"));
	outStrings.push_back(_text("Oh! How far you are from home"));
		
	outFile << data_out_int8;
	outFile << data_out_int16;
	outFile << data_out_int32;

	outFile << data_out_uint8;
	outFile << data_out_uint16;
	outFile << data_out_uint32;
	
	outFile << data_out_float;
	outFile << data_out_bool;

	outFile << (int32)outStrings.size();
	for(int i = 0; i < outStrings.size(); i++)
	{
		outFile << outStrings[i];
	}

	outFile.close();

	int8 data_in_int8;
	uint8 data_in_uint8;
	int16 data_in_int16;
	uint16 data_in_uint16;
	int32 data_in_int32;
	uint32 data_in_uint32;
	float data_in_float;
	bool data_in_bool;
	std::vector<String> inStrings;

	inFile.open(_text("test.data"), File::READ_ONLY);
	
	inFile >> data_in_int8;
	inFile >> data_in_int16;
	inFile >> data_in_int32;

	inFile >> data_in_uint8;
	inFile >> data_in_uint16;
	inFile >> data_in_uint32;
	
	inFile >> data_in_float;
	inFile >> data_in_bool;

	int32 numStrings;
	inFile >> numStrings;
	for(int32 i = 0; i < numStrings; i++)
	{
		String str;
		inFile >> str;

		inStrings.push_back(str);
	}
	
	inFile.close();

	assert(data_out_int8 == data_in_int8);
	assert(data_out_uint8 == data_in_uint8);
	assert(data_out_int16 == data_in_int16);
	assert(data_out_uint16 == data_in_uint16);
	assert(data_out_int32 == data_in_int32);
	assert(data_out_uint32 == data_in_uint32);
	assert(data_out_float == data_in_float);
	assert(data_out_bool == data_in_bool);

	OSUtils::getInstance().debugOutput("File IO test succeful complete");
}

void TestUnit::init(HWND hWnd)
{
	registerResources();
	playSound();
	testFileIO();

	m_grafManager.initialize(hWnd);
	m_grafManager.addLayout(this);

	m_lastTime = m_OSUtils.getCurrentTime();

	SpriteAnimation* process = new SpriteAnimation(k_textureFish, 2, 2);
	process->setFPS(16);
	m_animSprite = process->getSprite();
	m_processManager.attachProcess(ProcessPtr(process));

	m_pingo = new SpriteAnimation(k_texturePingo, 10, 8);
	((SpriteAnimation*)m_pingo.get())->setNumFrames(0, 10);
	((SpriteAnimation*)m_pingo.get())->setFPS(16);
	//((SpriteAnimation*)m_pingo.get())->setFlags(0);

	m_processManager.attachProcess(m_pingo);

	m_isActive = true;
}

void TestUnit::run()
{
	if(!m_isActive)
		return;

	MILLISECONDS currentTime = m_OSUtils.getCurrentTime();
	m_processManager.updateProcesses(currentTime - m_lastTime);
	m_lastTime = currentTime;

	m_grafManager.render();
}

void TestUnit::cleanup()
{
	m_grafManager.destroy();
	m_stringManager.destroyAll();
	m_textureManager.destroyAll();
	m_fontManager.destroyAll();
}

void TestUnit::resize(int width, int height)
{
	m_grafManager.setViewport(0, 0, width, height);
}

void TestUnit::draw(GrafManager& context)
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

	context.drawSprite(sprite);*/
	
	context.drawRectangle(10, 10, 100, 100, 0xffff0000, 0xff0000ff);
	context.drawEllipse(110, 10, 100, 100, 0xff000000, 0xff00ff00);
	context.drawLine(10, 120, 610, 120, 0xff0000ff);

	if(m_animSprite)
	{
		m_animSprite->_left = 210;
		m_animSprite->_top = 130;
		m_animSprite->_width = 128;
		m_animSprite->_height = 128;
		
		context.drawSprite(*m_animSprite);

		context.drawRectangle(210, 130, 128, 128, 0xffff0000, 0);
	}

	if(m_pingo->getStatus() != k_processStatusKilled)
	{
		SpriteParameters* sprite = ((SpriteAnimation*)m_pingo.get())->getSprite();

		sprite->_left = 410;
		sprite->_top = 130;
		sprite->_width = 128;
		sprite->_height = 128;
		//sprite->_flags |= k_flipVertical;
		sprite->_angle = -90.0f;
		

		context.drawSprite(*sprite);
		context.drawRectangle(410, 130, 128, 128, 0xffff0000, 0);
	}
		
	SpriteAnimation::Frame& frame = ((SpriteAnimation*)m_pingo.get())->getCurrentFrame();
	tchar buffer[64];
	wsprintf(buffer, _text("index = %d, i = %d, j = %d"), frame._index, frame._i, frame._j);

	TextParameters tp;
	tp._left = 410;
	tp._top = 270;
	tp._color = 0xffffffff;
	tp._font = k_fontSmall;

	context.drawText(buffer, tp);
	
	/*
	CURCOORD width, height;
	context.getTextExtent(k_stringYouWinThePrize, k_fontBig, width, height);
	
	TextParameters tp;
	tp._left = (context.getCanvasWidth() - width) / 2;
	tp._top = (context.getCanvasHeight() - height) / 2;
	tp._color = 0xffffffff;
	tp._font = k_fontBig;

	context.drawText(k_stringYouWinThePrize, tp);*/
	
}

void TestUnit::playSound()
{
	m_soundPlayer.playSound(k_soundYouWin);	
}

void TestUnit::registerResources()
{
	m_stringManager.registerResource(k_stringYouWinThePrize, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_YOU_WIN_THE_PRIZE));
	m_stringManager.registerResource(k_stringScores, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_SCORES));
	m_stringManager.registerResource(k_stringTurns, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_TURNS));
	m_stringManager.registerResource(k_stringGameWindowTitle, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_APP_TITLE));
	m_stringManager.loadAll();

	m_textureManager.registerResource(k_textureCardBack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARDBACK));
	m_textureManager.registerResource(k_textureCardKing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARDKING));
	m_textureManager.registerResource(k_textureFelt, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_FELT));
	m_textureManager.registerResource(k_textureFish, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_FISH));
	m_textureManager.registerResource(k_texturePingo, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_PINGO));

	m_fontManager.registerResource(k_fontBig, MAKE_FONT_RESOURCE_CODE(_T("Verdana"), 72));
	m_fontManager.registerResource(k_fontSmall, MAKE_FONT_RESOURCE_CODE(_T("Verdana"), 12));

	/*
	const RESOURCEID k_soundPassNewSeries = 1;
	const RESOURCEID k_soundReleaseCard = 2;
	const RESOURCEID k_soundShowValidTurn = 3;
	const RESOURCEID k_soundNoValidTurn = 4;
	const RESOURCEID k_soundTakeCard = 5;
	const RESOURCEID k_soundYouWin = 6;
	*/

	m_soundManager.registerResource(k_soundPassNewSeries, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE1));
	m_soundManager.registerResource(k_soundReleaseCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE2));
	m_soundManager.registerResource(k_soundShowValidTurn, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE3));
	m_soundManager.registerResource(k_soundNoValidTurn, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE4));
	m_soundManager.registerResource(k_soundTakeCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE5));
	m_soundManager.registerResource(k_soundYouWin, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE6));
	m_soundManager.loadAll();
}