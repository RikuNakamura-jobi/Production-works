//===========================
//
//�G�l�~�[AI����
//Author:�����@��
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

//�}�N����`
#define ENEMY_ARRIVAL_POINT (3.0f)		//�|�C���g���B�Ƃ݂Ȃ�����
#define ENEMY_LENGTH (300.0f)			//�v���C���[�̋߂��Ŏ~�܂鋗��
#define ENEMY_TIME (1800)				//���̃|�C���g�ɐ؂�ւ��鎞��
#define ENEMY_SIGHT (0.4f)				//���E�͈̔�
#define ENEMY_FIND_COUNTER (1000)		//�����Ă��猸�_�����܂ł̃J�E���^�[
#define ENEMY_ATTCK (400)		//�����Ă��猸�_�����܂ł̃J�E���^�[
#define SPEED_PIERCE	(0.2f)			//�˂��h���Ռ��g�̃X�s�[�h
#define SPEED_STEP	(0.1f)				//�����ݏՌ��g�̃X�s�[�h
#define SPEED_JUMP	(0.3f)				//�W�����v�U���Ռ��g�̃X�s�[�h
#define SCORE_LOSTFIND	(1500)				//��������U��؂莞�X�R�A

//�O���[�o���ϐ��錾
Enemy *g_pEnemy;						//�G�l�~�[�\����
int g_nCntFind;							//�����̃J�E���g
bool bEnemyFind;						//�������Ă邩�ǂ���

//===========================
//�G�l�~�[�̏���������
//===========================
void InitEnemyAi(void)
{
	g_pEnemy = GetEnemy();

	//���l������
	g_nCntFind = 0;							//���Ԃ̃��Z�b�g
	bEnemyFind = false;						//�������Ă邩�ǂ���
}

//===========================
//�G�l�~�[��AI(�܂Ƃ�)
//===========================
void AiEnemy(void)
{
	switch (g_pEnemy->state)
	{
	case ENEMYSTATE_NORMAL:
		//����
		AiEnemyWalk();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_SEARCH:
		//���G
		AiEnemySearch();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_LOST:
		//��������
		AiEnemyLost();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_FIND:
		//��������
		AiEnemyFind();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_TRACK:
		//�ǐ�
		AiEnemyTrack();
		g_pEnemy->nAttackCount++;
		break;
	case ENEMYSTATE_DAMAGE:
		//�_���[�W
		AiEnemyDamage();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_COUNTER:
		//����
		AiEnemyCounter();
		g_pEnemy->nAttackCount = 0;
		break;
	case ENEMYSTATE_JUMPATTACK:
		//�W�����v�U��
		AiEnemyJumpAttack();
		g_pEnemy->nAttackCount++;
		break;
	case ENEMYSTATE_DEATH:
		SetMotionEnemy(ENEMYMOTION_DEATH);

		//���x����
		g_pEnemy->fSpeed = 0.0f;
		break;
	case ENEMYSTATE_SLEEP:
		//����
		AiLengthEnemy();

		SetMotionEnemy(ENEMYMOTION_SLEEP);

		if (g_pEnemy->nHitRock >= 5)
		{
			g_pEnemy->state = ENEMYSTATE_STANDUP;
			SetMotionEnemy(ENEMYMOTION_STANDUP);
		}

		//���x����
		g_pEnemy->fSpeed = 0.0f;
		break;
	case ENEMYSTATE_STANDUP:
		//����
		AiLengthEnemy();

		SetMotionEnemy(ENEMYMOTION_STANDUP);

		//���x����
		g_pEnemy->fSpeed = 0.0f;
		break;
	default:
		break;
	}

	AiEnemyMotion();
}

//===========================
//�G�l�~�[��AI(�`���[�g���A��)
//===========================
void AiEnemyTutorial(void)
{
	//����
	AiLengthEnemy();

	if (*GetTutorialState() == TUTORIALSTATE_JUMP && GetMode() == MODE_TUTORIAL)
	{
		//��莞�ԂōU��
		if (g_pEnemy->nAttackCount >= 300 && g_pEnemy->motionState == ENEMYMOTION_NORMAL)
		{
			//�L�[�ƃt���[�������Z�b�g
			g_pEnemy->nCntKey = 0;
			g_pEnemy->nCntFrame = 0;

			//�˂��h�����[�V����
			SetMotionEnemy(ENEMYMOTION_PIERCE);
		}

		g_pEnemy->nAttackCount++;
	}

	//���x����
	g_pEnemy->fSpeed = 0.0f;

	AiEnemyMotion();
}

