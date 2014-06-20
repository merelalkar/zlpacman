#pragma once

#include "interfaces.h"
#include "cell_grid.h"

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

	class ICollisionHull
	{
	public:
		enum
		{
			k_typePoint = 0,
			k_typeCircle,
			k_typePolygon,
			k_typeTotal
		};
	public:
		ICollisionHull(int32 id, int32 collisionGroup):
		  m_id(id), m_collisionGroup(collisionGroup) {}

		int32 getId() const { return m_id; }
		int32 getCollisionGroup() const { return m_collisionGroup; }
		virtual int32 getType() = 0;

		virtual void moveObject(const Vector3& offset, bool absolute) = 0;
		virtual void rotateObject(float degreesOffset, bool absolute) = 0;
		virtual bool isIntersects(ICollisionHull* other)= 0;
		virtual Vector3 getPosition() = 0;

	protected:
		int32 m_id;
		int32 m_collisionGroup;
	};

	class CollisionManager
	{
	public:
		typedef std::vector<Vector3> PointList;
		typedef std::pair<int32, int32> CollisionPair;
		typedef std::list<CollisionPair> CollisionPairList;
		typedef CollisionPairList::iterator CollisionPairListIt;
		
		typedef ptr<ICollisionHull> CollisionHullPtr;
		typedef std::map<int32, CollisionHullPtr> CollisionHullMap;
		typedef std::set<int32> CollisionPairsHashes;

	public:
		bool registerPoint(int32 id, int32 group, const Vector3& position);
		bool registerCircle(int32 id, int32 group, const Vector3& position, float radius);
		bool registerPoligon(int32 id, int32 group, const PointList& points);
		void unregisterCollisionHull(int32 id);
		
		void moveObject(int32 id, const Vector3& offset, bool absolute = true);
		void rotateObject(int32 id, float degreesOffset, bool absolute = true);
		
		void update();
		CollisionPairList& getCollidedPairs();

	public:
		static bool isIntersectsPointCircle(ICollisionHull* point, ICollisionHull* circle);
		static bool isIntersectsCirclePoint(ICollisionHull* circle, ICollisionHull* point);

		static bool isIntersectsPointPolygon(ICollisionHull* point, ICollisionHull* polygon);
		static bool isIntersectsPolygonPoint(ICollisionHull* polygon, ICollisionHull* point);

		static bool isIntersectsCirclePolygon(ICollisionHull* circle, ICollisionHull* polygon);
		static bool isIntersectsPolygonCircle(ICollisionHull* polygon, ICollisionHull* circle);
		
		static bool isIntersectsPointPoint(ICollisionHull* point1, ICollisionHull* point2);
		static bool isIntersectsCircleCircle(ICollisionHull* circle1, ICollisionHull* circle2);
		static bool isIntersectsPolygonPolygon(ICollisionHull* polygon1, ICollisionHull* polygon2);

	private:
		CellGrid<ICollisionHull*> m_cellGrid;
		CollisionHullMap m_collisionHulls;
		CollisionPairsHashes m_previousCollisionPairs;
		CollisionPairList m_pairs;

		typedef bool (*IntersectsChecker)(ICollisionHull* a, ICollisionHull* b);
		IntersectsChecker m_checkers[ICollisionHull::k_typeTotal][ICollisionHull::k_typeTotal];
	};

	class PointCollisionHull: public ICollisionHull 
	{
	public:
		PointCollisionHull(int32 id, int32 group, const Vector3& position);

		virtual int32 getType() { return k_typePoint; }

		virtual void moveObject(const Vector3& offset, bool absolute);
		virtual void rotateObject(float degreesOffset, bool absolute);
		virtual bool isIntersects(ICollisionHull* other);
		virtual Vector3 getPosition();

	protected:
		Vector3 m_initialPosition;
		Vector3 m_currentPosition;
	};

	class CircleCollisionHull: public PointCollisionHull
	{
	public:
		CircleCollisionHull(int32 id, int32 group, const Vector3& position, float radius);

		virtual int32 getType() { return k_typeCircle; }
		virtual bool isIntersects(ICollisionHull* other);

		float getRadius() const { return m_radius; }

	protected:
		float m_radius;
	};

	class PoligonCollisionHull: public ICollisionHull
	{
	public:
		PoligonCollisionHull(int32 id, int32 group, const CollisionManager::PointList& points);

		virtual int32 getType() { return k_typePolygon; }

		virtual void moveObject(const Vector3& offset, bool absolute);
		virtual void rotateObject(float degreesOffset, bool absolute);
		virtual bool isIntersects(ICollisionHull* other);
		virtual Vector3 getPosition();
		CollisionManager::PointList getPoints() const { return m_currentPoints; }

	protected:
		CollisionManager::PointList m_initalPoints;
		CollisionManager::PointList m_currentPoints;
	};
}