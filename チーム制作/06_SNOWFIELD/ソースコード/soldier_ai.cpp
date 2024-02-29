//============================================
//
// 兵士AIのメイン処理[soldier_AI.cpp]
// Author：小原立暉
//
//============================================
//********************************************
// インクルードファイル
//********************************************
#include "manager.h"
#include "soldier_ai.h"
#include "game.h"
#include "flag.h"
#include "castle.h"
#include "off_respawn.h"
#include "sandbag.h"
#include "renderer.h"
#include "debugproc.h"
#include "model.h"
#include "useful.h"
#include "collision.h"

#include "input.h"
#include "motion.h"

#include "soldier_manager.h"
#include "sandbag_manager.h"

//=========================================
// コンストラクタ
//=========================================
CSoldierAI::CSoldierAI() : CSoldier(CObject::TYPE_SOLDIER, CObject::PRIORITY_PLAYER)
{
	m_BattleAI = BATTLEAI_ATTACK;
	m_Situation = SITUATION_ASSAULT;
	m_nCountJudge = 0;
	m_nNumNearPlayer = 0;
	m_nNumNearEnemy = 0;
	m_fTargetRot = 0.0f;
}

//=========================================
// デストラクタ
//=========================================
CSoldierAI::~CSoldierAI()
{

}

//===========================================
// プレイヤーの初期化処理
//===========================================
HRESULT CSoldierAI::Init(void)
{
	if (FAILED(CSoldier::Init()))
	{ // 初期化処理に失敗した場合

		// 停止
		assert(false);

		// 失敗を返す
		return E_FAIL;
	}

	m_nCountJudge = 0;

	// 値を返す
	return S_OK;
}

//===========================================
// プレイヤーの終了処理
//===========================================
void CSoldierAI::Uninit(void)
{
	// 終了処理
	CSoldier::Uninit();
}

//===========================================
// 更新処理
//===========================================
void CSoldierAI::Update(void)
{
	// 前回の位置の設定処理
	SetPosOld(GetPos());

	// AI
	AI();

	// 移動量
	Move();

	// モーションの更新処理
	GetMotion()->Update();

	// 起伏地面との当たり判定処理
	ElevationCollision();
}

//===========================================
// プレイヤーの描画処理
//===========================================
void CSoldierAI::Draw(void)
{
	// 描画処理
	CSoldier::Draw();
}

//=======================================
// 情報の設定処理
//=======================================
void CSoldierAI::SetData(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle)
{
	// 情報の設定処理
	CSoldier::SetData(pos, type, battle);

	int nRand = rand() % 2;

	if (nRand)
	{
		m_BattleAI = BATTLEAI_ATTACK;
	}
	else
	{
		m_BattleAI = BATTLEAI_GUARD;
	}
}

//=======================================
// AIの処理
//=======================================
void CSoldierAI::AI(void)
{
	m_nCountJudge++;
	m_nNumNearPlayer = 0;
	m_nNumNearEnemy = 0;

	CSoldierManager *soldierManager = CSoldierManager::Get();
	m_AimPos = GetPos();

	if (soldierManager != nullptr)
	{ // マネージャーが存在していた場合

	  // ポインタを宣言
		CSoldier *pObjectTop = soldierManager->GetTop();	// 先頭オブジェクト

		if (pObjectTop != nullptr)
		{
			// ポインタを宣言
			CSoldier *pObjCheck = pObjectTop;	// オブジェクト確認用

			while (pObjCheck != NULL)
			{ // オブジェクトが使用されている場合繰り返す

				CSoldier *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト
				D3DXVECTOR3 vecPlayer = pObjCheck->GetPos() - GetPos();
				float fLength = D3DXVec3Length(&vecPlayer);

				if (fLength < 1000.0f)
				{
					if (pObjCheck->GetBattle() == GetBattle())
					{
						m_nNumNearPlayer++;
					}
					else
					{
						m_nNumNearEnemy++;

						if (m_AimPos == GetPos())
						{
							m_AimPos = pObjCheck->GetPos();
						}
						else
						{
							int nRand = rand() % 4;

							if (nRand > 2)
							{
								m_AimPos = pObjCheck->GetPos();
							}
						}
					}
				}

				// 次のオブジェクトへのポインタを代入
				pObjCheck = pObjectNext;
			}
		}
	}

	if (GetBattle() == BATTLE_OFF)
	{
		AIOffense();
	}
	else
	{
		AIDefense();
	}

	if (GetState() != STATE_THROW)
	{ // 投げ状態以外の場合
		AIMove();
		AIShoot();
	}
}

