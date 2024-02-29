//==============================================================
//
//パーティクル処理[particle.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include"particle.h"
#include"effect.h"

//マクロ定義
#define MAX_PARTICLE		(1280)		//パーティクルの最大数

//パーティクル構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置（発生位置）
	int nLife;				//寿命（発生時間）
	D3DXCOLOR col;			//色
	float fRadius;			//半径（大きさ）
	int nType;				//種類
	bool bUse;				//使用しているかどうか
} Particle;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR col;			//色
	int nLife;				//寿命
	int nType;				//種類
	float fAngle;			//角度
	float fMove;			//移動量
	float fRadius;			//半径
} ParticleInfo;

//プロトタイプ宣言
void ParticleGeneration(int nCounter);
void DamageParticle(int nCounter);

//グローバル変数
Particle g_aParticle[MAX_PARTICLE];							//パーティクルの情報
ParticleInfo g_aParticleInfo;
int g_nEffectType = 0;

//==============================================================
//パーティクルの初期化処理
//==============================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntParticle;

	//弾の情報の初期化
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置（発生位置）の初期化
		g_aParticle[nCntParticle].nLife = 0;								//寿命（発生時間）の初期化
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色の初期化
		g_aParticle[nCntParticle].fRadius = 0.0f;							//半径の初期化
		g_aParticle[nCntParticle].nType = 0;								//種類の初期化
		g_aParticle[nCntParticle].bUse = false;								//使用していない状態にする
	}

	g_aParticleInfo.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
	g_aParticleInfo.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量の初期化
	g_aParticleInfo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色の初期化
	g_aParticleInfo.nLife = 0;									//寿命の初期化
	g_aParticleInfo.nType = 0;									//種類の初期化
	g_aParticleInfo.fAngle = 0.0f;								//角度の初期化
	g_aParticleInfo.fMove = 0.0f;								//計算用移動量を初期化
	g_aParticleInfo.fRadius = 0.0f;								//半径を初期化
	g_nEffectType = 0;
}

//==============================================================
//パーティクルの終了処理
//==============================================================
void UninitParticle(void)
{
	
}

//==============================================================
//パーティクルの更新処理
//==============================================================
void UpdateParticle(void)
{

}

//==============================================================
//パーティクルの描画処理
//==============================================================
void DrawParticle(void)
{
	
}

//==============================================================
//パーティクルの設定処理
//==============================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nType, int nLife, float fRadius)
{
	int nCntParticle;

	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//パーティクルが使用されていない
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].col = col;
			g_aParticle[nCntParticle].nType = nType;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_nEffectType = nType;

			g_aParticle[nCntParticle].bUse = true;		//使用している状態にする

			ParticleGeneration(nCntParticle);

			break;
		}
	}
}

//==============================================================
//パーティクル生成
//==============================================================
void ParticleGeneration(int nCounter)
{
	int nCntParticle = nCounter, nCntAppear, nCntLife;

	for (nCntLife = 0; nCntLife < MAX_PARTICLE; nCntLife++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{//パーティクルが使用されている

			//パーティクルの生成
			for (nCntAppear = 0; nCntAppear < 5; nCntAppear++)
			{
				//ダメージパーティクル
				DamageParticle(nCntParticle);

				//エフェクトの設定
				SetEffect(g_aParticleInfo.pos,
					g_aParticleInfo.move,
					g_aParticleInfo.col,
					g_aParticleInfo.fRadius,
					g_aParticleInfo.nLife,
					g_aParticleInfo.nType);
			}

			//寿命のカウントダウン
			g_aParticle[nCntParticle].nLife--;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{//寿命が尽きた場合
				g_aParticle[nCntParticle].bUse = false;
			}
		}
	}
}

//==============================================================
//ダメージパーティクル
//==============================================================
void DamageParticle(int nCounter)
{
	int nCntParticle = nCounter;

	//位置の設定
	g_aParticleInfo.pos = g_aParticle[nCntParticle].pos;

	if (g_nEffectType == 0)
	{
		//角度を求める
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//移動量を求める
		g_aParticleInfo.fMove = (float)(rand() % 50) / 10.0f + 1.0f;

		//移動量の設定
		g_aParticleInfo.move.x = sinf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;
		g_aParticleInfo.move.y = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;
		g_aParticleInfo.move.z = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//色の設定
		g_aParticleInfo.col = g_aParticle[nCntParticle].col;

		//半径の設定
		g_aParticleInfo.fRadius = g_aParticle[nCntParticle].fRadius;

		//寿命の設定
		g_aParticleInfo.nLife = g_aParticle[nCntParticle].nLife;

		//パーティクルの種類
		g_aParticleInfo.nType = 0;
	}
	else if (g_nEffectType == 1)
	{
		//角度を求める
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//移動量を求める
		g_aParticleInfo.fMove = (float)(rand() % 50) / 10.0f + 1.0f;

		//移動量の設定
		g_aParticleInfo.move.x = sinf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//角度を求める
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//移動量を求める
		g_aParticleInfo.fMove = (float)(rand() % 50) / 100.0f + 1.0f;

		g_aParticleInfo.move.y = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//色の設定
		g_aParticleInfo.col = g_aParticle[nCntParticle].col;

		//半径の設定
		g_aParticleInfo.fRadius = g_aParticle[nCntParticle].fRadius;

		//寿命の設定
		g_aParticleInfo.nLife = g_aParticle[nCntParticle].nLife;

		//パーティクルの種類
		g_aParticleInfo.nType = 0;
	}

	else if (g_nEffectType == 2)
	{
		//角度を求める
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//移動量を求める
		g_aParticleInfo.fMove = (float)(rand() % 50) / 10.0f + 1.0f;

		//移動量の設定
		g_aParticleInfo.move.y = cosf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		//角度を求める
		g_aParticleInfo.fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//移動量を求める
		g_aParticleInfo.fMove = (float)(rand() % 50) / 100.0f + 1.0f;

		g_aParticleInfo.move.x = sinf(g_aParticleInfo.fAngle) * g_aParticleInfo.fMove;

		if (g_aParticleInfo.move.y > 0.0f)
		{
			g_aParticleInfo.move.y *= -1.0f;
		}

		//色の設定
		g_aParticleInfo.col = g_aParticle[nCntParticle].col;

		//半径の設定
		g_aParticleInfo.fRadius = g_aParticle[nCntParticle].fRadius;

		//寿命の設定
		g_aParticleInfo.nLife = g_aParticle[nCntParticle].nLife;

		//パーティクルの種類
		g_aParticleInfo.nType = 0;
	}
}