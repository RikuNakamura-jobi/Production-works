//========================================
//
//2D�A�N�V�����Q�[��(�J��������)[item.cpp]
//Author:���v�ԗD��
//
//========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"
#include "meshfield.h"
#include "menu.h"
#include "tutorial.h"
#include "enemy.h"
#include "game.h"
#include "result.h"
#include "sound.h"
#include "book.h"
#include "fade.h"

//==========================================
//  �}�N����`
//==========================================
#define GAME_LENGTH	(270.0f)			//�Q�[����ʂł̎��_�����_�Ԃ̋���
#define NOT_GAME_LENGTH	(10.0f)		//�Q�[����ʈȊO�ł̎��_�����_�Ԃ̋���
#define MOVE	(0.05f)					//��]���鑬�x
#define MOVE_SPEED	(2.0f)				//�ړ�����X�s�[�h
#define UPROT_MAX	(D3DX_PI * 0.9f)	//���̕����̊p�x�̏��
#define DOWNROT_MIN	(D3DX_PI * 0.1f)	//��̕����̊p�x�̏��
#define POSR_HEGHT	(70.0f)				//�����_�̍���
#define CAMERA_RADIUS	(20.0f)	//�J�����̔���̔��a
#define CAMERA_SPEED	(0.10f)	//���j���[��ʂ̃J�����̑��x
#define FLOOR_HEIGHT	(2.2f)	//���̔���̍���
#define QUAKE_LENGE	(0.04f)	//�U���̔{��
#define NORMAL_FACT	(0.1f)	//�ʏ펞�̃J�����̈ړ�����
#define END_FACT	(0.05f)	//�I�����̃J�����̈ړ�����

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
Camera g_camera;					//�J�����̏��
float g_fLengthCamera;				//�����_���王�_�܂ł̋���

//========================================
//�J�����̏���������
//========================================
void InitCamera(void)
{
	//�ϐ�������
	g_camera.posRHeight = POSR_HEGHT;

	switch (GetMode())
	{
	case MODE_GAME:
	case MODE_TUTORIAL:
		g_fLengthCamera = GAME_LENGTH;

		g_camera.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���_�̏�����
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_�̏�����
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g���̏�����
		g_camera.rot = D3DXVECTOR3(1.5f, D3DX_PI * 0.5f, atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z));			//�����̏�����
		g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̎��_�̏�����
		g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړI�̒����_�̏�����
		g_camera.bBehind = false;

		g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.x) * cosf(g_camera.rot.y))* g_fLengthCamera;
		g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.x) * g_fLengthCamera;
		g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.x) * sinf(g_camera.rot.y))* g_fLengthCamera;
		break;

	case MODE_TITLE:
	case MODE_MENU:

		g_fLengthCamera = NOT_GAME_LENGTH;

		g_camera.posV = D3DXVECTOR3(0.0f, 680.0f, -1650.0f);		//���_�̏�����
		g_camera.posR = D3DXVECTOR3(0.0f, 670.0f, -1500.0f);		//�����_�̏�����
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g���̏�����
		g_camera.bBehind = false;
		break;

	case MODE_RANKING:

		g_fLengthCamera = NOT_GAME_LENGTH;

		g_camera.posV = D3DXVECTOR3(0.0f, 50.0f, -30.0f);		//���_�̏�����
		g_camera.posR = D3DXVECTOR3(0.0f, 50.0f, 200.0f);		//�����_�̏�����
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g���̏�����
		g_camera.bBehind = false;
		break;
	}
}

//========================================
//�J�����̏I������
//========================================
void UninitCamera(void)
{

}

