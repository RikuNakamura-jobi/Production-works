//==========================================
//
//  r{[h(billboard.cpp)
//  Author : ²vÔD / ûücÀË / àèüí
//
//==========================================
#include "main.h"
#include "billboard.h"

//==========================================
//  }Nè`
//==========================================
#define MAX_BILLBOARD				(32)		//r{[hÌÅå
#define BILLBOARD_ANIM				(2)			//r{[hAj[VÌXVt[
#define BILLBOARD_ANIM_CNT			(25)		//r{[hAj[VÌÌ
#define BILLBOARD_SIZE				(50.0f)		//r{\hÌTCY
#define BILLBOARD_SIZE_GUIDE		(35.0f)		//r{\hÌTCY(KCh)
#define BILLBOARD_SPEED_GUIDE		(0.5f)		//r{\hÌXs[h(KCh)
#define BILLBOARD_DIST_GUIDE		(30.0f)		//r{\hÌÚ®£(KCh)
#define BILLBOARD_SIZE_ALERT		(5.0f)		//r{\hÌTCY(A[g)
#define BILLBOARD_SPEED_ALERT		(0.1f)		//r{\hÌXs[h
#define BILLBOARD_LIMIT_ALERT		(0.01f)		//r{\hÌXs[h

//==========================================
//  eNX`t@C¼
//==========================================
const char *c_apFilenameBillboard[BILLBOARDTYPE_MAX] =
{
	"data\\TEXTURE\\bullet000.png",
	"data\\TEXTURE\\fireAnim_000.png",
	"data\\TEXTURE\\alert.png",
	"data\\TEXTURE\\guide.png",
};

//==========================================
//  O[oÏé¾
//==========================================
LPDIRECT3DTEXTURE9 g_apTextureBillboard[BILLBOARDTYPE_MAX] = {}; //eNX`ÖÌ|C^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL; //¸_obt@ÖÌ|C^
Billboard g_Billboard[MAX_BILLBOARD];

//==========================================
//r{[hÌú»
//==========================================
void InitBillboard(void)
{
	//ÏÌú»
	ZeroMemory(&g_Billboard[0], sizeof(Billboard) * MAX_BILLBOARD);

	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//¸_obt@Ì¶¬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL
	);

	//¸_f[^ÌÄÑoµ
	VERTEX_3D *pVtx;

	//eNX`ÌÇÝÝ
	for (int nCntTex = 0; nCntTex < BILLBOARDTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameBillboard[nCntTex],
			&g_apTextureBillboard[nCntTex]
		);
	}

	//¸_obt@ðbNµ¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//¸_ÀWÌÝè
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, BILLBOARD_SIZE * 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE, 0.0f, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//@üxNgÌÝè
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//¸_J[ÌÝè
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//eNX`ÌÀWÌÝè
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//¸_f[^Ì|C^ð4Âªißé
		pVtx += 4;
	}

	//¸_obt@ðAbN·é
	g_pVtxBuffBillboard->Unlock();
}

//==========================================
//r{[hÌI¹
//==========================================
void UninitBillboard(void)
{
	//eNX`Ìjü
	for (int nCntTex = 0; nCntTex < BILLBOARDTYPE_MAX; nCntTex++)
	{
		if (g_apTextureBillboard[nCntTex] != NULL)
		{
			g_apTextureBillboard[nCntTex]->Release();
			g_apTextureBillboard[nCntTex] = NULL;
		}
	}

	//¸_obt@Ìjü
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==========================================
//r{[hÌXV
//==========================================
void UpdateBillboard(void)
{
	//Ïé¾
	float fLengthDiff;	

	//¸_f[^ÌÄÑoµ
	VERTEX_3D *pVtx;

	//¸_obt@ðbNµA¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0;nCnt < MAX_BILLBOARD;nCnt++)
	{
		if (g_Billboard[nCnt].bUse == true)
		{
			switch (g_Billboard[nCnt].nType)
			{
			case BILLBOARDTYPE_FIRE:
				//JE^[ðÁZ
				g_Billboard[nCnt].nCnterAnim++;

				if ((g_Billboard[nCnt].nCnterAnim % BILLBOARD_ANIM) == 0)
				{
					//JE^[ðúlÉß·
					g_Billboard[nCnt].nCnterAnim = 0;

					//p^[No.ÌXV·é
					g_Billboard[nCnt].nPatternAnim = (g_Billboard[nCnt].nPatternAnim + 1) % BILLBOARD_ANIM_CNT;

					//eNX`ÀWÌÝè
					pVtx[0].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f);
					pVtx[1].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f);
					pVtx[2].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f + 0.2f);
					pVtx[3].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f + 0.2f);
				}
				break;

			case BILLBOARDTYPE_ALERT:
				fLengthDiff = BILLBOARD_SIZE_ALERT * 7 - pVtx[0].pos.y;

				//¸_ÀWÌÝè
				pVtx[0].pos.y += fLengthDiff * BILLBOARD_SPEED_ALERT;
				pVtx[1].pos.y += fLengthDiff * BILLBOARD_SPEED_ALERT;

				if (fLengthDiff * fLengthDiff <= BILLBOARD_LIMIT_ALERT * BILLBOARD_LIMIT_ALERT)
				{
					g_Billboard[nCnt].bUse = false;
				}
				break;

			case BILLBOARDTYPE_GUIDE:

				if (g_Billboard[nCnt].fMoveDist > BILLBOARD_DIST_GUIDE)
				{//ãªèØÁ½ç
					g_Billboard[nCnt].bMoveState = false;
				}
				else if (g_Billboard[nCnt].fMoveDist < -BILLBOARD_DIST_GUIDE)
				{//ºªèØÁ½ç
					g_Billboard[nCnt].bMoveState = true;
				}

				switch (g_Billboard[nCnt].bMoveState)
				{
				case true:
					g_Billboard[nCnt].pos.y += BILLBOARD_SPEED_GUIDE;
					g_Billboard[nCnt].fMoveDist += BILLBOARD_SPEED_GUIDE;
					break;

				case false:
					g_Billboard[nCnt].pos.y -= BILLBOARD_SPEED_GUIDE;
					g_Billboard[nCnt].fMoveDist -= BILLBOARD_SPEED_GUIDE;
					break;
				}

				break;
			}
		}

		//¸_f[^ð4Âißé
		pVtx += 4;
	}

	//¸_obt@ðAbN·é
	g_pVtxBuffBillboard->Unlock();
}

