#ifndef PEGAS_FAKESPIDER_GAME_COMMAND_H
#define PEGAS_FAKESPIDER_GAME_COMMAND_H

#pragma once

namespace Pegas
{
	class GameCommand
	{
	public:
		virtual ~GameCommand() {};
		virtual void undo() = 0;
	};

	class ComplexGameCommand: public GameCommand
	{
	public:
		virtual ~ComplexGameCommand() { deleteCommands(); };
		void addCommand(GameCommand* command);
		virtual void undo();
	private:
		void deleteCommands();

		std::list<GameCommand*> m_commandList;
	};
}

#endif //PEGAS_FAKESPIDER_GAME_COMMAND_H