//========================================
//�J�����̍X�V����
//========================================
void UpdateCamera(void)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 mouseMove = GetMouseMove();
	TUTORIALSTATE *tutorial = GetTutorialState();
	bool bTutorial = false;
	float fMeshU = 0.0f;	//��l�̕ǔ���p
	float fMeshV = 0.0f;	//��l�̕ǔ���p

	switch (GetMode())
	{
	case MODE_GAME:
	case MODE_TUTORIAL:

		if (GetPhotoMode())
		{//�t�H�g���[�h�J��������
			ControlCamera();
		}
		else
		{
			//�ړI�̒����_��ݒ�(�ړ�)
			Move();
		}
		
		//��ʐU��
		if (g_camera.nQuakeTime > 0)
		{
			//���Ԃ����炷
			g_camera.nQuakeTime--;

			//���_�����_�𓮂����ăJ������h�炷
			g_camera.posV.x += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * sinf(g_camera.rot.y);
			g_camera.posV.y += (float)(rand() % 101 - 50) * g_camera.fQuakeSize;
			g_camera.posV.z += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * cosf(g_camera.rot.y);

			g_camera.posR.x += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * sinf(g_camera.rot.y);
			g_camera.posR.y += (float)(rand() % 101 - 50) * g_camera.fQuakeSize;
			g_camera.posR.z += (float)(rand() % 201 - 100) * g_camera.fQuakeSize * cosf(g_camera.rot.y);

			//���X�ɗh��̃T�C�Y������������
			g_camera.fQuakeSize += (0.0f - g_camera.fQuakeSize) * 0.03f;
		}

		if (GetGameState() == GAMESTATE_NORMAL && (*GetBookState() == BOOKSTATE_NONE || *GetBookState() == BOOKSTATE_END) && GetFade() == FADE_NONE)
		{
			//���_����====================================
			if (GetKeyboardPress(DIK_LEFT) == true)
			{//������
				g_camera.rot.y += MOVE;
				bTutorial = true;
			}
			else if (GetKeyboardPress(DIK_RIGHT) == true)
			{//�E����
				g_camera.rot.y -= MOVE;
				bTutorial = true;
			}
			else if (GetKeyboardPress(DIK_DOWN) == true)
			{//�J�����グ��
				g_camera.rot.x -= MOVE;
				//g_fPosRHeight -= MOVE * 20;
				bTutorial = true;
			}
			else if (GetKeyboardPress(DIK_UP) == true)
			{//�J����������
				g_camera.rot.x += MOVE;
				//g_fPosRHeight += MOVE * 20;
				bTutorial = true;
			}
			//���_����====================================

			//�}�E�X�̒����_����
			g_camera.rot.x += mouseMove.y;
			g_camera.rot.y += mouseMove.x;
		}

		//��]�̐���
		if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2;
		}
		else if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2;
		}

		if (g_camera.rot.x > D3DX_PI)
		{
			g_camera.rot.x += -D3DX_PI * 2;
		}
		else if (g_camera.rot.x < -D3DX_PI)
		{
			g_camera.rot.x += D3DX_PI * 2;
		}

		//�J�����̏㉺��]�̏��
		if (g_camera.rot.x > UPROT_MAX)
		{//��
			g_camera.rot.x = UPROT_MAX;
		}
		if (g_camera.rot.x < DOWNROT_MIN)
		{//��
			g_camera.rot.x = DOWNROT_MIN;
		}

		if (GetMode() == MODE_GAME)
		{//��l�̕ǃT�C�Y�ݒ�
			fMeshU = MESH_U;
			fMeshV = MESH_V;
		}
		else
		{
			fMeshU = MESH_TUTORIAL_U;
			fMeshV = MESH_TUTORIAL_V;
		}

		if (g_camera.posV.x >(FIELD_RANGE * fMeshU) * 0.5f - CAMERA_RADIUS)
		{//�G���A�E�̐���
			g_camera.posV.x = (FIELD_RANGE * fMeshU) * 0.5f - CAMERA_RADIUS;
		}
		else if (g_camera.posV.x < -(FIELD_RANGE * fMeshU) * 0.5f + CAMERA_RADIUS)
		{//�G���A���̐���
			g_camera.posV.x = -(FIELD_RANGE * fMeshU) * 0.5f + CAMERA_RADIUS;
		}

		if (g_camera.posV.z >(FIELD_RANGE * fMeshV) * 0.5f - CAMERA_RADIUS)
		{//�G���A���̐���
			g_camera.posV.z = (FIELD_RANGE * fMeshV) * 0.5f - CAMERA_RADIUS;
		}
		else if (g_camera.posV.z < -(FIELD_RANGE * fMeshV) * 0.5f + CAMERA_RADIUS)
		{//�G���A��O�̐���
			g_camera.posV.z = -(FIELD_RANGE * fMeshV) * 0.5f + CAMERA_RADIUS;
		}

		if (g_camera.posR.x >(FIELD_RANGE * fMeshU) * 0.5f)
		{//�G���A�E�̐���
			g_camera.posR.x = (FIELD_RANGE * fMeshU) * 0.5f;
		}
		else if (g_camera.posR.x < -(FIELD_RANGE * fMeshU) * 0.5f)
		{//�G���A���̐���
			g_camera.posR.x = -(FIELD_RANGE * fMeshU) * 0.5f;
		}

		if (g_camera.posR.z >(FIELD_RANGE * fMeshV) * 0.5f)
		{//�G���A���̐���
			g_camera.posR.z = (FIELD_RANGE * fMeshV) * 0.5f;
		}
		else if (g_camera.posR.z < -(FIELD_RANGE * fMeshV) * 0.5f)
		{//�G���A��O�̐���
			g_camera.posR.z = -(FIELD_RANGE * fMeshV) * 0.5f;
		}

		if (g_camera.posV.y < FLOOR_HEIGHT)
		{//������
			g_camera.posV.y = FLOOR_HEIGHT;
		}

