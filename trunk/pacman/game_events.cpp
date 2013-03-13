#include "stdafx.h"
#include "game_events.h"

using namespace Pegas;

const EventType Event_ChangeDirection::k_type = "Event_ChangeDirection";
const EventType Event_CancelChangingDirection::k_type = "Event_CancelChangingDirection";
const EventType Event_DirectionChanged::k_type = "Event_DirectionChanged";
const EventType Event_CharacterStopped::k_type = "Event_CharacterStopped";
const EventType Event_CharacterMoveOn::k_type = "Event_CharacterMoveOn";
const EventType Event_EnableCharacterControl::k_type = "Event_EnableCharacterControl";
const EventType Event_DisableCharacterControl::k_type = "Event_DisableCharacterControl";
const EventType Event_PacmanSwallowedPill::k_type = "Event_PacmanSwallowedPill";
const EventType Event_PacmanDeath::k_type = "Event_PacmanDeath";
const EventType Event_PacmanDeathComplete::k_type = "Event_PacmanDeathComplete";
const EventType Event_GUI_ButtonClick::k_type = "Event_GUI_ButtonClick";
const EventType Event_GUI_StartFadein::k_type = "Event_GUI_StartFadein";
const EventType Event_GUI_StartFadeout::k_type = "Event_GUI_StartFadeout";
const EventType Event_GUI_FadeinComplete::k_type = "Event_GUI_FadeinComplete";
const EventType Event_GUI_FadeoutComplete::k_type = "Event_GUI_FadeoutComplete";
const EventType Event_Game_ChangeState::k_type = "Event_Game_ChangeState";
const EventType Event_Game_ForwardToState::k_type = "Event_Game_ForwardToState";
const EventType Event_Game_BackwardToPreviousState::k_type = "Event_Game_BackwardToPreviousState";

const EventType Event_HUD_LevelChanged::k_type = "Event_HUD_LevelChanged";
const EventType Event_HUD_ScoresChanged::k_type = "Event_HUD_ScoresChanged";
const EventType Event_HUD_LivesChanged::k_type = "Event_HUD_LivesChanged";

const EventType Event_Game_Pause::k_type = "Event_Game_Pause";
