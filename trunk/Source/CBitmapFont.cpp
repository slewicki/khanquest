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
#include "CProfile.h"
CBitmapFont::CBitmapFont(void)
{
	PROFILE("CBitmapFont::CBitmapFont()");
	SetImageID(-1);
	SetNumColumns(0);
	SetCellWidth(0);
	SetCellHeight(0);
	SetStartCharacter(0);
	m_nCustomWidth = 0;
	m_nOffsetX = 0;
	m_nCurrentPosX = 0;
	STOP("CBitmapFont::CBitmapFont()");
}

CBitmapFont::~CBitmapFont(void)
{
	
}


bool CBitmapFont::InitBitmapFont(int nImageID, char chStartCh, int nNumColumns, int nCellWidth, int nCellHeight)
{
	PROFILE("CBitmapFont::InitBitmapFont(int, char, int, int, int)");
	if( nImageID < 0)
	{
		STOP("CBitmapFont::InitBitmapFont(int, char, int, int, int)");
		return false;
	}
	SetImageID(nImageID);
	SetNumColumns(nNumColumns);
	SetCellWidth(nCellWidth);
	SetCellHeight(nCellHeight);
	SetStartCharacter(chStartCh);
	m_nCustomWidth = m_nCellWidth;
	m_nCurrentPosX = 0;
	STOP("CBitmapFont::InitBitmapFont(int, char, int, int, int)");
	return true;
}


void CBitmapFont::DrawText(string szText, int nPosX, int nPosY, float fScaleX, 
							float fScaleY, DWORD dwColor)
{
	PROFILE("CBitmapFont::DrawText(string, int, int, float, float, DWORD)");
	RECT rLetterBox;
	m_nCurrentPosX = nPosX;
	for (unsigned int i = 0; i < szText.size(); i++)
	{
		if(szText[i] == '/')
		{
			nPosY += (int)(m_nCellHeight*fScaleY)+5;
			m_nCurrentPosX = nPosX;
		}
		else
		{
			rLetterBox = CalcRect(szText[i]);
				CSGD_TextureManager::GetInstance()->Draw(m_nImageID, 
				m_nCurrentPosX, nPosY, 
					fScaleX, fScaleY, &rLetterBox, 0.f, 0.f, 0.f, dwColor);
			m_nCurrentPosX += (int)(m_nCustomWidth*fScaleX);
		}
	}
	STOP("CBitmapFont::DrawText(string, int, int, float, float, DWORD)");
}

