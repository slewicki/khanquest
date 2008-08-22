//////////////////////////////////////////////////////////////////////////
// File CParticle.h
//
// Author: Rodney Kite (RK)
//
// Purpose: Contains class definition for CParticle Class, which holds
//			all of the info for a particle
//////////////////////////////////////////////////////////////////////////
#pragma once
#include <string>
#include "CSGD_TextureManager.h"
#include "CProfile.h"
using std::string;

class CParticle
{

public:

	bool	m_bAlive;
	bool	m_bContinuous;
	float	m_fPartLocX ;
	float	m_fPartLocY ;
	int		m_nOffsetX ;
	int		m_nOffsetY ;
	float	m_fMaxParticles;
	float	m_fLife;
	float	m_fMaxLife;
	float	m_fMinLife ;
	float	m_fAge;

	float	m_fVelX;
	float	m_fVelY;

	float	m_fStartAlpha;
	float	m_fStartRed;
	float	m_fStartGreen;
	float	m_fStartBlue;

	float	m_fCurrentAlpha;
	float 	m_fCurrentRed;
	float 	m_fCurrentGreen;
	float 	m_fCurrentBlue;

	float 	m_fEndAlpha;
	float 	m_fEndRed ;
	float 	m_fEndGreen;
	float 	m_fEndBlue;

	float 	m_fStartScaleX;
	float 	m_fCurrentScaleX;
	float 	m_fEndScaleX;

	float 	m_fStartScaleY;
	float 	m_fCurrentScaleY;
	float 	m_fEndScaleY;

	float	m_fGravityX;
	float	m_fGravityY;

	string	m_szSourceBlend;
	string	m_szDestBlend;

	D3DXCOLOR	m_cStartColor;
	D3DXCOLOR	m_cEndColor;
	D3DXCOLOR	m_cCurrentColor;

	string	m_szTrigger;
	string	m_szFileName;
	int		m_nImageID;
	//////////////////////////////////////////////////////////////////////////
	//  Function: CParticle
	// Last Modified: July 24, 2008
	//  Purpose : Default constructor
	//////////////////////////////////////////////////////////////////////////
	CParticle(void);

	//////////////////////////////////////////////////////////////////////////
	//  Function: ~CParticle
	// Last Modified: July 24, 2008
	//  Purpose : Default constructor
	//////////////////////////////////////////////////////////////////////////
	~CParticle(void);

	//////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	// Last Modified: July 24, 2008
	//  Purpose : Returns the specified type.
	//////////////////////////////////////////////////////////////////////////
	bool	IsAlive(void)			{return m_bAlive;}
	bool	GetIsContinuous(void)	{return m_bContinuous;}
	float	GetLocX(void)			{return m_fPartLocX;}
	float	GetLocY(void)			{return m_fPartLocY;}
	int		GetOffsetX(void)		{return	m_nOffsetX;}
	int		GetOffsetY(void)		{return m_nOffsetY;}
	float	GetMaxParticles(void)	{return	m_fMaxParticles;}
	float	GetLife(void)			{return	m_fLife;}
	float	GetMaxLife(void)		{return	m_fMaxLife;}
	float	GetMinLife(void)		{return m_fMinLife;}
	float	GetAge(void)			{return	m_fAge;}

	float	GetVelX(void)			{return m_fVelX;}
	float	GetVelY(void)			{return m_fVelY;}

	float	GetStartAlpha(void)		{return m_fStartAlpha;}
	float	GetStartRed(void)		{return m_fStartRed;}
	float	GetStartGreen(void)		{return m_fStartGreen;}
	float	GetStartBlue(void)		{return m_fStartBlue;}

	float	GetCurrentAlpha(void)	{return m_fCurrentAlpha;}
	float 	GetCurrentRed(void)		{return m_fCurrentRed;}
	float 	GetCurrentGreen(void)	{return m_fCurrentGreen;}
	float 	GetCurrentBlue(void)	{return m_fCurrentBlue;}

	float 	GetEndAlpha(void)		{return	m_fEndAlpha;}
	float 	GetEndRed(void)			{return m_fEndRed;}
	float 	GetEndGreen(void)		{return m_fEndGreen;}
	float 	GetEndBlue(void)		{return m_fEndBlue;}

	float 	GetStartScaleX(void)	{return m_fStartScaleX;}
	float 	GetCurrentScaleX(void)	{return m_fCurrentScaleX;}
	float 	GetEndScaleX(void)		{return m_fEndScaleX;}

