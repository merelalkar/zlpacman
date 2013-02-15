// FakeSpiderView.h : interface of the CFakeSpiderView class
//


#pragma once

class CFakeSpiderView : public CView
{
protected: // create from serialization only
	CFakeSpiderView();
	DECLARE_DYNCREATE(CFakeSpiderView)

// Attributes
public:
	CFakeSpiderDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CFakeSpiderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

private:
	CPoint m_prevCoords;
};

#ifndef _DEBUG  // debug version in FakeSpiderView.cpp
inline CFakeSpiderDoc* CFakeSpiderView::GetDocument() const
   { return reinterpret_cast<CFakeSpiderDoc*>(m_pDocument); }
#endif

