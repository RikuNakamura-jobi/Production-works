//==========================================================
//
//メッシュフィールド処理 [meshfield.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"
#include "texture.h"
#include "fileload.h"
#include <stdio.h>

//==========================================================
//マクロ定義
//==========================================================
#define MAX_MESHFIELD		(256)		//メッシュフィールドの最大数

//==========================================================
//プロトタイプ宣言
//==========================================================
void CreateVertexFieldBuff(void);
void CreateIndexFieldBuff(void);

//==========================================================
//グローバル変数
//==========================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;		//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshField = NULL;		//インデックスバッファへのポインタ
MeshField g_aMeshField[MAX_MESHFIELD];					//メッシュフィールド 
int g_nMeshFieldVertex;									//頂点の使用数確認
int g_nMeshFieldIndex;									//インデックスの仕様数確認
float fTexU;	//テクスチャ座標

//==========================================================
//メッシュフィールドの初期化処理
//==========================================================
void InitMeshField(void)
{
	//各種変数の初期化
	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		g_aMeshField[nCntField].Polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntField].Polygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshField[nCntField].Polygon.fHeight = 0.0f;
		g_aMeshField[nCntField].Polygon.fWidth = 0.0f;
		g_aMeshField[nCntField].bUse = false;
	}

	fTexU = 0.0f;
}

//==========================================================
//メッシュフィールドの終了処理
//==========================================================
void UninitMeshField(void)
{
	//頂点バッファの廃棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//インデックスバッファの廃棄
	if (g_pIdexBuffMeshField != NULL)
	{
		g_pIdexBuffMeshField->Release();
		g_pIdexBuffMeshField = NULL;
	}

}

//==========================================================
//メッシュフィールドの更新処理
//==========================================================
void UpdateMeshField(void)
{
	/*if (GetMode() == MODE_RANKING)
	{
		g_nMeshFieldVertex = 0;
		fTexU += 0.0003f;

		if (fTexU > 1.0f)
		{
			fTexU = 0.0f;
		}

		for (int nCntMesh = 0; nCntMesh < MAX_MESHFIELD; nCntMesh++)
		{
			if (g_aMeshField[nCntMesh].bUse == true)
			{
				SetpVtxMeshField(nCntMesh);
			}
		}
	}*/
}

//==========================================================
//メッシュフィールドの描画処理
//==========================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	int nCntIndex = 0;

	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshField[nCntField].Polygon.mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntField].Polygon.rot.y, g_aMeshField[nCntField].Polygon.rot.x, g_aMeshField[nCntField].Polygon.rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].Polygon.mtxWorld, &g_aMeshField[nCntField].Polygon.mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntField].Polygon.pos.x, g_aMeshField[nCntField].Polygon.pos.y, g_aMeshField[nCntField].Polygon.pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].Polygon.mtxWorld, &g_aMeshField[nCntField].Polygon.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntField].Polygon.mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffMeshField,
				0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdexBuffMeshField);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, *SetTexture(g_aMeshField[nCntField].nTextype));

			//メッシュフィールドの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshField[nCntField].nVertex,		//用意した頂点数
				nCntIndex,
				g_aMeshField[nCntField].nIndex - 2	//描画するプリミティブ数
			);

			nCntIndex += g_aMeshField[nCntField].nIndex;
		}
	}
}

//==========================================================
//メッシュフィールドの設定処理
//==========================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType)
{
	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == false)
		{//使用されていない場合
			g_aMeshField[nCntField].Polygon.pos = pos;
			g_aMeshField[nCntField].Polygon.rot = rot;
			g_aMeshField[nCntField].Polygon.fHeight = fHeight;
			g_aMeshField[nCntField].Polygon.fWidth = fWidth;
			g_aMeshField[nCntField].nHeightMesh = nHeight;
			g_aMeshField[nCntField].nWidthMesh = nWidth;
			g_aMeshField[nCntField].nVertex = (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nHeightMesh + 1);
			g_aMeshField[nCntField].bUse = true;
			g_aMeshField[nCntField].nIndex = (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nHeightMesh) * 2 + (g_aMeshField[nCntField].nHeightMesh - 1) * 2;
			g_aMeshField[nCntField].nTextype = nType;

			//頂点数とインデックス数を代入
			g_nMeshFieldIndex += g_aMeshField[nCntField].nIndex;
			g_nMeshFieldVertex += g_aMeshField[nCntField].nVertex;
			break;
		}
	}
}

//==========================================================
//メッシュフィールドのインデックス設定処理
//==========================================================
void SetIndexMeshField(int nCntField)
{
	WORD *pIdx;	//インデックス情報のポインタ

				//インデックスバッファをロックし頂点番号データへのポインタを取得
	g_pIdexBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	pIdx += g_nMeshFieldIndex;

	for (int nCntIdx = 0; nCntIdx < g_aMeshField[nCntField].nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + g_aMeshField[nCntField].nWidthMesh + 1)
			- (nCntIdx % (g_aMeshField[nCntField].nWidthMesh + 2) / (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nWidthMesh + 2))
			- (nCntIdx / (g_aMeshField[nCntField].nWidthMesh + 2));
		pIdx[nCntIdx * 2 + 1] = nCntIdx
			+ (nCntIdx % (g_aMeshField[nCntField].nWidthMesh + 2) / (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].nWidthMesh + 1))
			- (nCntIdx / (g_aMeshField[nCntField].nWidthMesh + 2));
		pIdx[nCntIdx * 2] += g_nMeshFieldVertex;
		pIdx[nCntIdx * 2 + 1] += g_nMeshFieldVertex;

	}

	//インデックスバッファをアンロックする
	g_pIdexBuffMeshField->Unlock();

	g_nMeshFieldIndex += g_aMeshField[nCntField].nIndex;
}

