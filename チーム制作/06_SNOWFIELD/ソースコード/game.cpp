//============================================
//
// ゲームのメイン処理[game.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "input.h"
#include "game.h"
#include "fade.h"
#include "file.h"
#include "renderer.h"

#include "pause.h"
#include "debugproc.h"
#include "sound.h"

#include "objectElevation.h"
#include "skybox.h"

#include "soldier.h"
#include "snowball.h"
#include "castle.h"
#include "flag.h"
#include "off_respawn.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
#define GAMECOUNT		(7200)		// ゲームのカウント

//--------------------------------------------
// 静的メンバ変数宣言
//--------------------------------------------
CFlag* CGame::m_pFlag = nullptr;							// 旗の情報
COffRespawn* CGame::m_pRespawn = nullptr;					// 攻撃側の復活地点の情報
CCastle* CGame::m_pCastle = nullptr;						// 城の情報
CGame::STATE CGame::m_GameState = CGame::STATE_START;		// ゲームの進行状態
int CGame::m_nFinishCount = 0;								// 終了カウント

//=========================================
// コンストラクタ
//=========================================
CGame::CGame() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての値をクリアする
	m_nFinishCount = 0;			// 終了カウント
	m_GameState = STATE_START;	// 状態
}

//=========================================
// デストラクタ
//=========================================
CGame::~CGame()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CGame::Init(void)
{
	// テキスト読み込み処理
	CElevation::TxtSet();

	// メッシュの読み込み処理
	CMesh::TxtSet();

	// メッシュのテキスト読み込み
	//CMesh::TxtSet();

	//if (m_pField == NULL)
	//{ // フィールドへのポインタが NULL の場合

	//	// フィールドの設定処理
	//	m_pField = CField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f));
	//}

	// スカイボックスの生成処理
	CSkyBox::Create();

	// シーンの初期化
	CScene::Init();

	// 兵士を生成する
	CSoldier::Create(D3DXVECTOR3(0.0f, 0.0f, -800.0f), CSoldier::TYPE::TYPE_PLAYER, CSoldier::BATTLE_OFF);

	for (int nCnt = 0; nCnt < 24; nCnt++)
	{

		CSoldier::Create(D3DXVECTOR3((float)(rand() % 1200 - 600), 0.0f, -800.0f), CSoldier::TYPE::TYPE_AI, CSoldier::BATTLE_OFF);
	}

	for (int nCnt = 0; nCnt < 25; nCnt++)
	{

		CSoldier::Create(D3DXVECTOR3((float)(rand() % 1200 - 600), 0.0f, 2000.0f), CSoldier::TYPE::TYPE_AI, CSoldier::BATTLE_DEF);
	}

	// 旗を生成する
	m_pFlag = CFlag::Create(D3DXVECTOR3(0.0f, 0.0f, 720.0f));

	// 城を生成する
	m_pCastle = CCastle::Create(D3DXVECTOR3(0.0f, 0.0f, 2500.0f),NONE_D3DXVECTOR3);

	// 攻撃側の復活地点を生成する
	m_pRespawn = COffRespawn::Create(D3DXVECTOR3(0.0f, 0.0f, -600.0f));

	// 情報の初期化
	m_nFinishCount = 0;				// 終了カウント
	m_GameState = STATE_START;		// 状態

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CGame::Uninit(void)
{
	// ポインタを NULL にする
	m_pFlag = nullptr;			// 旗

	// 終了カウントを初期化する
	m_nFinishCount = 0;

	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CGame::Update(void)
{
	switch (m_GameState)
	{
	case CGame::STATE_START:

		// 終了カウントを加算する
		m_nFinishCount++;

		break;

	case CGame::STATE_PLAY:

		// 終了カウントを加算する
		m_nFinishCount++;

		break;

	case CGame::STATE_TIMEUP:
	case CGame::STATE_OFFWIN:

		// 遷移処理
		Transition();

		break;

	default:

		// 停止
		assert(false);

		break;
	}

	if (CManager::Get()->GetRenderer() != nullptr)
	{ // レンダラーが NULL じゃない場合

		// レンダラーの更新
		CManager::Get()->GetRenderer()->Update();
	}

	if (m_nFinishCount >= GAMECOUNT)
	{ // ゲームカウントが一定数を超えた場合

		// タイムアップ状態にする
		m_GameState = STATE_TIMEUP;
	}

	CManager::Get()->GetDebugProc()->Print("状態：%d", m_GameState);
}

//======================================
//描画処理
//======================================
void CGame::Draw(void)
{

}

//======================================
// 情報の設定処理
//======================================
void CGame::SetData(const MODE mode)
{
	// 情報の設定処理
	CScene::SetData(mode);

	// スタート状態にする
	m_GameState = STATE_START;

	// 情報の初期化
	m_nFinishCount = 0;				// 終了カウント
}

//======================================
// 遷移処理
//======================================
void CGame::Transition(void)
{
	// リザルトに遷移する
	CManager::Get()->GetFade()->SetFade(CScene::MODE_RESULT);
}

//======================================
// ゲームの進行状態の設定処理
//======================================
void CGame::SetState(const STATE state)
{
	// ゲームの進行状態を設定する
	m_GameState = state;
}

//======================================
// ゲームの進行状態の取得処理
//======================================
CGame::STATE CGame::GetState(void)
{
	// ゲームの進行状態を返す
	return m_GameState;
}

//======================================
// 旗の取得処理
//======================================
CFlag *CGame::GetFlag(void)
{
	// 旗を返す
	return m_pFlag;
}

//======================================
// 復活位置の取得処理
//======================================
COffRespawn *CGame::GetRespawn(void)
{
	// 復活位置を返す
	return m_pRespawn;
}

//======================================
// 城の取得処理
//======================================
CCastle *CGame::GetCastle(void)
{
	// 城を返す
	return m_pCastle;
}