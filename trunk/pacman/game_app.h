#pragma once
#include "platform_windows.h"

namespace Pegas
{

	class GameApplication: public Singleton<GameApplication>, public IPlatformContext
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

		void processInput(UINT message, WPARAM wParam, LPARAM lParam);

	public:

		virtual void addKeyboardController(IKeyboardController* controller);
		virtual void removeKeyboardController(IKeyboardController* controller);
		virtual void addMouseController(IMouseController* controller);
		virtual void removeMouseController(IMouseController* controller);
		
		virtual ProcessHandle attachProcess(ProcessPtr process);

		virtual void addGameState(GameStatePtr state);
		virtual void removeState(GameStateID id);
		virtual void changeState(GameStateID newStateId);
		virtual void forwardToState(GameStateID newStateId);
		virtual void backwardToPreviousState();

	private:
		std::set<IKeyboardController*> m_keyboardControllers;
		std::set<IMouseController*>	m_mouseControllers;

		std::map<GameStateID, GameStatePtr> m_statesMap;
		std::stack<GameStatePtr> m_statesStack;	

	private:
		GameApplication(const GameApplication& src);
		GameApplication& operator=(const GameApplication& src);

		void registerResources();
		
		HINSTANCE				m_hInstance;
		HWND					m_hWnd;
		bool                    m_isActive;
		bool					m_exitApplication;

		//graphics and sound
		OGLGrafManager			m_grafManager;
		WinMMSoundManager        m_soundPlayer;

		//resources
		StringResourceManager	m_stringManager;
		TextureResourceManager	m_textureManager;
		FontResourceManager		m_fontManager;
		SoundResourceManager    m_soundManager;

		//game loop
		EventManager			m_eventManager;
		ProcessManager			m_processManager;
	};

}