//==========================================================
//メッシュフィールドの頂点座標設定処理
//==========================================================
void SetpVtxMeshField(int nCntField)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += g_nMeshFieldVertex;

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < g_aMeshField[nCntField].nVertex; nCntpVtx++)
	{
		//頂点座標
		pVtx[nCntpVtx].pos.x = -(g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh) + (nCntpVtx % (g_aMeshField[nCntField].nWidthMesh + 1) * (g_aMeshField[nCntField].Polygon.fWidth * 2));
		pVtx[nCntpVtx].pos.y = 0.0f;
		pVtx[nCntpVtx].pos.z = (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh) + ((nCntpVtx / (g_aMeshField[nCntField].nWidthMesh + 1) * (-g_aMeshField[nCntField].Polygon.fHeight * 2)));

		//法線
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (g_aMeshField[nCntField].nWidthMesh + 1)), fTexU + 1.0f * (nCntpVtx / (g_aMeshField[nCntField].nWidthMesh + 1)));
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();

	g_nMeshFieldVertex += g_aMeshField[nCntField].nVertex;
}

//==========================================================
//メッシュフィールドの頂点バッファ生成
//==========================================================
void CreateVertexFieldBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

													//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshFieldIndex,	//必要なインデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshField,
		NULL);
}

//==========================================================
//メッシュフィールドのインデックスバッファ生成
//==========================================================
void CreateIndexFieldBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

													//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nMeshFieldVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);
}

//==========================================================
//メッシュフィールドの設置情報読み込み
//==========================================================
void LoadMeshFieldData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用
	MeshField meshField;

	//初期化
	memset(&meshField, NULL, sizeof(meshField));

	//終了文字まで読み込み
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//配置情報確認
		if (strcmp(&aStr[0], LOAD_TEXTYPE) == 0)
		{//テクスチャ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &meshField.nTextype);	//テクスチャ名読み込み
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//座標
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &meshField.Polygon.pos.x);	//x座標読み込み
			fscanf(pFile, "%f", &meshField.Polygon.pos.y);	//y座標読み込み
			fscanf(pFile, "%f", &meshField.Polygon.pos.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//向き
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &meshField.Polygon.rot.x);	//x座標読み込み
			fscanf(pFile, "%f", &meshField.Polygon.rot.y);	//y座標読み込み
			fscanf(pFile, "%f", &meshField.Polygon.rot.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_BLOCK) == 0)
		{//枚数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &meshField.nHeightMesh);	//縦幅読み込み
			fscanf(pFile, "%d", &meshField.nWidthMesh);		//横幅読み込み

		}
		else if (strcmp(&aStr[0], LOAD_SIZE) == 0)
		{//サイズ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &meshField.Polygon.fHeight);//縦幅読み込み
			fscanf(pFile, "%f", &meshField.Polygon.fWidth);	//横幅読み込み
		}

		//終了
		if (strcmp(&aStr[0], ENDFIELDSET_TXT) == 0)
		{//終了文字
			break;
		}
	}

	//フィールドの配置
	SetMeshField(meshField.Polygon.pos, D3DXToRadian(meshField.Polygon.rot),
		meshField.Polygon.fHeight, meshField.Polygon.fWidth,
		meshField.nHeightMesh, meshField.nWidthMesh, meshField.nTextype);

}

//==========================================================
//メッシュフィールドの生成関連
//==========================================================
void CreateFieldBuff(void)
{

	//頂点バッファ生成
	CreateVertexFieldBuff();

	//インデックスバッファ生成
	CreateIndexFieldBuff();

	//情報初期化
	g_nMeshFieldVertex = 0;
	g_nMeshFieldIndex = 0;

	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//使用されている場合
		 //インデックスの設定
			SetIndexMeshField(nCntField);

			//頂点座標の設定
			SetpVtxMeshField(nCntField);
		}
	}
}

//==========================================================
//メッシュフィールドとの当たり判定
//==========================================================
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{
	bool bLand = false;;
	for (int nCntField = 0; nCntField < MAX_MESHFIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//使用されている場合
			if (pPos->x + vtxMax.x >= g_aMeshField[nCntField].Polygon.pos.x - (g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh)
				&& pPos->x + vtxMin.x <= g_aMeshField[nCntField].Polygon.pos.x + (g_aMeshField[nCntField].Polygon.fWidth * g_aMeshField[nCntField].nWidthMesh)
				&& pPos->z + vtxMax.z >= g_aMeshField[nCntField].Polygon.pos.z - (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh)
				&& pPos->z + vtxMin.z <= g_aMeshField[nCntField].Polygon.pos.z + (g_aMeshField[nCntField].Polygon.fHeight * g_aMeshField[nCntField].nHeightMesh))
			{//フィールドの範囲内にいる場合
				if (pPosOld->y >= g_aMeshField[nCntField].Polygon.pos.y &&
					pPos->y < g_aMeshField[nCntField].Polygon.pos.y)
				{//めり込んだ場合
					pPos->y = g_aMeshField[nCntField].Polygon.pos.y;
					pMove->y = 0.0f;
					bLand = true;
				}
			}
		}
	}

	return bLand;
}