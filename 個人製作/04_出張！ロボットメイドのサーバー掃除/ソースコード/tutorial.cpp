//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "tutorial.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "object.h"
#include "debugproc.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CTutorialBillboard::m_pTexture[5] = {};
const char *CTutorialBillboard::m_apFilename[5] =
{
	"data\\TEXTURE\\tutorial001.png",
	"data\\TEXTURE\\tutorial002.png",
	"data\\TEXTURE\\tutorial003.png",
	"data\\TEXTURE\\tutorial004.png",
	"data\\TEXTURE\\tutorial005.png"
};

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CTutorialBillboard::CTutorialBillboard(int nPriority = 3) : CObjectBillboard(nPriority)
{
	
}

CTutorialBillboard::~CTutorialBillboard()
{
}

//=====================================
// 生成処理
//=====================================
CTutorialBillboard *CTutorialBillboard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight)
{
	CTutorialBillboard *pObjectEffect;

	//2Dオブジェクトの生成
	pObjectEffect = new CTutorialBillboard();

	if (pObjectEffect != NULL)
	{
		pObjectEffect->Set(pos, rot, fWidth, fHeight);

		//初期化
		if (FAILED(pObjectEffect->Init()))
		{
			pObjectEffect->Release();
		}

		pObjectEffect->SetMove(move);
		pObjectEffect->SetCol(col);

		pObjectEffect->BindTexture(m_pTexture[nLife]);
	}

	return pObjectEffect;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CTutorialBillboard::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//テクスチャの読み込み
		if (FAILED(D3DXCreateTextureFromFile(pDevice,
			m_apFilename[nCnt],
			&m_pTexture[nCnt])))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}
void CTutorialBillboard::Unload(void)
{
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CTutorialBillboard::Init(void)
{
	CObjectBillboard::Init();

	SetType(TYPE_LOCKON);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CTutorialBillboard::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CTutorialBillboard::Update(void)
{
	CInput *input = CManager::Get()->GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);

	//target();

	CObjectBillboard::Update();
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CTutorialBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	//Zテストを無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CObjectBillboard::Draw();

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}