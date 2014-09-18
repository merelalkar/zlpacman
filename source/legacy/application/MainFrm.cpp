// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "FakeSpider.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(IDMI_INVOKE_HELP, &CMainFrame::OnInvokeHelp)
	ON_COMMAND(IDMI_ABOUT, &CMainFrame::OnAbout)
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
	ON_WM_SIZING()	
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
	:m_timerID(0)
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_SPIDER);
	SetIcon(hIcon, TRUE);

	m_timerID = SetTimer(k_eventID, k_elapse, 0);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= (~FWS_ADDTOTITLE);
	

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnInvokeHelp()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}


void CMainFrame::OnAppExit()
{
	// TODO: Add your command handler code here
	OnClose();
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
	CFrameWnd::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here
	int minWidth = Pegas::GameBoard::k_minBoardWidth + 15;
	int minHeight = Pegas::GameBoard::k_minBoardHeight + 60;

	int width = pRect->right - pRect->left;
	int height = pRect->bottom - pRect->top;

	if(width < 	minWidth)
	{
		pRect->right = pRect->left + minWidth;		
	}

	if(height < minHeight)
	{
		pRect->bottom = pRect->top + minHeight;
	}
}



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	theApp.updateAnimation();

	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(m_timerID);
}
