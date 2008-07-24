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
	m_unRefCount = 1;	//	Always start with a reference to yourself
	SetPosX( 0.0f );
	SetPosY( 0.0f );
	SetVelX( 0.0f );
	SetVelY( 0.0f );
}


bool CBase::CheckCollisions(CBase* pBase)
{
	return false;
}

CBase::~CBase(void)
{
}

void CBase::Update(float fElapsedTime)
{
	SetPosX( GetPosX() + (GetVelX() * fElapsedTime));
	SetPosY( GetPosY() + (GetVelY() * fElapsedTime));
}

void CBase::Render(float fElapsedTime)
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
}
