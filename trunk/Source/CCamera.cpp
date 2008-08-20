//////////////////////////////////////////////////////
//	File	:	"CCamera.cpp"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	The base object for our class hierarchy
//				and implements our base interface.
//////////////////////////////////////////////////////
#include "CCamera.h"
#include "CTileEngine.h"


void CCamera::InitCamera(float fStartPosX, float fStartPosY)
{
	m_vPos.fX = fStartPosX;
	m_vPos.fY = fStartPosY;
	m_vVel.fX = 0;
	m_vVel.fY = 0;
	m_rScreenArea.left = (int)fStartPosX;
	m_rScreenArea.right = m_rScreenArea.left + 800;
	m_rScreenArea.top = (int)fStartPosY;
	m_rScreenArea.bottom = m_rScreenArea.top + 455;
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

tVector2D CCamera::TransformToGlobal(tVector2D vCamPos)
{
	tVector2D vGlobalPos = vCamPos + m_vPos;
	return vGlobalPos;
}

tVector2D CCamera::TransformToGlobal(float fCamPosX, float fCamPosY)
{
	tVector2D vGlobalPos = {fCamPosX + m_vPos.fX, fCamPosY + m_vPos.fY};
	return vGlobalPos;
}

POINT CCamera::TransformToGlobal(int nCamPosX, int nCamPosY)
{
	POINT ptGlobalPos = {nCamPosX + (int)m_vPos.fX, nCamPosY + (int)m_vPos.fY};
	return ptGlobalPos;
}

POINT CCamera::TransformToGlobal(POINT ptCamPos)
{
	POINT ptGlobalPos = {ptCamPos.x + (int)m_vPos.fX, ptCamPos.y + (int)m_vPos.fY};
	return ptGlobalPos;
}

void CCamera::Update(float fElapsedTime)
{
	int nMapHeight = CTileEngine::GetInstance()->GetMapHeight();
	int nMapWidth = CTileEngine::GetInstance()->GetMapWidth();
	int nTileWidth = CTileEngine::GetInstance()->GetTileWidth();
	int nTileHeight = CTileEngine::GetInstance()->GetTileHeight();
	// Keep camera on map
	//------------------------------------------------------------
	if(m_vPos.fY > (nMapHeight * nTileHeight)-227)
	{
		m_vVel.fY = 0;
		m_vPos.fY = (nMapHeight * nTileHeight)-227;
	}
	else if(m_vPos.fY < -(nMapWidth * nTileHeight)+455)
	{
		m_vVel.fY = 0;
		m_vPos.fY = -(nMapWidth * nTileHeight)+455;
	}

	if(m_vPos.fX < -400.f)
	{
		m_vVel.fX = 0;
		m_vPos.fX = -400.f;
	}
	else if((m_vPos.fX)+400 > (nMapHeight * nTileWidth))
	{
		m_vVel.fX = 0;
		m_vPos.fX = (nMapHeight * nTileWidth)-400;
	}
	if((m_vPos.fX)+400 > (nMapWidth  * nTileWidth))
	{
		m_vVel.fX = 0;
		m_vPos.fX = (nMapWidth * nTileWidth)-400;
	}
	//------------------------------------------------------------


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

	if(IntersectRect(&rBuffer, &m_rScreenArea, &rObjectArea))
		return true;
	else
		return false;
}
