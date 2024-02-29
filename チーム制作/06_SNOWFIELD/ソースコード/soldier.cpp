//============================================
//
// 兵士のメイン処理[soldier.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "soldier.h"
#include "game.h"
#include "castle.h"
#include "off_respawn.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "useful.h"

#include "motion.h"
#include "elevation_manager.h"
#include "objectElevation.h"
#include "snowball.h"
#include "character.h"
#include "sound.h"

#include "soldier_manager.h"
#include "soldier_player.h"
#include "soldier_ai.h"

//--------------------------------------------
// マクロ定義
//--------------------------------------------
namespace
{
	const int MAX_LIFE = 5;					// 体力の最大数
	const int NUM_MODEL = 15;				// モデルの総数
	const int THROWSTATE_COUNT = 40;		// 雪投げ状態のカウント
	const int THROW_COUNT = 20;				// 雪投げカウント
	const float ADD_GRAVITY = -50.0f;		// 着地時の追加の重力
	const float SPEED = 4.0f;				// 移動量
	const float GRAVITY = 0.4f;				// 重力
	const float JUMP = 4.0f;				// ジャンプの高さ
	const float SNOWBALL_SPEED = 16.0f;		// 雪玉の速度
	const float SNOWBALL_HEIGHT = 40.0f;	// 雪玉の高さ
	const float ROT_CORRECT = 0.1f;			// 向きの補正係数
	const float STAGE_RIGHTMAX = 800.0f;	// ステージの右側の最大値
	const float STAGE_LEFTMAX = -800.0f;	// ステージの左側の最大値
	const float STAGE_FARMAX = 2000.0f;		// ステージの奥側の最大値
	const float STAGE_NEARMAX = -2000.0f;	// ステージの手前側の最大値
}

//=========================================
// コンストラクタ
//=========================================
CSoldier::CSoldier() : CCharacter(CObject::TYPE_SOLDIER, CObject::PRIORITY_PLAYER)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// オーバーロードコンストラクタ
//=========================================
CSoldier::CSoldier(CObject::TYPE type, PRIORITY priority) : CCharacter(type, priority)
{
	// コンストラクタの箱
	Box();
}

//=========================================
// デストラクタ
//=========================================
CSoldier::~CSoldier()
{

}

//=========================================
// コンストラクタの箱
//=========================================
void CSoldier::Box(void)
{
	// 全ての値をクリアする
	m_pMotion = nullptr;			// モーションの情報
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotDest = NONE_D3DXVECTOR3;	// 目的の向き
	m_type = TYPE_AI;				// 種類
	m_battle = BATTLE_OFF;			// 攻守
	m_state = STATE_NONE;			// 状態
	m_nSnowCount = 0;				// 雪投げカウント
	m_fSpeed = SPEED;				// 速度
	m_bMove = false;				// 移動状況
	m_bJump = false;				// ジャンプ状況

	m_pPrev = nullptr;		// 前のへのポインタ
	m_pNext = nullptr;		// 次のへのポインタ

	if (CSoldierManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// マネージャーへの登録処理
		CSoldierManager::Get()->Regist(this);
	}
}

//============================
// 前のポインタの設定処理
//============================
void CSoldier::SetPrev(CSoldier* pPrev)
{
	// 前のポインタを設定する
	m_pPrev = pPrev;
}

//============================
// 後のポインタの設定処理
//============================
void CSoldier::SetNext(CSoldier* pNext)
{
	// 次のポインタを設定する
	m_pNext = pNext;
}

//============================
// 前のポインタの設定処理
//============================
CSoldier* CSoldier::GetPrev(void) const
{
	// 前のポインタを返す
	return m_pPrev;
}

//============================
// 次のポインタの設定処理
//============================
CSoldier* CSoldier::GetNext(void) const
{
	// 次のポインタを返す
	return m_pNext;
}


