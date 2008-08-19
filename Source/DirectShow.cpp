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
	m_nCount = 0;
	m_nCurrent = 0;
	vVideos.clear();
	CoInitialize(NULL);
	m_bIsPlaying = false;
	
}
int CDirectShow::LoadVideo(LPCWSTR szFilePath, HWND hwnd, bool bIsFullscreen)
{
	tVideo tLoad;
	m_bIsPlaying = false;
	memset(&tLoad, 0, sizeof(tLoad));

	if (FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, 
		IID_IGraphBuilder, (void **)&tLoad.m_pGraphBuilder))) 
	{
		return false;
	}
	tLoad.m_bIsFullscreen = bIsFullscreen;
	// Load this file and Build a filter graph for it.
	tLoad.m_pGraphBuilder->RenderFile(szFilePath, NULL);
	
	// Specify the owner window
	tLoad.m_hWnd = hwnd;
	tLoad.m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void **)&tLoad.m_pVideoWindow);
	tLoad.m_pVideoWindow->put_Owner((OAHWND)tLoad.m_hWnd);
	tLoad.m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
	
	// Make the Movie Take up the Entire Window.
	RECT rect;
	GetClientRect(tLoad.m_hWnd, &rect);
	if(!bIsFullscreen)
		tLoad.m_pVideoWindow->SetWindowPosition(0, 0, rect.right, rect.bottom);
	else
		tLoad.m_pVideoWindow->SetWindowPosition(0, 0, GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN));

	// Set the owner window to receive event notifications.
	tLoad.m_pGraphBuilder->QueryInterface(IID_IMediaEventEx, (void **)&tLoad.m_pMediaEvent);
	tLoad.m_pMediaEvent->SetNotifyWindow((OAHWND)tLoad.m_hWnd, WM_GRAPHNOTIFY, 0);

	tLoad.m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&tLoad.m_pMediaControl);

	vVideos.push_back(tLoad);

	return m_nCount++;
}

bool CDirectShow::Play(int nID)
{
	if (nID >= (signed)vVideos.size())
		return false;
	vVideos[nID].m_pVideoWindow->SetWindowForeground(-1);
	//if(vVideos[nID].m_bIsFullscreen)
	//	vVideos[nID].m_pVideoWindow->put_FullScreenMode(OATRUE);
	vVideos[nID].m_pMediaControl->Run();
	//InvalidateRect(vVideos[nID].m_hWnd, NULL, NULL);
	m_nCurrent = nID;
	m_bIsPlaying = true;
	return true;
}

bool CDirectShow::Stop(int nID)
{	
	if (nID >= (signed)vVideos.size())
		return false;
	m_bIsPlaying = false;
	vVideos[nID].m_pVideoWindow->put_Visible(OAFALSE);
	vVideos[nID].m_pMediaControl->Stop();

	
	//vVideos[nID].m_pVideoWindow->put_FullScreenMode(OAFALSE);
	

	return true;
}

void CDirectShow::Release(int nID)
{
	if (nID >= (signed)vVideos.size())
		return;
	
	//if (vVideos[nID].m_pMediaControl)
	//{
	//	vVideos[nID].m_pMediaControl->Stop();			// Stop the graph
	//	vVideos[nID].m_pMediaControl->Release();		// Release the Movie Controls	
	//	vVideos[nID].m_pMediaControl = NULL;			// Set to NULL for safety
	//}
	//if (vVideos[nID].m_pVideoWindow)
	//{
	//	vVideos[nID].m_pVideoWindow->put_Visible(OAFALSE);		// show no longer
	//	vVideos[nID].m_pVideoWindow->put_Owner(NULL);			// Have no Owner
	//	vVideos[nID].m_pVideoWindow->Release();						// Release the video Window
	//	vVideos[nID].m_pVideoWindow = NULL;							// NULL for safety
	//}
	//if (vVideos[nID].m_pMediaEvent)
	//{
	//	vVideos[nID].m_pMediaEvent->Release();			// release 
	//	vVideos[nID].m_pMediaEvent = NULL;				// NULL for safety
	//}
	if (vVideos[nID].m_pGraphBuilder)
	{
		vVideos[nID].m_pGraphBuilder->Release();
		vVideos[nID].m_pGraphBuilder = NULL;
	}
	
	m_nCount--;

}


void CDirectShow::HandleEvent(int nID)
{
	long evCode, param1, param2;		// parameters to be set on certain messages
	HRESULT result;							// for error tracking
	if (nID >= (signed)vVideos.size())
		return;
	
	/*if (vVideos[nID].m_pMediaEvent)
		return;*/

	// Call GetEvent until it returns a failure code, indicating that the queue is empty
	while (result = vVideos[nID].m_pMediaEvent->GetEvent(&evCode, &param1, &param2, 0), SUCCEEDED(result))
	{
		result	 = vVideos[nID].m_pMediaEvent->FreeEventParams(evCode, param1, param2);

		// if the sample stops playing or is stopped by the user then call cleanup
		if ((evCode == EC_COMPLETE) || (evCode == EC_USERABORT))
		{
			Release(nID);
			m_bIsFinished = true;
			m_bIsPlaying = false;
			break;
		}
	}
}

void CDirectShow::ShutDown()
{
	
	for (unsigned int i = 0; i < vVideos.size(); i++)
	{
		Release(i);
	}
	vVideos.clear();
	
}