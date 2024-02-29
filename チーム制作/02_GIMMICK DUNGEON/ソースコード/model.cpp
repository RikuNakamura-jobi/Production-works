//*****************************************
//
//モデル管理(model.cpp)
//Author:髙山桃也
//
//*****************************************

//*****************************************
// インクルード
//*****************************************
#include "main.h"
#include "model.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "book.h"

//*****************************************
// マクロ定義
//*****************************************
#define MAX_STRING	(256)	//文字数の最大

//*****************************************
// グローバル変数宣言
//*****************************************
const char *c_apFilePassObject[OBJECTTYPE_MAX] =
{//Xファイルパス
	NULL,
	"data\\MODEL\\OBJECT\\catapult_000.x",
	"data\\MODEL\\OBJECT\\reflector_000.x",
	"data\\MODEL\\OBJECT\\reflector_base_000.x",
	"data\\MODEL\\OBJECT\\lever_001.x",
	"data\\MODEL\\OBJECT\\lever_000.x",
	"data\\MODEL\\OBJECT\\torch_000.x",
	"data\\MODEL\\OBJECT\\button_000.x",
	"data\\MODEL\\OBJECT\\pole000.x",
	"data\\MODEL\\OBJECT\\sandpole_base_000.x",
	"data\\MODEL\\OBJECT\\stonepole000.x",
	"data\\MODEL\\OBJECT\\door_000.x",
	"data\\MODEL\\OBJECT\\book_000.x",
	"data\\MODEL\\OBJECT\\rock_frag.x",
	"data\\MODEL\\OBJECT\\rubble_000.x",
	"data\\MODEL\\OBJECT\\statue_002.x",
	"data\\MODEL\\OBJECT\\debris_000.x",
	"data\\MODEL\\OBJECT\\skull_000.x",
	"data\\MODEL\\OBJECT\\skullmt_000.x",
	"data\\MODEL\\OBJECT\\statue_001.x",
	"data\\MODEL\\OBJECT\\statue_004.x",
	"data\\MODEL\\OBJECT\\statue_005.x",
	"data\\MODEL\\OBJECT\\statue_006.x",
	"data\\MODEL\\OBJECT\\statue_007.x",
	"data\\MODEL\\OBJECT\\statue_008.x",
	"data\\MODEL\\OBJECT\\statue_009.x",
	"data\\MODEL\\OBJECT\\sword_001.x",
	"data\\MODEL\\OBJECT\\treasure_box_000.x",
	"data\\MODEL\\OBJECT\\statue_100.x",
	"data\\MODEL\\OBJECT\\statue_101.x",
	"data\\MODEL\\OBJECT\\button_000.x",
};
Model g_aModelObject[OBJECTTYPE_MAX];	//オブジェクトのモデル情報
Model g_aModelPlayer[MAX_PLAYER_MODEL];	//プレイヤーのモデル情報
Model g_aModelEnemy[MAX_ENEMY_MODEL];	//敵のモデル情報
Model g_aModelBook[NUM_BOOK];	//本のモデル情報

int g_nNumPlayerParts;	//プレイヤーのパーツ数
int g_nNumEnemyParts;	//エネミーのパーツ数

//========================================
//初期化処理
//========================================
void InitModel(void)
{
	//オブジェクトモデル情報初期化
	ZeroMemory(&g_aModelObject[0],sizeof(Model) * OBJECTTYPE_MAX);

	//プレイヤーモデル情報初期化
	ZeroMemory(&g_aModelPlayer[0], sizeof(Model) * MAX_PLAYER_MODEL);

	//エネミーモデル情報初期化
	ZeroMemory(&g_aModelEnemy[0], sizeof(Model) * MAX_ENEMY_MODEL);

	//本モデル情報初期化
	ZeroMemory(&g_aModelBook[0], sizeof(Model) * NUM_BOOK);

	g_nNumPlayerParts = 0;
	g_nNumEnemyParts = 0;
}

