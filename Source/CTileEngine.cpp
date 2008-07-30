//////////////////////////////////////////////////////////
//	File:	"CTileEngine.cpp"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Tile Engine
//////////////////////////////////////////////////////////
#include "CTileEngine.h"

CTileEngine::CTileEngine()
{
	m_pTM = CSGD_TextureManager::GetInstance();
	m_nImageID = -1;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("Resource/KQ_GroundTemplate.bmp", D3DCOLOR_XRGB(255, 0, 255));
}

CTileEngine::~CTileEngine()
{
	for (int nDelCount = 0; nDelCount < m_nMapWidth; nDelCount++)
		delete pTileArray[nDelCount];

	delete [] pTileArray;
}

//Mutators
//Map
#pragma region Map
void CTileEngine::SetMapSize(int nMapWidth, int nMapHeight)
{
	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;
}

void CTileEngine::SetMapWidth(int nMapWidth)
{
	m_nMapWidth = nMapWidth;
}

void CTileEngine::SetMapHeight(int nMapHeight)
{
	m_nMapHeight = nMapHeight;
}

//Tile
void CTileEngine::SetTileSize(int nTileWidth, int nTileHeight)
{
	m_nTileWidth = nTileWidth;
	m_nTileHeight = nTileHeight;
}

void CTileEngine::SetTileWidth(int nTileWidth)
{
	m_nTileWidth = nTileWidth;
}

void CTileEngine::SetTileHeight(int nTileHeight)
{
	m_nTileHeight = nTileHeight;
}

void CTileEngine::SetTile(tTile tile, int x, int y)
{
	pTileArray[x][y] = tile;
}

//Image
void CTileEngine::SetImageID(int nImageID)
{
	m_nImageID = nImageID;
}

void CTileEngine::SetImagePath(string szImagePath)
{
	m_szImagePath = szImagePath;
}

void CTileEngine::SetTileType(string szTileType)
{
	m_szTileType = szTileType;
}

//File Name
void CTileEngine::SetFileName(string szFileName)
{
	m_szImagePath = szFileName;
}

void CTileEngine::SetRectSize(int nLeft, int nTop, int nRight, int nBottom)
{
	m_rTileRect.left = nLeft;
	m_rTileRect.top = nTop;
	m_rTileRect.right = nRight;
	m_rTileRect.bottom = nBottom;
}

#pragma endregion Map
//Tile
#pragma region TileSet
void CTileEngine::SetSelectedTile(tPoint ptSelectedTile)
{
}

void CTileEngine::SetSelectedTileX(int nXPos)
{
}

void CTileEngine::SetSelectedTileY(int nYPos)
{
}

void CTileEngine::SetTileSetHeight(int nTileSetHeight)
{
}

void CTileEngine::SetTileSetWidth(int nTileSetWidth)
{
}

#pragma endregion TileSet

