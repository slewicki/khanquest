#pragma once
#include <windows.h>
#include "dshow.h"
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "quartz.lib")
#include <vector>
using std::vector;

#define WM_GRAPHNOTIFY WM_APP + 1
class CDirectShow
{
	private:

	struct tVideo
	{
		IGraphBuilder		*m_pGraphBuilder;		// Constructs the filter graph manager
		IMediaControl		*m_pMediaControl;		// Handles media streaming in filter graph
		IMediaEventEx		*m_pMediaEvent;			// Handles filter graph events
		IVideoWindow		*m_pVideoWindow;		// The window that will contain the video
		HWND				m_hWnd;					// The Main Window Handle
		bool				m_bIsFullscreen;
	};

	
	int m_nCount;
	int m_nCurrent;
	vector <tVideo> vVideos;
	//bool to see if the video is playing or not
	bool m_bIsPlaying;
	bool m_bIsFinished;


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
	
	//////////////////////////////////////////////////////
	//	Function: “Release”
	//	Last Modified: August 11, 2008
	//	Purpose: Release the video file of the passed in ID
	//////////////////////////////////////////////////////
	void Release(int nID);

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

	///////////////////////////////////////////////////////
	//	Function: “LoadVideoFile”
	//	Last Modified: August 11, 2008
	//	Purpose: Load in a video file, pass the filepath
	//			 and window to render to. Enter file as
	//			 so, with L in front: L"c:\\example.mpeg"
	//////////////////////////////////////////////////////	
	int LoadVideo(LPCWSTR szFilePath, HWND hwnd, bool bIsFullscreen);

	//////////////////////////////////////////////////////
	//	Function: “Play”
	//	Last Modified: August 11, 2008
	//	Purpose: Play the video file of the passed in ID
	//////////////////////////////////////////////////////
	bool Play(int nID);

	//////////////////////////////////////////////////////
	//	Function: “Play”
	//	Last Modified: August 11, 2008
	//	Purpose: Stop the video file of the passed in ID
	//////////////////////////////////////////////////////
	bool Stop(int nID);

	//////////////////////////////////////////////////////
	//	Function: “HandleEvent”
	//	Last Modified: August 9, 2008
	//	Purpose: Play the video file of the passed in ID
	//////////////////////////////////////////////////////
	void HandleEvent(int nID);
	
	//////////////////////////////////////////////////////
	//	Function: “Init”
	//	Last Modified: August 11, 2008
	//	Purpose: Initializes DirectShow Values
	//////////////////////////////////////////////////////
	void Init();

	//////////////////////////////////////////////////////
	//	Function: Accessors
	//	Last Modified: August 11, 2008
	//	Purpose: Returns values of a certain type
	//////////////////////////////////////////////////////
	bool IsPlaying() { return m_bIsPlaying; }
	bool IsFinished() { return m_bIsFinished; }
	int GetCurrent() { return m_nCurrent; }

	//////////////////////////////////////////////////////
	//	Function: "ShutDown"
	//	Last Modified: August 11, 2008
	//	Purpose: Shuts down all video files and DirectShow
	//////////////////////////////////////////////////////
	void ShutDown();
};