//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CSoldier::Init(void)
{
	if (FAILED(CCharacter::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	// ベタ打ち
	SetNumModel(NUM_MODEL);

	// データの設定処理
	CCharacter::SetData();

	if (m_pMotion == nullptr)
	{ // モーションが NULL だった場合

		// モーションの生成処理
		m_pMotion = CMotion::Create();
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	if (m_pMotion != nullptr)
	{ // ポインタが NULL じゃない場合

		// モーションの情報を取得する
		m_pMotion->SetModel(GetHierarchy(), GetNumModel());

		// ロード処理
		m_pMotion->Load(CMotion::TYPE_PLAYER);
	}
	else
	{ // ポインタが NULL じゃない場合

		// 停止
		assert(false);
	}

	// 待機モーションを設定
	GetMotion()->Set(0);

	// 全ての値を初期化する
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotDest = NONE_D3DXVECTOR3;	// 目的の向き
	m_type = TYPE_AI;				// 種類
	m_battle = BATTLE_OFF;			// 攻守
	m_state = STATE_NONE;			// 状態
	m_nSnowCount = 0;				// 雪投げカウント
	m_fSpeed = SPEED;				// 速度
	m_bMove = false;				// 移動状況
	m_bJump = false;				// ジャンプ状況

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CSoldier::Uninit(void)
{
	// モーションのメモリを開放する
	delete m_pMotion;
	m_pMotion = nullptr;

	// 終了処理
	CCharacter::Uninit();

	if (CSoldierManager::Get() != nullptr)
	{ // マネージャーが存在していた場合

		// リスト構造の引き抜き処理
		CSoldierManager::Get()->Pull(this);
	}

	// リスト構造関係のポインタを NULL にする
	m_pPrev = nullptr;
	m_pNext = nullptr;
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CSoldier::Draw(void)
{
	// 描画処理
	CCharacter::Draw();
}

//===========================================
// モーションの情報の設定処理
//===========================================
CMotion* CSoldier::GetMotion(void) const
{
	// モーションの情報を返す
	return m_pMotion;
}

//=======================================
// 当たり判定処理
//=======================================
void CSoldier::Hit()
{
	D3DXVECTOR3 posRes;

	if (m_battle == BATTLE_OFF)
	{
		posRes = CGame::GetRespawn()->GetPos();
	}
	else
	{
		posRes = CGame::GetCastle()->GetPos();
		posRes.z -= 400.0f;
	}

	posRes.x += (float)(rand() % 1200 - 600);
	posRes.z += (float)(rand() % 100 - 50);

	SetPos(posRes);

	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);
}

//===========================================
// 生成処理
//===========================================
CSoldier* CSoldier::Create(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle)
{
	// 兵士のポインタを宣言
	CSoldier* pSoldier = nullptr;

	if (pSoldier == nullptr)
	{ // オブジェクトが NULL の場合

		switch (type)
		{
		case CSoldier::TYPE_AI:			// AIの場合

			// メモリを確保する
			pSoldier = new CSoldierAI;

			break;

		case CSoldier::TYPE_PLAYER:		// プレイヤーの場合

			// メモリを確保する
			pSoldier = new CSoldierPlayer;

			break;

		default:

			// 停止
			assert(false);

			break;
		}
	}
	else
	{ // オブジェクトが NULL じゃない場合

		// NULL を返す
		return pSoldier;
	}

	if (pSoldier != nullptr)
	{ // オブジェクトが NULL じゃない場合

		// 初期化処理
		if (FAILED(pSoldier->Init()))
		{ // 初期化に失敗した場合

			// 停止
			assert(false);

			// NULL を返す
			return nullptr;
		}

		// 情報の設定処理
		pSoldier->SetData(pos, type, battle);
	}
	else
	{ // オブジェクトが NULL の場合

		// NULL を返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pSoldier;
}

//=======================================
// 情報の設定処理
//=======================================
void CSoldier::SetData(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle)
{
	// 全ての値を設定する
	m_move = NONE_D3DXVECTOR3;		// 移動量
	m_rotDest = NONE_D3DXVECTOR3;	// 目的の向き
	m_type = type;					// 種類
	m_battle = battle;				// 攻守
	m_state = STATE_NONE;			// 状態
	m_nSnowCount = 0;				// 雪投げカウント
	m_fSpeed = SPEED;				// 速度
	m_bMove = false;				// 移動状況
	m_bJump = false;				// ジャンプ状況

	// 全ての値を初期化する
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));		// 位置
	SetPosOld(GetPos());			// 前回の位置
	SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));				// 向き
	SetScale(NONE_SCALE);			// 拡大率

	m_rotDest = GetRot();	// 目的の向きを設定する

	for (int nCntData = 0; nCntData < GetNumModel(); nCntData++)
	{
		// 初期化処理
		GetHierarchy(nCntData)->SetPos(pos);										// 位置
		GetHierarchy(nCntData)->SetPosOld(pos);										// 前回の位置
		GetHierarchy(nCntData)->SetRot(NONE_D3DXVECTOR3);							// 向き
		GetHierarchy(nCntData)->SetScale(NONE_SCALE);								// 拡大率

		if (nCntData == 2)
		{ // 頭を決めるとき

			switch (m_battle)
			{
			case CSoldier::BATTLE_OFF:

				GetHierarchy(nCntData)->SetFileData(CXFile::TYPE_PLAYER_HEAD);		// データの設定処理

				break;

			case CSoldier::BATTLE_DEF:

				GetHierarchy(nCntData)->SetFileData(CXFile::TYPE_PLAYER_DEFHEAD);	// データの設定処理

				break;

			default:

				// 停止
				assert(false);

				break;
			}
		}
		else if(nCntData >= 3)
		{ // 頭以降

			GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(INIT_PLAYER + nCntData + 1));	// データの設定処理
		}
		else
		{ // 上記以外

			GetHierarchy(nCntData)->SetFileData(CXFile::TYPE(INIT_PLAYER + nCntData));		// データの設定処理
		}
	}
}

