//==========================================
//
//  取得スコア表示処理
//  Author : 髙山桃也
//
//==========================================
#include "point.h"
#include "input.h"
#include "point_log.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_NUM_PLACE (6) //スコアの桁数
#define POINT_WIDTH (40.0f) //スコアの横幅
#define POINT_HEIGHT (60.0f) //スコアの高さ
#define WAIT_RANGE	(0.1f)	//移動完了とみなす差分との直線距離
#define OUT_TIME	(90)	//待機状態から遷移するまでの時間
#define ALPHA_DEC_TIME	(0.04f)	//透明になる速度
#define MOVE_SPEED	(5.0f)	//移動速度

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTexturePoint = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPoint = NULL;
Point g_aPoint[MAX_POINT_NUM];

//==========================================
//  初期化処理
//==========================================
void InitPoint()
{
	//変数の初期化
	ZeroMemory(&g_aPoint[0],sizeof(Point) * MAX_POINT_NUM);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * MAX_NUM_PLACE * MAX_POINT_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPoint,
		NULL
	);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/number001.png",
		&g_pTexturePoint
	);

	//頂点バッファの呼び出し
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_NUM_PLACE * MAX_POINT_NUM; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhwの設定
			pVtx[nCnt].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffPoint->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitPoint()
{
	//頂点バッファの破棄
	if (g_pVtxBuffPoint != NULL)
	{
		g_pVtxBuffPoint->Release();
		g_pVtxBuffPoint = NULL;
	}

	//テクスチャの破棄
	if (g_pTexturePoint != NULL)
	{
		g_pTexturePoint->Release();
		g_pTexturePoint = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdatePoint()
{
	//情報取得
	Point *pPoint = GetPoint();

	for (int nCntPoint = 0; nCntPoint < MAX_POINT_NUM; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse)
		{//使用している状態なら

			switch (pPoint->type)
			{//種類ごとの更新分け
			case POINTTYPE_SLIDE:
				UpdatePointSlide(pPoint);
				break;
			case POINTTYPE_STAY:
				UpdatePointStay(pPoint);
				break;
			default:
				break;
			}

			//頂点情報更新=========================================================
			//頂点バッファの呼び出し
			VERTEX_2D *pVtx;

			//頂点バッファをロック
			g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

			//目標位置までポインタを進める
			pVtx += 4 * nCntPoint * MAX_NUM_PLACE;

			//頂点情報設定
			for (int nCntNum = 0; nCntNum < MAX_NUM_PLACE; nCntNum++)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);

				pVtx[0].col = pPoint->col;
				pVtx[1].col = pPoint->col;
				pVtx[2].col = pPoint->col;
				pVtx[3].col = pPoint->col;

				pVtx += 4; //頂点データのポインタを4つ進める
			}

			//頂点バッファをアンロック
			g_pVtxBuffPoint->Unlock();
			//頂点情報更新=========================================================
		}
	}
}

//==========================================
//  スライドタイプの更新
//==========================================
void UpdatePointSlide(Point *pPoint)
{
	//変数宣言
	D3DXVECTOR3 posDiff;
	float fLengthDiff;

	//目標との差分算出
	posDiff = pPoint->posDest - pPoint->pos;

	//差分の長さ算出
	fLengthDiff = D3DXVec3Length(&posDiff);

	//正規化
	D3DXVec3Normalize(&posDiff, &posDiff);

	//移動量加算
	for (int nCntPos = 0; nCntPos < 3; nCntPos++)
	{//XYZの三つ分のfor文
		if (posDiff[nCntPos] >= 0.01f || posDiff[nCntPos] <= -0.01f)
		{//差分を加算する
			pPoint->pos[nCntPos] += posDiff[nCntPos] * MOVE_SPEED;
		}
	}

	switch (pPoint->state)
	{//状態ごとの更新
	case POINTSTATE_IN:

		if (pPoint->col.a < 1.0f)
		{//不透明にする
			pPoint->col.a += ALPHA_DEC_TIME;
		}
		else
		{//1.0fに固定
			pPoint->col.a = 1.0f;
		}

		if (fLengthDiff <= WAIT_RANGE)
		{//待機状態へ移行
			pPoint->state = POINTSTATE_WAIT;
		}

		break;
	case POINTSTATE_WAIT:

		if (pPoint->nCntState <= OUT_TIME)
		{
			//遷移カウンター上昇
			pPoint->nCntState++;
		}
		else
		{//スライドアウト状態へ遷移
			pPoint->nCntState = 0;
			pPoint->state = POINTSTATE_OUT;
		}

		break;
	case POINTSTATE_OUT:

		if (pPoint->col.a > 0.0f)
		{//透明にする
			pPoint->col.a -= ALPHA_DEC_TIME;
		}
		else
		{//消去
			pPoint->col.a = 0.0f;
			pPoint->bUse = false;
		}

		break;
	default:
		break;
	}
}

