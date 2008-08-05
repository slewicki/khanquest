#ifndef _CMOUSEMAP_H_
#define _CMOUSEMAP_H_

//////////////////////////////////////////////////////////
//	File:	"CMouseMap.h"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Class that stores Mouse Mapping values
//////////////////////////////////////////////////////////

#include <windows.h>

enum MouseMapDirection {MM_CENTER, MM_NE, MM_SE, MM_SW, MM_NW};

class CMouseMap
{
private:
	//X and Y size of the Mouse Map
	POINT m_ptSize;
	//Reference point for overlaying the mousemap on tile 0,0
	POINT m_ptRef;
	//Lookup array
	MouseMapDirection* m_mmLookUp;

public:
	POINT GetSize() { return m_ptSize; }
	POINT GetRef() { return m_ptRef; }
	MouseMapDirection* GetDirection() {return m_mmLookUp; }

	void SetSize(int nX, int nY);
	void SetRef(int nX, int nY);
	void SetDirection(MouseMapDirection mmDirection);

	void CreateLookUpArray(int nSize);
	void DeleteLookUpArray();
};

#endif