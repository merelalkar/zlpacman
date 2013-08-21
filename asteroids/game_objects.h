#pragma once

/*
The objective of Asteroids is to score as many points as possible by destroying asteroids and flying saucers. 
The player controls a triangular-shaped ship that can rotate left and right, fire shots straight forward, and thrust forward. 
As the ship moves, momentum is not conserved � the ship eventually comes to a stop again when not thrusting. 
The player can also send their ship into hyperspace, causing it to disappear and reappear in a random location on the screen 
(with the risk of self-destructing or appearing on top of an asteroid).
Each stage starts with a few large asteroids drifting in random directions on the screen. Objects wrap around screen edges � 
for instance, an asteroid that drifts off the top edge of the screen reappears at the bottom and continues moving in the same direction. 
As the player shoots asteroids, they break into smaller asteroids that frequently move faster and are more difficult to hit. 
Smaller asteroids also score higher points. Periodically, a flying saucer appears on one side of the screen and moves across to the other before disappearing again. 
The saucers are of two kinds: Large saucers fire in random directions, while small saucers aim at the player's ship.
The minimalist soundtrack features a memorable deep-toned electronic "heartbeat", which quickens as the asteroid density is reduced by the player's fire.
Once the screen has been cleared of all asteroids and flying saucers, a new set of large asteroids appears. 
The number of asteroids increases each round up to a maximum of 12. The game is over when the player has lost all of his lives.
Like many games of its time, Asteroids contains several bugs that were mostly the result of the original programmers underestimating the game's 
popularity or the skill of its players. The maximum possible score in this game is 99,990 points, after which it "rolls over" back to zero. 
Other common terms for this bug were "turn over" and "flip over". Also, an oversight in the small saucer's programming gave rise to a popular strategy known as "lurking" 
� because the saucer could only shoot directly at the player's position on the screen, the player could "hide" at the opposite end of the screen 
and shoot across the screen boundary, while remaining relatively safe. Another popular method of exploiting this bug was to simply use thrust to keep 
the ship in motion with 1 or 2 asteroids in the play field, allowing the player to pick off as many 1,000 point UFOs as possible. 
This led to experienced players being able to play indefinitely on a single credit.[6] This oversight was addressed in the game's sequel, 
Asteroids Deluxe, and led to significant changes in the way game developers designed and tested their games in the future.[citation needed]
On some early versions of the game, it was also possible to hide the ship in the score area indefinitely without being hit by asteroids.
*/

namespace Pegas
{
	typedef std::string GameObjectType;

	class IGameObject
	{
	public:
		virtual GameObjectType getType() = 0;
		virtual void onCollisionEnter(IGameObject* other) = 0;
		virtual void onDraw(GrafManager& graphManager) = 0;
	};

	class Asteroid: public Process, public IGameObject
	{
	public:
		Asteroid(CollisionManager* collisionManager, 
			const Vector3& position, const Vector3& direction);

		//Process
		virtual void update(MILLISECONDS deltaTime);
		virtual void terminate();

		//IGameObject
		virtual GameObjectType getType() { return "Asteroid" };
		virtual void onCollisionEnter(IGameObject* other);
		virtual void onDraw(GrafManager& graphManager);

	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);

		void generatePoints(int32 numPoints, float maxRadius, std::vector<Vector3>& points);
		void movePoints(const Vector3& offset);

		void createShatters();

		CollisionManager* m_collisionManager;
		Vector3			  m_position;
		float			  m_radius;

		Vector3			  m_direction;
		std::vector<Vector3> m_points;
	};

	class Shatter: public Asteroid
	{
	public:
		virtual void onCollisionEnter(IGameObject* other);
	protected:
		virtual void start(ProcessHandle myHandle, ProcessManagerPtr owner);
	};
	
	class Bullet: public Process, public IEventListener
	{
	public:
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();
	};

	class Ship: public Process, public IEventListener
	{
	public:
		virtual void update(MILLISECONDS deltaTime);
		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName();
	};

}