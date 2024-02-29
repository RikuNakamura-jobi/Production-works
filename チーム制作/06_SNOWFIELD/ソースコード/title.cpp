//============================================
//
// タイトル画面のメイン処理[title.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "object.h"
#include "title.h"
#include "fade.h"
#include "Objectmesh.h"
#include "texture.h"

#include "skybox.h"
#include "screen.h"
#include "title_logo.h"
#include "sound.h"

//=========================================
// コンストラクタ
//=========================================
CTitle::CTitle() : CScene(TYPE_NONE, PRIORITY_BG)
{
	// 全ての値をクリアする
	m_nTransCount = 0;
}

//=========================================
// デストラクタ
//=========================================
CTitle::~CTitle()
{

}

//=========================================
//初期化処理
//=========================================
HRESULT CTitle::Init(void)
{
	//　シーンの初期化
	CScene::Init();

	// テキスト読み込み処理
	CMesh::TxtSet();

	// 2D画面を生成する
	CScreen::Create();

	// タイトルロゴを生成
	CTitleLogo::Create();

	// 全ての値を初期化する
	m_nTransCount = 0;

	// 成功を返す
	return S_OK;
}

//=============================================
//終了処理
//=============================================
void CTitle::Uninit(void)
{
	// 終了処理
	CScene::Uninit();
}

//======================================
//更新処理
//======================================
void CTitle::Update(void)
{
	// 遷移カウントを加算する
	m_nTransCount++;

	// レンダラーの更新
	CManager::Get()->GetRenderer()->Update();

	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_START, 0) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true)
	{ // ENTERキーを押した場合

		// 選択音を鳴らす
		CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SELECT);

		// チュートリアルに遷移する
		CManager::Get()->GetFade()->SetFade(CScene::MODE_TUTORIAL);

		// この先の処理を行わない
		return;
	}
}

//======================================
//描画処理
//======================================
void CTitle::Draw(void)
{

}