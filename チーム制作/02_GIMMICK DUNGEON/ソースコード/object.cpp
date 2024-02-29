//========================================
//
//3Dゲーム(オブジェクト処理)[object.cpp]
//Author:佐久間優香,中村陸(一部)
//
//========================================

//=============================
// インクルード
//=============================
#include "main.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "edit.h"
#include "rock.h"
#include "billboard.h"
#include "ceiling.h"
#include "enemy.h"
#include "stdio.h"
#include "score.h"
#include "shadow.h"
#include "particle.h"
#include "tutorial.h"
#include "effect.h"
#include "sound.h"
#include "collision.h"
#include "game.h"
#include "point.h"
#include "count.h"
#include "model.h"

//=============================
// マクロ定義
//=============================
// オブジェクト全般
#define NUM_BLOCK	(10)	//テクスチャの数
#define ROT_SPEED (0.04f)	//回転する速度
#define ROCK_LIMIT		(5)		//ストッパーがかかるまでの壊れた岩の数
#define OBJ_GLAVITY		(0.48f)	//オブジェクトにかかる重力
#define SET_SHADOW	(-1)	//影の設定
#define SHADOW_SCALE_CCL		(1.4f)	//影に渡す倍率(丸)
#define SHADOW_SCALE_SQR		(1.8f)	//影に渡す倍率(矩形)
#define COLLISION_RATE		(1.1f)	//当たり判定の倍率(忖度分)
#define SHIFT_SOUND_TIME	(5)	//ずらす音がなる頻度
#define DAMAGE_SCALE	(0.3f)	//ダメージが発生する判定サイズ

//反射板
#define REF_LENGTH			(29.0f)	//リフレクターとプレイヤーの距離

//発射台
#define INJECT_TIME	(300)	//岩が発射される間隔

//レバー
#define MOVE_WINDLEVER (0.2f)	//風の移動量

//ボタン
#define BUTTON_BILLBOARD (80.0f)	//ビルボードの+y座標
#define BUTTON_PUSH_DEPTH	(50.0f)	//ボタンの沈み込む量

// 砂の柱
#define POLE_SCALE_SPEED	(0.001f)	//砂の柱の成長速度
#define SANDPOLE_DAMAGE		(150)	//石柱が与えるダメージ
#define ROTATE_SPEED		(0.05f)	//砂の柱が倒れるスピード
#define COLLISION_HEIGHT	(250.0f)	//砂柱のオフセットの高さ
#define SAND_POLE_SCORE		(2200)		//砂の柱のスコア
#define PARTICLE_TIME		(2)	//パーティクルを出す頻度
#define SANDPOLE_FRAG		(20)	//破片の数

// 石柱
#define STONE_SCALE_SPEED	(0.0007f)	//石柱の成長速度
#define COLLISION_BELOW		(100.0f)	//石柱のオフセットの低さ
#define STONEPOLE_DAMAGE			(150)	//石柱が与えるダメージ
#define FALL_SPEED			(2.0f)	//石柱の落下速度
#define POLE_SCORE			(2500)	//石柱のスコア
#define STONE_POLE_RADIUS	(100.0f)	//石柱の当たり判定の半径
#define STONEPOLE_FRAG		(10)	//破片の数

// 岩の欠片
#define ROCKFRAG_SCALE_SPEED	(0.006f)	//破片のサイズ減少速度
#define ROCKFRAG_SPEED	(2.0f)	//破片のランダム基準速度
#define ROCKFRAG_GLAVITY		(0.1f)	//オブジェクトにかかる重力
#define FIELD_REFLECT		(0.6f)	//地面ではねた際のmoveへの反射力

//ランキングで使用
#define SETOBJ_ROOP		(50)	//ランキングでのセットフレーム数
#define SETOBJ_PERCENT		(100)	//ランキングでの確率
#define RANKING_SETOBJ		(D3DXVECTOR3( 1000.0f, 0.0f, 200.0f ))	//ランキング時のセット位置

#define SKULL_PER (15.0f)	//頭蓋骨の確率
#define SKULLMT_PER (15.0f + SKULL_PER)	//頭蓋骨の山の確率
#define STATUE_1_PER (5.0f + SKULLMT_PER)	//銅像(壊れ)の確率
#define STATUE_3_PER (5.0f + STATUE_1_PER)	//銅像(ルネサンス)の確率
#define STATUE_4_PER (5.0f + STATUE_3_PER)	//銅像(王)の確率
#define STATUE_5_PER (5.0f + STATUE_4_PER)	//銅像()の確率
#define STATUE_6_PER (5.0f + STATUE_5_PER)	//銅像()の確率
#define STATUE_7_PER (5.0f + STATUE_6_PER)	//銅像()の確率
#define STATUE_8_PER (5.0f + STATUE_7_PER)	//銅像()の確率
#define TREASUREBOX_PER (15.0f + STATUE_8_PER)	//宝箱の確率
#define STATUE_100_PER (2.0f + TREASUREBOX_PER)	//銅像(現場猫)の確率
#define STATUE_101_PER (2.0f + STATUE_100_PER)	//銅像(ジャックオー)の確率
//#define SWORD_PER (15.0f + STATUE_101_PER)	//剣の確率

//=============================
// 列挙型宣言
//=============================
//ボタンの状態
typedef enum
{
	BUTONSTATE_NONE = 0,	//何もしていない状態
	BUTONSTATE_FORWARD,		//前進状態
	BUTONSTATE_WAIT,	//待機状態
	BUTONSTATE_MAX
}BUTTONSTATE;

//=============================
// プロトタイプ宣言
//=============================
void StateChangeObject(void);						//状態の変更
void SetRockObject(int nCntObject);			//岩が一定に出る処理
void BillboardUse(Object *pObject);
void RotNormalize(D3DXVECTOR3 *pRot);
void ButtonMove(int nCntObject);
void ButtonPoleMove(int nCntObject);
void CollisionRockObject(int nCntObject);
void UpdatePosOffset(int nCntObject);
void UpdateRockFrag(int nCntObject);

//=============================
// グローバル変数宣言
//=============================
Object g_Object[MAX_OBJECT];
D3DXMATERIAL g_typeMat[COLTYPE_MAX] = {};				//使用色の管理
BUTTONSTATE g_buttonState;								//ボタンの状態
BUTTONSTATE g_buttonStatePole;							//落石ボタンの状態
bool g_bOpenRock;			//岩を使用しているかどうか
bool g_bPoleAttack;			//落石を使用しているかどうか
int g_nRoopSetTime;			//ランキング時のオブジェクト生成のループ時間
bool g_bButtonSound;

//========================================
//オブジェクトの初期化処理
//========================================
void InitObject(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			
	
	//変数宣言
	int nCntObject;

	g_bOpenRock = true;			//岩を使用していない
	g_bPoleAttack = true;			//岩を使用していない
	g_buttonState = BUTONSTATE_NONE;
	g_buttonStatePole = BUTONSTATE_FORWARD;
	g_nRoopSetTime = 0;

	//各種変数の初期化
	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		ZeroMemory(&g_Object[nCntObject],sizeof(Object));
		g_Object[nCntObject].nIdxshadow = SET_SHADOW;
	}

	//色の用意(初期化)
	{
		g_typeMat[COLTYPE_NORMAL].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_NORMAL].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_NORMAL].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_typeMat[COLTYPE_DAMAGE].MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_DAMAGE].MatD3D.Ambient = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_DAMAGE].MatD3D.Emissive = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);

		g_typeMat[COLTYPE_DEATH].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_DEATH].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_DEATH].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_typeMat[COLTYPE_EDIT].MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_EDIT].MatD3D.Ambient = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_EDIT].MatD3D.Emissive = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);

		g_typeMat[COLTYPE_DELETETARGET].MatD3D.Diffuse = D3DXCOLOR(0.9f, 0.0f, 0.0f, 0.8f);
		g_typeMat[COLTYPE_DELETETARGET].MatD3D.Ambient = D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f);
		g_typeMat[COLTYPE_DELETETARGET].MatD3D.Emissive = D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f);
	}
}

//========================================
// オブジェクトの終了処理
//========================================
void UninitObject(void)
{
}

