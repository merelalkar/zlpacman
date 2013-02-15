// FakeSpiderDoc.h : interface of the CFakeSpiderDoc class
//


#pragma once

#include "../engine/engine.h"
#include "../domain/domain.h"


class CFakeSpiderDoc : public CDocument
{
protected: // create from serialization only
	CFakeSpiderDoc();
	DECLARE_DYNCREATE(CFakeSpiderDoc)

// Attributes
public:	

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CFakeSpiderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNewGame();
	afx_msg void OnRestartGame();
	afx_msg void OnUndoTurn();
	afx_msg void OnTakeNewSeries();
	afx_msg void OnShowValidTurn();
	afx_msg void OnDifficutlyLevel();
	afx_msg void OnStatictics();
	afx_msg void OnGameSettings();
	afx_msg void OnSaveGame();
	afx_msg void OnOpenSavedGame();
	afx_msg void OnTakeCards();

	afx_msg void OnUpdateRestartGame(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUndoTurn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTakeNewSeries(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTakeCards(CCmdUI *pCmdUI);
	afx_msg void OnUpdateShowValidTurn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSaveGame(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOpenSavedGame(CCmdUI *pCmdUI);
	
	virtual BOOL CanCloseFrame(CFrameWnd* pFrame);

	void resizeGameBoard(int cx, int cy);

private:
	void chooseDifficutlyLevel();
	void saveGame();
	void loadGame();
		
	Pegas::SpiderGame m_spiderGameController;
	Pegas::SpiderGame::DifficultyLevel m_currentDifficutlyLevel;	
};


