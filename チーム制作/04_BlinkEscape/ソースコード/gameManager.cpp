//============================================================
//
//	ゲームマネージャー処理 [gameManager.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "gameManager.h"
#include "manager.h"
#include "sceneGame.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "score.h"
#include "timerManager.h"
#include "objectGauge2D.h"
#include "score.h"
#include "texture.h"

//==========================================
//  静的メンバ関数宣言
//==========================================
CPlayer* CGameManager::m_pPlayer = nullptr;
CMap* CGameManager::m_pMap = nullptr;
CObject2D* CGameManager::m_pObject2D[2] = { nullptr , nullptr };

CGameManager::STATE CGameManager::m_state = STATE_START;

//************************************************************
//	親クラス [CGameManager] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CGameManager::CGameManager()
{
	m_curtainInterbal = 0;
}

//============================================================
//	デストラクタ
//============================================================
CGameManager::~CGameManager()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CGameManager::Init(void)
{
	//世界の生成
	if (m_pMap == nullptr)
	{
		m_pMap = new CMap;
		m_pMap->Init();
	}

	//プレイヤーの生成
	//CEnemy::Create(D3DXVECTOR3(640.0f, 100.0f, 0.0f), D3DXVECTOR3(100.0f, 10.0f, 0.0f), VEC3_ZERO, XCOL_WHITE, CEnemy::TYPE_STICK_SLIDE);
	m_pPlayer = CPlayer::Create(m_pMap->GetHeightMin(), D3DXVECTOR3(100.0f, 100.0f, 0.0f));

	m_pObject2D[0] = CObject2D::Create(D3DXVECTOR3(320.0f, 360.0f, 0.0f), D3DXVECTOR3(640.0f, 720.0f, 0.0f));
	m_pObject2D[1] = CObject2D::Create(D3DXVECTOR3(960.0f, 360.0f, 0.0f), D3DXVECTOR3(640.0f, 720.0f, 0.0f));

	m_pObject2D[0]->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\fade003.jpg"));
	m_pObject2D[1]->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\fade003.jpg"));

	m_state = STATE_START;

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CGameManager::Uninit(void)
{
	//世界の破壊
	if (m_pMap != nullptr)
	{
		m_pMap->Uninit();
		delete m_pMap;
		m_pMap = nullptr;
	}
}

//============================================================
//	更新処理
//============================================================
void CGameManager::Update(void)
{
	if (m_state == STATE_START)
	{
		m_curtainInterbal++;

		if (m_curtainInterbal > 30)
		{
			D3DXVECTOR3 scale;
			D3DXVECTOR3 pos;

			pos = m_pObject2D[0]->GetPosition();
			scale = m_pObject2D[0]->GetScaling();
			pos.x -= 4.0f;
			scale.x -= 8.0f;
			m_pObject2D[0]->SetPosition(pos);
			m_pObject2D[0]->SetScaling(scale);

			pos = m_pObject2D[1]->GetPosition();
			scale = m_pObject2D[1]->GetScaling();
			pos.x += 4.0f;
			scale.x -= 8.0f;
			m_pObject2D[1]->SetPosition(pos);
			m_pObject2D[1]->SetScaling(scale);

			if (m_pObject2D[0]->GetPosition().x < 0.0f)
			{
				CSceneGame::GetTimerManager()->Start();	// 計測を開始
				m_state = STATE_NORMAL;
				m_pObject2D[0]->Uninit();
				m_pObject2D[1]->Uninit();
			}
		}
	}
	else
	{
		if (m_pMap != nullptr)
		{
			m_pMap->Update();
		}

		if (CSceneGame::GetTimerManager()->GetState() == CTimerManager::STATE_END)
		{ // タイマーの計測が終了済みの場合

			// リザルトに遷移
			TransitionResult(CRetentionManager::RESULT_CLEAR);
		}
		else if (CSceneGame::GetGameManager()->GetPlayer()->GetMiss())
		{ // プレイヤーが死亡した場合

			// リザルトに遷移する
			TransitionResult(CRetentionManager::RESULT_FAILED);
		}
	}
}

//============================================================
//	リザルト遷移処理
//============================================================
void CGameManager::TransitionResult(const CRetentionManager::RESULT result)
{
	// ポインタを宣言
	CRetentionManager *pRetention = CManager::GetRetentionManager();	// データ保存情報

	// クリア状況を設定
	pRetention->SetResult(result);

	// 獲得スコアを設定
	pRetention->SetScore(CSceneGame::GetScore()->Get());

	// リザルトに遷移
	CManager::SetScene(CScene::MODE_RESULT, 30);
}

//============================================================
//	生成処理
//============================================================
CGameManager *CGameManager::Create(void)
{
	// ポインタを宣言
	CGameManager *pGameManager = NULL;	// ゲームマネージャー生成用

	if (pGameManager == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pGameManager = new CGameManager;	// ゲームマネージャー
	}
	else { assert(false); return NULL; }	// 使用中

	if (pGameManager != NULL)
	{ // 使用されている場合
		
		// ゲームマネージャーの初期化
		if (FAILED(pGameManager->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pGameManager;
			pGameManager = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pGameManager;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	破棄処理
//============================================================
HRESULT CGameManager::Release(CGameManager *&prGameManager)
{
	if (prGameManager != NULL)
	{ // 使用中の場合

		// ゲームマネージャーの終了
		prGameManager->Uninit();

		// メモリ開放
		delete prGameManager;
		prGameManager = NULL;

		// 成功を返す
		return S_OK;
	}
	else { assert(false); return E_FAIL; }	// 非使用中
}

