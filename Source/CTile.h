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

class CUnit;
enum TileType { PLAIN, MOUNTAIN, FOREST, SHALLOW_WATER, DEEP_WATER };

class CTile
{
public:


	TileType nType;			//Used to determine Unit bonuses based on terrain
	POINT ptTileLoc;		//Used to store location based on the ***pTileArray
	POINT ptPos;			//Used to store tileset image location
	bool bIsPlayerSpawn;	//Used to check if tile has a player spawn point
	bool bIsEnemySpawn;		//Used to check if tile has an enemy spawn point
	bool bIsCollision;		//Used to check if tile has collision
	POINT ptLocalAnchor;	//Used to determine the center of the tile based on world space
	bool bIsOccupied;		//Used to see if there is a player or enemy unit
	bool bIsVisible;		//Used to see if the tile had been explored
	CUnit* pUnit;			//Used to allow the tile to know what type of unit is occupying it
};	

#endif