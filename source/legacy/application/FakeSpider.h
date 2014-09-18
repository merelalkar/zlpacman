// FakeSpider.h : main header file for the FakeSpider application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "../engine/engine.h"
#include "../platform/platform_windows.h"


// CFakeSpiderApp:
// See FakeSpider.cpp for the implementation of this class
//

class CFakeSpiderApp : public CWinApp
{
public:
	CFakeSpiderApp();


// Overrides
public:
	virtual BOOL InitInstance();

	Pegas::OGLGrafManager&	getGrafManager()  { return m_grafManager; };
	Pegas::GUIManager&      getGUIManager()  { return m_GUIManager; };
	Pegas::AnimationManager&	getAnimationManager()  { return m_animationManager; };
	Pegas::WinMMSoundManager&	getWinMMSoundManager() { return m_soundManager; };
	Pegas::WindowsOSUtils&      getWindowsOSUtilsManager() { return  m_OSUtilsManager; };

	Pegas::TextureResourceManager&  getTextureResourceManager() { return m_textureResourceManager; };
	Pegas::SoundResourceManager&    getSoundResourceManager()  { return m_soundResourceManager; };
	Pegas::StringResourceManager&   getStringResourceManager() { return m_stringResourceManager; };
	Pegas::FontResourceManager&     getFontResourceManager()  { return m_fontResourceManager; };

	virtual BOOL OnIdle(LONG lCount);
	void updateAnimation();
		

// Implementation
	DECLARE_MESSAGE_MAP()

private:
	void registerResources();
	void destroyResources();

	Pegas::OGLGrafManager		m_grafManager;
	Pegas::GUIManager           m_GUIManager;
	Pegas::AnimationManager		m_animationManager;
	Pegas::WinMMSoundManager	m_soundManager;
	Pegas::WindowsOSUtils       m_OSUtilsManager;

	Pegas::TextureResourceManager  m_textureResourceManager;
	Pegas::SoundResourceManager    m_soundResourceManager;
	Pegas::StringResourceManager   m_stringResourceManager;
	Pegas::FontResourceManager     m_fontResourceManager;
	virtual int ExitInstance();
};

extern CFakeSpiderApp theApp;