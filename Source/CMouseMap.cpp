//////////////////////////////////////////////////////////
//	File:	"CMouseMap.cpp"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Class that stores Mouse Mapping values
//////////////////////////////////////////////////////////

#include "CMouseMap.h"

void CMouseMap::SetSize(int nX, int nY)
{
	m_ptSize.x = nX;
	m_ptSize.y = nY;
}

void CMouseMap::SetRef(int nX, int nY)
{
	m_ptRef.x = nX;
	m_ptRef.y = nY;
}

void CMouseMap::SetDirection(MouseMapDirection mmDirection, int nLoc)
{
	m_mmLookUp[nLoc] = mmDirection;
}

void CMouseMap::CreateLookUpArray(int nSize)
{
	m_mmLookUp = NULL;
//	m_mmLookUp = new MouseMapDirection[nSize];
}

void CMouseMap::DeleteLookUpArray()
{
	if(m_mmLookUp)
		delete[] m_mmLookUp;
}