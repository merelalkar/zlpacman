// MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "../domain/domain.h"

class CMainFrame : public CFrameWnd
{
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	enum e_constants
	{
		k_eventID = 1,
		k_elapse = 10
	};

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnInvokeHelp();
	afx_msg void OnAbout();
	afx_msg void OnAppExit();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();

private:
	UINT_PTR m_timerID;
};


