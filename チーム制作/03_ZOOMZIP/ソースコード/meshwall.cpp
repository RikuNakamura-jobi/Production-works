//==========================================================
//
//メッシュウォール処理 [meshwall.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "meshwall.h"
#include "input.h"
#include "texture.h"
#include "fileload.h"

//==========================================================
//マクロ定義
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\field000.jpg"		//テクスチャデータ
#define MAX_FIELD		(28)			//フィールドの頂点数
#define FIELD_WIDTH		(3)				//フィールドの幅
#define FIELD_HEIGHT	(3)				//フィールドの高さ
#define FIELD_TYOUTEN	(4 * 4)			//超点数
#define FIELD_WIDTHWIDTH	(100.0f)	//幅距離
#define FIELD_HEIGHTHEIGHT	(100.0f)	//奥行距離

//==========================================================
//プロトタイプ宣言
//==========================================================
void CreateVertexWallBuff(void);
void CreateIndexWallBuff(void);

//==========================================================
//グローバル変数宣言
//==========================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshWall = NULL;	//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshWall;				//位置
D3DXVECTOR3 g_rotMeshWall;				//向き
D3DXMATRIX g_mtxWorldMeshWall;			//ワールドマトリックス
int g_nMeshWallVertex;					//頂点の使用数確認
int g_nMeshWallIndex;					//インデックスの仕様数確認
MeshWall g_aMeshWall[MAX_MESHWALL];		//メッシュウォールの情報

//==========================================================
//メッシュウォールの初期化処理
//==========================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//各種変数の初期化
	g_posMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		g_aMeshWall[nCntWall].Polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].Polygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].Polygon.fHeight = 0.0f;
		g_aMeshWall[nCntWall].Polygon.fWidth = 0.0f;
		g_aMeshWall[nCntWall].nHeightMesh = 0;
		g_aMeshWall[nCntWall].nWidthMesh = 0;
		g_aMeshWall[nCntWall].nVertex = 0;
		g_aMeshWall[nCntWall].nStartIndex = 0;
		g_aMeshWall[nCntWall].bUse = false;
		
	}

	//読み込み数データの初期化
	g_nMeshWallVertex = 0;
	g_nMeshWallIndex = 0;
}

//==========================================================
//メッシュウォールの終了処理
//==========================================================
void UninitMeshWall(void)
{
	//頂点バッファの廃棄
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//インデックスバッファの廃棄
	if (g_pIdexBuffMeshWall != NULL)
	{
		g_pIdexBuffMeshWall->Release();
		g_pIdexBuffMeshWall = NULL;
	}

}

//==========================================================
//メッシュウォールの更新処理
//==========================================================
void UpdateMeshWall(void)
{
	int nVertex = 0;

	////メッシュウォールの最大数確認
	//for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	//{
	//	if (g_aMeshWall[nCntWall].bUse == true)
	//	{//使用している場合
	//		SceltonWall(nVertex, g_aMeshWall[nCntWall]);
	//	}

	//	nVertex += g_aMeshWall[nCntWall].nVertex;

	//	if (nVertex >= g_nMeshWallVertex)
	//	{//生成数を超えた場合
	//		break;
	//	}
	//}
}

//==========================================================
//メッシュウォールの描画処理
//==========================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	int nCntIndex = 0;
	int aOrderIndex[MAX_MESHWALL] = {};	//番号
	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		aOrderIndex[nCntWall] = nCntWall;
	}

	//順番確認
	//OrderWall(&aOrderIndex[0]);

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{

		if (g_aMeshWall[aOrderIndex[nCntWall]].bUse == true)
		{//使用されているとき
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.rot.y, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.rot.x, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.pos.x, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.pos.y, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffMeshWall,
				0,
				sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdexBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, *SetTexture(g_aMeshWall[aOrderIndex[nCntWall]].nTextype));

			//メッシュウォールの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshWall[aOrderIndex[nCntWall]].nVertex,		//用意した頂点数
				g_aMeshWall[aOrderIndex[nCntWall]].nStartIndex,							//インデックス開始点
				g_aMeshWall[aOrderIndex[nCntWall]].nIndex - 2	//描画するプリミティブ数
			);

			//nCntIndex += g_aMeshWall[nCntWall].nIndex;
		}
	}
}

//==========================================================
//メッシュウォールの設定
//==========================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType)
{
	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == false)
		{//使用されていない場合
			g_aMeshWall[nCntWall].Polygon.pos = pos;
			g_aMeshWall[nCntWall].Polygon.rot = rot;

			if (g_aMeshWall[nCntWall].Polygon.rot.y > D3DX_PI || g_aMeshWall[nCntWall].Polygon.rot.y < -D3DX_PI)
			{//-3.14～3.14の範囲外の場合
				if (g_aMeshWall[nCntWall].Polygon.rot.y > D3DX_PI)
				{
					g_aMeshWall[nCntWall].Polygon.rot.y += (-D3DX_PI * 2);
				}
				else if (g_aMeshWall[nCntWall].Polygon.rot.y < -D3DX_PI)
				{
					g_aMeshWall[nCntWall].Polygon.rot.y += (D3DX_PI * 2);
				}
			}

			g_aMeshWall[nCntWall].Polygon.fHeight = fHeight;
			g_aMeshWall[nCntWall].Polygon.fWidth = fWidth;
			g_aMeshWall[nCntWall].nHeightMesh = nHeight;
			g_aMeshWall[nCntWall].nWidthMesh = nWidth;
			g_aMeshWall[nCntWall].nVertex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh + 1);
			g_aMeshWall[nCntWall].bUse = true;
			g_aMeshWall[nCntWall].nIndex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh) * 2 + (g_aMeshWall[nCntWall].nHeightMesh - 1) * 2;
			g_aMeshWall[nCntWall].nTextype = nType;
			g_aMeshWall[nCntWall].nStartIndex = g_nMeshWallIndex;

			//頂点数とインデックス数を代入
			g_nMeshWallIndex += g_aMeshWall[nCntWall].nIndex;
			g_nMeshWallVertex += g_aMeshWall[nCntWall].nVertex;
			break;
		}
	}
}

