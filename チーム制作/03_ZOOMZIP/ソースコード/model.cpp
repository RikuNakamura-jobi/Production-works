//==========================================================
//
//Xファイルモデル処理 [model.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "fileload.h"
#include "meshwall.h"
#include "debugproc.h"
#include "result.h"

//==========================================================
//マクロ定義
//==========================================================
#define STATE_TXT		"STATESET"		//ファイル情報確認
#define END_STATE_TXT	"END_STATESET"	//ファイル情報確認
#define WEIGHT_TXT		"WEIGHT"		//重さ確認文字
#define SKELTONLENGTH	(0.9f)			//半透明判定距離倍率
#define RANKING_MODELSET	(-900.0f)	//モデル設定位置
#define PUSH_MOVE		(0.8f)			//押している際の移動量減衰
#define POWER_MOVE		(0.98f)			//power装着時の移動量減衰
#define SKELTON_ALPHA	(0.2f)			//透過カラー
#define SKELTON_ALPMOVE	(0.02f)			//透過移動量

//==========================================================
//プロトタイプ宣言
//==========================================================
void SetVtxSize(int nCntModel);

//==========================================================
//グローバル変数宣言
//==========================================================
Model g_aModel[MAX_MODEL];				//モデル情報
ModelFileData g_aModelFileData[MAX_MODELFILE];	//モデルファイル情報

//==========================================================
//モデルの初期化処理
//==========================================================
void InitModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//デバイスへのポインタ

	//各種変数の初期化
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].nShadow = -1;
		g_aModel[nCntModel].fMoveY = 0.0f;
		g_aModel[nCntModel].nType = 0;
		g_aModel[nCntModel].nShadow = -1;
		g_aModel[nCntModel].bUse = false;
		g_aModel[nCntModel].pModelFiledata = NULL;
	}

	//モデルデータ最大値最小値初期化
	for (int nCntModelFile = 0; nCntModelFile < MAX_MODELFILE; nCntModelFile++)
	{
		g_aModelFileData[nCntModelFile].vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_aModelFileData[nCntModelFile].vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_aModelFileData[nCntModelFile].nType = MODELTYPE_OBJECT;
	}
}

//==========================================================
//モデルの終了処理
//==========================================================
void UninitModel(void)
{
	for (int nCntModelFile = 0; nCntModelFile < MAX_MODELFILE; nCntModelFile++)
	{
		//メッシュの廃棄
		if (g_aModelFileData[nCntModelFile].pMesh != NULL)
		{
			g_aModelFileData[nCntModelFile].pMesh->Release();
			g_aModelFileData[nCntModelFile].pMesh = NULL;
		}

		//マテリアルの廃棄
		if (g_aModelFileData[nCntModelFile].pBuffMat != NULL)
		{
			g_aModelFileData[nCntModelFile].pBuffMat->Release();
			g_aModelFileData[nCntModelFile].pBuffMat = NULL;
		}

		//テクスチャの廃棄
		for (int nCntTexture = 0; nCntTexture < MAX_MODELTEX; nCntTexture++)
		{
			if (g_aModelFileData[nCntModelFile].apTexture[nCntTexture] != NULL)
			{
				g_aModelFileData[nCntModelFile].apTexture[nCntTexture]->Release();
				g_aModelFileData[nCntModelFile].apTexture[nCntTexture] = NULL;
			}
		}
	}

}

//==========================================================
//モデルの更新処理
//==========================================================
void UpdateModel(void)
{

}

//==========================================================
//モデルの描画処理
//==========================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{//使用されている場合
				//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pModelFiledata->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].pModelFiledata->dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aModel[nCntModel].pModelFiledata->apTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_aModel[nCntModel].pModelFiledata->pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
//モデルの設定
//==========================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSetShadow)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{//使用されていない場合
			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].rot = rot;
			g_aModel[nCntModel].nType = nType;
			g_aModel[nCntModel].pModelFiledata = GetModelData(g_aModel[nCntModel].nType);
			g_aModel[nCntModel].bUse = true;
			g_aModel[nCntModel].fMoveY = 0.0f;

			break;
		}
	}
}

