//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectX.h"
#include "objectbillboard.h"
#include "player.h"
#include "bg.h"
#include "bgmulti.h"
#include "input.h"
#include "number.h"
#include "score.h"
#include "effect.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "debugproc.h"
#include "texture.h"
#include "fade.h"
#include "field.h"
#include "sky.h"
#include "block.h"
#include "deliverypoint.h"
#include "deliveryarrow.h"
#include "collision.h"
#include "chain.h"
#include "car.h"
#include "drone.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
CManager *CManager::pManager = NULL;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CManager::CManager()
{
	m_nCountFPS = 0;
}

CManager::~CManager()
{
}

//=====================================
// 初期化処理
//=====================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダラーの生成
	if (m_pRenderer != NULL)
	{
		//終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;

		m_pRenderer = new CRenderer;
	}
	else
	{
		m_pRenderer = new CRenderer;
	}

	//初期化処理
	if (m_pRenderer != NULL)
	{
		if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	//インプットの生成(キーボード)
	if (m_pInputKeyboard != NULL)
	{
		//終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;

		m_pInputKeyboard = new CInputKeyboard;
	}
	else
	{
		m_pInputKeyboard = new CInputKeyboard;
	}

	//初期化処理
	if (m_pInputKeyboard != NULL)
	{
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	//インプットの生成(パッド)
	if (m_pInputPad != NULL)
	{
		//終了処理
		m_pInputPad->Uninit();

		delete m_pInputPad;
		m_pInputPad = NULL;

		m_pInputPad = new CInputPad;
	}
	else
	{
		m_pInputPad = new CInputPad;
	}

	//初期化処理
	if (m_pInputPad != NULL)
	{
		if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			delete m_pInputPad; 
			m_pInputPad = NULL;
		}
	}
	else
	{
		return E_FAIL;
	}

	//インプットの生成(マウス)
	if (m_pInputMouse != NULL)
	{
		//終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;

		m_pInputMouse = new CInputMouse;
	}
	else
	{
		m_pInputMouse = new CInputMouse;
	}

	//初期化処理
	if (m_pInputMouse != NULL)
	{
		if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合
			delete m_pInputMouse;
			m_pInputMouse = NULL;
		}
	}
	else
	{
		return E_FAIL;
	}

	//サウンドの生成
	if (m_pSound != NULL)
	{
		//終了処理
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;

		m_pSound = new CSound;
	}
	else
	{
		m_pSound = new CSound;
	}

	//初期化処理
	if (m_pSound != NULL)
	{
		if (FAILED(m_pSound->Init(hWnd)))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	//ライトの生成
	if (m_pLight != NULL)
	{
		//終了処理
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;

		m_pLight = new CLight;
	}
	else
	{
		m_pLight = new CLight;
	}

	//初期化処理
	if (m_pLight != NULL)
	{
		if (FAILED(m_pLight->Init()))
		{//初期化処理が失敗した場合
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	//デバッグプロックの生成
	if (m_pDebugProc != NULL)
	{
		//終了処理
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = NULL;

		m_pDebugProc = new CDebugProc;
	}
	else
	{
		m_pDebugProc = new CDebugProc;
	}

	//初期化処理
	if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Init();
	}
	else
	{
		return E_FAIL;
	}

	//テクスチャの生成
	if (m_pTexture != NULL)
	{
		//終了処理
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;

		m_pTexture = new CTexture;
	}
	else
	{
		m_pTexture = new CTexture;
	}

	//初期化処理
	if (m_pTexture != NULL)
	{
		m_pTexture->Load();
	}
	else
	{
		return E_FAIL;
	}

	m_pCollision = CCollision::Get();

	//各オブジェクトのテクスチャの生成
	CBg::Load();
	CBgMulti::Load();
	CEffect::Load();
	CNumber::Load();
	CFade::Load();
	CField::Load();
	CSky::Load();
	CPlayer::Load();
	CBlock::Load();
	CHook::Load();
	CDeliverypoint::Load();
	CDeliveryarrow::Load();
	CCar::Load();
	CDrone::Load();

	if (m_pScene != NULL)
	{
		//終了処理
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = NULL;

		m_pScene = CScene::Create(CScene::MODE_TITLE);
	}
	else
	{
		m_pScene = CScene::Create(CScene::MODE_TITLE);
	}

	if (m_pScene != NULL)
	{
		m_pScene->Init();
	}

	return S_OK;
}

//=====================================
// 終了処理
//=====================================
void CManager::Uninit(void)
{
	//各オブジェクトのテクスチャの破棄
	CBg::Unload();
	CBgMulti::Unload();
	CEffect::Unload();
	CNumber::Unload();
	CFade::Unload();
	CField::Unload();
	CSky::Unload();
	CPlayer::Unload();
	CBlock::Unload();
	CHook::Unload();
	CDeliverypoint::Unload();
	CDeliveryarrow::Unload();
	CCar::Unload();
	CDrone::Unload();

	CObject::ReleaseAll();

	if (m_pInputKeyboard != NULL)
	{
		//終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	if (m_pInputPad != NULL)
	{
		//終了処理
		m_pInputPad->Uninit();

		delete m_pInputPad;
		m_pInputPad = NULL;
	}

	if (m_pInputMouse != NULL)
	{
		//終了処理
		m_pInputMouse->Uninit();

		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	if (m_pRenderer != NULL)
	{
		//終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	if (m_pLight != NULL)
	{
		//終了処理
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pSound != NULL)
	{
		//終了処理
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	if (m_pDebugProc != NULL)
	{
		//終了処理
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	if (m_pScene != NULL)
	{
		//終了処理
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = NULL;
	}

	if (m_pTexture != NULL)
	{
		//終了処理
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = NULL;
	}

	if (m_pCollision != NULL)
	{
		//終了処理
		m_pCollision->Release();
		m_pCollision = NULL;
	}
}

//=====================================
// 更新処理
//=====================================
void CManager::Update(void)
{
	m_pDebugProc->Print("FPS: %d\n", m_nCountFPS);

	//更新処理
	m_pInputMouse->Update();
	m_pInputKeyboard->Update();
	if (m_pInputPad != NULL)
	{
		m_pInputPad->Update();
	}
	m_pRenderer->Update();
	m_pLight->Update();
	m_pDebugProc->Update();

	if (m_pScene != NULL)
	{
		//更新処理
		m_pScene->Update();
	}

	m_pCollision->Update();
}

//=====================================
// 描画処理
//=====================================
void CManager::Draw(void)
{
	//描画処理
	m_pRenderer->Draw();

	if (m_pScene != NULL)
	{
		//描画処理
		m_pScene->Draw();
	}
}

//=====================================
// マネージャーの破棄
//=====================================
HRESULT CManager::Release(void)
{
	if (pManager != NULL)
	{
		pManager->Uninit();

		delete pManager;
		pManager = NULL;
	}

	return S_OK;
}

//=====================================
// モードの設定
//=====================================
void CManager::SetMode(CScene::MODE mode)
{
	//現在の画面の終了処理
	if (m_pScene != NULL)
	{
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = NULL;
	}

	//新しい画面の初期化処理
	m_pScene = CScene::Create(mode);

	m_pScene->Init();
	m_pScene->SetMode(mode);
}

//=====================================
// マネージャーの取得
//=====================================
CManager *CManager::Get(void)
{
	if (pManager == NULL)
	{
		return pManager = new CManager;
	}
	else
	{
		return pManager;
	}
}