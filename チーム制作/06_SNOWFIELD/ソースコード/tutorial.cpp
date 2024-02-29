//============================================
//
// チュートリアル画面のメイン処理[tutorial.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "tutorial.h"
#include "fade.h"
#include "input.h"
#include "renderer.h"

#include "sound.h"
#include "tutorial_screen.h"

// マクロ定義
#define TRANS_COUNT				(50)									// 遷移までのカウント数

//=========================================
// コンストラクタ
//=========================================
CTutorial::CTutorial() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての情報をクリアする
	m_nEndCount = 0;			// 終了までのカウント
	m_bEnd = false;				// 終了状況
}

//=========================================
// デストラクタ
//=========================================
CTutorial::~CTutorial()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CTutorial::Init(void)
{
	// シーンの初期化
	CScene::Init();

	// チュートリアルを生成する
	CTutorialScreen::Create();

	// 全ての値をクリアする
	m_nEndCount = 0;			// 終了までのカウント
	m_bEnd = false;				// 終了状況

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CTutorial::Uninit(void)
{
	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CTutorial::Update(void)
{
	if (CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{ // 決定を押した場合

		// 選択音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

		// true にする
		m_bEnd = true;
	}

	if (m_bEnd == true)
	{ // 終了状況が true の場合

		// 終了カウントを加算する
		m_nEndCount++;

		if (m_nEndCount >= TRANS_COUNT)
		{ // スキップ時または、終了時の場合

			// ゲームモードにする
			CManager::Get()->GetFade()->SetFade(MODE_GAME);
		}

		if (CManager::Get()->GetRenderer() != nullptr)
		{ // レンダラーが NULL じゃない場合

			// 更新処理
			CManager::Get()->GetRenderer()->Update();
		}
	}
}

//======================================
//描画処理
//======================================
void CTutorial::Draw(void)
{

}

//======================================
// 情報の設定処理
//======================================
void CTutorial::SetData(const MODE mode)
{
	// 情報の設定処理
	CScene::SetData(mode);

	// 全ての値を設定する
	m_nEndCount = 0;			// 終了までのカウント
	m_bEnd = false;				// 終了状況
}