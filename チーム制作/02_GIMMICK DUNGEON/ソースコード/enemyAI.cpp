//===========================
//
//エネミーAI処理
//Author:中村　陸
//
//===========================
#include "main.h"
#include "enemyAI.h"
#include "enemy.h"
#include "player.h"
#include "object.h"
#include "rock.h"
#include "shock.h"
#include "score.h"
#include "billboard.h"
#include "orbit.h"
#include "sound.h"
#include "camera.h"
#include "particle.h"
#include "point_log.h"
#include "tutorial.h"
#include "point_log.h"
#include "game.h"

//マクロ定義
#define ENEMY_ARRIVAL_POINT (3.0f)		//ポイント到達とみなす距離
#define ENEMY_LENGTH (300.0f)			//プレイヤーの近くで止まる距離
#define ENEMY_TIME (1800)				//次のポイントに切り替える時間
#define ENEMY_SIGHT (0.4f)				//視界の範囲
#define ENEMY_FIND_COUNTER (1000)		//見つけてから減点されるまでのカウンター
#define ENEMY_ATTCK (400)		//見つけてから減点されるまでのカウンター
#define SPEED_PIERCE	(0.2f)			//突き刺し衝撃波のスピード
#define SPEED_STEP	(0.1f)				//足踏み衝撃波のスピード
#define SPEED_JUMP	(0.3f)				//ジャンプ攻撃衝撃波のスピード
#define SCORE_LOSTFIND	(1500)				//発見判定振り切り時スコア

//グローバル変数宣言
Enemy *g_pEnemy;						//エネミー構造体
int g_nCntFind;							//発見のカウント
bool bEnemyFind;						//発見してるかどうか

//===========================
//エネミーの初期化処理
//===========================
void InitEnemyAi(void)
{
	g_pEnemy = GetEnemy();

	//数値初期化
	g_nCntFind = 0;							//時間のリセット
	bEnemyFind = false;						//発見してるかどうか
}

//===========================
//エネミーのAI(まとめ)
//===========================
void AiEnemy(void)
{
	switch (g_pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:
		//巡回
		AiEnemyWalk();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_SEARCH:
		//索敵
		AiEnemySearch();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_LOST:
		//見失った
		AiEnemyLost();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_FIND:
		//見失った
		AiEnemyFind();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_TRACK:
		//追跡
		AiEnemyTrack();
		g_pEnemy->nAttackCount++;
		break;
	case ENEMYSTATE_DAMAGE:
		//ダメージ
		AiEnemyDamage();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_COUNTER:
		//反撃
		AiEnemyCounter();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_JUMPATTACK:
		//ジャンプ攻撃
		AiEnemyJumpAttack();
		g_pEnemy->nAttackCount++;
		break;
	case ENEMYSTATE_DEATH:
		SetMotionEnemy(ENEMYMOTION_DEATH);

		//速度調整
		g_pEnemy->fSpeed = 0.0f;
		break;
	case ENEMYSTATE_SLEEP:
		//距離
		AiLengthEnemy();

		SetMotionEnemy(ENEMYMOTION_SLEEP);

		if (g_pEnemy->nHitRock >= 5)
		{
			g_pEnemy->state = ENEMYSTATE_STANDUP;
			SetMotionEnemy(ENEMYMOTION_STANDUP);
		}

		//速度調整
		g_pEnemy->fSpeed = 0.0f;
		break;
	case ENEMYSTATE_STANDUP:
		//距離
		AiLengthEnemy();

		SetMotionEnemy(ENEMYMOTION_STANDUP);

		//速度調整
		g_pEnemy->fSpeed = 0.0f;
		break;
	default:
		break;
	}

	AiEnemyMotion();
}

//===========================
//エネミーのAI(チュートリアル)
//===========================
void AiEnemyTutorial(void)
{
	//距離
	AiLengthEnemy();

	if (*GetTutorialState() == TUTORIALSTATE_JUMP && GetMode() == MODE_TUTORIAL)
	{
		//一定時間で攻撃
		if (g_pEnemy->nAttackCount >= 300 && g_pEnemy->motionState == ENEMYMOTION_NORMAL)
		{
			//キーとフレームをリセット
			g_pEnemy->nCntKey = 0;
			g_pEnemy->nCntFrame = 0;

			//突き刺しモーション
			SetMotionEnemy(ENEMYMOTION_PIERCE);
		}

		g_pEnemy->nAttackCount++;
	}

	//速度調整
	g_pEnemy->fSpeed = 0.0f;

	AiEnemyMotion();
}

