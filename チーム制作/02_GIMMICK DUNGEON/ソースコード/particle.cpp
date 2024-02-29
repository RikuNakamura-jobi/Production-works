//========================================
//
// 3Dパーティクルの処理[particle.cpp]
// Author:Kai Takada
//
//========================================
#include "main.h"
#include "particle.h"
#include "effect.h"

//================================
//マクロ定義
//================================
#define MAX_PARTICLE (256)		//パーティクルの最大数

//================================
//グローバル変数宣言
//================================
Particle g_aParticle[MAX_PARTICLE];		//パーティクル情報

//================================
//パーティクルの初期化処理
//================================
void InitParticle(void)
{
	for (int nCntInit = 0;nCntInit < MAX_PARTICLE;nCntInit++)
	{
		ZeroMemory(&g_aParticle[nCntInit],sizeof(Particle));
	}
}

//================================
//パーティクルの終了処理
//================================
void UninitParticle(void)
{

}

//================================
//パーティクルの更新処理
//================================
void UpdateParticle(void)
{
	Particle *pParticle = GetParticle();

	for (int nCntUpdate = 0; nCntUpdate < MAX_PARTICLE; nCntUpdate++, pParticle++)
	{
		if (pParticle->bUse == true)
		{
			for (int nCntEf = 0; nCntEf < pParticle->nNumEf; nCntEf++)
			{
				SetEffect(D3DXVECTOR3((float)(rand() % 21) + pParticle->effect.pos.x,
									  (float)(rand() % 21) + pParticle->effect.pos.y,
									  (float)(rand() % 21) + pParticle->effect.pos.z),
						  pParticle->effect.pPosOwner,
						  D3DXVECTOR3((float)(rand() % (pParticle->nSpeed * 2) - pParticle->nSpeed) * 0.1f + pParticle->effect.move.x,
									  (float)(rand() % (pParticle->nSpeed * 2) - pParticle->nSpeed) * 0.1f + pParticle->effect.move.y,
									  (float)(rand() % (pParticle->nSpeed * 2) - pParticle->nSpeed) * 0.1f + pParticle->effect.move.z),
						  pParticle->effect.col,
						  (rand() % 20) * 0.1f + pParticle->effect.fRadius,
						  rand() % 60 + pParticle->effect.nLife,
						  pParticle->effect.bAdd,
						  pParticle->effect.fGrav,
						  pParticle->effect.bZtest);
			}
		
			pParticle->nLifePtcl--;

			if (pParticle->nLifePtcl <= 0)
			{
				pParticle->bUse = false;
			}
		}
	}
}

//================================
//パーティクルの描画処理
//================================
void DrawParticle(void)
{

}

//================================
//パーティクルの設定処理
//================================
void SetParticle(D3DXVECTOR3 pos,PARTICLETYPE type)
{
	Particle *pParticle = GetParticle();

	for (int nCntSet = 0;nCntSet < MAX_PARTICLE;nCntSet++,pParticle++)
	{
		if (pParticle->bUse == false)
		{
			pParticle->Randpos = pos;
			pParticle->bUse = true;

			switch (type)
			{
				case PARTICLETYPE_SMOG:
					pParticle->nNumEf = 17;
					pParticle->nLifePtcl = 1;
					pParticle->nSpeed = 60;
					pParticle->effect.pos = pos;
					pParticle->effect.pPosOwner = NULL;
					pParticle->effect.col = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.3f);
					pParticle->effect.fRadius = 80.0f;
					pParticle->effect.nLife = 2000;
					pParticle->effect.bZtest = true;
					pParticle->effect.fGrav = 0.0f;
					pParticle->effect.bAdd = false;
					break;

				case PARTICLETYPE_SPARK:
					pParticle->nNumEf = 20;
					pParticle->nLifePtcl = 1;
					pParticle->nSpeed = 40;
					pParticle->effect.pos = pos;
					pParticle->effect.pPosOwner = NULL;
					pParticle->effect.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					pParticle->effect.col = D3DXCOLOR(0.9f, 0.5f, 0.2f, 0.6f);
					pParticle->effect.fRadius = 10.0f;
					pParticle->effect.nLife = 50;
					pParticle->effect.bZtest = false;
					pParticle->effect.fGrav = 0.0f;
					pParticle->effect.bAdd = true;
					break;

				case PARTICLETYPE_SANDPOLE:
					pParticle->nNumEf = 2;
					pParticle->nLifePtcl = 1;
					pParticle->nSpeed = 1;
					pParticle->effect.pos = pos;
					pParticle->effect.pPosOwner = NULL;
					pParticle->effect.col = D3DXCOLOR(0.55f, 0.47f, 0.3f, 0.2f);
					pParticle->effect.fRadius = 4.0f;
					pParticle->effect.nLife = 13000;
					pParticle->effect.bZtest = false;
					pParticle->effect.fGrav = -0.1f;
					pParticle->effect.bAdd = true;
					break;

				case PARTICLETYPE_SANDDUST:
					pParticle->nNumEf = 2;
					pParticle->nLifePtcl = 1;
					pParticle->nSpeed = 10;
					pParticle->effect.pos = pos;
					pParticle->effect.pPosOwner = NULL;
					pParticle->effect.col = D3DXCOLOR(0.9f, 0.6f, 0.0f, 0.2f);
					pParticle->effect.fRadius = 5.0f;
					pParticle->effect.nLife = 10000;
					pParticle->effect.bZtest = false;
					pParticle->effect.fGrav = -0.1f;
					pParticle->effect.bAdd = true;
					break;
			
				case PARTICLETYPE_LANDED:
					pParticle->nNumEf = 20;
					pParticle->nLifePtcl = 1;
					pParticle->nSpeed = 15;
					pParticle->effect.pos = pos;
					pParticle->effect.pPosOwner = NULL;
					pParticle->effect.col = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.7f);
					pParticle->effect.fRadius = 20.0f;
					pParticle->effect.nLife = 1000;
					pParticle->effect.bZtest = true;
					pParticle->effect.fGrav = 0.005f;
					pParticle->effect.bAdd = false;
					break;
			}
			break;
		}
	}
}

//================================
//パーティクルの取得処理
//================================
Particle *GetParticle()
{
	return &g_aParticle[0];
}
