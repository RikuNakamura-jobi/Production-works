//==================================================================================================
//
//gauge.cpp
//Author:ûüRç
//
//==================================================================================================

//===============================
// CN[h
//===============================
#include "main.h"
#include "gauge.h"
#include "player.h"

//===============================
// }Nè`
//===============================
#define NUM_TEX		(2)	//eNX`Ì
#define MAX_GAUGE	(16)	//Q[WÌÅå(Q[WÆgª é©çñ{·é)

#define PARAM_FACT	(0.1f)	//p[^[Ìµ«ÌW
#define MULTIPLE_POLYGON	(4)	//|SÌ{@Q[WAgAwiA·ªÌFá¢Ìª
#define DIFF_RANGE	(0.001f)	//·ªQ[Wª®«o·ÜÅÌ·ªÍÍ
#define RATE_FRAME	(2.0f)	//gÌQ[WÌ

//===============================
// O[oÏé¾
//===============================
LPDIRECT3DTEXTURE9 g_pTextureGauge[NUM_TEX] = {};								//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;									//¸_obt@ÖÌ|C^
const char *c_apFilenameGauge[NUM_TEX] =
{
	"data\\TEXTURE\\life000.png",
	"data\\TEXTURE\\gauge_frame.png"
};																				//eNX`t@C¼
Gauge g_aGauge[MAX_GAUGE];	//Q[WÌÅå

//==================================================================================================
//ú»
//==================================================================================================
void InitGauge(void)
{
	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//foCXÌæ¾
	pDevice = GetDevice();

	//îñæ¾
	Gauge *pGauge = GetGauge();

	for (int nCntTex = 0;nCntTex < NUM_TEX;nCntTex++)
	{
		//eNX`ÇÝÝ
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameGauge[nCntTex],
			&g_pTextureGauge[nCntTex]);
	}

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGE * MULTIPLE_POLYGON,	
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//Ïú»
	ZeroMemory(pGauge, sizeof(Gauge) * MAX_GAUGE);

	for (int nCntGauge = 0;nCntGauge < MAX_GAUGE * MULTIPLE_POLYGON;nCntGauge++,pGauge++)
	{
		//¸_îñú»=====================
		//¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwÌÝè
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//¸_J[ÌÝè
		pVtx[0].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[1].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[2].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[3].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//|C^ðißé
		pVtx += 4;
		//¸_îñú»=====================
	}

	//¸_obt@ðAbN
	g_pVtxBuffGauge->Unlock();
}

//==================================================================================================
//I¹
//==================================================================================================
void UninitGauge(void)
{
	for (int nCntGauge = 0;nCntGauge < NUM_TEX;nCntGauge++)
	{
		if (g_pTextureGauge[nCntGauge] != NULL)
		{//eNX`ÖÌ|C^Ìjü
			g_pTextureGauge[nCntGauge]->Release();
			g_pTextureGauge[nCntGauge] = NULL;
		}
	}

	if (g_pVtxBuffGauge != NULL)
	{//¸_obt@ÖÌ|C^Ìjü
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//==================================================================================================
//XV
//==================================================================================================
void UpdateGauge(void)
{
	//Ïé¾
	float fLength;
	float fGaugeDiff;

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//îñæ¾
	Gauge *pGauge = GetGauge();

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse)
		{//gp³êÄ¢½ç
			//p[^[ðÚIÌlÉñ¹é
			fGaugeDiff = pGauge->fParamDest - pGauge->fParam;

			if (fGaugeDiff > 0.0f)
			{//·ªªvXÌê
				pGauge->fParam += pGauge->fParamMax * pGauge->fRateChange;

				if (pGauge->fParam > pGauge->fParamDest)
				{//ÚWÌlÉâ³
					pGauge->fParam = pGauge->fParamDest;
				}
			}
			else if(fGaugeDiff < 0.0f)
			{//·ªª}CiXÌê
				pGauge->fParam -= pGauge->fParamMax * pGauge->fRateChange;

				if (pGauge->fParam < pGauge->fParamDest)
				{//ÚWÌlÉâ³
					pGauge->fParam = pGauge->fParamDest;
				}
			}

			//pGauge->fParam += fGaugeDiff * PARAM_FACT;

			//p[^[ÌðZo
			fLength = pGauge->fParam / pGauge->fParamMax;

			//¸_J[ÌÝè
			pVtx[0].col = pGauge->col;
			pVtx[1].col = pGauge->col;
			pVtx[2].col = pGauge->col;
			pVtx[3].col = pGauge->col;

			//íÞ²ÆÌ¸_îñÌÝè
			switch (pGauge->type)
			{
			case GAUGETYPE_VERTICAL:
				//¼^Cv=======================
				//¸_ÀWÌÝè
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y, 0.0f);

				//eNX`ÀW
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * fLength);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f * fLength);

				//Q[W·ªÜÅ|C^ðißé
				pVtx += 4 * MAX_GAUGE * 3;

				//Q[W·ªÌ¸_îñÝè
				//¸_ÀWÌÝè
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);

				//·ªQ[WÌû©
				if (fGaugeDiff * fGaugeDiff < DIFF_RANGE * DIFF_RANGE)
				{//·ªªêèÈºÌÆ«ÌÝXV
					pVtx[0].pos += (pVtx[2].pos - pVtx[0].pos) * PARAM_FACT;
					pVtx[1].pos += (pVtx[3].pos - pVtx[1].pos) * PARAM_FACT;
				}
				//¼^Cv=======================
				break;

			case GAUGETYPE_HORIZON:
				//½^Cv=======================
				//¸_ÀWÌÝè
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);

				//eNX`ÀW
				pVtx[0].tex = D3DXVECTOR2(1.0f - fLength, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f - fLength, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//Q[W·ªÜÅ|C^ðißé
				pVtx += 4 * MAX_GAUGE * 3;

				//Q[W·ªÌ¸_îñÝè
				//¸_ÀWÌÝè
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);

				//·ªQ[WÌû©
				if (fGaugeDiff * fGaugeDiff < DIFF_RANGE * DIFF_RANGE)
				{//·ªªêèÈºÌÆ«ÌÝXV
					pVtx[1].pos += (pVtx[0].pos - pVtx[1].pos) * PARAM_FACT;
					pVtx[3].pos += (pVtx[2].pos - pVtx[3].pos) * PARAM_FACT;
				}
				
				//½^Cv=======================
				break;
			default:
				break;
			}

			//|C^ðß·
			pVtx -= 4 * MAX_GAUGE * 3;
		}

		//|C^ðißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffGauge->Unlock();
}

