//==================================================================================================
//
//shadow.cpp
//Author:ϋόRη
//
//==================================================================================================

#include "main.h"
#include "shadow.h"
#include "input.h"

//}Nθ`
#define NUM_TEX					(2)											//eNX`
#define SHADOW_TEX_FILE			"data\\TEXTURE\\shadow000.jpg"		//eNX`t@CΌ
#define SHADOW_TEX_FILE2		"data\\TEXTURE\\shadow001.jpg"		//eNX`t@CΌ
#define SHADOW_WIDTH			(50)										//
#define SHADOW_HEIGHT			(50)										//³
#define MAX_SHADOW				(256)										//eΜΕε

//O[oΟιΎ
LPDIRECT3DTEXTURE9 g_apTextureShadow[NUM_TEX] = {};	//eNX`ΦΜ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//Έ_obt@ΦΜ|C^
Shadow g_aShadow[MAX_SHADOW];						//\’ΜΜξρ

//==================================================================================================
//ϊ»
//==================================================================================================
void InitShadow(void)
{
	//foCX|C^έθ
	LPDIRECT3DDEVICE9 pDevice;

	//foCXΜζΎ
	pDevice = GetDevice();

	//eNX`Ηέέ
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEX_FILE,
		&g_apTextureShadow[0]);

	//eNX`Ηέέ
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEX_FILE2,
		&g_apTextureShadow[1]);

	//Έ_obt@ΜΆ¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//ξρζΎ
	Shadow *pShadow = GetShadow();

	//Έ_ξρΜ|C^
	VERTEX_3D *pVtx;

	//Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		//ΟΜϊ»
		ZeroMemory(pShadow, sizeof(Shadow));

		//Έ_ΐWΜέθ
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//@όxNgΜέθ
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//Έ_J[Μέθ
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//eNX`ΐW
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//|C^πiίι
		pVtx += 4;
	}

	//Έ_obt@πAbN
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================================
//IΉ
//==================================================================================================
void UninitShadow(void)
{
	for (int nCntTex = 0;nCntTex < NUM_TEX;nCntTex++)
	{//eNX`jό
		if (g_apTextureShadow[nCntTex] != NULL)
		{
			g_apTextureShadow[nCntTex]->Release();
			g_apTextureShadow[nCntTex] = NULL;
		}
	}
	

	if (g_pVtxBuffShadow != NULL)
	{//Έ_obt@|C^Μjό
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//==================================================================================================
//XV
//==================================================================================================
void UpdateShadow(void)
{

}

//==================================================================================================
//`ζ
//==================================================================================================
void DrawShadow(void)
{
	//foCX|C^έθ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//ΈZ¬Ιέθ
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ξρζΎ
	Shadow *pShadow = GetShadow();

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse == true)
		{//gp΅Δ’ιΘη
		 //[h}gbNXϊ»
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			//ό«π½f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			//Κuπ½f
			D3DXMatrixTranslation(&mtxTrans,
				pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			//[h}gbNXέθ
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			//Έ_obt@πf[^Xg[Ιέθ
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//Έ_tH[}bgΜέθ
			pDevice->SetFVF(FVF_VERTEX_3D);

			//eNX`έθ
			pDevice->SetTexture(0, g_apTextureShadow[pShadow->type]);

			//|S`ζ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);

		}
	}

	//ΑZ¬Ιί·
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================================================
//έθ
//==================================================================================================
int SetShadow(float fRadius,SHADOWTYPE type, float fWidth, float fHeight ,D3DXVECTOR3 rot)
{
	//ΟιΎ
	int nCntShadow = -1;

	//ξρζΎ
	Shadow *pShadow = GetShadow();

	//Έ_ξρΜ|C^
	VERTEX_3D *pVtx;

	//Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++, pShadow++)
	{
		if (pShadow->bUse == false)
		{//gp΅Δ’Θ’σΤΘη

			//Όaγό
			pShadow->rot = rot;
			pShadow->fRadius = fRadius;
			pShadow->fWidth = fWidth;
			pShadow->fHeight = fHeight;
			pShadow->type = type;

			//Έ_ΐWΜέθ
			if (type == SHADOWTYPE_SQUARE)
			{
				pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
				pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
				pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
				pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(-pShadow->fRadius, 0.0f, pShadow->fRadius);
				pVtx[1].pos = D3DXVECTOR3(pShadow->fRadius, 0.0f, pShadow->fRadius);
				pVtx[2].pos = D3DXVECTOR3(-pShadow->fRadius, 0.0f, -pShadow->fRadius);
				pVtx[3].pos = D3DXVECTOR3(pShadow->fRadius, 0.0f, -pShadow->fRadius);
			}

			//@όxNgΜέθ
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//Έ_J[Μέθ
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//eNX`ΐW
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			nCntShadow = nCount;

			//gpσΤΙ·ι
			pShadow->bUse = true;

			//forΆπ²―ι
			break;
		}

		//|C^πiίι
		pVtx += 4;
	}

	//Έ_obt@πAbN
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//==================================================================================================
//Κuέθ
//==================================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	//gp§ΐ
	if (nIdxShadow == -1)
	{
		return;
	}

	//ΟιΎ
	float fRadius, fHeight, fWidth;

	//ΚuEό«Μσ―ζθ
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].rot = rot;

	//Έ_ξρΜ|C^
	VERTEX_3D *pVtx;

	//Έ_obt@πbN΅AΈ_ξρΦΜ|C^πζΎ
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//|C^πiίι
	pVtx += 4 * nIdxShadow;

	//Έ_ΐWΜέθ
	switch (g_aShadow[nIdxShadow].type)
	{
	case SHADOWTYPE_CIRCLE:
		fRadius = g_aShadow[nIdxShadow].fRadius * fScale;

		pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f, fRadius);
		pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, fRadius);
		pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
		pVtx[3].pos = D3DXVECTOR3(fRadius, 0.0f, -fRadius);
		break;

	case SHADOWTYPE_SQUARE:
		fHeight = g_aShadow[nIdxShadow].fHeight * fScale;
		fWidth = g_aShadow[nIdxShadow].fWidth * fScale;

		pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
		pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
		pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
		pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);
		break;
	}
	
	//Έ_obt@πAbN
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================================
//ξρζΎ
//==================================================================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}

//==================================================================================================
//eLψ³ψΗ
//==================================================================================================
void EnableShadow(int nIdxShadow, bool bUse)
{
	g_aShadow[nIdxShadow].bUse = bUse;
}

//==================================================================================================
//eΜfobO
//==================================================================================================
void DebugShadow()
{
	RECT rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };	//κ
	char aStr[1][256];					//
	LPD3DXFONT Font = GetFont();		//CΜβΒ
	int nUseCnter = 0;

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{
			nUseCnter++;
		}
	}
	
	//ΆρΙγό
	sprintf(&aStr[0][0], "eΜF%d", nUseCnter);

	for (int nCntDebug = 0; nCntDebug < 1; nCntDebug++)
	{
		Font->DrawText(NULL, &aStr[nCntDebug][0], -1, &rect, DT_RIGHT, D3DXCOLOR(0.7f, 0.0f, 0.7f, 1.0f));
		rect.top += 20;
	}
}
