//========================================
//
//3Dモデルのエディット機能[edit.cpp]
//Author:Kai Takada
//
//========================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "edit.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "stdio.h"

//==========================================
//  マクロ定義
//==========================================
#define EDITER_MOVE (2.0f)		//エディターのサイズ
#define EDIT_MAX (3000)			//エディターの最大数
#define TEX_MAX (10)			//テクスチャの数
#define DEBUG_NUM (11)			//デバッグ表示の文字列の数
#define SAVE_MAX (6)			//セーブする情報の種類の数
#define MAP_FILE "data\\SAVE\\MAP\\editdata003.txt"		//セーブデータテキストファイル
#define MAP_RANKING "data\\SAVE\\MAP\\RankingMap000.txt"		//セーブデータテキストファイル
#define MAP_TITLE "data\\SAVE\\MAP\\TitleMap000.txt"		//セーブデータテキストファイル
#define MAP_TUTORIAL "data\\SAVE\\MAP\\TutorialMap000.txt"		//セーブデータテキストファイル

//==========================================
//  列挙型定義
//==========================================
//エディットの状態の列挙型
typedef enum
{
	EDITMODE_SET = 0,		//設置ツール
	EDITMODE_DELETE,		//削除ツール
	EDITMODE_MAX
}EDITMODE;

//エディットが扱う種類の列挙型
typedef enum
{
	EDITTYPE_OBJECT = 0,		//オブジェクト
	EDITTYPE_PLAYER,		//プレイヤー
	EDITTYPE_ENEMY,		//敵
	EDITTYPE_WALL,		//壁
	EDITTYPE_MAX
}EDITTYPE;

//==========================================
//  構造体宣言
//==========================================
//セーブ専用構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nType;
	float fHeight;
	float fWidth;
}EDITSAVE;

//エディット先の何番目を使用するのか決める構造体
typedef struct
{
	int nObjectType;
}EDITNUM;

//エディットカーソルの色構造体
typedef struct
{
	D3DXMATERIAL SetMat;		//設置モード色
	D3DXMATERIAL DeleteMat;		//削除モード色
}EDITCOLOR;

//Xファイル用の構造体
typedef struct
{
	DWORD dwNumMat = 0;		//マテリアルの数
	LPD3DXMESH pMesh = {};		//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat = {};		//マテリアルへのポインタ
	LPDIRECT3DTEXTURE9 pTexture[TEX_MAX] = {};	//テクスチャへのポインタ
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;		//モデルの最小値・最大値
}Xcont;

//エディター構造体
typedef struct
{
	D3DXVECTOR3 pos;		//座標
	D3DXVECTOR3 rot;		//向き
	float fHeight,fWidth;	//ポリゴン用高さ・幅
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	EDITNUM EditModeType;	//何番目をSetするか
	int EditType;	//どのタイプを選択中か
	Xcont xData;		//Xファイルの情報
	EDITCOLOR col;		//エディター自体の色
	int nState;		//エディターの状態
}Edit;

//==========================================
//  プロトタイプ宣言
//==========================================
void MoveEdit(void);	//エディターの移動処理
void RotEdit(void);		//エディターの方向処理
void SetEdit(void);		//エディターによる設置処理
void DeleteEdit(void);	//エディターによる削除処理
void DebugEdit(void);	//デバッグ表示処理
void SaveEdit(void);	//エディット内容のセーブ
void SetXDataEdit(void);		//Set予定のモデル情報を取得
void SelectIdx(int nSelect);	//選択中のインデックスを使用されているものにずらす

//==========================================
//  グローバル変数宣言
//==========================================
Edit g_edit;						//エディット情報
int g_nSelectIdx;	//現在選択中のインデックス(番号)