//===========================
//エネミーのAI(巡回)
//===========================
void AiEnemyWalk(void)
{
	//距離
	AiLengthEnemy();

	//視界
	AiSightEnemy();

	//移動地点
	AiPointEnemy();

	//角度
	AiRotEnemy();

	//待機、索敵モーションの場合移動モーションに変更
	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_OVERLOOK)
	{
		SetMotionEnemy(ENEMYMOTION_MOVE);
	}
}

//===========================
//エネミーのAI(索敵)
//===========================
void AiEnemySearch(void)
{
	//距離
	AiLengthEnemy();

	//視界
	AiSightEnemy();

	//速度調整
	g_pEnemy->fSpeed = 0.0f;

	//待機、移動モーションの場合索敵モーションに変更
	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_MOVE)
	{
		SetMotionEnemy(ENEMYMOTION_OVERLOOK);
	}
}

//===========================
//エネミーのAI(見失う)
//===========================
void AiEnemyLost(void)
{
	//移動モーションに変更
	SetMotionEnemy(ENEMYMOTION_MOVE);

	//距離
	AiLengthEnemy();

	//視界
	AiSightEnemy();

	//動き
	AiMoveEnemy();

	//角度
	AiRotEnemy();
}

//===========================
//エネミーのAI(発見)
//===========================
void AiEnemyFind(void)
{
	//距離
	AiLengthEnemy();

	//視界
	AiSightEnemy();

	//角度
	AiRotEnemy();

	SetMotionEnemy(ENEMYMOTION_FIND);

	g_pEnemy->fSpeed = 0.0f;
}

//===========================
//エネミーのAI(追跡)
//===========================
void AiEnemyTrack(void)
{
	//待機、索敵モーションの場合移動モーションに変更
	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_OVERLOOK)
	{
		SetMotionEnemy(ENEMYMOTION_MOVE);
	}

	//攻撃頻度調整
	if (g_pEnemy->nLife > 300)
	{
		g_pEnemy->nAttackTime = ENEMY_ATTCK;
	}
	else if (g_pEnemy->nLife > 100)
	{
		g_pEnemy->nAttackTime = (int)(ENEMY_ATTCK * 0.75f);
	}
	else
	{
		g_pEnemy->nAttackTime = (int)(ENEMY_ATTCK * 0.5f);
	}

	//距離
	AiLengthEnemy();

	//視界
	AiSightEnemy();

	//動き
	AiMoveEnemy();

	//攻撃
	AiAttackEnemy();

	//攻撃モーション一覧
	if (g_pEnemy->motionState == ENEMYMOTION_SHAKE ||
		g_pEnemy->motionState == ENEMYMOTION_PIERCE ||
		g_pEnemy->motionState == ENEMYMOTION_STEP ||
		g_pEnemy->motionState == ENEMYMOTION_THROW ||
		g_pEnemy->motionState == ENEMYMOTION_JUMPATTACK ||
		g_pEnemy->motionState == ENEMYMOTION_JUMPATTACK_LIGHT)
	{

	}
	else
	{
		//角度
		AiRotEnemy();
	}

	//発見モーション以外で移動速度調整
	if (g_pEnemy->motionState != ENEMYMOTION_MOVE)
	{
		g_pEnemy->fSpeed = 0.0f;
	}
}

//===========================
//エネミーのAI(ダメージ)
//===========================
void AiEnemyDamage(void)
{
	//移動速度調整
	g_pEnemy->fSpeed = 0.0f;

	//ダメージの体力判定
	if (g_pEnemy->motionState == ENEMYMOTION_DOWN)
	{
		SetMotionEnemy(ENEMYMOTION_DOWN);
	}
	else if (g_pEnemy->motionState == ENEMYMOTION_DAMAGE)
	{
		SetMotionEnemy(ENEMYMOTION_DAMAGE);
	}
}

//===========================
//エネミーのAI(反撃)
//===========================
void AiEnemyCounter(void)
{
	//移動速度調整
	g_pEnemy->fSpeed = 0.0f;

	//移動時間リセット
	g_pEnemy->nTimerMove = 0;

	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_BACKJUMP)
	{//通常状態およびバックジャンプ中

		//バックジャンプモーションに変更
		SetMotionEnemy(ENEMYMOTION_BACKJUMP);

		//モーションに合わせて移動量をたす
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//自分の向いてる向きに進む
			g_pEnemy->move.x = -sinf(g_pEnemy->rot.y + D3DX_PI) * 30.0f;
			g_pEnemy->move.y = 10.0f;
			g_pEnemy->move.z = -cosf(g_pEnemy->rot.y + D3DX_PI) * 30.0f;
		}

		//モーションのタイミングで角度調整
		if (g_pEnemy->nCntKey >= 5)
		{
			//角度
			AiRotEnemy();
		}
	}
	else
	{//他の状態
			
		//角度設定
		AiRotEnemy();

		//モーションに合わせて岩を設置する
		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 2)
		{
			SetRock(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 50.0f, g_pEnemy->aEnemyX[13].mtxWorld._43),
				D3DXVECTOR3(sinf(g_pEnemy->rot.y) * -SPEED_ROCK, 0.7f, cosf(g_pEnemy->rot.y)  * -SPEED_ROCK),
				g_pEnemy->rot,
				LIFE_ROCK,
				ROCKTYPE_THROW);
		}
	}
}

