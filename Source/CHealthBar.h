//////////////////////////////////////////////////////
//	File	:	"CHealthBar.h"
//
//	Author	:	Tom (TI)
//
//	Purpose	:	Store all information and functionality
//				for the unit health bar.
//////////////////////////////////////////////////////
#ifndef _CHEALTHBAR_H_
#define _CHEALTHBAR_H_

#include "CSGD_Direct3D.h"
#include <windows.h>
#define	COLOR_DARKGREEN		D3DCOLOR_XRGB(0, 190, 0)
#define COLOR_LIGHTGREEN	D3DCOLOR_XRGB(0, 240, 0)
#define COLOR_YELLOW		D3DCOLOR_XRGB(255, 255, 0)
#define COLOR_ORANGE		D3DCOLOR_XRGB(255, 128, 0)
#define COLOR_RED			D3DCOLOR_XRGB(255, 0, 0)

class CHealthBar
{

private:
	int				m_nHealth;
	D3DCOLOR	  	m_currColor;
	RECT			m_rOutterRect;
	RECT			m_rHealthBar;
	CSGD_Direct3D*	m_pD3D;

	void					SetBarColor(int nPercentHealth);

public:


	CHealthBar();
	~CHealthBar();

	//////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//	Last Modified: August 04, 2008
	//  Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////
	void					SetHealth(int nHealth) { m_nHealth = nHealth; }

	//////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//	Last Modified: August 04, 2008
	//  Purpose: Modifies the specified type.
	//////////////////////////////////////////////////////////////////////////
	int						GetHealth()			   { return m_nHealth; }
	void					DamageHealth(int nPercentHealth);


	////////////////////////////////////////////////////////////////////////// 
	// Function: “Render” 
	// Last Modified: July 27, 2008
	// Purpose: Renders the animation
	////////////////////////////////////////////////////////////////////////// 
	void	Render(RECT toDraw);
};

#endif