//==================================================
//エディターの初期化処理
//==================================================
void InitEdit(void)
{
	g_edit.pos = { 0.0f,0.0f,0.0f };
	g_edit.rot = { 0.0f,0.0f,0.0f };
	g_edit.fHeight = 0.0f;
	g_edit.fWidth = 0.0f;
	g_edit.EditModeType.nObjectType = 1;		//モデルの0がコメントアウト状態のため1を代入
	g_edit.nState = EDITMODE_SET;
	g_edit.EditType = EDITTYPE_OBJECT;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//設置時色
	ZeroMemory(&g_edit.col.SetMat, sizeof(D3DXMATERIAL));
	g_edit.col.SetMat.MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
	g_edit.col.SetMat.MatD3D.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	g_edit.col.SetMat.MatD3D.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.8f, 1.0f);

	//削除時色
	ZeroMemory(&g_edit.col.DeleteMat, sizeof(D3DXMATERIAL));
	g_edit.col.DeleteMat.MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.2f, 0.2f, 0.5f);
	g_edit.col.DeleteMat.MatD3D.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	g_edit.col.DeleteMat.MatD3D.Emissive = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);

	int nNumVtx;		//頂点数
	DWORD dwSizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;		//頂点バッファへのポインタ

	SetXDataEdit();

	//頂点数を取得
	nNumVtx = g_edit.xData.pMesh->GetNumVertices();

	//頂点フォーマットのサイズの取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_edit.xData.pMesh->GetFVF());

	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_edit.xData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_edit.xData.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//テクスチャのファイル名
				&g_edit.xData.pTexture[nCntMat]);
		}
	}

	//頂点バッファをロック
	g_edit.xData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//頂点座標の代入

		//すべての頂点を比較して敵の最小値・最大値を抜き出す
		if (g_edit.xData.vtxMaxObject.x < vtx.x)
		{
			g_edit.xData.vtxMaxObject.x = vtx.x;
		}
		if (g_edit.xData.vtxMaxObject.y < vtx.y)
		{
			g_edit.xData.vtxMaxObject.y = vtx.y;
		}
		if (g_edit.xData.vtxMaxObject.z < vtx.z)
		{
			g_edit.xData.vtxMaxObject.z = vtx.z;
		}

		if (g_edit.xData.vtxMinObject.x > vtx.x)
		{
			g_edit.xData.vtxMinObject.x = vtx.x;
		}
		if (g_edit.xData.vtxMinObject.y > vtx.y)
		{
			g_edit.xData.vtxMinObject.y = vtx.y;
		}
		if (g_edit.xData.vtxMinObject.z > vtx.z)
		{
			g_edit.xData.vtxMinObject.z = vtx.z;
		}
		
		pVtxBuff += dwSizeFVF;			//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	g_edit.xData.pMesh->UnlockVertexBuffer();
}

//==================================================
//エディターの終了処理
//==================================================
void UninitEdit(void)
{
	for (int nCntTex = 0;nCntTex < TEX_MAX;nCntTex++)
	{
		if (g_edit.xData.pTexture[nCntTex] != NULL)
		{
			g_edit.xData.pTexture[nCntTex]->Release();
			g_edit.xData.pTexture[nCntTex] = NULL;					//テクスチャ破棄
		}
	}

	if (g_edit.xData.pBuffMat != NULL)
	{
		g_edit.xData.pBuffMat->Release();
		g_edit.xData.pBuffMat = NULL;					//バッファの破棄
	}

	if (g_edit.xData.pMesh != NULL)
	{
		g_edit.xData.pMesh->Release();
		g_edit.xData.pMesh = NULL;					//メッシュの破棄
	}
}

