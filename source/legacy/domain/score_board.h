#ifndef PEGAS_FAKESPIDER_SCORE_BOARD_H
#define PEGAS_FAKESPIDER_SCORE_BOARD_H
#pragma once

namespace Pegas
{
	class ScoreBoard: public ClickableGameObject
	{
	public:
		enum
		{
			k_initialScores = 500,
			k_scoreBoardWidth = 200,
			k_scoreBoardHeight = 96,
			k_textColor = 0xffffffff,
			k_borderColor = 0xff000000,
			k_fillColor = 0xff007f00,
			k_fontSize = 9
		};

		enum
		{
			k_scoresLabel = 0,
			k_scoresValue,
			k_turnsLabel,
			k_turnsValue
		};

		enum
		{
			k_left = 0,
			k_top
		};

		ScoreBoard();
		virtual ~ScoreBoard() {};

		virtual void draw(GrafManager& context);
		virtual void click(CURCOORD x, CURCOORD y);
		virtual void setLeft(CURCOORD left);
		virtual void setTop(CURCOORD top);

        void reset();
		void addScores(int scores);
		void removeScores(int scores);
		void addTurn();

	private:
		void recalkTextCoords();

		CURCOORD m_textCoords[4][2];
		
		static const CURCOORD k_scoreLabelRelativeLeft = 65;
		static const CURCOORD k_scoreLabelRelativeTop = 32;
		static const CURCOORD k_scoreValueRelativeLeft = 110;
		static const CURCOORD k_scoreValueRelativeTop = 32;
		static const CURCOORD k_turnLabelRelativeLeft = 52;
		static const CURCOORD k_turnLabelRelativeTop = 52;
		static const CURCOORD k_turnValueRelativeLeft = 110;
		static const CURCOORD k_turnValueRelativeTop = 52;
		
		
		int m_totalScores;
		int m_totalTurns;
	};

}

#endif //PEGAS_FAKESPIDER_SCORE_BOARD_H