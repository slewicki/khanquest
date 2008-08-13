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
	int m_nScrollImage;				//Image scrolling on the camera;
	string m_szImagePath;			//Name of the image to load
	string m_szTileType;			//Type of the tile
	CTile*** pTileArray;			//3D array of tiles that make a map
	int m_nLayer;					//Number of Map Layers
	POINT m_ptMousePoint;			//Mouse Location
	int m_nScrollX;					//Scroll by X value
	int m_nScrollY;					//Scroll by Y value


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
	void Render(RECT nCamPos);

	//Accessors
	//Map
#pragma region Accessor
	/////////////////////////////////
	//	Function:	"Accessors"
	//
	//	Purpose: Gets the specified Type
	/////////////////////////////////
	int GetMapWidth() { return m_nMapWidth; }
	int GetMapHeight() { return m_nMapHeight; }

	//Tile
	int GetTileWidth() { return m_nTileWidth; }
	int GetTileHeight() { return m_nTileHeight; }
	CTile GetTile(int Layer, int x, int y) { return pTileArray[Layer][x][y]; }
	POINT GetLocalAnchor(int Layer, int x, int y) { return pTileArray[Layer][x][y].ptLocalAnchor; }

	//Image
	string GetImagePath() { return m_szImagePath; }
#pragma endregion Accessor

#pragma region Mutator
	/////////////////////////////////
	//	Function:	"Mutators"
	//
	//	Purpose: Sets the specified Type
	/////////////////////////////////
	void SetLocalAnchor();
	void SetOccupy(int x, int y, bool bOccupy, int nUnit);
	void SetVisible(int x, int y, bool Visible, int nUnit);

#pragma endregion Mutator

	/////////////////////////////////
	//	Function:	"GetInstance"
	//
	//	Purpose: Returns the instance of the Tile Engine
	/////////////////////////////////
	static CTileEngine* GetInstance();

	/////////////////////////////////
	//	Function:	"IsoMouse"
	//
	//	Purpose: Returns the tile location based on Screen coordinates
	/////////////////////////////////
	POINT IsoMouse(int x, int y, int z);

	/////////////////////////////////
	//	Function:	"Clear"
	//
	//	Purpose: Clears all dynamic memory
	/////////////////////////////////
	void Clear();

	/////////////////////////////////
	//	Function:	"ParallaxScroll"
	//
	//	Purpose: Scrolls the image along the camera's view
	/////////////////////////////////
	void ParalaxScroll(bool bIsScrollable, RECT rCamPos);
};

#endif