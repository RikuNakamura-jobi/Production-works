//===========================
//
//ダイレクトX.enemyAIヘッダー
//Author:中村　陸
//
//===========================
#ifndef _ENEMYAI_H_
#define _ENEMYAI_H_

#include "enemy.h"

//プロトタイプ宣言
void InitEnemyAi(void);
void AiEnemy(void);
void AiEnemyTutorial(void);
void AiEnemyWalk(void);
void AiEnemySearch(void);
void AiEnemyLost(void);
void AiEnemyFind(void);
void AiEnemyTrack(void);
void AiEnemyDamage(void);
void AiEnemyCounter(void);
void AiEnemyJumpAttack(void);
void AiEnemyMotion(void);
void AiSightEnemy(void);
void AiPointEnemy(void);
void AiLengthEnemy(void);
void AiRotEnemy(void);
void AiMoveEnemy(void);
void AiAttackEnemy(void);
void SetMotionEnemy(ENEMYMOTION motion);
#endif // !_ENEMYAI_H_