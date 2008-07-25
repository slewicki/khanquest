//////////////////////////////////////////////////////////
//	File:	"CBitmapFont.cpp"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To print bitmap fonts to the screen
//////////////////////////////////////////////////////////

#include "CBitmapFont.h"
#include "CSGD_TextureManager.h"
#include "CSGD_WaveManager.h"
CBitmapFont::CBitmapFont(void)
{
	SetImageID(-1);
	SetNumColumns(0);
	SetCellWidth(0);
	SetCellHeight(0);
	SetStartCharacter(0);
}

CBitmapFont::~CBitmapFont(void)
{
	
}


bool CBitmapFont::InitBitmapFont(int nImageID, char chStartCh, int nNumColumns, int nCellWidth, int nCellHeight)
{
	if( nImageID < 0)
		return false;
	SetImageID(nImageID);
	SetNumColumns(nNumColumns);
	SetCellWidth(nCellWidth);
	SetCellHeight(nCellHeight);
	SetStartCharacter(chStartCh);
	return true;
}


void CBitmapFont::DrawText(string szText, int nPosX, int nPosY, float fScaleX, 
							float fScaleY, DWORD dwColor)
{
	RECT rLetterBox;
	int x = 0;
	for (unsigned int i = 0; i < szText.size(); i++)
	{
		if(szText[i] == '/')
		{
			nPosY += (int)(m_nCellHeight*fScaleY)+5;
			x = 0;
		}
		else
		{
			x++;
			rLetterBox = CalcRect(szText[i]);
			CSGD_TextureManager::GetInstance()->Draw(m_nImageID, nPosX+(int)(m_nCellWidth*fScaleX*x), nPosY, 
				fScaleX, fScaleY, &rLetterBox, 0.f, 0.f, 0.f, dwColor);
		}
	}
}

RECT CBitmapFont::CalcRect(char chLetter)
{
	int nID = chLetter - m_chStartCharacter;
	RECT rCell;
	rCell.left = ( nID % m_nColumns ) * m_nCellWidth;
	rCell.top  = ( nID / m_nColumns ) * m_nCellHeight;
	rCell.right = rCell.left + m_nCellWidth;
	rCell.bottom = rCell.top + m_nCellHeight;
	return rCell;
}

