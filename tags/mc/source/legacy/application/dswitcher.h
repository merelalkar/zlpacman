#pragma once


// CDifficutlySwitcher dialog

class CDifficutlySwitcher : public CDialog
{
	DECLARE_DYNAMIC(CDifficutlySwitcher)

public:
	enum 
	{
		k_difficutlyLevelEasy = 0,
		k_difficutlyLevelMedium = 1,
		k_difficutlyLevelHeavy = 2
	};

public:
	CDifficutlySwitcher(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDifficutlySwitcher();

// Dialog Data
	enum { IDD = IDD_DIFFICUTLY_SWITCHER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_difficutlyLevel;
};
