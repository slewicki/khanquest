/////////////////////////////////////////////
//	File	:	"CBase.cpp"
//
//	Author	:	Sean Hamstra (SH)
//
//	Purpose	:	The base object for our class hierarchy
//				and implements our base interface.
//////////////////////////////////////////////
#include "CBase.h"
#include "CSGD_TextureManager.h"

CBase::CBase(void)
{
	PROFILE("CBase::CBase()");
	m_unRefCount = 1;	//	Always start with a reference to yourself
	SetPosX( 0.0f );
	SetPosY( 0.0f );
	SetVelX( 1.0f );
	SetVelY( 1.0f );
	STOP("CBase::CBase()");
}


bool CBase::CheckCollisions(CBase* pBase)
{
	PROFILE("CBase::CheckCollisions(CBase*)");
	STOP("CBase::CBase()");
	return false;
}

CBase::~CBase(void)
{
}

void CBase::Update(float fElapsedTime)
{
	//SetPosX( GetPosX() + (GetVelX() * fElapsedTime));
	//SetPosY( GetPosY() + (GetVelY() * fElapsedTime));
}

void CBase::Render(float fElapsedTime)
{
	PROFILE("CBase::Render(float)");
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	STOP("CBase::CBase()");
}