//===========================
//エネミーのAI(ジャンプ攻撃)
//===========================
void AiEnemyJumpAttack(void)
{
	//距離
	AiLengthEnemy();

	//視界
	AiSightEnemy();

	//攻撃
	AiAttackEnemy();
}

//===========================
//エネミーのAI(モーション)
//===========================
void AiEnemyMotion(void)
{
	//差分取得
	D3DXVECTOR3 posDiff = g_pEnemy->pos - GetPlayer()->pos;
	float fDiffLength;

	fDiffLength = D3DXVec3Length(&posDiff) * 0.5f;

	switch (g_pEnemy->motionState)
	{
	case ENEMYMOTION_NORMAL:
		//移動量を設定
		g_pEnemy->fSpeed = 0.0f;
		break;
	case ENEMYMOTION_MOVE:
		//移動量を設定
		g_pEnemy->fSpeed = ENEMY_SPEED;

		if (g_pEnemy->nCntKey == 1 || g_pEnemy->nCntKey == 6)
		{
			//移動量を設定
			g_pEnemy->fSpeed = ENEMY_SPEED * 1.5f;
		}

		if (g_pEnemy->nCntKey == 2 || g_pEnemy->nCntKey == 7)
		{
			//移動量を設定
			g_pEnemy->fSpeed = 0.0f;
			g_pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 10)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:足音
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[12].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[12].mtxWorld._43), PARTICLETYPE_SMOG);

			//画面振動
			SetQuake(40, (9.0f / g_pEnemy->fLength) + 0.005f);
		}

		if (g_pEnemy->nCntKey == 6 && g_pEnemy->nCntFrame == 10)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:足音
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), PARTICLETYPE_SMOG);

			//画面振動
			SetQuake(40, (9.0f / g_pEnemy->fLength) + 0.005f);
		}
		break;
	case ENEMYMOTION_SHAKE:
		//軌跡======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			// 軌跡の設定
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntKey <= 3)
		{
			//軌跡の位置設定
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 4 && g_pEnemy->nCntFrame == 0)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//軌跡の削除
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//軌跡======================

		//攻撃======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[13].mtxWorld._43), PARTICLETYPE_SMOG);

			//画面振動
			SetQuake(120, 0.05f);
		}
		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 0 && g_pEnemy->nCntFrame <= 20)
		{
			//パーティクル:衝撃波予告
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 600.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 600.0f)), 
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 30)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 600.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 600.0f)),
				30, SPEED_STEP);

			//画面振動
			SetQuake(120, 0.05f);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 20 && g_pEnemy->nCntFrame <= 40)
		{
			//パーティクル:衝撃波予告
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 800.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 800.0f)),
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 50)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 800.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 800.0f)),
				25, SPEED_STEP);

			//画面振動
			SetQuake(120, 0.05f);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 40 && g_pEnemy->nCntFrame <= 60)
		{
			//パーティクル:衝撃波予告
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f)), 
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 70)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f)),
				20, SPEED_STEP);

			//画面振動
			SetQuake(120, 0.05f);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 60 && g_pEnemy->nCntFrame <= 80)
		{
			//パーティクル:衝撃波予告
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f)), 
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 90)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f)),
				15, SPEED_STEP);

			//画面振動
			SetQuake(120, 0.05f);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_PIERCE:
		//軌跡======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			// 軌跡の設定
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntKey <= 3)
		{
			//軌跡の位置設定
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 4 && g_pEnemy->nCntFrame == 0)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//軌跡の削除
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//軌跡======================

		//攻撃======================
		if (g_pEnemy->nCntKey == 0 && g_pEnemy->nCntFrame == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[13].mtxWorld._43), 500, SPEED_PIERCE);

			//画面振動
			SetQuake(120, 0.05f);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_STEP:
		//攻撃======================
		if (g_pEnemy->nCntKey == 0 && g_pEnemy->nCntFrame == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 2)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:足音
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), PARTICLETYPE_SMOG);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), 500, SPEED_STEP);

			//画面振動
			SetQuake(120, 0.05f);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_THROW:
		//軌跡======================
		if (g_pEnemy->nCntKey == 0 && g_pEnemy->nCntFrame == 0)
		{
			// 軌跡の設定
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntKey <= 6)
		{
			//軌跡の位置設定
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 7 && g_pEnemy->nCntFrame == 0)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//軌跡の削除
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//軌跡======================

		//攻撃======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[13].mtxWorld._43), PARTICLETYPE_SMOG);
			SetRock(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 50.0f, g_pEnemy->aEnemyX[13].mtxWorld._43),
				D3DXVECTOR3(sinf(g_pEnemy->rot.y) * -SPEED_ROCK, 0.75f, cosf(g_pEnemy->rot.y)  * -SPEED_ROCK),
				g_pEnemy->rot,
				LIFE_ROCK,
				ROCKTYPE_THROW);

			//画面振動
			SetQuake(120, 0.03f);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_OVERLOOK:
		break;
	case ENEMYMOTION_DAMAGE:
		break;
	case ENEMYMOTION_DOWN:
		break;
	case ENEMYMOTION_BACKJUMP:
		//攻撃======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_JUMP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:ジャンプ
			PlaySound(SOUND_LABEL_SE_JUMP_001);
		}

		if (g_pEnemy->nCntKey == 4 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:着地
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[12].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[12].mtxWorld._43), PARTICLETYPE_SMOG);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_JUMPATTACK:
		//軌跡======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 0)
		{
			// 軌跡の設定
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 2 && g_pEnemy->nCntKey <= 9)
		{
			//軌跡の位置設定
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 9 && g_pEnemy->nCntFrame == 50)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//軌跡の削除
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//軌跡======================

		//移動======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 1)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_JUMP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:ジャンプ
			PlaySound(SOUND_LABEL_SE_JUMP_001);
			//モーションに合わせて移動量をたす
			g_pEnemy->move.x = sinf(g_pEnemy->rot.y + D3DX_PI) * (g_pEnemy->fLength * 0.13f);
			g_pEnemy->move.y = 25.0f;
			g_pEnemy->move.z = cosf(g_pEnemy->rot.y + D3DX_PI) * (g_pEnemy->fLength * 0.13f);
		}

		if (g_pEnemy->nCntKey == 3)
		{
			g_pEnemy->move.y = 0.0f;
		}

		if (g_pEnemy->nCntKey == 4)
		{
			g_pEnemy->move.y -= 5.0f;
		}
		//移動======================

		//攻撃======================
		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 5 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 280.0f), g_pEnemy->pos.y + 10.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 280.0f)), 500, SPEED_JUMP);
			
			//画面振動
			SetQuake(120, 0.06f);
		}

		if (g_pEnemy->nCntKey == 6 && g_pEnemy->nCntFrame == 20)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 8 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 280.0f), g_pEnemy->pos.y + 10.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 280.0f)), 500, SPEED_JUMP);
			
			//画面振動
			SetQuake(120, 0.05f);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_FIND:
		break;
	case ENEMYMOTION_DEATH:
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 35)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:足音
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[8].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[8].mtxWorld._43), PARTICLETYPE_SMOG);

			//画面振動
			SetQuake(120, 0.07f);
		}

		if (g_pEnemy->nCntKey == 6 && g_pEnemy->nCntFrame == 15)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:足音
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[8].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[8].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[11].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[11].mtxWorld._43), PARTICLETYPE_SMOG);

			//画面振動
			SetQuake(120, 0.07f);
		}

		if (g_pEnemy->nCntKey == 8 && g_pEnemy->nCntFrame == 20)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:足音
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//パーティクル:煙
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[0].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[0].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[1].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[1].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[2].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[1].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[3].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[3].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[5].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[5].mtxWorld._43), PARTICLETYPE_SMOG);

			//画面振動
			SetQuake(120, 0.07f);
		}
		break;
	case ENEMYMOTION_JUMPATTACK_LIGHT:
		//軌跡======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 0)
		{
			// 軌跡の設定
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 2 && g_pEnemy->nCntKey <= 7)
		{
			//軌跡の位置設定
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 7 && g_pEnemy->nCntFrame == 8)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//軌跡の削除
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//軌跡======================

		//移動======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 1)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_JUMP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:ジャンプ
			PlaySound(SOUND_LABEL_SE_JUMP_001);
			//モーションに合わせて移動量をたす
			g_pEnemy->move.x = sinf(g_pEnemy->rot.y + D3DX_PI) * (g_pEnemy->fLength * 0.06f);
			g_pEnemy->move.y = 15.0f;
			g_pEnemy->move.z = cosf(g_pEnemy->rot.y + D3DX_PI) * (g_pEnemy->fLength * 0.06f);
		}

		if (g_pEnemy->nCntKey == 3)
		{
			g_pEnemy->move.y = 0.0f;
		}

		if (g_pEnemy->nCntKey == 4)
		{
			g_pEnemy->move.y -= 5.0f;
		}
		//移動======================

		//攻撃======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 5 && g_pEnemy->nCntFrame == 0)
		{
			//差分の量によって音量を変える
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//効果音:攻撃
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//衝撃波
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 280.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 280.0f)), 500, SPEED_JUMP);
			
			//画面振動
			SetQuake(120, 0.06f);
		}
		//攻撃======================
		break;
	case ENEMYMOTION_SLEEP:
		if (g_pEnemy->nHitRock <= 1)
		{
			if (g_pEnemy->nCntFrame % 400 == 200)
			{
				//効果音:攻撃予告
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//画面振動
				SetQuake(600, 0.005f);
			}
		}
		else if (g_pEnemy->nHitRock <= 2)
		{
			if (g_pEnemy->nCntFrame % 200 == 100)
			{
				//効果音:攻撃予告
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//画面振動
				SetQuake(600, 0.01f);
			}
		}
		else if (g_pEnemy->nHitRock <= 3)
		{
			if (g_pEnemy->nCntFrame % 100 == 50)
			{
				//効果音:攻撃予告
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//画面振動
				SetQuake(600, 0.01f);
			}
		}
		else
		{
			if (g_pEnemy->nCntFrame % 50 == 25)
			{
				//効果音:攻撃予告
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//画面振動
				SetQuake(600, 0.02f);
			}
		}
		break;
	case ENEMYMOTION_STANDUP:
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntFrame % 10 == 0)
		{
			//効果音:攻撃予告
			PlaySound(SOUND_LABEL_SE_ATTACK_000);
		}

		if (g_pEnemy->nCntKey >= 1)
		{
			//画面振動
			SetQuake(600, 0.025f);
		}
		break;
	default:
		break;
	}
}

