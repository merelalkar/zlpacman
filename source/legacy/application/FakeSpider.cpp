// FakeSpider.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FakeSpider.h"
#include "MainFrm.h"

#include "FakeSpiderDoc.h"
#include "FakeSpiderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFakeSpiderApp

BEGIN_MESSAGE_MAP(CFakeSpiderApp, CWinApp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CFakeSpiderApp construction

CFakeSpiderApp::CFakeSpiderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance	
}


// The one and only CFakeSpiderApp object

CFakeSpiderApp theApp;


// CFakeSpiderApp initialization

BOOL CFakeSpiderApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CFakeSpiderDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CFakeSpiderView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	registerResources();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

void CFakeSpiderApp::registerResources()
{
	m_stringResourceManager.registerResource(Pegas::k_stringGameWindowCaption, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_MESSAGE_TITLE));
	m_stringResourceManager.registerResource(Pegas::k_stringSeriesPassImposible, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_MESSAGE_PASS_NEW_SERIES_IMPOSIBBLE));
	m_stringResourceManager.registerResource(Pegas::k_stringScoreBoardScores, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_LABEL_SCORES));
	m_stringResourceManager.registerResource(Pegas::k_stringScoreBoardTurns, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_LABEL_TURNS));
	m_stringResourceManager.registerResource(Pegas::k_stringYouWinThePrize, MAKE_INT_RESOURCE_CODE(m_hInstance, IDS_MESSAGE_YOU_WIN));
	m_stringResourceManager.loadAll();

	
	m_soundResourceManager.registerResource(Pegas::k_soundValidTurnExist, MAKE_INT_RESOURCE_CODE(m_hInstance, IDSOUND_SHOW_VALID_TURN));
	m_soundResourceManager.registerResource(Pegas::k_soundValidTurnAbsent, MAKE_INT_RESOURCE_CODE(m_hInstance, IDSOUND_NO_VALID_TURN));
	m_soundResourceManager.registerResource(Pegas::k_soundCardPassing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDSOUND_PASS_NEW_SERIES));
	//m_soundResourceManager.registerResource(Pegas::k_soundCardPassing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDR_WAVE2));
	m_soundResourceManager.registerResource(Pegas::k_soundYouAreWin, MAKE_INT_RESOURCE_CODE(m_hInstance, IDSOUND_YOU_WIN));
	m_soundResourceManager.registerResource(Pegas::k_soundTakeCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDSOUND_TAKE_CARD));
	m_soundResourceManager.registerResource(Pegas::k_soundReleaseCard, MAKE_INT_RESOURCE_CODE(m_hInstance, IDSOUND_RELEASE_CARD));
	m_soundResourceManager.loadAll();

	m_fontResourceManager.registerResource(Pegas::k_fontScoreBoardText, MAKE_FONT_RESOURCE_CODE(_T("Arial"), 13));
	m_fontResourceManager.registerResource(Pegas::k_fontYouAreWinMessage, MAKE_FONT_RESOURCE_CODE(_T("Arial"), 72));

	m_textureResourceManager.registerResource(Pegas::k_textureGameBoardFelt, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_FELT));
	//m_textureResourceManager.registerResource(Pegas::k_textureGameBoardFelt, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_BACK));
	m_textureResourceManager.registerResource(Pegas::k_textureColumnGag, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_GAG));
	m_textureResourceManager.registerResource(Pegas::k_textureCardBack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARDBACK));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamondAce, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_ACE));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_2, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_2));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_3, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_3));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_4, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_4));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_5, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_5));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_6, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_6));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_7, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_7));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_8, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_8));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_9, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_9));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamond_10, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_10));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamondJack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_JACK));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamondQueen, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_QUEEN));
	m_textureResourceManager.registerResource(Pegas::k_textureDeamondKing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_DIAMOND_KING));
	m_textureResourceManager.registerResource(Pegas::k_textureClubAce, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_ACE));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_2, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_2));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_3, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_3));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_4, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_4));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_5, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_5));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_6, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_6));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_7, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_7));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_8, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_8));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_9, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_9));
	m_textureResourceManager.registerResource(Pegas::k_textureClub_10, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_10));
	m_textureResourceManager.registerResource(Pegas::k_textureClubJack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_JACK));
	m_textureResourceManager.registerResource(Pegas::k_textureClubQueen, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_QUEEN));
	m_textureResourceManager.registerResource(Pegas::k_textureClubKing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_CLUB_KING));
	m_textureResourceManager.registerResource(Pegas::k_textureHeartAce, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_ACE));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_2, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_2));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_3, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_3));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_4, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_4));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_5, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_5));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_6, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_6));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_7, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_7));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_8, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_8));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_9, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_9));
	m_textureResourceManager.registerResource(Pegas::k_textureHeart_10, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_10));
	m_textureResourceManager.registerResource(Pegas::k_textureHeartJack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_JACK));
	m_textureResourceManager.registerResource(Pegas::k_textureHeartQueen, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_QUEEN));
	m_textureResourceManager.registerResource(Pegas::k_textureHeartKing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_HEART_KING));
	m_textureResourceManager.registerResource(Pegas::k_textureSpadeAce, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_ACE));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_2, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_2));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_3, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_3));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_4, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_4));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_5, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_5));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_6, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_6));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_7, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_7));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_8, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_8));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_9, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_9));
	m_textureResourceManager.registerResource(Pegas::k_textureSpade_10, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_10));
	m_textureResourceManager.registerResource(Pegas::k_textureSpadeJack, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_JACK));
	m_textureResourceManager.registerResource(Pegas::k_textureSpadeQueen, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_QUEEN));
	m_textureResourceManager.registerResource(Pegas::k_textureSpadeKing, MAKE_INT_RESOURCE_CODE(m_hInstance, IDB_CARD_SPADE_KING));
}

void CFakeSpiderApp::destroyResources()
{
	m_textureResourceManager.destroyAll();
	m_soundResourceManager.destroyAll();
	m_stringResourceManager.destroyAll();
	m_fontResourceManager.destroyAll();
}

// CFakeSpiderApp message handlers


BOOL CFakeSpiderApp::OnIdle(LONG lCount)
{
	// TODO: Add your specialized code here and/or call the base class
	m_grafManager.render();

	//данная конструкция необходима для того чтобы отобразить
	//окно выбора уровня сложности и начала новой игры
	//ПОСЛЕ того как, окно показываеться на экране
	//К сожалению найти подходящий обработчик или переопределяемую функцию
	//для цивилизованного решения проблемы найти не удалось
	//Все функции вызваються ДО появления окна на экране. 
	static bool doOnce = false;
	if(!doOnce)
	{
		m_pMainWnd->SendMessage(WM_COMMAND, IDMI_DIFFICUTLY_LEVEL, 0); 
		doOnce = true;
	}

	return CWinApp::OnIdle(lCount);
}

int CFakeSpiderApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	destroyResources();

	return CWinApp::ExitInstance();
}

void CFakeSpiderApp::updateAnimation()
{
	m_animationManager.updateAnimations();
	m_grafManager.render();
}
