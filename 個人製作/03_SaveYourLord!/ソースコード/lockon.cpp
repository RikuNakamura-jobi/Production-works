//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "lockon.h"
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
LPDIRECT3DTEXTURE9 CLockon::m_pTexture = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CLockon::CLockon(int nPriority = 4) : CObjectBillboard(nPriority)
{
	m_bLock = false;
}

CLockon::~CLockon()
{
}

//=====================================
// 生成処理
//=====================================
CLockon *CLockon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight)
{
	CLockon *pObjectEffect;

	//2Dオブジェクトの生成
	pObjectEffect = new CLockon();

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
		pObjectEffect->SetHeightDef(fHeight);
		pObjectEffect->SetWidthDef(fWidth);
		
		pObjectEffect->BindTexture(m_pTexture);
	}

	return pObjectEffect;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CLockon::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//テクスチャの読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\lockon000.png",
		&m_pTexture)))
	{
		return E_FAIL;
	}

	return S_OK;
}
void CLockon::Unload(void)
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
HRESULT CLockon::Init(void)
{
	CObjectBillboard::Init();

	SetType(TYPE_LOCKON);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CLockon::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CLockon::Update(void)
{
	CInput *input = CManager::GetInputKeyboard();
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
void CLockon::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Zテストを無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	CObjectBillboard::Draw();

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CLockon::target(void)
{
	CObject *objTarget = NULL;
	D3DXVECTOR3 vecCamera = CManager::GetScene()->GetCamera()->GetPosRDest() - CManager::GetScene()->GetCamera()->GetPosVDest();
	D3DXVECTOR3 posV1 = CManager::GetScene()->GetCamera()->GetPosVDest(), posV2 = CManager::GetScene()->GetCamera()->GetPosRDest();
	D3DXVECTOR3 posTarget;
	int nTargetPriority = -1;
	float answerOuter;				//答え
	float answerLength;				//答え
	float fTargetOuter;
	float fTargetLength;
	bool bCenter = false;

	SetPos(CManager::GetScene()->GetCamera()->GetPosRDest());
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetHeight(15.0f);
	SetWidth(15.0f);
	m_bLock = false;

	for (int nCntPriority = 0; nCntPriority < ALL_PRIORITY; nCntPriority++)
	{
		CObject *obj = CObject::GetObjectTop(nCntPriority);

		while (obj != NULL)
		{
			CObject *pObjNext = obj->GetObjectNext();

			if (obj->GetType() == CObject::TYPE_ENEMY)
			{
				D3DXVECTOR3 vecTarget = obj->GetPos() - posV1;
				D3DXVECTOR3 vecCameraNor = D3DXVECTOR3(-vecCamera.z, 0.0f, vecCamera.x);
				D3DXVECTOR3 vecTemp;

				D3DXVec3Normalize(&vecCamera, &vecCamera);
				D3DXVec3Normalize(&vecTarget, &vecTarget);

				answerOuter = fabsf(D3DXVec3Cross(&vecTemp, &vecCamera, &vecTarget)->y);
				answerLength = D3DXVec3Length(&(obj->GetPos() - GetPos()));

				if (answerOuter < 0.7f && D3DXVec3Cross(&vecTemp, &vecCameraNor, &vecTarget)->y > 0.0f && answerLength < 2000.0f)
				{
					if (answerOuter < 0.3f)
					{
						if (nTargetPriority == -1)
						{
							nTargetPriority = nCntPriority;
							objTarget = obj;
							fTargetOuter = answerOuter;
							fTargetLength = answerLength;
							bCenter = true;
						}
						else
						{
							if (answerLength < fTargetLength || bCenter == false)
							{
								nTargetPriority = nCntPriority;
								objTarget = obj;
								fTargetOuter = answerOuter;
								fTargetLength = answerLength;
								bCenter = true;
							}
						}
					}
					else if(bCenter == false)
					{
						if (nTargetPriority == -1)
						{
							nTargetPriority = nCntPriority;
							objTarget = obj;
							fTargetOuter = answerOuter;
							fTargetLength = answerLength;
						}
						else
						{
							if (answerLength < fTargetLength)
							{
								nTargetPriority = nCntPriority;
								objTarget = obj;
								fTargetOuter = answerOuter;
								fTargetLength = answerLength;
							}
						}
					}
				}
			}

			obj = pObjNext;
		}
	}

	if (nTargetPriority != -1 && objTarget != NULL)
	{
			SetPos(D3DXVECTOR3(objTarget->GetPos().x, objTarget->GetPos().y + (m_fHeightDef * 1.0f), objTarget->GetPos().z));
			SetMove(objTarget->GetMove());
			SetHeight(m_fHeightDef);
			SetWidth(m_fWidthDef);
			SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			m_bLock = true;

			CManager::GetDebugProc()->Print("ロックオンのpos: %f, %f, %f\n", objTarget->GetPos().x, objTarget->GetPos().y, objTarget->GetPos().z);
	}
}