//========================================
// オブジェクトの更新処理
//========================================
void UpdateObject(void)
{
	//情報取得
	int *pHitRockCouter = GetHitRockNum();
	Enemy *pEnemy = GetEnemy();
	
	//差分取得
	D3DXVECTOR3 posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDiffLength = 0.0f;

	if (*pHitRockCouter >= ROCK_LIMIT)
	{//一定数岩が壊れたタイミングでストッパーがかかる

		g_buttonState = BUTONSTATE_FORWARD;

		g_bOpenRock = false;

		*pHitRockCouter = 0;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{//使用されている状態なら
			//前回の位置を保存
			g_Object[nCntObject].posOld = g_Object[nCntObject].pos;

			posDiff = g_Object->pos - GetPlayer()->pos;
			fDiffLength = D3DXVec3Length(&posDiff);

			if (g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR &&
				g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR_BASE &&
				g_Object[nCntObject].nType != OBJECTTYPE_SANDPOLE_BASE)
			{//当たり判定を行わないオブジェクトのみ行う
				if (g_Object[nCntObject].rotate.x == 0)
				{// 回転してないときに判定する
					//岩との当たり判定
					CollisionRockObject(nCntObject);
				}
			}

			//回転量の代入
			g_Object[nCntObject].rot += g_Object[nCntObject].rotate;

			//移動量の加算
			g_Object[nCntObject].pos += g_Object[nCntObject].move;

			switch (g_Object[nCntObject].nType)
			{
			case OBJECTTYPE_CATAPULT:
				//岩が一定に出る
				if (GetGameState() != GAMESTATE_END)
				{
					SetRockObject(nCntObject);
				}
				break;
			case OBJECTTYPE_REFLECTOR:
				
				break;
			case OBJECTTYPE_LEVER_BAR:

				break;
			case OBJECTTYPE_TORCH:
				//ビルボードの位置設定処理
				SetPositionBillboard(g_Object[nCntObject].nIdxBillboard, D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 80.0f, g_Object[nCntObject].pos.z));
				break;
			case OBJECTTYPE_BUTTON:
				//ボタンの更新
				ButtonMove(nCntObject);
				break;
			case OBJECTTYPE_BUTTON_POLE:
				//ボタンの更新
				ButtonPoleMove(nCntObject);
				break;
			case OBJECTTYPE_ROCK_FRAG:
				//岩の欠片の更新
				UpdateRockFrag(nCntObject);
				break;

				//砂の柱========================================
			case OBJECTTYPE_SANDPOLE:

				if (g_Object[nCntObject].nCounterEff % PARTICLE_TIME == 0)
				{//一定間隔でパーティクル設定
					//パーティクルの設定
					SetParticle(D3DXVECTOR3(g_Object[nCntObject].pos.x,
						g_Object[nCntObject].pos.y + 500.0f,
						g_Object[nCntObject].pos.z),
						PARTICLETYPE_SANDPOLE);

					//カウンターのリセット
					g_Object[nCntObject].nCounterEff = 0;
				}

				//エフェクトのカウンター加算
				g_Object[nCntObject].nCounterEff++;

				//スケールの拡大
				g_Object[nCntObject].fScale += POLE_SCALE_SPEED;

				if (g_Object[nCntObject].fScale >= 1.0f)
				{//スケールの制限
					g_Object[nCntObject].fScale = 1.0f;
				}

				if (g_Object[nCntObject].rot.x >= D3DX_PI * 0.5f)
				{//倒れきったら初期状態に戻す

					//差分の量によって音量を変える
					SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);

					//破壊される音の再生
					PlaySound(SOUND_LABEL_SE_CRASH);

					//破片の設定
					SetBreakObj(
						D3DXVECTOR3
						(
							g_Object[nCntObject].mtxWorldCollision._41,
							g_Object[nCntObject].mtxWorldCollision._42,
							g_Object[nCntObject].mtxWorldCollision._43
						), 
						SANDPOLE_FRAG);
					
					//デフォルトの状態に戻す
					g_Object[nCntObject].rot.x = 0;
					g_Object[nCntObject].rotate = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
					g_Object[nCntObject].fScale = 0.0f;
				}

				//敵との当たり判定
				if (g_Object[nCntObject].rotate.x != 0)
				{//回転している時
					//位置の差分を取得
					D3DXVECTOR3 PosDiff = pEnemy->pos - D3DXVECTOR3(g_Object[nCntObject].mtxWorldCollision._41, g_Object[nCntObject].mtxWorldCollision._42, g_Object[nCntObject].mtxWorldCollision._43);

					//差分の距離を取得
					float fLength = D3DXVec3Length(&PosDiff);
					float fRadEnemy = ENEMY_RADIUS;

					//±の補正
					fLength = fLength * fLength;
					fRadEnemy = fRadEnemy * fRadEnemy;

					if (fLength <= fRadEnemy + g_Object[nCntObject].vtxMax.x * g_Object[nCntObject].vtxMax.x)
					{
						//差分の量によって音量を変える
						SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

						//攻撃音再生
						PlaySound(SOUND_LABEL_SE_ATTACK_000);

						//破片設定
						SetBreakObj(g_Object[nCntObject].pos, SANDPOLE_FRAG);

						if (g_Object[nCntObject].fScale > DAMAGE_SCALE)
						{
							//敵ヒット処理
							HitEnemy((int)(SANDPOLE_DAMAGE * g_Object[nCntObject].fScale), 0);
							//スコアの加算
							AddScore(SAND_POLE_SCORE);
							//取得スコア表示設定
							SetPointScore(SAND_POLE_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITSANDPOLE);
						}
						else
						{
							HitEnemy(0, 0);
						}

						//球の判定がぶつかったら初期状態に戻す
						g_Object[nCntObject].rot.x = 0;
						g_Object[nCntObject].rotate = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
						g_Object[nCntObject].fScale = 0.0f;
					}

				}

				//オフセットの位置設定
				UpdatePosOffset(nCntObject);
				break;
				//砂の柱========================================

				//石柱========================================
			case OBJECTTYPE_STONEPOLE:
				
				//オフセットの位置設定
				UpdatePosOffset(nCntObject);

				//スケールの拡大
				g_Object[nCntObject].fScale += STONE_SCALE_SPEED;

				if (g_Object[nCntObject].fScale >= 1.0f)
				{//スケールの制限
					g_Object[nCntObject].fScale = 1.0f;
				}

				if (g_Object[nCntObject].move.y != 0)
				{//移動量の加算
					g_Object[nCntObject].move.y -= OBJ_GLAVITY;
				}

				//敵との当たり判定==========
				//位置の差分を取得
				D3DXVECTOR3 PosDiff =
					pEnemy->pos -
					D3DXVECTOR3
					(
						g_Object[nCntObject].mtxWorldCollision._41,
						g_Object[nCntObject].mtxWorldCollision._42,
						g_Object[nCntObject].mtxWorldCollision._43
					);

				//差分の距離を取得
				float fLength = D3DXVec3Length(&PosDiff);
				float fRadEnemy = ENEMY_RADIUS;

				//±の補正
				fLength = fLength * fLength;
				fRadEnemy = fRadEnemy * fRadEnemy;

				if (g_Object[nCntObject].move.y != 0)
				{
					if (fLength <= fRadEnemy + STONE_POLE_RADIUS)
					{
						//差分の量によって音量を変える
						SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

						//攻撃音再生
						PlaySound(SOUND_LABEL_SE_ATTACK_000);

						//破片設定
						SetBreakObj(g_Object[nCntObject].pos, STONEPOLE_FRAG);

						if (g_Object[nCntObject].fScale > DAMAGE_SCALE)
						{
							//敵ヒット処理
							HitEnemy((int)(STONEPOLE_DAMAGE * g_Object[nCntObject].fScale), 0);
							//スコアの増加
							AddScore(POLE_SCORE);
							//取得スコア表示設定
							SetPointScore(POLE_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITSTONE);
						}
						else
						{
							HitEnemy(0, 0);
						}

						//球の判定がぶつかったら初期状態に戻す
						g_Object[nCntObject].pos = g_Object[nCntObject].posBlock;
						g_Object[nCntObject].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
						g_Object[nCntObject].fScale = 0.0f;
					}
					else if (g_Object[nCntObject].mtxWorldCollision._42 < 0)
					{//石柱が落ちきったらもとに戻す

						//差分の量によって音量を変える
						SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);

						//破壊される音の再生
						PlaySound(SOUND_LABEL_SE_CRASH);

						//破片の設定
						SetBreakObj(g_Object[nCntObject].pos, STONEPOLE_FRAG);

						//デフォルトの状態に戻す
						g_Object[nCntObject].pos = g_Object[nCntObject].posBlock;
						g_Object[nCntObject].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
						g_Object[nCntObject].fScale = 0.0f;
					}
				}
				
				//====================

				break;
				//石柱========================================
			}

			//判定基準点の設定
			SetPoint(nCntObject);

			//状態の変更
			StateChangeObject();

			//方向の正常化
			RotNormalize(&g_Object[nCntObject].rot);

			if (g_Object[nCntObject].nType == OBJECTTYPE_ROCK_FRAG)
			{
				//影の位置設定
				D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				rotShadow.y = g_Object[nCntObject].rot.y;

				SetPositionShadow(g_Object[nCntObject].nIdxshadow,
					D3DXVECTOR3(g_Object[nCntObject].pos.x, 0.1f, g_Object[nCntObject].pos.z),
					rotShadow,
					g_Object[nCntObject].fScale);
			}
			else
			{
				//影の位置設定
				D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				rotShadow.y = g_Object[nCntObject].rot.y;

				SetPositionShadow(g_Object[nCntObject].nIdxshadow,
					D3DXVECTOR3(g_Object[nCntObject].pos.x, 0.1f, g_Object[nCntObject].pos.z),
					rotShadow,
					1.0f);
			}


		}
	}
}

