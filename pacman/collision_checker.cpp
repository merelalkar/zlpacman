#include "stdafx.h"
#include "collision_checker.h"

using namespace Pegas;

CollisionChecker::CollisionChecker()
	:m_tileGrid(0)
{
	
}

/*
	typedef std::list<IColissionHull*> ProcessingList;
		typedef ProcessingList::iterator ProcessingListIt;
		typedef std::map<int32, int32> ActionMap;
		typedef ActionMap::iterator	ActionMapIt;
		
		TileGrid*				  m_tileGrid;
		CellGrid<IColissionHull*> m_collisionGrid;
		ProcessingList			  m_processingLists;
		ActionMap				  m_actionMap;
*/

void CollisionChecker::init(TileGrid* tileGrid)
{
	m_tileGrid = tileGrid;

	m_collisionGrid.create(tileGrid->getNumRows(), tileGrid->getNumColumns());
	m_collisionGrid.setArea(m_tileGrid->getLeft(), m_tileGrid->getTop(), m_tileGrid->getWidth(), m_tileGrid->getLeft());
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

	static int32 rowOffsets[] = { -1, -1, -1, 0, 1, 1, 1, 0, 0 };
	static int32 columnOffsets[] = { -1, 0, 1, 1, 1, 0, -1, -1, 0 };

	for(ProcessingListIt it = m_processingLists.begin(); it != m_processingLists.end(); ++it)
	{
		IColissionHull* subject = (*it);
		Vector3 pos = subject->getPosition();
		int32 row, column;
		m_tileGrid->pointToCell(pos._x, pos._y, row, column);

		Cell<IColissionHull*>* myCell = m_collisionGrid.getCell(pos); 
		for(int32 index = 0; index <= 8; index++)
		{
			Cell<IColissionHull*>* cell = myCell->getSibling(index);
			for(Cell<IColissionHull*>::ObjectListIt iit = cell->begin(); iit != cell->end(); ++iit)
			{
				IColissionHull* object = (*iit);
				if(subject == object) continue;
				
				if(isIntersects(subject, object))
				{
					//столкновение с динамическим объектом
					//processCollision(subject, object);
				}
			}//for(Cell<IColissionHull*>::ObjectListIt iit = cell->begin(); iit != cell->end(); ++iit)
					
			if(isIntersects(subject, row + rowOffsets[index], column + columnOffsets[index]))
			{
				//столкновение со статическим объектом
				//processCollision(IColissionHull* subject, int32 row, int32 column);
			}
			
		}//for(int32 index = 0; index <= 8; index++)		
	}//for(ProcessingListIt it = m_processingLists.begin(); it != m_processingLists.end(); ++it)


	
}

void CollisionChecker::addCollisionHull(IColissionHull* hull)
{
	m_processingLists.push_back(hull);	
}

void CollisionChecker::setCollisionAction(int32 subject, int32 object, int32 action)
{
	int32 key = (subject << 16) | object;
	if(m_actionMap.count(key) == 0)
	{
		m_actionMap[key] = action;
	}
}

int32 CollisionChecker::getAction(int32 subject, int32 object)
{
	int32 key = (subject << 16) | object;
	if(m_actionMap.count(key) > 0)
	{
		return m_actionMap[key];
	}

	return k_actionNothing;
}

bool CollisionChecker::isIntersects(IColissionHull* a, IColissionHull* b)
{
	assert(a != 0);
	assert(b != 0);

	Vector3 distance = a->getPosition() - b->getPosition();

	return distance.length() <= (a->getRadius() + b->getRadius());
}

bool CollisionChecker::isIntersects(IColissionHull* a, int32 row, int32 column)
{

}

void CollisionChecker::processCollision(IColissionHull* subject, IColissionHull* object)
{

}

void CollisionChecker::processCollision(IColissionHull* subject, int32 row, int32 column)
{

}

