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
		virtual void	onCollision(IColissionHull* other) {};
		virtual void	onCollision(int32 group) {};
		virtual void	bounce() {};
		virtual void	kill() {};
	};

	class CollisionChecker
	{
	public:
		enum eCollisionActions
		{
			k_actionNothing,
			k_actionCustom,
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
		void setCollisionAction(int32 subject, int32 object, int32 action);

	private:
		bool isIntersects(IColissionHull* a, IColissionHull* b);
		bool isIntersects(IColissionHull* a, int32 row, int32 column);

		int32 getAction(int32 subject, int32 object);

		void processCollision(IColissionHull* subject, IColissionHull* object);
		void processCollision(IColissionHull* subject, int32 row, int32 column);

		typedef std::list<IColissionHull*> ProcessingList;
		typedef ProcessingList::iterator ProcessingListIt;
		typedef std::map<int32, int32> ActionMap;
		typedef ActionMap::iterator	ActionMapIt;

		typedef std::pair<int32, int32> CollisionPair;
		typedef std::set<CollisionPair> CollisionPairs;
				
		TileGrid*				  m_tileGrid;
		CellGrid<IColissionHull*> m_collisionGrid;
		ProcessingList			  m_processingLists;
		ActionMap				  m_actionMap;
		CollisionPairs			  m_prevCollisionPairs;	
	};
}