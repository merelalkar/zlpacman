// StatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FakeSpider.h"
#include "StatisticsDlg.h"


// CStatisticsDlg dialog

IMPLEMENT_DYNAMIC(CStatisticsDlg, CDialog)

CStatisticsDlg::CStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticsDlg::IDD, pParent)
{

}

CStatisticsDlg::~CStatisticsDlg()
{
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CStatisticsDlg::OnSelchange)
END_MESSAGE_MAP()


// CStatisticsDlg message handlers

BOOL CStatisticsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	TCITEM tcItem;

	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("Простая игра");
	m_tabCtrl.InsertItem(0, &tcItem);

	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("Средняя игра");
	m_tabCtrl.InsertItem(1, &tcItem);

	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("Сложная игра");
	m_tabCtrl.InsertItem(2, &tcItem);

	m_tabEasy.Create(IDD_STATISTICS_TAB, &m_tabCtrl);
	m_tabMedium.Create(IDD_STATISTICS_TAB, &m_tabCtrl);
	m_tabHeavy.Create(IDD_STATISTICS_TAB, &m_tabCtrl);

	RECT rect;
	m_tabCtrl.GetClientRect(&rect);
	
	rect.top += 25;
	rect.left += 2;
	rect.right -= 2;
	rect.bottom -= 2;

	m_tabEasy.MoveWindow(&rect);
	m_tabMedium.MoveWindow(&rect);
	m_tabHeavy.MoveWindow(&rect);

	m_tabCtrl.SetCurSel(0);
	m_tabEasy.ShowWindow(SW_SHOW);
	m_tabMedium.ShowWindow(SW_HIDE);
	m_tabHeavy.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsDlg::OnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int index = m_tabCtrl.GetCurSel();
    
	if(index == 0)
	{
		m_tabEasy.ShowWindow(SW_SHOW);
		m_tabMedium.ShowWindow(SW_HIDE);
		m_tabHeavy.ShowWindow(SW_HIDE);
	}

	if(index == 1)
	{
		m_tabEasy.ShowWindow(SW_HIDE);
		m_tabMedium.ShowWindow(SW_SHOW);
		m_tabHeavy.ShowWindow(SW_HIDE);
	}

	if(index == 2)
	{
		m_tabEasy.ShowWindow(SW_HIDE);
		m_tabMedium.ShowWindow(SW_HIDE);
		m_tabHeavy.ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}
