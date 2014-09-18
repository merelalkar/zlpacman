// dswitcher.cpp : implementation file
//

#include "stdafx.h"
#include "FakeSpider.h"
#include "dswitcher.h"


// CDifficutlySwitcher dialog

IMPLEMENT_DYNAMIC(CDifficutlySwitcher, CDialog)

CDifficutlySwitcher::CDifficutlySwitcher(CWnd* pParent /*=NULL*/)
	: CDialog(CDifficutlySwitcher::IDD, pParent)
	, m_difficutlyLevel(0)
{

}

CDifficutlySwitcher::~CDifficutlySwitcher()
{
}

void CDifficutlySwitcher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_DIFFICUTLY_EASY, m_difficutlyLevel);
}


BEGIN_MESSAGE_MAP(CDifficutlySwitcher, CDialog)
END_MESSAGE_MAP()


// CDifficutlySwitcher message handlers
