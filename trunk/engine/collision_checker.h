#pragma once

#include "interfaces.h"

namespace Pegas
{
	/*
	class IColissionHull
	{
	public:
		virtual int32	getCollisionGroup() = 0;
		virtual Vector3 getPosition() = 0;
		virtual float	getRadius() = 0;
		virtual void	onCollisionEnter(IColissionHull* other) {};
		virtual void	onCollisionLeave(IColissionHull* other) {};
	};

	class CollisionChecker
	{
	public:
		CollisionChecker() {}

		void init(TileGrid* tileGrid);
		void destroy();
		void update(MILLISECONDS deltaTime);

		void addCollisionHull(IColissionHull* hull);		

	private:
		bool isIntersects(IColissionHull* a, IColissionHull* b);
		
		typedef std::list<IColissionHull*> ProcessingList;
		typedef ProcessingList::iterator ProcessingListIt;
		
		typedef std::pair<IColissionHull*, IColissionHull*> CollisionPair;
		typedef std::set<CollisionPair> CollisionPairs;
		typedef CollisionPairs::iterator	CollisionPairsIt;
				
		CellGrid<IColissionHull*> m_collisionGrid;
		ProcessingList			  m_processingLists;
		CollisionPairs			  m_prevCollisionPairs;	
	};*/

	class CollisionManager
	{
	public:
		typedef std::list<Vector3> PointList;
		typedef std::pair<int32, int32> CollisionPair;
		typedef std::list<CollisionPair> CollisionPairList;

	public:
		bool registerPoint(int32 id, int32 group, const Vector3& position);
		bool registerCircle(int32 id, int32 group, const Vector3& position, float radius);
		bool registerPoligon(int32 id, int32 group, const PointList& points);
		void unregisterCollisionHull(int32 id);
		
		void moveObject(int32 id, const Vector3& offset);
		void rotateObject(int32 id, float degreesOffset);
		
		void update();
		void getCollidedPairs(CollisionPairList& outPairs);
	};
}