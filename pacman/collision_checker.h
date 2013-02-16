#pragma once

#include "tile_grid.h"
#include "cell_grid.h"

namespace Pegas
{
	class IColissionHull
	{
	public:
		virtual int32	getCollisionGroup() = 0;
		virtual Vector3 getPosition() = 0;
		virtual float	getRadius() = 0;
		virtual void	onCollision(IColissionHull* other) = 0;
	};

	class CollisionChecker
	{
	public:
		enum eCollisionActions
		{
			k_actionNothing,
			k_actionStop,
			k_actionBounse,
			k_actionKill
		};
	public:
		CollisionChecker();

		void init(TileGrid* tileGrid);
		void destroy();
		void update(MILLISECONDS deltaTime);

		void addCollisionHull(IColissionHull* hull);
		void setCollisionAction(int32 groupA, int32 groupB, int32 action);

	private:
		bool isIntersects(IColissionHull* a, IColissionHull* b);
		int32 getAction(int32 groupA, int32 groupB);

		typedef std::list<IColissionHull*> ProcessingList;
		typedef ProcessingList::iterator ProcessingListIt;
		typedef std::map<int32, int32> ActionMap;
		typedef ActionMap::iterator	ActionMapIt;
		
		TileGrid*				  m_tileGrid;
		CellGrid<IColissionHull*> m_collisionGrid;
		ProcessingList			  m_processingLists;
		ActionMap				  m_actionMap;
	};
}