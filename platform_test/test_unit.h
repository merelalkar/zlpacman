
#ifndef TEST_UNIT_H
#define TEST_UNIT_H

#include "../platform/platform_windows.h"

class TestUnit: public Pegas::IDrawable, public Pegas::Singleton<TestUnit>
{
public:
	TestUnit(HINSTANCE hInstance);
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

	Pegas::WindowsOSUtils			m_OSUtils;

	Pegas::OGLGrafManager			m_grafManager;
	Pegas::WinMMSoundManager        m_soundPlayer;
	Pegas::StringResourceManager	m_stringManager;
	Pegas::TextureResourceManager	m_textureManager;
	Pegas::FontResourceManager		m_fontManager;
	Pegas::SoundResourceManager     m_soundManager;

	Pegas::ProcessManager			m_processManager;
	Pegas::MILLISECONDS				m_lastTime;

	Pegas::ProcessPtr m_pingo;
	Pegas::SpriteParameters* m_animSprite;	
};

#endif