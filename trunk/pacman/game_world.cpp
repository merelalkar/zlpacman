#include "stdafx.h"
#include "game_world.h"

using namespace Pegas;

GameWorld::GameWorld()
{

}

void GameWorld::create(IPlatformContext* context)
{
	m_context = context;
	int32 canvasWidth = (int32)GrafManager::getInstance().getCanvasWidth();
	int32 canvasHeight = (int32)GrafManager::getInstance().getCanvasHeight();

	tchar fileName[256];
#ifdef		_UNICODE
	wsprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);  
#else
	sprintf(fileName, _text("maze_%d_%d.map"), canvasWidth, canvasHeight);
#endif
	
	ISerializer* fileStream = m_context->openFile(fileName, FileOpenMode::k_readOnly);
	if(fileStream)
	{
		OSUtils::getInstance().debugOutput("loading...");

		m_tileGrid.load(*fileStream);
				
		int32 numSprites;
		TILEID id;
		CURCOORD left, top, width, height;
				
		(*fileStream) >> numSprites;
		for(int32 i = 0; i < numSprites; i++)
		{
			(*fileStream) >> id;
			(*fileStream) >> left;
			(*fileStream) >> top;
			(*fileStream) >> width;
			(*fileStream) >> height;

			if(m_staticSprites.count(id))
			{
				m_sprites[id]._left = left;
				m_sprites[id]._top = top;
				m_sprites[id]._width = width;
				m_sprites[id]._height = height;
			}
		}//for(int32 i = 0; i < numSprites; i++)				

		m_context->closeFile(fileStream);

		OSUtils::getInstance().debugOutput("loading complete");
	}else
	{
		OSUtils::getInstance().debugOutput("can not open file");
	}
}

void GameWorld::destroy(IPlatformContext* context)
{

}

void GameWorld::update(IPlatformContext* context, MILLISECONDS deltaTime, MILLISECONDS timeLimit)
{

}

void GameWorld::render(IPlatformContext* context)
{

}

void GameWorld::handleEvent(EventPtr evt)
{

}