//===========================
//�G�l�~�[��AI(����)
//===========================
void AiEnemyWalk(void)
{
	//����
	AiLengthEnemy();

	//���E
	AiSightEnemy();

	//�ړ��n�_
	AiPointEnemy();

	//�p�x
	AiRotEnemy();

	//�ҋ@�A���G���[�V�����̏ꍇ�ړ����[�V�����ɕύX
	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_OVERLOOK)
	{
		SetMotionEnemy(ENEMYMOTION_MOVE);
	}
}

//===========================
//�G�l�~�[��AI(���G)
//===========================
void AiEnemySearch(void)
{
	//����
	AiLengthEnemy();

	//���E
	AiSightEnemy();

	//���x����
	g_pEnemy->fSpeed = 0.0f;

	//�ҋ@�A�ړ����[�V�����̏ꍇ���G���[�V�����ɕύX
	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_MOVE)
	{
		SetMotionEnemy(ENEMYMOTION_OVERLOOK);
	}
}

//===========================
//�G�l�~�[��AI(������)
//===========================
void AiEnemyLost(void)
{
	//�ړ����[�V�����ɕύX
	SetMotionEnemy(ENEMYMOTION_MOVE);

	//����
	AiLengthEnemy();

	//���E
	AiSightEnemy();

	//����
	AiMoveEnemy();

	//�p�x
	AiRotEnemy();
}

//===========================
//�G�l�~�[��AI(����)
//===========================
void AiEnemyFind(void)
{
	//����
	AiLengthEnemy();

	//���E
	AiSightEnemy();

	//�p�x
	AiRotEnemy();

	SetMotionEnemy(ENEMYMOTION_FIND);

	g_pEnemy->fSpeed = 0.0f;
}

//===========================
//�G�l�~�[��AI(�ǐ�)
//===========================
void AiEnemyTrack(void)
{
	//�ҋ@�A���G���[�V�����̏ꍇ�ړ����[�V�����ɕύX
	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_OVERLOOK)
	{
		SetMotionEnemy(ENEMYMOTION_MOVE);
	}

	//�U���p�x����
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

	//����
	AiLengthEnemy();

	//���E
	AiSightEnemy();

	//����
	AiMoveEnemy();

	//�U��
	AiAttackEnemy();

	//�U�����[�V�����ꗗ
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
		//�p�x
		AiRotEnemy();
	}

	//�������[�V�����ȊO�ňړ����x����
	if (g_pEnemy->motionState != ENEMYMOTION_MOVE)
	{
		g_pEnemy->fSpeed = 0.0f;
	}
}

