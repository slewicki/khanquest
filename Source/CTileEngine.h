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
#include "CSGD_Direct3D.h"

#include "Math.h"

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

class CTileEngine
{
private:
	CSGD_TextureManager* m_pTM;		//Texture Manager Wrapper
	CSGD_Direct3D* m_pD3D;			//Direct3D Wrapper
	int m_nMapHeight;				//Map Height
	int m_nMapWidth;				//Map Width
	int m_nTileHeight;				//Tile Height
	int m_nTileWidth;				//Tile Width
	int m_nImageID;					//Image loaded through texture manager
	//string m_szFileName;
	string m_szImagePath;			//Name of the image to load
	string m_szTileType;			//Type of the tile
	CTile** pTileArray;				//2D array of tiles that make a map
	//tTile pTileArray[4][4];
	//POINT m_ptWorldPosition;
	//RECT m_rTileRect;
	int m_nLayer;					//Number of Map Layers
	POINT m_ptMousePoint;			//Mouse Location


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
	void Render(int nCamPosX, int nCamPosY);

	/////////////////////////////////
	//	Function:	"Update"
	//
	//	Purpose: Updates values based on time
	/////////////////////////////////
	//void LoadTileSet(char* szFileName);

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
	string GetImagePath() { return m_szImagePath; }
#pragma endregion Map_Accessor

	static CTileEngine* GetInstance();

	POINT IsoMouse(int x, int y, int z);

	CTile GetTile(int x, int y) { return pTileArray[x][y]; }
	POINT GetLocalAnchor(int x, int y) { return pTileArray[x][y].ptLocalAnchor; }
	void SetLocalAnchor();
	void SetOccupy(int x, int y, bool bOccupy);

	void Clear();
};

#endif