//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================

#pragma once

struct screenfade_t
{
	float fadeSpeed;					 // How fast to fade (tics / second) (+ fade in, - fade out)
	float fadeEnd;						 // When the fading hits maximum
	float fadeTotalEnd;					 // Total End Time of the fade (used for FFADE_OUT)
	float fadeReset;					 // When to reset to not fading (for fadeout and hold)
	byte fader, fadeg, fadeb, fadealpha; // Fade color
	int fadeFlags;						 // Fading flags
};
