//////////////////////////////////////////////////////////
//	File:	"CTileEngine.cpp"
//
//	Author:	Sebastian Lewicki (SL)
//
//	Purpose: Tile Engine
//////////////////////////////////////////////////////////
#include "CTileEngine.h"
#include "CUnit.h"
#include "CCamera.h"
#include "CProfile.h"

CTileEngine::CTileEngine()
{
	PROFILE("CTileEngine::CTileEngine()");
	m_pTM = CSGD_TextureManager::GetInstance();
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_nImageID = -1;
	m_nImageID = m_pTM->LoadTexture("Resource/KQ_Terrain.png", D3DCOLOR_XRGB(255, 0, 255));
	m_nScrollImage = m_pTM->LoadTexture("Resource/KQ_Clouds.png");
	m_nBlankTileID = m_pTM->LoadTexture("Resource/KQ_BlankTile.png");
	m_ptMousePoint.x = 0;
	m_ptMousePoint.y = 0;
	m_nScrollX = 800;
	m_nScrollY = 0;
}

CTileEngine::~CTileEngine()
{
	PROFILE("CTileEngine::~CTileEngine()");
	//m_pTM->ReleaseTexture(m_nBlankTileID);
	Clear();
}

CTileEngine* CTileEngine::GetInstance()
{
	PROFILE("CTileEngine::GetInstance()");
	static CTileEngine Instance;
	return &Instance;
}

void CTileEngine::LoadFile(char* szFileName)
{
	PROFILE("CTileEngine::LoadFile(char*)");
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

	fin.read((char*)&m_nMapWidth, sizeof(m_nMapWidth));
	fin.read((char*)&m_nMapHeight, sizeof(m_nMapHeight));
	fin.read((char*)&m_nTileWidth, sizeof(m_nTileWidth));
	fin.read((char*)&m_nTileHeight, sizeof(m_nTileHeight));

	for(int nLayer = 0; nLayer < m_nLayer; nLayer++)
	{
		/*fin.read((char*)&m_nMapWidth, sizeof(m_nMapWidth));
		fin.read((char*)&m_nMapHeight, sizeof(m_nMapHeight));
		fin.read((char*)&m_nTileWidth, sizeof(m_nTileWidth));
		fin.read((char*)&m_nTileHeight, sizeof(m_nTileHeight));*/

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

					if(m_szTileType == "Plains")
						pTileArray[nLayer][x][y].nType = PLAIN;
					else if(m_szTileType == "Mountains")
						pTileArray[nLayer][x][y].nType = MOUNTAIN;
					else if(m_szTileType == "Forest")
						pTileArray[nLayer][x][y].nType = FOREST;
					else if(m_szTileType == "Shallow Water")
						pTileArray[nLayer][x][y].nType = SHALLOW_WATER;
					else if(m_szTileType == "Deep Water")
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
						pTileArray[nLayer][x][y].bIsOccupied = false;
					}
					else
					{
						pTileArray[nLayer][x][y].bIsCollision = false;
						pTileArray[nLayer][x][y].bIsOccupied = false;
					}

					pTileArray[nLayer][x][y].bIsVisible = false;
					pTileArray[nLayer][x][y].pUnit = NULL;
					pTileArray[nLayer][x][y].ptTileLoc.x = x;
					pTileArray[nLayer][x][y].ptTileLoc.y = y;
					pTileArray[nLayer][x][y].vColor = D3DCOLOR_ARGB(255, 255, 255, 255);
				}
			}
		}

		fin.close();

		SetLocalAnchor();
}

