//===========================
//
//�G�l�~�[�֘A����
//Author:�����@��
//
//===========================

//==========================================
//  �C���N���[�h
//==========================================
#include "main.h"
#include "enemy.h"
#include "enemyAI.h"
#include "player.h"
#include "object.h"
#include "rock.h"
#include "shock.h"
#include <stdio.h>
#include "input.h"
#include "gauge.h"
#include "shadow.h"
#include "score.h"
#include "wall.h"
#include "orbit.h"
#include "particle.h"
#include "sound.h"
#include "game.h"
#include "tutorial.h"
#include "count.h"
#include "model.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_ENEMY (64)					//�G�l�~�[�̍ő吔
#define ENEMY_INERTIA (0.15f)			//�ړ����x�̊���
#define ENEMY_GRAVITY (0.8f)			//���̃|�C���g�ɐ؂�ւ��鎞��
#define SHADOW_RADIUS	(120.0f)		//�e�̔��a
#define ENEMY_COLLISION (100.0f)		//���̃|�C���g�ɐ؂�ւ��鎞��
#define JUMP_DAMAGE	(50)	//�G���o�b�N�W�����v����З�
#define TUTORIAL_HIT_COUNTER	(3)		//�`���[�g���A���J�ڂ���q�b�g��

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void MoveEnemy(void);
void ModelMotion(void);

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
Enemy g_enemy;					//�G�l�~�[�\����
int g_nTutorialHitCtr;			//�`���[�g���A�����q�b�g��

//===========================
//�G�l�~�[�̏���������
//===========================
void InitEnemy(void)
{
	//�ϐ�������
	ZeroMemory(&g_enemy,sizeof(Enemy));

	//�p�[�c���擾
	g_enemy.nNumModel = GetNumEnemyPart();

	//�t�@�C���ǂݍ���
	ReadFileEnemy();

	//���l������
	g_enemy.nFindCount = 400;											//��������
	g_enemy.nLife = ENEMY_LIFE;													//�̗�
	g_enemy.fLength = 300.0f;
	g_enemy.fSpeed = ENEMY_SPEED;
	g_nTutorialHitCtr = 0;

	for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
	{
		g_enemy.aEnemyX[nCntModel].pos = g_enemy.aEnemyX[nCntModel].posMotion;
		g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyX[nCntModel].rotMotion;
		g_enemy.aEnemyX[nCntModel].pos += g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel];
		g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel];
	}

	InitEnemyAi();

	if (GetMode() == MODE_GAME)
	{
		// �̗̓Q�[�W�̐ݒ�
		g_enemy.nIdxGauge = SetGauge(D3DXVECTOR3(80.0f, 680.0f, 0.0f), (float)g_enemy.nLife, 50.0f, 1100.0f, GAUGETYPE_HORIZON,ENEMY_LIFE_CR);
	}
	else
	{
		g_enemy.nIdxGauge = -1;
	}
}

//===========================
//�G�l�~�[�̏I������
//===========================
void UninitEnemy(void)
{
	for (int nCnt = 0; nCnt < g_enemy.nNumModel; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_enemy.aEnemyX[nCnt].pMeshEnemy != NULL)
		{
			g_enemy.aEnemyX[nCnt].pMeshEnemy->Release();
			g_enemy.aEnemyX[nCnt].pMeshEnemy = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_enemy.aEnemyX[nCnt].pBuffMatEnemy != NULL)
		{
			g_enemy.aEnemyX[nCnt].pBuffMatEnemy->Release();
			g_enemy.aEnemyX[nCnt].pBuffMatEnemy = NULL;
		}

		for (int nCntMat = 0;nCntMat < (int)g_enemy.aEnemyX[nCnt].dwNumMatEnemy;nCntMat++)
		{//�e�N�X�`���̔j��
			if (g_enemy.aEnemyX[nCnt].apTextureEnemy[nCntMat] != NULL)
			{
				g_enemy.aEnemyX[nCnt].apTextureEnemy[nCntMat]->Release();
				g_enemy.aEnemyX[nCnt].apTextureEnemy[nCntMat] = NULL;
			}
		}
	}
}

