// FakeSpiderDoc.cpp : implementation of the CFakeSpiderDoc class
//

#include "stdafx.h"
#include "FakeSpider.h"

#include "FakeSpiderDoc.h"

#include "dswitcher.h"
#include "StatisticsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFakeSpiderDoc

IMPLEMENT_DYNCREATE(CFakeSpiderDoc, CDocument)

BEGIN_MESSAGE_MAP(CFakeSpiderDoc, CDocument)
	ON_COMMAND(IDMI_NEW_GAME, &CFakeSpiderDoc::OnNewGame)
	ON_COMMAND(IDMI_RESTART_GAME, &CFakeSpiderDoc::OnRestartGame)
	ON_COMMAND(IDMI_UNDO_TURN, &CFakeSpiderDoc::OnUndoTurn)
	ON_COMMAND(IDMI_TAKE_NEW_SERIES, &CFakeSpiderDoc::OnTakeNewSeries)
	ON_COMMAND(IDMI_SHOW_VALID_TURN, &CFakeSpiderDoc::OnShowValidTurn)
	ON_COMMAND(IDMI_DIFFICUTLY_LEVEL, &CFakeSpiderDoc::OnDifficutlyLevel)
	ON_COMMAND(IDMI_STATICTICS, &CFakeSpiderDoc::OnStatictics)
	ON_COMMAND(IDMI_GAME_SETTINGS, &CFakeSpiderDoc::OnGameSettings)
	ON_COMMAND(IDMI_SAVE_GAME, &CFakeSpiderDoc::OnSaveGame)
	ON_COMMAND(IDMI_OPEN_SAVED_GAME, &CFakeSpiderDoc::OnOpenSavedGame)
	ON_COMMAND(IDMI_TAKE_CARDS, &CFakeSpiderDoc::OnTakeCards)
	ON_UPDATE_COMMAND_UI(IDMI_RESTART_GAME, &CFakeSpiderDoc::OnUpdateRestartGame)
	ON_UPDATE_COMMAND_UI(IDMI_UNDO_TURN, &CFakeSpiderDoc::OnUpdateUndoTurn)
	ON_UPDATE_COMMAND_UI(IDMI_TAKE_NEW_SERIES, &CFakeSpiderDoc::OnUpdateTakeNewSeries)
	ON_UPDATE_COMMAND_UI(IDMI_TAKE_CARDS, &CFakeSpiderDoc::OnUpdateTakeCards)
	ON_UPDATE_COMMAND_UI(IDMI_SHOW_VALID_TURN, &CFakeSpiderDoc::OnUpdateShowValidTurn)
	ON_UPDATE_COMMAND_UI(IDMI_SAVE_GAME, &CFakeSpiderDoc::OnUpdateSaveGame)
	ON_UPDATE_COMMAND_UI(IDMI_OPEN_SAVED_GAME, &CFakeSpiderDoc::OnUpdateOpenSavedGame)
END_MESSAGE_MAP()


// CFakeSpiderDoc construction/destruction

CFakeSpiderDoc::CFakeSpiderDoc()	
{
	// TODO: add one-time construction code here
	m_currentDifficutlyLevel = Pegas::SpiderGame::k_difficultyLevelEasy;	
}

CFakeSpiderDoc::~CFakeSpiderDoc()
{
}

BOOL CFakeSpiderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}




// CFakeSpiderDoc serialization

void CFakeSpiderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CFakeSpiderDoc diagnostics

#ifdef _DEBUG
void CFakeSpiderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFakeSpiderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFakeSpiderDoc commands

void CFakeSpiderDoc::OnNewGame()
{
	// TODO: Add your command handler code here
	if(m_spiderGameController.playerAlreadyHasMadeTurn())
	{
		int result = AfxMessageBox(IDS_MESSAGE_BEGIN_NEW_GAME, MB_YESNO|MB_ICONQUESTION);
		if(result == IDYES)
		{
			m_spiderGameController.beginNewGame(m_currentDifficutlyLevel);
		}
	}else
	{
		m_spiderGameController.beginNewGame(m_currentDifficutlyLevel);
	}
}

void CFakeSpiderDoc::OnRestartGame()
{
	// TODO: Add your command handler code here
	if(m_spiderGameController.playerAlreadyHasMadeTurn())
	{
		int result = AfxMessageBox(IDS_MESSAGE_RESTART_CURRENT_GAME, MB_YESNO|MB_ICONQUESTION);
		if(result == IDYES)
		{
			m_spiderGameController.restartCurrentGame();
		}
	}else
	{
		m_spiderGameController.restartCurrentGame();
	}
}


void CFakeSpiderDoc::OnUndoTurn()
{
	// TODO: Add your command handler code here
	m_spiderGameController.undoLastTurn();
}


void CFakeSpiderDoc::OnTakeNewSeries()
{
	// TODO: Add your command handler code here
	m_spiderGameController.passNewSeries();
}

void CFakeSpiderDoc::OnShowValidTurn()
{
	// TODO: Add your command handler code here
	m_spiderGameController.showValidTurn();
}

