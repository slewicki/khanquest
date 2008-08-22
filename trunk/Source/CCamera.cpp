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
	PROFILE("CCamera::InitCamera(float, float)");
	m_vPos.fX = fStartPosX;
	m_vPos.fY = fStartPosY;
	m_vVel.fX = 0;
	m_vVel.fY = 0;
	m_rScreenArea.left = (int)fStartPosX;
	m_rScreenArea.right = m_rScreenArea.left + 800;
	m_rScreenArea.top = (int)fStartPosY;
	m_rScreenArea.bottom = m_rScreenArea.top + 455;
	STOP("CCamera::InitCamera(float, float)");
}



tVector2D CCamera::TransformToScreen(tVector2D vWorldPos)
{
	PROFILE("CCamera::TransformToScreen(tVector2D)");
	tVector2D vScreenPos = vWorldPos - m_vPos;
	STOP("CCamera::TransformToScreen(tVector2D)");
	return vScreenPos;
}

tVector2D CCamera::TransformToScreen(float fWorldPosX, float fWorldPosY)
{
	PROFILE("CCamera::TransformToScreen(float, float)");
	tVector2D vScreenPos = {fWorldPosX - m_vPos.fX, fWorldPosY - m_vPos.fY};
	STOP("CCamera::TransformToScreen(float, float)");
	return vScreenPos;
}

POINT CCamera::TransformToScreen(int nWorldPosX, int nWorldPosY)
{
	PROFILE("CCamera::TransformToScreen(int, int)");
	POINT ptScreenPos = {nWorldPosX - (int)m_vPos.fX, nWorldPosY - (int)m_vPos.fY};
	STOP("CCamera::TransformToScreen(int, int)");
	return ptScreenPos;
}

POINT CCamera::TransformToScreen(POINT ptWorldPos)
{
	PROFILE("CCamera::TransformToScreen(POINT)");
	POINT ptScreenPos = {ptWorldPos.x - (int)m_vPos.fX, ptWorldPos.y - (int)m_vPos.fY};
	STOP("CCamera::TransformToScreen(POINT)");
	return ptScreenPos;
}

tVector2D CCamera::TransformToGlobal(tVector2D vCamPos)
{
	PROFILE("CCamera::TransformToGlobal(tVector2D)");
	tVector2D vGlobalPos = vCamPos + m_vPos;
	STOP("CCamera::TransformToGlobal(tVector2D)");
	return vGlobalPos;
}

tVector2D CCamera::TransformToGlobal(float fCamPosX, float fCamPosY)
{
	PROFILE("CCamera::TransformToGlobal(float, float)");
	tVector2D vGlobalPos = {fCamPosX + m_vPos.fX, fCamPosY + m_vPos.fY};
	STOP("CCamera::TransformToGlobal(float, float)");
	return vGlobalPos;
}

POINT CCamera::TransformToGlobal(int nCamPosX, int nCamPosY)
{
	PROFILE("CCamera::TransformToGlobal(int, int)");
	POINT ptGlobalPos = {nCamPosX + (int)m_vPos.fX, nCamPosY + (int)m_vPos.fY};
	STOP("CCamera::TransformToGlobal(int, int)");
	return ptGlobalPos;
}

POINT CCamera::TransformToGlobal(POINT ptCamPos)
{
	PROFILE("CCamera::TransformToGlobal(POINT)");
	POINT ptGlobalPos = {ptCamPos.x + (int)m_vPos.fX, ptCamPos.y + (int)m_vPos.fY};
	STOP("CCamera::TransformToGlobal(POINT)");
	return ptGlobalPos;
}

void CCamera::Update(float fElapsedTime)
{
	PROFILE("CCamera::Update(float)");
	int nMapHeight = CTileEngine::GetInstance()->GetMapHeight();
	int nMapWidth = CTileEngine::GetInstance()->GetMapWidth();
	int nTileWidth = CTileEngine::GetInstance()->GetTileWidth();
	int nTileHeight = CTileEngine::GetInstance()->GetTileHeight();
	// Keep camera on map
	//------------------------------------------------------------
	if(m_vPos.fY >  (float)((nMapHeight * nTileHeight)-227))
	{
		m_vVel.fY = 0.f;
		m_vPos.fY = (float)((nMapHeight * nTileHeight)-227);
	}
	else if(m_vPos.fY < -(nMapWidth * nTileHeight)+455)
	{
		m_vVel.fY = 0.f;
		m_vPos.fY = - (float)((nMapWidth * nTileHeight)+455);
	}

	if(m_vPos.fX < -400.f)
	{
		m_vVel.fX = 0.f;
		m_vPos.fX = -400.f;
	}
	else if((m_vPos.fX)+400.f >  (float)(nMapHeight * nTileWidth))
	{
		m_vVel.fX = 0.f;
		m_vPos.fX =  (float)(nMapHeight * nTileWidth)-400.f;
	}
	if((m_vPos.fX)+400.f >  (float)(nMapWidth  * nTileWidth))
	{
		m_vVel.fX = 0;
		m_vPos.fX =  (float)(nMapWidth * nTileWidth)-400.f;
	}
	//------------------------------------------------------------


	m_vPos.fX += m_vVel.fX*fElapsedTime;
	m_vPos.fY += m_vVel.fY*fElapsedTime;
	m_rScreenArea.left = (int)m_vPos.fX;
	m_rScreenArea.right = m_rScreenArea.left + 800;
	m_rScreenArea.top = (int)m_vPos.fY;
	m_rScreenArea.bottom = m_rScreenArea.top + 600;

	
	STOP("CCamera::Update(float)");
}


bool CCamera::IsOnScreen(RECT rObjectArea)
{
	PROFILE("CCamera::IsOnScreen(RECT)");
	RECT rBuffer;

	if(IntersectRect(&rBuffer, &m_rScreenArea, &rObjectArea))
	{
		STOP("CCamera::IsOnScreen(RECT)");
		return true;
	}
	else
	{
		STOP("CCamera::IsOnScreen(RECT)");
		return false;
	}
}

void CCamera::CenterCamera(int nPosX, int nPosY)
{
	PROFILE("CCamera::CenterCamera(int, int)");
	m_vPos.fX = (float)(nPosX - 400);
	m_vPos.fY = (float)(nPosY - 225);
	STOP("CCamera::CenterCamera(int, int)");
}
