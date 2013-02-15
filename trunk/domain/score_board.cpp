
#include "domain.h"
using namespace Pegas;

/*
   Согласно Страуструпу, константные члены класса следует сначала обьявлять как
   статические члены в классе (с инициализатором). А затем определять (один раз) в исходном файле, 
   без инициализатора.
   Лжет, собака - линковщик сообщает об избыточном определении. Достаточно обьявления + инициализации в классе
   Так что следующие определения закомментированы. Возможно другие компиляторы более придерживаються
   **СТАНДАРТА**
*/

/*
const CURCOORD ScoreBoard::k_scoreLabelRelativeLeft = 65;
const CURCOORD ScoreBoard::k_scoreLabelRelativeTop = 32;
const CURCOORD ScoreBoard::k_scoreValueRelativeLeft = 110;
const CURCOORD ScoreBoard::k_scoreValueRelativeTop = 32;
const CURCOORD ScoreBoard::k_turnLabelRelativeLeft = 52;
const CURCOORD ScoreBoard::k_turnLabelRelativeTop = 52;
const CURCOORD ScoreBoard::k_turnValueRelativeLeft = 110;
const CURCOORD ScoreBoard::k_turnValueRelativeTop = 52;
*/

ScoreBoard::ScoreBoard(): 
	m_totalScores(0), m_totalTurns(0)  
{
	setWidth(k_scoreBoardWidth);
	setHeight(k_scoreBoardHeight);

	recalkTextCoords();		
}

void ScoreBoard::draw(GrafManager& context)
{
	tchar buffer[10];

	context.drawRectangle(getLeft(), getTop(), getWidth(), getHeight(), 
		k_borderColor, k_fillColor);

	TextParameters tp;
	tp._font = k_fontScoreBoardText;
	tp._color = k_textColor;
		
	tp._left = m_textCoords[k_scoresLabel][k_left];
	tp._top = m_textCoords[k_scoresLabel][k_top];
	context.drawText(k_stringScoreBoardScores, tp);

	tp._left = m_textCoords[k_turnsLabel][k_left];
	tp._top = m_textCoords[k_turnsLabel][k_top];
	context.drawText(k_stringScoreBoardTurns, tp);

	tp._left = m_textCoords[k_scoresValue][k_left];
	tp._top = m_textCoords[k_scoresValue][k_top];
#ifdef _UNICODE
	context.drawText(_itow(m_totalScores, buffer, 10), tp);
#else
	context.drawText(_itoa(m_totalScores, buffer, 10), tp);
#endif

	tp._left = m_textCoords[k_turnsValue][k_left];
	tp._top = m_textCoords[k_turnsValue][k_top];
#ifdef _UNICODE
	context.drawText(_itow(m_totalTurns, buffer, 10), tp);
#else
	context.drawText(_itoa(m_totalTurns, buffer, 10), tp);
#endif
}

void ScoreBoard::click(CURCOORD x, CURCOORD y)
{
	SpiderGame& gameHost = SpiderGame::getInstance();
	gameHost.showValidTurn(); 
}

void ScoreBoard::reset()
{
	m_totalScores = k_initialScores;
	m_totalTurns = 0;
}

void ScoreBoard::addScores(int scores)
{
	m_totalScores+= scores;
}

void ScoreBoard::removeScores(int scores)
{
	m_totalScores-= scores;
}

void ScoreBoard::addTurn()
{
	m_totalTurns+= 1;
}

void ScoreBoard::setLeft(CURCOORD left)
{
	GameObject::setLeft(left);

	recalkTextCoords();
}

void ScoreBoard::setTop(CURCOORD top)
{
	GameObject::setTop(top);

	recalkTextCoords();
}

void ScoreBoard::recalkTextCoords()
{
	CURCOORD left = getLeft();
	CURCOORD top = getTop();

	m_textCoords[k_scoresLabel][k_left] = left + k_scoreLabelRelativeLeft;
	m_textCoords[k_scoresLabel][k_top] = top + k_scoreLabelRelativeTop;
	m_textCoords[k_scoresValue][k_left] = left + k_scoreValueRelativeLeft;
	m_textCoords[k_scoresValue][k_top] = top + k_scoreValueRelativeTop;
	m_textCoords[k_turnsLabel][k_left] = left + k_turnLabelRelativeLeft;
	m_textCoords[k_turnsLabel][k_top] = top + k_turnLabelRelativeTop;
	m_textCoords[k_turnsValue][k_left] = left + k_turnValueRelativeLeft;
	m_textCoords[k_turnsValue][k_top] = top + k_turnValueRelativeTop;
}