//========================================
//オブジェクトの描画処理
//========================================
void DrawObject(void)
{
	//情報取得
	Model *pModelObject = GetModelobject();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;						//計算用マトリックス
	D3DMATERIAL9 matDef;								//現在のマテリアル保存用
	D3DXMATERIAL *pMat;									//マテリアルデータへのポインタ
	
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Object[nCntObject].mtxWorld);

			if (g_Object[nCntObject].nType == OBJECTTYPE_SANDPOLE
				|| g_Object[nCntObject].nType == OBJECTTYPE_STONEPOLE
				|| g_Object[nCntObject].nType == OBJECTTYPE_ROCK_FRAG)
			{//スケールの変更
				g_Object[nCntObject].mtxWorld._11 = g_Object[nCntObject].fScale;
				g_Object[nCntObject].mtxWorld._22 = g_Object[nCntObject].fScale;
				g_Object[nCntObject].mtxWorld._33 = g_Object[nCntObject].fScale;
			}

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object[nCntObject].rot.y, g_Object[nCntObject].rot.x, g_Object[nCntObject].rot.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld, &g_Object[nCntObject].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y, g_Object[nCntObject].pos.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld, &g_Object[nCntObject].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCntObject].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)pModelObject[g_Object[nCntObject].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pModelObject[g_Object[nCntObject].nType].dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				switch (g_Object[nCntObject].state)
				{
				case OBJECTSTATE_NORMAL:
					if (g_Object[nCntObject].nIdxEdit == GetSelectIdxEdit())
					{
						pDevice->SetMaterial(&g_typeMat[COLTYPE_DELETETARGET].MatD3D);
					}
					else
					{
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}
					break;

				case OBJECTSTATE_DAMEGE:
					pDevice->SetMaterial(&g_Object[nCntObject].RedMat.MatD3D);
					break;
				}
				//テクスチャの設定
				pDevice->SetTexture(0, pModelObject[g_Object[nCntObject].nType].apTexture[nCntMat]);

				//モデル(パーツ)の描画
				pModelObject[g_Object[nCntObject].nType].pMesh->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す 
			pDevice->SetMaterial(&matDef);
		}
	}
}

//========================================
//オブジェクトの設定処理
//========================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	//情報取得
	Model *pModelObject = GetModelobject();

	int nCntObject;
	float fAvrRadius = 0.0f;	//平均値算出用
	float fWidth, fHeight;
	SHADOWTYPE type;

	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == false)
		{//弾が使用されてない

			//変数初期化
			ZeroMemory(&g_Object[nCntObject],sizeof(Object));
			g_Object[nCntObject].nIdxshadow = -1;

			//引数受け取り
			g_Object[nCntObject].pos = pos;
			g_Object[nCntObject].rot = rot;
			g_Object[nCntObject].rotBase = rot;
			g_Object[nCntObject].nType = nType;
			g_Object[nCntObject].bUse = true;						//使用している状態にする
			g_Object[nCntObject].vtxMax = pModelObject[nType].vtxMax;
			g_Object[nCntObject].vtxMin = pModelObject[nType].vtxMin;

			g_Object[nCntObject].posBlock = g_Object[nCntObject].pos;

			switch (nType)
			{
			case OBJECTTYPE_BUTTON: //ボタンの引っ込めておく設定
				g_Object[nCntObject].pos.z += BUTTON_PUSH_DEPTH;
				break;

			case OBJECTTYPE_BUTTON_POLE: //ボタンの引っ込めておく設定
				g_Object[nCntObject].pos.y += BUTTON_PUSH_DEPTH;
				break;

			case OBJECTTYPE_TORCH: //松明のビルボード設定
				g_Object[nCntObject].nIdxBillboard = SetBillboard(BILLBOARDTYPE_FIRE, g_Object[nCntObject].pos);
				break;

			case OBJECTTYPE_SANDPOLE: //砂柱のオフセット設定
				g_Object[nCntObject].posCollision = D3DXVECTOR3{ 0.0f, COLLISION_HEIGHT, 0.0f };
				break;

			case OBJECTTYPE_STONEPOLE: //石柱のオフセット設定
				g_Object[nCntObject].posCollision = D3DXVECTOR3{ 0.0f, -COLLISION_BELOW, 0.0f };
				break;

			case OBJECTTYPE_ROCK_FRAG: //岩の欠片のオフセット設定
				g_Object[nCntObject].fScale = ((float)(rand() % 501) / 1000.0f) + 0.5f;
				g_Object[nCntObject].move = D3DXVECTOR3(sinf((float)((rand() % 629 - 314) / 100.0f)) * ROCKFRAG_SPEED,
					cosf((float)((rand() % 629 - 314) / 100.0f)) * ROCKFRAG_SPEED,
					cosf((float)((rand() % 629 - 314) / 100.0f)) * ROCKFRAG_SPEED);
				break;
			}

			//判定基準点の設定
			SetPoint(nCntObject);

			//インデックスの設定
			SetEditIndex(nCntObject);

			//影の設定
			if (pos.y < CEILING_POSY)
			{
				if (nType != OBJECTTYPE_STONEPOLE)
				{
					if (g_Object[nCntObject].nType == OBJECTTYPE_CATAPULT ||
						g_Object[nCntObject].nType == OBJECTTYPE_BUTTON ||
						g_Object[nCntObject].nType == OBJECTTYPE_REFLECTOR ||
						g_Object[nCntObject].nType == OBJECTTYPE_SANDPOLE_BASE ||
						g_Object[nCntObject].nType == OBJECTTYPE_DOOR ||
						g_Object[nCntObject].nType == OBJECTTYPE_TREASUREBOX ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_0 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_1 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_4 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_5 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_6 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_7 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_8 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_100 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_101)
					{
						type = SHADOWTYPE_SQUARE;

							fWidth = g_Object[nCntObject].vtxMax.x * SHADOW_SCALE_SQR;
							fHeight = g_Object[nCntObject].vtxMax.z * SHADOW_SCALE_SQR;
					}
					else
					{
						g_Object[nCntObject].vtxMax.x <= g_Object[nCntObject].vtxMax.z ?
							fAvrRadius = g_Object[nCntObject].vtxMax.z * SHADOW_SCALE_CCL : 
							fAvrRadius = g_Object[nCntObject].vtxMax.x * SHADOW_SCALE_CCL;
						type = SHADOWTYPE_CIRCLE;
						fWidth = 0.0f;
						fHeight = 0.0f;
					}

					D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					rotShadow.y = g_Object[nCntObject].rot.y;

					g_Object[nCntObject].nIdxshadow = SetShadow(fAvrRadius,	type, fWidth, fHeight, rotShadow);
				}
			}

			break;
		}
	}
}

//========================================
//オブジェクトの消去処理
//========================================
void DeleteObject(int nIdx)
{
	g_Object[nIdx].bUse = false;
	g_Object[nIdx].pos = { 0.0f,0.0f,0.0f };
	g_Object[nIdx].rot = { 0.0f,0.0f,0.0f };
	g_Object[nIdx].fScale = 1.0f;

	//影の削除
	EnableShadow(g_Object[nIdx].nIdxshadow, false);
}

//========================================
//状態の変更
//========================================
void StateChangeObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		g_Object[nCntObject].posOld = g_Object[nCntObject].pos;

		if (g_Object[nCntObject].bUse == true)
		{
			switch (g_Object[nCntObject].state)
			{
			case OBJECTSTATE_NORMAL:
				break;

			case OBJECTSTATE_DAMEGE:
				g_Object[nCntObject].nCounterState--;

				if (g_Object[nCntObject].nCounterState <= 0)
				{
					g_Object[nCntObject].state = OBJECTSTATE_NORMAL;
				}
				break;
			}
		}
	}
}

