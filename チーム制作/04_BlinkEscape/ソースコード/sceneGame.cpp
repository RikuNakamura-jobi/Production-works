//============================================================
//
//	ゲーム画面処理 [sceneGame.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "sceneGame.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "gameManager.h"
#include "timerManager.h"
#include "stage.h"
#include "pause.h"
#include "score.h"
#include "bg.h"

//************************************************************
//	マクロ定義
//************************************************************
#define TIME_LIMIT		(60)										// 制限時間
#define TIME_POS		(D3DXVECTOR3(40.0f, 50.0f, 0.0f))			// タイマー位置
#define TIME_VAL_SIZE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))			// タイマー数字大きさ
#define TIME_PART_SIZE	(D3DXVECTOR3(35.0f, 80.0f, 0.0f))			// タイマー区切り大きさ
#define TIME_VAL_SPACE	(D3DXVECTOR3(TIME_VAL_SIZE.x, 0.0f, 0.0f))	// タイマー数字空白
#define TIME_PART_SPACE	(D3DXVECTOR3(TIME_PART_SIZE.x, 0.0f, 0.0f))	// タイマー区切り空白

#define SCO_POS			(D3DXVECTOR3(745.0f, 50.0f, 0.0f))			// スコア位置
#define SCO_SIZE_VALUE	(D3DXVECTOR3(60.0f, 80.0f, 0.0f))			// 数字のスコア大きさ
#define SCO_SIZE_UNIT	(D3DXVECTOR3(80.0f, 80.0f, 0.0f))			// 単位のスコア大きさ
#define SCO_SPACE_VALUE	(D3DXVECTOR3(SCO_SIZE_VALUE.x, 0.0f, 0.0f))	// 数字のスコア空白
#define SCO_SPACE_UNIT	(D3DXVECTOR3(485.0f, 0.0f, 0.0f))			// 単位のスコア空白

#define WARNING_SIZE	(D3DXVECTOR3(1000.0f, 500.0f, 0.0f))	// 警告大きさ

//************************************************************
//	静的メンバ変数宣言
//************************************************************
CGameManager	*CSceneGame::m_pGameManager  = NULL;	// ゲームマネージャー
CTimerManager	*CSceneGame::m_pTimerManager = NULL;	// タイマーマネージャー
CPause	*CSceneGame::m_pPause	= NULL;					// ポーズ
CScore	*CSceneGame::m_pScore	= NULL;					// スコアオブジェクト
CBg		*CSceneGame::m_pBg		= NULL;					// 背景オブジェクト

bool CSceneGame::m_bDrawUI = true;		// UIの描画状況
bool CSceneGame::m_bDrawPause = true;	// ポーズの描画状況

//************************************************************
//	子クラス [CSceneGame] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CSceneGame::CSceneGame(const MODE mode) : CScene(mode)
{

}