//==================================================
//エディターの更新処理
//==================================================
void UpdateEdit(void)
{
	MoveEdit();
	RotEdit();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{//[ F2 ]キーでエディットのタイプ変更
		g_edit.EditType++;
		g_edit.EditType %= EDITTYPE_MAX;

		SetXDataEdit();
	}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{//[ F3 ]キーでブロックのタイプ変更+
		if (g_edit.EditType == EDITTYPE_OBJECT)
		{
			g_edit.EditModeType.nObjectType++;

			if (g_edit.EditModeType.nObjectType == OBJECTTYPE_LEVER_BAR || 
				g_edit.EditModeType.nObjectType == OBJECTTYPE_REFLECTOR_BASE)
			{
				g_edit.EditModeType.nObjectType++;
			}

			g_edit.EditModeType.nObjectType %= OBJECTTYPE_MAX;
			SetXDataEdit();
		}
	}
	
	if (GetKeyboardTrigger(DIK_F4) == true)
	{//[ F4 ]キーでブロックのタイプ変更-
		if (g_edit.EditType == EDITTYPE_OBJECT)
		{
			g_edit.EditModeType.nObjectType--;
			if (g_edit.EditModeType.nObjectType == OBJECTTYPE_LEVER_BAR ||
				g_edit.EditModeType.nObjectType == OBJECTTYPE_REFLECTOR_BASE)
			{
				g_edit.EditModeType.nObjectType--;
			}

			if (g_edit.EditModeType.nObjectType == -1)
			{
				g_edit.EditModeType.nObjectType = OBJECTTYPE_MAX - 1;
			} 
			SetXDataEdit();
		}
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//[ F5 ]キーでエディターの状態を通常に戻す
		g_edit.nState++;
		g_edit.nState %= EDITMODE_MAX;
	}

	if (GetKeyboardTrigger(DIK_F6) == true)
	{//[ F6 ]キーで更地にする
		Object *pObject = ObjectGet();
		Wall *pWall = GetWall();

		switch (g_edit.EditType)
		{
		case EDITTYPE_OBJECT:
			for (int nCntDelete = 0; nCntDelete < MAX_OBJECT; nCntDelete++, pObject++)
			{
				if (pObject->bUse == true)
				{
					pObject->bUse = false;
					pObject->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					pObject->nType = 0;
				}
			}
			break;
		case EDITTYPE_WALL:
			for (int nCntDelete = 0; nCntDelete < MAX_WALL; nCntDelete++, pWall++)
			{
				if (pWall->bUse == true)
				{
					pWall->bUse = false;
					pWall->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_F9) == true)
	{//[ F9 ]キーでtxtファイルにセーブ
		SaveEdit();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//[ Enter ]キーで設置
		switch (g_edit.nState)
		{
		case EDITMODE_SET:
			SetEdit();
			break;
		case EDITMODE_DELETE:
			DeleteEdit();
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_K) == true)
	{//[ K ]キーで選択対象をずらす
		g_nSelectIdx++;

		SelectIdx(g_nSelectIdx);
	}
	if (GetKeyboardTrigger(DIK_L) == true)
	{//[ L ]キーで選択対象を一つずらす
		g_nSelectIdx--;

		if (g_nSelectIdx < 0)
		{
			g_nSelectIdx = 0;
		}

		Object *pObject = ObjectGet();

		if (pObject[g_nSelectIdx].bUse == false)
		{//選択対象が使用されていなかったら
			while (1)
			{//無限ループ
				g_nSelectIdx--;

				if (pObject[g_nSelectIdx].bUse == true)
				{//使われているのを見つけたら
					break;
				}
				if (g_nSelectIdx < 0)
				{//全て見たら
					g_nSelectIdx = 0;
					break;
				}
			}
		}
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//[ → ]キーで幅を伸ばす
		g_edit.fWidth += EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_LEFT) == true)
	{//[ ← ]キーで幅を縮める
		g_edit.fWidth -= EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_UP) == true)
	{//[ ↑ ]キーで高さを伸ばす
		g_edit.fHeight += EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN) == true)
	{//[ ↓ ]キーで高さを縮める
		g_edit.fHeight -= EDITER_MOVE;
	}
}

//==================================================
//エディターの描画処理
//==================================================
void DrawEdit(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_edit.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_edit.rot.y, g_edit.rot.x, g_edit.rot.z);

	D3DXMatrixMultiply(&g_edit.mtxWorld, &g_edit.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		g_edit.pos.x, g_edit.pos.y, g_edit.pos.z);

	D3DXMatrixMultiply(&g_edit.mtxWorld, &g_edit.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_edit.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_edit.xData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_edit.xData.dwNumMat; nCntMat++)
	{
		switch (g_edit.nState)
		{
		case EDITMODE_SET:
			//マテリアルの設定
			pDevice->SetMaterial(&g_edit.col.SetMat.MatD3D);
			break;

		case EDITMODE_DELETE:
			//マテリアルの設定
			pDevice->SetMaterial(&g_edit.col.DeleteMat.MatD3D);
			break;
		}

		//テクスチャの設定
		pDevice->SetTexture(0, g_edit.xData.pTexture[nCntMat]);

		//カーソルの描画
		g_edit.xData.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	DebugEdit();
}