RECT CBitmapFont::CalcRect(char chLetter)
{
	PROFILE("CBitmapFont::CalcRect(char)");
	int nID = chLetter - m_chStartCharacter;

	// How wide should this character be
	m_nCustomWidth = m_nCellWidth;

	// Where should the width box start
	m_nOffsetX = 0;

	// Curning, our font ONLY
	switch (chLetter)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case ':':
		m_nCustomWidth = 72;
		m_nOffsetX = 22;
		break;
#pragma region LowerCase
	case 'a':
		m_nCustomWidth = 68;
		m_nOffsetX = 26;
		break;
	case 'b':
		m_nCustomWidth = 68;
		m_nOffsetX = 26;
		break;
	case 'c':
		m_nCustomWidth = 52;
		m_nOffsetX = 30;
		break;
	case 'e':
		m_nCustomWidth = 50;
		m_nOffsetX = 34;
		break;
	case 'h':
		m_nCustomWidth = 66;
		m_nOffsetX = 28;
		break;
	case 'k':
		m_nCustomWidth = 60;
		m_nOffsetX = 28;
		break;
	case 'o':
		m_nCustomWidth = 56;
		m_nOffsetX = 32;
		break;
	case 'q':
		m_nCustomWidth = 60;
		m_nOffsetX = 28;
		break;
	case 'r':
		m_nCustomWidth = 66;
		m_nOffsetX = 28;
		break;
	case 's':
		m_nCustomWidth = 50;
		m_nOffsetX = 32;
		break;
	case 't':
		m_nCustomWidth = 46;
		m_nOffsetX = 34;
		break;
	case 'u':
		m_nCustomWidth = 71;
		m_nOffsetX = 24;
		break;
	case 'v':
		m_nCustomWidth = 70;
		m_nOffsetX = 22;
		break;
	case 'x':
		m_nCustomWidth = 74;
		m_nOffsetX = 22;
		break;
	case 'y':
		m_nCustomWidth = 68;
		m_nOffsetX = 24;
		break;
	case 'z':
		m_nCustomWidth = 56;
		m_nOffsetX = 32;
		break;
	case 'n':
		m_nCustomWidth = 76;
		m_nOffsetX = 22;
		break;
	case 'm':
		m_nCustomWidth = 108;
		m_nOffsetX = 0;
		break;
	case 'd':
		m_nCustomWidth = 92;
		m_nOffsetX = 29;
		break;
	case 'g':
		m_nCustomWidth = 70;
		m_nOffsetX = 22;
		break;
	case 'l':
		m_nCustomWidth = 55;
		m_nOffsetX = 38;
		break;
	case 'i':
		m_nCustomWidth = 40;
		m_nOffsetX = 44;
		break;
	case 'f':
		m_nCustomWidth = 86;
		m_nOffsetX = 18;
		break;
	case 'j':
		m_nCustomWidth = 72;
		m_nOffsetX = 14;
		break; 
	case 'p':
		m_nCustomWidth = 96;
		m_nOffsetX = 10;
		break; 
	case 'w':
		m_nCustomWidth = 82;
		m_nOffsetX = 10;
		break; 
#pragma endregion
	case ' ':
		m_nCustomWidth = 40;
		m_nOffsetX = 0;
		break;
	case ',':
		m_nCustomWidth = 26;
		m_nOffsetX = 42;
		break;
	case '.':
		m_nCustomWidth = 22;
		m_nOffsetX = 42;
		break;
	case '!':
		m_nCustomWidth = 30;
		m_nOffsetX = 46;
		break;
	case '\'':
		m_nCustomWidth = 22;
		m_nOffsetX = 56;
		break;
	// Capital Letters (H,N,W are all cellWidth)
#pragma region Capitals
	case 'A':
		m_nCustomWidth = 98;
		m_nOffsetX = 20;
		break;
	case 'B':
		m_nCustomWidth = 78;
		m_nOffsetX = 20;
		break;
	case 'C':
		m_nCustomWidth = 68;
		m_nOffsetX = 30;
		break;
	case 'D':
		m_nCustomWidth = 92;
		m_nOffsetX = 16;
		break;
	case 'E':
		m_nCustomWidth = 74;
		m_nOffsetX = 26;
		break;
	case 'F':
		m_nCustomWidth = 90;
		m_nOffsetX = 16;
		break;
	case 'G':
		m_nCustomWidth = 74;
		m_nOffsetX = 24;
		break;
	case 'I':
		m_nCustomWidth = 60;
		m_nOffsetX = 28;
		break;
	case 'J':
		m_nCustomWidth = 74;
		m_nOffsetX = 18;
		break;
	case 'K':
		m_nCustomWidth = 106;
		m_nOffsetX = 4;
		break;
	case 'L':
		m_nCustomWidth = 72;
		m_nOffsetX = 20;
		break;
	case 'M':
		m_nCustomWidth = 116;
		m_nOffsetX = 0;
		break;
	case 'O':
		m_nCustomWidth = 76;
		m_nOffsetX = 20;
		break;
	case 'P':
		m_nCustomWidth = 76;
		m_nOffsetX = 27;
		break;
	case 'Q':
		m_nCustomWidth = 76;
		m_nOffsetX = 24;
		break;
	case 'R':
		m_nCustomWidth = 88;
		m_nOffsetX = 20;
		break;
	case 'S':
		m_nCustomWidth = 60;
		m_nOffsetX = 30;
		break;
	case 'T':
		m_nCustomWidth = 90;
		m_nOffsetX = 26;
		break;
	case 'U':
		m_nCustomWidth = 92;
		m_nOffsetX = 14;
		break;
	case 'V':
		m_nCustomWidth = 96;
		m_nOffsetX = 20;
		break;
	case 'X':
		m_nCustomWidth = 88;
		m_nOffsetX = 18;
		break;
	case 'Y':
		m_nCustomWidth = 76;
		m_nOffsetX = 26;
		break;
	case 'Z':
		m_nCustomWidth = 74;
		m_nOffsetX = 20;
		break; 
#pragma endregion
	
	}


	RECT rCell;
	rCell.left = ( nID % m_nColumns ) * m_nCellWidth+m_nOffsetX;
	rCell.top  = ( nID / m_nColumns ) * m_nCellHeight;
	rCell.right = rCell.left + m_nCustomWidth;
	rCell.bottom = rCell.top + m_nCellHeight;
	
	STOP("CBitmapFont::CalcRect(char)");
	return rCell;
}

