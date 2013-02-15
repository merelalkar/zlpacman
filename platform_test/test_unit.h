
#ifndef TEST_UNIT_H
#define TEST_UNIT_H

#include "../platform/platform_windows.h"

class TestUnit: public Pegas::IDrawable, public Pegas::Singleton<TestUnit>
{
public:
	TestUnit(HINSTANCE hInstance): 
	  Singleton(*this), m_hInstance(hInstance), m_isActive(false) {};
	virtual ~TestUnit() {};
	
	void init(HWND hWnd);
	void run();
	void cleanup();
	void resize(int width, int height);
	void activate(bool bActive) { m_isActive = bActive; };

	virtual void draw(Pegas::GrafManager& context);

private:
	TestUnit(const TestUnit& src);
	TestUnit& operator=(const TestUnit& src);

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
};

#endif