//===========================
//�G�l�~�[��AI(�_���[�W)
//===========================
void AiEnemyDamage(void)
{
	//�ړ����x����
	g_pEnemy->fSpeed = 0.0f;

	//�_���[�W�̗͔̑���
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
//�G�l�~�[��AI(����)
//===========================
void AiEnemyCounter(void)
{
	//�ړ����x����
	g_pEnemy->fSpeed = 0.0f;

	//�ړ����ԃ��Z�b�g
	g_pEnemy->nTimerMove = 0;

	if (g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_BACKJUMP)
	{//�ʏ��Ԃ���уo�b�N�W�����v��

		//�o�b�N�W�����v���[�V�����ɕύX
		SetMotionEnemy(ENEMYMOTION_BACKJUMP);

		//���[�V�����ɍ��킹�Ĉړ��ʂ�����
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//�����̌����Ă�����ɐi��
			g_pEnemy->move.x = -sinf(g_pEnemy->rot.y + D3DX_PI) * 30.0f;
			g_pEnemy->move.y = 10.0f;
			g_pEnemy->move.z = -cosf(g_pEnemy->rot.y + D3DX_PI) * 30.0f;
		}

		//���[�V�����̃^�C�~���O�Ŋp�x����
		if (g_pEnemy->nCntKey >= 5)
		{
			//�p�x
			AiRotEnemy();
		}
	}
	else
	{//���̏��
			
		//�p�x�ݒ�
		AiRotEnemy();

		//���[�V�����ɍ��킹�Ċ��ݒu����
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
//�G�l�~�[��AI(�W�����v�U��)
//===========================
void AiEnemyJumpAttack(void)
{
	//����
	AiLengthEnemy();

	//���E
	AiSightEnemy();

	//�U��
	AiAttackEnemy();
}

//===========================
//�G�l�~�[��AI(���[�V����)
//===========================
void AiEnemyMotion(void)
{
	//�����擾
	D3DXVECTOR3 posDiff = g_pEnemy->pos - GetPlayer()->pos;
	float fDiffLength;

	fDiffLength = D3DXVec3Length(&posDiff) * 0.5f;

	switch (g_pEnemy->motionState)
	{
	case ENEMYMOTION_NORMAL:
		//�ړ��ʂ�ݒ�
		g_pEnemy->fSpeed = 0.0f;
		break;
	case ENEMYMOTION_MOVE:
		//�ړ��ʂ�ݒ�
		g_pEnemy->fSpeed = ENEMY_SPEED;

		if (g_pEnemy->nCntKey == 1 || g_pEnemy->nCntKey == 6)
		{
			//�ړ��ʂ�ݒ�
			g_pEnemy->fSpeed = ENEMY_SPEED * 1.5f;
		}

		if (g_pEnemy->nCntKey == 2 || g_pEnemy->nCntKey == 7)
		{
			//�ړ��ʂ�ݒ�
			g_pEnemy->fSpeed = 0.0f;
			g_pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 10)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:����
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[12].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[12].mtxWorld._43), PARTICLETYPE_SMOG);

			//��ʐU��
			SetQuake(40, (9.0f / g_pEnemy->fLength) + 0.005f);
		}

		if (g_pEnemy->nCntKey == 6 && g_pEnemy->nCntFrame == 10)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:����
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), PARTICLETYPE_SMOG);

			//��ʐU��
			SetQuake(40, (9.0f / g_pEnemy->fLength) + 0.005f);
		}
		break;
	case ENEMYMOTION_SHAKE:
		//�O��======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			// �O�Ղ̐ݒ�
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntKey <= 3)
		{
			//�O�Ղ̈ʒu�ݒ�
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 4 && g_pEnemy->nCntFrame == 0)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//�O�Ղ̍폜
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//�O��======================

		//�U��======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[13].mtxWorld._43), PARTICLETYPE_SMOG);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 0 && g_pEnemy->nCntFrame <= 20)
		{
			//�p�[�e�B�N��:�Ռ��g�\��
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 600.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 600.0f)), 
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 30)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 600.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 600.0f)),
				30, SPEED_STEP);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 20 && g_pEnemy->nCntFrame <= 40)
		{
			//�p�[�e�B�N��:�Ռ��g�\��
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 800.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 800.0f)),
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 50)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 800.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 800.0f)),
				25, SPEED_STEP);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 40 && g_pEnemy->nCntFrame <= 60)
		{
			//�p�[�e�B�N��:�Ռ��g�\��
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f)), 
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 70)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1000.0f)),
				20, SPEED_STEP);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame >= 60 && g_pEnemy->nCntFrame <= 80)
		{
			//�p�[�e�B�N��:�Ռ��g�\��
			SetParticle(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f)), 
				PARTICLETYPE_SPARK);
		}
		else if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 90)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 1200.0f)),
				15, SPEED_STEP);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		//�U��======================
		break;
	case ENEMYMOTION_PIERCE:
		//�O��======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			// �O�Ղ̐ݒ�
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntKey <= 3)
		{
			//�O�Ղ̈ʒu�ݒ�
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 4 && g_pEnemy->nCntFrame == 0)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//�O�Ղ̍폜
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//�O��======================

		//�U��======================
		if (g_pEnemy->nCntKey == 0 && g_pEnemy->nCntFrame == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[13].mtxWorld._43), 500, SPEED_PIERCE);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		//�U��======================
		break;
	case ENEMYMOTION_STEP:
		//�U��======================
		if (g_pEnemy->nCntKey == 0 && g_pEnemy->nCntFrame == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 2)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:����
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), PARTICLETYPE_SMOG);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), 500, SPEED_STEP);

			//��ʐU��
			SetQuake(120, 0.05f);
		}
		//�U��======================
		break;
	case ENEMYMOTION_THROW:
		//�O��======================
		if (g_pEnemy->nCntKey == 0 && g_pEnemy->nCntFrame == 0)
		{
			// �O�Ղ̐ݒ�
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntKey <= 6)
		{
			//�O�Ղ̈ʒu�ݒ�
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 7 && g_pEnemy->nCntFrame == 0)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//�O�Ղ̍폜
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//�O��======================

		//�U��======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[13].mtxWorld._43), PARTICLETYPE_SMOG);
			SetRock(D3DXVECTOR3(g_pEnemy->aEnemyX[13].mtxWorld._41, 50.0f, g_pEnemy->aEnemyX[13].mtxWorld._43),
				D3DXVECTOR3(sinf(g_pEnemy->rot.y) * -SPEED_ROCK, 0.75f, cosf(g_pEnemy->rot.y)  * -SPEED_ROCK),
				g_pEnemy->rot,
				LIFE_ROCK,
				ROCKTYPE_THROW);

			//��ʐU��
			SetQuake(120, 0.03f);
		}
		//�U��======================
		break;
	case ENEMYMOTION_OVERLOOK:
		break;
	case ENEMYMOTION_DAMAGE:
		break;
	case ENEMYMOTION_DOWN:
		break;
	case ENEMYMOTION_BACKJUMP:
		//�U��======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_JUMP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�W�����v
			PlaySound(SOUND_LABEL_SE_JUMP_001);
		}

		if (g_pEnemy->nCntKey == 4 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:���n
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[9].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[9].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[12].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[12].mtxWorld._43), PARTICLETYPE_SMOG);
		}
		//�U��======================
		break;
	case ENEMYMOTION_JUMPATTACK:
		//�O��======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 0)
		{
			// �O�Ղ̐ݒ�
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 2 && g_pEnemy->nCntKey <= 9)
		{
			//�O�Ղ̈ʒu�ݒ�
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 9 && g_pEnemy->nCntFrame == 50)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//�O�Ղ̍폜
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//�O��======================

		//�ړ�======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 1)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_JUMP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�W�����v
			PlaySound(SOUND_LABEL_SE_JUMP_001);
			//���[�V�����ɍ��킹�Ĉړ��ʂ�����
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
		//�ړ�======================

		//�U��======================
		if (g_pEnemy->nCntKey == 3 && g_pEnemy->nCntFrame == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 5 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 280.0f), g_pEnemy->pos.y + 10.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 280.0f)), 500, SPEED_JUMP);
			
			//��ʐU��
			SetQuake(120, 0.06f);
		}

		if (g_pEnemy->nCntKey == 6 && g_pEnemy->nCntFrame == 20)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 8 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 280.0f), g_pEnemy->pos.y + 10.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 280.0f)), 500, SPEED_JUMP);
			
			//��ʐU��
			SetQuake(120, 0.05f);
		}
		//�U��======================
		break;
	case ENEMYMOTION_FIND:
		break;
	case ENEMYMOTION_DEATH:
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 35)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:����
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[8].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[8].mtxWorld._43), PARTICLETYPE_SMOG);

			//��ʐU��
			SetQuake(120, 0.07f);
		}

		if (g_pEnemy->nCntKey == 6 && g_pEnemy->nCntFrame == 15)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:����
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[8].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[8].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[11].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[11].mtxWorld._43), PARTICLETYPE_SMOG);

			//��ʐU��
			SetQuake(120, 0.07f);
		}

		if (g_pEnemy->nCntKey == 8 && g_pEnemy->nCntFrame == 20)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_STEP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:����
			PlaySound(SOUND_LABEL_SE_STEP_001);

			//�p�[�e�B�N��:��
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[0].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[0].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[1].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[1].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[2].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[1].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[3].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[3].mtxWorld._43), PARTICLETYPE_SMOG);
			SetParticle(D3DXVECTOR3(g_pEnemy->aEnemyX[5].mtxWorld._41, 0.0f, g_pEnemy->aEnemyX[5].mtxWorld._43), PARTICLETYPE_SMOG);

			//��ʐU��
			SetQuake(120, 0.07f);
		}
		break;
	case ENEMYMOTION_JUMPATTACK_LIGHT:
		//�O��======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 0)
		{
			// �O�Ղ̐ݒ�
			g_pEnemy->nIdxOrbit = SetOrbit(g_pEnemy->aEnemyX[13].mtxWorld, D3DXVECTOR3(0.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, -300.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50);
		}
		if (g_pEnemy->nCntKey >= 2 && g_pEnemy->nCntKey <= 7)
		{
			//�O�Ղ̈ʒu�ݒ�
			SetPositionOffset(g_pEnemy->aEnemyX[13].mtxWorld, g_pEnemy->nIdxOrbit);
		}
		if (g_pEnemy->nCntKey == 7 && g_pEnemy->nCntFrame == 8)
		{
			if (g_pEnemy->nIdxOrbit != -1)
			{
				//�O�Ղ̍폜
				EnableOrbit(g_pEnemy->nIdxOrbit);
				g_pEnemy->nIdxOrbit = -1;
			}
		}
		//�O��======================

		//�ړ�======================
		if (g_pEnemy->nCntKey == 2 && g_pEnemy->nCntFrame == 1)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_JUMP_001, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�W�����v
			PlaySound(SOUND_LABEL_SE_JUMP_001);
			//���[�V�����ɍ��킹�Ĉړ��ʂ�����
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
		//�ړ�======================

		//�U��======================
		if (g_pEnemy->nCntKey == 1 && g_pEnemy->nCntFrame == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_001);
		}

		if (g_pEnemy->nCntKey == 5 && g_pEnemy->nCntFrame == 0)
		{
			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

			//���ʉ�:�U��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);

			//�Ռ��g
			SetShock(D3DXVECTOR3(g_pEnemy->pos.x + (sinf(g_pEnemy->rot.y + D3DX_PI) * 280.0f), g_pEnemy->pos.y + 0.0f, g_pEnemy->pos.z + (cosf(g_pEnemy->rot.y + D3DX_PI) * 280.0f)), 500, SPEED_JUMP);
			
			//��ʐU��
			SetQuake(120, 0.06f);
		}
		//�U��======================
		break;
	case ENEMYMOTION_SLEEP:
		if (g_pEnemy->nHitRock <= 1)
		{
			if (g_pEnemy->nCntFrame % 400 == 200)
			{
				//���ʉ�:�U���\��
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//��ʐU��
				SetQuake(600, 0.005f);
			}
		}
		else if (g_pEnemy->nHitRock <= 2)
		{
			if (g_pEnemy->nCntFrame % 200 == 100)
			{
				//���ʉ�:�U���\��
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//��ʐU��
				SetQuake(600, 0.01f);
			}
		}
		else if (g_pEnemy->nHitRock <= 3)
		{
			if (g_pEnemy->nCntFrame % 100 == 50)
			{
				//���ʉ�:�U���\��
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//��ʐU��
				SetQuake(600, 0.01f);
			}
		}
		else
		{
			if (g_pEnemy->nCntFrame % 50 == 25)
			{
				//���ʉ�:�U���\��
				PlaySound(SOUND_LABEL_SE_ATTACK_001);

				//��ʐU��
				SetQuake(600, 0.02f);
			}
		}
		break;
	case ENEMYMOTION_STANDUP:
		if (g_pEnemy->nCntKey >= 1 && g_pEnemy->nCntFrame % 10 == 0)
		{
			//���ʉ�:�U���\��
			PlaySound(SOUND_LABEL_SE_ATTACK_000);
		}

		if (g_pEnemy->nCntKey >= 1)
		{
			//��ʐU��
			SetQuake(600, 0.025f);
		}
		break;
	default:
		break;
	}
}