//==========================================
//  ステイタイプの更新
//==========================================
void UpdatePointStay(Point *pPoint)
{

}

//==========================================
//  描画処理
//==========================================
void DrawPoint()
{
	//情報取得
	Point *pPoint = GetPoint();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPoint, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPoint = 0;nCntPoint < MAX_POINT_NUM;nCntPoint++, pPoint++)
	{
		if (pPoint->bUse)
		{//使用している状態なら
			for (int nCnt = MAX_NUM_PLACE - pPoint->nNumPlace; nCnt < MAX_NUM_PLACE; nCnt++)
			{//桁数分描画
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTexturePoint);

				//描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt + 4 * nCntPoint * MAX_NUM_PLACE, 2);
			}
		}
	}
}

//==========================================
//  設定処理
//==========================================
void SetPointScore(int nPoint, D3DXVECTOR3 pos, D3DXVECTOR3 posDest, POINTTYPE type, float fScale, LOGTYPE logType)
{
	//ローカル変数宣言
	int aTex[MAX_NUM_PLACE];
	int nCntNum;

	//計算用変数
	int nCalc = nPoint;

	//情報取得
	Point *pPoint = GetPoint();

	for (int nCntPoint = 0; nCntPoint < MAX_POINT_NUM; nCntPoint++, pPoint++)
	{
		if (pPoint->bUse == false)
		{//使用されていない状態なら
			//引数受け取り
			pPoint->pos = pos;
			pPoint->posDest = posDest;
			pPoint->type = type;
			pPoint->fScale = fScale;

			if (nPoint >= 0)
			{//加算なら
				pPoint->col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			}
			else
			{//減算なら
				pPoint->col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				//±補正
				nCalc *= -1;
			}

			if (pPoint->type == POINTTYPE_SLIDE)
			{//スライドインするなら
				//初期を透明にする
				pPoint->col.a = 0.0f;
			}
			
			//メッセージを表示
			SetPointLog(pPoint, logType,fScale);

			//テクスチャ座標の計測
			for (int nCntPoint = MAX_NUM_PLACE - 1; nCntPoint >= 0; nCntPoint--)
			{
				aTex[nCntPoint] = nCalc % 10;
				nCalc /= 10;

				if (aTex[nCntPoint] != 0)
				{//桁数保存
					pPoint->nNumPlace = MAX_NUM_PLACE - nCntPoint;
				}
			}

			//頂点バッファの呼び出し
			VERTEX_2D *pVtx;

			//頂点バッファをロック
			g_pVtxBuffPoint->Lock(0, 0, (void**)&pVtx, 0);

			//目標位置までポインタを進める
			pVtx += 4 * nCntPoint * MAX_NUM_PLACE;

			//設定処理
			for (nCntNum = 0; nCntNum < MAX_NUM_PLACE; nCntNum++)
			{
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * nCntNum, pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pPoint->pos.x + POINT_HEIGHT * pPoint->fScale * (nCntNum + 1), pPoint->pos.y + POINT_HEIGHT * pPoint->fScale * 2, 0.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(aTex[nCntNum] * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((aTex[nCntNum] * 0.1f) + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(aTex[nCntNum] * 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((aTex[nCntNum] * 0.1f) + 0.1f, 1.0f);

				pVtx += 4; //頂点データのポインタを4つ進める
			}

			//頂点バッファをアンロック
			g_pVtxBuffPoint->Unlock();

			//使用状態にする
			pPoint->bUse = true;

			//スライドイン状態にする
			pPoint->state = POINTSTATE_IN;

			//他のスコア押し上げ処理
			PushUpScore(nCntPoint);

			break;
		}
	}
}

//==========================================
//  既にあるスコアを押し上げる処理
//==========================================
void PushUpScore(int nCntpoint)
{
	//情報取得
	Point *pPoint = GetPoint();

	for (int nCntScore = 0; nCntScore < MAX_POINT_NUM; nCntScore++,pPoint++)
	{
		if (pPoint->bUse || nCntpoint != nCntScore)
		{//新しいスコア以外を押し上げる
			pPoint->posDest.y -= POINT_HEIGHT;
		}
	}
}

//==========================================
//  情報取得処理
//==========================================
Point *GetPoint()
{
	return &g_aPoint[0];
}