//===========================
//�G�l�~�[�̍X�V����
//===========================
void UpdateEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_enemy.bUse == true && g_enemy.bDeath == false)
	{
		g_enemy.motionStateOld = g_enemy.motionState;
		g_enemy.stateOld = g_enemy.state;
		g_enemy.posOld = g_enemy.pos;

		MoveEnemy();

		CollisionRockEnemy(&g_enemy.pos, &g_enemy.posOld, &g_enemy.pointMove, g_enemy.aEnemyX[0].vtxMinEnemy, g_enemy.aEnemyX[0].vtxMaxEnemy, false);

		CollisionWall(&g_enemy.pos, &g_enemy.posOld, &g_enemy.move, ENEMY_RADIUS);

		ModelMotion();

		if (g_enemy.nLife <= 0)
		{//�G�̎��S
			g_enemy.nLife = 0;
		}

		if (GetMode() == MODE_GAME)
		{
			//�̗̓Q�[�W�X�V
			SetInfoGauge(g_enemy.nIdxGauge, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), (float)g_enemy.nLife);
		}

		//�e�̈ʒu�X�V
		SetPositionShadow(g_enemy.nIdxShadow, D3DXVECTOR3(g_enemy.aEnemyX[0].mtxWorld._41, 0.2f, g_enemy.aEnemyX[0].mtxWorld._43),g_enemy.rot, 1.0f);

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_0))
		{
			g_enemy.bUse = false;
		}

		if (GetKeyboardPress(DIK_B) == true)
		{//W�L�[�������ꂽ��
			HitEnemy(1, 0);
		}

		if (GetKeyboardPress(DIK_LSHIFT) == true &&
			GetKeyboardTrigger(DIK_B) == true)
		{//W�L�[�������ꂽ��
			HitEnemy(10000, 0);
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true &&
			GetKeyboardPress(DIK_B) == true)
		{//W�L�[�������ꂽ��
			HitEnemy(-20, 0);
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true &&
			GetKeyboardPress(DIK_LSHIFT) == true &&
			GetKeyboardTrigger(DIK_N) == true)
		{//W�L�[�������ꂽ��
			HitEnemy(60, 0);
		}

		if (GetKeyboardPress(DIK_LCONTROL) == true &&
			GetKeyboardPress(DIK_LSHIFT) == true && 
			GetKeyboardPress(DIK_X) == true)
		{//W�L�[�������ꂽ��
			g_enemy.nAttackCount += 200;
		}
#endif
	}
}

