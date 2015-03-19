// StatisticTab.cpp : implementation file
//

#include "stdafx.h"
#include "FakeSpider.h"
#include "StatisticTab.h"


// CStatisticTab dialog

IMPLEMENT_DYNAMIC(CStatisticTab, CDialog)

CStatisticTab::CStatisticTab(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticTab::IDD, pParent)
{
	m_brushBackGround.CreateSolidBrush(RGB(255, 255, 255));
}

CStatisticTab::~CStatisticTab()
{
}

void CStatisticTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStatisticTab, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CStatisticTab message handlers

HBRUSH CStatisticTab::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here
	
	// TODO:  Return a different brush if the default is not desired
	return m_brushBackGround;
}
