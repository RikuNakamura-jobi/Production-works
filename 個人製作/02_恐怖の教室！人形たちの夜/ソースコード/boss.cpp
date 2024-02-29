//===========================
//
//�_�C���N�gX.boss�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "boss.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "camera.h"
#include "meshwall.h"
#include "model.h"
#include "effect.h"
#include "result.h"
#include "game.h"
#include "particle.h"
#include <stdio.h>

//�v���g�^�C�v�錾
void SetRotBoss(void);
void MoveBoss(void);
void ModelMotionBoss(void);

//�O���[�o���ϐ��錾
Boss g_Boss;
int g_nIdxShadowBoss = -1;
char g_aModelNameBoss[32][128];
int g_nCntFrameBoss;
int g_nCntKeyBoss;
int g_nCheckBoss;

//===========================
//�v���C���[�̏���������
//===========================
void InitBoss(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;
	
	ReadFileBoss();

	g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Boss.state = BOSSSTATE_NORMAL;
	g_Boss.motionState = BOSSMOTION_NORMAL;
	g_Boss.motionStateOld = BOSSMOTION_NORMAL;
	g_Boss.nLife = 150;
	g_Boss.nTimerState = 0;
	g_Boss.fAngle = 0.0f;
	g_nCntFrameBoss = 0;
	g_nCntKeyBoss = 0;
	g_nCheckBoss = 0;

	for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelNameBoss[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_Boss.aBossModel[nCntModel].pBuffMatBoss,
			NULL,
			&g_Boss.aBossModel[nCntModel].dwNumMatBoss,
			&g_Boss.aBossModel[nCntModel].pMeshBoss);

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Boss.aBossModel[nCntModel].pBuffMatBoss->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Boss.aBossModel[nCntModel].dwNumMatBoss; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Boss.aBossModel[nCntModel].apTextureBoss[nCntMat]);
			}
		}

		g_Boss.aBossModel[nCntModel].rot = g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].rot[nCntModel];
		g_Boss.aBossModel[nCntModel].rotMotion = g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].rot[nCntModel];
	}

	g_nIdxShadowBoss = SetShadow(30.0f);
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitBoss(void)
{
	for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
	{
		//�e�N�X�`���̔j��
		if (g_Boss.aBossModel[nCntModel].pMeshBoss != NULL)
		{
			g_Boss.aBossModel[nCntModel].pMeshBoss->Release();
			g_Boss.aBossModel[nCntModel].pMeshBoss = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_Boss.aBossModel[nCntModel].pBuffMatBoss != NULL)
		{
			g_Boss.aBossModel[nCntModel].pBuffMatBoss->Release();
			g_Boss.aBossModel[nCntModel].pBuffMatBoss = NULL;
		}
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateBoss(void)
{
	g_Boss.motionStateOld = g_Boss.motionState;
	g_nCheckBoss = 0;

	MoveBoss();
	
	SetRotBoss();

	ModelMotionBoss();

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIdxShadowBoss, g_Boss.pos);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawBoss(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	switch (g_Boss.state)
	{
	case BOSSSTATE_NORMAL:
		break;
	case BOSSSTATE_DAMAGE:
		g_Boss.nTimerState--;

		if (g_Boss.nTimerState <= 0)
		{
			if (g_Boss.nLife > 0)
			{
				g_Boss.state = BOSSSTATE_NORMAL;
			}
		}
		break;
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Boss.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_Boss.rot.y, g_Boss.rot.x, g_Boss.rot.z);
	D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_Boss.pos.x, g_Boss.pos.y, g_Boss.pos.z);
	D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Boss.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//�v�Z�p�}�g���b�N�X

																//�_���[�W��Ԕ���
		switch (g_Boss.state)
		{
		case BOSSSTATE_NORMAL:
			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Boss.aBossModel[nCntModel].pBuffMatBoss->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Boss.aBossModel[nCntModel].dwNumMatBoss; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}

			break;
		case BOSSSTATE_DAMAGE:
			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Boss.aBossModel[nCntModel].pBuffMatBoss->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Boss.aBossModel[nCntModel].dwNumMatBoss; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}
			break;
		}

		//�p�[�c�̃��[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_Boss.aBossModel[nCntModel].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_Boss.aBossModel[nCntModel].rot.y, g_Boss.aBossModel[nCntModel].rot.x, g_Boss.aBossModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Boss.aBossModel[nCntModel].mtxWorld, &g_Boss.aBossModel[nCntModel].mtxWorld, &mtxRotModel);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel,
			g_Boss.aBossModel[nCntModel].pos.x, g_Boss.aBossModel[nCntModel].pos.y, g_Boss.aBossModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Boss.aBossModel[nCntModel].mtxWorld, &g_Boss.aBossModel[nCntModel].mtxWorld, &mtxTransModel);

		//�p�[�c�̐e�}�g���b�N�X��ݒ�
		if (g_Boss.aBossModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_Boss.aBossModel[g_Boss.aBossModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_Boss.mtxWorld;
		}

		//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&g_Boss.aBossModel[nCntModel].mtxWorld,
			&g_Boss.aBossModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_Boss.aBossModel[nCntModel].mtxWorld);

		//�}�e���A���̃f�[�^�̃|�C���g���擾
		pMat = (D3DXMATERIAL*)g_Boss.aBossModel[nCntModel].pBuffMatBoss->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Boss.aBossModel[nCntModel].dwNumMatBoss; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Boss.aBossModel[nCntModel].apTextureBoss[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_Boss.aBossModel[nCntModel].pMeshBoss->DrawSubset(nCntMat);
		}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
