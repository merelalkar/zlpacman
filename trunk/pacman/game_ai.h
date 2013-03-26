#pragma once

#include "engine.h"
#include "tile_grid.h"

namespace Pegas
{
	class BaseAIState: public Process, public IEventListener
	{
	public:
		BaseAIState(TileGrid* tileGrid, int32 controlledActor, int32 stateId);

		virtual void terminate();
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

		TileGrid* m_tileGrid;
		
		int32 m_controlledActor;
		int32 m_stateId;

		int32 m_myRow;
		int32 m_myColumn;
		int32 m_myCurrentDirection;
		bool  m_characterMoved;
	};
}