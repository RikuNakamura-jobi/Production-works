//==================================================================================================
//
//effect_2D.cpp
//Author:ûüRç		edit: ûücÀË
//
//==================================================================================================

#include "main.h"
#include "effect_2D.h"

//==========================================
//  }Nè`
//==========================================
#define MAX_EFFECT2D		(4096)										//Åå
#define LIFE_SPEED		(10)										//õ½Ì¸é¬x
#define EFFECT2D_TEX_FILE		"data\\TEXTURE\\effect000.png"		//eNX`t@C¼
#define MOVE_FACT		(0.96f)										//Ú®¸W

//==========================================
//  O[oÏé¾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureEffect2D = NULL;			//eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect2D = NULL;	//¸_obt@ÖÌ|C^
Effect2D g_aEffect2D[MAX_EFFECT2D];						//GtFNgÌîñ

//==========================================
//  vg^Cvé¾
//==========================================
void UpdateEffect2DPos(Effect2D *pEffect2D);
void UpdateEffect2DPolygon(Effect2D *pEffect2D,int nCntEffect2D);
void DebugEffect2D(void);

//==================================================================================================
//ú»
//==================================================================================================
void InitEffect2D(void)
{
	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice;

	//|C^é¾
	Effect2D *pEffect2D = GetEffect2D();

	//Ïé¾
	int nCntEffect2D;

	//foCXÌæ¾
	pDevice = GetDevice();

	//eNX`ÇÝÝ
	D3DXCreateTextureFromFile(pDevice,
		EFFECT2D_TEX_FILE,
		&g_pTextureEffect2D);

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT2D,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect2D,
		NULL);

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++,pEffect2D++)
	{//SÄÌeÌú»
	    //¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//eNX`ÀW
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pEffect2D->fRadius = 0.0f;
		pEffect2D->col = D3DXCOLOR{ 1.0f,1.0f,1.0f,1.0f };
		pEffect2D->pos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->relPos = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->move = D3DXVECTOR3{ 0.0f,0.0f,0.0f };
		pEffect2D->nLife = 0;
		pEffect2D->nMaxLife = 0;
		pEffect2D->bUse = false; //GtFNgðgpµÄ¢È¢óÔÉ·é
		pEffect2D->fGrav = 0.0f;

		//¸_f[^Ì|C^ð4Âªißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//I¹
//==================================================================================================
void UninitEffect2D(void)
{
	if (g_pTextureEffect2D != NULL)
	{//eNX`ÖÌ|C^jü
		g_pTextureEffect2D->Release();
		g_pTextureEffect2D = NULL;
	}

	if (g_pVtxBuffEffect2D != NULL)
	{//¸_obt@ÖÌ|C^jü
		g_pVtxBuffEffect2D->Release();
		g_pVtxBuffEffect2D = NULL;
	}
}

//==================================================================================================
//XV
//==================================================================================================
void UpdateEffect2D(void)
{
	//Ïé¾
	int nCntEffect2D;
	
	//|C^é¾
	Effect2D *pEffect2D = GetEffect2D();

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++, pEffect2D++)
	{//SÄÌXV

		if (pEffect2D->bUse)
		{//gp³êÄ¢éóÔÈç

			//ÊuXV
			UpdateEffect2DPos(pEffect2D);
			
			//¼aÌXV(|STCYÌXV)
			pEffect2D->fRadius *= ((float)pEffect2D->nLife / (float)pEffect2D->nMaxLife);

			//õ½JEg_E
			pEffect2D->nLife--;

			if (pEffect2D->fRadius <= 0.3f)
			{
			 //gpµÄ¢È¢óÔÉ·é
				pEffect2D->bUse = false;
			}

			//|SXV
			UpdateEffect2DPolygon(pEffect2D, nCntEffect2D);
		}
	}
}

//==================================================================================================
//ÊuXV
//==================================================================================================
void UpdateEffect2DPos(Effect2D *pEffect2D)
{
	//Ú®Ê¸
	pEffect2D->move = pEffect2D->move * MOVE_FACT;

	pEffect2D->move.y += pEffect2D->fGrav;

	if (pEffect2D->pPosOwner != NULL)
	{//ÎÊuÅÌÝè
		pEffect2D->relPos.x += pEffect2D->move.x;
		pEffect2D->relPos.y += pEffect2D->move.y;
		pEffect2D->relPos.z += pEffect2D->move.z;

		pEffect2D->pos.x = pEffect2D->pPosOwner->x + pEffect2D->relPos.x;
		pEffect2D->pos.y = pEffect2D->pPosOwner->y + pEffect2D->relPos.y;
		pEffect2D->pos.z = pEffect2D->pPosOwner->z + pEffect2D->relPos.z;
	}
	else
	{//âÎÀWXV
		pEffect2D->pos += pEffect2D->move;
	}
}