//========================================
//オブジェクトとの当たり判定
//========================================
bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump)
{
	//変数宣言
	int nCntObject;
	int *pHitRockCouter = GetHitRockNum();
	Billboard *pBillboard = GetBillboard();
	float fConv = 0.0f;

	bool bLand = false;			//着地したかどうか

	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true
			&& (g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR
			&& g_Object[nCntObject].nType != OBJECTTYPE_LEVER_BAR
			&& g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR_BASE
			&& g_Object[nCntObject].nType != OBJECTTYPE_ROCK_FRAG))
		{

			if ((g_Object[nCntObject].rot.y <= D3DX_PI * 0.51f && g_Object[nCntObject].rot.y >= D3DX_PI * 0.49f ) ||
				(g_Object[nCntObject].rot.y >= D3DX_PI * -0.51f && g_Object[nCntObject].rot.y <= D3DX_PI * -0.49f))
			{//90度だったらzとxを入れ替える
				fConv = g_Object[nCntObject].vtxMin.x;
				g_Object[nCntObject].vtxMin.x = g_Object[nCntObject].vtxMin.z;
				g_Object[nCntObject].vtxMin.z = fConv;

				fConv = g_Object[nCntObject].vtxMax.x;
				g_Object[nCntObject].vtxMax.x = g_Object[nCntObject].vtxMax.z;
				g_Object[nCntObject].vtxMax.z = fConv;
			}

			if (pPos->y <= g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y &&
				pPos->y + vtxMax.y >= g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMin.y)
			{//上下の範囲内にいるとき
				if (pPosOld->x + vtxMax.z <= g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
					&& pPos->x + vtxMax.z > g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
					&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
					&& pPos->z + vtxMin.z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z)
				{//右の判定
					pPos->x = g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x - vtxMax.z - 0.05f;
				}
				if (pPosOld->x - vtxMax.z >= g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
					&& pPos->x - vtxMax.z < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
					&& pPos->z - vtxMax.z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z
					&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z)
				{//左の判定
					pPos->x = g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x + vtxMax.z + 0.05f;
				}
				if (pPosOld->z + vtxMax.z <= g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
					&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
					&& pPos->x + vtxMax.x > g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
					&& pPos->x - vtxMax.x < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x)
				{//前の判定
					pPos->z = g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z - vtxMax.z - 0.05f;

					if (g_buttonState == BUTONSTATE_WAIT && g_Object[nCntObject].nType == OBJECTTYPE_BUTTON)
					{//待機状態でのみ押せる
						g_Object[nCntObject].pos.z += 1.0f;
					
						if (g_bOpenRock == false)
						{
							if (g_Object[nCntObject].nCntSound <= 0)
							{
								PlaySound(SOUND_LABEL_SE_SHIFT);
								g_Object[nCntObject].nCntSound = SHIFT_SOUND_TIME;
							}
							else
							{
								g_Object[nCntObject].nCntSound--;
							}
						}
					}

					if (g_Object[nCntObject].pos.z >= g_Object[nCntObject].posBlock.z + BUTTON_PUSH_DEPTH)
					{//移動制限
						g_Object[nCntObject].pos.z = g_Object[nCntObject].posBlock.z + BUTTON_PUSH_DEPTH;

						g_bOpenRock = true;
						*pHitRockCouter = 0;
						if (g_bButtonSound == false)
						{							
							PlaySound(SOUND_LABEL_SE_SHIFT);
							g_bButtonSound = true;
						}

						//チュートリアルを進める
						TUTORIALSTATE *tutorial = GetTutorialState();
						if (*tutorial == TUTORIALSTATE_BUTTON && GetMode() == MODE_TUTORIAL)
						{
							*tutorial = TUTORIALSTATE_JUMP;

							SetCount(D3DXVECTOR3(640.0f, 120.0, 0.0f), 100.0f, 2);
							PlaySound(SOUND_LABEL_SE_BUTTON_004);

							for (int nCntButton = 0; nCntButton < MAX_OBJECT; nCntButton++)
							{
								switch (g_Object[nCntButton].nType)
								{
								case OBJECTTYPE_BUTTON:
									pBillboard[g_Object[nCntButton].nIdxBillboard].bUse = false;
									break;
								}
							}
						}
					}
				}

				if (pPosOld->z - vtxMax.z >= g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z
					&& pPos->z - vtxMax.z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z
					&& pPos->x - vtxMax.x < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
					&& pPos->x + vtxMax.x > g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x)
				{//後ろの判定
					pPos->z = g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z + vtxMax.z + 0.05f;

				}
			}
			if (pPos->x + vtxMax.x >= g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
				&& pPos->x < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
				&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
				&& pPos->z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z)
			{
				if (pPosOld->y >= g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y
					&& pPos->y < g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y)
				{
					bLand = true;
					pPos->y = g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y;
					pMove->y = 0.0f;
					if (g_Object[nCntObject].nType == OBJECTTYPE_TORCH)
					{
						HitPlayer(1);
					}

					if (g_buttonStatePole == BUTONSTATE_WAIT && g_Object[nCntObject].nType == OBJECTTYPE_BUTTON_POLE)
					{//待機状態でのみ押せる
						g_Object[nCntObject].pos.y -= 1.0f;

						if (g_bPoleAttack)
						{
							if (g_Object[nCntObject].nCntSound <= 0)
							{
								PlaySound(SOUND_LABEL_SE_SHIFT);
								g_Object[nCntObject].nCntSound = SHIFT_SOUND_TIME;
							}
							else
							{
								g_Object[nCntObject].nCntSound--;
							}
						}

						if (g_Object[nCntObject].pos.y <= g_Object[nCntObject].posBlock.y && g_bPoleAttack)
						{//移動制限
							g_Object[nCntObject].pos.y = g_Object[nCntObject].posBlock.y;

							g_bPoleAttack = false;

							Enemy *enemy = GetEnemy();
							D3DXVECTOR3 posEnemy = enemy->pos;
							posEnemy.y += 490.0f;

							SetRock
							(
								posEnemy,
								D3DXVECTOR3(0.0f,0.0f,0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								LIFE_ROCK,
								ROCKTYPE_DROP
							);
						}
					}
				}
			}
		}
	}
	return bLand;
}

//========================================
// オブジェクトのヒット処理
//========================================
void HitObject(int nDamage,int nCntObject)
{
	//体力を減らす
	g_Object[nCntObject].nLife -= nDamage;
	
	//プレイヤーの体力がなくなった
	if (g_Object[nCntObject].nLife == 0)
	{
		DeleteObject(nCntObject);
	}
	else
	{//体力が残っている
		g_Object[nCntObject].state = OBJECTSTATE_DAMEGE;
		g_Object[nCntObject].nCounterState = 5;
	}
}

//========================================
//オブジェクトの取得
//========================================
Object *ObjectGet(void)
{
	return &g_Object[0];
}

//========================================
//指定のオブジェクトのメッシュの取得
//========================================
LPD3DXMESH ObjectMeshGet(OBJECTTYPE type)
{
	//情報取得
	Model *pModelObject = GetModelobject();

	return pModelObject[type].pMesh;
}

//========================================
//指定のオブジェクトのマテリアルの取得
//========================================
LPD3DXBUFFER ObjectBuffmatGet(OBJECTTYPE type)
{
	//情報取得
	Model *pModelObject = GetModelobject();

	return pModelObject[type].pBuffMat;
}

//========================================
//指定のオブジェクトのマテリアル数の取得
//========================================
DWORD ObjectdwNummatGet(OBJECTTYPE type)
{
	//情報取得
	Model *pModelObject = GetModelobject();

	return pModelObject[type].dwNumMat;
}

//==============================
//オブジェクトの使用情報取得処理
//==============================
int GetUseObject(void)
{
	int nNumUse = 0;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (g_Object[nCnt].bUse == true)
		{
			nNumUse++;
		}
	}

	return nNumUse;
}

//========================================
//岩が一定で出る
//========================================
void SetRockObject(int nCntObject)
{
	if (g_bOpenRock == true)
	{
		g_Object[nCntObject].nCounter++;

		if ((g_Object[nCntObject].nCounter % INJECT_TIME) == 0)
		{//一定時間ごとに岩を発射する
			SetRock
			(
				D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 50.0f, g_Object[nCntObject].pos.z),
				D3DXVECTOR3
				(
					sinf(g_Object[nCntObject].rot.y) * -SPEED_ROCK,
					0.0f,
					cosf(g_Object[nCntObject].rot.y)  * -SPEED_ROCK
				),
				g_Object[nCntObject].rot,
				LIFE_ROCK,
				ROCKTYPE_GIMMICK
			);
			g_Object[nCntObject].RockCounter++;
		}

		if (g_bOpenRock == false)
		{
			g_Object[nCntObject].nCounter = 1;
		}
	}

	if (g_Object[nCntObject].pos.y >= GetCeilingPos().y)
	{//天井より高い場合
		SetRock
		(
			D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 50.0f, g_Object[nCntObject].pos.z),
			D3DXVECTOR3
			(
				sinf(g_Object[nCntObject].rot.y) * -SPEED_ROCK * 1.0f,
				0.0f,
				cosf(g_Object[nCntObject].rot.y)  * -SPEED_ROCK * 1.0f
			),
			g_Object[nCntObject].rot,
			180,
			ROCKTYPE_INVISIBLE
		);
	}
	else
	{
		SetRock
		(
			D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 50.0f, g_Object[nCntObject].pos.z),
			D3DXVECTOR3
			(
				sinf(g_Object[nCntObject].rot.y) * -SPEED_ROCK * 1.0f,
				0.0f,
				cosf(g_Object[nCntObject].rot.y)  * -SPEED_ROCK * 1.0f
			),
			g_Object[nCntObject].rot,
			5,
			ROCKTYPE_INVISIBLE
		);
	}
}

//========================================
//ビルボードの出現
//========================================
void BillboardUse(Object *pObject)
{
	Player *pPlayer = GetPlayer();				//プレイヤーのポインタ
	Billboard *pBillboard = GetBillboard();
	//int nCntObject;

	float fLength;		//2点間の長さ

	if (pObject->bBillboard == false)
	{
		//円の当たり判定
		fLength = (pPlayer->pos.x - pObject->pos.x) * (pPlayer->pos.x - pObject->pos.x)
			+ (pPlayer->pos.z - pObject->pos.z) * (pPlayer->pos.z - pObject->pos.z);

		if (fLength < (200.0f * 200.0f))
		{
			//ビルボードの設定処理
			pObject->nIdxBillboard = SetBillboard(BILLBOARDTYPE_ICON,pObject->pos);
			pObject->bBillboard = true;
			//GetKeyboardTrigger
			if (GetKeyboardTrigger(DIK_M) == true)
			{
				g_Object[pObject->nIdxBillboard].rot.y += D3DX_PI * 0.2f;
			}
		}
	}

	if (pObject->bBillboard == true)
	{
		//円の当たり判定
		fLength = (pPlayer->pos.x - pObject->pos.x) * (pPlayer->pos.x - pObject->pos.x)
			+ (pPlayer->pos.z - pObject->pos.z) * (pPlayer->pos.z - pObject->pos.z);

		if (fLength > (200.0f * 200.0f))
		{
			//ビルボードの設定処理
			pBillboard[pObject->nIdxBillboard].bUse = false;
			pObject->bBillboard = false;
		}
	}
}

