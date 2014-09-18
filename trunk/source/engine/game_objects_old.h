#ifndef PEGAS_GAME_OBJECTS_H
#define PEGAS_GAME_OBJECTS_H
#pragma once

namespace Pegas
{
	class GameObject: public IDrawable
	{
	public:
		GameObject() {};
		virtual ~GameObject() {};
		
		virtual void setLeft(CURCOORD left) { m_left = left; };
		virtual void setTop(CURCOORD top) { m_top = top; };
		virtual void setWidth(CURCOORD width) { m_width = width; };
		virtual void setHeight(CURCOORD height) { m_height = height; };

		virtual CURCOORD getLeft() { return m_left; };
		virtual CURCOORD getTop() { return m_top; };
		virtual CURCOORD getWidth() { return m_width; };
		virtual CURCOORD getHeight() { return m_height; };

		
	protected:
		CURCOORD m_left;
		CURCOORD m_top;
		CURCOORD m_width;
		CURCOORD m_height;
	};

	class ClickableGameObject: public GameObject, public IClickable
	{
	public:
		ClickableGameObject();
		virtual ~ClickableGameObject();

		virtual void click(CURCOORD x, CURCOORD y);
		virtual bool isIntersects(CURCOORD x, CURCOORD y);

		virtual void setSuccessor(IClickable* pSucessor) { m_pSucessor = pSucessor; };
		virtual IClickable* getSucessor() { return m_pSucessor; };
		virtual void setZIndex(int zIndex) { m_zIndex = zIndex; };
		virtual int getZIndex() { return m_zIndex; };
		

	protected:
		IClickable* m_pSucessor;
		int m_zIndex;
	};

	class MovableGameObject: public ClickableGameObject, public IMovable
	{
	public:
		MovableGameObject();
		virtual ~MovableGameObject() {};

		virtual void capture();
		virtual void move(CURCOORD cx, CURCOORD cy);
		virtual void release(CURCOORD x, CURCOORD y);	
	};
}

#endif //PEGAS_GAME_OBJECTS_H