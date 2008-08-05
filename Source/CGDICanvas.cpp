// CGDICanvas.cpp: implementation of the CGDICanvas class.
//
//////////////////////////////////////////////////////////////////////

#include "CGDICanvas.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGDICanvas::CGDICanvas()
{
	//initialize all members to NULL or 0
	hdcMem=NULL;
	hbmNew=NULL;
	hbmOld=NULL;
	nWidth=0;
	nHeight=0;
}

CGDICanvas::~CGDICanvas()
{
	//if the hdcMem has not been destroyed, do so
	if(hdcMem) Destroy();
}

//////////////////////////////////////////////////////////////////////
//Creation/Loading
//////////////////////////////////////////////////////////////////////

//loads bitmap from a file
void CGDICanvas::Load(HDC hdcCompatible,LPCTSTR lpszFilename)
{
	//if the hdcMem is not null, destroy
	if(hdcMem) Destroy();

	//create the memory dc
	hdcMem=CreateCompatibleDC(hdcCompatible);

	//load the image
	hbmNew=(HBITMAP)LoadImage(NULL,lpszFilename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	//select the image into the dc
	hbmOld=(HBITMAP)SelectObject(hdcMem,hbmNew);

	//fetch the bitmaps properties
	BITMAP bmp;
	GetObject(hbmNew,sizeof(BITMAP),(LPVOID)&bmp);

	//assign height and width
	nWidth=bmp.bmWidth;
	nHeight=bmp.bmHeight;

}

//creates a blank bitmap
void CGDICanvas::CreateBlank(HDC hdcCompatible, int width, int height)
{
	//if the hdcMem is not null, destroy
	if(hdcMem) Destroy();

	//create the memory dc
	hdcMem=CreateCompatibleDC(hdcCompatible);

	//create the image
	hbmNew=CreateCompatibleBitmap(hdcCompatible,width,height);

	//select the image into the dc
	hbmOld=(HBITMAP)SelectObject(hdcMem,hbmNew);

	//assign the width and height
	nWidth=width;
	nHeight=height;

}

//////////////////////////////////////////////////////////////////////
//Clean-up
//////////////////////////////////////////////////////////////////////

//destroys bitmap and dc
void CGDICanvas::Destroy()
{
	//restore old bitmap
	SelectObject(hdcMem,hbmOld);

	//delete new bitmap
	DeleteObject(hbmNew);

	//delete device context
	DeleteDC(hdcMem);

	//set all members to 0 or NULL
	hdcMem=NULL;
	hbmNew=NULL;
	hbmOld=NULL;
	nWidth=0;
	nHeight=0;
}

//////////////////////////////////////////////////////////////////////
//Conversion
//////////////////////////////////////////////////////////////////////

//converts to HDC
CGDICanvas::operator HDC()
{
	//return the hdcMem member
	return(hdcMem);
}

//////////////////////////////////////////////////////////////////////
//Returning information
//////////////////////////////////////////////////////////////////////
//return width
int CGDICanvas::GetWidth()
{
	//return the width
	return(nWidth);
}

//return height
int CGDICanvas::GetHeight()
{
	//return the height
	return(nHeight);
}