#ifdef _DEBUG
		//�f�o�b�O����
		if (GetKeyboardPress(DIK_U))
		{
			g_fLengthCamera -= 5.0f;
		}
		if (GetKeyboardPress(DIK_J))
		{
			g_fLengthCamera += 5.0f;
		}
#endif

		break;
	case MODE_TITLE:
		RotMove();
		break;
	case MODE_RANKING:
		//�Œ�
		break;

	}


}

//========================================
// ���쏈��
//========================================
void ControlCamera(void)
{
	//�ϐ��錾
	D3DXVECTOR3 RDiff, VDiff;
	float fSpeed;

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//�������
		fSpeed = MOVE_SPEED * 5;
	}
	else
	{
		fSpeed = MOVE_SPEED;
	}

	//���_�ړ�===============================================
	if (GetKeyboardPress(DIK_A) == true)
	{//���ړ�
		g_camera.posRDest.x -= sinf(g_camera.rot.y + D3DX_PI * -0.5f) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y + D3DX_PI * -0.5f) * fSpeed;
	}
	if (GetKeyboardPress(DIK_D) == true)
	{//�E�ړ�
		g_camera.posRDest.x -= sinf(g_camera.rot.y - D3DX_PI * -0.5f) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y - D3DX_PI * -0.5f) * fSpeed;
	}
	if (GetKeyboardPress(DIK_W) == true)
	{//�O�ړ�
		g_camera.posRDest.x -= sinf(g_camera.rot.y) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y) * fSpeed;
	}
	if (GetKeyboardPress(DIK_S) == true)
	{//��ړ�
		g_camera.posRDest.x -= sinf(g_camera.rot.y + D3DX_PI) * fSpeed;
		g_camera.posRDest.z -= cosf(g_camera.rot.y + D3DX_PI) * fSpeed;
	}
	if (GetKeyboardPress(DIK_T) == true)
	{//��ړ�
		g_camera.posRDest.y += fSpeed;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{//���ړ�
		g_camera.posRDest.y -= fSpeed;
	}

	//���_��]===============================================
	if (GetKeyboardPress(DIK_Q) == true)
	{//��������
		g_camera.rot.y -= MOVE;
		//�����_�𑊑Έʒu�ɐݒ�
	}
	if (GetKeyboardPress(DIK_E) == true)
	{//�E������
		g_camera.rot.y += MOVE;
		//�����_�𑊑Έʒu�ɐݒ�
	}

	//�c��]====================================================================
	if (GetKeyboardPress(DIK_N) == true)
	{//��
		g_camera.rot.x += MOVE;

		//�����_�𑊑Έʒu�ɐݒ�

		if (g_camera.rot.x < 0.1f)
		{
			g_camera.rot.x = 0.1f;
		}
	}
	if (GetKeyboardPress(DIK_Y) == true)
	{//��
		g_camera.rot.x -= MOVE;
		//�����_�𑊑Έʒu�ɐݒ�

		if (g_camera.rot.x > 3.0f)
		{
			g_camera.rot.x = 3.0f;
		}
	}

	g_camera.posVDest =
	{
		g_camera.posRDest.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_fLengthCamera,
		g_camera.posRDest.y + cosf(g_camera.rot.x) * g_fLengthCamera,
		g_camera.posRDest.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_fLengthCamera
	};

	//�����_�̍���
	RDiff = g_camera.posRDest - g_camera.posR;

	//���_�̍���
	VDiff = g_camera.posVDest - g_camera.posV;

	//����
	g_camera.posR += RDiff * NORMAL_FACT;
	g_camera.posV += VDiff * NORMAL_FACT;
}

