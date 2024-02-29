//==========================================
//
//  ビルボード処理(billboard.cpp)
//  Author : 佐久間優香 / 髙田佳依 / 金崎朋弥
//
//==========================================
#include "main.h"
#include "billboard.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_BILLBOARD				(32)		//ビルボードの最大数
#define BILLBOARD_ANIM				(2)			//ビルボードアニメーションの更新フレーム数
#define BILLBOARD_ANIM_CNT			(25)		//ビルボードアニメーションの炎の枚数
#define BILLBOARD_SIZE				(50.0f)		//ビルボ―ドのサイズ
#define BILLBOARD_SIZE_GUIDE		(35.0f)		//ビルボ―ドのサイズ(ガイド)
#define BILLBOARD_SPEED_GUIDE		(0.5f)		//ビルボ―ドのスピード(ガイド)
#define BILLBOARD_DIST_GUIDE		(30.0f)		//ビルボ―ドの移動距離(ガイド)
#define BILLBOARD_SIZE_ALERT		(5.0f)		//ビルボ―ドのサイズ(アラート)
#define BILLBOARD_SPEED_ALERT		(0.1f)		//ビルボ―ドのスピード
#define BILLBOARD_LIMIT_ALERT		(0.01f)		//ビルボ―ドのスピード

//==========================================
//  テクスチャファイル名
//==========================================
const char *c_apFilenameBillboard[BILLBOARDTYPE_MAX] =
{
	"data\\TEXTURE\\bullet000.png",
	"data\\TEXTURE\\fireAnim_000.png",
	"data\\TEXTURE\\alert.png",
	"data\\TEXTURE\\guide.png",
};

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_apTextureBillboard[BILLBOARDTYPE_MAX] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL; //頂点バッファへのポインタ
Billboard g_Billboard[MAX_BILLBOARD];

