#ifndef _CTILEENGINE_H_
#define _CTILEENGINE_H_

//////////////////////////////////////////////////////////
//	File:	"CTileEngine.h"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: TileEngine
//////////////////////////////////////////////////////////
#include "CSGD_TextureManager.h"

#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>

#include "CTile.h"
#include "CMouseMap.h"
#include "CGDICanvas.h"

using std::iostream;
using std::string;
using std::ifstream;
using std::ofstream;


//enum TileType { PLAIN, MOUNTAIN, FOREST, SHALLOW_WATER, DEEP_WATER };


//struct tTile
//{
//	TileType nType;
//	tPoint ptPos;
//	bool bIsPlayerSpawn;
//	bool bIsEnemySpawn;
//	bool bIsCollision;
//};

class CTileEngine
{
private:
	CSGD_TextureManager* m_pTM;
	int m_nMapHeight;
	int m_nMapWidth;
	int m_nTileHeight;
	int m_nTileWidth;
	int m_nImageID;
	string m_szFileName;
	string m_szImagePath;
	string m_szTileType;
	CTile** pTileArray;
	//tTile pTileArray[4][4];
	POINT m_ptWorldPosition;
	RECT m_rTileRect;
	int m_nLayer;

	POINT m_ptSelectedTile;
	int m_nTileSetHeight;
	int m_nTileSetWidth;
	int m_nBitmapSizeHeight;
	int m_nBitmapSizeWidth;

public:
	CTileEngine();
	~CTileEngine();

	/////////////////////////////////
	//	Function:	"LoadFile"
	//
	//	Purpose: Loads in the binary map file
	/////////////////////////////////
	void LoadFile(char* szFileName);

	/////////////////////////////////
	//	Function:	"Render"
	//
	//	Purpose: Updates values based on time
	/////////////////////////////////
	void Render();

	/////////////////////////////////
	//	Function:	"Update"
	//
	//	Purpose: Updates values based on time
	/////////////////////////////////
	void LoadTileSet(char* szFileName);

	//Accessors
	//Map
#pragma region Map_Accessor
	/////////////////////////////////
	//	Function:	"Accessors"
	//
	//	Purpose: Gets the sepcified Type
	/////////////////////////////////
	int GetMapWidth() { return m_nMapWidth; }
	int GetMapHeight() { return m_nMapHeight; }
	int GetLayer() { return m_nLayer; }

	//Tile
	int GetTileWidth() { return m_nTileWidth; }
	int GetTileHeight() { return m_nTileHeight; }
	CTile GetTile(int x, int y) { return pTileArray[x][y]; }

	//Image
	int GetImageID() { return m_nImageID; }
	string GetImagePath() { return m_szImagePath; }
	string GetTileType() { return m_szTileType; }

	//File Name
	string GetFileName() { return m_szFileName; }

	RECT GetRectSize() { return m_rTileRect; }
#pragma endregion Map_Accessor
	
	//TileSet
#pragma region TileSet_Accessor
	/////////////////////////////////
	//	Function:	"Accessor"
	//
	//	Purpose: Gets the specified type
	/////////////////////////////////
	POINT GetSelectedTile() { return m_ptSelectedTile; }
	int GetTileSetHeight() { return m_nTileSetHeight; }
	int GetTileSetWidth() { return m_nTileSetWidth; }
#pragma endregion TileSet_Accessor


	//Mutators
	//Map
#pragma region Map_Mutator
	/////////////////////////////////
	//	Function:	"Mutators"
	//
	//	Purpose: Sets the specified value
	/////////////////////////////////
	void SetMapSize(int nMapWidth, int nMapHeight);
	void SetMapWidth(int nMapWidth);
	void SetMapHeight(int nMapHeight);

	//Tile
	void SetTileSize(int nTileWidth, int nTileHeight);
	void SetTileWidth(int nTileWidth);
	void SetTileHeight(int nTileHeight);
	void SetTile(CTile tile, int x, int y);

	//Image
	void SetImageID(int nImageID);
	void SetImagePath(string szImagePath);
	void SetTileType(string szTileType);

	//File Name
	void SetFileName(string szFileName);

	void SetRectSize(int nLeft, int nTop, int nRight, int nBottom);
#pragma endregion Map_Mutator
	
	//TileSet
#pragma region TileSet_Mutator
	/////////////////////////////////
	//	Function:	"Mutators"
	//
	//	Purpose: Sets the specified value
	/////////////////////////////////
	void SetSelectedTile(POINT ptSelectedTile);
	void SetSelectedTileX(int nXPos);
	void SetSelectedTileY(int nYPos);
	void SetTileSetHeight(int nTileSetHeight);
	void SetTileSetWidth(int nTileSetWidth);
#pragma endregion TileSet_Mutator

	void MouseMapLoad(CMouseMap* pmm, char* szFileName);
};

#endif