//===========================
//判定基準点の設定処理
//===========================
void SetPoint(int nNum)
{
	float fAngle, fLength;

	if (g_Object[nNum].nType != OBJECTTYPE_LEVER_BAR)
	{
		//対角線の長さを算出する
		fLength = sqrtf((float)pow(g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x, 2) + (float)pow(g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z, 2)) * 0.5f;

		//対角線の角度を算出する
		fAngle = atan2f(g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x, g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z);

		//判定の基準点計算
		g_Object[nNum].point[0].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y + fAngle) * fLength;
		g_Object[nNum].point[0].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[0].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + fAngle) * fLength;

		g_Object[nNum].point[1].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y - fAngle) * fLength;
		g_Object[nNum].point[1].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[1].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y - fAngle) * fLength;

		g_Object[nNum].point[2].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y - D3DX_PI + fAngle) * fLength;
		g_Object[nNum].point[2].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[2].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y - D3DX_PI + fAngle) * fLength;

		g_Object[nNum].point[3].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y + D3DX_PI - fAngle) * fLength;
		g_Object[nNum].point[3].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[3].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + D3DX_PI - fAngle) * fLength;
	}
	else if (g_Object[nNum].nType == OBJECTTYPE_LEVER_BAR)
	{
		//対角線の長さを算出する
		fLength = sqrtf((float)pow((g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x), 2) + (float)pow(g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z, 2));

		//対角線の角度を算出する
		fAngle = atan2f((g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x), g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z);

		//判定の基準点計算
		g_Object[nNum].point[0].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y + (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);
		g_Object[nNum].point[0].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[0].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);

		g_Object[nNum].point[1].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y - (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);
		g_Object[nNum].point[1].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[1].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y - (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);

		g_Object[nNum].point[2].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y  + D3DX_PI + fAngle) * fLength * 0.95f;
		g_Object[nNum].point[2].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[2].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + D3DX_PI  + fAngle) * fLength * 0.95f;

		g_Object[nNum].point[3].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y  + D3DX_PI - fAngle) * fLength * 0.95f;
		g_Object[nNum].point[3].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[3].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y  + D3DX_PI - fAngle) * fLength * 0.95f;
	}
}

//===========================
//反射板の当たり判定反射処理
//===========================
bool CollisionReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength, int type)
{
	//差分取得
	D3DXVECTOR3 posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDiffLength = 0.0f;

	bool bMeshWall;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fDot, fRock, fLengthRock, fOuterRef[2], fRotRock;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef, posCol;

	bMeshWall = false;
	bool bNear;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		posDiff = g_Object->pos - GetPlayer()->pos;
		fDiffLength = D3DXVec3Length(&posDiff);

		if (g_Object[nCnt].bUse == true && g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR)
		{
			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				//判定用ベクトル計算
				if (nCnt1 == 3)
				{
					//岩の角度
					fRock = atan2f(g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//板側面のベクトル
					vecLine.x = g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z;

					//基準点と岩の位置のベクトル
					vecToPos.x = (pos->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;

					//基準点と岩の前の位置のベクトル
					vecToPosOld.x = (posOld->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;
				}
				else
				{
					//岩の角度
					fRock = atan2f(g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//板側面のベクトル
					vecLine.x = g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z;

					//基準点と岩の位置のベクトル
					vecToPos.x = (pos->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;

					//基準点と岩の前の位置のベクトル
					vecToPosOld.x = (posOld->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;
				}

				//移動量ベクトル
				vecMove = *pos - *posOld;

				//側面と岩の位置の外積
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
				fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

				//交点計算
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				//岩の中心とポイントの距離計算
				fLengthRock = sqrtf((float)pow(g_Object[nCnt].point[nCnt1].x - pos->x, 2) + (float)pow(g_Object[nCnt].point[nCnt1].z - pos->z, 2));

				//角に当たっている
				if (fLengthRock < fLength)
				{
					if (pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y && pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y)
					{
						//岩の角度
						fRock = atan2f(pos->x - g_Object[nCnt].point[nCnt1].x, pos->z - g_Object[nCnt].point[nCnt1].z);

						//岩の位置移動
						pos->x = g_Object[nCnt].point[nCnt1].x + (sinf(fRock) * fLength);
						pos->z = g_Object[nCnt].point[nCnt1].z + (cosf(fRock) * fLength);

						//法線ベクトル計算
						vecNor.x = pos->x - g_Object[nCnt].point[nCnt1].x;
						vecNor.y = 0.0f;
						vecNor.z = pos->z - g_Object[nCnt].point[nCnt1].z;

						//法線ベクトル単位化
						D3DXVec3Normalize(&vecNor, &vecNor);

						//水平方向大きさ
						fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

						//水平方向ベクトル計算
						vecDot = (vecNor * fDot) * 2;

						//移動量ベクトル計算
						vecMoveRef = vecMove + vecDot;

						//移動量ベクトル単位化
						D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

						//当たったポイントから岩の移動方向と当たったポイントと次のポイントの外積計算
						if (nCnt1 == 0)
						{
							fOuterRef[0] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[3]);
						}
						else
						{
							fOuterRef[0] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[nCnt1 - 1]);
						}

						//当たったポイントから岩の移動方向と当たったポイントと前のポイントの外積計算
						if (nCnt1 == 3)
						{
							fOuterRef[1] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[0]);
						}
						else
						{
							fOuterRef[1] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[nCnt1 + 1]);
						}

						//移動方向がオブジェクト内を向いてるかどうか
						if (fOuterRef[0] < 0.0f && fOuterRef[1] > 0.0f)
						{
							//どちらの直線が近いか
							if (-fOuterRef[0] < fOuterRef[1])
							{
								//角度計算
								if (nCnt1 == 3)
								{
									fRotRock = atan2f(g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z);
								}
								else
								{
									fRotRock = atan2f(g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z);
								}

								//移動方向調整
								vecMoveRef.x = (-sinf(fRotRock) * vecMoveRef.z) + (cosf(fRotRock) * vecMoveRef.x);
								vecMoveRef.z = (sinf(fRotRock) * vecMoveRef.x) + (cosf(fRotRock) * vecMoveRef.z);
							}
							else
							{
								//角度計算
								if (nCnt1 == 0)
								{
									fRotRock = atan2f(g_Object[nCnt].point[3].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[3].z - g_Object[nCnt].point[nCnt1].z);
								}
								else
								{
									fRotRock = atan2f(g_Object[nCnt - 1].point[nCnt1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 - 1].z - g_Object[nCnt].point[nCnt1].z);
								}

								//移動方向調整
								vecMoveRef.x = (-sinf(fRotRock) * vecMoveRef.z) + (cosf(fRotRock) * vecMoveRef.x);
								vecMoveRef.z = (sinf(fRotRock) * vecMoveRef.x) + (cosf(fRotRock) * vecMoveRef.z);
							}
						}

						//移動量ベクトル単位化
						D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

						if (type == ROCKTYPE_INVISIBLE)
						{
							//移動量代入
							*move = vecMoveRef * SPEED_ROCK * 1.0f;
						}
						else
						{
							//移動量代入
							*move = vecMoveRef * SPEED_ROCK;
						
							//情報取得
							Player *pPlayer = GetPlayer();

							// 音が聞こえる位置にいるかの当たり判定
							bNear = SphereCollision(g_Object[nCnt].pos, pPlayer->pos, ROLLSOUND_RANGE, 0.0f);

							if (bNear)
							{//音が聞こえる範囲なら

								//差分の量によって音量を変える
								SetVolume(SOUND_LABEL_SE_COLROCK, 1.0f - fDiffLength / ROLLSOUND_RANGE);

								PlaySound(SOUND_LABEL_SE_COLROCK);
							}
						}

						bMeshWall = true;

						break;
					}
				}

				//側面衝突判定
				if (fOutToPos < 0.0f && fOutToPosOld > 0.0f && fRate < 1.0f && fRate > 0.0f)
				{
					if (pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y && pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y)
					{
						//岩の位置移動
						pos->x = g_Object[nCnt].point[nCnt1].x + (vecLine.x * fRate) - (fLength * sinf(fRock));
						pos->z = g_Object[nCnt].point[nCnt1].z + (vecLine.z * fRate) - (fLength * cosf(fRock));

						//法線ベクトル計算
						vecNor.x = vecLine.z;
						vecNor.y = 0.0f;
						vecNor.z = -vecLine.x;

						//法線ベクトル単位化
						D3DXVec3Normalize(&vecNor, &vecNor);

						//水平方向大きさ
						fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

						//水平方向ベクトル計算
						vecDot = (vecNor * fDot) * 2;

						//移動量ベクトル計算
						vecMoveRef = vecMove + vecDot;

						//移動量ベクトル単位化(必要か分からない)
						D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

						if (type == ROCKTYPE_INVISIBLE)
						{
							//移動量代入
							*move = vecMoveRef * SPEED_ROCK * 1.0f;
						}
						else
						{
							//移動量代入
							*move = vecMoveRef * SPEED_ROCK;
						
							//情報取得
							Player *pPlayer = GetPlayer();

							// 音が聞こえる位置にいるかの当たり判定
							bNear = SphereCollision(g_Object[nCnt].pos, pPlayer->pos, ROLLSOUND_RANGE, 0.0f);

							if (bNear)
							{//音が聞こえる範囲なら

							 //差分の量によって音量を変える
								SetVolume(SOUND_LABEL_SE_COLROCK, 1.0f - fDiffLength / ROLLSOUND_RANGE);

								PlaySound(SOUND_LABEL_SE_COLROCK);
							}
						}

						bMeshWall = true;
					}
				}
			}
		}
	}

	return bMeshWall;
}