//========================================
// 終了処理
//========================================
void UninitModel(void)
{
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{//オブジェクトモデル終了処理
		UninitXFile(&g_aModelObject[nCntObject]);
	}

	for (int nCntPart = 0;nCntPart < MAX_PLAYER_MODEL;nCntPart++)
	{//プレイヤーモデル終了処理
		UninitXFile(&g_aModelPlayer[nCntPart]);
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_MODEL; nCntEnemy++)
	{//敵モデル終了処理
		UninitXFile(&g_aModelEnemy[nCntEnemy]);
	}

	for (int nCntBook = 0; nCntBook < NUM_BOOK; nCntBook++)
	{//本モデル終了処理
		UninitXFile(&g_aModelBook[nCntBook]);
	}
}

//========================================
// 読み込み処理まとめ
//========================================
void LoadAllModel(void)
{
	//モデル読み込み
	LoadObjectModel();

	//プレイヤーモデル読み込み
	LoadPlayerModel();

	//敵モデル読み込み
	LoadEnemyModel();

	//本読み込み
	LoadBookModel();
}

//========================================
// オブジェクトのモデル情報取得
//========================================
void LoadObjectModel(void)
{
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{//モデル読み込み
		if (c_apFilePassObject[nCntObject] != NULL)
		{
			LoadModel(&g_aModelObject[nCntObject], c_apFilePassObject[nCntObject]);
		}
	}
}

//========================================
// 本のモデル情報取得
//========================================
void LoadBookModel(void)
{
	//変数宣言
	const char *acBookFilePass[NUM_BOOK] = 
	{
		"data\\MODEL\\OBJECT\\book_001.x",
		"data\\MODEL\\OBJECT\\book_002.x",
	};

	for (int nCntBook = 0; nCntBook < NUM_BOOK; nCntBook++)
	{//モデル読み込み
		if (acBookFilePass[nCntBook] != NULL)
		{
			LoadModel(&g_aModelBook[nCntBook], acBookFilePass[nCntBook]);
		}
	}
}

//========================================
// プレイヤーのモデル情報取得
//========================================
void LoadPlayerModel(void)
{
	//変数宣言
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	char acFilenamePlayer[MAX_PLAYER_MODEL][MAX_STRING];

	//ファイルからモデルを読み込む
	FILE *pFile = fopen("data\\MOTION\\motion_TH.txt", "r");

	while (1)
	{
		//文字読み込み
		fscanf(pFile, "%s", &cTemp[0]);

		//ファイル名読み込み=========================================
		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{//モデル数読み込み
			//"="読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			//モデル数読み込み
			fscanf(pFile, "%d", &g_nNumPlayerParts);

			for (int nCntFile = 0; nCntFile < g_nNumPlayerParts;)
			{//ファイル名読み込み
				//文字読み込み
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "MODEL_FILENAME") == 0)
				{//ファイル名読み込み
					//"="読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					//ファイル名取得
					fscanf(pFile, "%s", &acFilenamePlayer[nCntFile][0]);

					//Xファイルの読み込み
					LoadModel(&g_aModelPlayer[nCntFile], acFilenamePlayer[nCntFile]);

					//読み込んだモデル数カウンタ加算
					nCntFile++;
				}
			}
		}
		//ファイル名読み込み=========================================

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			break;
		}
	}
}

