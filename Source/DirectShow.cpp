#include "DirectShow.h"
#include "CGame.h"
CDirectShow::CDirectShow(void)
{
	
}

CDirectShow::~CDirectShow(void)
{
}
void CDirectShow::Init()
{
	//initialize the graph manager
	pGB = NULL;
	//initialize the media controller
	pMC = NULL;
	//initialize the video window
	pVW = NULL;
	//intiaize the video event handler
	pME = NULL;

	//set fullscreen to false
	m_bIsFullScreen = false;
	
}
void CDirectShow::ShutDown()
{
	//pVW->SetWindowForeground(0);
	pMC->Stop();
	//set the window to play behind the game window
	//pVW->SetWindowForeground(0);
	//set playing to false;
	m_bIsPlaying = false;

	//release the media controller
	if(pMC)
	{
		pMC->Release();
		pMC = NULL;
	}
	//release the video window
	if(pVW)
	{
		pVW->put_Visible(OAFALSE);
		pVW->put_Owner(NULL);
		pVW->Release();
	}
	//release the media event handler
	if(pME)
	{
		pME->Release();
		pME = NULL;
	}
	//release the graph builder
	if(pGB)
	{
		pGB->Release();
		pGB = NULL;
	}
	//unitialize the com interface
	CoUninitialize();
	//update the game window
	if(!CGame::GetInstance()->GetIsWindowed())
		CSGD_Direct3D::GetInstance()->ChangeDisplayParam(800, 600, CGame::GetInstance()->GetIsWindowed());
}

void CDirectShow::Play(LPCWSTR lstrFilename)
{	
	//initialize the com interface
	CoInitialize(NULL);

	//This creates the filter graph manager
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC,
	IID_IGraphBuilder, (void **)&pGB);

	//Query COM interface
	pGB->QueryInterface(IID_IMediaControl, (void **)&pMC);
	pGB->QueryInterface(IID_IVideoWindow, (void **)&pVW);
	
	//set is playing to true
	m_bIsPlaying = true;

	//build the streming video file
	pGB->RenderFile(lstrFilename,NULL);
	//Set the parent window
	pVW->put_Owner((OAHWND)g_hwnd);
	g_hwnd = CGame::GetInstance()->GetWindowHandle();
	//Set the style to the child window
	pVW->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	//here we create a RECT in which to draw the child window on
	//make a rect to store the parent information 
	RECT vwrect;	
	//get the size of the Parent
	GetClientRect(g_hwnd,&vwrect);
	//get an set the actual position in the window to display at
	POINT actual;
	actual.x = 0;
	actual.y = 0;
	ScreenToClient(g_hwnd, &actual);
	actual.x *= -1;
	actual.y *= -1;
	//Set the Child to this position
	pVW->SetWindowPosition(actual.x,actual.y,vwrect.right,vwrect.bottom);
	//hide the cursor while video plays
	pVW->HideCursor(OATRUE);
	//set the video to play on top of the game window
	//pVW->SetWindowForeground(-1);
	//query the graph builder for the event its sending
	pGB->QueryInterface(IID_IMediaEventEx, (void**)&pME);
	//send the message to the parent window
	pME->SetNotifyWindow((OAHWND)g_hwnd,WM_GRAPHNOTIFY, 0);
	//send the graphic builder info to the media controller
	pGB->QueryInterface(IID_IMediaControl, (void **)&pMC);
	//play the video
	pMC->Run();
}
void CDirectShow::Update(float fElapsedTime)
{
	////if the video is playing
	//if(m_bIsPlaying)
	//{
	//	//set the window to play on top of the game window
	//	//pVW->SetWindowForeground(OATRUE);
	//	//set the window handle
	//	g_hwnd = CGame::GetInstance()->GetWindowHandle();
	//	//make a rect to store the parent window info
	//	RECT vwrect;	
	//	//get the size of the Parent
	//	GetClientRect(g_hwnd,&vwrect);
	//	//get an set the actual position in the window to display at
	//	POINT actual;
	//	actual.x = 0;
	//	actual.y = 0;
	//	ScreenToClient(g_hwnd, &actual);
	//	actual.x *= -1;
	//	actual.y *= -1;
	//	//if not in fullscreen
	//	if(!m_bIsFullScreen)
	//		//set the screen to fit nside the game window
	//		pVW->SetWindowPosition(actual.x,actual.y, vwrect.right,vwrect.bottom);//actual.x + 120,actual.y + 182);//648, 515);
	//	else
	//		//if in fullscreen set to the resolution of the screen 
	//		pVW->SetWindowPosition(0,0, GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	//}

	if(CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_RETURN))
	{
		pVW->put_Visible(OAFALSE);
		Stop();
		ShowWindow(g_hwnd,SW_SHOW);
		SetFocus(g_hwnd);
		m_bIsPlaying = false;
	}


}
void CDirectShow::Stop()
{

	//stop the video
	//pMC->Stop();
	
	
	//initialize the graph manager
	pGB = NULL;
	//initialize the media controller
	pMC = NULL;
	//initialize the video window
	pVW = NULL;
	//intiaize the video event handler
	pME = NULL;
}