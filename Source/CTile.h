#ifndef _CTILE_H_
#define _CTILE_H_

//////////////////////////////////////////////////////////
//	File:	"CTile.h"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Class to store tile values
//////////////////////////////////////////////////////////
#include <windows.h>
enum TileType { PLAIN, MOUNTAIN, FOREST, SHALLOW_WATER, DEEP_WATER };

class CTile
{
public:


	TileType nType;
	POINT ptTileLoc;
	POINT ptPos;
	bool bIsPlayerSpawn;
	bool bIsEnemySpawn;
	bool bIsCollision;
	POINT ptLocalAnchor;
	POINT ptGlobalAnchor;
	bool bIsOccupied;
	int nUnitIndex;
	CTile()
	{
		ptPos.x = ptPos.y = 0;
		nUnitIndex = -1;
	}
};	

#endif