/*
class BulletPhysics : public IGamePhysics, boost::noncopyable 
{ 
	// use auto pointers to automatically call delete on these objects 
	// during ~BulletPhysics 
	// these are all of the objects that Bullet uses to do its work. 
	// see BulletPhysics::VInitialize() for some more info. 
	std::auto_ptr<btDynamicsWorld> m_dynamicsWorld; 
	std::auto_ptr<btBroadphaseInterface> m_broadphase; 
	std::auto_ptr<btCollisionDispatcher> m_dispatcher; 
	std::auto_ptr<btConstraintSolver> m_solver; 
	std::auto_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration; 
	std::auto_ptr<BulletDebugDrawer> m_debugDrawer; 
	
	// keep track of the existing rigid bodies: To check them for updates 
	// to the actors' positions and to remove them when their lives are 
	// over. 
	typedef std::map<ActorId, BulletActor*> ActorIDToBulletActorMap; 
	ActorIDToBulletActorMap m_actorBodies; 
	
	// also keep a map to get the actor ID from the btRigidBody* 
	typedef std::map<btRigidBody const *, ActorId> RigidBodyToActorIDMap;
	RigidBodyToActorIDMap m_rigidBodyToActorId; 
	
	// data used to store which collision pair (bodies that are touching) 
	// need collision events sent. When a new pair of touching bodies are 
	// detected, they are added to m_previousTickCollisionPairs and an 
	// event is sent. When the pair is no longer detected, they are 
	// removed and another event is sent. 
	typedef std::pair< btRigidBody const *, btRigidBody const * > CollisionPair; 
	typedef std::set< CollisionPair > CollisionPairs; 
	CollisionPairs m_previousTickCollisionPairs; 
	
	// helpers for sending events relating to collision pairs 
	void SendCollisionPairAddEvent( btPersistentManifold const * manifold, btRigidBody const * body0, btRigidBody const * body1 ); 
	void SendCollisionPairRemoveEvent( btRigidBody const * body0, btRigidBody const * body1 ); 
	
	// common functionality used by VAddSphere, VAddBox, etc 
	void AddShape( IActor * actor, btCollisionShape * shape, btScalar mass, enum PhysicsMaterial mat ); 
	//helper for cleaning up objects 
	void RemoveCollisionObject( btCollisionObject * removeMe ); 
	
	// find the rigid body associated with the given actor ID 
	btRigidBody * FindActorBody( ActorId id ) const; 
	// find the BulletActor object with the given actor ID 
	BulletActor* FindBulletActor(ActorId id) const; 
	// find the actor ID associated with the given body 
	optional<ActorId> FindActorID( btRigidBody const * ) const; 
	// callback from bullet for each physics time step. set in VInitialize static 
	void BulletInternalTickCallback( btDynamicsWorld * const world, btScalar const timeStep ); public: BulletPhysics(); virtual ~BulletPhysics();
	// Initialiazation and Maintenance of the Physics World 
	
	virtual bool VInitialize(); 
	virtual void VSyncVisibleScene(); 
	virtual void VOnUpdate( float deltaSeconds ); 
	
	// Initialization of Physics Objects 
	virtual void VAddSphere(float radius, IActor *actor, float specificGravity, enum PhysicsMaterial mat); 
	virtual void VAddBox(const Vec3& dimensions, IActor *gameActor, float specificGravity, enum PhysicsMaterial mat); 
	virtual void VAddPointCloud(Vec3 *verts, int numPoints, IActor *gameActor, float specificGravity, enum PhysicsMaterial mat); 
	virtual void VRemoveActor(ActorId id); 
	
	// Debugging virtual void VRenderDiagnostics(); 
	// Physics world modifiers 
	virtual void VCreateTrigger(const Vec3 &pos, const float dim, int triggerID); 
	virtual void VApplyForce(const Vec3 &dir, float newtons, ActorId aid); 
	virtual void VApplyTorque(const Vec3 &dir, float newtons, ActorId aid); 
	virtual bool VKinematicMove(const Mat4x4 &mat, ActorId aid); 
	virtual void VRotateY(ActorId actorId, float angleRadians, float time); 
};


void BulletPhysics::BulletInternalTickCallback( btDynamicsWorld * const world, btScalar const timeStep ) 
{ 
	assert( world ); 
	assert( world->getWorldUserInfo() ); 
	
	BulletPhysics * const bulletPhysics = static_cast<BulletPhysics*>( world->getWorldUserInfo() ); 
	CollisionPairs currentTickCollisionPairs;
	// look at all existing contacts 
	
	btDispatcher * const dispatcher = world->getDispatcher(); 
	
	for ( int manifoldIdx = 0; manifoldIdx < dispatcher->getNumManifolds(); ++manifoldIdx ) 
	{ 
		// get the "manifold", which is the set of data corresponding to a 
		// contact point between two physics objects 
		
		btPersistentManifold const * const manifold = dispatcher->getManifoldByIndexInternal( manifoldIdx ); 
		assert( manifold ); 
		
		// get the two bodies used in the manifold. Bullet stores them as 
		// void*, so we must cast them back to btRigidBody*s. 
		// Manipulating void* pointers is usually a bad idea, but we 
		// have to work with the environment that we're given. We know 
		// this is safe because we only ever add btRigidBodys to the 
		// simulation 
		
		btRigidBody const * const body0 = static_cast<btRigidBody const *>(manifold->getBody0()); 
		btRigidBody const * const body1 = static_cast<btRigidBody const *>(manifold->getBody1()); 
		
		// always create the pair in a predictable order 
		bool const swapped = body0 > body1; 
		btRigidBody const * const sortedBodyA = swapped ? body1 : body0; 
		btRigidBody const * const sortedBodyB = swapped ? body0 : body1; 
		
		CollisionPair const thisPair = std::make_pair( sortedBodyA, sortedBodyB); 
		currentTickCollisionPairs.insert( thisPair ); 
		
		if ( bulletPhysics->m_previousTickCollisionPairs.find( thisPair ) == bulletPhysics->m_previousTickCollisionPairs.end() ) 
		{ 
			// this is a new contact, which wasn't in our list before. 
			// send an event to the game. 
			bulletPhysics->SendCollisionPairAddEvent( manifold, body0, body1 ); 
		}
	} 
	
	CollisionPairs removedCollisionPairs; 
	// use the STL set difference function to find collision pairs that 
	// existed during the previous tick but not any more 
	
	std::set_difference(bulletPhysics->m_previousTickCollisionPairs.begin(), 
						bulletPhysics->m_previousTickCollisionPairs.end(), 
						currentTickCollisionPairs.begin(), 
						currentTickCollisionPairs.end(), 
						std::inserter(removedCollisionPairs, removedCollisionPairs.begin())); 
	
	for (CollisionPairs::const_iterator it = removedCollisionPairs.begin(), end = removedCollisionPairs.end(); it != end; ++it ) 
	{ 
		btRigidBody const * const body0 = it->first; 
		btRigidBody const * const body1 = it->second; 
		bulletPhysics->SendCollisionPairRemoveEvent( body0, body1 ); 
	} 
	
	// the current tick becomes the previous tick. this is the way of all 
	// things. 
	bulletPhysics->m_previousTickCollisionPairs = currentTickCollisionPairs; 
}
*/

