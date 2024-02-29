//===========================
//
//ダイレクトX.stageヘッダー
//Author:中村　陸
//
//===========================
#ifndef _STAGE_H_
#define _STAGE_H_

//ステージの状態
typedef enum
{
	STAGESTATE_NONE = 0,			//何もしてない状態
	STAGESTATE_1,					//STAGE1状態
	STAGESTATE_2,					//STAGE2状態
	STAGESTATE_3,					//STAGE3状態
	STAGESTATE_4,					//STAGE4状態
	STAGESTATE_5,					//STAGE5状態
	STAGESTATE_6,					//STAGE6状態
	STAGESTATE_7,					//STAGE7状態
	STAGESTATE_8,					//STAGE8状態
	STAGESTATE_9,					//STAGE9状態
	STAGESTATE_10,					//STAGE10状態
	STAGESTATE_MAX
}STAGESTATE;

//プロトタイプ宣言
void InitStage(void);
void SetStageState(STAGESTATE state, int nCounter);
int GetStageType();

#endif // !_STAGE_H_