//=======================================
// 移動量の設定処理
//=======================================
void CSoldier::SetMove(const D3DXVECTOR3& move)
{
	// 移動量を設定する
	m_move = move;
}

//=======================================
// 移動量の取得処理
//=======================================
D3DXVECTOR3 CSoldier::GetMove(void) const
{
	// 移動量を返す
	return m_move;
}

//=======================================
// 目標の向きの設定処理
//=======================================
void CSoldier::SetRotDest(const D3DXVECTOR3& rot)
{
	// 目標の向きを設定する
	m_rotDest = rot;
}

//=======================================
// 目標の向きの取得処理
//=======================================
D3DXVECTOR3 CSoldier::GetRotDest(void) const
{
	// 目標の向きを返す
	return m_rotDest;
}

//=======================================
// 種類の取得処理
//=======================================
CSoldier::TYPE CSoldier::GetType(void) const
{
	// 種類を返す
	return m_type;
}

//=======================================
// 状態の設定処理
//=======================================
void CSoldier::SetState(const STATE state)
{
	// 状態を設定する
	m_state = state;
}

//=======================================
// 状態の取得処理
//=======================================
CSoldier::STATE CSoldier::GetState(void) const
{
	// 状態を返す
	return m_state;
}

//=======================================
// 速度の設定処理
//=======================================
void CSoldier::SetSpeed(float fSpeed)
{
	// 速度を設定する
	m_fSpeed = fSpeed;
}

//=======================================
// 速度の取得処理
//=======================================
float CSoldier::GetSpeed(void) const
{
	// 速度を返す
	return m_fSpeed;
}

//=======================================
// 陣営の取得処理
//=======================================
CSoldier::BATTLE CSoldier::GetBattle(void) const
{
	return m_battle;
}

//=======================================
// 移動状況の設定処理
//=======================================
void CSoldier::SetEnableMove(bool bMove)
{
	// 移動状況を設定する
	m_bMove = bMove;
}

//=======================================
// 移動状況の取得処理
//=======================================
bool CSoldier::IsMove(void) const
{
	// 移動状況を返す
	return m_bMove;
}

//=======================================
// ジャンプ状況の設定処理
//=======================================
void CSoldier::SetEnableJump(bool bJump)
{
	// ジャンプ状況を設定する
	m_bJump = bJump;
}

//=======================================
// ジャンプ状況の取得処理
//=======================================
bool CSoldier::IsJump(void) const
{
	// ジャンプ状況を返す
	return m_bJump;
}

//=======================================
// 移動処理
//=======================================
void CSoldier::Move(void)
{
	// 位置と向きを取得する
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();

	if (m_bMove == true &&
		m_state == STATE_NONE)
	{ // 移動状況が true の場合

		// 移動量を設定する
		m_move.x = sinf(m_rotDest.y) * m_fSpeed;
		m_move.z = cosf(m_rotDest.y) * m_fSpeed;
	}
	else
	{ // 上記以外

		// 移動量を初期化する
		m_move.x = 0.0f;
		m_move.z = 0.0f;
	}

	// 位置を移動する
	pos += m_move;

	// 重力を足す
	useful::Gravity(&m_move.y, pos, GRAVITY);

	// 向きの正規化処理
	useful::RotCorrect(m_rotDest.y, &rot.y, ROT_CORRECT);

	// 位置と向きを設定する
	SetPos(pos);
	SetRot(rot);
}