//==========================================
//r{[hÌ`æ
//==========================================
void DrawBillboard(void)
{
	//[JÏé¾
	D3DXMATRIX mtxTrans; //vZp}gbNX
	D3DXMATRIX mtxView; //r[}gbNXÌæ¾p

	//foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//CeBOÌ³ø»
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_Billboard[nCntBillboard].bUse == true)
		{
			//[h}gbNXÌú»
			D3DXMatrixIdentity(&g_Billboard[nCntBillboard].mtxWorld);

			//r[}gbNXðæ¾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//|SðJÉÎµÄ³ÊÉü¯é
			D3DXMatrixInverse(&g_Billboard[nCntBillboard].mtxWorld, NULL, &mtxView); //tsñðßé
			g_Billboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_Billboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_Billboard[nCntBillboard].mtxWorld._43 = 0.0f;

			if (g_Billboard[nCntBillboard].nType == BILLBOARDTYPE_FIRE)
			{
				g_Billboard[nCntBillboard].mtxWorld._21 = 0.0f;
				g_Billboard[nCntBillboard].mtxWorld._23 = 0.0f;
				g_Billboard[nCntBillboard].mtxWorld._24 = 0.0f;
			}

			//Êuð½f
			D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCntBillboard].pos.x, g_Billboard[nCntBillboard].pos.y, g_Billboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_Billboard[nCntBillboard].mtxWorld, &g_Billboard[nCntBillboard].mtxWorld, &mtxTrans);

			//ZeXgð³øÉ·é
			if (g_Billboard[nCntBillboard].nType == BILLBOARDTYPE_GUIDE)
			{
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			//At@eXgðLøÉ·é
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//[h}gbNXÌÝè
			pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCntBillboard].mtxWorld);

			//¸_obt@ðf[^Xg[ÉÝè
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//¸_tH[}bgÌÝè
			pDevice->SetFVF(FVF_VERTEX_3D);

			//eNX`ÌÝè
			pDevice->SetTexture(0, g_apTextureBillboard[g_Billboard[nCntBillboard].nType]);

			//|SÌ`æ
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//ZeXgðLøÉ·é
			if (g_Billboard[nCntBillboard].nType == BILLBOARDTYPE_GUIDE)
			{
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}

			//At@eXgð³øÉ·é
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		}
	}

	//CeBOÌLø»
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================
//r{[hÌÝè
//==========================================
int SetBillboard(int nType, D3DXVECTOR3 pos)
{
	//[JÏé¾
	int nCount = -1;

	//¸_îñÖÌ|C^
	VERTEX_3D *pVtx;

	//¸_obt@ðbNµ¸_îñÖÌ|C^ðæ¾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_Billboard[nCntBillboard].bUse == false)
		{
			//r{[hªgp³êÄÈ¢
			nCount = nCntBillboard;
			g_Billboard[nCount].bUse = true;
			g_Billboard[nCount].nType = nType;
			g_Billboard[nCount].pos = pos;

			switch (nType)
			{
			case BILLBOARDTYPE_FIRE:
				g_Billboard[nCount].nPatternAnim = rand() % BILLBOARD_ANIM_CNT;

				if (g_Billboard[nCount].nPatternAnim < 0)
				{
					g_Billboard[nCount].nPatternAnim *= -1;
				}

				//eNX`ÀWÌÝè
				pVtx[0].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f);
				pVtx[1].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f);
				pVtx[2].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f + 0.2f);
				pVtx[3].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f + 0.2f);
				break;

			case BILLBOARDTYPE_ALERT:
				//¸_ÀWÌÝè
				pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				break;

			case BILLBOARDTYPE_GUIDE:
				//¸_ÀWÌÝè
				pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_GUIDE, BILLBOARD_SIZE_GUIDE * 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_GUIDE, BILLBOARD_SIZE_GUIDE * 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_GUIDE, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_GUIDE, 0.0f, 0.0f);
				break;
			}
			break;
		}

		//¸_f[^Ì|C^ð4Âªißé
		pVtx += 4;
	}
	//¸_obt@ðAbN·é
	g_pVtxBuffBillboard->Unlock();

	//ÏðÔ·
	return nCount;
}

//==========================================
//r{[hÌÊuÝè
//==========================================
void SetPositionBillboard(int nIdxBillboard, D3DXVECTOR3 pos)
{
	if (nIdxBillboard >= -1)
	{
		g_Billboard[nIdxBillboard].pos.x = pos.x;
		g_Billboard[nIdxBillboard].pos.y = pos.y;
		g_Billboard[nIdxBillboard].pos.z = pos.z;
	}
}

//==========================================
//r{[hÌæ¾
//==========================================
Billboard *GetBillboard(void)
{
	return &g_Billboard[0];
}
