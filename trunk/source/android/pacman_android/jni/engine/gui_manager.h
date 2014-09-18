#ifndef PEGAS_GUIMANAGER_H
#define PEGAS_GUIMANAGER_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{
	class IClickable
	{
	public:
		virtual ~IClickable() {};
		
		virtual void click(CURCOORD x, CURCOORD y) = 0;
		virtual bool isIntersects(CURCOORD x, CURCOORD y) = 0;
		virtual void setSuccessor(IClickable* pSucessor) = 0;
		virtual IClickable* getSucessor() =  0;
		virtual void setZIndex(int zIndex) = 0;
		virtual int getZIndex() = 0;
	};

	class IMovable
	{
	public:
		virtual ~IMovable() {};

		virtual void capture() = 0;
		virtual void move(CURCOORD cx, CURCOORD cy) = 0;
		virtual void release(CURCOORD x, CURCOORD y) = 0;
	};

	class GUIManager: public Singleton<GUIManager>
	{
		typedef std::set<IClickable*> ClickableObjectList;
		typedef std::set<IMovable*> MovableObjectList;

	public:
		GUIManager(): Singleton(*this) {};
		virtual ~GUIManager() {};

		void mouseLButtonDown(CURCOORD x, CURCOORD y);
		void mouseLButtonUp(CURCOORD x, CURCOORD y);
		void mouseMove(CURCOORD cx, CURCOORD cy);

		void addClickableObject(IClickable* pObject);
		void removeClickableObject(IClickable* pObject);
		void addMovableObject(IMovable* pObject);
		void removeMovableObject(IMovable* pObject);

		IClickable* findClickableObject(CURCOORD x, CURCOORD y);

	private:
		GUIManager(const GUIManager& src);
		GUIManager& operator=(const GUIManager& src);

		static bool clickableObjectsSortPredicate(IClickable* a, IClickable* b);

		ClickableObjectList m_clickableObjects;
		MovableObjectList   m_movableObjects;
	};
}

#endif //PEGAS_GUIMANAGER_H