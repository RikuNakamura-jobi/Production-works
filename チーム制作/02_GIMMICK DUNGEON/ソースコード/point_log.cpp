//==================================================================================================
//
//point_log.cpp
//Author:ûüRç
//
//==================================================================================================

#include "main.h"
#include "point_log.h"
#include "point.h"

//==============================
// O[oÏé¾
//==============================
#define LOG_WIDTH	(500)	//¡
#define LOG_HEIGHT	(100)	//³

//==============================
// O[oÏé¾
//==============================
LPDIRECT3DTEXTURE9 g_apTexturePointLog[LOGTYPE_MAX] = {};			//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPointLog = NULL;	//¸_obt@ÖÌ|C^
PointLog g_aPointLog[MAX_POINT_NUM];	//\¢Ìîñ
const char *c_apFilenameMessage[LOGTYPE_MAX] =
{//Xt@CpX
	"data\\TEXTURE\\PointMessage\\pointmessage_000.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_001.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_002.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_003.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_004.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_005.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_006.png",
	"data\\TEXTURE\\PointMessage\\pointmessage_007.png",
};

//==================================================================================================
// ú»
//==================================================================================================
void InitPointLog(void)
{
	//Ïú»
	ZeroMemory(&g_aPointLog[0],sizeof(PointLog) * MAX_POINT_NUM);

	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//foCXÌæ¾
	pDevice = GetDevice();

	for (int nCntTex = 0; nCntTex < LOGTYPE_MAX;nCntTex++)
	{
		//eNX`ÇÝÝ
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMessage[nCntTex],
			&g_apTexturePointLog[nCntTex]);
	}

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POINT_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPointLog,
		NULL);

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffPointLog->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPoint = 0;nCntPoint < MAX_POINT_NUM;nCntPoint++)
	{
		g_aPointLog[nCntPoint].col = D3DXCOLOR{1.0f,1.0f,1.0f,1.0f};

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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//|C^ðißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffPointLog->Unlock();
}

//==================================================================================================
//I¹
//==================================================================================================
void UninitPointLog(void)
{
	for (int nCntTex = 0; nCntTex < LOGTYPE_MAX; nCntTex++)
	{
		if (g_apTexturePointLog[nCntTex] != NULL)
		{//eNX`ÖÌ|C^Ìjü
			g_apTexturePointLog[nCntTex]->Release();
			g_apTexturePointLog[nCntTex] = NULL;
		}
	}
	
	if (g_pVtxBuffPointLog != NULL)
	{//¸_obt@ÖÌ|C^Ìjü
		g_pVtxBuffPointLog->Release();
		g_pVtxBuffPointLog = NULL;
	}
}

//==================================================================================================
// XV
//==================================================================================================
void UpdatePointLog(void)
{
	//îñæ¾
	PointLog *pPointLog = GetPointLog();

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffPointLog->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPointLog = 0; nCntPointLog < MAX_POINT_NUM; nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse)
		{//gp³êÄ¢éóÔ

			if (pPointLog->type == LOGTYPE_TOTALSCORE)
			{
				//ÎÊuÌÝè
				pPointLog->pos =
				{
					pPointLog->pPoint->pos.x - LOG_WIDTH * pPointLog->fScale * 0.7f,
					pPointLog->pPoint->pos.y,
					0.0f
				};
			}
			else
			{
				//ÎÊuÌÝè
				pPointLog->pos =
				{
					pPointLog->pPoint->pos.x - LOG_WIDTH * pPointLog->fScale,
					pPointLog->pPoint->pos.y,
					0.0f
				};
			}

			//FÌÝè
			pPointLog->col.a = pPointLog->pPoint->col.a;

			//ÊÆ«Íê¾æ
			pPointLog->bUse = pPointLog->pPoint->bUse;

			//¸_îñÌXV===========================================

			//¸_ÀWÌÝè
			pVtx[0].pos = D3DXVECTOR3(pPointLog->pos.x, pPointLog->pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pPointLog->pos.x + LOG_WIDTH * pPointLog->fScale, pPointLog->pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pPointLog->pos.x, pPointLog->pos.y + LOG_HEIGHT * pPointLog->fScale, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pPointLog->pos.x + LOG_WIDTH * pPointLog->fScale, pPointLog->pos.y + LOG_HEIGHT * pPointLog->fScale, 0.0f);

			//¸_J[ÌÝè
			pVtx[0].col = pPointLog->col;
			pVtx[1].col = pPointLog->col;
			pVtx[2].col = pPointLog->col;
			pVtx[3].col = pPointLog->col;

			//¸_îñÌXV===========================================
		}

		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffPointLog->Unlock();
}

//==================================================================================================
// `æ
//==================================================================================================
void DrawPointLog(void)
{
	//îñæ¾
	PointLog *pPointLog = GetPointLog();

	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//foCXÌæ¾
	pDevice = GetDevice();

	//¸_obt@ðf[^Xg[ÉÝè
	pDevice->SetStreamSource(0, g_pVtxBuffPointLog, 0, sizeof(VERTEX_2D));

	//¸_tH[}bgÌÝè
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPointLog = 0;nCntPointLog < MAX_POINT_NUM;nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse)
		{//gp³êÄ¢éóÔ
			//eNX`Ýè
			pDevice->SetTexture(0, g_apTexturePointLog[pPointLog->type]);

			//wiÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPointLog, 2);
		}
	}
}

//==================================================================================================
// Ýè
//==================================================================================================
void SetPointLog(Point *pPoint, LOGTYPE type, float fScale)
{
	//îñæ¾
	PointLog *pPointLog = GetPointLog();

	for (int nCntPointLog = 0;nCntPointLog < MAX_POINT_NUM;nCntPointLog++, pPointLog++)
	{
		if (pPointLog->bUse == false)
		{//gp³êÄ¢È¢Èç

			//øó¯æè
			pPointLog->pPoint = pPoint;
			pPointLog->type = type;
			pPointLog->fScale = fScale;

			//gpµÄ¢éóÔÉ·é
			pPointLog->bUse = true;

			break;
		}
	}
}

//==================================================================================================
// îñæ¾
//==================================================================================================
PointLog *GetPointLog(void)
{
	return &g_aPointLog[0];
}