//============================================================
//	デストラクタ
//============================================================
CSceneGame::~CSceneGame()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CSceneGame::Init(void)
{
	// タイマーマネージャーの生成
	m_pTimerManager = CTimerManager::Create
	( // 引数
		CTimerManager::TIME_SEC,	// 設定タイム
		TIME_LIMIT,					// 制限時間
		TIME_POS,					// 位置
		TIME_VAL_SIZE,				// 数字の大きさ
		TIME_PART_SIZE,				// 区切りの大きさ
		TIME_VAL_SPACE,				// 数字の空白
		TIME_PART_SPACE				// 区切りの空白
	);
	if (m_pTimerManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// スコアオブジェクトの生成
	m_pScore = CScore::Create
	( // 引数
		SCO_POS,			// 位置
		SCO_SIZE_VALUE,		// 数字の大きさ
		SCO_SIZE_UNIT,		// 単位の大きさ
		SCO_SPACE_VALUE,	// 数字の空白
		SCO_SPACE_UNIT		// 単位の空白
	);
	if (m_pScore == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// シーンの初期化
	CScene::Init();		// ステージ・地面・プレイヤーの生成

	// ゲームマネージャーの生成
	m_pGameManager = CGameManager::Create();
	if (m_pGameManager == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの生成
	m_pPause = CPause::Create();
	if (m_pPause == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 背景の生成
	m_pBg = CBg::Create(CBg::TEXTURE_BG, SCREEN_CENT, SCREEN_SIZE);
	if (m_pBg == NULL)
	{ // 非使用中の場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	//--------------------------------------------------------
	//	初期設定
	//--------------------------------------------------------
	// UIの描画状況を設定
	SetEnableDrawUI(m_bDrawUI);

	// ポーズの描画状況を設定
	SetEnableDrawPause(m_bDrawPause);

	// BGMの再生
	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
HRESULT CSceneGame::Uninit(void)
{
	// ゲームマネージャーの破棄
	if (FAILED(CGameManager::Release(m_pGameManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// タイマーマネージャーの破棄
	if (FAILED(CTimerManager::Release(m_pTimerManager)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// ポーズの破棄
	if (FAILED(CPause::Release(m_pPause)))
	{ // 破棄に失敗した場合

		// 失敗を返す
		assert(false);
		return E_FAIL;
	}

	// 終了済みのオブジェクトポインタをNULLにする
	m_pScore = NULL;	// スコアオブジェクト
	m_pBg = NULL;		// 背景オブジェクト

	// シーンの終了
	CScene::Uninit();

	// 成功を返す
	return S_OK;
}

//============================================================
//	更新処理
//============================================================
void CSceneGame::Update(void)
{
#if _DEBUG

	if (CManager::GetKeyboard()->GetTrigger(DIK_F2))
	{
		// UIの描画状況を反転
		SetEnableDrawUI((!m_bDrawUI) ? true : false);
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F3))
	{
		// ポーズの描画状況を反転
		SetEnableDrawPause((!m_bDrawPause) ? true : false);
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_F4))
	{
		// リザルトに遷移
		CManager::SetScene(CScene::MODE_RESULT);	// リザルト画面
	}

	// デバッグ表示
	CManager::GetDebugProc()->Print("[F2]：UI描画のON/OFF\n");
	CManager::GetDebugProc()->Print("[F3]：ポーズ描画のON/OFF\n");
	CManager::GetDebugProc()->Print("[F4]：リザルト遷移\n");

#endif

	if (m_pTimerManager != NULL)
	{ // 使用中の場合

		// タイマーマネージャーの更新
		m_pTimerManager->Update();
	}
	else { assert(false); }	// 非使用中

	if (m_pPause != NULL)
	{ // 使用中の場合

		// ポーズの更新
		m_pPause->Update();
	}
	else { assert(false); }	// 非使用中

	if (!m_pPause->IsPause())
	{ // ポーズ中ではない場合

		if (m_pGameManager != NULL)
		{ // 使用中の場合

			// ゲームマネージャーの更新
			m_pGameManager->Update();
		}
		else { assert(false); }	// 非使用中

		// シーンの更新
		CScene::Update();
	}
}

//============================================================
//	描画処理
//============================================================
void CSceneGame::Draw(void)
{

}

//============================================================
//	ゲームマネージャー取得処理
//============================================================
CGameManager *CSceneGame::GetGameManager(void)
{
	// ゲームマネージャーのポインタを返す
	return m_pGameManager;
}

//============================================================
//	タイマーマネージャー取得処理
//============================================================
CTimerManager *CSceneGame::GetTimerManager(void)
{
	// タイマーマネージャーのポインタを返す
	return m_pTimerManager;
}

//============================================================
//	ポーズ取得処理
//============================================================
CPause *CSceneGame::GetPause(void)
{
	// ポーズのポインタを返す
	return m_pPause;
}

//============================================================
//	スコア取得処理
//============================================================
CScore *CSceneGame::GetScore(void)
{
	// スコアのポインタを返す
	return m_pScore;
}

//============================================================
//	背景取得処理
//============================================================
CBg *CSceneGame::GetBg(void)
{
	// 背景のポインタを返す
	return m_pBg;
}

//============================================================
//	UIの描画状況の設定処理
//============================================================
void CSceneGame::SetEnableDrawUI(const bool bDraw)
{
	// 引数のUIの描画状況を設定
	m_bDrawUI = bDraw;

	// タイマーの描画状況を設定
	m_pTimerManager->SetEnableDraw(bDraw);

	// スコアの描画状況を設定
	m_pScore->SetEnableDraw(bDraw);
}

//============================================================
//	ポーズの描画状況の設定処理
//============================================================
void CSceneGame::SetEnableDrawPause(const bool bDraw)
{
	// 引数のポーズの描画状況を設定
	m_bDrawPause = bDraw;

	// ポーズの描画状況を設定
	m_pPause->SetEnableDraw(m_pPause->IsPause());
}

//============================================================
//	UIの描画状況取得処理
//============================================================
bool CSceneGame::IsDrawUI(void)
{
	// UIの描画状況を返す
	return m_bDrawUI;
}

//============================================================
//	ポーズの描画状況取得処理
//============================================================
bool CSceneGame::IsDrawPause(void)
{
	// ポーズの描画状況を返す
	return m_bDrawPause;
}