//=======================================
// 移動AIの処理
//=======================================
void CSoldierAI::AIMove(void)
{
	// 目的の向きを取得する
	D3DXVECTOR3 rotDest = GetRotDest();

	if (m_Situation == SITUATION_ASSAULT)
	{
		if (m_nCountJudge == 0)
		{
			D3DXVECTOR3 vecFlag = CGame::GetFlag()->GetPos() - GetPos();

			m_fTargetRot = atan2f(vecFlag.x, vecFlag.z) + ((float)((rand() % 61) - 30) * 0.01f);
		}

		// 目的の向きを設定する
		rotDest.y = m_fTargetRot;

		// 移動状況を設定する
		SetEnableMove(true);
	}
	else if (m_Situation == SITUATION_SHOOT)
	{
		if (m_nCountJudge == 0)
		{
			D3DXVECTOR3 vecEnemy = m_AimPos - GetPos();

			m_fTargetRot = atan2f(vecEnemy.x, vecEnemy.z);
		}

		// 目的の向きを設定する
		rotDest.y = m_fTargetRot;

		// 移動状況を設定する
		SetEnableMove(false);
	}
	else if (m_Situation == SITUATION_GUARD)
	{
		if (m_nCountJudge == 0)
		{
			D3DXVECTOR3 vecFlag = CGame::GetRespawn()->GetPos() - GetPos();

			m_fTargetRot = atan2f(vecFlag.x, vecFlag.z) + ((float)((rand() % 61) - 30) * 0.01f);
		}

		// 目的の向きを設定する
		rotDest.y = m_fTargetRot;

		D3DXVECTOR3 vecFlag = CGame::GetFlag()->GetPos() - GetPos();

		if (D3DXVec3Length(&vecFlag) < 400.0f)
		{
			// 移動状況を設定する
			SetEnableMove(true);
		}
		else
		{
			D3DXVECTOR3 vecRes = CGame::GetRespawn()->GetPos() - GetPos();
			D3DXVECTOR3 vecFlagToRes = CGame::GetRespawn()->GetPos() - CGame::GetFlag()->GetPos();
			float lengthRes, lengthFlag;

			lengthRes = D3DXVec3Length(&vecRes);
			lengthFlag = D3DXVec3Length(&vecFlagToRes);

			if (lengthRes < lengthFlag)
			{
				// 移動状況を設定する
				SetEnableMove(false);
			}
			else
			{
				// 移動状況を設定する
				SetEnableMove(true);
			}
		}

		CSandbagManager *sandbagManager = CSandbagManager::Get();

		if (sandbagManager != nullptr)
		{ // マネージャーが存在していた場合

		  // ポインタを宣言
			CSandbag *pObjectTop = sandbagManager->GetTop();	// 先頭オブジェクト

			if (pObjectTop != nullptr)
			{
				// ポインタを宣言
				CSandbag *pObjCheck = pObjectTop;	// オブジェクト確認用

				while (pObjCheck != NULL)
				{ // オブジェクトが使用されている場合繰り返す

					CSandbag *pObjectNext = pObjCheck->GetNext();	// 次オブジェクト

					D3DXVECTOR3 movePos = collision::PosRelativeMtx(GetPos(), D3DXVECTOR3(0.0f, rotDest.y, 0.0f), D3DXVECTOR3(10.0f, 0.0f, 0.0f));

					if (collision::SquareTrigger(pObjCheck->GetPos(), movePos, pObjCheck->GetRot(), D3DXVECTOR3(100.0f, 100.0f, 100.0f), D3DXVECTOR3(-100.0f, -100.0f, -100.0f)))
					{
						// 移動状況を設定する
						SetEnableMove(false);
					}

					// 次のオブジェクトへのポインタを代入
					pObjCheck = pObjectNext;
				}
			}
		}
	}
	else if (m_Situation == SITUATION_ESCAPE)
	{
		if (m_nCountJudge == 0)
		{
			D3DXVECTOR3 vecEnemy = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			if (GetBattle() == BATTLE_OFF)
			{
				vecEnemy = CGame::GetRespawn()->GetPos() - GetPos();
			}
			else
			{
				vecEnemy = CGame::GetCastle()->GetPos() - GetPos();
			}

			m_fTargetRot = atan2f(vecEnemy.x, vecEnemy.z);
		}

		// 目的の向きを設定する
		rotDest.y = m_fTargetRot;

		// 移動状況を設定する
		SetEnableMove(true);
	}
	else
	{
		// 移動状況を設定する
		SetEnableMove(false);
	}

	if (IsMove() == true &&
		GetMotion()->GetType() == MOTIONTYPE_NEUTRAL)
	{ // 移動モーションじゃない場合

		// 移動モーションを設定する
		GetMotion()->Set(MOTIONTYPE_MOVE);
	}
	else if(IsMove() == false &&
		GetMotion()->GetType() == MOTIONTYPE_MOVE)
	{ // 待機モーションじゃない場合

		// 移動モーションを設定する
		GetMotion()->Set(MOTIONTYPE_NEUTRAL);
	}

	// 目的の向きを設定する
	SetRotDest(rotDest);
}