void CTileEngine::LoadFile(char* szFileName)
{
	ifstream fin;
	int nStringSize = 0;
	char* szBuffer = NULL;
	string szTest;
	bool bTempBool;

	fin.open(szFileName, std::ios_base::in | std::ios_base::binary);

	fin.read((char*)&nStringSize, sizeof(nStringSize));

	szBuffer = new char[nStringSize];

	fin.read(szBuffer, nStringSize);

	m_szImagePath = szBuffer;
	m_szImagePath.resize(nStringSize);

	fin.read((char*)&m_nLayer, sizeof(m_nLayer));

	delete szBuffer;

		for(int nLayer = 0; nLayer < m_nLayer; nLayer++)
		{
			fin.read((char*)&m_nMapWidth, sizeof(m_nMapWidth));
			fin.read((char*)&m_nMapHeight, sizeof(m_nMapHeight));
			fin.read((char*)&m_nTileWidth, sizeof(m_nTileWidth));
			fin.read((char*)&m_nTileHeight, sizeof(m_nTileHeight));

			//pTileArray** = new tTile[m_nMapHeight][m_nMapWidth];
			pTileArray = new tTile*[m_nMapWidth];

			for(int i = 0; i < m_nMapWidth; i++)
			{
				pTileArray[i] = new tTile[m_nMapHeight];
			}

			for(int x = 0; x < m_nMapHeight; x++)
			{
				for(int y = 0; y < m_nMapWidth; y++)
				{
					nStringSize = 0;
					fin.read((char*)&nStringSize, sizeof(nStringSize));

					szBuffer = 0;
					szBuffer = new char[nStringSize];

					fin.read((char*)szBuffer, nStringSize);

					m_szTileType = szBuffer;
					m_szTileType.resize(nStringSize);

					delete szBuffer;

					if(m_szTileType == "PLAIN")
						pTileArray[x][y].nType = PLAIN;
					else if(m_szTileType == "MOUNTAIN")
						pTileArray[x][y].nType = MOUNTAIN;
					else if(m_szTileType == "FOREST")
						pTileArray[x][y].nType = FOREST;
					else if(m_szTileType == "SHALLOW_WATER")
						pTileArray[x][y].nType = SHALLOW_WATER;
					else if(m_szTileType == "DEEP_WATER")
						pTileArray[x][y].nType = DEEP_WATER;
					else if(m_szTileType == "COLLISION")
						pTileArray[x][y].nType = COLLISION;

					int nPosX = 0;
					int nPosY = 0;
					fin.read((char*)&nPosX, sizeof(nPosX));
					fin.read((char*)&nPosY, sizeof(nPosY));

					pTileArray[x][y].ptPos.X = nPosX;
					pTileArray[x][y].ptPos.Y = nPosY;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[x][y].bIsEnemySpawn = true;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[x][y].bIsPlayerSpawn = true;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[x][y].bIsCollision = true;
				}
			}
		}

		fin.close();
}

void CTileEngine::Render()
{
	//Place inside Gamestates Render

	/*for ( int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		for (int Col = 0; Col < m_nMapHeight; Col++)
		{
			for(int Row = 0; Row < m_nMapWidth; Row++)
			{
				RECT rTile;
				rTile.left = pTileArray[Row][Col].ptPos.X * m_nTileWidth;
				rTile.top = pTileArray[Row][Col].ptPos.Y * m_nTileHeight;
				rTile.right = rTile.left + m_nTileWidth;
				rTile.bottom = rTile.top + m_nTileHeight;

				m_pTM->Draw(Map.GetImageID(), ((Row * Map.GetTileWidth() / 2)) + (Col * Map.GetTileWidth() / 2) + m_fPosX, ((Row * -(Map.GetTileHeight() / 2)) + (Col * Map.GetTileHeight() / 2)) + m_fPosY, 1, 1, &rTile, 0, 0, 0); 
			}
		}
	}*/
	for ( int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		for (int Col = 0; Col < m_nMapHeight; Col++)
		{
			for(int Row = 0; Row < m_nMapWidth; Row++)
			{
				RECT rTile;
				rTile.left = pTileArray[Row][Col].ptPos.X * m_nTileWidth;
				rTile.top = pTileArray[Row][Col].ptPos.Y * m_nTileHeight;
				rTile.right = rTile.left + m_nTileWidth;
				rTile.bottom = rTile.top + m_nTileHeight;

				m_pTM->Draw(m_nImageID, ((Row * m_nTileWidth / 2)) + (Col * m_nTileWidth / 2), ((Row * -(m_nTileHeight / 2)) + (Col * m_nTileHeight / 2)) + 300, 1, 1, &rTile, 0, 0, 0); 
			}
		}
	}
}

void CTileEngine::LoadTileSet(char *szFileName)
{
	m_nBitmapSizeHeight = 256;
	m_nBitmapSizeWidth = 256;
	m_nTileSetHeight = 8;
	m_nTileSetWidth = 4;

	for ( int x = 0; x < m_nMapHeight; x++)
	{
		for ( int y = 0; y < m_nMapWidth; y++)
		{
			if(m_ptSelectedTile.X == pTileArray[x][y].ptPos.X)
				m_ptSelectedTile.X = pTileArray[x][y].ptPos.X; // / m_nBitmapSizeWidth;

			if(m_ptSelectedTile.Y == pTileArray[x][y].ptPos.Y)
			m_ptSelectedTile.Y = pTileArray[x][y].ptPos.Y; 
		}
	}
}