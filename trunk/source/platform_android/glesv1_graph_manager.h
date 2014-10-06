/*
 * glesv1_graph_manager.h
 *
 *  Created on: 06.10.2014
 *      Author: borisov.v
 */

#pragma once

#include "engine.h"

#include <android_native_app_glue.h>
#include <EGL/egl.h>

namespace Pegas
{
	class GLESv1_GraphManager: public GrafManager
	{
	public:
		bool initialize(ANativeWindow* window);
		virtual void destroy();

		virtual void beginScene();
		virtual void endScene();

		virtual CURCOORD getCanvasWidth() { return mWidth; }
		virtual CURCOORD getCanvasHeight() { return mHeight; }

		virtual void drawLine(CURCOORD fromX, CURCOORD fromY, CURCOORD toX, CURCOORD toY, RGBCOLOR color);
		virtual void drawRectangle(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, RGBCOLOR border, RGBCOLOR fill);
		virtual void drawEllipse(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, RGBCOLOR border, RGBCOLOR fill);
		virtual void drawSprite(const SpriteParameters& params);

		virtual void drawText(const String& text, const TextParameters& params);
		virtual void drawText(RESOURCEID textID, const TextParameters& params);
		virtual void getTextExtent(const String& text, RESOURCEID fontID, CURCOORD& width, CURCOORD& height);
		virtual void getTextExtent(RESOURCEID textID, RESOURCEID fontID, CURCOORD& width, CURCOORD& height);

	private:
		void setup();
		void errorReport(const char* message);

		EGLDisplay mDisplay;
		EGLSurface mSurface;
		EGLContext mContext;

		CURCOORD mWidth;
		CURCOORD mHeight;
	};
}
