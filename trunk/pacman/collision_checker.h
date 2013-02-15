#pragma once

#include "tile_grid.h"

namespace Pegas
{
	class IColissionHull
	{
	public:
		virtual int32	getId() = 0;
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
			k_actionStop,
			k_actionBounse,
			k_actionKill
		};
	public:
		CollisionChecker();

		void init(TileGrid* tileGrid);
		void update(MILLISECONDS deltaTime);

		void addCollisionHull(IColissionHull* hull);
		void setCollisionAction(int32 id, int32 collisionGroup, int32 action);
	};
}