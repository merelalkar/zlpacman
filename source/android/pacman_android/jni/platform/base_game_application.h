#pragma once

namespace Pegas
{
	class BaseGameApplication: public Singleton<BaseGameApplication>, public IPlatformContext, public IEventListener
	{
	public:
		BaseGameApplication():
		  Singleton(*this), m_isActive(false), m_exitApplication(false) {};
		virtual ~BaseGameApplication() {};

		virtual void init();
		bool run();
		void cleanup();
		void resize(int width, int height);
		void activate(bool bActive) { m_isActive = bActive; };

		void processInput();

		virtual void handleEvent(EventPtr evt);
		virtual ListenerType getListenerName() { return "GameApplication"; }

	public:

		virtual void addKeyboardController(IKeyboardController* controller);
		virtual void removeKeyboardController(IKeyboardController* controller);
		virtual void addMouseController(IMouseController* controller);
		virtual void removeMouseController(IMouseController* controller);

		virtual ProcessHandle attachProcess(ProcessPtr process);
		virtual void terminateProcess(const ProcessHandle& handle);

		virtual ISerializer* createFile(const String& fileName);
		virtual ISerializer* openFile(const String& fileName, uint32 mode);
		virtual void closeFile(ISerializer* file);

		virtual void addGameState(GameStatePtr state);
		virtual void removeState(GameStateID id);
		virtual void changeState(GameStateID newStateId);
		virtual void forwardToState(GameStateID newStateId);
		virtual void backwardToPreviousState();
		virtual void shutdownGame();

	private:
		std::set<IKeyboardController*> m_keyboardControllers;
		std::set<IMouseController*>	m_mouseControllers;

		std::map<GameStateID, GameStatePtr> m_statesMap;
		std::stack<GameStatePtr> m_statesStack;

		typedef SmartPointer<File> FilePtr;
		std::list<FilePtr> m_openedFiles;

	private:
		BaseGameApplication(const BaseGameApplication& src);
		BaseGameApplication& operator=(const BaseGameApplication& src);

		void _changeState(GameStateID newStateId);
		void _forwardToState(GameStateID newStateId);
		void _backwardToPreviousState();

		bool                    m_isActive;
		bool					m_exitApplication;

		//graphics, sound and tools
		GLESGraphManager			m_grafManager;
		SLESSoundManager       		m_soundPlayer;
		AndroidOSUtils				m_utils;

		//resources
		//StringResourceManager	m_stringManager;
		//TextureResourceManager	m_textureManager;
		//FontResourceManager		m_fontManager;
		//SoundResourceManager    m_soundManager;

		//game loop
		EventManager			m_eventManager;
		ProcessManager			m_processManager;
		MILLISECONDS			m_lastTime;
	};
}
