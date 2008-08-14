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
#pragma region Map_Accessor
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

	//Image
	string GetImagePath() { return m_szImagePath; }
#pragma endregion Map_Accessor

	static CTileEngine* GetInstance();

	POINT IsoMouse(int x, int y, int z);

	CTile GetTile(int Layer, int x, int y) { return pTileArray[Layer][x][y]; }
	CTile MapToTile(int Layer, int x, int y);
	POINT GetLocalAnchor(int Layer, int x, int y) { return pTileArray[Layer][x][y].ptLocalAnchor; }
	void SetLocalAnchor();
	void SetOccupy(int x, int y, bool bOccupy, int nUnit);
	void SetVisible(int x, int y, bool Visible, int nUnit);

	void Clear();

	void ParalaxScroll(bool bIsScrollable, RECT rCamPos);
};

#endif