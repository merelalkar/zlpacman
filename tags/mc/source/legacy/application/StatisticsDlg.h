#pragma once
#include "StatisticTab.h"
#include "afxcmn.h"


// CStatisticsDlg dialog

class CStatisticsDlg : public CDialog
{
	DECLARE_DYNAMIC(CStatisticsDlg)

public:
	CStatisticsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatisticsDlg();

// Dialog Data
	enum { IDD = IDD_STATISTICS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CTabCtrl m_tabCtrl;
	CStatisticTab m_tabEasy;
	CStatisticTab m_tabMedium;
	CStatisticTab m_tabHeavy;

	virtual BOOL OnInitDialog();	
	afx_msg void OnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};