//===========================
//岩とオブジェクトの当たり判定
//===========================
void CollisionRockObject(int nCntObject)
{
	//情報の取得
	Rock *pRock = GetRock();

	//変数宣言
	float fAveMax;	//最大頂点の平均
	D3DXVECTOR3 PosDiff;
	bool bHit;

	for (int nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++, pRock++)
	{
		if (pRock->bUse && pRock->nCounter >= ROCK_TIME)
		{//使用している状態なら
			//差分を取得
			PosDiff = pRock->pos - g_Object[nCntObject].pos;

			//オブジェクトの最大頂点から平均を算出
			fAveMax = (g_Object[nCntObject].vtxMax.x + g_Object[nCntObject].vtxMax.z) / 2;

			//球の当たり判定処理
			bHit = SphereCollision(g_Object[nCntObject].pos, pRock->pos, fAveMax * COLLISION_RATE, pRock->vtxMax.x * COLLISION_RATE);

			if (bHit)
			{//判定が当たったら
				float fAngle = 0.0f;

				switch (g_Object[nCntObject].nType)
				{
				case OBJECTTYPE_SANDPOLE:
					//砂の柱を倒す============
					//角度を算出
					fAngle = atan2f(PosDiff.x, PosDiff.z);

					//柱を倒れる方に向ける
					g_Object[nCntObject].rot.y = fAngle + D3DX_PI;

					//回転量に代入
					g_Object[nCntObject].rotate.x = ROTATE_SPEED;
					//========================

					//岩を破壊する
					DisableRock(pRock);
					break;

				case OBJECTTYPE_STONEPOLE:
					//石柱を落とす
					g_Object[nCntObject].move.y = -FALL_SPEED;
					break;

				case OBJECTTYPE_ROCK_FRAG:
					break;

				default:

					//岩消去
					DisableRock(pRock);

					break;
				}
			}
		}
	}
}

//===========================
//透明な岩とオブジェクトの当たり判定
//===========================
bool CollisionRockInvisible(D3DXVECTOR3 *pos, float *vtxMax)
{
	//変数宣言
	float fAveMax;	//最大頂点の平均
	bool bHit;

	bHit = false;

	for (int nCntObject = 0; nCntObject < MAX_ROCK; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{
			if (g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR &&
				g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR_BASE &&
				g_Object[nCntObject].nType != OBJECTTYPE_SANDPOLE_BASE &&
				g_Object[nCntObject].nType != OBJECTTYPE_ROCK_FRAG &&
				g_Object[nCntObject].nType != OBJECTTYPE_CATAPULT)
			{
				//オブジェクトの最大頂点から平均を算出
				fAveMax = (g_Object[nCntObject].vtxMax.x + g_Object[nCntObject].vtxMax.z) / 2;

				//球の当たり判定処理
				if (SphereCollision(g_Object[nCntObject].pos, *pos, fAveMax, *vtxMax) == true)
				{
					bHit = true;
				}
			}
		}
	}

	return bHit;
}

