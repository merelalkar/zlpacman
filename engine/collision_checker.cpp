#include "engine.h"
#include "collision_checker.h"

/*
void CollisionChecker::init(TileGrid* tileGrid)
{
	m_collisionGrid.create(tileGrid->getNumRows(), tileGrid->getNumColumns());
	m_collisionGrid.setArea(tileGrid->getLeft(), tileGrid->getTop(), tileGrid->getWidth(), tileGrid->getLeft());
}

void CollisionChecker::destroy()
{
	m_collisionGrid.destroy();
}

void CollisionChecker::update(MILLISECONDS deltaTime)
{
	for(ProcessingListIt it = m_processingLists.begin(); it != m_processingLists.end(); ++it)
	{
		m_collisionGrid.placeToGrid((*it)->getPosition(), (*it));	
	}

	CollisionPairs currentCollisionPairs, removedCollisionPairs;

	for(ProcessingListIt it = m_processingLists.begin(); it != m_processingLists.end(); ++it)
	{
		IColissionHull* subject = (*it);
		Cell<IColissionHull*>* myCell = m_collisionGrid.getCell(subject->getPosition()); 
		for(int32 index = 0; index <= 8; index++)
		{
			Cell<IColissionHull*>* cell = myCell->getSibling(index);
			for(Cell<IColissionHull*>::ObjectListIt iit = cell->begin(); iit != cell->end(); ++iit)
			{
				IColissionHull* object = (*iit);
				if(subject == object) continue;
				
				if(isIntersects(subject, object))
				{
					IColissionHull* a = max(subject, object);
					IColissionHull* b = min(subject, object);

					CollisionPair pair = std::make_pair(a, b);
					currentCollisionPairs.insert(pair);
					
					if(m_prevCollisionPairs.count(pair) == 0)
					{
						a->onCollisionEnter(b);
						b->onCollisionEnter(a);
					}
				}
			}//for(Cell<IColissionHull*>::ObjectListIt iit = cell->begin(); iit != cell->end(); ++iit)			
		}//for(int32 index = 0; index <= 8; index++)		
	}//for(ProcessingListIt it = m_processingLists.begin(); it != m_processingLists.end(); ++it)

	std::set_difference(m_prevCollisionPairs.begin(), m_prevCollisionPairs.end(),
						currentCollisionPairs.begin(), currentCollisionPairs.end(),
						std::inserter(removedCollisionPairs, removedCollisionPairs.begin()));

	for(CollisionPairsIt it = removedCollisionPairs.begin(); it != removedCollisionPairs.end(); ++it)
	{
		IColissionHull* a =	(*it).first;
		IColissionHull* b = (*it).second;

		a->onCollisionLeave(b);
		b->onCollisionLeave(a);
	}
	
	m_prevCollisionPairs = currentCollisionPairs;
}

void CollisionChecker::addCollisionHull(IColissionHull* hull)
{
	m_processingLists.push_back(hull);	
}

bool CollisionChecker::isIntersects(IColissionHull* a, IColissionHull* b)
{
	assert(a != 0);
	assert(b != 0);

	Vector3 distance = a->getPosition() - b->getPosition();

	return distance.length() <= (a->getRadius() + b->getRadius());
}
*/

namespace Pegas
{
	/*********************************************************************************************************
		CollisionManager implementation
	**********************************************************************************************************/
	bool CollisionManager::registerPoint(int32 id, int32 group, const Vector3& position)
	{
		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}

		CollisionHullPtr hull = new PointCollisionHull(id, group, position);
		m_collisionHulls[id] = hull;
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());

		return true;
	}
	
	bool CollisionManager::registerCircle(int32 id, int32 group, const Vector3& position, float radius)
	{
		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}

		CollisionHullPtr hull = new CircleCollisionHull(id, group, position, radius);
		m_collisionHulls[id] = hull;
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());

		return true;
	}
	
	bool CollisionManager::registerPoligon(int32 id, int32 group, const PointList& points)
	{
		assert(id > 0);
		assert(group > 0);
		assert(m_collisionHulls.count(id) == 0);

		if(m_collisionHulls.count(id) > 0)
		{
			return false;
		}

		CollisionHullPtr hull = new PoligonCollisionHull(id, group, points);
		m_collisionHulls[id] = hull;
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());

		return true;
	}
	
	void CollisionManager::unregisterCollisionHull(int32 id)
	{
		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		m_collisionHulls.erase(id);
		m_cellGrid.removeObject(hull.get());
	}
		
	void CollisionManager::moveObject(int32 id, const Vector3& offset, bool absolute)
	{
		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->moveObject(offset, absolute);
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());
	}
	
	void CollisionManager::rotateObject(int32 id, float degreesOffset, bool absolute)
	{
		assert(m_collisionHulls.count(id) > 0);

		CollisionHullPtr hull = m_collisionHulls[id];
		hull->rotateObject(degreesOffset, absolute);
		m_cellGrid.placeToGrid(hull->getPosition(), hull.get());
	}
		
	void CollisionManager::update()
	{

	}
	
	void CollisionManager::getCollidedPairs(CollisionPairList& outPairs)
	{

	}
}