//==================================================
//エディターの移動処理
//==================================================
void MoveEdit(void)
{
	Camera *pCamera = GetCamera();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fSpeed = EDITER_MOVE;

	if (GetKeyboardPress(DIK_LSHIFT))
	{//加速するかどうか
		fSpeed = EDITER_MOVE * 2;
	}

	//キーが押されたとき
	if (GetKeyboardPress(DIK_A) == true)
	{//Aキーが押された
		if (GetKeyboardPress(DIK_W) == true)
		{//左上移動
			move.x += cosf(pCamera->rot.y + -D3DX_PI * 0.75f) * fSpeed;
			move.z += sinf(pCamera->rot.y + -D3DX_PI * 0.75f) * fSpeed;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//左下移動
			move.x += cosf(pCamera->rot.y + -D3DX_PI * 0.25f) * fSpeed;
			move.z += sinf(pCamera->rot.y + -D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//左移動
			move.x += cosf(pCamera->rot.y + -D3DX_PI * 0.5f) * fSpeed;
			move.z += sinf(pCamera->rot.y + -D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//Dキーが押された

		if (GetKeyboardPress(DIK_W) == true)
		{//
			move.x += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;
			move.z += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//
			move.x += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;
			move.z += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//
			move.x += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * fSpeed;
			move.z += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//Wキーが押された
		move.x += cosf(pCamera->rot.y + D3DX_PI * 1.0f) * fSpeed;
		move.z += sinf(pCamera->rot.y + D3DX_PI * 1.0f) * fSpeed;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//Sキーが押された
		move.x += cosf(pCamera->rot.y + D3DX_PI * 0.0f) * fSpeed;
		move.z += sinf(pCamera->rot.y + D3DX_PI * 0.0f) * fSpeed;
	}

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//Sキーが押された
		move.x *= 2.0f;
		move.z *= 2.0f;
	}

	if (GetKeyboardPress(DIK_T) == true)
	{//[ T ]キーが押された
		move.y += EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{//[ B ]キーが押された
		move.y -= EDITER_MOVE;
	}

	g_edit.pos.x += move.x;
	g_edit.pos.y += move.y;
	g_edit.pos.z += move.z;
}

//==================================================
//エディターの向きの処理
//==================================================
void RotEdit(void)
{
	if (GetKeyboardPress(DIK_Z) == true)
	{//[ Z ]キーが押された
		g_edit.rot.y -= 0.01f;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{//[ C ]キーが押された
		g_edit.rot.y += 0.01f;
	}

	if (GetKeyboardTrigger(DIK_V) == true)
	{//[ V ]キーが押された
		g_edit.rot.y += D3DX_PI * 0.5;
	}
	if (GetKeyboardTrigger(DIK_X) == true)
	{//[ X ]キーが押された
		g_edit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_edit.rot.y > D3DX_PI)
	{
		g_edit.rot.y += -D3DX_PI * 2;
	}
	else if (g_edit.rot.y < -D3DX_PI)
	{
		g_edit.rot.y += D3DX_PI * 2;
	}
}

//==================================================
//エディターの移動処理
//==================================================
void SelectIdx(int nSelect)
{
	Object *pObject = ObjectGet();
	Wall *pWall = GetWall();

	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		if (pObject[g_nSelectIdx].bUse == false)
		{//選択対象が使用されていなかったら
			while (1)
			{
				g_nSelectIdx++;

				if (pObject[g_nSelectIdx].bUse == true)
				{//使われているのを見つけたら
					break;
				}
				if (g_nSelectIdx > MAX_OBJECT)
				{//全て見たら
					g_nSelectIdx = 0;
					break;
				}
			}
		}
		break;

	case EDITTYPE_WALL:
		if (pWall[g_nSelectIdx].bUse == false)
		{//選択対象が使用されていなかったら
			while (1)
			{
				g_nSelectIdx++;

				if (pWall[g_nSelectIdx].bUse == true)
				{//使われているのを見つけたら
					break;
				}
				if (g_nSelectIdx > MAX_WALL)
				{//全て見たら
					g_nSelectIdx = 0;
					break;
				}
			}
		}
		break;
	}
}

//==================================================
//エディターによる設置処理
//==================================================
void SetEdit(void)
{
	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		SetObject(g_edit.pos, g_edit.rot, g_edit.EditModeType.nObjectType);		//設置

		switch (g_edit.EditModeType.nObjectType)
		{
		case OBJECTTYPE_LEVER_BASE:
			SetObject(D3DXVECTOR3(g_edit.pos.x, g_edit.pos.y + 40.0f, g_edit.pos.z),
						g_edit.rot,
						OBJECTTYPE_LEVER_BAR);
			break;
		case OBJECTTYPE_REFLECTOR:
			SetObject(g_edit.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBJECTTYPE_REFLECTOR_BASE);
			break;
		case OBJECTTYPE_SANDPOLE:
			SetObject(g_edit.pos, g_edit.rot, OBJECTTYPE_SANDPOLE_BASE);
			break;
		}
		break;
	case EDITTYPE_PLAYER:
		SetPosPlayer(g_edit.pos, g_edit.rot);		//座標変更
		break;
	case EDITTYPE_ENEMY:
		SetPosEnemy(g_edit.pos, g_edit.rot);		//座標変更
		break;
	case EDITTYPE_WALL:
		SetWall(g_edit.pos, g_edit.rot,g_edit.fWidth,g_edit.fHeight);		//設置
		break;
	}
}

//==================================================
//エディターによる削除処理
//==================================================
void DeleteEdit(void)
{
	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		DeleteObject(g_nSelectIdx);
		break;
	case EDITTYPE_WALL:
		DeleteWall(g_nSelectIdx);
		break;
	}
}

//==================================================
//Xファイル情報の取得処理
//==================================================
void SetXDataEdit(void)
{
	if (g_edit.EditType == EDITTYPE_OBJECT)
	{
		//	指定中のオブジェクトのメッシュを手に入れる
		g_edit.xData.pMesh = ObjectMeshGet((OBJECTTYPE)g_edit.EditModeType.nObjectType);
		g_edit.xData.pBuffMat = ObjectBuffmatGet((OBJECTTYPE)g_edit.EditModeType.nObjectType);
		g_edit.xData.dwNumMat = ObjectdwNummatGet((OBJECTTYPE)g_edit.EditModeType.nObjectType);
	}
}

//==================================================
//インデックス設定の処理
//==================================================
void SetEditIndex(int nIdx)
{
	Object *pObject = ObjectGet();
	Wall *pWall = GetWall();

	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		pObject[nIdx].nIdxEdit = nIdx;
		break;
	case EDITTYPE_WALL:
		pWall[nIdx].nIdx = nIdx;
		break;
	}

	g_nSelectIdx = nIdx;
}

//==================================================
//現在選択中のインデックスの取得処理
//==================================================
int GetSelectIdxEdit(void)
{
	if (g_edit.nState == EDITMODE_DELETE)
	{
		return g_nSelectIdx;
	}

	return -1;
}

//==================================================
//外部ファイルから位置情報を読み取る処理
//==================================================
void LoadEdit(void)
{
	D3DXVECTOR3 Objectpos;			//座標
	D3DXVECTOR3 Objectrot;			//方向
	float fWidth, fHeight = 0.0f;	//幅、高さ
	int nType = 0;					//種類
	int nCntMax = 0;			//セット個数
	char cTemp[128] = {};		//ゴミ箱
	char aSetTarget[128] = {};	//セットする対象の種類

	FILE *pFile;

	switch (GetMode())
	{
	case MODE_TITLE:
	case MODE_MENU:
		pFile = fopen(MAP_TITLE, "r");
		break;

	case MODE_TUTORIAL:
		pFile = fopen(MAP_TUTORIAL, "r");
		break;

	case MODE_RANKING:
		pFile = fopen(MAP_RANKING, "r");
		break;
	
	default:
		pFile = fopen(MAP_FILE, "r");
		break;
	}

	if (pFile != NULL)
	{
		while (fscanf(pFile, "%s", &cTemp[0]) != EOF)
		{
			//オブジェクト設定===========================================
			if (strcmp(cTemp, "OBJECTSET") == 0)
			{
				while (1)
				{//終わりまでオブジェクト設定
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "TYPE") == 0)
					{//タイプ読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// タイプ読み込み
						fscanf(pFile, "%d", &nType);
					}

					if (strcmp(cTemp, "POS") == 0)
					{//位置読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 位置読み込み
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// 向き読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 向き読み込み
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "END_OBJECTSET") == 0)
					{// for文を抜ける
					 //オブジェクト設置
						SetObject(Objectpos, Objectrot, nType);

						break;
					}
				}//END_OBJ
			}
			// オブジェクト設定===========================================================

			// プレイヤー設定===========================================
			if (strcmp(cTemp, "PLAYERSET") == 0)
			{
				while (1)
				{//終わりまでオブジェクト設定
				 //文字列読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//位置読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 位置読み込み
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// 向き読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 向き読み込み
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "END_PLAYERSET") == 0)
					{// for文を抜ける
					 //プレイヤー設定
						SetPosPlayer(Objectpos, Objectrot);

						break;
					}
				}//END_PLAYER
			}
			// プレイヤー設定===========================================

			// 敵設定===========================================
			if (strcmp(cTemp, "ENEMYSET") == 0)
			{
				while (1)
				{//終わりまでオブジェクト設定
				 //文字列読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//位置読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 位置読み込み
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// 向き読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 向き読み込み
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "END_ENEMYSET") == 0)
					{// for文を抜ける
					 //敵設定
						SetPosEnemy(Objectpos, Objectrot);

						break;
					}
				}//END_ENEMY
			}
			// 敵設定===========================================

			// 壁設定===========================================
			if (strcmp(cTemp, "WALLSET") == 0)
			{
				while (1)
				{//終わりまでオブジェクト設定
				 //文字列読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//位置読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 位置読み込み
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// 向き読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 向き読み込み
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// サイズ読み込み
					 // "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// サイズ読み込み
						fscanf(pFile, "%f", &fHeight);
						fscanf(pFile, "%f", &fWidth);
					}

					if (strcmp(cTemp, "END_WALLSET") == 0)
					{// for文を抜ける
					 // 壁設定
						SetWall(Objectpos, Objectrot, fWidth, fHeight);

						break;
					}
				}//END_WALL
			}
			// 壁設定===========================================
		}//while閉じ
		fclose(pFile);
	}
}

