#include "CHealthBar.h"

CHealthBar::CHealthBar()
{
	m_currColor = COLOR_DARKGREEN;
	m_pD3D = CSGD_Direct3D::GetInstance();
}
CHealthBar::~CHealthBar()
{

}

void CHealthBar::SetBarColor(int nPercentHealth)
{
	if ( nPercentHealth <= 20 && m_currColor != COLOR_RED )
		m_currColor = COLOR_RED;
	else if ( nPercentHealth <= 40 && nPercentHealth > 20 && m_currColor != COLOR_ORANGE )
		m_currColor = COLOR_ORANGE;
	else if ( nPercentHealth <= 60 && nPercentHealth > 40 && m_currColor != COLOR_YELLOW )
		m_currColor = COLOR_YELLOW;
	else if ( nPercentHealth <= 80 && nPercentHealth > 60 && m_currColor != COLOR_LIGHTGREEN )
		m_currColor = COLOR_LIGHTGREEN;
	else if ( nPercentHealth > 80 && m_currColor != COLOR_DARKGREEN ) 
		m_currColor = COLOR_DARKGREEN;
}

void CHealthBar::DamageHealth(int nPercentHealth)
{
	if(m_nHealth > 0 )
		m_nHealth -= nPercentHealth;
	
	SetBarColor(m_nHealth);
}

void CHealthBar::Render(RECT toDraw)
{	
	toDraw.right;// += (m_nHealth - 60);
	m_pD3D->DrawRect(toDraw, m_currColor);
	m_pD3D->DrawPrimitiveRect(toDraw, D3DCOLOR_ARGB(255,0,0,0));
}