//==========================================
//ビルボードの初期化処理
//==========================================
void InitBillboard(void)
{
	//変数の初期化
	ZeroMemory(&g_Billboard[0], sizeof(Billboard) * MAX_BILLBOARD);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL
	);

	//頂点データの呼び出し
	VERTEX_3D *pVtx;

	//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < BILLBOARDTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameBillboard[nCntTex],
			&g_apTextureBillboard[nCntTex]
		);
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE, BILLBOARD_SIZE * 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE, BILLBOARD_SIZE * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE, 0.0f, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャの座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//==========================================
//ビルボードの終了処理
//==========================================
void UninitBillboard(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < BILLBOARDTYPE_MAX; nCntTex++)
	{
		if (g_apTextureBillboard[nCntTex] != NULL)
		{
			g_apTextureBillboard[nCntTex]->Release();
			g_apTextureBillboard[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//==========================================
//ビルボードの更新処理
//==========================================
void UpdateBillboard(void)
{
	//変数宣言
	float fLengthDiff;	

	//頂点データの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0;nCnt < MAX_BILLBOARD;nCnt++)
	{
		if (g_Billboard[nCnt].bUse == true)
		{
			switch (g_Billboard[nCnt].nType)
			{
			case BILLBOARDTYPE_FIRE:
				//カウンターを加算
				g_Billboard[nCnt].nCnterAnim++;

				if ((g_Billboard[nCnt].nCnterAnim % BILLBOARD_ANIM) == 0)
				{
					//カウンターを初期値に戻す
					g_Billboard[nCnt].nCnterAnim = 0;

					//パターンNo.の更新する
					g_Billboard[nCnt].nPatternAnim = (g_Billboard[nCnt].nPatternAnim + 1) % BILLBOARD_ANIM_CNT;

					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f);
					pVtx[1].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f);
					pVtx[2].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f + 0.2f);
					pVtx[3].tex = D3DXVECTOR2((g_Billboard[nCnt].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCnt].nPatternAnim / 5) * 0.2f + 0.2f);
				}
				break;

			case BILLBOARDTYPE_ALERT:
				fLengthDiff = BILLBOARD_SIZE_ALERT * 7 - pVtx[0].pos.y;

				//頂点座標の設定
				pVtx[0].pos.y += fLengthDiff * BILLBOARD_SPEED_ALERT;
				pVtx[1].pos.y += fLengthDiff * BILLBOARD_SPEED_ALERT;

				if (fLengthDiff * fLengthDiff <= BILLBOARD_LIMIT_ALERT * BILLBOARD_LIMIT_ALERT)
				{
					g_Billboard[nCnt].bUse = false;
				}
				break;

			case BILLBOARDTYPE_GUIDE:

				if (g_Billboard[nCnt].fMoveDist > BILLBOARD_DIST_GUIDE)
				{//上がり切ったら
					g_Billboard[nCnt].bMoveState = false;
				}
				else if (g_Billboard[nCnt].fMoveDist < -BILLBOARD_DIST_GUIDE)
				{//下がり切ったら
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

		//頂点データを4つ進める
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//==========================================
//ビルボードの描画処理
//==========================================
void DrawBillboard(void)
{
	//ローカル変数宣言
	D3DXMATRIX mtxTrans; //計算用マトリックス
	D3DXMATRIX mtxView; //ビューマトリックスの取得用

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ライティングの無効化
	pDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_Billboard[nCntBillboard].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Billboard[nCntBillboard].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_Billboard[nCntBillboard].mtxWorld, NULL, &mtxView); //逆行列を求める
			g_Billboard[nCntBillboard].mtxWorld._41 = 0.0f;
			g_Billboard[nCntBillboard].mtxWorld._42 = 0.0f;
			g_Billboard[nCntBillboard].mtxWorld._43 = 0.0f;

			if (g_Billboard[nCntBillboard].nType == BILLBOARDTYPE_FIRE)
			{
				g_Billboard[nCntBillboard].mtxWorld._21 = 0.0f;
				g_Billboard[nCntBillboard].mtxWorld._23 = 0.0f;
				g_Billboard[nCntBillboard].mtxWorld._24 = 0.0f;
			}

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Billboard[nCntBillboard].pos.x, g_Billboard[nCntBillboard].pos.y, g_Billboard[nCntBillboard].pos.z);
			D3DXMatrixMultiply(&g_Billboard[nCntBillboard].mtxWorld, &g_Billboard[nCntBillboard].mtxWorld, &mtxTrans);

			//Zテストを無効にする
			if (g_Billboard[nCntBillboard].nType == BILLBOARDTYPE_GUIDE)
			{
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}

			//アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Billboard[nCntBillboard].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBillboard[g_Billboard[nCntBillboard].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);

			//Zテストを有効にする
			if (g_Billboard[nCntBillboard].nType == BILLBOARDTYPE_GUIDE)
			{
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			}

			//アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		}
	}

	//ライティングの有効化
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================
//ビルボードの設定処理
//==========================================
int SetBillboard(int nType, D3DXVECTOR3 pos)
{
	//ローカル変数宣言
	int nCount = -1;

	//頂点情報へのポインタ
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_Billboard[nCntBillboard].bUse == false)
		{
			//ビルボードが使用されてない
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

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f);
				pVtx[1].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f);
				pVtx[2].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f + 0.2f);
				pVtx[3].tex = D3DXVECTOR2((g_Billboard[nCount].nPatternAnim % 5) * 0.2f + 0.2f, (g_Billboard[nCount].nPatternAnim / 5) * 0.2f + 0.2f);
				break;

			case BILLBOARDTYPE_ALERT:
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_ALERT, 0.0f, 0.0f);
				break;

			case BILLBOARDTYPE_GUIDE:
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-BILLBOARD_SIZE_GUIDE, BILLBOARD_SIZE_GUIDE * 2, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(BILLBOARD_SIZE_GUIDE, BILLBOARD_SIZE_GUIDE * 2, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-BILLBOARD_SIZE_GUIDE, 0.0f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(BILLBOARD_SIZE_GUIDE, 0.0f, 0.0f);
				break;
			}
			break;
		}

		//頂点データのポインタを4つ分進める
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();

	//変数を返す
	return nCount;
}

//==========================================
//ビルボードの位置設定処理
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
//ビルボードの取得
//==========================================
Billboard *GetBillboard(void)
{
	return &g_Billboard[0];
}