//===========================
//�G�l�~�[��AI(���E)
//===========================
void AiSightEnemy(void)
{
	Player *pPlayer = GetPlayer();
	Object *pObject = ObjectGet();
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//�O�όv�Z�p�ϐ�
	D3DXVECTOR3 posObj[4] = {};														//�O�όv�Z�p�ϐ�
	float vecTemp, vecMax = 0, vecMin = 0, vecObjL, vecObjR, vecObjLength;			//�O�όv�Z���ʊi�[�p�ϐ�
	int nNumR = -1, nNumL = -1;														//�ԍ��ۑ��p�ϐ�
	
	//�������ԑ�������
	g_pEnemy->nFindCount++;

	//�G�l�~�[�̎��E�����߂�v�Z
	pos.x = g_pEnemy->pos.x + sinf(g_pEnemy->rot.y + D3DX_PI - (D3DX_PI * ENEMY_SIGHT));
	pos.z = g_pEnemy->pos.z + cosf(g_pEnemy->rot.y + D3DX_PI - (D3DX_PI * ENEMY_SIGHT));

	//�G�l�~�[���猩�č����̊O��
	g_pEnemy->vecL = OuterProduct(g_pEnemy->pos, pPlayer->pos, pos);

	pos.x = g_pEnemy->pos.x + sinf(g_pEnemy->rot.y + D3DX_PI + (D3DX_PI * ENEMY_SIGHT));
	pos.z = g_pEnemy->pos.z + cosf(g_pEnemy->rot.y + D3DX_PI + (D3DX_PI * ENEMY_SIGHT));

	//�G�l�~�[���猩�ĉE���̊O��
	g_pEnemy->vecR = OuterProduct(g_pEnemy->pos, pPlayer->pos, pos);

	//���E�����̔���
	if (g_pEnemy->vecL <= 0.0f && g_pEnemy->vecR >= 0.0f)
	{
		//�������
		g_pEnemy->bFind = true;

		//��Q������
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (pObject->bUse == true)
			{
				for (int nCntOut = 0; nCntOut < 4; nCntOut++)
				{
					//�I�u�W�F�N�g�̊O���̓_��ݒ�
					posObj[nCntOut].x = pObject->point[nCntOut].x;
					posObj[nCntOut].z = pObject->point[nCntOut].z;

					//�G�l�~�[�ƃI�u�W�F�N�g�̊O��
					vecTemp = OuterProduct(g_pEnemy->pos, pObject->pos, posObj[nCntOut]);

					//�ő�̊O�ς�����1�ԉE�ɂ���_��ۑ�����
					if (vecTemp >= vecMax)
					{
						vecMax = vecTemp;
						nNumR = nCntOut;
					}

					//�ŏ��̊O�ς�����1�ԍ��ɂ���_��ۑ�����
					if (vecTemp <= vecMin)
					{
						vecMin = vecTemp;
						nNumL = nCntOut;
					}
				}

				//�G�l�~�[���猩�ăI�u�W�F�N�g�̉E�[��荶�ɂ��邩�̔���
				vecObjL = OuterProduct(g_pEnemy->pos, pPlayer->pos, posObj[nNumL]);

				//�G�l�~�[���猩�ăI�u�W�F�N�g�̍��[���E�ɂ��邩�̔���
				vecObjR = OuterProduct(g_pEnemy->pos, pPlayer->pos, posObj[nNumR]);

				//�G�l�~�[���猩�ăI�u�W�F�N�g�̎�O�������̔���
				vecObjLength = OuterProduct(posObj[nNumR], pPlayer->pos, posObj[nNumL]);

				//�u���b�N�̉e�ɂ��邩�̔���
				if (vecObjL <= 0.0f && vecObjR >= 0.0f && vecObjLength <= 0.0f && pPlayer->pos.y <= pObject->pos.y + pObject->vtxMax.y && pPlayer->pos.y >= pObject->pos.y + pObject->vtxMin.y)
				{
					//�I�u�W�F�N�g�̏�Ԕ���
					if (pObject->nType == OBJECTTYPE_SANDPOLE || pObject->nType == OBJECTTYPE_SANDPOLE_BASE || pObject->nType == OBJECTTYPE_REFLECTOR_BASE)
					{//�����{�̂ƍ����Ɣ��˔̓y��̎�

						//���肵�Ȃ�
					}
					else 
					{//���̑�

						//�������������
						g_pEnemy->bFind = false;
					}

					//�v�Z�p�ϐ�������
					vecMax = 0.0f;
					vecMin = 0.0f;
					nNumL = 0;
					nNumR = 0;

					break;
				}
			}

			//�v�Z�p�ϐ�������
			vecMax = 0.0f;
			vecMin = 0.0f;
			nNumL = 0;
			nNumR = 0;

			pObject++;
		}
	}
	else
	{
		//�������������
		g_pEnemy->bFind = false;
	}

	//���������ȓ��̎��󋵂Ɋւ�炸�ǐՏ��
	if(g_pEnemy->fLength <= ENEMY_LENGTH)
	{
		//��������
		g_pEnemy->bFind = true;
	}

	//�ǐՏ�Ԏ��J�E���g���Z�b�g����
	if (g_pEnemy->bFind == true)
	{
		if (g_pEnemy->state == ENEMYSTATE_SEARCH || g_pEnemy->state == ENEMYSTATE_NORMAL)
		{
			SetFadeSound(SOUND_LABEL_BGM001, SOUND_LABEL_BGM002,0.2f);
		}

		g_pEnemy->nFindCount = 0;
		g_pEnemy->nTimerMove = 0;
	}

	//�J�E���g�̐��l�ɂ���ď�Ԃ�ύX���鏈��
	if (g_pEnemy->nFindCount == 0)
	{
		//�G�l�~�[�̏�Ԃ�ǐՏ�ԂɕύX
		g_pEnemy->state = ENEMYSTATE_TRACK;
	}
	else if (g_pEnemy->nFindCount < 200)
	{
		//�G�l�~�[�̏�Ԃ�����������ԂɕύX
		g_pEnemy->state = ENEMYSTATE_LOST;

		//�����o�����X�R�A���Z
		if (g_pEnemy->nFindCount == 200 - 1)
		{
			AddScore(SCORE_LOSTFIND);

			SetFadeSound(SOUND_LABEL_BGM002, SOUND_LABEL_BGM001, 0.02f);

			//�擾�X�R�A�\���ݒ�
			SetPointScore(SCORE_LOSTFIND, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f,LOGTYPE_LOST);
		}
	}
	else if (g_pEnemy->nFindCount < 500)
	{
		//�G�l�~�[�̏�Ԃ����G��ԂɕύX
		g_pEnemy->state = ENEMYSTATE_SEARCH;
	}
	else
	{
		//�G�l�~�[�̏�Ԃ�ʏ��ԂɕύX
		g_pEnemy->state = ENEMYSTATE_NORMAL;
		g_pEnemy->nFindCount = 800;
	}

	if (g_pEnemy->motionState == ENEMYMOTION_SHAKE ||
		g_pEnemy->motionState == ENEMYMOTION_PIERCE ||
		g_pEnemy->motionState == ENEMYMOTION_STEP ||
		g_pEnemy->motionState == ENEMYMOTION_THROW)
	{
		//�G�l�~�[�̏�Ԃ����G��ԂɕύX
		g_pEnemy->state = ENEMYSTATE_TRACK;
	}
	else if (g_pEnemy->motionState == ENEMYMOTION_JUMPATTACK || g_pEnemy->motionState == ENEMYMOTION_JUMPATTACK_LIGHT)
	{
		//�G�l�~�[�̏�Ԃ����G��ԂɕύX
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
	
	//���_�̌����A����
	if (g_pEnemy->state == ENEMYSTATE_TRACK)
	{
		g_nCntFind++;

		if (g_nCntFind > ENEMY_FIND_COUNTER)
		{
			//��������
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
//�G�l�~�[��AI(�ړ��n�_)
//===========================
void AiPointEnemy(void)
{
	float fLengthPoint;								//��������p�ϐ�
	int nCntPoint = 0;								//�|�C���g�L�^�p�ϐ�

	//��ԂŃ^�C�}�[�����ύX
	if (g_pEnemy->state == ENEMYSTATE_SEARCH)
	{
		g_pEnemy->nTimerMove = 0;
		g_pEnemy->nFindCount = 200;
	}
	else
	{
		g_pEnemy->nTimerMove++;
	}

	//�|�C���g�ƃG�l�~�[�̋����v�Z
	fLengthPoint = hypotf(g_pEnemy->MovePattern[g_pEnemy->nPattern].point[g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount].x - g_pEnemy->pos.x, g_pEnemy->MovePattern[g_pEnemy->nPattern].point[g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount].z - g_pEnemy->pos.z);

	//���B����
	if (fLengthPoint < ENEMY_ARRIVAL_POINT)
	{
		//�|�C���g�̍X�V
		g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount++;

		if (g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount == 8)
		{
			g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount = 0;
		}

		//�ړ����ԃ��Z�b�g
		g_pEnemy->nTimerMove = 0;

		//�G�l�~�[�̏�Ԃ����G��ԂɕύX
		g_pEnemy->state = ENEMYSTATE_SEARCH;
		g_pEnemy->nFindCount = 200;
	}

	//�����|����h�~
	if (g_pEnemy->nTimerMove >= ENEMY_TIME)
	{//�ړ����Ԃ���莞�Ԃ����������ړ��̃|�C���g��ύX
		g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount = g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount + 1;

		if (g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount == 8)
		{
			g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount = 0;
		}

		//�ړ����ԃ��Z�b�g
		g_pEnemy->nTimerMove = 0;
	}

	//�ړ��n�_�ۑ�
	g_pEnemy->pointMove = g_pEnemy->MovePattern[g_pEnemy->nPattern].point[g_pEnemy->MovePattern[g_pEnemy->nPattern].posCount];
}

//===========================
//�G�l�~�[��AI(����)
//===========================
void AiLengthEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//�v���C���[�ƃG�l�~�[�̋����v�Z
	g_pEnemy->fLength = hypotf(pPlayer->pos.x - g_pEnemy->pos.x, pPlayer->pos.z - g_pEnemy->pos.z);
}

//===========================
//�G�l�~�[��AI(�p�x)
//===========================
void AiRotEnemy(void)
{
	float fRotMove, fRotDest, fRotDiff;				//�p�x�����p�ϐ�

	//�p�x����
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

	//�����ǉ�
	fRotMove += fRotDiff * 0.1f;

	//�p�x��v����
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
//�G�l�~�[��AI(�ړ�)
//===========================
void AiMoveEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//�ړ��|�C���g���v���C���[�ɐݒ�
	g_pEnemy->pointMove = pPlayer->pos;

	//�������߂��Ǝ~�܂邻��ȊO���Ɠ���
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
//�G�l�~�[��AI(�U��)
//===========================
void AiAttackEnemy(void)
{
	Player *pPlayer = GetPlayer();

	//��莞�ԂōU��
	if (g_pEnemy->nAttackCount >= g_pEnemy->nAttackTime && (g_pEnemy->motionState == ENEMYMOTION_MOVE || g_pEnemy->motionState == ENEMYMOTION_NORMAL || g_pEnemy->motionState == ENEMYMOTION_OVERLOOK))
	{
		int nAttack = rand() % 101;
		
		//�L�[�ƃt���[�������Z�b�g
		g_pEnemy->nCntKey = 0;
		g_pEnemy->nCntFrame = 0;

		//�����Ɨ����ōU�����[�V����������
		if (g_pEnemy->fLength <= ENEMY_LENGTH)
		{
			if (nAttack > 50)
			{//�U�艺�낵���[�V����
				SetMotionEnemy(ENEMYMOTION_SHAKE);
			}
			else if (nAttack > 30)
			{//�˂��h�����[�V����
				SetMotionEnemy(ENEMYMOTION_PIERCE);
			}
			else
			{//�����݃��[�V����
				SetMotionEnemy(ENEMYMOTION_STEP);
			}
		}
		else if (g_pEnemy->fLength <= ENEMY_LENGTH * 5.0f && CollisionEnemyPoint(&g_pEnemy->pos) == true)
		{
			if (nAttack > 60)
			{//�˂��h�����[�V����
				SetMotionEnemy(ENEMYMOTION_PIERCE);
			}
			else if (nAttack > 20)
			{//�U�艺�낵���[�V����
				SetMotionEnemy(ENEMYMOTION_SHAKE);
			}
			else
			{//���΂����[�V����
				SetMotionEnemy(ENEMYMOTION_THROW);
			}
		}
		else if (g_pEnemy->fLength <= ENEMY_LENGTH * 5.0f)
		{
			if (nAttack > 50)
			{//�˂��h�����[�V����
				SetMotionEnemy(ENEMYMOTION_PIERCE);
			}
			else if (nAttack > 30)
			{//�U�艺�낵���[�V����
				SetMotionEnemy(ENEMYMOTION_SHAKE);
			}
			else if (nAttack > 10)
			{//���΂����[�V����
				SetMotionEnemy(ENEMYMOTION_THROW);
			}
			else
			{//�W�����v�U�����[�V����
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
			//���΂����[�V����
			SetMotionEnemy(ENEMYMOTION_THROW);
		}
		else
		{
			if (nAttack > 70)
			{//���΂����[�V����
				SetMotionEnemy(ENEMYMOTION_THROW);
			}
			else
			{//�W�����v�U�����[�V����
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
//���[�V�����̐ݒ�
//===========================
void SetMotionEnemy(ENEMYMOTION motion)
{
	Player *pPlayer = GetPlayer();

	g_pEnemy->motionState = motion;

	//�U�����[�V�����ꗗ
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