//===========================
//エネミーのAI(視界)
//===========================
void AiSightEnemy(void)
{
	Player *pPlayer = GetPlayer();
	Object *pObject = ObjectGet();
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//外積計算用変数
	D3DXVECTOR3 posObj[4] = {};														//外積計算用変数
	float vecTemp, vecMax = 0, vecMin = 0, vecObjL, vecObjR, vecObjLength;			//外積計算結果格納用変数
	int nNumR = -1, nNumL = -1;														//番号保存用変数
	
	//発見時間増加処理
	g_pEnemy->nFindCount++;

	//エネミーの視界を求める計算
	pos.x = g_pEnemy->pos.x + sinf(g_pEnemy->rot.y + D3DX_PI - (D3DX_PI * ENEMY_SIGHT));
	pos.z = g_pEnemy->pos.z + cosf(g_pEnemy->rot.y + D3DX_PI - (D3DX_PI * ENEMY_SIGHT));

	//エネミーから見て左側の外積
	g_pEnemy->vecL = OuterProduct(g_pEnemy->pos, pPlayer->pos, pos);

	pos.x = g_pEnemy->pos.x + sinf(g_pEnemy->rot.y + D3DX_PI + (D3DX_PI * ENEMY_SIGHT));
	pos.z = g_pEnemy->pos.z + cosf(g_pEnemy->rot.y + D3DX_PI + (D3DX_PI * ENEMY_SIGHT));

	//エネミーから見て右側の外積
	g_pEnemy->vecR = OuterProduct(g_pEnemy->pos, pPlayer->pos, pos);

	//視界内かの判定
	if (g_pEnemy->vecL <= 0.0f && g_pEnemy->vecR >= 0.0f)
	{
		//発見状態
		g_pEnemy->bFind = true;

		//障害物判定
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (pObject->bUse == true)
			{
				for (int nCntOut = 0; nCntOut < 4; nCntOut++)
				{
					//オブジェクトの外周の点を設定
					posObj[nCntOut].x = pObject->point[nCntOut].x;
					posObj[nCntOut].z = pObject->point[nCntOut].z;

					//エネミーとオブジェクトの外積
					vecTemp = OuterProduct(g_pEnemy->pos, pObject->pos, posObj[nCntOut]);

					//最大の外積を求め1番右にある点を保存する
					if (vecTemp >= vecMax)
					{
						vecMax = vecTemp;
						nNumR = nCntOut;
					}

					//最小の外積を求め1番左にある点を保存する
					if (vecTemp <= vecMin)
					{
						vecMin = vecTemp;
						nNumL = nCntOut;
					}
				}

				//エネミーから見てオブジェクトの右端より左にいるかの判定
				vecObjL = OuterProduct(g_pEnemy->pos, pPlayer->pos, posObj[nNumL]);

				//エネミーから見てオブジェクトの左端より右にいるかの判定
				vecObjR = OuterProduct(g_pEnemy->pos, pPlayer->pos, posObj[nNumR]);

				//エネミーから見てオブジェクトの手前か奥かの判定
				vecObjLength = OuterProduct(posObj[nNumR], pPlayer->pos, posObj[nNumL]);

				//ブロックの影にいるかの判定
				if (vecObjL <= 0.0f && vecObjR >= 0.0f && vecObjLength <= 0.0f && pPlayer->pos.y <= pObject->pos.y + pObject->vtxMax.y && pPlayer->pos.y >= pObject->pos.y + pObject->vtxMin.y)
				{
					//オブジェクトの状態判定
					if (pObject->nType == OBJECTTYPE_SANDPOLE || pObject->nType == OBJECTTYPE_SANDPOLE_BASE || pObject->nType == OBJECTTYPE_REFLECTOR_BASE)
					{//砂柱本体と砂柱と反射板の土台の時

						//判定しない
					}
					else 
					{//その他

						//発見判定取り消し
						g_pEnemy->bFind = false;
					}

					//計算用変数初期化
					vecMax = 0.0f;
					vecMin = 0.0f;
					nNumL = 0;
					nNumR = 0;

					break;
				}
			}

			//計算用変数初期化
			vecMax = 0.0f;
			vecMin = 0.0f;
			nNumL = 0;
			nNumR = 0;

			pObject++;
		}
	}
	else
	{
		//発見判定取り消し
		g_pEnemy->bFind = false;
	}

	//距離が一定以内の時状況に関わらず追跡状態
	if(g_pEnemy->fLength <= ENEMY_LENGTH)
	{
		//発見判定
		g_pEnemy->bFind = true;
	}

	//追跡状態時カウントリセット処理
	if (g_pEnemy->bFind == true)
	{
		if (g_pEnemy->state == ENEMYSTATE_SEARCH || g_pEnemy->state == ENEMYSTATE_NORMAL)
		{
			SetFadeSound(SOUND_LABEL_BGM001, SOUND_LABEL_BGM002,0.2f);
		}

		g_pEnemy->nFindCount = 0;
		g_pEnemy->nTimerMove = 0;
	}

	//カウントの数値によって状態を変更する処理
	if (g_pEnemy->nFindCount == 0)
	{
		//エネミーの状態を追跡状態に変更
		g_pEnemy->state = ENEMYSTATE_TRACK;
	}
	else if (g_pEnemy->nFindCount < 200)
	{
		//エネミーの状態を見失った状態に変更
		g_pEnemy->state = ENEMYSTATE_LOST;

		//抜け出す時スコア加算
		if (g_pEnemy->nFindCount == 200 - 1)
		{
			AddScore(SCORE_LOSTFIND);

			SetFadeSound(SOUND_LABEL_BGM002, SOUND_LABEL_BGM001, 0.02f);

			//取得スコア表示設定
			SetPointScore(SCORE_LOSTFIND, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f,LOGTYPE_LOST);
		}
	}
	else if (g_pEnemy->nFindCount < 500)
	{
		//エネミーの状態を索敵状態に変更
		g_pEnemy->state = ENEMYSTATE_SEARCH;
	}
	else
	{
		//エネミーの状態を通常状態に変更
		g_pEnemy->state = ENEMYSTATE_NORMAL;
		g_pEnemy->nFindCount = 800;
	}

	if (g_pEnemy->motionState == ENEMYMOTION_SHAKE ||
		g_pEnemy->motionState == ENEMYMOTION_PIERCE ||
		g_pEnemy->motionState == ENEMYMOTION_STEP ||
		g_pEnemy->motionState == ENEMYMOTION_THROW)
	{
		//エネミーの状態を索敵状態に変更
		g_pEnemy->state = ENEMYSTATE_TRACK;
	}
	else if (g_pEnemy->motionState == ENEMYMOTION_JUMPATTACK || g_pEnemy->motionState == ENEMYMOTION_JUMPATTACK_LIGHT)
	{
		//エネミーの状態を索敵状態に変更
		g_pEnemy->state = ENEMYSTATE_JUMPATTACK;
	}
	else
	{
		if (g_pEnemy->bFind == true && 
			(g_pEnemy->stateOld == ENEMYSTATE_NORMAL || g_pEnemy->stateOld == ENEMYSTATE_SEARCH))
		{
			g_pEnemy->state = ENEMYSTATE_FIND;
		}
	}
	
	//得点の減少、増加
	if (g_pEnemy->state == ENEMYSTATE_TRACK)
	{
		g_nCntFind++;

		if (g_nCntFind > ENEMY_FIND_COUNTER)
		{
			//見つかった
			//AddScore(-1);
		}
	}
	else if (g_pEnemy->state == ENEMYSTATE_SEARCH)
	{
		g_nCntFind--;
	}
	else
	{
		g_nCntFind = 0;
	}
}

