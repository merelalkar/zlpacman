#ifndef PEGAS_GRAF_MANAGER_H
#define PEGAS_GRAF_MANAGER_H
#pragma once
#pragma warning(disable : 4355)

namespace Pegas
{
	enum SpiteDrawingFlags
	{
		k_stretchTextureAlongX = 0x00000001,
        k_stretchTextureAlongY = 0x00000002,
		k_repeatTextureAlongX = 0x00000004,
		k_repeatTextureAlongY = 0x00000008,
	    k_invertTextureColors = 0x00000010,
		k_customTextureCoords = 0x00000020
	};

	struct SpriteParameters
	{
		CURCOORD	_left;
		CURCOORD	_top;
		CURCOORD	_width;
		CURCOORD	_height;
		RESOURCEID	_texture;
		FLAGSET     _flags;

		float		_minU;
		float		_minV;
		float		_maxU;
		float		_maxV;
	};

	struct TextParameters
	{
		CURCOORD	_left;
		CURCOORD	_top;
		RESOURCEID  _font;
		RGBCOLOR    _color;
	};
	
	class IDrawable
	{
	public:
		virtual ~IDrawable() {};
		virtual void draw(GrafManager& context) = 0;
	};

	class GrafManager: public Singleton<GrafManager>
	{
	public:
		GrafManager():Singleton(*this){};
		virtual ~GrafManager() {};

		typedef std::list<IDrawable*> DrawableLayoutsList;
        
        virtual void destroy() = 0;
		virtual void render() = 0;

		virtual CURCOORD getCanvasWidth() = 0;
		virtual CURCOORD getCanvasHeight() = 0;

		virtual void drawLine(CURCOORD fromX, CURCOORD fromY, CURCOORD toX, CURCOORD toY, RGBCOLOR color) = 0;
		virtual void drawRectangle(CURCOORD left, CURCOORD top, CURCOORD width, 
			CURCOORD height, RGBCOLOR border, RGBCOLOR fill) = 0;
		virtual void drawEllipse(CURCOORD left, CURCOORD top, CURCOORD width, 
			CURCOORD height, RGBCOLOR border, RGBCOLOR fill) = 0;		
		virtual void drawSprite(const SpriteParameters& params) = 0;
		
		virtual void drawText(const String& text, const TextParameters& params) = 0;
		virtual void drawText(RESOURCEID textID, const TextParameters& params) = 0;
		virtual void getTextExtent(const String& text, RESOURCEID fontID, CURCOORD& width, CURCOORD& height) = 0;
		virtual void getTextExtent(RESOURCEID textID, RESOURCEID fontID, CURCOORD& width, CURCOORD& height) = 0;

		void resetLayouts();
		void addLayout(IDrawable* layout);
        void removeLayout(IDrawable* layout); 
	protected:
		DrawableLayoutsList m_layouts;
				
		GrafManager(const GrafManager& src);
		GrafManager& operator=(const GrafManager& src);
	};
}

#endif //PEGAS_FAKESPIDER_GRAF_MANAGER_H