//===========================
//�G�l�~�[�̕`�揈��
//===========================
void DrawEnemy(void)
{
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (g_enemy.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_enemy.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_enemy.rot.y, g_enemy.rot.x, g_enemy.rot.z);
		D3DXMatrixMultiply(&g_enemy.mtxWorld, &g_enemy.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_enemy.pos.x, g_enemy.pos.y, g_enemy.pos.z);
		D3DXMatrixMultiply(&g_enemy.mtxWorld, &g_enemy.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_enemy.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//���擾
		Model *pEnemyPart = GetModelEnemy();

		for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//�v�Z�p�}�g���b�N�X

			//�p�[�c�̃��[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_enemy.aEnemyX[nCntModel].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
				g_enemy.aEnemyX[nCntModel].rot.y, g_enemy.aEnemyX[nCntModel].rot.x, g_enemy.aEnemyX[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_enemy.aEnemyX[nCntModel].mtxWorld, &g_enemy.aEnemyX[nCntModel].mtxWorld, &mtxRotModel);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTransModel,
				g_enemy.aEnemyX[nCntModel].pos.x, g_enemy.aEnemyX[nCntModel].pos.y, g_enemy.aEnemyX[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_enemy.aEnemyX[nCntModel].mtxWorld, &g_enemy.aEnemyX[nCntModel].mtxWorld, &mtxTransModel);

			//�p�[�c�̐e�}�g���b�N�X��ݒ�
			if (g_enemy.aEnemyX[nCntModel].nIdxModelParent != -1)
			{
				mtxPalent = g_enemy.aEnemyX[g_enemy.aEnemyX[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{
				mtxPalent = g_enemy.mtxWorld;
			}

			//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
			D3DXMatrixMultiply(&g_enemy.aEnemyX[nCntModel].mtxWorld,
				&g_enemy.aEnemyX[nCntModel].mtxWorld,
				&mtxPalent);

			pDevice->SetTransform(D3DTS_WORLD, &g_enemy.aEnemyX[nCntModel].mtxWorld);

			//�}�e���A���̃f�[�^�̃|�C���g���擾
			pMat = (D3DXMATERIAL*)pEnemyPart[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pEnemyPart[nCntModel].dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, pEnemyPart[nCntModel].apTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				pEnemyPart[nCntModel].pMesh->DrawSubset(nCntMat);
			}
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//===========================
//�G�l�~�[�̃f�o�b�O�\��
//===========================
void DrawEnemyDebug(void)
{
	//�G�̈ʒu
	DebugEnemyPos();

	//�ړ���
	DebugEnemyPoint();

	//�O��
	DebugEnemyOuter();

	//��������
	DebugEnemyFind();

	//����
	DebugEnemyLength();

	//�p�x
	DebugEnemyRot();

	//���[�V����
	DebugEnemyMotion();

	//���
	DebugEnemyState();
}

//===========================
//�G�l�~�[�̔z�u����
//===========================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nType)
{
	if (g_enemy.bUse == false)
	{
		g_enemy.pos = pos;						//�ʒu
		g_enemy.move = move;					//�ړ���
		g_enemy.rot = rot;						//�p�x
		if (GetMode() == MODE_TUTORIAL)
		{
			g_enemy.state = ENEMYSTATE_NORMAL;
			SetMotionEnemy(ENEMYMOTION_NORMAL);
			g_enemy.motionStateOld = ENEMYMOTION_NORMAL;
		}
		else
		{
			g_enemy.state = ENEMYSTATE_SLEEP;
			SetMotionEnemy(ENEMYMOTION_SLEEP);
			g_enemy.motionStateOld = ENEMYMOTION_SLEEP;
		}
		g_enemy.bDown = false;
		g_enemy.nIdxOrbit = -1;
		g_enemy.nPattern = rand() % g_enemy.nNumPattern;

		for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
		{
			g_enemy.aEnemyX[nCntModel].pos = g_enemy.aEnemyX[nCntModel].posMotion + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel];
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyX[nCntModel].rotMotion;
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel];
		}

		// �e�̐ݒ�
		g_enemy.nIdxShadow = SetShadow(SHADOW_RADIUS, SHADOWTYPE_CIRCLE, 0.0f, 0.0f, g_enemy.rot);

		//�g���Ă邩�ǂ���
		g_enemy.bUse = true;
	}
}

//===========================
//�G�l�~�[�̈ʒu�E�������ߏ���
//===========================
void SetPosEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_enemy.pos = pos;
	g_enemy.rot = rot;
}

//===========================
//�G�l�~�[�̏��擾����
//===========================
Enemy *GetEnemy(void)
{
	return &g_enemy;
}

//===========================
//�G�l�~�[�ƓG�̓����蔻�菈��
//===========================
bool CollisionEnemy(D3DXVECTOR3 *posMin, D3DXVECTOR3 *posMax)
{
	bool bCollision;

	bCollision = false;

	if (g_enemy.bUse == true)
	{
		if (g_enemy.fLength <= ENEMY_COLLISION && posMax->y >= g_enemy.pos.y && posMin->y <= g_enemy.aEnemyX[2].mtxWorld._42)
		{
			bCollision = true;
		}
	}

	return bCollision;
}

//===========================
//�G�l�~�[�̍U�������蔻�菈��
//===========================
bool CollisionEnemyAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot)
{
	bool bAttack;
	float fLength1, fLength2;
	D3DXVECTOR3 posFocus1, posFocus2;
	D3DXMATRIX mtxFocus1, mtxFocus2;
	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	bAttack = false;
	
	posFocus1 = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	posFocus2 = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

	//�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxFocus1);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posFocus1.x, posFocus1.y, posFocus1.z);
	D3DXMatrixMultiply(&mtxFocus1, &mtxFocus1, &mtxTrans);

	//�}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&mtxFocus1, &mtxFocus1, &g_enemy.aEnemyX[13].mtxWorld);

	//�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxFocus2);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, posFocus2.x, posFocus2.y, posFocus2.z);
	D3DXMatrixMultiply(&mtxFocus2, &mtxFocus2, &mtxTrans);

	//�}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&mtxFocus2, &mtxFocus2, &g_enemy.aEnemyX[13].mtxWorld);

	posFocus1.x = mtxFocus1._41;
	posFocus1.y = mtxFocus1._42;
	posFocus1.z = mtxFocus1._43;
	posFocus2.x = mtxFocus2._41;
	posFocus2.y = mtxFocus2._42;
	posFocus2.z = mtxFocus2._43;

	fLength1 = hypotf(hypotf(pos->x - posFocus1.x, pos->y - posFocus1.y), pos->z - posFocus1.z);
	fLength2 = hypotf(hypotf(pos->x - posFocus2.x, pos->y - posFocus2.y), pos->z - posFocus2.z);

	if (fLength1 + fLength2 <= 250.0f && g_enemy.motionState == ENEMYMOTION_SHAKE && (g_enemy.nCntKey == 2 || g_enemy.nCntKey == 3))
	{
		rot->y = atan2f(pos->x - ((posFocus1.x + posFocus2.x) * 0.5f), pos->z - ((posFocus1.z + posFocus2.z) * 0.5f));

		bAttack = true;
	}

	return bAttack;
}

//===========================
//�G�l�~�[�̍s���͈͓��O���菈��
//===========================
bool CollisionEnemyPoint(D3DXVECTOR3 *pos)
{
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 vecLine, vecToPos, vecMove;
	float fRate, fOutToPos, fOutUnit, fOutRate;
	bool bPoint;
	int nPoint = 0;

	bPoint = false;

	for (int nCntPoint = 0; nCntPoint < g_enemy.MovePattern[0].nNumPoint; nCntPoint++)
	{
		if (nCntPoint == g_enemy.MovePattern[0].nNumPoint - 1)
		{
			if (OuterProduct(g_enemy.MovePattern[0].point[nCntPoint], *pos, g_enemy.MovePattern[0].point[0]) < 0.0f)
			{
				//��_�ƃv���C���[�̈ʒu�̃x�N�g��
				vecToPos.x = pPlayer->pos.x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecToPos.z = pPlayer->pos.z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//���ʂ̃x�N�g��
				vecLine.x = g_enemy.MovePattern[0].point[0].x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecLine.z = g_enemy.MovePattern[0].point[0].z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//�ړ��ʃx�N�g��
				vecMove = pPlayer->pos - *pos;

				//���ʂƃv���C���[�̈ʒu�̊O��
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

				//��_�v�Z
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				if (fRate > 0.0f && fRate < 1.0f)
				{
					bPoint = true;
					break;
				}
			}
		}
		else
		{
			if (OuterProduct(g_enemy.MovePattern[0].point[nCntPoint], *pos, g_enemy.MovePattern[0].point[nCntPoint + 1]) < 0.0f)
			{
				//��_�ƃv���C���[�̈ʒu�̃x�N�g��
				vecToPos.x = pPlayer->pos.x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecToPos.z = pPlayer->pos.z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//���ʂ̃x�N�g��
				vecLine.x = g_enemy.MovePattern[0].point[nCntPoint + 1].x - g_enemy.MovePattern[0].point[nCntPoint].x;
				vecLine.z = g_enemy.MovePattern[0].point[nCntPoint + 1].z - g_enemy.MovePattern[0].point[nCntPoint].z;

				//�ړ��ʃx�N�g��
				vecMove = pPlayer->pos - *pos;

				//���ʂƃv���C���[�̈ʒu�̊O��
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

				//��_�v�Z
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				if (fRate > 0.0f && fRate < 1.0f && fOutToPos > 0.0f)
				{
					bPoint = true;
					break;
				}
			}
		}
	}

	return bPoint;
}

//===========================
//�G�l�~�[�̈ړ�����
//===========================
void MoveEnemy(void)
{
	//�����̌����Ă�����ɐi��
	g_enemy.move.x += sinf(g_enemy.rot.y + D3DX_PI) * g_enemy.fSpeed;
	g_enemy.move.z += cosf(g_enemy.rot.y + D3DX_PI) * g_enemy.fSpeed;

	//�d��
	g_enemy.move.y -= ENEMY_GRAVITY;

	//����
	g_enemy.move.x += (0.0f - g_enemy.move.x) * ENEMY_INERTIA;
	g_enemy.move.z += (0.0f - g_enemy.move.z) * ENEMY_INERTIA;

	if (GetMode() == MODE_TUTORIAL)
	{
		//AI
		AiEnemyTutorial();
	}
	else
	{
		//AI
		AiEnemy();
	}

	//�ړ��ʔ��f
	g_enemy.pos += g_enemy.move;

	if (g_enemy.pos.y < 0.0f)
	{
		g_enemy.move.y = 0.0f;
		g_enemy.pos.y = 0.0f;
	}

	//�u���b�N�Ɠ����蔻�菈��
	if (CollisionObject2(&g_enemy.pos, &g_enemy.posOld, &g_enemy.move, g_enemy.aEnemyX[0].vtxMinEnemy, g_enemy.aEnemyX[0].vtxMaxEnemy, false) == true)
	{
		
	}
	else
	{

	}

	//CollisionReflector(&g_enemy.pos, &g_enemy.posOld, &g_enemy.move, 100.0f, 0);
}

//===========================
//�G�l�~�[�̃��[�V��������
//===========================
void ModelMotion(void)
{
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 posDiff, rotDiff;

	//�L�[�����w��̒l��菭�Ȃ�
	if (g_enemy.nCntKey < g_enemy.aEnemyMotion[g_enemy.motionState].nNumKey)
	{
		//�t���[�����w��̒l��菭�Ȃ�
		if (g_enemy.nCntFrame < g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame)
		{
			//�L�[�����ő���1���Ȃ�
			if (g_enemy.nCntKey == g_enemy.aEnemyMotion[g_enemy.motionState].nNumKey - 1)
			{
				//���f�����Ƃ�pos��rot�𑫂�
				for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
				{
					if (g_enemy.aEnemyMotion[g_enemy.motionState].nLoop == 0)
					{
						posDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x;
						posDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y;
						posDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z;

						rotDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x;
						rotDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y;
						rotDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z;
						
						//pos�𑫂�
						g_enemy.aEnemyX[nCntModel].pos.x = g_enemy.aEnemyX[nCntModel].posMotion.x + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x + (posDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.y = g_enemy.aEnemyX[nCntModel].posMotion.y + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y + (posDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.z = g_enemy.aEnemyX[nCntModel].posMotion.z + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z + (posDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						
						//rot�𑫂�
						g_enemy.aEnemyX[nCntModel].rot.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x + (rotDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y + (rotDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z + (rotDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					}
					else
					{
						posDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].pos[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x;
						posDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].pos[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y;
						posDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].pos[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z;

						rotDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].rot[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x;
						rotDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].rot[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y;
						rotDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[0].rot[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z;

						//pos�𑫂�
						g_enemy.aEnemyX[nCntModel].pos.x = g_enemy.aEnemyX[nCntModel].posMotion.x + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x + (posDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.y = g_enemy.aEnemyX[nCntModel].posMotion.y + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y + (posDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].pos.z = g_enemy.aEnemyX[nCntModel].posMotion.z + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z + (posDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));

						//rot�𑫂�
						g_enemy.aEnemyX[nCntModel].rot.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x + (rotDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y + (rotDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
						g_enemy.aEnemyX[nCntModel].rot.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z + (rotDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					}

					//rot�̏C��
					if (g_enemy.aEnemyX[nCntModel].rot.x > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.x < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.y > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.y < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.z > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.z < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				//���f�����Ƃ�pos��rot�𑫂�
				for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
				{
					posDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].pos[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x;
					posDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].pos[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y;
					posDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].pos[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z;

					rotDiff.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].rot[nCntModel].x - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x;
					rotDiff.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].rot[nCntModel].y - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y;
					rotDiff.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey + 1].rot[nCntModel].z - g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z;

					//pos�𑫂�
					g_enemy.aEnemyX[nCntModel].pos.x = g_enemy.aEnemyX[nCntModel].posMotion.x + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].x + posDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame);
					g_enemy.aEnemyX[nCntModel].pos.y = g_enemy.aEnemyX[nCntModel].posMotion.y + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].y + posDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame);
					g_enemy.aEnemyX[nCntModel].pos.z = g_enemy.aEnemyX[nCntModel].posMotion.z + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel].z + posDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame);

					//rot�𑫂�
					g_enemy.aEnemyX[nCntModel].rot.x = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].x + (rotDiff.x * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					g_enemy.aEnemyX[nCntModel].rot.y = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].y + (rotDiff.y * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));
					g_enemy.aEnemyX[nCntModel].rot.z = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel].z + (rotDiff.z * (g_enemy.nCntFrame / (float)g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].nFrame));

					//rot�̏C��
					if (g_enemy.aEnemyX[nCntModel].rot.x > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.x < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.x += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.y > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.y < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.y += 6.28f;
					}

					if (g_enemy.aEnemyX[nCntModel].rot.z > 3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z -= 6.28f;
					}
					else if (g_enemy.aEnemyX[nCntModel].rot.z < -3.14f)
					{
						g_enemy.aEnemyX[nCntModel].rot.z += 6.28f;
					}
				}
			}

			//�t���[���𑝂₷
			g_enemy.nCntFrame++;
		}
		else
		{
			//�L�[�𑝂₷
			g_enemy.nCntKey++;

			//�t���[�������Z�b�g����
			g_enemy.nCntFrame = 0;

			//rot���C��
			for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
			{
				g_enemy.aEnemyX[nCntModel].rotMotion = g_enemy.aEnemyX[nCntModel].rot;
			}
		}
	}
	else
	{
		//���[�v���Ă邩�ǂ����̔���
		if (g_enemy.aEnemyMotion[g_enemy.motionState].nLoop == 0)
		{
			g_enemy.nAttackCount = 0;

			//���̃��[�V��������
			if (g_enemy.state == ENEMYSTATE_TRACK)
			{
				if (CollisionEnemyPoint(&g_enemy.pos) == true)
				{
					SetMotionEnemy(ENEMYMOTION_NORMAL);
				}
				else
				{
					SetMotionEnemy(ENEMYMOTION_MOVE);
				}

				if (g_enemy.fLength <= 100.0f)
				{
					SetMotionEnemy(ENEMYMOTION_NORMAL);
				}
			}
			else if (g_enemy.state == ENEMYSTATE_DAMAGE)
			{
				if (g_enemy.motionState == ENEMYMOTION_DAMAGE)
				{
					SetMotionEnemy(ENEMYMOTION_NORMAL);
					g_enemy.state = ENEMYSTATE_COUNTER;
				}
				else if (g_enemy.motionState == ENEMYMOTION_DOWN)
				{
					SetMotionEnemy(ENEMYMOTION_OVERLOOK);
					g_enemy.state = ENEMYSTATE_SEARCH;
				}
			}
			else if (g_enemy.state == ENEMYSTATE_COUNTER)
			{
				if (g_enemy.motionState == ENEMYMOTION_BACKJUMP)
				{
					if (g_enemy.bFind == true)
					{
						g_enemy.state = ENEMYSTATE_TRACK;
						SetMotionEnemy(ENEMYMOTION_NORMAL);
					}
					else
					{
						SetMotionEnemy(ENEMYMOTION_THROW);
					}
				}
				else
				{
					g_enemy.state = ENEMYSTATE_NORMAL;
					SetMotionEnemy(ENEMYMOTION_NORMAL);
				}
			}
			else if (g_enemy.state == ENEMYSTATE_FIND)
			{
				g_enemy.state = ENEMYSTATE_TRACK;
				SetMotionEnemy(ENEMYMOTION_NORMAL);
			}
			else if(g_enemy.state == ENEMYSTATE_DEATH)
			{
				g_enemy.bDeath = true;					//�g�p���Ă��Ȃ���Ԃɂ���
			}
			else if (g_enemy.state == ENEMYSTATE_STANDUP)
			{
				g_enemy.state = ENEMYSTATE_SEARCH;
				SetMotionEnemy(ENEMYMOTION_OVERLOOK);
			}
			else
			{
				SetMotionEnemy(ENEMYMOTION_NORMAL);
			}
		}

		//�L�[�ƃt���[�������Z�b�g
		g_enemy.nCntKey = 0;
		g_enemy.nCntFrame = 0;
	}

	//���[�V�������؂�ւ�������Ƃ̔���
	if (g_enemy.motionState != g_enemy.motionStateOld)
	{
		g_enemy.nCntKey = 0;
		g_enemy.nCntFrame = 0;

		for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
		{
			g_enemy.aEnemyX[nCntModel].pos = g_enemy.aEnemyX[nCntModel].posMotion + g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].pos[nCntModel];
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyX[nCntModel].rotMotion;
			g_enemy.aEnemyX[nCntModel].rot = g_enemy.aEnemyMotion[g_enemy.motionState].aKeyMotion[g_enemy.nCntKey].rot[nCntModel];
		}
	}
}

