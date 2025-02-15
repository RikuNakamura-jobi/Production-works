//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "effect.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "bullet.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CEffect::CEffect(int nPriority = 4) : CObjectBillboard(nPriority)
{
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;
	m_nLife = 0;
	m_nLifeMax = 1;
}

CEffect::~CEffect()
{
}

//=====================================
// 生成処理
//=====================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight)
{
	CEffect *pObjectEffect;

	//2Dオブジェクトの生成
	pObjectEffect = new CEffect();

	if (pObjectEffect != NULL)
	{
		pObjectEffect->Set(pos, rot, fWidth, fHeight);

		//初期化
		if (FAILED(pObjectEffect->Init()))
		{
			pObjectEffect->Release();

			return NULL;
		}

		pObjectEffect->m_nLife = nLife;
		pObjectEffect->m_nLifeMax = nLife;
		pObjectEffect->SetMove(move);
		pObjectEffect->SetCol(col);

		pObjectEffect->BindTexture(m_pTexture);
	}

	return pObjectEffect;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CEffect::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CEffect::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CEffect::Init(void)
{
	CObjectBillboard::Init();

	SetType(TYPE_EFFECT);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CEffect::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CEffect::Update(void)
{
	CInput *input = CManager::GetInputKeyboard();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();

	pos += move;

	//移動量減衰
	move *= 0.96f;

	//サイズ縮小
	fHeight *= ((float)m_nLife / (float)m_nLifeMax);
	fWidth *= ((float)m_nLife / (float)m_nLifeMax);

	//寿命現象
	m_nLife--;

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);

	CObjectBillboard::Update();

	if (fHeight <= 0.3f || fWidth <= 0.3f || m_nLife <= 0)
	{
		//使用していない状態にする
		Uninit();
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Zテストを無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectBillboard::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}