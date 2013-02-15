
#include "domain.h"
using namespace Pegas;

/****************************************************************************
               ComplexGameCommand class implementation
*****************************************************************************/

void ComplexGameCommand::addCommand(GameCommand* command)
{
	m_commandList.push_back(command);
}

void ComplexGameCommand::undo()
{
	for(std::list<GameCommand*>::iterator it = m_commandList.begin();
		it != m_commandList.end(); ++it)
	{
		(*it)->undo();
	}
}

void ComplexGameCommand::deleteCommands()
{
	for(std::list<GameCommand*>::iterator it = m_commandList.begin();
		it != m_commandList.end(); ++it)
	{
		delete (*it);
	}

	m_commandList.clear();
}