//========================================
//�J�����̕`�揈��
//========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(54.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		4243.0f);

	//�v���W�F�N�g�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//========================================
//�J�����̎擾����
//========================================
Camera *GetCamera(void)
{
	return &g_camera;
}

//========================================
//�ړI�̒����_��ݒ�(�ړ�) 
//========================================
void Move(void)
{
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fSlow = 0.0f;	//����

	Player *pPlayer = GetPlayer();

	if (GetGameState() != GAMESTATE_END)
	{//�Q�[���ʏ펞�̋���
		g_camera.posRDest = D3DXVECTOR3
		(
			pPlayer->pos.x + sinf(pPlayer->rot.y) * -20.0f,
			pPlayer->pos.y + g_camera.posRHeight,
			pPlayer->pos.z + cosf(pPlayer->rot.y) * -20.0f
		);

		//�ړI�̎��_��ݒ�
		g_camera.posVDest =
		{
			g_camera.posRDest.x + sinf(g_camera.rot.x) * sinf(-g_camera.rot.y + D3DX_PI * 0.5f) * g_fLengthCamera,
			pPlayer->pos.y + cosf(g_camera.rot.x) * g_fLengthCamera,
			g_camera.posRDest.z + sinf(g_camera.rot.x) * cosf(-g_camera.rot.y + D3DX_PI * 0.5f) * g_fLengthCamera
		};

		fSlow = NORMAL_FACT;
	}
	else
	{//�Q�[���I�����̋���
		GameEndCamera(GetResult());

		fSlow = END_FACT;
	}

	//�����_�̕␳
	RDiff = g_camera.posRDest - g_camera.posR;

	//���_�̕␳
	VDiff = g_camera.posVDest - g_camera.posV;

	//����
	g_camera.posR += RDiff * fSlow;
	g_camera.posV += VDiff * fSlow;
}

//========================================
//�ړI�̒����_��ݒ�(�ړ�) 
//========================================
void RotMove()
{
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos;

	switch (GetMenuState())
	{
	case MENUSTATE_START:
		g_camera.posRDest = D3DXVECTOR3(0.0f, 670.0f, -1500.0f);
		g_camera.posVDest = D3DXVECTOR3(0.0f, 680.0f, -1650.0f);
		break;
	case MENUSTATE_TUTORIAL:
		g_camera.posRDest = D3DXVECTOR3(-130.0f, 620.0f, -1650.0f);
		g_camera.posVDest = D3DXVECTOR3(-80.0f, 650.0f, -1650.0f);
		break;
	case MENUSTATE_QUIT:
		g_camera.posRDest = D3DXVECTOR3(0.0f, 660.0f, -1800.0f);
		g_camera.posVDest = D3DXVECTOR3(0.0f, 680.0f, -1650.0f);
		break;
	case MENUSTATE_FADE:
		g_camera.posRDest = D3DXVECTOR3(0.0f, 670.0f, -1350.0f);
		break;
	}

	//�����_�̕␳
	RDiff = g_camera.posRDest - g_camera.posR;
	VDiff = g_camera.posVDest - g_camera.posV;

	//����
	if (GetMenuState() == MENUSTATE_FADE)
	{
		g_camera.posV += RDiff * 0.02f;
		g_camera.posR += RDiff * 0.02f;
	}
	else
	{
		g_camera.posV += VDiff * CAMERA_SPEED;
		g_camera.posR += RDiff * CAMERA_SPEED;
	}

}

