#include "stdafx.h"
#include "game_events.h"

namespace Pegas
{
	const EventType Event_Actor_CreateAsteroid::k_type = "Event_Actor_CreateAsteroid";
	const EventType Event_Actor_CreateShatter::k_type = "Event_Actor_CreateShatter";
	const EventType Event_Actor_CreateBullet::k_type = "Event_Actor_CreateBullet";
	const EventType Event_Actor_CreateExplosion::k_type = "Event_Actor_CreateExplosion";
	const EventType Event_Actor_Destroy::k_type = "Event_Actor_Destroy";

	const EventType Event_Player_EnableControl::k_type = "Event_Player_EnableControl";
	const EventType Event_Player_DisableControl::k_type = "Event_Player_DisableControl";
	const EventType Event_Player_RotateLeft::k_type = "Event_Player_RotateLeft";
	const EventType Event_Player_RotateRight::k_type = "Event_Player_RotateRight";
	const EventType Event_Player_Thrust::k_type = "Event_Player_Thrust";
	const EventType Event_Player_Fire::k_type = "Event_Player_Fire";
	const EventType Event_Player_Stop_RotateLeft::k_type = "Event_Player_Stop_RotateLeft";
	const EventType Event_Player_Stop_RotateRight::k_type = "Event_Player_Stop_RotateRight";
	const EventType Event_Player_Stop_Thrust::k_type = "Event_Player_Stop_Thrust";
	const EventType Event_Player_Stop_Fire::k_type = "Event_Player_Stop_Fire";
}