//==========================================================
//モデルデータを取得
//==========================================================
ModelFileData *GetModelData(int nModelType)
{
	return &g_aModelFileData[nModelType];
}

//==========================================================
//ファイル読み込み
//==========================================================
void LoadModelFile(int nMaxModelFile)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//ファイル数分読み込み
	for (int nCntModelFile = 0; nCntModelFile < nMaxModelFile; nCntModelFile++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(pModelFileName(nCntModelFile),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelFileData[nCntModelFile].pBuffMat,
			NULL,
			&g_aModelFileData[nCntModelFile].dwNumMat,
			&g_aModelFileData[nCntModelFile].pMesh);

		//当たり判定サイズ取得
		SetVtxSize(nCntModelFile);

		D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aModelFileData[nCntModelFile].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelFileData[nCntModelFile].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャファイルが存在している
			 //テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelFileData[nCntModelFile].apTexture[nCntMat]);
			}
		}
	}
}

//==========================================================
//Xファイル最大値最小値取得
//==========================================================
void SetVtxSize(int nCntModel)
{
	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファのポインタ

	//頂点数を取得
	nNumVtx = g_aModelFileData[nCntModel].pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_aModelFileData[nCntModel].pMesh->GetFVF());

	//頂点バッファをロック
	g_aModelFileData[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//頂点座標の代入

		if (vtx.x < g_aModelFileData[nCntModel].vtxMin.x)
		{//最小値よりも値が小さい場合
			g_aModelFileData[nCntModel].vtxMin.x = vtx.x;
		}
		else if (vtx.x > g_aModelFileData[nCntModel].vtxMax.x)
		{//最大値よりも値が大きい場合
			g_aModelFileData[nCntModel].vtxMax.x = vtx.x;
		}

		if (vtx.z < g_aModelFileData[nCntModel].vtxMin.z)
		{//最小値よりも値が小さい場合
			g_aModelFileData[nCntModel].vtxMin.z = vtx.z;
		}
		else if (vtx.z > g_aModelFileData[nCntModel].vtxMax.z)
		{//最大値よりも値が大きい場合
			g_aModelFileData[nCntModel].vtxMax.z = vtx.z;
		}

		if (vtx.y < g_aModelFileData[nCntModel].vtxMin.y)
		{//最小値よりも値が小さい場合
			g_aModelFileData[nCntModel].vtxMin.y = vtx.y;
		}
		else if (vtx.y > g_aModelFileData[nCntModel].vtxMax.y)
		{//最大値よりも値が大きい場合
			g_aModelFileData[nCntModel].vtxMax.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_aModelFileData[nCntModel].pMesh->UnlockVertexBuffer();
}

//==========================================================
//モデルタイプ情報読み込み
//==========================================================
void LoadModelFileData(FILE *pFile)
{
	int nNumCnt = 0;	//現在のファイル番号
	char aStr[256];
	do
	{
			//テキスト読み込み
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], STATE_TXT) == 0)
			{//スクリプト開始の文字が確認できた場合
				while (1)
				{
					nResult = fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(&aStr[0], LOAD_MODELTYPE) == 0)
					{//モデル種類
						fscanf(pFile, "%s", &aStr[0]);	//=
						fscanf(pFile, "%d", &g_aModelFileData[nNumCnt].nType);		//種類取得
					}
					else if (strcmp(&aStr[0], END_STATE_TXT) == 0)
					{//終了文字	
						nNumCnt++;	//次のファイルへ
						break;
					}
					else if (nResult == EOF)
					{//ファイルの最後まで読み込んでしまった場合
						nNumCnt = GetModelCnt();
						break;
					}
				}
			}
			else if (nResult == EOF)
			{//ファイルの最後まで読み込んでしまった場合
				break;
			}
	}while (nNumCnt < GetModelCnt());	//モデルファイル総数まで読み込み
}