//===========================
//エネミーのAI(移動地点)
//===========================
void AiPointEnemy(void)
{
	float fLengthPoint;								//距離判定用変数
	int nCntPoint = 0;								//ポイント記録用変数

	//状態でタイマー処理変更
	if (g_pEnemy->state == ENEMYSTATE_SEARCH)
	{
		g_pEnemy->nTimerMove = 0;
		g_pEnemy->nFindCount = 200;
	}
	else
	{
		g_pEnemy->nTimerMove++;
	}

	//ポイントとエネミーの距離計算
	fLengthPoint = hypotf(g_pEnemy->MovePattern[g_pEnemy->nPattern].point[g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount].x - g_pEnemy->pos.x, g_pEnemy->MovePattern[g_pEnemy->nPattern].point[g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount].z - g_pEnemy->pos.z);

	//到達判定
	if (fLengthPoint < ENEMY_ARRIVAL_POINT)
	{
		//ポイントの更新
		g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount++;

		if (g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount == 8)
		{
			g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount = 0;
		}

		//移動時間リセット
		g_pEnemy->nTimerMove = 0;

		//エネミーの状態を索敵状態に変更
		g_pEnemy->state = ENEMYSTATE_SEARCH;
		g_pEnemy->nFindCount = 200;
	}

	//引っ掛かり防止
	if (g_pEnemy->nTimerMove >= ENEMY_TIME)
	{//移動時間が一定時間をこえた時移動のポイントを変更
		g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount = g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount + 1;

		if (g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount == 8)
		{
			g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount = 0;
		}

		//移動時間リセット
		g_pEnemy->nTimerMove = 0;
	}

	//移動地点保存
	g_pEnemy->pointMove = g_pEnemy->MovePattern[g_pEnemy->nPattern].point[g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount];
}