//=======================================
// ジャンプ処理
//=======================================
void CSoldier::Jump(void)
{
	// 縦の移動量を設定する
	m_move.y = JUMP;

	// ジャンプ状況を true にする
	m_bJump = true;
}

//=======================================
// 投げる処理
//=======================================
void CSoldier::Throw(void)
{
	// 移動量を宣言
	D3DXVECTOR3 move = NONE_D3DXVECTOR3;

	// 移動量を設定する
	move.x = sinf(GetRot().y) * SNOWBALL_SPEED;
	move.z = cosf(GetRot().y) * SNOWBALL_SPEED;

	// 雪玉を生成する
	CSnowBall::Create(D3DXVECTOR3(GetPos().x, GetPos().y + SNOWBALL_HEIGHT, GetPos().z), move, m_battle);

	CManager::Get()->GetSound()->Play(CSound::SOUND_LABEL_SE_SNOW);
}

//=======================================
// 起伏地面の当たり判定処理
//=======================================
void CSoldier::ElevationCollision(void)
{
	// ローカル変数宣言
	CElevation* pMesh = CElevationManager::Get()->GetTop();		// 起伏の先頭のオブジェクトを取得する
	D3DXVECTOR3 pos = GetPos();		// 位置を取得する
	float fHeight = 0.0f;			// 高さ
	bool bJump = true;				// ジャンプ状況
	bool bRange = false;			// 範囲内状況

	while (pMesh != nullptr)
	{ // 地面の情報がある限り回す

		// 当たり判定を取る
		fHeight = pMesh->ElevationCollision(pos, bRange);
		
		if (pos.y < fHeight)
		{ // 当たり判定の位置が高かった場合

			// 高さを設定する
			pos.y = fHeight;

			// 重力を設定する
			m_move.y = ADD_GRAVITY;

			// ジャンプ状況を false にする
			bJump = false;

			if (GetMotion()->GetType() == MOTIONTYPE_JUMP)
			{ // ジャンプモーションの場合

				// 待機モーションに設定する
				GetMotion()->Set(MOTIONTYPE_NEUTRAL);
			}
		}

		// 次のポインタを取得する
		pMesh = pMesh->GetNext();
	}

	// ジャンプ状況を設定する
	m_bJump = bJump;

	// 位置を更新する
	SetPos(pos);
}

//=======================================
// 魔法壁の当たり判定処理
//=======================================
void CSoldier::MagicWall(void)
{
	// 位置を取得する
	D3DXVECTOR3 pos = GetPos();

	if (pos.x >= STAGE_RIGHTMAX)
	{ // 右の限界地を超えた場合

		// 位置を設定する
		pos.x = STAGE_RIGHTMAX;
	}

	if (pos.x <= STAGE_LEFTMAX)
	{ // 左の限界地を超えた場合

		// 位置を設定する
		pos.x = STAGE_LEFTMAX;
	}

	if (pos.z >= STAGE_FARMAX)
	{ // 奥の限界地を超えた場合

		// 位置を設定する
		pos.z = STAGE_FARMAX;
	}

	if (pos.z <= STAGE_NEARMAX)
	{ // 手前の限界地を超えた場合

		// 位置を設定する
		pos.z = STAGE_NEARMAX;
	}

	// 位置を適用する
	SetPos(pos);
}

//=======================================
// 状態マネージャー
//=======================================
void CSoldier::StateManager(void)
{
	switch (m_state)
	{
	case CSoldier::STATE_NONE:



		break;

	case CSoldier::STATE_THROW:

		// 雪投げカウントを加算する
		m_nSnowCount++;

		if (m_nSnowCount == THROW_COUNT)
		{ // 一定カウントの場合

			// 投げ処理
			Throw();
		}

		if (m_nSnowCount >= THROWSTATE_COUNT)
		{ // カウントが一定以上になった場合

			// 雪投げカウントを 0 にする
			m_nSnowCount = 0;

			// 通常状態にする
			m_state = STATE_NONE;

			// 待機モーションにする
			GetMotion()->Set(MOTIONTYPE_NEUTRAL);
		}

		break;

	default:
		
		// 停止
		assert(false);

		break;
	}
}