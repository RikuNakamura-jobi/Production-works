//==========================================================
//
//�t�H�O���� [fog.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FOG_H_
#define _FOG_H_

#include "main.h"

//==========================================================
//�\���̂̒�`
//==========================================================
typedef struct
{
	D3DCOLOR col;	//�F
	float fStart;	//�J�n�_
	float fEnd;		//�I���_
	float fDensity;	//�t�H�O���x
}Fog;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitFog(void);
void UninitFog(void);
void UpdateFog(void);
void DrawFog(void);
void ResetStateFog(void);

#endif
