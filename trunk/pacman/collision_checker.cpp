#include "stdafx.h"
#include "collision_checker.h"

using namespace Pegas;

CollisionChecker::CollisionChecker()
	:m_tileGrid(0)
{
	
}

void CollisionChecker::init(TileGrid* tileGrid)
{
	m_tileGrid = tileGrid;
}

void CollisionChecker::destroy()
{

}

void CollisionChecker::update(MILLISECONDS deltaTime)
{

}

void CollisionChecker::addCollisionHull(IColissionHull* hull)
{

}

void CollisionChecker::setCollisionAction(int32 groupA, int32 groupB, int32 action)
{

}

bool CollisionChecker::isIntersects(IColissionHull* a, IColissionHull* b)
{

}

int32 CollisionChecker::getAction(int32 groupA, int32 groupB)
{

}