//========================================
//�ړI�̒����_��ݒ�(�ړ�) 
//========================================
void GameEndCamera(int nResult)
{
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 pos;

		if (nResult == RESULTTYPE_WIN)
		{
			Enemy *pEnemy = GetEnemy();

			g_camera.posRDest = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
			g_camera.posVDest = D3DXVECTOR3(pEnemy->pos.x + sinf(pEnemy->rot.y + (D3DX_PI * 1.0f)) * 800.0f, 
											300.0f, 
											pEnemy->pos.z + cosf(pEnemy->rot.y + (D3DX_PI * 1.0f)) * 800.0f);
		}
		else
		{
			Player *pPlayer = GetPlayer();

			g_camera.posRDest = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 20.0f, pPlayer->pos.z);
			g_camera.posVDest = D3DXVECTOR3(pPlayer->pos.x + sinf(pPlayer->rot.y + (D3DX_PI * 1.0f)) * 100.0f,
											100.0f,
											pPlayer->pos.z + cosf(pPlayer->rot.y + (D3DX_PI * 1.0f)) * 100.0f);
		}
}

//========================================
//�ړI�̒����_��ݒ�(��]) 
//========================================
void Rot(void)
{
	D3DXVECTOR3 RotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	Player *pPlayer = GetPlayer();

	RotDest.y = -pPlayer->rot.y + D3DX_PI * 0.5f;

	RotDiff.y = RotDest.y - g_camera.rot.y;

	//��������Ƃ��ɖ߂�����
	if (RotDiff.y > D3DX_PI)
	{
		RotDiff.y += -D3DX_PI * 2;
	}
	else if (RotDiff.y < -D3DX_PI)
	{
		RotDiff.y += D3DX_PI * 2;
	}

	g_camera.rot.y += RotDiff.y * 0.05f;

	if (pPlayer->rot.y > D3DX_PI)
	{
		pPlayer->rot.y += -D3DX_PI * 2;
	}
	else if (pPlayer->rot.y < -D3DX_PI)
	{
		pPlayer->rot.y += D3DX_PI * 2;
	}
}

//========================================
//���_���v���C���[�̌��ɓ�����
//========================================
void RotPlayer(void)
{
	Player *pPlayer = GetPlayer();
	float fRotMove, fRotDest, fRotDiff;

	if (g_camera.bBehind == true)
	{
		fRotMove = g_camera.rot.y;
		fRotDest = -pPlayer->rot.y + D3DX_PI * 0.5f;
		fRotDiff = fRotDest - fRotMove;

		if (fRotDiff > 3.14f)
		{
			fRotDiff -= 6.28f;
		}
		else if (fRotDiff <= -3.14f)
		{
			fRotDiff += 6.28f;
		}

		fRotMove += fRotDiff * 0.1f;

		if (fRotMove > 3.14f)
		{
			fRotMove -= 6.28f;
		}
		else if (fRotMove <= -3.14f)
		{
			fRotMove += 6.28f;
		}

		g_camera.rot.y = fRotMove;
	}
}

//========================================
//�J�������h��鏈��
//========================================
void SetQuake(int time, float size)
{
	if (size > 0.0f)
	{
		//�h��鎞�ԑ��
		g_camera.nQuakeTime = time;

		//�h��̑傫�����
		g_camera.fQuakeSize = size;
	}
}

//========================================
//�J�����̃f�o�b�O�\��
//========================================
void DrawDebugCamera(LPD3DXFONT pFont)
{
	RECT rect4 = { 0,290,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "�����_ [ %.2f : %.2f : %.2f ]\n���_ [ %.2f : %.2f : %.2f ]\n�J�����̋����@[%.2f]", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z, g_camera.posV.x, g_camera.posV.y, g_camera.posV.z,g_fLengthCamera);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
