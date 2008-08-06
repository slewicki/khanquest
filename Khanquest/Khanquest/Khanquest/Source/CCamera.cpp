#include "CCamera.h"




void CCamera::InitCamera(float fStartPosX, float fStartPosY)
{
	m_vPos.fX = fStartPosX;
	m_vPos.fY = fStartPosY;
	m_vVel.fX = 0;
	m_vVel.fY = 0;
	m_rScreenArea.left = (int)fStartPosX;
	m_rScreenArea.right = m_rScreenArea.left + 800;
	m_rScreenArea.top = (int)fStartPosY;
	m_rScreenArea.bottom = m_rScreenArea.bottom + 600;
}



tVector2D CCamera::TransformToScreen(tVector2D vWorldPos)
{
	tVector2D vScreenPos = vWorldPos - m_vPos;
	return vScreenPos;
}

tVector2D CCamera::TransformToScreen(float fWorldPosX, float fWorldPosY)
{
	tVector2D vScreenPos = {fWorldPosX - m_vPos.fX, fWorldPosY - m_vPos.fY};
	return vScreenPos;
}

POINT CCamera::TransformToScreen(int nWorldPosX, int nWorldPosY)
{
	POINT ptScreenPos = {nWorldPosX - (int)m_vPos.fX, nWorldPosY - (int)m_vPos.fY};
	return ptScreenPos;
}

POINT CCamera::TransformToScreen(POINT ptWorldPos)
{
	POINT ptScreenPos = {ptWorldPos.x - (int)m_vPos.fX, ptWorldPos.y - (int)m_vPos.fY};
	return ptScreenPos;
}

void CCamera::Update(float fElapsedTime)
{
	m_vPos.fX += m_vVel.fX*fElapsedTime;
	m_vPos.fY += m_vVel.fY*fElapsedTime;
	m_rScreenArea.left = (int)m_vPos.fX;
	m_rScreenArea.right = m_rScreenArea.left + 800;
	m_rScreenArea.top = (int)m_vPos.fY;
	m_rScreenArea.bottom = m_rScreenArea.top + 600;
}


bool CCamera::IsOnScreen(RECT rObjectArea)
{
	RECT rBuffer;
	return IntersectRect(&rBuffer, &m_rScreenArea, &rObjectArea);
}