void CTileEngine::Render(RECT nCamPos)
{
	PROFILE("CTileEngine::Render(RECT)");
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
					if(nCamPos.left < pTileArray[nLayer][Row][Col].ptLocalAnchor.x + 32 && nCamPos.right > pTileArray[nLayer][Row][Col].ptLocalAnchor.x - 32 && nCamPos.top < pTileArray[nLayer][Row][Col].ptLocalAnchor.y  + 16 && nCamPos.bottom > pTileArray[nLayer][Row][Col].ptLocalAnchor.y - 16)
					{
						//Temp
						//-------------------------
						pTileArray[nLayer][Row][Col].vColor = D3DCOLOR_ARGB(255, 255, 255, 255);
						
						if(pTileArray[nLayer][Row][Col].bIsOccupied) 
							pTileArray[nLayer][Row][Col].vColor = D3DCOLOR_ARGB(255,255, 0, 0);
						//----------------------------
						m_pTM->Draw(m_nImageID, ptTilePos.x, ptTilePos.y, 1, 1, &rTile, 0, 0, 0, pTileArray[0][Row][Col].vColor); 
						// Temp for anchors
						//m_pTM->Draw(m_nImageID, pTileArray[0][Row][Col].ptLocalAnchor.x, pTileArray[0][Row][Col].ptLocalAnchor.y, .1, .1, &rTile, 0, 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0)); 
						// Set all to normal color
						//pTileArray[0][Row][Col].vColor = D3DCOLOR_ARGB(255,255,255,255);
					}
				}
			}
		}
	}
}

void CTileEngine::RenderMiniMap(RECT nCamPos)
{
	PROFILE("CTileEngine::RenderMiniMap(RECT)");
	float fPercentX, fPercentY;
	fPercentX = 266.f/(m_nMapWidth*62); 
	fPercentY = 135.f/(m_nMapHeight*32); 
	for ( int nLayer = 0; nLayer < 1; nLayer++)
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

				POINT ptTilePos = { (int)(((Row * (m_nTileWidth*fPercentX) / 2)) + (Col * (m_nTileWidth*fPercentX) / 2))+510, (int)(((Row * -((m_nTileHeight*fPercentY) / 2)) + (Col * (m_nTileHeight*fPercentY) / 2)))+ 528 };
				//m_pTM->Draw(m_nBlankTileID, ptTilePos.x, ptTilePos.y, fPercentX, fPercentY, &rTile, 0, 0, 0, D3DCOLOR_ARGB(255, 122, 122, 122)); 
				
				//if( (nCamPosX > pTileArray[Row][Col].ptLocalAnchor.x || nCamPosX < pTileArray[Row][Col].ptLocalAnchor.x) && (nCamPosY > pTileArray[Row][Col].ptLocalAnchor.y || nCamPosY < pTileArray[Row][Col].ptLocalAnchor.y))
				if(pTileArray[0][Row][Col].bIsVisible == true)
				{
					if(pTileArray[0][Row][Col].bIsOccupied && pTileArray[0][Row][Col].pUnit && pTileArray[0][Row][Col].pUnit->IsPlayerUnit())
						m_pTM->Draw(m_nBlankTileID, ptTilePos.x, ptTilePos.y, fPercentX, fPercentY, 0, 0, 0, 0, D3DCOLOR_ARGB(255, 0, 255, 0)); 

					else if(pTileArray[0][Row][Col].bIsOccupied && pTileArray[0][Row][Col].pUnit && !pTileArray[0][Row][Col].pUnit->IsPlayerUnit())
						m_pTM->Draw(m_nBlankTileID, ptTilePos.x, ptTilePos.y, fPercentX, fPercentY, 0, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 0, 0)); 
					else 
						m_pTM->Draw(m_nImageID, ptTilePos.x, ptTilePos.y, fPercentX, fPercentY, &rTile, 0, 0, 0, D3DCOLOR_ARGB(255, 255, 255, 255)); 
					
				}
				else
					m_pTM->Draw(m_nBlankTileID, ptTilePos.x, ptTilePos.y, fPercentX, fPercentY, &rTile, 0, 0, 0, D3DCOLOR_ARGB(255, 0, 0, 0)); 

			}
		}
	}
	RECT rCamera;
	int x = (int)CCamera::GetInstance()->GetPosX();
	int y = (int)CCamera::GetInstance()->GetPosY();
	rCamera.left	=	(int)(x*fPercentX)+510;
	rCamera.right	=	rCamera.left+(int)(800*fPercentX);
	rCamera.top		=	(int)(y*fPercentY)+(528 - (int)(300*fPercentY));
	rCamera.bottom	=	rCamera.top+(int)(455*fPercentY);
	CSGD_Direct3D::GetInstance()->SpriteEnd();
	CSGD_Direct3D::GetInstance()->LineEnd();
	CSGD_Direct3D::GetInstance()->DeviceEnd();
	
	CSGD_Direct3D::GetInstance()->DrawPrimitiveRect(rCamera, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	
	CSGD_Direct3D::GetInstance()->DeviceBegin();
	CSGD_Direct3D::GetInstance()->LineBegin();
	CSGD_Direct3D::GetInstance()->SpriteBegin();

	
}

