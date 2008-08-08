#pragma once

#include <windows.h>
#include "dshow.h"

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "quartz.lib")
#define WM_GRAPHNOTIFY WM_APP + 1

class CDirectShow
{
	private:
	//The filter graph manager, builds the streaming media file.
	IGraphBuilder *pGB;
	//Handles media streeaming in the filter graph
	IMediaControl *pMC;
	//The window the video plays in
	IVideoWindow *pVW;
	//Handles the events for the filter graph
	IMediaEventEx *pME;
	//The handle to the window the video will play in
	HWND g_hwnd;
	//bool to see if the video is playing or not
	bool m_bIsPlaying;
	//bool to see if the video is in fullscreen or not
	bool m_bIsFullScreen;

	/////////////////////////////////
	// Function: "DirectShow"
	//
	// Purpose: Constructor for the class.	
	////////////////////////////////
	CDirectShow();
	///////////////////////////////
	// Function: "~CDirectShow"
	//
	// Purpose: Destructor for the class.		
	////////////////////////////////	
	~CDirectShow();
	///////////////////////////////
	// Function: "CDirectShow"
	//
	// Purpose: Copy constructor for the class.	
	////////////////////////////////
	CDirectShow(const CDirectShow&);
	///////////////////////////////
	// Function: "operator="
	//
	// Purpose: Assignment operator for the class.		
	////////////////////////////////
	CDirectShow& operator=(const CDirectShow&);


public:

	///////////////////////////////
	// Function: "GetInstance"
	//
	// Purpose: Gets the Current instance of the 
	//			Direct Show singleton.	
	////////////////////////////////
	static CDirectShow* GetInstance(void)
	{
		static CDirectShow instance;
		return &instance;
	}

	///////////////////////////////
	// Function: "GetMediaEvent"
	//
	// Purpose:  Gets the IMediaEvent pointer from the instance 
	//				
	////////////////////////////////
	IMediaEventEx* GetMediaEvent()	{return pME;}
	///////////////////////////////
	// Function: "GetVideoWindow"
	//
	// Purpose: Gets the IVideoWindow pointer from the instance 
	//				
	////////////////////////////////
	IVideoWindow*  GetVideoWindow()	{return pVW;}

	///////////////////////////////
	// Function: "Init"
	//
	// Purpose: Initializes the instance DirectShow
	//			
	////////////////////////////////
	void Init();
	///////////////////////////////
	// Function: "ShutDown"
	//
	// Purpose: Shuts down the instance of direct show
	//				
	////////////////////////////////
	void ShutDown();

	///////////////////////////////
	// Function: "SetWindowed"
	//
	// Purpose: Sets if the window is windowed or not
	//			
	////////////////////////////////
	void SetWindowed(bool bWindowed)	{m_bIsFullScreen = bWindowed;}
	///////////////////////////////
	// Function: "SetFullScreen"
	//
	// Purpose: Sets if the window is fulscreen or not
	//			
	////////////////////////////////
	void SetFullScreen(bool bIsFullScreen)	{m_bIsFullScreen = bIsFullScreen;}
	///////////////////////////////
	// Function: "GetIsWindowed"
	//
	// Purpose: Gets if the window is fullscreen or not
	//			
	////////////////////////////////
	bool GetIsWindowed()	{return m_bIsFullScreen;}
	///////////////////////////////
	// Function: "GetIsPlaying"
	//
	// Purpose: Gets if the video is playing
	//
	////////////////////////////////
	bool GetIsPlaying()		{return m_bIsPlaying;}
	
	///////////////////////////////
	// Function: "Play"
	//
	// Purpose: Plays the vidoe file passed in
	//			
	////////////////////////////////
	void Play(LPCWSTR lstrFilename);
	///////////////////////////////
	// Function: "Stop"
	//
	// Purpose: Stops the video
	//				
	////////////////////////////////
	void Stop();
	///////////////////////////////
	// Function: "Update"
	//
	// Purpose: Updates the video 
	//				
	////////////////////////////////
	void Update();
};
