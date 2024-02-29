//===========================
//
//�_�C���N�gX.player�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "player.h"
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
#include "ui.h"
#include "sound.h"
#include <stdio.h>

//�v���g�^�C�v�錾
void SetRot(void);
void MovePlayer(void);
void ConPlayer(void);
void ConPlayerPad(void);
void ModelMotion(void);

//�O���[�o���ϐ��錾
Player g_player;
int g_nIdxShadow = -1;
char g_aModelName[32][128];
int g_nCntFrame;
int g_nCntKey;
int g_nCheck;

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, -1800.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.motionState = PLAYERMOTION_RAND;
	g_player.motionStateOld = PLAYERMOTION_RAND;
	g_player.nLife = 50;
	g_player.nTimerState = 0;
	g_player.fAngle = 0.0f;
	g_nCntFrame = 0;
	g_nCntKey = 0;
	g_nCheck = 0;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aPlayerModel[nCntModel].pBuffMatPlayer,
			NULL,
			&g_player.aPlayerModel[nCntModel].dwNumMatPlayer,
			&g_player.aPlayerModel[nCntModel].pMeshPlayer);

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_player.aPlayerModel[nCntModel].apTexturePlayer[nCntMat]);
			}
		}

		g_player.aPlayerModel[nCntModel].pos = g_player.aPlayerModel[nCntModel].posMotion;
		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerModel[nCntModel].rotMotion;
		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
	}

	g_nIdxShadow = SetShadow(30.0f);
}

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayerTitle(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	g_player.pos = D3DXVECTOR3(0.0f, 550.0f, 1825.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.state = PLAYERSTATE_NORMAL;
	g_player.motionState = PLAYERMOTION_TITLE;
	g_player.motionStateOld = PLAYERMOTION_TITLE;
	g_player.nLife = 150;
	g_player.nTimerState = 0;
	g_player.fAngle = 0.0f;
	g_nCntFrame = 0;
	g_nCntKey = 0;
	g_nCheck = 0;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aPlayerModel[nCntModel].pBuffMatPlayer,
			NULL,
			&g_player.aPlayerModel[nCntModel].dwNumMatPlayer,
			&g_player.aPlayerModel[nCntModel].pMeshPlayer);

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_player.aPlayerModel[nCntModel].apTexturePlayer[nCntMat]);
			}
		}

		g_player.aPlayerModel[nCntModel].pos = g_player.aPlayerModel[nCntModel].posMotion;
		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerModel[nCntModel].rotMotion;
		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
	}

	g_nIdxShadow = SetShadow(30.0f);
}

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayerResult(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, -1800.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.state = PLAYERSTATE_NORMAL;

	if (GetResult() == RESULT_CLEAR)
	{
		g_player.motionState = PLAYERMOTION_CLEAR;
		g_player.motionStateOld = PLAYERMOTION_CLEAR;
	}
	else if (GetResult() == RESULT_OVER)
	{
		g_player.motionState = PLAYERMOTION_DEATH;
		g_player.motionStateOld = PLAYERMOTION_DEATH;
	}
	g_player.nLife = 150;
	g_player.nTimerState = 0;
	g_player.fAngle = 0.0f;
	g_nCntFrame = 0;
	g_nCntKey = 0;
	g_nCheck = 0;

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelName[nCntModel][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_player.aPlayerModel[nCntModel].pBuffMatPlayer,
			NULL,
			&g_player.aPlayerModel[nCntModel].dwNumMatPlayer,
			&g_player.aPlayerModel[nCntModel].pMeshPlayer);

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_player.aPlayerModel[nCntModel].apTexturePlayer[nCntMat]);
			}
		}

		g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
		g_player.aPlayerModel[nCntModel].rotMotion = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
	}

	g_nIdxShadow = SetShadow(30.0f);
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitPlayer(void)
{
	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		//�e�N�X�`���̔j��
		if (g_player.aPlayerModel[nCntModel].pMeshPlayer != NULL)
		{
			g_player.aPlayerModel[nCntModel].pMeshPlayer->Release();
			g_player.aPlayerModel[nCntModel].pMeshPlayer = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_player.aPlayerModel[nCntModel].pBuffMatPlayer != NULL)
		{
			g_player.aPlayerModel[nCntModel].pBuffMatPlayer->Release();
			g_player.aPlayerModel[nCntModel].pBuffMatPlayer = NULL;
		}
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdatePlayer(void)
{
	g_player.motionStateOld = g_player.motionState;
	g_nCheck = 0;

	if (g_player.state != PLAYERSTATE_DEATH)
	{
		ConPlayer();
		ConPlayerPad();
	}

	MovePlayer();
	
	SetRot();

	ModelMotion();

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIdxShadow, g_player.pos);
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdatePlayerResult(void)
{
	g_player.motionStateOld = g_player.motionState;
	g_nCheck = 0;

	ModelMotion();

	//�e�̈ʒu��ݒ�
	SetPositionShadow(g_nIdxShadow, g_player.pos);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_DAMAGE:
		g_player.nTimerState--;

		if (g_player.nTimerState <= 0)
		{
			if (g_player.nLife > 0)
			{
				g_player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;
	case PLAYERSTATE_DEATH:
		break;
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//�v�Z�p�}�g���b�N�X

																//�_���[�W��Ԕ���
		switch (g_player.state)
		{
		case PLAYERSTATE_NORMAL:
			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}

			break;
		case PLAYERSTATE_DAMAGE:
			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}
			break;
		case PLAYERSTATE_DEATH:
			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}
			break;
		}

		//�p�[�c�̃��[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_player.aPlayerModel[nCntModel].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_player.aPlayerModel[nCntModel].rot.y, g_player.aPlayerModel[nCntModel].rot.x, g_player.aPlayerModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_player.aPlayerModel[nCntModel].mtxWorld, &g_player.aPlayerModel[nCntModel].mtxWorld, &mtxRotModel);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel,
			g_player.aPlayerModel[nCntModel].pos.x, g_player.aPlayerModel[nCntModel].pos.y, g_player.aPlayerModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_player.aPlayerModel[nCntModel].mtxWorld, &g_player.aPlayerModel[nCntModel].mtxWorld, &mtxTransModel);

		//�p�[�c�̐e�}�g���b�N�X��ݒ�
		if (g_player.aPlayerModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_player.aPlayerModel[g_player.aPlayerModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_player.mtxWorld;
		}

		//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&g_player.aPlayerModel[nCntModel].mtxWorld,
			&g_player.aPlayerModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_player.aPlayerModel[nCntModel].mtxWorld);

		//�}�e���A���̃f�[�^�̃|�C���g���擾
		pMat = (D3DXMATERIAL*)g_player.aPlayerModel[nCntModel].pBuffMatPlayer->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aPlayerModel[nCntModel].dwNumMatPlayer; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_player.aPlayerModel[nCntModel].apTexturePlayer[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_player.aPlayerModel[nCntModel].pMeshPlayer->DrawSubset(nCntMat);
		}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void MovePlayer(void)
{
	Camera pCamera = GetCamera();

	if (g_player.motionState == PLAYERMOTION_ACTION)
	{
		g_player.move.x += sinf(pCamera.rot.y + g_player.fAngle) * 3.0f;
		g_player.move.z += cosf(pCamera.rot.y + g_player.fAngle) * 3.0f;

		SetParticle(g_player.pos + g_player.aPlayerModel[0].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 10.0f, 10, 1, 20, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50);
	}

	if (g_player.move == D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	{
		if (g_player.motionState != PLAYERMOTION_ACTION)
		{
			g_player.point.y = pCamera.rot.y + D3DX_PI;
		}

		if (g_player.aPlayerMotion[g_player.motionState].nLoop == 1)
		{
			g_player.motionState = PLAYERMOTION_NORMAL;
		}
	}
	else
	{
		if (g_player.motionState != PLAYERMOTION_ACTION)
		{
			g_player.point.y = pCamera.rot.y + D3DX_PI;

			g_player.motionState = PLAYERMOTION_MOVE;

			if (g_nCheck == 0)
			{
				g_player.motionState = PLAYERMOTION_NORMAL;
			}
		}
	}

	if (g_player.state == PLAYERSTATE_DEATH)
	{
		g_player.motionState = PLAYERMOTION_DEATH;
	}

	g_player.move.y -= 0.8f;

	g_player.pos += g_player.move;

	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.15f;

	if (g_player.pos.y < 0.0f)
	{
		g_player.move.y = 0.0f;
		g_player.pos.y = 0.0f;
	}

	if (CollisionMeshWall(&g_player.pos, &g_player.posOld, &g_player.move) == true)
	{
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.pos = g_player.posOld;
	}

	if (CollisionModel(&g_player.pos, &g_player.posOld, &g_player.move, 0) == true)
	{
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.pos = g_player.posOld;
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ConPlayer(void)
{
	Camera pCamera = GetCamera();
	g_player.posOld = g_player.pos;

	if (g_player.motionState != PLAYERMOTION_ACTION)
	{
		g_player.fAngle = 0.0f;

		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_player.move.x += sinf(pCamera.rot.y - (D3DX_PI * 0.75f));
				g_player.move.z += cosf(pCamera.rot.y - (D3DX_PI * 0.75f));
				g_player.fAngle = D3DX_PI * -0.75f;
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_player.move.x += sinf(pCamera.rot.y - (D3DX_PI * 0.25f));
				g_player.move.z += cosf(pCamera.rot.y - (D3DX_PI * 0.25f));
				g_player.fAngle = D3DX_PI * -0.25f;
			}
			else
			{
				g_player.move.x += sinf(pCamera.rot.y - (D3DX_PI * 0.5f));
				g_player.move.z += cosf(pCamera.rot.y - (D3DX_PI * 0.5f));
				g_player.fAngle = D3DX_PI * -0.5f;
			}
			g_nCheck = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_player.move.x += sinf(pCamera.rot.y + (D3DX_PI * 0.75f));
				g_player.move.z += cosf(pCamera.rot.y + (D3DX_PI * 0.75f));
				g_player.fAngle = D3DX_PI * 0.75f;
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_player.move.x += sinf(pCamera.rot.y + (D3DX_PI * 0.25f));
				g_player.move.z += cosf(pCamera.rot.y + (D3DX_PI * 0.25f));
				g_player.fAngle = D3DX_PI * 0.25f;
			}
			else
			{
				g_player.move.x += sinf(pCamera.rot.y + (D3DX_PI * 0.5f));
				g_player.move.z += cosf(pCamera.rot.y + (D3DX_PI * 0.5f));
				g_player.fAngle = D3DX_PI * 0.5f;
			}
			g_nCheck = 1;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ��
			g_player.move.x += sinf(pCamera.rot.y + D3DX_PI);
			g_player.move.z += cosf(pCamera.rot.y + D3DX_PI);
			g_player.fAngle = D3DX_PI * 1.0f;
			g_nCheck = 1;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ��
			g_player.move.x += sinf(pCamera.rot.y);
			g_player.move.z += cosf(pCamera.rot.y);
			g_player.fAngle = D3DX_PI * 0.0f;
			g_nCheck = 1;
		}
	}

	if (GetKeyboardTrigger(DIK_I) == true)
	{//W�L�[�������ꂽ��
		g_player.motionState = PLAYERMOTION_ACTION;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_DASH);

		g_player.point.y = pCamera.rot.y + g_player.fAngle + D3DX_PI;
	}

	if (GetKeyboardRepeat(DIK_SPACE) == true && g_player.motionState != PLAYERMOTION_ACTION)
	{//W�L�[�������ꂽ��
		SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 40.0f, g_player.pos.z), g_player.move, pCamera.posR, pCamera.rot, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), 30, 1);

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SHOT);
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ConPlayerPad(void)
{
	Camera pCamera = GetCamera();
	int nLStickLR = GetPadLStickLRPress();
	int nLStickUD = GetPadLStickUDPress();
	float fLength;
	g_player.posOld = g_player.pos;

	//�Ίp���̒������Z�o����
	fLength = sqrtf(((float)nLStickLR * (float)nLStickLR) + ((float)nLStickUD * (float)nLStickUD)) * 0.5f;
	
	if (g_nCheck == 0)
	{
		g_nCheck = (int)fLength;
	}

	nLStickUD = nLStickUD * -1;
	if (g_player.motionState != PLAYERMOTION_ACTION)
	{
		//�Ίp���̊p�x���Z�o����
		g_player.fAngle = atan2f((float)nLStickLR, (float)nLStickUD);
	}

	if (GetPadButtonTrigger(4) == true && g_player.motionState != PLAYERMOTION_ACTION)
	{//W�L�[�������ꂽ��
		g_player.motionState = PLAYERMOTION_ACTION;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_DASH);

		g_player.point.y = pCamera.rot.y + g_player.fAngle + D3DX_PI;
	}

	if (g_player.motionState != PLAYERMOTION_ACTION)
	{
		g_player.move.x += sinf(pCamera.rot.y + g_player.fAngle) * (fLength * 0.0015f);
		g_player.move.z += cosf(pCamera.rot.y + g_player.fAngle) * (fLength * 0.0015f);
	}

	if (GetPadButtonRepeat(5) == true && g_player.motionState != PLAYERMOTION_ACTION)
	{//W�L�[�������ꂽ��
		SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 40.0f, g_player.pos.z), g_player.move, pCamera.posR, pCamera.rot, D3DXCOLOR(1.0f, 1.0f, 0.5f, 1.0f), 30, 1);

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_SHOT);
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
bool CollisionPlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType)
{
	bool bCollision;
	float fLength;

	bCollision = false;

	fLength = ((g_player.pos.x - pos->x) * (g_player.pos.x - pos->x)) + ((g_player.pos.z - pos->z) * (g_player.pos.z - pos->z));

	if (fLength <= 800.0f && pos->y >= g_player.pos.y && pos->y <= g_player.pos.y + 48.0f)
	{
		if (nType == 2 && g_player.state != PLAYERSTATE_DEATH && g_player.state != PLAYERMOTION_ACTION)
		{
			bCollision = true;

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_HIT);

			if (g_player.nLife <= 0)
			{
				g_player.state = PLAYERSTATE_DEATH;
				SetResult(RESULT_OVER);
				SetGameState(GAMESTATE_END, 0);
			}
			else
			{
				g_player.state = PLAYERSTATE_DAMAGE;
				g_player.nTimerState = 10;
				g_player.nLife--;
				AddLife(6.0f);
			}
		}
		else if (nType == 3)
		{
			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_ITEM);

			bCollision = true;
			g_player.nLife += 10;
			AddLife(-60.0f);
		}
	}

	return bCollision;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ModelMotion(void)
{
	if (g_nCntKey < g_player.aPlayerMotion[g_player.motionState].nNumKey)
	{
		if (g_nCntFrame < g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame)
		{
			if (g_nCntKey == g_player.aPlayerMotion[g_player.motionState].nNumKey - 1)
			{
				for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
				{
					g_player.aPlayerModel[nCntModel].pos.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].pos[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].pos[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].pos[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;

					g_player.aPlayerModel[nCntModel].rot.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].rot[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].rot[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[0].rot[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
				
					if (g_player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
				{
					g_player.aPlayerModel[nCntModel].pos.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].pos[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].pos[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].pos.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].pos[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].pos[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;

					g_player.aPlayerModel[nCntModel].rot.x += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].rot[nCntModel].x - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].x) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.y += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].rot[nCntModel].y - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].y) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
					g_player.aPlayerModel[nCntModel].rot.z += (g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey + 1].rot[nCntModel].z - g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel].z) / (float)g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].nFrame;
				
					if (g_player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}

			g_nCntFrame++;
		}
		else
		{
			g_nCntKey++;
			g_nCntFrame = 0;

			for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
			{
				g_player.aPlayerModel[nCntModel].rotMotion = g_player.aPlayerModel[nCntModel].rot;
			}
		}
	}
	else
	{
		if (g_player.aPlayerMotion[g_player.motionState].nLoop == 0)
		{
			g_player.motionState = PLAYERMOTION_NORMAL;
		}
		else
		{
			g_nCntKey = 0;
			g_nCntFrame = 0;
		}
	}

	if (g_player.motionState != g_player.motionStateOld)
	{
		g_nCntKey = 0;
		g_nCntFrame = 0;
		for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
		{
			g_player.aPlayerModel[nCntModel].pos = g_player.aPlayerModel[nCntModel].posMotion;
			g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerModel[nCntModel].rotMotion;
			g_player.aPlayerModel[nCntModel].rot = g_player.aPlayerMotion[g_player.motionState].aKeyMotion[g_nCntKey].rot[nCntModel];
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void SetRot(void)
{
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_player.rot.y;
	fRotDest = g_player.point.y;
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

	g_player.rot.y = fRotMove;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ReadFilePlayer(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_dessan_3Dgame.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < 11; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_player.nNumModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aModelName[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fgets(&aText[0], 128, pFile);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_player.fMove, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_player.fJump, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_player.nNumModel, &aText[0], &aText[0]);

	for (int nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_player.aPlayerModel[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_player.aPlayerModel[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_player.aPlayerModel[nCntModel].posMotion.x, &g_player.aPlayerModel[nCntModel].posMotion.y, &g_player.aPlayerModel[nCntModel].posMotion.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_player.aPlayerModel[nCntModel].rotMotion.x, &g_player.aPlayerModel[nCntModel].rotMotion.y, &g_player.aPlayerModel[nCntModel].rotMotion.z);
		fgets(&aText[0], 128, pFile);
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < 7; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCntMotion = 0; nCntMotion < PLAYERMOTION_MAX; nCntMotion++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_player.aPlayerMotion[nCntMotion].nLoop, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_player.aPlayerMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);

		for (int nCntKey = 0; nCntKey < g_player.aPlayerMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_player.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
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
