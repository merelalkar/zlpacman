
#ifndef TEST_UNIT_H
#define TEST_UNIT_H

#include "platform_windows.h"

class GameApplication: public Pegas::Singleton<GameApplication>/*, public Pegas::IPlatformContext*/
{
public:
	GameApplication(HINSTANCE hInstance): 
	  Singleton(*this), m_hInstance(hInstance), m_isActive(false) {};
	virtual ~GameApplication() {};
	
	void init(HWND hWnd);
	void run();
	void cleanup();
	void resize(int width, int height);
	void activate(bool bActive) { m_isActive = bActive; };

	virtual void draw(Pegas::GrafManager& context);

	//TODO: implement functions
	/*
	void addKeyboardController(IKeyboardController* controller);
	void removeKeyboardController(IKeyboardController* controller);
	void addMouseController(IMouseController* controller);
	void removeMouseController(IMouseController* controller);*/

private:
	GameApplication(const GameApplication& src);
	GameApplication& operator=(const GameApplication& src);

	void registerResources();
	void playSound();

	HINSTANCE				m_hInstance;
	HWND					m_hWnd;
	bool                    m_isActive;

	Pegas::OGLGrafManager			m_grafManager;
	Pegas::WinMMSoundManager        m_soundPlayer;
	Pegas::StringResourceManager	m_stringManager;
	Pegas::TextureResourceManager	m_textureManager;
	Pegas::FontResourceManager		m_fontManager;
	Pegas::SoundResourceManager     m_soundManager;
	Pegas::EventManager				m_eventManager;
};

#endif