void CFakeSpiderDoc::OnDifficutlyLevel()
{
	// TODO: Add your command handler code here
	int result = IDNO;
	if(m_spiderGameController.playerAlreadyHasMadeTurn())
	{
		result = AfxMessageBox(IDS_MESSAGE_SAVE_GAME, MB_YESNOCANCEL|MB_ICONQUESTION);
		if(result == IDYES)
		{
			saveGame();
		}
	}

	if(result != IDCANCEL)
	{
		chooseDifficutlyLevel();
	}
}

void CFakeSpiderDoc::OnStatictics()
{
	// TODO: Add your command handler code here
	//добавить программный код позже
}

void CFakeSpiderDoc::OnGameSettings()
{
	// TODO: Add your command handler code here
	//добавить программный код позже
}

void CFakeSpiderDoc::OnSaveGame()
{
	// TODO: Add your command handler code here
	saveGame();
}

void CFakeSpiderDoc::OnOpenSavedGame()
{
	// TODO: Add your command handler code here
	loadGame();
}

void CFakeSpiderDoc::OnTakeCards()
{
	// TODO: Add your command handler code here
	m_spiderGameController.passNewSeries();
}

void CFakeSpiderDoc::chooseDifficutlyLevel()
{
	CDifficutlySwitcher dlg;
	if(dlg.DoModal() == IDOK)
	{
		switch(dlg.m_difficutlyLevel)
		{
		case CDifficutlySwitcher::k_difficutlyLevelEasy:
			
			m_currentDifficutlyLevel = Pegas::SpiderGame::k_difficultyLevelEasy;
			break;
		
		case CDifficutlySwitcher::k_difficutlyLevelMedium:

			m_currentDifficutlyLevel = Pegas::SpiderGame::k_difficultyLevelMedium;
			break;

		case CDifficutlySwitcher::k_difficutlyLevelHeavy:

			m_currentDifficutlyLevel = Pegas::SpiderGame::k_difficultyLevelHeavy;
			break;

		default:
			break;
		}

		m_spiderGameController.beginNewGame(m_currentDifficutlyLevel);
	}
}

void CFakeSpiderDoc::saveGame()
{
	//добавить программный код позже
}

void CFakeSpiderDoc::loadGame()
{
	//добавить программный код позже
}

void CFakeSpiderDoc::OnUpdateRestartGame(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//Недоступен если игра не начата
	pCmdUI->Enable(m_spiderGameController.getGameState() != Pegas::SpiderGame::k_gameStateUnknown);
}

void CFakeSpiderDoc::OnUpdateUndoTurn(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//Недоступен, если очередь сделанных ходов пуста
	pCmdUI->Enable(m_spiderGameController.hasCommands());
}

void CFakeSpiderDoc::OnUpdateTakeNewSeries(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//Недоступен, если игра не начата
	bool bGameHasBegan = m_spiderGameController.getGameState() != Pegas::SpiderGame::k_gameStateUnknown;
	//Недоступен если разданы все доступные ряды
	bool bResiesIsRemain = m_spiderGameController.getRemainingSeries() > 0;

	pCmdUI->Enable(bGameHasBegan && bResiesIsRemain);
}

void CFakeSpiderDoc::OnUpdateTakeCards(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//Недоступен, если игра не начата
	bool bGameHasBegan = m_spiderGameController.getGameState() != Pegas::SpiderGame::k_gameStateUnknown;
	//Недоступен если разданы все доступные ряды
	bool bResiesIsRemain = m_spiderGameController.getRemainingSeries() > 0;

	pCmdUI->Enable(bGameHasBegan && bResiesIsRemain);
}

void CFakeSpiderDoc::OnUpdateShowValidTurn(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//Недоступен, если игра не начата
	pCmdUI->Enable(m_spiderGameController.getGameState() != Pegas::SpiderGame::k_gameStateUnknown);
}

void CFakeSpiderDoc::OnUpdateSaveGame(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//недоступен, если игра не начата
	pCmdUI->Enable(m_spiderGameController.getGameState() != Pegas::SpiderGame::k_gameStateUnknown);
}

void CFakeSpiderDoc::OnUpdateOpenSavedGame(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//не доступен, если нет файла сохранения
	//пока ставим заглушку
	pCmdUI->Enable(FALSE);
}



BOOL CFakeSpiderDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// TODO: Add your specialized code here and/or call the base class
	int result = IDNO;
	if(m_spiderGameController.getGameState() != Pegas::SpiderGame::k_gameStateUnknown)
	{
		result = AfxMessageBox(IDS_MESSAGE_SAVE_GAME, MB_YESNOCANCEL|MB_ICONQUESTION);
		if(result == IDYES)
		{
			saveGame();
		}
	}

	return (result != IDCANCEL);
}

void CFakeSpiderDoc::resizeGameBoard(int cx, int cy)
{
	m_spiderGameController.resizeGameBoard(cx, cy);	
}