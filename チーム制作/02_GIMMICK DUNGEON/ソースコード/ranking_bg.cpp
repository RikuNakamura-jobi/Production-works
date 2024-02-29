//==========================================
//
// ランキングの背景の処理(ranking_bg.cpp)
// Author : Kai Takada
//
//==========================================
#include "ranking_bg.h"
#include "camera.h"
#include "light.h"
#include "wall.h"
#include "meshfield.h"
#include "object.h"
#include "billboard.h"
#include "player.h"
#include "shadow.h"
#include "edit.h"

//================================
//マクロ定義
//================================
#define RANKINGBG_MOVE (-5.0f)	//オブジェクトなどの移動速度

//================================
//ランキング背景の初期化
//================================
void InitRankingBg()
{
	//各初期化
	InitCamera();
	InitLight();
	InitShadow();
	InitPlayer();
	InitWall();
	InitMeshField();
	InitBillboard();
	InitObject();
	LoadEdit();
}

//================================
//ランキング背景の終了
//================================
void UninitRankingBg()
{
	//各終了
	UninitCamera();
	UninitLight();
	UninitShadow();
	UninitPlayer();
	UninitWall();
	UninitMeshField();
	UninitObject();
	UninitBillboard();
}

//================================
//ランキング背景の更新
//================================
void UpdateRankingBg()
{
	//各更新
	UpdateCamera();
	UpdateLight();
	UpdateShadow();
	UpdatePlayerRanking();
	UpdateWallRanking(RANKINGBG_MOVE);
	UpdateObjectRanking(RANKINGBG_MOVE);
	UpdateMeshFieldRanking(RANKINGBG_MOVE);
	UpdateBillboard();
}

//================================
//ランキング背景の描画
//================================
void DrawRankingBg()
{
	//各描画
	SetCamera();
	DrawPlayer();
	DrawWall();
	DrawMeshField();
	DrawShadow();
	DrawBillboard();
	DrawObject();
}