	float 	GetStartScaleY(void)	{return m_fStartScaleY;}
	float 	GetCurrentScaleY(void)	{return m_fCurrentScaleY;}
	float 	GetEndScaleY(void)		{return m_fEndScaleY;}

	float	GetGravityX(void)		{return m_fGravityX;}
	float	GetGravityY(void)		{return m_fGravityY;}

	string	GetSourceBlend(void)	{return m_szSourceBlend;}
	string	GetDestBlend(void)		{return m_szDestBlend;}

	string	GetFileName(void)		{return m_szFileName;}
	int		GetImageID(void)		{return m_nImageID;}

	//////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	// Last Modified: July 24, 2008
	//  Purpose: Modifies the specified type.
	//////////////////////////////////////////////////////////////////////////
	void	SetIsAlive(bool Alive)					{m_bAlive = Alive;}
	void	SetIsContinuous(bool Continuous)		{m_bContinuous = Continuous;}
	void	SetLocX(float LocX)						{m_fPartLocX = LocX ;}
	void	SetLocY(float LocY)						{m_fPartLocY = LocY ;}
	void	SetOffsetX(int OffsetX)					{m_nOffsetX =  OffsetX;}
	void	SetOffsetY(int OffsetY)					{m_nOffsetY =  OffsetY;}
	void	SetMaxParticles(float MaxParticles)		{m_fMaxParticles = MaxParticles ;}
	void	SetLife(float Life)						{m_fLife = Life ;}
	void	SetMaxLife(float MaxLife)				{m_fMaxLife = MaxLife;}
	void	SetMinLife(float MinLife)				{m_fMinLife = MinLife;}
	void	SetAge(float Age)						{m_fAge = Age;}

	void	SetVelX(float VelX)						{m_fVelX = VelX ;}
	void	SetVelY(float VelY)						{m_fVelY = VelY ;}

	void	SetStartAlpha(float StartAlpha)			{m_fStartAlpha =  StartAlpha;}
	void	SetStartRed(float StartRed)				{m_fStartRed = StartRed ;}
	void	SetStartGreen(float StartGreen)			{m_fStartGreen = StartGreen ;}
	void	SetStartBlue(float StartBlue)			{m_fStartBlue = StartBlue ;}

	void	SetCurrentAlpha(float CurrentAlpha)		{m_fCurrentAlpha = CurrentAlpha ;}
	void 	SetCurrentRed(float CurrentRed)			{m_fCurrentRed = CurrentRed ;}
	void 	SetCurrentGreen(float CurrentGreen)		{m_fCurrentGreen =  CurrentGreen;}
	void 	SetCurrentBlue(float CurrentBlue)		{m_fCurrentBlue = CurrentBlue ;}

	void 	SetEndAlpha(float EndAlpha)				{m_fEndAlpha = EndAlpha ;}
	void 	SetEndRed(float EndRed)					{m_fEndRed = EndRed ;}
	void 	SetEndGreen(float EndGreen)				{m_fEndGreen = EndGreen ;}
	void 	SetEndBlue(float EndBlue)				{m_fEndBlue = EndBlue ;}

	void 	SetStartScaleX(float StarScaleX)		{m_fStartScaleX = StarScaleX ;}
	void 	SetCurrentScaleX(float CurrentScaleX)	{m_fCurrentScaleX = CurrentScaleX;}
	void 	SetEndScaleX(float EndScaleX)			{m_fEndScaleX =  EndScaleX;}

	void 	SetStartScaleY(float StartScaleY)		{m_fStartScaleY = StartScaleY ;}
	void 	SetCurrentScaleY(float CurrentScaleY)	{m_fCurrentScaleY =  CurrentScaleY;}
	void 	SetEndScaleY(float EndScaleY)			{m_fEndScaleY =  EndScaleY;}

	float	SetGravityX(float GravityX)				{m_fGravityX = GravityX;}
	float	SetGravityY(float GravityY)				{m_fGravityX = GravityY;}


	void	SetSourceBlend(string SourceBlend)	    {m_szSourceBlend =  SourceBlend;}
	void	SetDestBlend(string DestBlend)			{m_szDestBlend =  DestBlend;}

	void	SetFileName(string FileName)			{m_szFileName =  FileName;}
	void	SetImageID(int ImageID)					{m_nImageID =  ImageID;}

};