//===========================
//エネミーのAI(距離)
//===========================
void AiLengthEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//プレイヤーとエネミーの距離計算
	g_pEnemy->fLength = hypotf(pPlayer->pos.x - g_pEnemy->pos.x, pPlayer->pos.z - g_pEnemy->pos.z);
}

//===========================
//エネミーのAI(角度)
//===========================
void AiRotEnemy(void)
{
	float fRotMove, fRotDest, fRotDiff;				//角度調整用変数

	//角度調整
	fRotMove = g_pEnemy->rot.y;
	fRotDest = atan2f(g_pEnemy->pointMove.x - g_pEnemy->pos.x, g_pEnemy->pointMove.z - g_pEnemy->pos.z);
	fRotDiff = fRotDest - fRotMove + D3DX_PI;

	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= (D3DX_PI * 2);
	}
	else if (fRotDiff <= -D3DX_PI)
	{
		fRotDiff += (D3DX_PI * 2);
	}

	//差分追加
	fRotMove += fRotDiff * 0.1f;

	//角度一致判定
	if (fRotDiff >= 3.13f || fRotDiff <= -3.13f)
	{
		g_pEnemy->fSpeed = 0.0f;
	}

	if (fRotMove > D3DX_PI)
	{
		fRotMove -= (D3DX_PI * 2);
	}
	else if (fRotMove <= -D3DX_PI)
	{
		fRotMove += (D3DX_PI * 2);
	}

	g_pEnemy->rot.y = fRotMove;

	if (g_pEnemy->rot.y > D3DX_PI)
	{
		g_pEnemy->rot.y -= (D3DX_PI * 2);
	}
	else if (g_pEnemy->rot.y <= -D3DX_PI)
	{
		g_pEnemy->rot.y += (D3DX_PI * 2);
	}
}

