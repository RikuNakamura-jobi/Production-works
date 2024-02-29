//============================================
//
// 兵士プレイヤーのメイン処理[soldier_player.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "soldier_player.h"
#include "game.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "useful.h"

#include "input.h"
#include "motion.h"

namespace
{
	const float GRAVITY = 0.5f;				// 重力
	const float CAMERA_DISTANCE = -100.0f;	// カメラの距離
	const float CAMERA_CYCLE = 0.04f;		// カメラの回す速度
}

//=========================================
// コンストラクタ
//=========================================
CSoldierPlayer::CSoldierPlayer() : CSoldier(CObject::TYPE_SOLDIER, CObject::PRIORITY_PLAYER)
{
	// 全ての値をクリアする
	m_CameraRot = NONE_D3DXVECTOR3;		// カメラの向き
}

//=========================================
// デストラクタ
//=========================================
CSoldierPlayer::~CSoldierPlayer()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CSoldierPlayer::Init(void)
{
	if (FAILED(CSoldier::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// 全ての値を初期化する
	m_CameraRot = NONE_D3DXVECTOR3;		// カメラの向き

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CSoldierPlayer::Uninit(void)
{
	// 終了処理
	CSoldier::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CSoldierPlayer::Update(void)
{
	// 前回の位置の設定処理
	SetPosOld(GetPos());

	// 操作処理
	Control();

	// 移動量
	Move();

	// モーションの更新処理
	GetMotion()->Update();

	// 起伏地面との当たり判定処理
	ElevationCollision();

	// 魔法壁の当たり判定処理
	MagicWall();

	// 状態マネージャー
	StateManager();
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CSoldierPlayer::Draw(void)
{
	// 描画処理
	CSoldier::Draw();
}

//=======================================
// 情報の設定処理
//=======================================
void CSoldierPlayer::SetData(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle)
{
	// 情報の設定処理
	CSoldier::SetData(pos, type, battle);

	// 全ての値を設定する
	m_CameraRot = NONE_D3DXVECTOR3;		// カメラの向き
}

//=======================================
// 操作処理
//=======================================
void CSoldierPlayer::Control(void)
{
	if (GetState() != STATE_THROW)
	{ // 投げ状態以外の場合

		// 移動コントロール
		MoveControl();

		// ジャンプコントロール
		JumpControl();

		// 投げのコントロール
		ThrowControl();
	}

	// カメラのコントロール
	CameraControl();
}

//=======================================
// 移動コントロール
//=======================================
void CSoldierPlayer::MoveControl(void)
{
	// 目的の向きを取得する
	D3DXVECTOR3 rotDest = GetRotDest();
	bool bMove;		// 移動状況

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_W) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickLYPress(0) > 0)
	{ // Wキーを押している場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_A) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0) < 0)
		{ // Aキーを押している場合

			// 目的の向きを設定する
			rotDest.y = -D3DX_PI * 0.25f + m_CameraRot.y;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_D) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0) > 0)
		{ // Dキーを押している場合

			// 目的の向きを設定する
			rotDest.y = D3DX_PI * 0.25f + m_CameraRot.y;
		}
		else
		{ // 上記以外

			// 目的の向きを設定する
			rotDest.y = 0.0f + m_CameraRot.y;
		}

		// 移動状況を true にする
		bMove = true;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_S) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickLYPress(0) < 0)
	{ // Sキーを押している場合

		if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_A) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0) < 0)
		{ // Aキーを押している場合

			// 目的の向きを設定する
			rotDest.y = -D3DX_PI * 0.75f + m_CameraRot.y;
		}
		else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_D) == true ||
			CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0) > 0)
		{ // Dキーを押している場合

			// 目的の向きを設定する
			rotDest.y = D3DX_PI * 0.75f + m_CameraRot.y;
		}
		else
		{ // 上記以外

			// 目的の向きを設定する
			rotDest.y = D3DX_PI + m_CameraRot.y;
		}

		// 移動状況を true にする
		bMove = true;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_A) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0) < 0)
	{ // Aキーを押している場合

		// 目的の向きを設定する
		rotDest.y = -D3DX_PI * 0.5f + m_CameraRot.y;

		// 移動状況を true にする
		bMove = true;
	}
	else if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_D) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickLXPress(0) > 0)
	{ // Dキーを押している場合

		// 目的の向きを設定する
		rotDest.y = D3DX_PI * 0.5f + m_CameraRot.y;

		// 移動状況を true にする
		bMove = true;
	}
	else
	{ // 上記以外

		// 移動状況を false にする
		bMove = false;
	}

	if (bMove == true &&
		GetMotion()->GetType() == MOTIONTYPE_NEUTRAL)
	{ // 移動モーションじゃない場合

		// 移動モーションを設定する
		GetMotion()->Set(MOTIONTYPE_MOVE);
	}
	else if(bMove == false &&
		GetMotion()->GetType() == MOTIONTYPE_MOVE)
	{ // 待機モーションじゃない場合

		// 移動モーションを設定する
		GetMotion()->Set(MOTIONTYPE_NEUTRAL);
	}

	// 移動状況を設定する
	SetEnableMove(bMove);

	// 向きの正規化
	useful::RotNormalize(&rotDest.y);

	// 目的の向きを設定する
	SetRotDest(rotDest);
}

//=======================================
// ジャンプコントロール
//=======================================
void CSoldierPlayer::JumpControl(void)
{
	if ((CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_SPACE) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_A, 0) == true) &&
		IsJump() == false)
	{ // SPACEキーを押した場合

		// ジャンプ処理
		Jump();

		if (GetMotion()->GetType() != MOTIONTYPE_JUMP)
		{ // ジャンプモーションじゃない場合

			// ジャンプモーションを設定する
			GetMotion()->Set(MOTIONTYPE_JUMP);
		}
	}
}

//=======================================
// 投げのコントロール
//=======================================
void CSoldierPlayer::ThrowControl(void)
{
	if (CManager::Get()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::Get()->GetInputGamePad()->GetTrigger(CInputGamePad::JOYKEY_RB, 0) == true)
	{ // ENTERキーを押した場合

		// 投げ状態にする
		SetState(STATE_THROW);

		if (GetMotion()->GetType() != MOTIONTYPE_THROW)
		{ // ジャンプモーションじゃない場合

			// ジャンプモーションを設定する
			GetMotion()->Set(MOTIONTYPE_THROW);
		}
	}
}

//=======================================
// カメラのコントロール
//=======================================
void CSoldierPlayer::CameraControl(void)
{
	// 注視点と視点を取得する
	D3DXVECTOR3 posR = CManager::Get()->GetCamera()->GetPosR();
	D3DXVECTOR3 posV = CManager::Get()->GetCamera()->GetPosV();

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_LSHIFT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) < 0)
	{ // 左にスティックを倒した場合

		// 向きを加算する
		m_CameraRot.y -= CAMERA_CYCLE;
	}

	if (CManager::Get()->GetInputKeyboard()->GetPress(DIK_RSHIFT) == true ||
		CManager::Get()->GetInputGamePad()->GetGameStickRXPress(0) > 0)
	{ // 右にスティックを倒した場合

		// 向きを加算する
		m_CameraRot.y += CAMERA_CYCLE;
	}

	// 視点を設定する
	posV.x = posR.x + sinf(m_CameraRot.y) * CAMERA_DISTANCE;
	posV.z = posR.z + cosf(m_CameraRot.y) * CAMERA_DISTANCE;

	// 視点を設定する
	CManager::Get()->GetCamera()->SetPosV(posV);
}