//========================================
//�G�l�~�[�̈ʒu
//========================================
void DebugEnemyPos(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�G�l�~�[�̈ʒu; %0.3f %0.3f %0.3f\n�G�l�~�[�̈ړ���; %0.3f %0.3f %0.3f", g_enemy.pos.x, g_enemy.pos.y, g_enemy.pos.z, g_enemy.move.x, g_enemy.move.y, g_enemy.move.z);
	
	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̈ړ��|�C���g
//========================================
void DebugEnemyPoint(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,440,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�ړ���; %d\n�ړ�����; %d", g_enemy.MovePattern[g_enemy.nPattern].posCount, g_enemy.nTimerMove);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̊O��
//========================================
void DebugEnemyOuter(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,480,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�O��;L %0.3f | R %0.3f", g_enemy.vecL, g_enemy.vecR);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̔�������
//========================================
void DebugEnemyFind(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,500,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "����; %d(0:�͈͊O | 1:�͈͓�)\n��������; %d", g_enemy.bFind, g_enemy.nFindCount);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̋���
//========================================
void DebugEnemyLength(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,540,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "����; %0.3f", g_enemy.fLength);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̊p�x
//========================================
void DebugEnemyRot(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,560,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�p�x; %0.3f %0.3f %0.3f", g_enemy.rot.x, g_enemy.rot.y, g_enemy.rot.z);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̃��[�V����
//========================================
void DebugEnemyMotion(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,620,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "���̃��[�V����(�G�l�~�[); %d\n�O�̃��[�V����(�G�l�~�[); %d", g_enemy.motionState, g_enemy.motionStateOld);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�G�l�~�[�̏��
//========================================
void DebugEnemyState(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,660,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "���̏��; %d\n�L�[��,�t���[����; %d | %d", g_enemy.state, g_enemy.nCntKey, g_enemy.nCntFrame);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//========================================
//�O�όv�Z�p�֐�(�_A����L�т�2�̃x�N�g��(AB��AC)�̊O��)
//�_B������AC�̍��ɂ���Ƃ��̓v���X�A�_B������AC�̉E�ɂ���Ƃ��̓}�C�i�X�̒l��Ԃ�������ɂ���ꍇ0��Ԃ�
//========================================
float OuterProduct(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	D3DXVECTOR3 vecLine1, vecLine2;
	float vecAnswer;

	//�_AB�̃x�N�g��
	vecLine1.x = posB.x - posA.x;
	vecLine1.z = posB.z - posA.z;

	//�_AC�̃x�N�g��
	vecLine2.x = posC.x - posA.x;
	vecLine2.z = posC.z - posA.z;

	//�v�Z�����x�N�g��
	vecAnswer = (vecLine1.z * vecLine2.x) - (vecLine1.x * vecLine2.z);

	return vecAnswer;
}

//========================================
//�G�l�~�[�̃q�b�g����
//========================================
void HitEnemy(int nDamage, int nCntEnemy)
{
	//�`���[�g���A����i�߂�
	TUTORIALSTATE *tutorial = GetTutorialState();
	if (*tutorial == TUTORIALSTATE_ENEMY && GetMode() == MODE_TUTORIAL)
	{
		AddCount(1);

		if (g_nTutorialHitCtr < TUTORIAL_HIT_COUNTER)
		{
			g_nTutorialHitCtr++;
		}
		else
		{
			*tutorial = TUTORIALSTATE_END;

			PlaySound(SOUND_LABEL_SE_BUTTON_004);
		}
	}

	if (g_enemy.state != ENEMYSTATE_DAMAGE)
	{
		//�̗͂����炷
		g_enemy.nLife -= nDamage;

		if (g_enemy.state == ENEMYSTATE_SLEEP)
		{
			g_enemy.nHitRock++;
		}
	}

	//�G�̗̑͂��Ȃ��Ȃ���
	if (g_enemy.nLife <= 0 && g_enemy.state != ENEMYSTATE_DEATH)
	{

		if (GetMode() == MODE_TUTORIAL)
		{
			// �̗͂�1�ŌŒ�
			g_enemy.nLife = 1;
		}
		else
		{
			g_enemy.state = ENEMYSTATE_DEATH;

			if (g_enemy.nIdxOrbit != -1)
			{
				//�O�Ղ̍폜
				EnableOrbit(g_enemy.nIdxOrbit);
				g_enemy.nIdxOrbit = -1;
			}

			//�L�[�ƃt���[�������Z�b�g
			g_enemy.nCntKey = 0;
			g_enemy.nCntFrame = 0;

			// �̗͂��O�ŌŒ�
			g_enemy.nLife = 0;
		}
	}
	
	if(g_enemy.state != ENEMYSTATE_DEATH)
	{
		if (nDamage > JUMP_DAMAGE)
		{//���ރ_���[�W���傫��������
			if (g_enemy.motionState != ENEMYMOTION_BACKJUMP && g_enemy.motionState != ENEMYMOTION_JUMPATTACK && g_enemy.motionState != ENEMYMOTION_JUMPATTACK_LIGHT)
			{
				if (g_enemy.state == ENEMYSTATE_SLEEP)
				{
					g_enemy.nHitRock = 5;
				}
				else
				{
					g_enemy.state = ENEMYSTATE_DAMAGE;

					//�_���[�W���[�V�����ֈڍs
					g_enemy.motionState = ENEMYMOTION_DAMAGE;

					if (g_enemy.nLife <= (int)(ENEMY_LIFE * 0.25f) && g_enemy.bDown == false)
					{//�_�E�����[�V�����ֈڍs
						g_enemy.motionState = ENEMYMOTION_DOWN;
						g_enemy.bDown = true;
					}
				}
				
				if (g_enemy.nIdxOrbit != -1)
				{
					//�O�Ղ̍폜
					EnableOrbit(g_enemy.nIdxOrbit);
					g_enemy.nIdxOrbit = -1;
				}

				//�L�[�ƃt���[�������Z�b�g
				g_enemy.nCntKey = 0;
				g_enemy.nCntFrame = 0;

				//�ړ��p�^�[���̕ύX
				g_enemy.nPattern = rand() % g_enemy.nNumPattern;
			}
		}
	}
}

//===========================
//�G�l�~�[�̃t�@�C���ǂݍ��ݏ���
//===========================
void ReadFileEnemy(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_golem.txt", "r");

	//�t�@�C���ǂݍ���
	if (pFile == NULL)
	{
		return;
	}

	do
	{
		fscanf(pFile, "%s", &aText[0]);
	} while (strcmp(aText, "CHARACTERSET") != 0);

	//���f����
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_enemy.nNumModel, &aText[0], &aText[0]);

	//���f���z�u
	for (int nCntModel = 0; nCntModel < g_enemy.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_enemy.aEnemyX[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_enemy.aEnemyX[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_enemy.aEnemyX[nCntModel].posMotion.x, &g_enemy.aEnemyX[nCntModel].posMotion.y, &g_enemy.aEnemyX[nCntModel].posMotion.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_enemy.aEnemyX[nCntModel].rotMotion.x, &g_enemy.aEnemyX[nCntModel].rotMotion.y, &g_enemy.aEnemyX[nCntModel].rotMotion.z);
		fgets(&aText[0], 128, pFile);
		fgets(&aText[0], 128, pFile);
	}

	//����Ȃ������ǂݍ���
	for (int nCnt = 0; nCnt < 7; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCntMotion = 0; nCntMotion < ENEMYMOTION_MAX; nCntMotion++)
	{
		//���[�V��������
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d	%s %s %s %s", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].nLoop, &aText[0], &aText[0], &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);
		
		//�L�[���[�V����
		for (int nCntKey = 0; nCntKey < g_enemy.aEnemyMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_enemy.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_enemy.aEnemyMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
				fgets(&aText[0], 128, pFile);
				fgets(&aText[0], 128, pFile);
			}

			fgets(&aText[0], 128, pFile);
		}

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			fgets(&aText[0], 128, pFile);
		}
	}

	fclose(pFile);

	pFile = fopen("data\\SAVE\\ENEMY\\enemy.txt", "r");		//�t�@�C���I�[�v��

	//�t�@�C���ǂݍ���
	if (pFile == NULL)
	{
		return;
	}

	//����Ȃ������ǂݍ���
	for (int nCnt = 0; nCnt < 16; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	//�ړ��n�_�̐�
	fscanf(pFile, "%s = %d		%s %s\n", &aText[0], &g_enemy.nNumPattern, &aText[0], &aText[0]);

	//�ړ��n�_�̍��W
	for (int nCntPattern = 0; nCntPattern < g_enemy.nNumPattern; nCntPattern++)
	{
		//����Ȃ������ǂݍ���
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			fgets(&aText[0], 128, pFile);
		}

		fscanf(pFile, "%s = %d		%s %s\n", &aText[0], &g_enemy.MovePattern[nCntPattern].nNumPoint, &aText[0], &aText[0]);

		for (int nCnt = 0; nCnt < g_enemy.MovePattern[nCntPattern].nNumPoint; nCnt++)
		{
			fscanf(pFile, "%s = %f %f %f		%s %s\n", &aText[0], &g_enemy.MovePattern[nCntPattern].point[nCnt].x, &g_enemy.MovePattern[nCntPattern].point[nCnt].y, &g_enemy.MovePattern[nCntPattern].point[nCnt].z, &aText[0], &aText[0]);
		}
	}

	//�t�@�C���N���[�Y
	fclose(pFile);
}

//==========================================
//  �G�l�~�[���C�t�̎擾
//==========================================
bool GetEnemyUse()
{
	return g_enemy.bUse;
}