//==========================================================
//メッシュウォールのインデックス番号設定
//==========================================================
void SetIndexMeshWall(int nCntWall)
{
	WORD *pIdx;	//インデックス情報のポインタ

	//インデックスバッファをロックし頂点番号データへのポインタを取得
	g_pIdexBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	pIdx += g_nMeshWallIndex;

	for (int nCntIdx = 0; nCntIdx < g_aMeshWall[nCntWall].nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + g_aMeshWall[nCntWall].nWidthMesh + 1)
			- (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 2))
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));

		pIdx[nCntIdx * 2 + 1] = nCntIdx
			+ (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 1))
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));

		pIdx[nCntIdx * 2] += g_nMeshWallVertex;

		pIdx[nCntIdx * 2 + 1] += g_nMeshWallVertex;
	}

	//インデックスバッファをアンロックする
	g_pIdexBuffMeshWall->Unlock();

	g_nMeshWallIndex += g_aMeshWall[nCntWall].nIndex;
}

//==========================================================
//メッシュウォールのpVtx設定
//==========================================================
void SetpVtxMeshWall(int nCntWall)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMeshWall->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += g_nMeshWallVertex;

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < g_aMeshWall[nCntWall].nVertex; nCntpVtx++)
	{
		//頂点座標
		pVtx[nCntpVtx].pos.x = -(g_aMeshWall[nCntWall].Polygon.fWidth * g_aMeshWall[nCntWall].nWidthMesh) + (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].Polygon.fWidth * 2));
		pVtx[nCntpVtx].pos.y = ((g_aMeshWall[nCntWall].Polygon.fHeight * 2) * g_aMeshWall[nCntWall].nHeightMesh) + ((nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (-g_aMeshWall[nCntWall].Polygon.fHeight * 2)));
		pVtx[nCntpVtx].pos.z = 0.0f;

		//法線
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//色
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1)), 1.0f * (nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1)));
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshWall->Unlock();

	g_nMeshWallVertex += g_aMeshWall[nCntWall].nVertex;
}

//==========================================================
//メッシュウォールの情報取得
//==========================================================
MeshWall *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}

//==========================================================
//メッシュフィールドの頂点バッファ生成
//==========================================================
void CreateVertexWallBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshWallIndex,	//必要なインデックス数
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshWall,
		NULL);
}

//==========================================================
//メッシュフィールドのインデックスバッファ生成
//==========================================================
void CreateIndexWallBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nMeshWallVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);
}

//==========================================================
//メッシュウォールの情報読み込み
//==========================================================
void LoadMeshWallData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用
	MeshWall meshWall;

	//初期化
	memset(&meshWall, NULL, sizeof(meshWall));

	//終了文字まで読み込み
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//配置情報確認
		if (strcmp(&aStr[0], LOAD_TEXTYPE) == 0)
		{//テクスチャ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &meshWall.nTextype);	//テクスチャ名読み込み
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//座標
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.pos.x);	//x座標読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.pos.y);	//y座標読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.pos.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//向き
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.rot.x);	//x座標読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.rot.y);	//y座標読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.rot.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_BLOCK) == 0)
		{//枚数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &meshWall.nHeightMesh);	//縦幅読み込み
			fscanf(pFile, "%d", &meshWall.nWidthMesh);		//横幅読み込み

		}
		else if (strcmp(&aStr[0], LOAD_SIZE) == 0)
		{//サイズ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.fHeight);//縦幅読み込み
			fscanf(pFile, "%f", &meshWall.Polygon.fWidth);	//横幅読み込み
		}

		//終了
		if (strcmp(&aStr[0], ENDWALLSET_TXT) == 0)
		{//終了文字
			break;
		}
	}

	if (meshWall.Polygon.rot.y > 180.0f)
	{//180度超えた場合
		meshWall.Polygon.rot.y += -360.0f;
	}
	else if (meshWall.Polygon.rot.y < -180.0f)
	{//-180度超えた場合
		meshWall.Polygon.rot.y += 360.0f;
	}

	//壁の配置
	SetMeshWall(meshWall.Polygon.pos, D3DXToRadian(meshWall.Polygon.rot),
		meshWall.Polygon.fHeight, meshWall.Polygon.fWidth,
		meshWall.nHeightMesh, meshWall.nWidthMesh, meshWall.nTextype);
}

//==========================================================
//メッシュウォールの生成関連
//==========================================================
void CreateWallBuff(void)
{

	//頂点バッファ生成
	CreateVertexWallBuff();

	//インデックスバッファ生成
	CreateIndexWallBuff();

	//情報初期化
	g_nMeshWallVertex = 0;
	g_nMeshWallIndex = 0;

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//使用されている場合
		 //インデックスの設定
			SetIndexMeshWall(nCntWall);

			//頂点座標の設定
			SetpVtxMeshWall(nCntWall);
		}
	}
}
