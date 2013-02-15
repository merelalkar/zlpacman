// FakeSpiderView.cpp : implementation of the CFakeSpiderView class
//

#include "stdafx.h"
#include "FakeSpider.h"

#include "FakeSpiderDoc.h"
#include "FakeSpiderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFakeSpiderView

IMPLEMENT_DYNCREATE(CFakeSpiderView, CView)

BEGIN_MESSAGE_MAP(CFakeSpiderView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()	
END_MESSAGE_MAP()

// CFakeSpiderView construction/destruction

CFakeSpiderView::CFakeSpiderView()
{
	// TODO: add construction code here

}

CFakeSpiderView::~CFakeSpiderView()
{
}

BOOL CFakeSpiderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CFakeSpiderView drawing

void CFakeSpiderView::OnDraw(CDC*)
{
	// TODO: add draw code for native data here
	Pegas::OGLGrafManager& grafManager = theApp.getGrafManager();
	grafManager.render();
}


// CFakeSpiderView diagnostics

#ifdef _DEBUG
void CFakeSpiderView::AssertValid() const
{
	CView::AssertValid();
}

void CFakeSpiderView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFakeSpiderDoc* CFakeSpiderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFakeSpiderDoc)));
	return (CFakeSpiderDoc*)m_pDocument;
}
#endif //_DEBUG


// CFakeSpiderView message handlers

void CFakeSpiderView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Pegas::GUIManager& guiManager = theApp.getGUIManager();
	guiManager.mouseLButtonDown(point.x, point.y);

	m_prevCoords = point;

	CView::OnLButtonDown(nFlags, point);
}

void CFakeSpiderView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Pegas::GUIManager& guiManager = theApp.getGUIManager();
	guiManager.mouseLButtonUp(point.x, point.y);

	CView::OnLButtonUp(nFlags, point);
}

void CFakeSpiderView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	Pegas::GUIManager& guiManager = theApp.getGUIManager();
	guiManager.mouseMove(point.x - m_prevCoords.x, point.y - m_prevCoords.y);

	m_prevCoords = point;

	CView::OnMouseMove(nFlags, point);
}



int CFakeSpiderView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	Pegas::OGLGrafManager& grafManager = theApp.getGrafManager();
	grafManager.initialize(GetSafeHwnd());

	return 0;
}

void CFakeSpiderView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	Pegas::OGLGrafManager& grafManager = theApp.getGrafManager();
	grafManager.setViewport(0, 0, cx, cy);

	GetDocument()->resizeGameBoard(cx, cy);
}


void CFakeSpiderView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	Pegas::OGLGrafManager& grafManager = theApp.getGrafManager();
	grafManager.destroy();
}