//==================================================================================================
//|SXV
//==================================================================================================
void UpdateEffect2DPolygon(Effect2D *pEffect2D,int nCntEffect2D)
{
	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	//¸_f[^Ì|C^ð4Âªißé
	pVtx += 4 * nCntEffect2D;

	//¸_ÀWÌÝè
	pVtx[0].pos = { pEffect2D->pos.x - pEffect2D->fRadius, pEffect2D->pos.y - pEffect2D->fRadius, 0.0f };
	pVtx[1].pos = { pEffect2D->pos.x + pEffect2D->fRadius, pEffect2D->pos.y - pEffect2D->fRadius, 0.0f };
	pVtx[2].pos = { pEffect2D->pos.x - pEffect2D->fRadius, pEffect2D->pos.y + pEffect2D->fRadius, 0.0f };
	pVtx[3].pos = { pEffect2D->pos.x + pEffect2D->fRadius, pEffect2D->pos.y + pEffect2D->fRadius, 0.0f };

	//¸_J[ÌÝè
	pVtx[0].col = pEffect2D->col;
	pVtx[1].col = pEffect2D->col;
	pVtx[2].col = pEffect2D->col;
	pVtx[3].col = pEffect2D->col;

	//¸_obt@ðAbN
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//`æ
//==================================================================================================
void DrawEffect2D(void)
{
	//foCX|C^Ýè
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//|C^é¾
	Effect2D *pEffect2D = GetEffect2D();

	//At@eXgÌLø»
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	for (int nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++, pEffect2D++)
	{
		if (pEffect2D->bUse)
		{
			//¸_obt@ðf[^Xg[ÉÝè
			pDevice->SetStreamSource(0, g_pVtxBuffEffect2D, 0, sizeof(VERTEX_2D));

			//¸_tH[}bgÌÝè
			pDevice->SetFVF(FVF_VERTEX_2D);

			//eNX`Ýè
			pDevice->SetTexture(0, g_pTextureEffect2D);

			if (pEffect2D->bAdd == true)
			{
				//¿ufBOðÁZ¬ÉÝè
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//|S`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect2D, 2);

			if (pEffect2D->bAdd == true)
			{
				//¿ufBOð³Éß·
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
		}
	}

	//At@eXgÌ³ø»
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

#ifdef _DEBUG
	DebugEffect2D();
#endif
}

//==================================================================================================
//Ýè
//==================================================================================================
void SetEffect2D(D3DXVECTOR3 pos, D3DXVECTOR3 *pPosOwner, D3DXVECTOR3 move, D3DXCOLOR col,float fRadius, int nLife,bool bAdd, float fGrav)
{
	//Ïé¾
	int nCntEffect2D;

	//¸_îñÌ|C^
	VERTEX_2D *pVtx;

	//eÖÌ|C^
	Effect2D *pEffect2D = GetEffect2D();

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffEffect2D->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2D = 0; nCntEffect2D < MAX_EFFECT2D; nCntEffect2D++,pEffect2D++)
	{//SÄÌeð`FbN·é
		if (pEffect2D->bUse == false)
		{//gÁÄ¢È¢óÔÈç

			//ÊuÌÝè
			pEffect2D->pos = pos;

			if (pPosOwner != NULL)
			{
				//¿åÊuÌÝè
				pEffect2D->pPosOwner = pPosOwner;

				//ÎÊuÝè
				pEffect2D->relPos = pos - *pPosOwner;
			}

			//ÊuÌÝè
			pEffect2D->move = move;

			//FÌÝè
			pEffect2D->col = col;

			//¼aÌÝè
			pEffect2D->fRadius = fRadius;

			//õ½ÌÝè
			pEffect2D->nLife = nLife;
			pEffect2D->nMaxLife = nLife;

			//¸_ÀWÌÝè
			pVtx[0].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[1].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[2].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };
			pVtx[3].col = D3DXCOLOR{ pEffect2D->col.r,pEffect2D->col.g,pEffect2D->col.b,pEffect2D->col.a };

			//gpµÄ¢éÝè
			pEffect2D->bUse = true;

			//ÁZ¬·é©Ç¤©
			pEffect2D->bAdd = bAdd;

			//dÍªe¿·é©Ç¤©
			pEffect2D->fGrav = fGrav;

			//for¶ð²¯é
			break;
		}

		//¸_f[^Ì|C^ð4Âªißé
		pVtx += 4;
	}

	//¸_obt@ðAbN
	g_pVtxBuffEffect2D->Unlock();
}

//==================================================================================================
//|C^æ¾
//==================================================================================================
Effect2D *GetEffect2D(void)
{
	return &g_aEffect2D[0];
}

//==================================================
//fobO\¦
//==================================================
void DebugEffect2D(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ê
	char aStr[1][256];					//
	int nNum = 0;
	LPD3DXFONT Font = GetFont();		//CÌâÂ

	for (int nCnt = 0; nCnt < MAX_EFFECT2D; nCnt++)
	{
		if (g_aEffect2D[nCnt].bUse == true)
		{
			nNum++;
		}
	}

	//¶ñÉãü
	sprintf(&aStr[0][0], "2DGtFNgÌF%d", nNum);

	rect.top += (int)(SCREEN_HEIGHT * 0.4f) + 40;
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