//===========================
//反射板の当たり判定回転処理
//===========================
bool PushReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float *rot, bool jump)
{
	//情報取得
	Camera *pCamara = GetCamera();
	Player *pPlayer = GetPlayer();
	Billboard *pBillboard = GetBillboard();
	
	//変数宣言
	bool bReflector;
	int nInnerCnt = 0;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fRef, fLengthPlayer, fRotRef, fRefLength;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef;

	bReflector = false;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (g_Object[nCnt].bUse == true && (g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR || g_Object[nCnt].nType == OBJECTTYPE_LEVER_BAR))
		{
			if (g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR)
			{
				fRefLength = REF_LENGTH;
			}
			else if (g_Object[nCnt].nType == OBJECTTYPE_LEVER_BAR)
			{
				fRefLength = 23.0f;
			}

			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				//判定用ベクトル計算
				if (nCnt1 == 3)
				{
					//プレイヤーの角度
					fRef = atan2f(g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//基準点とプレイヤーの位置のベクトル
					vecToPos.x = (pos->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//基準点とプレイヤーの前の位置のベクトル
					vecToPosOld.x = (posOld->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//板側面のベクトル
					vecLine.x = g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z;
				}
				else
				{
					//プレイヤーの角度
					fRef = atan2f(g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//基準点とプレイヤーの位置のベクトル
					vecToPos.x = (pos->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//基準点とプレイヤーの前の位置のベクトル
					vecToPosOld.x = (posOld->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//板側面のベクトル
					vecLine.x = g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z;
				}

				//移動量ベクトル
				vecMove = *pos - *posOld;

				//側面とプレイヤーの位置の外積
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
				fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

				//交点計算
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				//プレイヤーの中心とポイントの距離計算
				fLengthPlayer = hypotf(g_Object[nCnt].point[nCnt1].x - pos->x, g_Object[nCnt].point[nCnt1].z - pos->z);

				if (fOutToPos < 0.0f)
				{
					nInnerCnt++;
				}

				//側面衝突判定
				if (fOutToPos < 0.0f && fOutToPosOld > 0.0f && fRate < 1.0f && fRate > 0.0f)
				{
					if (pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y - 40.0f)
					{
						//当たったのでbool型をtrueにする
						bReflector = true;

						if (jump == false)
						{
							if (g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR)
							{
								//側面衝突判定
								if (fRate > 0.6f)
								{
									//反射板回転
									g_Object[nCnt].rot.y -= ROT_SPEED * fRate;

									//カメラ回転
									pCamara->rot.y += ROT_SPEED * fRate;

									if (g_Object[nCnt].nCntSound == 0)
									{
										//石をずらす音再生
										PlaySound(SOUND_LABEL_SE_SHIFT);

										//カウントリセット
										g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
									}

									//カウント減算
									g_Object[nCnt].nCntSound--;
								}
								else if (fRate < 0.4f)
								{
									//反射板回転
									g_Object[nCnt].rot.y += ROT_SPEED * (1.0f - fRate);

									//カメラ回転
									pCamara->rot.y -= ROT_SPEED * (1.0f - fRate);
									
									if (g_Object[nCnt].nCntSound == 0)
									{
										//石をずらす音再生
										PlaySound(SOUND_LABEL_SE_SHIFT);

										//カウントリセット
										g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
									}

									//カウント減算
									g_Object[nCnt].nCntSound--;
								}

								TUTORIALSTATE *tutorial = GetTutorialState();
								if (*tutorial == TUTORIALSTATE_REFLECTOR && GetMode() == MODE_TUTORIAL)
								{//ボタンのチュートリアルに進む
									*tutorial = TUTORIALSTATE_BUTTON;

									g_bOpenRock = false;

									g_buttonState = BUTONSTATE_FORWARD;

									PlaySound(SOUND_LABEL_SE_BUTTON_004);
								
									for (int nCntButton = 0; nCntButton < MAX_OBJECT; nCntButton++)
									{
										if (g_Object[nCntButton].nType == OBJECTTYPE_BUTTON)
										{
											g_Object[nCntButton].nIdxBillboard = SetBillboard(BILLBOARDTYPE_GUIDE,
												D3DXVECTOR3(g_Object[nCntButton].pos.x,
													g_Object[nCntButton].pos.y + BUTTON_BILLBOARD,
													g_Object[nCntButton].pos.z));
											break;
										}
									}
								}
							}
							else if (g_Object[nCnt].nType == OBJECTTYPE_LEVER_BAR)
							{
								for (int nCntBar = 0; nCntBar < MAX_OBJECT; nCntBar++)
								{
									if (g_Object[nCntBar].nType == OBJECTTYPE_LEVER_BAR)
									{
										if (nCnt1 == 1)
										{
											//反射板回転
											g_Object[nCntBar].rot.y -= (ROT_SPEED * 0.4f) * fRate;

											if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
											{
												//カメラ回転
												pCamara->rot.y += (ROT_SPEED * 0.4f) * fRate;
											}

											if (g_Object[nCnt].nCntSound == 0)
											{
												//石をずらす音再生
												PlaySound(SOUND_LABEL_SE_SHIFT);

												//カウントリセット
												g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
											}

											//カウント減算
											g_Object[nCnt].nCntSound--;
										}
										else if (nCnt1 == 3)
										{
											//反射板回転
											g_Object[nCntBar].rot.y += (ROT_SPEED * 0.4f) * (1.0f - fRate);

											if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
											{
												//カメラ回転
												pCamara->rot.y -= (ROT_SPEED * 0.4f) * (1.0f - fRate);
											}

											if (g_Object[nCnt].nCntSound == 0)
											{
												//石をずらす音再生
												PlaySound(SOUND_LABEL_SE_SHIFT);

												//カウントリセット
												g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
											}

											//カウント減算
											g_Object[nCnt].nCntSound--;
										}
										else
										{
											//側面衝突判定
											if (fRate > 0.6f)
											{
												//反射板回転
												g_Object[nCntBar].rot.y -= (ROT_SPEED * 0.4f) * fRate;

												if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
												{
													//カメラ回転
													pCamara->rot.y += (ROT_SPEED * 0.4f) * fRate;
												}

												if (g_Object[nCnt].nCntSound == 0)
												{
													//石をずらす音再生
													PlaySound(SOUND_LABEL_SE_SHIFT);

													//カウントリセット
													g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
												}

												//カウント減算
												g_Object[nCnt].nCntSound--;
											}
											else if (fRate < 0.4f)
											{
												//反射板回転
												g_Object[nCntBar].rot.y += (ROT_SPEED * 0.4f) * (1.0f - fRate);

												if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
												{
													//カメラ回転
													pCamara->rot.y -= (ROT_SPEED * 0.4f) * (1.0f - fRate);
												}

												if (g_Object[nCnt].nCntSound == 0)
												{
													//石をずらす音再生
													PlaySound(SOUND_LABEL_SE_SHIFT);

													//カウントリセット
													g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
												}

												//カウント減算
												g_Object[nCnt].nCntSound--;
											}
										}

										fRotRef = g_Object[nCntBar].rot.y - g_Object[nCntBar].rotBase.y;

										if (fRotRef > D3DX_PI)
										{
											fRotRef -= (D3DX_PI * 2);
										}
										else if (fRotRef <= -D3DX_PI)
										{
											fRotRef += (D3DX_PI * 2);
										}
										
										if (fRotRef > D3DX_PI * 0.15f)
										{
											g_Object[nCntBar].rot.y = g_Object[nCntBar].rotBase.y + D3DX_PI * 0.15f;
										}

										if (fRotRef < -D3DX_PI * 0.15f)
										{
											g_Object[nCntBar].rot.y = g_Object[nCntBar].rotBase.y - D3DX_PI * 0.15f;
										}
									}
								}
							}
						}

						SetPoint(nCnt);

						//判定用ベクトル計算
						if (nCnt1 == 3)
						{
							//板側面のベクトル
							vecLine.x = g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x;
							vecLine.z = g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z;
						}
						else
						{
							//板側面のベクトル
							vecLine.x = g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x;
							vecLine.z = g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z;
						}

						//法線ベクトル計算
						vecNor.x = vecLine.z;
						vecNor.y = 0.0f;
						vecNor.z = -vecLine.x;

						//法線ベクトル単位化
						D3DXVec3Normalize(&vecNor, &vecNor);

						//移動量代入
						move->x = 0.0f;
						move->z = 0.0f;

						if (jump == false)
						{
							*rot = atan2f(vecNor.x, vecNor.z);
						}

						//プレイヤーの位置移動
						pos->x = g_Object[nCnt].point[nCnt1].x + (vecLine.x * fRate) + vecNor.x + (fRefLength * sinf(atan2f(vecNor.x, vecNor.z)));
						pos->z = g_Object[nCnt].point[nCnt1].z + (vecLine.z * fRate) + vecNor.z + (fRefLength * cosf(atan2f(vecNor.x, vecNor.z)));

						break;
					}
					
				}

				//角に当たっている
				if (fLengthPlayer < fRefLength)
				{
					if (pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y - 40.0f)
					{
						if (jump == false)
						{
							//プレイヤーの角度
							fRef = atan2f(pos->x - g_Object[nCnt].point[nCnt1].x, pos->z - g_Object[nCnt].point[nCnt1].z);

							float fMoveSize;

							fMoveSize = hypotf(move->x, move->z);

							//プレイヤーの位置移動
							pos->x = g_Object[nCnt].point[nCnt1].x + (sinf(fRef) * (fRefLength + 0.1f));
							pos->z = g_Object[nCnt].point[nCnt1].z + (cosf(fRef) * (fRefLength + 0.1f));

							//法線ベクトル計算
							vecNor.x = pos->x - g_Object[nCnt].point[nCnt1].x;
							vecNor.y = 0.0f;
							vecNor.z = pos->z - g_Object[nCnt].point[nCnt1].z;

							//法線ベクトル単位化
							D3DXVec3Normalize(&vecNor, &vecNor);

							if (OuterProduct(g_Object[nCnt].point[nCnt1], *posOld, *pos) < 0.0f)
							{
								vecNor.x = -vecNor.z;
								vecNor.y = 0.0f;
								vecNor.z = vecNor.x;
							}
							else
							{
								vecNor.x = vecNor.z;
								vecNor.y = 0.0f;
								vecNor.z = -vecNor.x;
							}

							//法線ベクトル単位化
							D3DXVec3Normalize(&vecNor, &vecNor);

							move->x = vecNor.x * fMoveSize;
							move->z = vecNor.z * fMoveSize;

							break;
						}
					}
				}
			}

			if (nInnerCnt == 4 && pos->y <= g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && posOld->y >= g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && pos->y >= g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y)
			{
				pos->y = g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y;
				posOld->y = g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y;
				move->y = 0.0f;

				if (pPlayer->motionState != PLAYERMOTION_JUMP)
				{
					pPlayer->bJump = false;
				}
			}

			nInnerCnt = 0;
		}
	}

	return bReflector;
}

//==================================
//風の移動量処理
//==================================
void WindMove(int nCntRock)
{
	Rock *pRock = GetRock();
	int nNumBar = 0;

	//天井裏の岩に移動量を与える
	for (int nCntObject = 0; nCntObject < MAX_ROCK; nCntObject++)
	{
		if (g_Object[nCntObject].nType == OBJECTTYPE_LEVER_BAR)
		{
			g_Object[nCntObject].WindMove.x = (g_Object[nCntObject].rot.y - g_Object[nCntObject].rotBase.y) * -MOVE_WINDLEVER;

			if (pRock[nCntRock].bUse == true && pRock[nCntRock].pos.y >= GetCeilingPos().y)
			{//天井より高い場合
				if (nNumBar == 0)
				{
					pRock[nCntRock].move.x += g_Object[nCntObject].WindMove.x * 1.0f;
				}
			}

			nNumBar++;
		}
	}
}

//==================================================
//オブジェクトのrotの修正処理
//==================================================
void RotNormalize(D3DXVECTOR3 *pRot)
{
	if (pRot->y > D3DX_PI)
	{
		pRot->y -= D3DX_PI * 2;
	}
	else if (pRot->y < -D3DX_PI)
	{
		pRot->y += D3DX_PI * 2;
	}
}

//========================================
//ボタンの動き
//========================================
void ButtonMove(int nCntObject)
{
	if (g_buttonState == BUTONSTATE_FORWARD)
	{
		//元の位置に戻ろうとする
		g_Object[nCntObject].pos.z += (g_Object[nCntObject].posBlock.z - g_Object[nCntObject].pos.z) * 0.1f;

		//一定以上で待機状態になる
		if ((g_Object[nCntObject].posBlock.z - g_Object[nCntObject].pos.z) < -3)
		{
			g_buttonState = BUTONSTATE_WAIT;
			g_bButtonSound = false;
		}
	}
}

//========================================
//ボタンの動き
//========================================
void ButtonPoleMove(int nCntObject)
{
	if (g_buttonStatePole == BUTONSTATE_FORWARD)
	{
		//元の位置に戻ろうとする
		g_Object[nCntObject].pos.y += (g_Object[nCntObject].posBlock.y - g_Object[nCntObject].pos.y) * 0.1f;

		//一定以上で待機状態になる
		if ((g_Object[nCntObject].posBlock.y - g_Object[nCntObject].pos.y) < -3.0f)
		{
			g_buttonStatePole = BUTONSTATE_WAIT;
			g_bButtonSound = false;
		}
	}
}

//==================================================
//オブジェクトのデバッグ表示
//==================================================
void DebugObject(void)
{
	RECT rect = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };	//場所
	char aStr[2][256];					//数
	LPD3DXFONT Font = GetFont();		//メインのやつ

	//文字列に代入
	sprintf(&aStr[0][0], "オブジェクトの数：%d", GetUseObject());
	sprintf(&aStr[1][0], "ストッパー状況：%d", (int)g_bOpenRock);

	for (int nCntDebug = 0; nCntDebug < 2; nCntDebug++)
	{
		Font->DrawText(NULL, &aStr[nCntDebug][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		rect.top += 20;
	}
}

//==================================================
//点と直線の距離を求める関数
//==================================================
float LengthPointToLine(D3DXVECTOR3 point, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2)
{
	D3DXVECTOR3 lineCoe;		//直線の係数
	float den, num;				//分母と分子
	float answer;				//答え

	lineCoe.x = (linePoint2.z - linePoint1.z) / (linePoint2.x - linePoint1.x);
	lineCoe.y = -1.0f;
	lineCoe.z = (lineCoe.x * linePoint1.x) - linePoint1.z;

	den = hypotf(lineCoe.x, lineCoe.y);
	num = fabsf((lineCoe.x * point.x) + (lineCoe.y * point.z) + lineCoe.z);

	answer = num / den;

	return answer;
}

//==================================================
//オフセットの位置を更新する処理
//==================================================
void UpdatePosOffset(int nCntObject)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Object[nCntObject].mtxWorldCollision);
	
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Object[nCntObject].posCollision.x, g_Object[nCntObject].posCollision.y, g_Object[nCntObject].posCollision.z);
	D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorldCollision, &g_Object[nCntObject].mtxWorldCollision, &mtxTrans);

	//マトリックスをかけ合わせる
	D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorldCollision, &g_Object[nCntObject].mtxWorldCollision, &g_Object[nCntObject].mtxWorld);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCntObject].mtxWorldCollision);
}