//========================================
// エネミーのモデル情報取得
//========================================
void LoadEnemyModel(void)
{
	//変数宣言
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	char acFilenameEnemy[MAX_ENEMY_MODEL][MAX_STRING];

	//ファイルからモデルを読み込む
	FILE *pFile = fopen("data\\MOTION\\motion_golem.txt", "r");

	while (1)
	{
		//文字読み込み
		fscanf(pFile, "%s", &cTemp[0]);

		//ファイル名読み込み=========================================
		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{//モデル数読み込み
		 //"="読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			//モデル数読み込み
			fscanf(pFile, "%d", &g_nNumEnemyParts);

			for (int nCntFile = 0; nCntFile < g_nNumEnemyParts;)
			{//ファイル名読み込み
			 //文字読み込み
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "MODEL_FILENAME") == 0)
				{//ファイル名読み込み
				 //"="読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					//ファイル名取得
					fscanf(pFile, "%s", &acFilenameEnemy[nCntFile][0]);

					//Xファイルの読み込み
					LoadModel(&g_aModelEnemy[nCntFile], acFilenameEnemy[nCntFile]);

					//読み込んだモデル数カウンタ加算
					nCntFile++;
				}
			}
		}
		//ファイル名読み込み=========================================

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			break;
		}
	}
}

//========================================
// モデルの読み込み(汎用)
//========================================
void LoadModel(Model *pModel,const char *pFilePass)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//計算用ローカル変数宣言
	int nNumVtx;	//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファへのポインタ
	
	//マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pFilePass,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pModel->apTexture[nCntMat]);
		}
	}

	//最大・最小頂点数取得================================================
	//頂点数を取得
	nNumVtx = pModel->pMesh->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pModel->pMesh->GetFVF());

	//頂点バッファのロック
	pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntObject = 0; nCntObject < nNumVtx; nCntObject++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//頂点座標の代入

		if (vtx.x < pModel->vtxMin.x)
		{//X軸の最小
			pModel->vtxMin.x = vtx.x;
		}
		if (vtx.x > pModel->vtxMax.x)
		{//X軸の最大
			pModel->vtxMax.x = vtx.x;
		}

		if (vtx.y < pModel->vtxMin.y)
		{//Y軸の最小
			pModel->vtxMin.y = vtx.y;
		}
		if (vtx.y > pModel->vtxMax.y)
		{//Y軸の最大
			pModel->vtxMax.y = vtx.y;
		}

		if (vtx.z < pModel->vtxMin.z)
		{//Z軸の最小
			pModel->vtxMin.z = vtx.z;
		}
		if (vtx.z > pModel->vtxMax.z)
		{//Z軸の最大
			pModel->vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;			//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	pModel->pMesh->UnlockVertexBuffer();
	//最大・最小頂点数取得================================================
}

//========================================
// オブジェクトモデル情報取得
//========================================
Model *GetModelobject(void)
{
	return &g_aModelObject[0];
}

//========================================
// 本モデル情報取得
//========================================
Model *GetModelBook(void)
{
	return &g_aModelBook[0];
}

//========================================
// プレイヤーモデル情報取得
//========================================
Model *GetModelPlayer(void)
{
	return &g_aModelPlayer[0];
}

//========================================
// プレイヤーモデル情報取得
//========================================
Model *GetModelEnemy(void)
{
	return &g_aModelEnemy[0];
}

//========================================
// プレイヤーモデル数取得
//========================================
int GetNumPlayerPart(void)
{
	return g_nNumPlayerParts;
}

//========================================
// プレイヤーモデル数取得
//========================================
int GetNumEnemyPart(void)
{
	return g_nNumEnemyParts;
}

//========================================
// 汎用終了処理
//========================================
void UninitXFile(Model *pModel)
{
	if (pModel->pBuffMat != NULL)
	{//マテリアルの破棄
		pModel->pBuffMat->Release();
		pModel->pBuffMat = NULL;
	}

	if (pModel->pMesh != NULL)
	{//メッシュの破棄
		pModel->pMesh->Release();
		pModel->pMesh = NULL;
	}

	for (int nCntTex = 0; nCntTex < NUM_TEX; nCntTex++)
	{//テクスチャの破棄
		if (pModel->apTexture[nCntTex] != NULL)
		{
			pModel->apTexture[nCntTex]->Release();
			pModel->apTexture[nCntTex] = NULL;
		}
	}
}