//=======================================
// 雪玉投げAIの処理
//=======================================
void CSoldierAI::AIShoot(void)
{
	if (m_Situation == SITUATION_SHOOT)
	{
		if (m_nCountJudge == rand() % 60 && m_nNumNearEnemy > 0)
		{
			// 投げ処理
			Throw();

			if (GetMotion()->GetType() != MOTIONTYPE_THROW)
			{  // 投げモーション以外の場合

				// 投げモーションにする
				GetMotion()->Set(MOTIONTYPE_THROW);
			}
		}
	}
}

//=======================================
// 攻めAIの処理
//=======================================
void CSoldierAI::AIOffense(void)
{
	if (m_BattleAI == BATTLEAI_ATTACK)
	{
		AIOffenseAttack();
	}
	else
	{
		AIOffenseGuard();
	}

	D3DXVECTOR3 vecFlag = CGame::GetFlag()->GetPos() - GetPos();

	if (D3DXVec3Length(&vecFlag) < 200.0f)
	{
		if (m_nNumNearEnemy > 1)
		{
			int nRand = rand() % 4;

			if (nRand > 2)
			{
				m_Situation = SITUATION_ASSAULT;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else
		{
			m_Situation = SITUATION_WAIT;
		}
	}
}
void CSoldierAI::AIOffenseAttack(void)
{//攻撃的

	int nRand = 0;

	if (m_nCountJudge % 60 == 0)
	{
		m_nCountJudge = 0;

		nRand = rand() % 4;

		if (m_nNumNearEnemy > 1)
		{
			if (nRand > 1)
			{
				m_Situation = SITUATION_ASSAULT;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else if (m_nNumNearEnemy > 0)
		{
			if (nRand > 0)
			{
				m_Situation = SITUATION_ASSAULT;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else
		{
			m_Situation = SITUATION_ASSAULT;
		}
	}
}
void CSoldierAI::AIOffenseGuard(void)
{//守備的

	int nRand = 0;

	if (m_nCountJudge % 60 == 0)
	{
		m_nCountJudge = 0;

		nRand = rand() % 4;

		if (m_nNumNearEnemy > 2)
		{
			if (m_nNumNearPlayer == 0)
			{
				if (nRand > 0)
				{
					m_Situation = SITUATION_ESCAPE;
				}
				else
				{
					m_Situation = SITUATION_SHOOT;
				}
			}
			else
			{
				if (nRand > 2)
				{
					m_Situation = SITUATION_ASSAULT;
				}
				else
				{
					m_Situation = SITUATION_SHOOT;
				}
			}
		}
		else if (m_nNumNearEnemy > 0)
		{
			if (nRand > 2)
			{
				m_Situation = SITUATION_ASSAULT;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else
		{
			m_Situation = SITUATION_ASSAULT;
		}
	}
}

//=======================================
// 守りAIの処理
//=======================================
void CSoldierAI::AIDefense(void)
{
	if (m_BattleAI == BATTLEAI_ATTACK)
	{
		AIDefenseAttack();
	}
	else
	{
		AIDefenseGuard();
	}
}
void CSoldierAI::AIDefenseAttack(void)
{// 攻撃的

	int nRand = 0;

	if (m_nCountJudge % 60 == 0)
	{
		m_nCountJudge = 0;

		nRand = rand() % 4;

		if (m_nNumNearEnemy > 1)
		{
			if (nRand > 0)
			{
				m_Situation = SITUATION_GUARD;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else if (m_nNumNearEnemy > 0)
		{
			if (nRand > 1)
			{
				m_Situation = SITUATION_GUARD;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else
		{
			m_Situation = SITUATION_GUARD;
		}
	}
}
void CSoldierAI::AIDefenseGuard(void)
{// 守備的
	
	int nRand = 0;

	if (m_nCountJudge % 60 == 0)
	{
		m_nCountJudge = 0;

		nRand = rand() % 4;

		if (m_nNumNearEnemy > 2)
		{
			if (m_nNumNearPlayer == 0)
			{
				m_Situation = SITUATION_SHOOT;
			}
			else
			{
				if (nRand > 2)
				{
					m_Situation = SITUATION_GUARD;
				}
				else
				{
					m_Situation = SITUATION_SHOOT;
				}
			}
		}
		else if (m_nNumNearEnemy > 0)
		{
			if (nRand > 2)
			{
				m_Situation = SITUATION_GUARD;
			}
			else
			{
				m_Situation = SITUATION_SHOOT;
			}
		}
		else
		{
			m_Situation = SITUATION_GUARD;
		}
	}
}