//==================================================
//外部ファイルに位置情報を書き出す処理
//==================================================
void SaveEdit(void)
{
	Wall *pWall = GetWall();			//壁の情報
	Object *pObject = ObjectGet();		//オブジェクトの情報
	Player *pPlayer = GetPlayer();		//プレイヤー
	Enemy *pEnemy = GetEnemy();			//敵
	EDITSAVE SaveData[EDITTYPE_MAX][EDIT_MAX] = {};		//セーブ用データ

	int nCntMax[EDITTYPE_MAX] = {0};		//配置数
	char aTypename[EDITTYPE_MAX][128] = {"OBJECT","PLAYER" ,"ENEMY" ,"WALL" };		//対象の種類名代入用
	char aObjTypename[128] = {};		//オブジェクトのタイプ名代入用
	int aObjectTypeNum[OBJECTTYPE_MAX] = { 0 };		//オブジェクトのタイプ別個数代入用
	int nWallNum = 0;		//壁の個数代入用
	int aMaxCnter[EDITTYPE_MAX] = {MAX_OBJECT,1,1,MAX_WALL};		//for文回す用の各最大数

	for (int nCntSaveType = 0; nCntSaveType < SAVE_MAX; nCntSaveType++)
	{
		for (int nCntUse = 0; nCntUse < aMaxCnter[nCntSaveType]; nCntUse++)
		{
			switch (nCntSaveType)
			{
			case EDITTYPE_OBJECT:
				if (pObject[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = pObject[nCntUse].pos;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = pObject[nCntUse].rot;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].nType = pObject[nCntUse].nType;
					nCntMax[nCntSaveType]++;
				}
				break;

			case EDITTYPE_PLAYER:
				if (pPlayer[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = D3DXVECTOR3(0.0f,20.0f,300.0f);
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
					nCntMax[nCntSaveType] = 1;
				}
				break;

			case EDITTYPE_ENEMY:
				if (pEnemy[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = D3DXVECTOR3(0.0f, 20.0f, -300.0f);
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					nCntMax[nCntSaveType] = 1;
				}
				break;

			case EDITTYPE_WALL:
				if (pWall[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = pWall[nCntUse].pos;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = pWall[nCntUse].rot;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].fHeight = pWall[nCntUse].fHeight;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].fWidth = pWall[nCntUse].fWidth;
					nCntMax[nCntSaveType]++;
				}
				break;
			}
		}
	}

	FILE *pFile;

	pFile = fopen(MAP_FILE, "w");

	if (pFile != NULL)
	{
		//ヘッダーコメント読み
		fprintf(pFile, "#==============================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# 設定・設置ファイル[editdata.txt]\n");
		fprintf(pFile, "# Author:_KAI_TAKADA\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================\n");

		for (int nCntSaveType = 0; nCntSaveType < EDITTYPE_MAX; nCntSaveType++)
		{
			//個数書き
			fprintf(pFile, "%d\n\n", nCntMax[nCntSaveType]);

			//コメント用ネーム
			switch (nCntSaveType)
			{
			case EDITTYPE_PLAYER:
				strcpy(aObjTypename, "プレイヤー");
				break;

			case EDITTYPE_ENEMY:
				strcpy(aObjTypename, "敵");
				break;

			case EDITTYPE_WALL:
				strcpy(aObjTypename, "壁");
				break;
			}

			for (int nCntStr = 0; nCntMax[nCntSaveType] > 0; nCntStr++)
			{
				if (nCntSaveType == EDITTYPE_OBJECT)
				{
					switch (pObject[nCntStr].nType)
					{
					case OBJECTTYPE_CATAPULT:			strcpy(aObjTypename, "オブジェクト-岩発射台");	break;
					case OBJECTTYPE_REFLECTOR:		strcpy(aObjTypename, "オブジェクト-反射板");	break;
					case OBJECTTYPE_REFLECTOR_BASE:	strcpy(aObjTypename, "オブジェクト-反射板の台座");	break;
					case OBJECTTYPE_LEVER_BAR:		strcpy(aObjTypename, "オブジェクト-レバー");	break;
					case OBJECTTYPE_LEVER_BASE:		strcpy(aObjTypename, "オブジェクト-レバー支点");	break;
					case OBJECTTYPE_TORCH:			strcpy(aObjTypename, "オブジェクト-松明");	break;
					case OBJECTTYPE_BUTTON:			strcpy(aObjTypename, "オブジェクト-ボタン");	break;
					case OBJECTTYPE_SANDPOLE:		strcpy(aObjTypename, "オブジェクト-砂柱");	break;
					case OBJECTTYPE_SANDPOLE_BASE:	strcpy(aObjTypename, "オブジェクト-砂柱の台座");	break;
					case OBJECTTYPE_STONEPOLE:		strcpy(aObjTypename, "オブジェクト-石柱");	break;
					case OBJECTTYPE_DOOR:			strcpy(aObjTypename, "オブジェクト-ドア");	break;
					case OBJECTTYPE_BOOK:			strcpy(aObjTypename, "オブジェクト-本");	break;
					case OBJECTTYPE_ROCK_FRAG:		strcpy(aObjTypename, "オブジェクト-岩の破片");	break;
					case OBJECTTYPE_RUBBLE:			strcpy(aObjTypename, "オブジェクト-がれき");	break;
					case OBJECTTYPE_STATUE_0:		strcpy(aObjTypename, "オブジェクト-銅像0");	break;
					case OBJECTTYPE_DEBRIS:			strcpy(aObjTypename, "オブジェクト-残骸");	break;
					case OBJECTTYPE_SKULL:			strcpy(aObjTypename, "オブジェクト-頭蓋骨");	break;
					case OBJECTTYPE_SKULLMT:		strcpy(aObjTypename, "オブジェクト-頭蓋骨の山");	break;
					case OBJECTTYPE_STATUE_1:		strcpy(aObjTypename, "オブジェクト-銅像1");	break;
					case OBJECTTYPE_STATUE_3:		strcpy(aObjTypename, "オブジェクト-銅像3-ルネサンス");	break;
					default:						strcpy(aObjTypename, "新しく追加した何か");	break;
					}
				
					aObjectTypeNum[pObject[nCntStr].nType]++;
				}

				//コメント書き
				fprintf(pFile, "#------------------\n");
				fprintf(pFile, "#%s", aObjTypename);

				switch (nCntSaveType)
				{
				case EDITTYPE_OBJECT:
					fprintf(pFile, "%d\n", aObjectTypeNum[pObject[nCntStr].nType]);
					break;
				case EDITTYPE_WALL:
					fprintf(pFile, "%d\n", nWallNum);
					nWallNum++;
					break;

				default:
					fprintf(pFile, "\n");
					break;
				}

				fprintf(pFile, "#------------------\n");
				fprintf(pFile, "%sSET\n", aTypename[nCntSaveType]);		//種類書き

				//タイプ書き
				if (nCntSaveType == EDITTYPE_OBJECT)
				{
					fprintf(pFile, "	TYPE ");
					fprintf(pFile, "= ");
					fprintf(pFile, "%d\n", SaveData[nCntSaveType][nCntStr].nType);
				}

				//ポジション書き
				fprintf(pFile, "	POS ");
				fprintf(pFile, "= ");
				fprintf(pFile, "%.2f ", SaveData[nCntSaveType][nCntStr].pos.x);
				fprintf(pFile, "%.2f ", SaveData[nCntSaveType][nCntStr].pos.y);
				fprintf(pFile, "%.2f\n", SaveData[nCntSaveType][nCntStr].pos.z);

				//ローテート書き
				fprintf(pFile, "	ROT ");
				fprintf(pFile, "= ");
				fprintf(pFile, "%.4f ", SaveData[nCntSaveType][nCntStr].rot.x);
				fprintf(pFile, "%.4f ", SaveData[nCntSaveType][nCntStr].rot.y);
				fprintf(pFile, "%.4f\n", SaveData[nCntSaveType][nCntStr].rot.z);

				if (nCntSaveType == EDITTYPE_WALL)
				{
					fprintf(pFile, "	SIZE ");
					fprintf(pFile, "= ");
					fprintf(pFile, "%.4f ", SaveData[nCntSaveType][nCntStr].fHeight);
					fprintf(pFile, "%.4f\n", SaveData[nCntSaveType][nCntStr].fHeight);
				}
				//エンド書き
				fprintf(pFile, "END_%sSET\n\n", aTypename[nCntSaveType]);

				nCntMax[nCntSaveType]--;
			}
		}

		fclose(pFile);
	}
}

//==================================================
//エディットのデバッグ表示
//==================================================
void DebugEdit(void)
{
	LPD3DXFONT Font = GetFont();
	RECT rect = { 0,SCREEN_HEIGHT / 2,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[DEBUG_NUM][256];
	int nNumUseEdit = 0;	//使用エディット数
	char sEditMode[12] = "設置モード";		//エディットのモード
	char sObjectType[16] = "花";		//エディットのオブジェクトの種類
	char sModelType[16] = "オブジェクト";		//エディットのモデルの種類

	switch (g_edit.nState)
	{
	case EDITMODE_SET:
		strcpy(sEditMode,"設置モード");
		break;
	case EDITMODE_DELETE:
		strcpy(sEditMode, "削除モード");
		break;
	}

	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		strcpy(sModelType, "オブジェクト");
		nNumUseEdit = GetUseObject();		//使用数の取得
		break;
	case EDITTYPE_PLAYER:
		strcpy(sModelType, "プレイヤ―");
		break;
	case EDITTYPE_ENEMY:
		strcpy(sModelType, "敵");
		break;
	case EDITTYPE_WALL:
		strcpy(sModelType, "壁");
		nNumUseEdit = GetUseWall();		//使用数の取得
		break;
	}

	switch (g_edit.EditModeType.nObjectType)
	{
	case OBJECTTYPE_CATAPULT:			strcpy(sObjectType, "ブロック");	break;
	case OBJECTTYPE_REFLECTOR:		strcpy(sObjectType, "反射板");	break;
	case OBJECTTYPE_LEVER_BAR:		strcpy(sObjectType, "レバー");	break;
	case OBJECTTYPE_LEVER_BASE:		strcpy(sObjectType, "レバーの支点");	break;
	case OBJECTTYPE_TORCH:			strcpy(sObjectType, "松明");	break;
	case OBJECTTYPE_BUTTON:			strcpy(sObjectType, "ボタン");	break;
	case OBJECTTYPE_SANDPOLE:		strcpy(sObjectType, "砂柱");	break;
	case OBJECTTYPE_SANDPOLE_BASE:	strcpy(sObjectType, "砂柱の台座");	break;
	case OBJECTTYPE_STONEPOLE:		strcpy(sObjectType, "石柱");	break;
	case OBJECTTYPE_DOOR:			strcpy(sObjectType, "ドア");	break;
	case OBJECTTYPE_BOOK:			strcpy(sObjectType, "本");	break;
	case OBJECTTYPE_ROCK_FRAG:		strcpy(sObjectType, "岩の破片");	break;
	case OBJECTTYPE_RUBBLE:			strcpy(sObjectType, "がれき");	break;
	case OBJECTTYPE_STATUE_0:		strcpy(sObjectType, "銅像0");	break;
	case OBJECTTYPE_STATUE_1:		strcpy(sObjectType, "銅像1");	break;
	case OBJECTTYPE_DEBRIS:			strcpy(sObjectType, "残骸");	break;
	case OBJECTTYPE_SKULL:			strcpy(sObjectType, "頭蓋骨");	break;
	case OBJECTTYPE_SKULLMT:		strcpy(sObjectType, "頭蓋骨の山");	break;
	default:						strcpy(sObjectType, "デフォルト");	break;
	}

	//文字列に代入
	sprintf(&aStr[0][0], "現在のモード<F5>：%s", sEditMode);
	sprintf(&aStr[1][0], "選択中のモデルの種類<F2>：%s", sModelType);
	sprintf(&aStr[2][0], "オブジェクトの種類<F3><F4>：%s", sObjectType);
	sprintf(&aStr[3][0], "使用インデックス数：%d", nNumUseEdit);
	sprintf(&aStr[4][0], "選択インデックス<K/L>：%d番目", g_nSelectIdx);
	sprintf(&aStr[5][0], "座標<WASD><T/B>：[%.3f、%.3f、%.3f]", g_edit.pos.x, g_edit.pos.y, g_edit.pos.z);
	sprintf(&aStr[6][0], "方向<Z/C><V><X>：[%.3f、%.3f、%.3f]", g_edit.rot.x, g_edit.rot.y, g_edit.rot.z);
	sprintf(&aStr[7][0], "壁の高さ<↑/↓>：%.3f、幅<←/→>：%.3f", g_edit.fHeight, g_edit.fWidth);
	sprintf(&aStr[8][0], "設置<ENTER> || セーブ<F9>");
	sprintf(&aStr[9][0], "全削除<F6>");
	sprintf(&aStr[10][0], "加速<LSHIFT>");

	//テキスト描画
	for (int nCntDraw = 0; nCntDraw < DEBUG_NUM; nCntDraw++)
	{
		Font->DrawText(NULL, &aStr[nCntDraw][0], -1, &rect, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		rect.top += 20;
	}
}