//==================================================
//欠片の移動処理
//==================================================
void UpdateRockFrag(int nCntObject)
{
	//移動量をposに加算
	g_Object[nCntObject].move.y -= ROCKFRAG_GLAVITY;
	g_Object[nCntObject].pos += g_Object[nCntObject].move;
	g_Object[nCntObject].fScale -= ROCKFRAG_SCALE_SPEED;
	
	if (g_Object[nCntObject].pos.y + (g_Object[nCntObject].vtxMin.y * g_Object[nCntObject].fScale) < 0.0f)
	{//地面で反射
		g_Object[nCntObject].move.x *= FIELD_REFLECT;
		g_Object[nCntObject].move.y *= -(FIELD_REFLECT + 0.1f);
		g_Object[nCntObject].move.z *= FIELD_REFLECT;
	}

	//if (g_Object[nCntObject].pos.x + (g_Object[nCntObject].vtxMax.x * g_Object[nCntObject].fScale) >= 1500.0f ||
	//	g_Object[nCntObject].pos.x + (g_Object[nCntObject].vtxMin.x * g_Object[nCntObject].fScale) <= -1500.0f)
	//{//x壁で反射
	//	g_Object[nCntObject].move.x *= -1;
	//}
	//else if (g_Object[nCntObject].pos.z + (g_Object[nCntObject].vtxMax.z * g_Object[nCntObject].fScale) >= 1500.0f ||
	//	g_Object[nCntObject].pos.z + (g_Object[nCntObject].vtxMin.z * g_Object[nCntObject].fScale) <= -1500.0f)
	//{//z壁で反射
	//	g_Object[nCntObject].move.z *= -1;
	//}

	if (g_Object[nCntObject].fScale <= 0)
	{//サイズで削除
		DeleteObject(nCntObject);
	}
}

//========================================
//何かが壊れた時セット
//========================================
void SetBreakObj(D3DXVECTOR3 pos , int nNum)
{
	SetParticle(pos, PARTICLETYPE_SMOG);
	SetParticle(pos, PARTICLETYPE_SPARK);

	for (int nCntFrag = 0; nCntFrag < nNum; nCntFrag++)
	{
		SetObject(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBJECTTYPE_ROCK_FRAG);
	}
}

//========================================
//ランキングでのオブジェクトの更新
//========================================
void UpdateObjectRanking(float moveX)
{
	float fRand = 0.0f;	//ランダム結果保持
	int nObjectType = OBJECTTYPE_SKULL;	//ランダム結果保持
	int nTorchCntr = 0;	//現在松明カウント
	D3DXVECTOR3 Setrot;	//設置する物の向き
	D3DXVECTOR3 TorchPos[3];	//ランキングの松明位置
	D3DXVECTOR3 TorchVtxmtx[3];	//ランキングの松明サイズ

	//情報取得
	Model *pModelObject = GetModelobject();

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{
			//移動処理
			g_Object[nCntObject].posOld = g_Object[nCntObject].pos;
			g_Object[nCntObject].move.x = moveX;
			g_Object[nCntObject].pos.x += g_Object[nCntObject].move.x;

			if (g_Object[nCntObject].pos.x <= -1000.0f)
			{//消去orループ
				switch (g_Object[nCntObject].nType)
				{
				case OBJECTTYPE_TORCH:
					g_Object[nCntObject].pos.x = 2000.0f;
					break;
				default:
					DeleteObject(nCntObject);
					break;
				}
			}

			//影の位置設定
			D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			rotShadow.y = g_Object[nCntObject].rot.y;

			SetPositionShadow(g_Object[nCntObject].nIdxshadow,
				D3DXVECTOR3(g_Object[nCntObject].pos.x, 0.1f, g_Object[nCntObject].pos.z),
				rotShadow,
				1.0f);

			if (g_Object[nCntObject].nType == OBJECTTYPE_TORCH)
			{
				//ビルボードの位置設定処理
				SetPositionBillboard(g_Object[nCntObject].nIdxBillboard,
					D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 80.0f, g_Object[nCntObject].pos.z));

				TorchVtxmtx[nTorchCntr] = g_Object[nCntObject].vtxMax;
				TorchPos[nTorchCntr] = g_Object[nCntObject].pos;
				nTorchCntr++;
			}
		}
	}

	if (g_nRoopSetTime >= SETOBJ_ROOP && rand() % 5 == 0)
	{//周期でランダムセット
		fRand = (float)(rand() % 1000 / 10.0f);		// %以降が100になるように

		if (fRand <= SKULL_PER)
		{//頭蓋骨
			nObjectType = OBJECTTYPE_SKULL;
		}
		else if (fRand <= SKULLMT_PER)
		{//頭蓋骨の山
			nObjectType = OBJECTTYPE_SKULLMT;
		}
		else if (fRand <= STATUE_1_PER)
		{//壊れ？銅像
			nObjectType = OBJECTTYPE_STATUE_1;
		}
		else if (fRand <= STATUE_3_PER)
		{//ルネサンス銅像
			nObjectType = OBJECTTYPE_STATUE_3;
		}
		else if (fRand <= STATUE_4_PER)
		{//座銅像
			nObjectType = OBJECTTYPE_STATUE_4;
		}
		else if (fRand <= STATUE_5_PER)
		{//剣立ち銅像
			nObjectType = OBJECTTYPE_STATUE_5;
		}
		else if (fRand <= STATUE_6_PER)
		{//剣刺し銅像
			nObjectType = OBJECTTYPE_STATUE_6;
		}
		else if (fRand <= STATUE_7_PER)
		{//座銅像
			nObjectType = OBJECTTYPE_STATUE_7;
		}
		else if (fRand <= STATUE_8_PER)
		{//座銅像
			nObjectType = OBJECTTYPE_STATUE_8;
		}
		else if (fRand <= TREASUREBOX_PER)
		{//宝箱
			nObjectType = OBJECTTYPE_TREASUREBOX;
		}
		else if (fRand <= STATUE_100_PER)
		{//現場猫銅像
			nObjectType = OBJECTTYPE_STATUE_100;
		}
		else if (fRand <= STATUE_101_PER)
		{//ジャックオー銅像
			nObjectType = OBJECTTYPE_STATUE_101;
		}
		else
		{//剣
			nObjectType = OBJECTTYPE_SWORD;
		}


		switch (nObjectType)
		{
		case OBJECTTYPE_STATUE_1:
		case OBJECTTYPE_STATUE_3:
		case OBJECTTYPE_STATUE_4:
		case OBJECTTYPE_STATUE_5:
		case OBJECTTYPE_STATUE_6:
		case OBJECTTYPE_STATUE_7:
		case OBJECTTYPE_STATUE_8:
		case OBJECTTYPE_STATUE_100:
		case OBJECTTYPE_STATUE_101:
			Setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;
		default:
			Setrot = D3DXVECTOR3(0.0f, (float)(rand() % 629 - 314) / 100.0f, 0.0f);
			break;
		}

		if ((TorchPos[0].x + TorchVtxmtx[0].x <= RANKING_SETOBJ.x - pModelObject[nObjectType].vtxMax.x ||
			 TorchPos[0].x - TorchVtxmtx[0].x >= RANKING_SETOBJ.x + pModelObject[nObjectType].vtxMax.x) &&
			(TorchPos[1].x + TorchVtxmtx[1].x <= RANKING_SETOBJ.x - pModelObject[nObjectType].vtxMax.x ||
			 TorchPos[1].x - TorchVtxmtx[1].x >= RANKING_SETOBJ.x + pModelObject[nObjectType].vtxMax.x) &&
			(TorchPos[2].x + TorchVtxmtx[2].x <= RANKING_SETOBJ.x - pModelObject[nObjectType].vtxMax.x ||
			 TorchPos[2].x - TorchVtxmtx[2].x >= RANKING_SETOBJ.x + pModelObject[nObjectType].vtxMax.x))
		{//松明にかぶっていなかったら

			SetObject(RANKING_SETOBJ,
					Setrot,
					nObjectType);

			g_nRoopSetTime = 0;
		}
	}

	g_nRoopSetTime++;
}