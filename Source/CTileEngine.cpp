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
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_nImageID = -1;
	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture("Resource/KQ_GroundTemplate.bmp", D3DCOLOR_XRGB(255, 0, 255));
	m_ptMousePoint.x = 0;
	m_ptMousePoint.y = 0;
}

CTileEngine::~CTileEngine()
{
	Clear();
}

CTileEngine* CTileEngine::GetInstance()
{
	static CTileEngine Instance;
	return &Instance;
}

void CTileEngine::LoadFile(char* szFileName)
{
	ifstream fin;
	int nStringSize = 0;
	char* szBuffer = NULL;
	string szTest;
	bool bTempBool;

	if(pTileArray)
		Clear();

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

			pTileArray = new CTile*[m_nMapWidth];

			for(int i = 0; i < m_nMapWidth; i++)
			{
				pTileArray[i] = new CTile[m_nMapHeight];
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

					int nPosX = 0;
					int nPosY = 0;
					fin.read((char*)&nPosX, sizeof(nPosX));
					fin.read((char*)&nPosY, sizeof(nPosY));

					pTileArray[x][y].ptPos.x = nPosX;
					pTileArray[x][y].ptPos.y = nPosY;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[x][y].bIsEnemySpawn = true;
					else
						pTileArray[x][y].bIsEnemySpawn = false;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[x][y].bIsPlayerSpawn = true;
					else
						pTileArray[x][y].bIsPlayerSpawn = false;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
					{
						pTileArray[x][y].bIsCollision = true;
						pTileArray[x][y].bIsOccupied = true;
					}
					else
					{
						pTileArray[x][y].bIsCollision = false;
						pTileArray[x][y].bIsOccupied = false;
					}
				}
			}
		}

		fin.close();

		SetLocalAnchor();
}

void CTileEngine::Render(int nCamPosX, int nCamPosY)
{
	for ( int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		for (int Col = 0; Col < m_nMapHeight; Col++)
		{
			for(int Row = 0; Row < m_nMapWidth; Row++)
			{
				RECT rTile;
				rTile.left = pTileArray[Row][Col].ptPos.x * m_nTileWidth;
				rTile.top = pTileArray[Row][Col].ptPos.y * m_nTileHeight;
				rTile.right = rTile.left + m_nTileWidth;
				rTile.bottom = rTile.top + m_nTileHeight;

				m_pTM->Draw(m_nImageID, (((Row * m_nTileWidth / 2)) + (Col * m_nTileWidth / 2)) - nCamPosX, (((Row * -(m_nTileHeight / 2)) + (Col * m_nTileHeight / 2)) + 300) - nCamPosY, 1, 1, &rTile, 0, 0, 0); 
			}
		}
	}
}

void CTileEngine::SetLocalAnchor()
{
	for ( int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		for (int Col = 0; Col < m_nMapHeight; Col++)
		{
			for(int Row = 0; Row < m_nMapWidth; Row++)
			{
				pTileArray[Row][Col].ptLocalAnchor.x = (((Row * m_nTileWidth / 2)) + (Col * m_nTileWidth / 2)) + (m_nTileWidth / 2);
				pTileArray[Row][Col].ptLocalAnchor.y = (((Row * -(m_nTileHeight / 2)) + (Col * m_nTileHeight / 2)) + 300) + (m_nTileHeight / 2);
			}
		}
	}
}

POINT CTileEngine::IsoMouse(int x, int y, int z)
{
	static float isoCos = cos(0.46365f);
	static float isoSin = sin(0.46365f);

	POINT newPoint;

	newPoint.y = (m_nTileWidth * y + m_nTileHeight * x) / (64 * 32) - 10;
	newPoint.x = (m_nTileWidth * y - m_nTileHeight * x) / (64 * 32);


	for(int i = 0, j = m_nMapHeight - 1; i < m_nMapHeight; i++, j--)
	{
		if(j == newPoint.x)
		{
			newPoint.x = i;
			break;
		}
	}

	if(newPoint.y < m_nMapHeight && newPoint.x < m_nMapWidth && newPoint.y >= 0 && newPoint.x >= 0)
	{
		m_ptMousePoint = newPoint;
	}

	return m_ptMousePoint;
}

void CTileEngine::Clear()
{
	for (int nDelCount = 0; nDelCount < m_nMapWidth; nDelCount++)
		delete pTileArray[nDelCount];

	delete [] pTileArray;
}

void CTileEngine::SetOccupy(int x, int y, bool bOccupy)
{
	pTileArray[x][y].bIsOccupied = bOccupy;
}