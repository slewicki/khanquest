// CGDICanvas.h: interface for the CGDICanvas class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_CGDICANVAS_H__F5EB4F68_2CBC_11D4_A1EE_8F7A3049432E__INCLUDED_)
//#define AFX_CGDICANVAS_H__F5EB4F68_2CBC_11D4_A1EE_8F7A3049432E__INCLUDED_
#ifndef _CGDICANVAS_H_
#define _CGDICANVAS_H_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#include <windows.h>

//CGDICanvas--wrapper for a dc and a bitmap
class CGDICanvas  
{
private:
	//memory dc
	HDC hdcMem;

	//new bitmap
	HBITMAP hbmNew;

	//old bitmap
	HBITMAP hbmOld;

	//width and height
	int nWidth;
	int nHeight;

public:
	//Constructor
	CGDICanvas();

	//Loads bitmap from a file
	void Load(HDC hdcCompatible,LPCTSTR lpszFilename);

	//Creates a blank bitmap
	void CreateBlank(HDC hdcCompatible, int width, int height);

	//Destroys bitmap and dc
	void Destroy();

	//Converts to HDC
	operator HDC();

	//Return width
	int GetWidth();

	//Return height
	int GetHeight();

	//Destructor
	~CGDICanvas();
};

#endif // !defined(AFX_CGDICANVAS_H__F5EB4F68_2CBC_11D4_A1EE_8F7A3049432E__INCLUDED_)