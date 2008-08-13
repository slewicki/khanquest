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
	m_nImageID = m_pTM->LoadTexture("Resource/KQ_GroundTemplate.bmp", D3DCOLOR_XRGB(255, 0, 255));
	m_nScrollImage = m_pTM->LoadTexture("Resource/KQ_Clouds.png");
	m_ptMousePoint.x = 0;
	m_ptMousePoint.y = 0;
	m_nScrollX = 800;
	m_nScrollY = 0;
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

	pTileArray = new CTile**[m_nLayer];

		for(int nLayer = 0; nLayer < m_nLayer; nLayer++)
		{
			fin.read((char*)&m_nMapWidth, sizeof(m_nMapWidth));
			fin.read((char*)&m_nMapHeight, sizeof(m_nMapHeight));
			fin.read((char*)&m_nTileWidth, sizeof(m_nTileWidth));
			fin.read((char*)&m_nTileHeight, sizeof(m_nTileHeight));

			//pTileArray = new CTile**[m_nLayer];

			pTileArray[nLayer] = new CTile*[m_nMapWidth];

			for(int i = 0; i < m_nMapWidth; i++)
			{
				pTileArray[nLayer][i] = new CTile[m_nMapHeight];
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
						pTileArray[nLayer][x][y].nType = PLAIN;
					else if(m_szTileType == "MOUNTAIN")
						pTileArray[nLayer][x][y].nType = MOUNTAIN;
					else if(m_szTileType == "FOREST")
						pTileArray[nLayer][x][y].nType = FOREST;
					else if(m_szTileType == "SHALLOW_WATER")
						pTileArray[nLayer][x][y].nType = SHALLOW_WATER;
					else if(m_szTileType == "DEEP_WATER")
						pTileArray[nLayer][x][y].nType = DEEP_WATER;

					int nPosX = 0;
					int nPosY = 0;
					fin.read((char*)&nPosX, sizeof(nPosX));
					fin.read((char*)&nPosY, sizeof(nPosY));

					pTileArray[nLayer][x][y].ptPos.x = nPosX;
					pTileArray[nLayer][x][y].ptPos.y = nPosY;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[nLayer][x][y].bIsEnemySpawn = true;
					else
						pTileArray[nLayer][x][y].bIsEnemySpawn = false;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
						pTileArray[nLayer][x][y].bIsPlayerSpawn = true;
					else
						pTileArray[nLayer][x][y].bIsPlayerSpawn = false;

					fin.read((char*)&bTempBool, sizeof(bTempBool));
					if (bTempBool)
					{
						pTileArray[nLayer][x][y].bIsCollision = true;
						pTileArray[nLayer][x][y].bIsOccupied = true;
					}
					else
					{
						pTileArray[nLayer][x][y].bIsCollision = false;
						pTileArray[nLayer][x][y].bIsOccupied = false;
					}

					pTileArray[nLayer][x][y].bIsVisible = false;
					pTileArray[nLayer][x][y].ptTileLoc.x = x;
					pTileArray[nLayer][x][y].ptTileLoc.y = y;
				}
			}
		}

		fin.close();

		SetLocalAnchor();
}

void CTileEngine::Render(RECT nCamPos)
{
	for ( int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		for (int Col = 0; Col < m_nMapHeight; Col++)
		{
			for(int Row = 0; Row < m_nMapWidth; Row++)
			{
				RECT rTile;
				rTile.left = pTileArray[nLayer][Row][Col].ptPos.x * m_nTileWidth;
				rTile.top = pTileArray[nLayer][Row][Col].ptPos.y * m_nTileHeight;
				rTile.right = rTile.left + m_nTileWidth;
				rTile.bottom = rTile.top + m_nTileHeight;

				POINT ptTilePos = { (((Row * m_nTileWidth / 2)) + (Col * m_nTileWidth / 2)) - nCamPos.left, (((Row * -(m_nTileHeight / 2)) + (Col * m_nTileHeight / 2)) + 300) - nCamPos.top };

				//if( (nCamPosX > pTileArray[Row][Col].ptLocalAnchor.x || nCamPosX < pTileArray[Row][Col].ptLocalAnchor.x) && (nCamPosY > pTileArray[Row][Col].ptLocalAnchor.y || nCamPosY < pTileArray[Row][Col].ptLocalAnchor.y))
				if(pTileArray[0][Row][Col].bIsVisible == true)
				{
					if(nCamPos.left < pTileArray[nLayer][Row][Col].ptLocalAnchor.x && nCamPos.right > pTileArray[nLayer][Row][Col].ptLocalAnchor.x && nCamPos.top < pTileArray[nLayer][Row][Col].ptLocalAnchor.y && nCamPos.bottom > pTileArray[nLayer][Row][Col].ptLocalAnchor.y)
					{
						m_pTM->Draw(m_nImageID, ptTilePos.x, ptTilePos.y, 1, 1, &rTile, 0, 0, 0); 
					}
				}
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
				pTileArray[nLayer][Row][Col].ptLocalAnchor.x = (((Row * m_nTileWidth / 2)) + (Col * m_nTileWidth / 2)) + (m_nTileWidth / 2);
				pTileArray[nLayer][Row][Col].ptLocalAnchor.y = (((Row * -(m_nTileHeight / 2)) + (Col * m_nTileHeight / 2)) + 300) + (m_nTileHeight / 2);
			}
		}
	}
}

