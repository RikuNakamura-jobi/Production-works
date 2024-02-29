//==================================================================================================
//
//Book.cpp
//Author:髙山桃也
//
//==================================================================================================

#include "main.h"
#include "book.h"
#include "model.h"
#include "fade.h"
#include "camera.h"
#include "input.h"

//************************************
// マクロ定義
//************************************
#define MOVE_SPEED	(1.0f)	// 移動速度
#define ROLL_SPEED	(0.015f)	// 回転速度
#define DIFF_HEIGHT	(10.0f) // カメラの高さとの差分
#define DIST_BOOK	(49.0f)	// カメラからの本の距離
#define BOOK_ROLL_LIMIT	(D3DX_PI * 0.9f)	// 本の回転制限

//************************************
// グローバル変数宣言
//************************************
Book g_aBook[NUM_BOOK];	//構造体の情報
D3DXVECTOR3 g_posBook;	//全体の位置
D3DXVECTOR3 g_rotBook;	//全体の向き
D3DXMATRIX g_mtxBook;	//全体のマトリックス
BOOKSTATE g_bookState;	//本の状態

//==================================================================================================
// 初期化処理
//==================================================================================================
void InitBook(void)
{
	// 変数初期化
	ZeroMemory(&g_aBook[0],sizeof(Book) * NUM_BOOK);

	// 各種変数初期化
	g_posBook = { 0.0f,100.0f,0.0f };
	g_rotBook = { 0.0f,D3DX_PI * 0.5f,0.0f };
	g_bookState = BOOKSTATE_NONE;
}

//==================================================================================================
// 終了処理
//==================================================================================================
void UninitBook(void)
{
}

//==================================================================================================
// 更新処理
//==================================================================================================
void UpdateBook(void)
{
	if (g_bookState != BOOKSTATE_END)
	{
		// 情報取得
		Camera *pCamera = GetCamera();

		//位置の設定
		g_posBook =
		{
			pCamera->posVDest.x,
			g_posBook.y,
			pCamera->posVDest.z - DIST_BOOK
		};

		switch (g_bookState)
		{
		case BOOKSTATE_NONE:
			if (GetFade() == FADE_NONE)
			{//動き始める
				g_bookState = BOOKSTATE_IN;
			}
			break;
		case BOOKSTATE_IN:
			//本下降
			g_posBook.y -= MOVE_SPEED;

			if (pCamera->posVDest.y - DIFF_HEIGHT > g_posBook.y)
			{//スライドイン終了
			 //高さ補正
				g_posBook.y = GetCamera()->posVDest.y - DIFF_HEIGHT;

				//本を開く
				g_bookState = BOOKSTATE_OPEN;
			}
			break;
		case BOOKSTATE_OPEN:
			//本開く
			g_aBook[0].rot.y += ROLL_SPEED;

			if (g_aBook[0].rot.y > BOOK_ROLL_LIMIT)
			{//本開きの終了
			 //角度の補正
				g_aBook[0].rot.y = BOOK_ROLL_LIMIT;

				//待機状態へ移行
				g_bookState = BOOKSTATE_WAIT;
			}
			break;
		case BOOKSTATE_WAIT:
			if (GetKeyboardTrigger(DIK_RETURN) || GetJoyPadButtonTrigger(KEY_START, 0) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_B, 0))
			{//待機終了
				g_bookState = BOOKSTATE_OUT;
			}
			break;
		case BOOKSTATE_OUT:
			//本閉じる
			g_aBook[0].rot.y -= ROLL_SPEED * 4.0f;

			if (g_aBook[0].rot.y < 0.0f)
			{//角度の補正
				g_aBook[0].rot.y = 0.0f;
			}

			//本上昇
			g_posBook.y += MOVE_SPEED;

			if (130.0f < g_posBook.y)
			{//スライドアウト終了
				//高さ補正
				g_posBook.y = 130.0f;

				//本を終了状態に
				g_bookState = BOOKSTATE_END;
			}
			break;
		default:
			break;
		}
	}
}

//==================================================================================================
// 描画処理
//==================================================================================================
void DrawBook(void)
{
	if (g_bookState != BOOKSTATE_END)
	{
		//デバイスポインタ設定
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//変数宣言
		D3DXMATRIX mtxRot, mtxTrans, mtxParent;
		D3DMATERIAL9 matDef;			//現在のマテリアル保存用
		D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

										//情報取得
		Book *pBook = GetBook();
		Model *pModel = GetModelBook();

		for (int nCntBook = 0; nCntBook < NUM_BOOK; nCntBook++, pBook++)
		{
			//全体のマトリックス==========================================
			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&g_mtxBook);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_rotBook.y, g_rotBook.x, g_rotBook.z);
			D3DXMatrixMultiply(&g_mtxBook, &g_mtxBook, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				g_posBook.x, g_posBook.y, g_posBook.z);
			D3DXMatrixMultiply(&g_mtxBook, &g_mtxBook, &mtxTrans);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxBook);
			//全体のマトリックス==========================================

			//本パーツ==========================================
			//ワールドマトリックス初期化
			D3DXMatrixIdentity(&pBook->mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				pBook->rot.y, pBook->rot.x, pBook->rot.z);
			D3DXMatrixMultiply(&pBook->mtxWorld, &pBook->mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				pBook->pos.x, pBook->pos.y, pBook->pos.z);
			D3DXMatrixMultiply(&pBook->mtxWorld, &pBook->mtxWorld, &mtxTrans);

			//算出したパーツのワールドマトリックスと親のマトリックスをかけ合わせる
			D3DXMatrixMultiply(&pBook->mtxWorld,
				&pBook->mtxWorld,
				&g_mtxBook);

			//ワールドマトリックス設定
			pDevice->SetTransform(D3DTS_WORLD, &pBook->mtxWorld);
			//本パーツ==========================================

			//現在のマテリアル取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pModel[nCntBook].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pModel[nCntBook].dwNumMat; nCntMat++)
			{
				//マテリアル設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャ設定
				pDevice->SetTexture(0, pModel[nCntBook].apTexture[nCntMat]);

				//モデル（パーツ）描画
				pModel[nCntBook].pMesh->DrawSubset(nCntMat);
			}

			//マテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================================
// ポインタ取得処理
//==================================================================================================
Book *GetBook(void)
{
	return &g_aBook[0];
}

//==================================================================================================
// 状態取得
//==================================================================================================
BOOKSTATE *GetBookState(void)
{
	return &g_bookState;
}