//=============================================================================
//
// 
// Author :中村陸
//
//=============================================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
int CTexture::m_nNumAll = 0;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CTexture::CTexture()
{
	memset(m_apTexture, 0, sizeof(m_apTexture));
}

CTexture::~CTexture()
{
}

//===========================
//テクスチャの初期化処理
//===========================
HRESULT CTexture::Load(void)
{


	return S_OK;
}

//===========================
//テクスチャの終了処理
//===========================
void CTexture::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (m_apTexture[nCnt] != NULL)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = NULL;
		}
	}
}

//===========================
//テクスチャの終了処理
//===========================
int CTexture::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (m_apTexture[nCnt] == NULL)
		{
			if (FAILED(D3DXCreateTextureFromFile(pDevice,
				pFilename,
				&m_apTexture[nCnt])))
			{
				return -1;
			}

			return nCnt;
		}
	}

	return -1;
}

//===========================
//テクスチャの終了処理
//===========================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	if (nIdx == -1)
	{
		return NULL;
	}

	return m_apTexture[nIdx];
}