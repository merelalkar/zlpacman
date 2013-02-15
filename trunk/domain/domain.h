#ifndef PEGAS_FAKESPIDER_DOMAIN_H
#define PEGAS_FAKESPIDER_DOMAIN_H
#pragma once

#include "../engine/engine.h"

namespace Pegas
{
	class GameCommand;
	class SeriesPassAnimator;
	class ShowValidTurnAnimator;
	class BuildedStackDeck;
	class CardColumn;
	class CardDeck;
	class CardStack;
	class PortableCardStack;
	class ScoreBoard;
	class GameBoard;
	class PlayingCard;
	class SpiderGame;

	typedef std::vector<PlayingCard*> CardList;
}
#include "game_resources.h"
#include "game_command.h"
#include "game_animations.h"
#include "builded_stack_deck.h"
#include "card_column.h"
#include "card_deck.h"
#include "card_stack.h"
#include "playing_card.h"
#include "portable_card_stack.h"
#include "score_board.h"
#include "game_board.h"
#include "spider_game.h"

#endif //PEGAS_FAKESPIDER_DOMAIN_H