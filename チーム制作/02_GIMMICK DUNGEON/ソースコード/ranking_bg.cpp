//==========================================
//
// �����L���O�̔w�i�̏���(ranking_bg.cpp)
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
//�}�N����`
//================================
#define RANKINGBG_MOVE (-5.0f)	//�I�u�W�F�N�g�Ȃǂ̈ړ����x

//================================
//�����L���O�w�i�̏�����
//================================
void InitRankingBg()
{
	//�e������
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
//�����L���O�w�i�̏I��
//================================
void UninitRankingBg()
{
	//�e�I��
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
//�����L���O�w�i�̍X�V
//================================
void UpdateRankingBg()
{
	//�e�X�V
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
//�����L���O�w�i�̕`��
//================================
void DrawRankingBg()
{
	//�e�`��
	SetCamera();
	DrawPlayer();
	DrawWall();
	DrawMeshField();
	DrawShadow();
	DrawBillboard();
	DrawObject();
}