void CTileEngine::SetLocalAnchor()
{
	PROFILE("CTileEngine::SetLocalAnchor()");
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
	PROFILE("CTileEngine::IsoMouse(int, int, int)");
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
POINT CTileEngine::IsoMiniMouse(int nLocalX, int nLocalY, int z)
{
	PROFILE("CTileEngine::IsoMiniMouse(int, int, int)");
	nLocalX -= 512;
	nLocalY -= 530;
	float fPercentX, fPercentY;
	fPercentY = fPercentX = 266.f/(m_nMapWidth*62); 
	fPercentY = 135.f/(m_nMapHeight*32); 
	//nLocalX *= fPercentX;
	//nLocalY *= fPercentX;

	POINT newPoint;
	int nTileWidth = (int)(m_nTileWidth*fPercentX);
	int nTileHeight = (int)(m_nTileHeight*fPercentX);
	newPoint.y = (int)((nTileWidth * nLocalY + nTileHeight * nLocalX) / (nTileHeight * nTileWidth)-(1*fPercentY));
	float fTempX = (2*fPercentX)-(((nTileWidth * nLocalY - (float)nTileHeight * nLocalX) / (nTileHeight * nTileWidth)));
	newPoint.x = (int)fTempX;

	if(fTempX > (newPoint.x + .5))
		newPoint.x += 1;
	
	if(newPoint.y >= m_nMapHeight && newPoint.y < m_nMapHeight+2)
		newPoint.y = m_nMapHeight-1;
	else if(newPoint.y < 0 && newPoint.y > -2)
		newPoint.y = 0;
	else if(newPoint.y <= -5 || newPoint.y >= m_nMapWidth+2)
		newPoint.y = -1;

	if(newPoint.x >= m_nMapWidth && newPoint.x < m_nMapWidth+2)
		newPoint.x = m_nMapWidth-1;
	else if(newPoint.x < 0 && newPoint.x > -2)
		newPoint.x = 0;
	else if(newPoint.y <= -5 || newPoint.x >= m_nMapWidth+2)
		newPoint.x = -1;
	return newPoint;
	

}

void CTileEngine::Clear()
{
	PROFILE("CTileEngine::Clear()");
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

void CTileEngine::SetOccupy(int x, int y, bool bOccupy, CUnit* Unit)
{
	PROFILE("CTileEngine::SetOccupy(int, int, bool, CUnit*)");
	for(int i = 0; i < m_nLayer; i++)
	{
		pTileArray[i][x][y].bIsOccupied = bOccupy;
		pTileArray[i][x][y].pUnit = Unit;
	}
}

void CTileEngine::SetVisible(int x, int y, bool Visible, CUnit* Unit)
{
	PROFILE("CTileEngine::SetVisible(int, int, bool, CUnit*)");
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
	PROFILE("CTileEngine::ParalaxScroll(bool, RECT)");
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

CTile* CTileEngine::MapToTile(int Layer, int x, int y)
{
	PROFILE("CTileEngine::MapToTile(int, int, int)");
	POINT ptNewPoint;
	ptNewPoint = IsoMouse(x, y, 0);

	return &pTileArray[Layer][ptNewPoint.x][ptNewPoint.y];
}

void CTileEngine::SetColor(int Layer, int x, int y, int nRed, int nGreen, int nBlue)
{
	PROFILE("CTileEngine::SetColor(int, int, int, int, int, int)");
	pTileArray[Layer][x][y].vColor.a = 255;
	pTileArray[Layer][x][y].vColor.r = (float)nRed;
	pTileArray[Layer][x][y].vColor.g = (float)nGreen;
	pTileArray[Layer][x][y].vColor.b = (float)nBlue;
}