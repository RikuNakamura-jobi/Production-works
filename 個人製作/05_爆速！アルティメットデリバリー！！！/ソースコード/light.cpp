//=============================================================================
//
// 
// Author :中村陸
//
//=============================================================================
#include "light.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CLight::CLight()
{
	
}

CLight::~CLight()
{
}

//===========================
//ライトの初期化処理
//===========================
HRESULT CLight::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;		//設定用方向ベクトル

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		//ライトの種類を設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.95f, 0.95f, 0.95f, 0.95f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.5f, -0.37f, -0.5f);
		}
		else if (nCntLight == 1)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.45f, 0.25f, 0.65f, 0.75f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
		}
		else if (nCntLight == 2)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.85f, 0.75f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
		}
		else if (nCntLight == 3)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.95f, 0.95f, 0.95f, 0.95f);

			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.69f, -0.11f, 0.54f);
		}

		D3DXVec3Normalize(&vecDir, &vecDir);		//ベクトルを正規化する
		m_aLight[nCntLight].Direction = vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//===========================
//ライトの終了処理
//===========================
void CLight::Uninit(void)
{

}

//===========================
//ライトの更新処理
//===========================
void CLight::Update(void)
{

}