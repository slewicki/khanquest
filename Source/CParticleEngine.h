//////////////////////////////////////////////////////////////////////////
// File CParticleEngine.h
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CParticleEngine Class, draws particles
//			to the screen using the CEmitter Class
//////////////////////////////////////////////////////////////////////////
#pragma once
#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CEmitter.h"
#include "CParticle.h"
#include "CCamera.h"

#define RAND_FLOAT(min,max) (((rand()/(float)RAND_MAX)*((max)-(min)))+(min))

class CParticleEngine
{

	CSGD_Direct3D*			m_pD3D;			// direct 3D
	CSGD_TextureManager*	m_pTM;			// Texture Manager
	CCamera*				m_pCamera;		// Camera

	int						m_nEmitterXLocation; // X location to draw the emitter
	int						m_nEmitterYLocation; // X location to draw the emitter

	float 					m_fPercentAlpha;	
	float 					m_fPercentRed;	
	float 					m_fPercentGreen;	
	float 					m_fPercentBlue;	

	float					m_fPercentScale;
	float					m_fPercentGravity;

	float					m_fUpdateFloat;	
	float					m_fUpdateQuarter;
	int						m_nImageID;
	int						m_nPartCount;
	float					m_fPartTimer;

	// update colors
	D3DXCOLOR			m_cStartColor;
	D3DXCOLOR			m_cEndColor;
	D3DXCOLOR			m_cCurrentColor;
	float				m_fColorPercent;
	int					m_nBlendMode;

	bool				m_bIsRunning;
	DWORD source;
	DWORD dest;

	CParticle				particle;		// single particle

		////////////////////////////////////////////
		//	Function:	"CParticleEngine(Constructor)"
		//	Last Modified: July 18, 2008
		//	Purpose:	Proper singleton
		////////////////////////////////////////////
		CParticleEngine(void);
		//CParticleEngine(const CParticleEngine&);
		//CParticleEngine& operator=(const CParticleEngine&);

		////////////////////////////////////////////
		//	Function:	"~CWorldMapState(Destructor)"
		//	Last Modified: July 18, 2008
		//	Purpose:	Proper singleton
		////////////////////////////////////////////
		~CParticleEngine(void);

		//////////////////////////////////////////////////////////////////////////
		// Function: “UpdateLoc”
		//	Last Modified: July 27, 2008
		// Purpose: Update the Location of the particle
		//////////////////////////////////////////////////////////////////////////
		void UpdateLoc(int i, int j);

	public:

		CEmitter							m_ceEmitter;
		//vector<CParticle>					vParticleList;	// vector of particles
		vector<CEmitter>					vEmitterList;	// vector of emitters
		//////////////////////////////////////////////////////
		// Function: “GetInstance”
		//	Last Modified: July 18, 2008
		// Purpose: To return an instance of the class
		//////////////////////////////////////////////////////
		static CParticleEngine* GetInstance(void)
		{
			static CParticleEngine instance;

			return &instance;
		}


		//////////////////////////////////////////////////////
		//  Function: Accessors
		//	Last Modified: July 23, 2008
		//  Purpose : Returns the specified type.
		//////////////////////////////////////////////////////
		bool GetIsRunning(void){return m_bIsRunning;}


		//////////////////////////////////////////////////////
		//  Function: Modifiers
		//	Last Modified: July 23, 2008
		//  Purpose : Modifies the specified type.
		//////////////////////////////////////////////////////
		void ClearEmitter(void){vEmitterList.clear();}

		//////////////////////////////////////////////////////
		//	Function: “Exit”
		//	Last Modified: July 18, 2008
		//	Purpose: To unload any needed information
		//////////////////////////////////////////////////////
		void Exit(void);

		//////////////////////////////////////////////////////
		//	Function: “Update”
		//	Last Modified: July 27, 2008
		//	Purpose: To update our information after input
		//////////////////////////////////////////////////////
		void Update(float fElapsedTime);

		//////////////////////////////////////////////////////
		//	Function: “Render”
		//	Last Modified: July 27, 2008
		//	Purpose: To render our information to the screen
		//////////////////////////////////////////////////////
		void Render(float fElapsedTime);

		//////////////////////////////////////////////////////
		//	Function: “LoadEmitter”
		//	Last Modified: August 2, 2008
		//	Purpose: To Load an Emitter into the CParticle Engine
		//////////////////////////////////////////////////////
		int LoadXmlEmitter(char* emitterFileName , float locX, float locY);

		//////////////////////////////////////////////////////
		//	Function: “LoadBineryEmitter”
		//	Last Modified: August 2, 2008
		//	Purpose: To Load an Emitter into the CParticle Engine
		//////////////////////////////////////////////////////
		int LoadBineryEmitter(char* emitterFileName, float locX, float locY);

		//////////////////////////////////////////////////////
		//	Function: “SetEmitterVel”
		//	Last Modified: July 18, 2008
		//	Purpose: Set the velocity of the emitter
		//	if it needs to move independently.
		//////////////////////////////////////////////////////
		void SetEmitterVel(float XVel, float YVel);

		//////////////////////////////////////////////////////////////////////////
		// Function: “UpdateAge”
		//	Last Modified: July 27, 2008
		// Purpose: Update the age of the particle
		//////////////////////////////////////////////////////////////////////////
		void UpdateAge(int i, int j);

		//////////////////////////////////////////////////////////////////////////
		// Function: “UpdateColor”
		//	Last Modified: July 27, 2008
		// Purpose: Update the color of the particle
		//////////////////////////////////////////////////////////////////////////
		void UpdateColor(int i, int j);

		//////////////////////////////////////////////////////////////////////////
		// Function: “UpdateScale”
		//	Last Modified: July 27, 2008
		// Purpose: Update the scale of the particle
		//////////////////////////////////////////////////////////////////////////
		void UpdateScale(int i, int j);

		//////////////////////////////////////////////////////////////////////////
		// Function: “GetBlendMode”
		//	Last Modified: July 27, 2008
		// Purpose: Update the Location of the particle
		//////////////////////////////////////////////////////////////////////////
		int GetBlendMode(string mode);

		//////////////////////////////////////////////////////////////////////////
		// Function: “SetPostion”
		//	Last Modified: August 2, 2008
		// Purpose: Update the Location of the particle
		//////////////////////////////////////////////////////////////////////////
		void SetPostion(float XPos, float YPos, int ID);

		//////////////////////////////////////////////////////////////////////////
		// Function: “SetIsRunning”
		//	Last Modified: August 2, 2008
		// Purpose: Set Emitter off or on
		//////////////////////////////////////////////////////////////////////////
		void SetIsRunning(int ID, bool isRunnig);

		//////////////////////////////////////////////////////////////////////////
		// Function: “SetIsRunning”
		//	Last Modified: August 4, 2008
		// Purpose: Set Emitter off or on
		//////////////////////////////////////////////////////////////////////////
		void UnLoadEmitter(int ID);


	};