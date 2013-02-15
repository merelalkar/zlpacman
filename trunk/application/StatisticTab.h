#pragma once


// CStatisticTab dialog

class CStatisticTab : public CDialog
{
	DECLARE_DYNAMIC(CStatisticTab)

public:
	CStatisticTab(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatisticTab();

// Dialog Data
	enum { IDD = IDD_STATISTICS_TAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CBrush m_brushBackGround;
};
