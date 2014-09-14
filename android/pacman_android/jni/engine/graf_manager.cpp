
#include "engine.h"
using namespace Pegas;


void GrafManager::resetLayouts()
{
	m_layouts.clear();
}

void GrafManager::addLayout(IDrawable* layout)
{
	m_layouts.push_back(layout);	
}

void GrafManager::removeLayout(IDrawable* layout)
{
	DrawableLayoutsList::iterator it = std::find(m_layouts.begin(), 
		m_layouts.end(), layout);

	if(it != m_layouts.end())
	{
		m_layouts.erase(it);
	}
}