Boss *GetBoss(void)
{
	return &g_Boss;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void MoveBoss(void)
{
	g_Boss.move.y -= 0.8f;

	g_Boss.pos += g_Boss.move;

	g_Boss.move.x += (0.0f - g_Boss.move.x) * 0.15f;
	g_Boss.move.z += (0.0f - g_Boss.move.z) * 0.15f;

	if (g_Boss.pos.y < 0.0f)
	{
		g_Boss.move.y = 0.0f;
		g_Boss.pos.y = 0.0f;
	}

	if (CollisionMeshWall(&g_Boss.pos, &g_Boss.posOld, &g_Boss.move) == true)
	{
		g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Boss.pos = g_Boss.posOld;
	}

	if (CollisionModel(&g_Boss.pos, &g_Boss.posOld, &g_Boss.move, 0) == true)
	{
		g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Boss.pos = g_Boss.posOld;
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
bool CollisionBoss(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType)
{
	bool bCollision;
	float fLength;

	bCollision = false;

	fLength = ((g_Boss.pos.x - pos->x) * (g_Boss.pos.x - pos->x)) + ((g_Boss.pos.z - pos->z) * (g_Boss.pos.z - pos->z));

	if (fLength <= 400.0f && pos->y >= g_Boss.pos.y && pos->y <= g_Boss.pos.y + 48.0f)
	{
		if (nType == 2)
		{
			bCollision = true;

			if (g_Boss.nLife <= 0)
			{
				g_Boss.state = BOSSSTATE_DEATH;
				SetResult(RESULT_OVER);
				SetGameState(GAMESTATE_END, 0);
			}
			else
			{
				g_Boss.state = BOSSSTATE_DAMAGE;
				g_Boss.nTimerState = 10;
				g_Boss.nLife--;
			}
		}
	}

	return bCollision;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ModelMotionBoss(void)
{
	if (g_Boss.motionState != g_Boss.motionStateOld)
	{
		g_nCntKeyBoss = 0;
		g_nCntFrameBoss = 0;
		for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
		{
			g_Boss.aBossModel[nCntModel].rot = g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].rot[nCntModel];
			g_Boss.aBossModel[nCntModel].rotMotion = g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].rot[nCntModel];
		}
	}

	if (g_nCntKeyBoss < g_Boss.aBossMotion[g_Boss.motionState].nNumKey)
	{
		if (g_nCntFrameBoss < g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame)
		{
			if (g_nCntKeyBoss == g_Boss.aBossMotion[g_Boss.motionState].nNumKey - 1)
			{
				for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
				{
					g_Boss.aBossModel[nCntModel].rot.x += (g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[0].rot[nCntModel].x - g_Boss.aBossModel[nCntModel].rotMotion.x) / (float)g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame;
					g_Boss.aBossModel[nCntModel].rot.y += (g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[0].rot[nCntModel].y - g_Boss.aBossModel[nCntModel].rotMotion.y) / (float)g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame;
					g_Boss.aBossModel[nCntModel].rot.z += (g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[0].rot[nCntModel].z - g_Boss.aBossModel[nCntModel].rotMotion.z) / (float)g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame;
				
					if (g_Boss.aBossModel[nCntModel].rot.x > 3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_Boss.aBossModel[nCntModel].rot.x < -3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.x += 6.28f;
					}

					if (g_Boss.aBossModel[nCntModel].rot.y > 3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_Boss.aBossModel[nCntModel].rot.y < -3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.y += 6.28f;
					}

					if (g_Boss.aBossModel[nCntModel].rot.z > 3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_Boss.aBossModel[nCntModel].rot.z < -3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
				{
					g_Boss.aBossModel[nCntModel].rot.x += (g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss + 1].rot[nCntModel].x - g_Boss.aBossModel[nCntModel].rotMotion.x) / (float)g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame;
					g_Boss.aBossModel[nCntModel].rot.y += (g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss + 1].rot[nCntModel].y - g_Boss.aBossModel[nCntModel].rotMotion.y) / (float)g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame;
					g_Boss.aBossModel[nCntModel].rot.z += (g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss + 1].rot[nCntModel].z - g_Boss.aBossModel[nCntModel].rotMotion.z) / (float)g_Boss.aBossMotion[g_Boss.motionState].aKeyMotion[g_nCntKeyBoss].nFrame;
				
					if (g_Boss.aBossModel[nCntModel].rot.x > 3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_Boss.aBossModel[nCntModel].rot.x < -3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.x += 6.28f;
					}

					if (g_Boss.aBossModel[nCntModel].rot.y > 3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_Boss.aBossModel[nCntModel].rot.y < -3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.y += 6.28f;
					}

					if (g_Boss.aBossModel[nCntModel].rot.z > 3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_Boss.aBossModel[nCntModel].rot.z < -3.14f)
					{
						g_Boss.aBossModel[nCntModel].rot.z += 6.28f;
					}
				}
			}

			g_nCntFrameBoss++;
		}
		else
		{
			g_nCntKeyBoss++;
			g_nCntFrameBoss = 0;

			for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
			{
				g_Boss.aBossModel[nCntModel].rotMotion = g_Boss.aBossModel[nCntModel].rot;
			}
		}
	}
	else
	{
		if (g_Boss.aBossMotion[g_Boss.motionState].nLoop == 0)
		{
			g_Boss.motionState = BOSSMOTION_NORMAL;
		}
		else
		{
			g_nCntKeyBoss = 0;
			g_nCntFrameBoss = 0;
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void SetRotBoss(void)
{
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_Boss.rot.y;
	fRotDest = g_Boss.point.y;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	fRotMove += fRotDiff * 0.2f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_Boss.rot.y = fRotMove;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ReadFileBoss(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_dessan_enoch.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < 11; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_Boss.nNumModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_Boss.nNumModel; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aModelNameBoss[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fgets(&aText[0], 128, pFile);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_Boss.fMove, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_Boss.fJump, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_Boss.nNumModel, &aText[0], &aText[0]);

	for (int nCntModel = 0; nCntModel < g_Boss.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_Boss.aBossModel[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_Boss.aBossModel[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_Boss.aBossModel[nCntModel].pos.x, &g_Boss.aBossModel[nCntModel].pos.y, &g_Boss.aBossModel[nCntModel].pos.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_Boss.aBossModel[nCntModel].rot.x, &g_Boss.aBossModel[nCntModel].rot.y, &g_Boss.aBossModel[nCntModel].rot.z);
		fgets(&aText[0], 128, pFile);
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < 7; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCntMotion = 0; nCntMotion < BOSSMOTION_MAX; nCntMotion++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_Boss.aBossMotion[nCntMotion].nLoop, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_Boss.aBossMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);

		for (int nCntKey = 0; nCntKey < g_Boss.aBossMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_Boss.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_Boss.aBossMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
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
}
