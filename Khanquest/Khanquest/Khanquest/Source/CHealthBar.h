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

	void					SetHealth(int nHealth) { m_nHealth = nHealth; }

	void					DamageHealth(int nPercentHealth);

	CHealthBar();
	~CHealthBar();

	void					Render(RECT toDraw);
};

#endif