POINT CTileEngine::IsoMouse(int x, int y, int z)
{
	POINT newPoint;

	newPoint.y = (m_nTileWidth * y + m_nTileHeight * x) / (m_nTileHeight * m_nTileWidth) - 10;
	float fTempX = (9 - ((m_nTileWidth * y - (float)m_nTileHeight * x) / (m_nTileHeight * m_nTileWidth)));
	newPoint.x = (int)fTempX;

	if(fTempX > (newPoint.x + .5))
		newPoint.x += 1;

	if(newPoint.y < m_nMapHeight && newPoint.x < m_nMapWidth && newPoint.y >= 0 && newPoint.x >= 0)
	{
		m_ptMousePoint = newPoint;
	}

	return m_ptMousePoint;
}

void CTileEngine::Clear()
{
	/*for (int nDelCount = 0; nDelCount < m_nMapWidth; nDelCount++)
		delete pTileArray[nDelCount];*/

	for(int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		for(int nDelCountX = 0; nDelCountX < m_nMapWidth; nDelCountX++)
		{
				delete pTileArray[nLayer][nDelCountX];
		}

		delete pTileArray[nLayer];
	}

	delete [] pTileArray;
}

void CTileEngine::SetOccupy(int x, int y, bool bOccupy, int nUnit)
{
	for(int i = 0; i < m_nLayer; i++)
	{
		pTileArray[i][x][y].bIsOccupied = bOccupy;
		pTileArray[i][x][y].nUnitIndex = nUnit;


	}
}

void CTileEngine::SetVisible(int x, int y, bool Visible, int nUnit)
{
	for(int i = 0; i < m_nLayer; i++)
	{
		pTileArray[i][x][y].bIsVisible = true;

		if(x - 1 > -1 && y > -1 && x < m_nMapWidth && y + 1 < m_nMapHeight)
			pTileArray[i][x - 1][y + 1].bIsVisible = true;

		if(x > -1 && y > -1 && x < m_nMapWidth && y + 1< m_nMapHeight)
			pTileArray[i][x][y + 1].bIsVisible = true;

		if(x > -1 && y > -1 && x + 1< m_nMapWidth && y + 1< m_nMapHeight)
			pTileArray[i][x + 1][y + 1].bIsVisible = true;

		if(x - 1 > -1 && y > -1 && x < m_nMapWidth && y < m_nMapHeight)
			pTileArray[i][x - 1][y].bIsVisible = true;
		
		if(x > -1 && y > -1 && x + 1< m_nMapWidth && y < m_nMapHeight)
			pTileArray[i][x + 1][y].bIsVisible = true;

		if(x - 1> -1 && y - 1> -1 && x < m_nMapWidth && y < m_nMapHeight)
			pTileArray[i][x - 1][y - 1].bIsVisible = true;

		if(x > -1 && y - 1> -1 && x < m_nMapWidth && y < m_nMapHeight)
			pTileArray[i][x][y - 1].bIsVisible = true;

		if(x > -1 && y - 1> -1 && x + 1< m_nMapWidth && y < m_nMapHeight)
			pTileArray[i][x + 1][y - 1].bIsVisible = true;
	}
}

void CTileEngine::ParalaxScroll(bool bIsScrollable, RECT rCamPos)
{
	if(!bIsScrollable)
		return;

	m_pTM->Draw(m_nScrollImage, m_nScrollX, m_nScrollY);
	if(m_pTM->GetTextureWidth(m_nScrollImage) > 0) 
		m_pTM->Draw(m_nScrollImage, m_pTM->GetTextureWidth(m_nScrollImage) - (1820 - m_nScrollX), 0);

	m_nScrollX += 1;

	if(m_nScrollX >= rCamPos.right)
		m_nScrollX = rCamPos.left;
	else if(m_nScrollX <= rCamPos.left)
		m_nScrollX = rCamPos.right;
	//m_nScrollY += 1;
}