//==================================================================================================
//`æ
//==================================================================================================
void DrawGauge(void)
{
	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//foCXÌæ¾
	pDevice = GetDevice();

	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	//îñæ¾
	Gauge *pGauge = GetGauge();

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse)
		{//gp³êÄ¢½ç
			//wiÌ`æ=========================
			//eNX`Ýè
			pDevice->SetTexture(0, NULL);

			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4 * 2, 2);

			//·ªQ[WÌ`æ=========================
			//eNX`Ýè
			pDevice->SetTexture(0, NULL);

			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4 * 3, 2);

			//Q[WÌ`æ=========================
			//eNX`Ýè
			pDevice->SetTexture(0, g_pTextureGauge[0]);

			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);

			//gÌ`æ=========================
			//eNX`Ýè
			pDevice->SetTexture(0, g_pTextureGauge[1]);

			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4, 2);
		}
	}
}

//==================================================================================================
// Ýè
//==================================================================================================
int SetGauge(D3DXVECTOR3 pos,float fParamMax, float height, float width, GAUGETYPE type, float fRateChange)
{
	//Ïé¾
	int nCntGauge = -1;

	//îñæ¾
	Gauge *pGauge = GetGauge();

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_GAUGE; nCount++, nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == false)
		{//gpµÄ¢È¢óÔÈç

			//øÌó¯æè
			pGauge->fParamMax = fParamMax;
			pGauge->fParam = fParamMax;
			pGauge->fParamDest = fParamMax;
			pGauge->fRateChange = fRateChange;
			pGauge->height = height;
			pGauge->width = width;
			pGauge->pos = pos;
			pGauge->type = type;

			//Q[WÌÝè========================================
			//¸_ÀWÌÝè
			pVtx[0].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y, 0.0f);

			//rhwÌÝè
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//¸_J[ÌÝè
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//eNX`ÀW
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//Q[WÌÝè========================================

			//gÜÅ|C^ðißé
			pVtx += 4 * MAX_GAUGE;

			//gÌÝè========================================
			//¸_ÀWÌÝè
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + height * RATE_FRAME, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + height * RATE_FRAME, pos.y + height * 0.5f, 0.0f);

			//rhwÌÝè
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//¸_J[ÌÝè
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//eNX`ÀW
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//gÌÝè========================================

			//wiÜÅ|C^ðißé
			pVtx += 4 * MAX_GAUGE;

			//wiÌÝè========================================
			//¸_ÀWÌÝè
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height * 0.5f, 0.0f);

			//rhwÌÝè
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//¸_J[ÌÝè
			pVtx[0].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[1].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[2].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[3].col = D3DCOLOR_RGBA(200, 200, 200, 200);

			//eNX`ÀW
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//wiÌÝè========================================

			//·ªQ[WÜÅ|C^ðißé
			pVtx += 4 * MAX_GAUGE;

			//·ªQ[WÌÝè========================================
			//¸_ÀWÌÝè
			pVtx[0].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);

			//rhwÌÝè
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//¸_J[ÌÝè
			pVtx[0].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 215, 0, 255);

			//eNX`ÀW
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//·ªQ[WÌÝè========================================

			nCntGauge = nCount;

			//gpóÔÉ·é
			pGauge->bUse = true;

			//for¶ð²¯é
			break;
		}

		//|C^ðißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffGauge->Unlock();

	return nCntGauge;
}

//==================================================================================================
// îñÝè
//==================================================================================================
void SetInfoGauge(int nIdx, D3DXCOLOR col, float fParam)
{
	if (nIdx == -1)
	{//jóh~
		return;
	}

	//îñæ¾
	Gauge *pGauge = GetGauge();

	//FÌÝè
	pGauge[nIdx].col = col;

	//p[^[ÌÝè
	pGauge[nIdx].fParamDest = fParam;
}

//==================================================================================================
// îñæ¾
//==================================================================================================
Gauge *GetGauge(void)
{
	return &g_aGauge[0];
}