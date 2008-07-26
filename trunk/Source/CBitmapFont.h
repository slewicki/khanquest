//////////////////////////////////////////////////////////
//	File:	"CBitmapFont.h"
//
//	Author:	Sean Hamstra (SH)
//
//	Purpose: To print bitmap fonts to the screen
//////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <windows.h>
using std::string;
class CBitmapFont
{
private:
	int		m_nImageID;
	int		m_nColumns;
	int		m_nCellWidth;
	int		m_nCellHeight;
	char	m_chStartCharacter;
	int		m_nCurrentPosX;
	int		m_nCustomWidth;
	int		m_nOffsetX;
	//////////////////////////////////////////////////////
	//	Function: “CalcRect”
	//	Last Modified: July 18, 2008
	//	Purpose: To caculate the location of a letter
	//			for drawing
	//////////////////////////////////////////////////////
	RECT CalcRect(char chLetter);

public:
	////////////////////////////////////////////
	//	Function:	"CBitmapFont(Constructor)"
	//	Last Modified: July 18, 2008
	//	Purpose:	Constructor.
	////////////////////////////////////////////
	CBitmapFont();
	////////////////////////////////////////////
	//	Function:	"~CBitmapFont(Destructor)"
	//	Last Modified: July 18, 2008
	//	Destructor.
	////////////////////////////////////////////
	~CBitmapFont();
	
	//////////////////////////////////////////////////////
	//	Function: “InitBitmapFont”
	//	Last Modified: July 18, 2008
	//	Purpose: Initialize our bitmap font with the imageID,
	//			starting character, number of columns, and
	//			cell width and height
	//////////////////////////////////////////////////////
	bool InitBitmapFont(int nImageID, char chStartCh, int nNumColumns, int nCellWidth, int nCellHeight);	

	//////////////////////////////////////////////////////
	//	Function: “DrawText”
	//	Last Modified: July 18, 2008
	//	Purpose: To draw the passed in text at the give location
	//////////////////////////////////////////////////////
	void DrawText(string szText, int nPosX, int nPosY, float fScaleX = 1.0f, 
		float fScaleY = 1.0f, DWORD dwColor = -1 );

	///////////////////////////////////////////
	//  Function: Modifiers
	//	Last Modified: July 18, 2008
	//  Purpose : Modifies the specified type.
	///////////////////////////////////////////
	inline void SetImageID(int nID) { m_nImageID = nID; }
	inline void SetNumColumns(int nColumns) { m_nColumns = nColumns; }
	inline void SetCellWidth(int nWidth) { m_nCellWidth = nWidth; }
	inline void SetCellHeight(int nHeight) { m_nCellHeight = nHeight; }
	inline void SetStartCharacter(char chStart) { m_chStartCharacter = chStart; }
};