//==========================================================
//モデル配置データ読み込み
//==========================================================
void LoadModelData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用
	Model Model;
	int nType = 0;	//使用するモデル番号

	//初期化
	memset(&Model, NULL, sizeof(Model));

	//終了文字まで読み込み
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);
		//配置情報確認
		if (strcmp(&aStr[0], LOAD_MODELTYPE) == 0)
		{//モデル種類
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &Model.nType);	//使用するモデル番号読み込み
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//座標
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &Model.pos.x);	//x座標読み込み
			fscanf(pFile, "%f", &Model.pos.y);	//y座標読み込み
			fscanf(pFile, "%f", &Model.pos.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//向き
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &Model.rot.x);	//x座標読み込み
			fscanf(pFile, "%f", &Model.rot.y);	//y座標読み込み
			fscanf(pFile, "%f", &Model.rot.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_SHADOW) == 0)
		{//影
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &Model.nShadow);	//影読み込み
		}

		//終了
		if (strcmp(&aStr[0], ENDMODELSET_TXT) == 0)
		{//終了文字
			break;
		}
	}

	SetModel(Model.pos, D3DXToRadian(Model.rot), Model.nType, Model.nShadow);
}

//==========================================================
//モデルの情報取得
//==========================================================
Model *GetModel(void)
{
	return &g_aModel[0];
}

//==========================================================
//指定番号モデルを使用されていない状態にする
//==========================================================
void DeleteModel(int nCntModel)
{
	g_aModel[nCntModel].bUse = false;			//使用されていない状態にする
	g_aModel[nCntModel].pModelFiledata = NULL;	//モデルのデータをNULLにする
}

//==========================================================
//モデルの向きによる当たり判定サイズの調整
//==========================================================
void RotSetVtxSize(D3DXVECTOR3 *SetVtxMax, D3DXVECTOR3 *SetVtxMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, float fRot)
{
	//向きによって変更する
	if (fRot == 0.0f || fRot == -0.0f)
	{//左向き
		//最大値反映
		SetVtxMax->x = vtxMax.x;	//x座標
		SetVtxMax->y = vtxMax.y;	//y座標
		SetVtxMax->z = vtxMax.z;	//z座標

		//最小値反映
		SetVtxMin->x = vtxMin.x;	//x座標
		SetVtxMin->y = vtxMin.y;	//y座標
		SetVtxMin->z = vtxMin.z;	//z座標
	}
	if (fRot == D3DX_PI * 0.5f)
	{//左向き
		//最大値反映
		SetVtxMax->x = vtxMax.z;	//x座標
		SetVtxMax->y = vtxMax.y;	//y座標
		SetVtxMax->z = -vtxMin.x;	//z座標

		//最小値反映
		SetVtxMin->x = vtxMin.z;	//x座標
		SetVtxMin->y = vtxMin.y;	//y座標
		SetVtxMin->z = -vtxMax.x;	//z座標
	}
	else if (fRot == -D3DX_PI * 0.5f)
	{//右向き
	 //最大値反映
		SetVtxMax->x = -vtxMin.z;	//x座標
		SetVtxMax->y = vtxMax.y;	//y座標
		SetVtxMax->z = vtxMax.x;	//z座標

		//最小値反映
		SetVtxMin->x = -vtxMax.z;	//x座標
		SetVtxMin->y = vtxMin.y;	//y座標
		SetVtxMin->z = vtxMin.x;	//z座標
	}
	else if (fRot == -D3DX_PI || fRot == D3DX_PI)
	{//反転している場合
		//高さ以外の最大値を最小値にする
		SetVtxMax->x = -vtxMin.x;	//x座標
		SetVtxMax->y = vtxMax.y;	//y座標
		SetVtxMax->z = -vtxMin.z;	//z座標

		//高さ以外の最小値を最大値にする
		SetVtxMin->x = -vtxMax.x;	//x座標
		SetVtxMin->y = vtxMin.y;	//y座標
		SetVtxMin->z = -vtxMax.z;	//z座標
	}
	else
	{//それ以外の場合
		//最大値反映
		SetVtxMax->x = vtxMax.x;	//x座標
		SetVtxMax->y = vtxMax.y;	//y座標
		SetVtxMax->z = vtxMax.z;	//z座標

		//最小値反映
		SetVtxMin->x = vtxMin.x;	//x座標
		SetVtxMin->y = vtxMin.y;	//y座標
		SetVtxMin->z = vtxMin.z;	//z座標
	}
}