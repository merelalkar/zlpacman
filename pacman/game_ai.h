#pragma once

#include "engine.h"
#include "tile_grid.h"

namespace Pegas
{
	class BaseAIState: public Process, public IEventListener
	{
	public:
		BaseAIState(TileGrid* tileGrid, int32 controlledActor, int32 stateId);

		virtual void suspend();
		virtual void resume();
		virtual void terminate();
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

		virtual int32 chooseDirection();
		virtual float getGoalHeuristic(int32 row, int32 column) = 0;

		TileGrid* m_tileGrid;
		
		int32 m_controlledActor;
		int32 m_stateId;

		int32 m_myRow;
		int32 m_myColumn;
		int32 m_myCurrentDirection;

		int32 m_blockMutex;
		bool  m_characterMoved;
	};

	class BlinkyChaseState: public BaseAIState
	{
	public:
		BlinkyChaseState(TileGrid* tileGrid);
		virtual void handleEvent(EventPtr evt);

	protected:
		virtual float getGoalHeuristic(int32 row, int32 column);

	private:
		Vector3 m_pacmanPosition;		
	};
}