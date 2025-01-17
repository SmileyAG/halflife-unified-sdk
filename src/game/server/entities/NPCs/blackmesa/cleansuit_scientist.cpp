/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// human scientist (passive lab worker)
//=========================================================

#include "cbase.h"
#include "talkmonster.h"
#include "defaultai.h"
#include "scripted.h"
#include "scientist.h"

//=======================================================
// Scientist
//=======================================================

class CCleansuitScientist : public CScientist
{
public:
	void Spawn() override;
	void Precache() override;

	void Heal() override;
};

LINK_ENTITY_TO_CLASS(monster_cleansuit_scientist, CCleansuitScientist);

//=========================================================
// Spawn
//=========================================================
void CCleansuitScientist::Spawn()
{
	SpawnCore("models/cleansuit_scientist.mdl", GetSkillFloat("cleansuit_scientist_health"sv));
}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CCleansuitScientist::Precache()
{
	PrecacheCore("models/cleansuit_scientist.mdl");
}

void CCleansuitScientist::Heal()
{
	if (!CanHeal())
		return;

	Vector target = m_hTargetEnt->pev->origin - pev->origin;
	if (target.Length() > 100)
		return;

	m_hTargetEnt->TakeHealth(GetSkillFloat("cleansuit_scientist_heal"sv), DMG_GENERIC);
	// Don't heal again for 1 minute
	m_healTime = gpGlobals->time + 60;
}

//=========================================================
// Dead Scientist PROP
//=========================================================
class CDeadCleansuitScientist : public CBaseMonster
{
public:
	void Spawn() override;
	int Classify() override { return CLASS_HUMAN_PASSIVE; }

	bool KeyValue(KeyValueData* pkvd) override;
	int m_iPose; // which sequence to display
	static const char* m_szPoses[9];
};
const char* CDeadCleansuitScientist::m_szPoses[] =
	{
		"lying_on_back",
		"lying_on_stomach",
		"dead_sitting",
		"dead_hang",
		"dead_table1",
		"dead_table2",
		"dead_table3",
		"scientist_deadpose1",
		"dead_against_wall"};

bool CDeadCleansuitScientist::KeyValue(KeyValueData* pkvd)
{
	if (FStrEq(pkvd->szKeyName, "pose"))
	{
		m_iPose = atoi(pkvd->szValue);
		return true;
	}

	return CBaseMonster::KeyValue(pkvd);
}
LINK_ENTITY_TO_CLASS(monster_cleansuit_scientist_dead, CDeadCleansuitScientist);

//
// ********** DeadScientist SPAWN **********
//
void CDeadCleansuitScientist::Spawn()
{
	PRECACHE_MODEL("models/cleansuit_scientist.mdl");
	SET_MODEL(ENT(pev), "models/cleansuit_scientist.mdl");

	pev->effects = 0;
	pev->sequence = 0;
	// Corpses have less health
	pev->health = 8; //GetSkillFloat("scientist_health"sv);

	m_bloodColor = BLOOD_COLOR_RED;

	if (pev->body == -1)
	{														 // -1 chooses a random head
		pev->body = RANDOM_LONG(0, NUM_SCIENTIST_HEADS - 1); // pick a head, any head
	}
	// Luther is black, make his hands black
	if (pev->body == HEAD_LUTHER)
		pev->skin = 1;
	else
		pev->skin = 0;

	pev->sequence = LookupSequence(m_szPoses[m_iPose]);
	if (pev->sequence == -1)
	{
		ALERT(at_console, "Dead scientist with bad pose\n");
	}

	//	pev->skin += 2; // use bloody skin -- UNDONE: Turn this back on when we have a bloody skin again!
	MonsterInitDead();
}

//=========================================================
// Sitting Scientist PROP
//=========================================================

class CSittingCleansuitScientist : public CSittingScientist // kdb: changed from public CBaseMonster so he can speak
{
public:
	void Spawn() override;
};

LINK_ENTITY_TO_CLASS(monster_sitting_cleansuit_scientist, CSittingCleansuitScientist);

//
// ********** Scientist SPAWN **********
//
void CSittingCleansuitScientist::Spawn()
{
	SpawnCore("models/cleansuit_scientist.mdl");
}