//===========================
//エネミーのAI(移動)
//===========================
void AiMoveEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//移動ポイントをプレイヤーに設定
	g_pEnemy->pointMove = pPlayer->pos;

	//距離が近いと止まるそれ以外だと動く
	if (g_pEnemy->fLength <= ENEMY_LENGTH && g_pEnemy->nAttackCount < g_pEnemy->nAttackTime)
	{
		g_pEnemy->fSpeed = 0.0f;
		SetMotionEnemy(ENEMYMOTION_NORMAL);
	}
	else if (g_pEnemy->state == ENEMYSTATE_LOST)
	{
		g_pEnemy->fSpeed -= 0.002f;

		if (g_pEnemy->fSpeed < 0.0f)
		{
			g_pEnemy->fSpeed = 0.0f;
		}
	}
	else
	{
		
	}

	if (CollisionEnemyPoint(&g_pEnemy->pos) == true && g_pEnemy->nAttackCount < g_pEnemy->nAttackTime)
	{
		g_pEnemy->fSpeed = 0.0f;
		SetMotionEnemy(ENEMYMOTION_NORMAL);
	}
}

//===========================
//エネミーのAI(攻撃)
//===========================
void AiAttackEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//一定時間で攻撃
	if (g_pEnemy->nAttackCount >= g_pEnemy->nAttackTime && (g_pEnemy->motionState == ENEMYMOTION_MOVE || g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_OVERLOOK))
	{
		int nAttack = rand() % 101;
		
		//キーとフレームをリセット
		g_pEnemy->nCntKey = 0;
		g_pEnemy->nCntFrame = 0;

		//距離と乱数で攻撃モーションを決定
		if (g_pEnemy->fLength <= ENEMY_LENGTH)
		{
			if (nAttack > 50)
			{//振り下ろしモーション
				SetMotionEnemy(ENEMYMOTION_SHAKE);
			}
			else if (nAttack > 30)
			{//突き刺しモーション
				SetMotionEnemy(ENEMYMOTION_PIERCE);
			}
			else
			{//足踏みモーション
				SetMotionEnemy(ENEMYMOTION_STEP);
			}
		}
		else if (g_pEnemy->fLength <= ENEMY_LENGTH * 5.0f && CollisionEnemyPoint(&g_pEnemy->pos) == true)
		{
			if (nAttack > 60)
			{//突き刺しモーション
				SetMotionEnemy(ENEMYMOTION_PIERCE);
			}
			else if (nAttack > 20)
			{//振り下ろしモーション
				SetMotionEnemy(ENEMYMOTION_SHAKE);
			}
			else
			{//岩飛ばしモーション
				SetMotionEnemy(ENEMYMOTION_THROW);
			}
		}
		else if (g_pEnemy->fLength <= ENEMY_LENGTH * 5.0f)
		{
			if (nAttack > 50)
			{//突き刺しモーション
				SetMotionEnemy(ENEMYMOTION_PIERCE);
			}
			else if (nAttack > 30)
			{//振り下ろしモーション
				SetMotionEnemy(ENEMYMOTION_SHAKE);
			}
			else if (nAttack > 10)
			{//岩飛ばしモーション
				SetMotionEnemy(ENEMYMOTION_THROW);
			}
			else
			{//ジャンプ攻撃モーション
				if (g_pEnemy->nLife <= (int)(ENEMY_LIFE * 0.25f))
				{
					SetMotionEnemy(ENEMYMOTION_JUMPATTACK);
					g_pEnemy->state = ENEMYSTATE_JUMPATTACK;
				}
				else
				{
					SetMotionEnemy(ENEMYMOTION_JUMPATTACK_LIGHT);
					g_pEnemy->state = ENEMYSTATE_JUMPATTACK;
				}
			}
		}
		else if (CollisionEnemyPoint(&g_pEnemy->pos) == true)
		{
			//岩飛ばしモーション
			SetMotionEnemy(ENEMYMOTION_THROW);
		}
		else
		{
			if (nAttack > 70)
			{//岩飛ばしモーション
				SetMotionEnemy(ENEMYMOTION_THROW);
			}
			else
			{//ジャンプ攻撃モーション
				if (g_pEnemy->nLife <= (int)(ENEMY_LIFE * 0.25f))
				{
					SetMotionEnemy(ENEMYMOTION_JUMPATTACK);
					g_pEnemy->state = ENEMYSTATE_JUMPATTACK;
				}
				else
				{
					SetMotionEnemy(ENEMYMOTION_JUMPATTACK_LIGHT);
					g_pEnemy->state = ENEMYSTATE_JUMPATTACK;
				}
			}
		}
	}
}

//===========================
//モーションの設定
//===========================
void SetMotionEnemy(ENEMYMOTION motion)
{
	Player *pPlayer = GetPlayer();

	g_pEnemy->motionState = motion;

	//攻撃モーション一覧
	if (motion == ENEMYMOTION_SHAKE ||
		motion == ENEMYMOTION_PIERCE ||
		motion == ENEMYMOTION_STEP ||
		motion == ENEMYMOTION_THROW ||
		motion == ENEMYMOTION_JUMPATTACK || 
		motion == ENEMYMOTION_JUMPATTACK_LIGHT)
	{
		pPlayer->nIdxBillboard = SetBillboard(BILLBOARDTYPE_ALERT, pPlayer->pos);
	}
}