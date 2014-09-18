/*
 * gles_graph_manager.h
 *
 *  Created on: 24.07.2014
 *      Author: DNS
 */

#ifndef GLES_GRAPH_MANAGER_H_
#define GLES_GRAPH_MANAGER_H_

namespace Pegas
{
	class GLESGraphManager: public GrafManager
	{
		virtual void destroy();

		virtual void beginScene();
		virtual void endScene();

		virtual CURCOORD getCanvasWidth();
		virtual CURCOORD getCanvasHeight();

		virtual void drawLine(CURCOORD fromX, CURCOORD fromY, CURCOORD toX, CURCOORD toY, RGBCOLOR color);
		virtual void drawRectangle(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, RGBCOLOR border, RGBCOLOR fill);
		virtual void drawEllipse(CURCOORD left, CURCOORD top, CURCOORD width, CURCOORD height, RGBCOLOR border, RGBCOLOR fill);
		virtual void drawSprite(const SpriteParameters& params);

		virtual void drawText(const String& text, const TextParameters& params);
		virtual void drawText(RESOURCEID textID, const TextParameters& params);
		virtual void getTextExtent(const String& text, RESOURCEID fontID, CURCOORD& width, CURCOORD& height);
		virtual void getTextExtent(RESOURCEID textID, RESOURCEID fontID, CURCOORD& width, CURCOORD& height);
	};
}


#endif /* GLES_GRAPH_MANAGER_H_ */
