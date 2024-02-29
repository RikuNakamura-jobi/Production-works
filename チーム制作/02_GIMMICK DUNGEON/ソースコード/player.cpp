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
#include "object.h"
#include "rock.h"
#include "shock.h"
#include <stdio.h>
#include "meshfield.h"
#include "gauge.h"
#include "score.h"
#include "shadow.h"
#include "enemy.h"
#include "tutorial.h"
#include "orbit.h"
#include "particle.h"
#include "wall.h"
#include "orbit.h"
#include "billboard.h"
#include "sound.h"
#include "game.h"
#include "point.h"
#include "point_log.h"
#include "time.h"
#include "model.h"
#include "fade.h"
#include "book.h"

//================
//�}�N����`
//================
#define LENGTH				(0.3f)							//����
#define LIFE				(20)							//�̗�
#define PLAYER_JUMP_POW		(10.0f)							//�����̃W�����v��
#define PLAYER_GRAVITY		(0.49f)							//�d��
#define SHOCK_SCORE			(-100)							//�Ռ��g�ɓ����������̃X�R�A
#define GAUGE_COLOR_FRAME	(30)							//HP�Q�[�W�̐F�ς����x
#define PLAYER_GROUND_LENG	(1.5f)							//�����ŉe�̃T�C�Y�␳

#define CAMERA_FACT			(0.03f)							//�J�����̕␳�W��
#define ROTATE_FACT			(0.1f)							//�v���C���[�̉�]�W��

#define SHADOW_RADIUS		(30.0f)							//�e�̔��a

//================
//�v���g�^�C�v�錾
//================
void SetRot(void);
void SetMotion(void);
void SetMotionRanking(void);
void MovePlayer(void);
void ConPlayer(void);
void ConPlayerPad(void);
void ModelMotionPlayer(void);
void LimitMovePlayer(void);

//================
//�O���[�o���錾
//================
Player g_Player;
char g_aModelName[32][128];
int g_nCntFramePlayer;
int g_nCntKeyPlayer;
int g_nCheckPlayer;

float g_fRBJustGaugePlayer;	//HP�Q�[�W��r,b�l����
int g_fRBJustFramePlayer;	//HP�Q�[�W��r,b�l�����t���[����

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayer(void)
{
	//�e��ϐ��̏�����
	ZeroMemory(&g_Player,sizeof(Player));

	//�p�[�c���擾
	g_Player.nNumModel = GetNumPlayerPart();

	//�e��ϐ��̏�����
	SetPlayer(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	g_nCheckPlayer = 0;
	g_fRBJustGaugePlayer = 0.0f;
	g_fRBJustFramePlayer = 0;

	//�v���C���[���f���ǂݍ���
	ReadFilePlayer();

	//���f�����擾
	int nNumVtx = 0;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	//���擾
	Model *pPlayerPart = GetModelPlayer();

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		if (pPlayerPart[nCntModel].vtxMax.x >= g_Player.vtxMax.x)
		{
			g_Player.vtxMax.x = pPlayerPart[nCntModel].vtxMax.x;
		}
		if (pPlayerPart[nCntModel].vtxMin.x < g_Player.vtxMin.x)
		{
			g_Player.vtxMin.x = pPlayerPart[nCntModel].vtxMin.x;
		}

		if (pPlayerPart[nCntModel].vtxMax.y >= g_Player.vtxMax.y)
		{
			g_Player.vtxMax.y = pPlayerPart[nCntModel].vtxMax.y;
		}
		if (pPlayerPart[nCntModel].vtxMin.y < g_Player.vtxMin.y)
		{
			g_Player.vtxMin.y = pPlayerPart[nCntModel].vtxMin.y;
		}

		if (pPlayerPart[nCntModel].vtxMax.z >= g_Player.vtxMax.z)
		{
			g_Player.vtxMax.z = pPlayerPart[nCntModel].vtxMax.z;
		}
		if (pPlayerPart[nCntModel].vtxMin.z < g_Player.vtxMin.z)
		{
			g_Player.vtxMin.z = pPlayerPart[nCntModel].vtxMin.z;
		}

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		g_Player.pMatDef = *pMat;

		//���[�V�������Z�b�g
		g_Player.aPlayerModel[nCntModel].pos = g_Player.aPlayerModel[nCntModel].posMotion;
		g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerModel[nCntModel].rotMotion;
		g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel];
	}

	//�Q�[�W�̐ݒ�
	switch (GetMode())
	{
	case MODE_RANKING:
		g_Player.nIdxLifeGauge = -1;
		break;
	case MODE_GAME:
		g_Player.nIdxLifeGauge = SetGauge(D3DXVECTOR3(20.0f, 50.0f, 0.0f), (float)g_Player.nLife, 50.0f, 400.0f, GAUGETYPE_HORIZON,PLAYER_LIFE_CR);
		break;
	default:
		break;
	}

	//�e�̐ݒ�
	g_Player.nIdxShadow = SetShadow(40.0f, SHADOWTYPE_CIRCLE, 0.0f, 0.0f,g_Player.rot);
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitPlayer(void)
{
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdatePlayer(void)
{
	//�ϐ��錾
	Camera *pCamera = GetCamera();
	float fHeight = 1.0f - g_Player.pos.y / (PLAYER_JUMP_POW * 10 * PLAYER_GROUND_LENG);
	g_Player.posOld = g_Player.pos;
	g_Player.motionStateOld = g_Player.motionState;
	g_nCheckPlayer = 0;

	if (GetGameState() == GAMESTATE_NORMAL && GetFade() == FADE_NONE && (*GetBookState() == BOOKSTATE_NONE || *GetBookState() == BOOKSTATE_END) && GetPhotoMode() == false)
	{
		if (g_Player.state != PLAYERSTATE_DEATH)
		{
			//���쏈��
			ConPlayer();
			ConPlayerPad();
		}
	}

	//���[�V�����ݒ�
	SetMotion();

	//�ʒu�X�V����
	MovePlayer();

	//�T�E���h�֌W
	SoundPlayer();

	//�G���A��������
	CollisionWall(&g_Player.pos, &g_Player.posOld, &g_Player.move, 20.0f);
	//LimitMovePlayer();

	//�e�̈ʒu�ݒ菈��
	SetPositionShadow(g_Player.nIdxShadow, D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z),g_Player.rot, fHeight);

	//�r���{�[�h�̈ʒu�ݒ菈��
	SetPositionBillboard(g_Player.nIdxBillboard, D3DXVECTOR3(g_Player.pos.x + 20.0f, g_Player.pos.y + 58.0f, g_Player.pos.z));

	//�u���b�N�Ɠ����蔻�菈��
	if (CollisionObject2(&g_Player.pos, &g_Player.posOld, &g_Player.move, g_Player.vtxMin, g_Player.vtxMax, g_Player.bJump) == true)
	{
		if (g_Player.motionState != PLAYERMOTION_JUMP)
		{
			g_Player.bJump = false;
		}
		g_Player.move.y = 0.0f;

		if (g_Player.motionState == PLAYERMOTION_AIR)
		{
			g_Player.motionState = PLAYERMOTION_LAND;
		}
	}
	else
	{
		g_Player.bJump = true;

		//���n����
		if (g_Player.pos.y < 0.0f)
		{
			if (g_Player.motionState != PLAYERMOTION_JUMP)
			{
				g_Player.bJump = false;
			}
			g_Player.move.y = 0.0f;
			g_Player.pos.y = 0.0f;

			if (g_Player.motionState == PLAYERMOTION_AIR)
			{
				g_Player.motionState = PLAYERMOTION_LAND;
			}
		}
	}

	if (g_Player.motionState == PLAYERMOTION_DAMAGE)
	{
		CollisionReflector(&g_Player.pos, &g_Player.posOld, &g_Player.move, 25.0f, 0);
	}
	else
	{
		//���˔Ƃ̓����蔻��
		if (PushReflector(&g_Player.pos, &g_Player.posOld, &g_Player.move, &g_Player.fAngle, g_Player.bJump) == true)
		{
			if (g_Player.bJump == false)
			{
				//���[�V�������������݃��[�V�����ɕύX
				g_Player.motionState = PLAYERMOTION_PUSH;

				pCamera->bBehind = true;
			}
			else
			{
				if (g_Player.motionState == PLAYERMOTION_PUSH)
				{
					//���[�V�������������݃��[�V�����ɕύX
					g_Player.motionState = PLAYERMOTION_NORMAL;
				}
			}
		}
		else
		{
			pCamera->bBehind = false;
		}
	}

	if (g_Player.state != PLAYERSTATE_DAMAGE && g_Player.state != PLAYERSTATE_DEATH)
	{
		//�Ռ��g�Ƃ̓����蔻��
		if (CollisionShock(&g_Player.pos, &g_Player.posOld, &g_Player.rot,g_Player.bJump) == true)
		{
			if (g_Player.motionState != PLAYERMOTION_DAMAGE)
			{
				g_Player.fAngle = g_Player.rot.y;
			}

			//�v���C���[�̃q�b�g����
			HitPlayer(4);
		}

		D3DXVECTOR3 playerModelPos = D3DXVECTOR3(g_Player.aPlayerModel[2].mtxWorld._41, g_Player.aPlayerModel[2].mtxWorld._42, g_Player.aPlayerModel[2].mtxWorld._43);

		//�G�ƃv���C���[�̓����蔻��
		if (CollisionEnemy(&g_Player.pos, &playerModelPos) == true)
		{
			if (g_Player.motionState != PLAYERMOTION_DAMAGE)
			{
				g_Player.fAngle = g_Player.rot.y;
			}

			HitPlayer(2);
		}

		//�G�̍U���ƃv���C���[�̓����蔻��
		if (CollisionEnemyAttack(&g_Player.pos, &g_Player.rot) == true)
		{
			if (g_Player.motionState != PLAYERMOTION_DAMAGE)
			{
				g_Player.fAngle = g_Player.rot.y;
			}

			HitPlayer(1);
		}
	}

	//�ړ������Ɍ�������
	SetRot();

	ModelMotionPlayer();

	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_DAMAGE:
		g_Player.nTimerState--;

		if (g_Player.nTimerState <= 0)
		{
			if (g_Player.nLife > 0)
			{
				g_Player.state = PLAYERSTATE_NORMAL;
			}
		}
		break;
	case PLAYERSTATE_DEATH:
		g_Player.nTimerState--;

		if (g_Player.nTimerState <= 0)
		{
			if (g_Player.nLife > 0)
			{
				g_Player.state = PLAYERSTATE_DEATH;
			}
		}
		break;
	}

	if ((float)(g_Player.nLife) / (float)(g_Player.nMaxLife) <= 0.4f)
	{//HP�Q�[�W�̕ϐF
		if ((g_fRBJustFramePlayer % GAUGE_COLOR_FRAME) == 0)
		{
			g_fRBJustGaugePlayer == 0.0f ? g_fRBJustGaugePlayer = 1.0f : g_fRBJustGaugePlayer = 0.0f;
			g_fRBJustFramePlayer = 0;
		}

		g_fRBJustFramePlayer++;
	}

	//�̗̓Q�[�W�̐ݒ�
	if (GetMode() == MODE_GAME)
	{
		SetInfoGauge(g_Player.nIdxLifeGauge, D3DXCOLOR(g_fRBJustGaugePlayer, 1.0f, g_fRBJustGaugePlayer, 1.0f), (float)g_Player.nLife);
	}

	if (GetMode() == MODE_TUTORIAL && *GetBookState() == BOOKSTATE_END)
	{
		if (g_nCheckPlayer == 1)
		{
			//�`���[�g���A����i�߂�
			TUTORIALSTATE *tutorial = GetTutorialState();
			if (*tutorial == TUTORIALSTATE_MOVE && GetMode() == MODE_TUTORIAL)
			{
				*tutorial = TUTORIALSTATE_CAMERA;

				PlaySound(SOUND_LABEL_SE_BUTTON_004);
			}
		}

		//�v���C���[���񕜂���
		g_Player.nLife = LIFE;
	}

	//�t�H�O��ݒ�
	g_Player.environment = PLAYERENVIRONMENT_CLOUD;

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_7))
	{
		g_Player.environment = PLAYERENVIRONMENT_CLOUD;
	}
	if (GetKeyboardTrigger(DIK_8))
	{
		g_Player.environment = PLAYERENVIRONMENT_NORMAL;
	}

	if (GetKeyboardTrigger(DIK_V))
	{
		HitPlayer(20);
	}
#endif

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

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//���擾
	Model *pPlayerPart = GetModelPlayer();

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//�v�Z�p�}�g���b�N�X
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		switch (g_Player.state)
		{
		case PLAYERSTATE_NORMAL:
			//�}�e���A�����ɑ΂���|�C���^���擾
			for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
			{
				pMat->MatD3D.Diffuse = g_Player.pMatDef.MatD3D.Diffuse;

				pMat++;
			}
			break;
		case PLAYERSTATE_DAMAGE:
			if (g_Player.nTimerState >= 180)
			{
				//�}�e���A�����ɑ΂���|�C���^���擾
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pMat++;
				}
			}
			else
			{
				//�}�e���A�����ɑ΂���|�C���^���擾
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = g_Player.pMatDef.MatD3D.Diffuse;

					pMat++;
				}
			}
			break;
		case PLAYERSTATE_DEATH:
			if (g_Player.nTimerState >= 1980)
			{
				//�}�e���A�����ɑ΂���|�C���^���擾
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pMat++;
				}
			}
			else
			{
				//�}�e���A�����ɑ΂���|�C���^���擾
				for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
				{
					pMat->MatD3D.Diffuse = g_Player.pMatDef.MatD3D.Diffuse;

					pMat++;
				}
			}
			break;
		}

		//�p�[�c�̃��[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_Player.aPlayerModel[nCntModel].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_Player.aPlayerModel[nCntModel].rot.y, g_Player.aPlayerModel[nCntModel].rot.x, g_Player.aPlayerModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxRotModel);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel,
			g_Player.aPlayerModel[nCntModel].pos.x, g_Player.aPlayerModel[nCntModel].pos.y, g_Player.aPlayerModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxTransModel);

		//�p�[�c�̐e�}�g���b�N�X��ݒ�
		if (g_Player.aPlayerModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_Player.aPlayerModel[g_Player.aPlayerModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_Player.mtxWorld;
		}

		//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aPlayerModel[nCntModel].mtxWorld);

		//�}�e���A���̃f�[�^�̃|�C���g���擾
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pPlayerPart[nCntModel].apTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			pPlayerPart[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================================
//�v���C���[�̉e�̕`�揈��
//========================================
void DrawPlayerShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	D3DXMATRIX mtxShadow;						//�V���h�E�}�g���b�N�X
	D3DLIGHT9 light;							//���C�g�̏��
	D3DXVECTOR4 posLight;						//���C�g�̈ʒu
	D3DXVECTOR3 pos, normal;					//���ʏ�̔C�ӂ̈ʒu�A�@���x�N�g��
	D3DXPLANE plane;							//���ʏ��
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	//���C�g�̈ʒu��ݒ�
	pDevice->GetLight(0, &light);
	posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//���ʏ��𐶐�
	pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);

	//�V���h�E�}�g���b�N�X�̍쐬
	D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	D3DXMatrixMultiply(&mtxShadow, &g_Player.mtxWorld, &mtxShadow);

	//�V���h�E�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���擾
	Model *pPlayerPart = GetModelPlayer();

	//�S���f��(�p�[�c)�̕`��
	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;		//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxPalent;						//�e�̃}�g���b�N�X

		//�p�[�c�̃��[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&g_Player.aPlayerModel[nCntModel].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel,
			g_Player.aPlayerModel[nCntModel].rot.y, g_Player.aPlayerModel[nCntModel].rot.x, g_Player.aPlayerModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxRotModel);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTransModel,
			g_Player.aPlayerModel[nCntModel].pos.x, g_Player.aPlayerModel[nCntModel].pos.y, g_Player.aPlayerModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld, &g_Player.aPlayerModel[nCntModel].mtxWorld, &mtxTransModel);

		//�p�[�c�̐e�}�g���b�N�X��ݒ�
		if (g_Player.aPlayerModel[nCntModel].nIdxModelParent != -1)
		{
			mtxPalent = g_Player.aPlayerModel[g_Player.aPlayerModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxPalent = g_Player.mtxWorld;
		}

		//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&g_Player.aPlayerModel[nCntModel].mtxWorld,
			&mtxPalent);

		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aPlayerModel[nCntModel].mtxWorld);

		//�}�e���A���̃f�[�^�̃|�C���g���擾
		pMat = (D3DXMATERIAL*)pPlayerPart[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)pPlayerPart[nCntModel].dwNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pPlayerPart[nCntModel].apTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			pPlayerPart[nCntModel].pMesh->DrawSubset(nCntMat);
		}
	}

	//�ۑ����Ă����}�e���A����߂� 
	pDevice->SetMaterial(&matDef);

	//�ʏ�̍����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//========================================
//�v���C���[�̐ݒu
//========================================
void SetPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	if (g_Player.bUse == false)
	{
		g_Player.pos = pos;
		g_Player.posOld = pos;
		g_Player.point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.rot = rot;
		g_Player.state = PLAYERSTATE_NORMAL;
		g_Player.motionState = PLAYERMOTION_NORMAL;
		g_Player.motionStateOld = PLAYERMOTION_NORMAL;
		g_Player.environment = PLAYERENVIRONMENT_NORMAL;
		g_Player.nTimerState = 0;
		g_Player.nEasterTimer = 0;
		g_Player.fAngle = 0.0f;
		g_Player.fJumpPow = PLAYER_JUMP_POW;
		g_Player.nLife = LIFE;										//�̗�
		g_Player.nMaxLife = LIFE;									//�ő�̗�
		g_Player.bJump = false;									//�W�����v���g�p���ĂȂ���Ԃɂ���
		g_Player.bEaster = false;									//�W�����v���g�p���ĂȂ���Ԃɂ���
		g_Player.bUse = true;
		g_nCntFramePlayer = 0;
		g_nCntKeyPlayer = 0;
		g_nCheckPlayer = 0;
	}
}

//===========================
//�v���C���[�̏��擾����
//===========================
Player *GetPlayer(void)
{
	return &g_Player;
}

//===========================
//�v���C���[�̈ړ�����
//===========================
void MovePlayer(void)
{
	g_Player.move.y -= PLAYER_GRAVITY;

	g_Player.pos += g_Player.move;

	g_Player.move.x += (0.0f - g_Player.move.x) * 0.1f;
	g_Player.move.z += (0.0f - g_Player.move.z) * 0.1f;
}

//===========================
//�v���C���[�̑���(�L�[�{�[�h)����
//===========================
void ConPlayer(void)
{
	Camera *pCamera = GetCamera();
	float fRotCamera;

	//�J�����̊p�x�Z�o
	fRotCamera = atan2f(pCamera->posV.x - pCamera->posR.x, pCamera->posV.z - pCamera->posR.z);

	if (g_Player.motionState == PLAYERMOTION_PUSH)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.5f);
			}

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.5f);
			}

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ��
			g_Player.fAngle = fRotCamera + D3DX_PI;

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ��
			g_Player.fAngle = fRotCamera;

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}

		if (g_Player.fAngle > D3DX_PI)
		{
			g_Player.fAngle -= (D3DX_PI * 2);
		}
		else if (g_Player.fAngle <= -D3DX_PI)
		{
			g_Player.fAngle += (D3DX_PI * 2);
		}

		if (g_Player.fAngle >= g_Player.rot.y - D3DX_PI * 0.3f && g_Player.fAngle <= g_Player.rot.y + D3DX_PI * 0.7f)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ��
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ��
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
		}
		else
		{
			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ��
				if (GetKeyboardPress(DIK_S) == true)
				{//S�L�[�������ꂽ��
					g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
				}
				else if (GetKeyboardPress(DIK_W) == true)
				{//W�L�[�������ꂽ��
					g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
				}
				else
				{
					g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ��
				if (GetKeyboardPress(DIK_S) == true)
				{//S�L�[�������ꂽ��
					g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
				}
				else if (GetKeyboardPress(DIK_W) == true)
				{//W�L�[�������ꂽ��
					g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
				}
				else
				{
					g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
					g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y) * LENGTH;
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y + D3DX_PI) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + D3DX_PI) * LENGTH;
			}

			//���͂���Ă���
			g_nCheckPlayer = 0;
		}
	}
	else if (g_Player.state == PLAYERSTATE_DAMAGE)
	{
		
	}
	else if (g_Player.bJump == true)
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ��
			g_Player.move.x += cosf(pCamera->rot.y) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y) * LENGTH;

			g_Player.fAngle = fRotCamera + D3DX_PI;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ��
			g_Player.move.x += cosf(pCamera->rot.y + D3DX_PI) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + D3DX_PI) * LENGTH;

			g_Player.fAngle = fRotCamera;
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y - (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera - (D3DX_PI * 0.5f);
			}

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ��
			if (GetKeyboardPress(DIK_S) == true)
			{//S�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.25f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_W) == true)
			{//W�L�[�������ꂽ��
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.75f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.25f);
			}
			else
			{
				g_Player.move.x += cosf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;
				g_Player.move.z += sinf(pCamera->rot.y + (D3DX_PI * 0.5f)) * LENGTH;

				g_Player.fAngle = fRotCamera + (D3DX_PI * 0.5f);
			}

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//S�L�[�������ꂽ��
			g_Player.move.x += cosf(pCamera->rot.y) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y) * LENGTH;

			g_Player.fAngle = fRotCamera + D3DX_PI;

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{//W�L�[�������ꂽ��
			g_Player.move.x += cosf(pCamera->rot.y + D3DX_PI) * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + D3DX_PI) * LENGTH;

			g_Player.fAngle = fRotCamera;

			//���͂���Ă���
			g_nCheckPlayer = 1;
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player.state == PLAYERSTATE_NORMAL)
	{// �W�����v
		if (g_Player.bJump == false)
		{
			g_Player.bJump = true;
			g_Player.motionState = PLAYERMOTION_JUMP;
			g_nCntKeyPlayer = 0;
			g_nCntFramePlayer = 0;
		}
	}
}

//===========================
//�v���C���[�̑���(PAD)����
//===========================
void ConPlayerPad(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();
	float fRotCamera;

	//�J�����̊p�x�Z�o
	fRotCamera = atan2f(pCamera->posV.x - pCamera->posR.x, pCamera->posV.z - pCamera->posR.z);

	//�ړ�����===========================
	//�X�e�b�N�̏����擾
	D3DXVECTOR3 StickLength = GetStickL(0);	//�X�e�B�b�N�̏��ۑ�
	float fAngle = atan2f(StickLength.x, StickLength.y);	//�X�e�B�b�N�̊p�x
	float fLength = D3DXVec3Length(&StickLength); //�x�N�g���̒������擾
	if (fLength > 1.0f)
	{
		fLength = 1.0f;
	}
	D3DXVec3Normalize(&StickLength, &StickLength);	//�ۑ������x�N�g���̐��K��

	if (fLength >= 0.2f)
	{//�f�b�h�]�[���̐ݒ�

		//���͂���Ă���
		g_nCheckPlayer = 1;

		if (g_Player.motionState == PLAYERMOTION_PUSH)
		{
			g_Player.fAngle = -(pCamera->rot.y + fAngle + (D3DX_PI * 0.5f));

			if (g_Player.fAngle > D3DX_PI)
			{
				g_Player.fAngle -= (D3DX_PI * 2);
			}
			else if (g_Player.fAngle <= -D3DX_PI)
			{
				g_Player.fAngle += (D3DX_PI * 2);
			}

			if (g_Player.fAngle >= g_Player.rot.y - D3DX_PI * 0.5f && g_Player.fAngle <= g_Player.rot.y + D3DX_PI * 0.5f)
			{
				g_Player.move.x -= cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
				g_Player.move.z -= sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH;
			}
			else
			{
				g_Player.move.x += sinf(fRotCamera + (fAngle - (D3DX_PI * 0.5f))) * LENGTH;
				g_Player.move.z += cosf(fRotCamera + (fAngle - (D3DX_PI * 0.5f))) * LENGTH;

				g_Player.motionState = PLAYERMOTION_NORMAL;

				//���͂���Ă���
				g_nCheckPlayer = 0;
			}
			
		}
		else if (g_Player.state == PLAYERSTATE_DAMAGE)
		{

		}
		else if (g_Player.bJump)
		{
			g_Player.move.x += cosf(pCamera->rot.y + fAngle) * fLength * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + fAngle) * fLength * LENGTH;

			g_Player.fAngle = -(pCamera->rot.y + fAngle + (D3DX_PI * 0.5f));
		}
		else
		{
			g_Player.move.x += cosf(pCamera->rot.y + fAngle) * fLength * LENGTH;
			g_Player.move.z += sinf(pCamera->rot.y + fAngle) * fLength * LENGTH;

			g_Player.fAngle = -(pCamera->rot.y + fAngle + (D3DX_PI * 0.5f));
		}
	}

	//�ړ�����===========================

	//���_����===========================
	StickLength = GetStickR(0);	//�X�e�B�b�N�̏��ۑ�
	fLength = D3DXVec3Length(D3DXVec3Normalize(&StickLength, &StickLength));	//�ۑ������x�N�g���̐��K��

	if (fLength >= 0.2f)
	{//�f�b�h�]�[���̐ݒ�
		pCamera->rot.y -= StickLength.x * CAMERA_FACT;
		pCamera->rot.x -= StickLength.y * CAMERA_FACT;
	}
	//���_����===========================

	//�W�����v
	if (GetJoyPadButtonTrigger(KEY_A,0) && g_Player.state == PLAYERSTATE_NORMAL)
	{
		if (g_Player.bJump == false)
		{
			g_Player.bJump = true;
			g_Player.motionState = PLAYERMOTION_JUMP;
			g_nCntKeyPlayer = 0;
			g_nCntFramePlayer = 0;
		}
	}
}

//===========================
//�v���C���[�̃��[�V�����Z�b�g����
//===========================
void SetMotion(void)
{
	//���͂���Ă��邩�̔���
	if (g_nCheckPlayer == 0)
	{//���͂���Ă��Ȃ�
		if (g_Player.motionState == PLAYERMOTION_PUSH)
		{
			g_Player.motionState = PLAYERMOTION_NORMAL;
		}
		else if (g_Player.state == PLAYERSTATE_DAMAGE)
		{

		}
		else if (g_Player.bJump == true)
		{
			if (g_Player.motionState != PLAYERMOTION_JUMP)
			{
				g_Player.motionState = PLAYERMOTION_AIR;
			}
		}
		else
		{
			if (g_Player.motionState == PLAYERMOTION_LAND)
			{

			}
			else
			{
				g_Player.motionState = PLAYERMOTION_NORMAL;
			}
		}
	}
	else
	{//���͂���Ă���
		if (g_Player.motionState == PLAYERMOTION_PUSH)
		{
			g_Player.fAngle = g_Player.rot.y;
		}
		else if (g_Player.state == PLAYERSTATE_DAMAGE)
		{

		}
		else if (g_Player.bJump == true)
		{
			if (g_Player.motionState != PLAYERMOTION_JUMP)
			{
				g_Player.motionState = PLAYERMOTION_AIR;
			}
		}
		else
		{
			g_Player.motionState = PLAYERMOTION_MOVE;
		}
	}

	//�W�����v�̈ړ��ʂ�����
	if (g_Player.motionState == PLAYERMOTION_JUMP && g_nCntKeyPlayer == 1 && g_nCntFramePlayer == 0)
	{
		g_Player.move.y = g_Player.fJumpPow;
	}

	if (GetTimeUp() == true)
	{
		g_Player.motionState = PLAYERMOTION_TIMEUP;
	}

	if (g_Player.state == PLAYERSTATE_DEATH)
	{
		g_Player.motionState = PLAYERMOTION_DEATH;
	}
}

//===========================
//�v���C���[�̃��[�V��������
//===========================
void ModelMotionPlayer(void)
{
	D3DXVECTOR3 posDiff, rotDiff;

	//�G�l�~�[�̃��[�V�����Ƃقړ���
	if (g_nCntKeyPlayer < g_Player.aPlayerMotion[g_Player.motionState].nNumKey)
	{
		if (g_nCntFramePlayer < g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame)
		{
			if (g_nCntKeyPlayer == g_Player.aPlayerMotion[g_Player.motionState].nNumKey - 1)
			{
				for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
				{
					if (g_Player.aPlayerMotion[g_Player.motionState].nLoop == 0)
					{
						posDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x;
						posDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y;
						posDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z;

						rotDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x;
						rotDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y;
						rotDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z;

						//pos�𑫂�
						g_Player.aPlayerModel[nCntModel].pos.x = g_Player.aPlayerModel[nCntModel].posMotion.x + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x + (posDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.y = g_Player.aPlayerModel[nCntModel].posMotion.y + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y + (posDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.z = g_Player.aPlayerModel[nCntModel].posMotion.z + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z + (posDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));

						//rot�𑫂�
						g_Player.aPlayerModel[nCntModel].rot.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x + (rotDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y + (rotDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z + (rotDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					}
					else
					{
						posDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].pos[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x;
						posDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].pos[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y;
						posDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].pos[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z;

						rotDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].rot[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x;
						rotDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].rot[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y;
						rotDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[0].rot[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z;

						//pos�𑫂�
						g_Player.aPlayerModel[nCntModel].pos.x = g_Player.aPlayerModel[nCntModel].posMotion.x + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x + (posDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.y = g_Player.aPlayerModel[nCntModel].posMotion.y + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y + (posDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].pos.z = g_Player.aPlayerModel[nCntModel].posMotion.z + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z + (posDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));

						//rot�𑫂�
						g_Player.aPlayerModel[nCntModel].rot.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x + (rotDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y + (rotDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
						g_Player.aPlayerModel[nCntModel].rot.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z + (rotDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					}
				
					if (g_Player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}
			else
			{
				for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
				{
					posDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].pos[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x;
					posDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].pos[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y;
					posDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].pos[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z;

					rotDiff.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].rot[nCntModel].x - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x;
					rotDiff.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].rot[nCntModel].y - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y;
					rotDiff.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer + 1].rot[nCntModel].z - g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z;

					//pos�𑫂�
					g_Player.aPlayerModel[nCntModel].pos.x = g_Player.aPlayerModel[nCntModel].posMotion.x + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].x + posDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame);
					g_Player.aPlayerModel[nCntModel].pos.y = g_Player.aPlayerModel[nCntModel].posMotion.y + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].y + posDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame);
					g_Player.aPlayerModel[nCntModel].pos.z = g_Player.aPlayerModel[nCntModel].posMotion.z + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel].z + posDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame);

					//rot�𑫂�
					g_Player.aPlayerModel[nCntModel].rot.x = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].x + (rotDiff.x * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					g_Player.aPlayerModel[nCntModel].rot.y = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].y + (rotDiff.y * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));
					g_Player.aPlayerModel[nCntModel].rot.z = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel].z + (rotDiff.z * (g_nCntFramePlayer / (float)g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].nFrame));

					if (g_Player.aPlayerModel[nCntModel].rot.x > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.x < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.x += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.y > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.y < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.y += 6.28f;
					}

					if (g_Player.aPlayerModel[nCntModel].rot.z > 3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z -= 6.28f;
					}
					else if (g_Player.aPlayerModel[nCntModel].rot.z < -3.14f)
					{
						g_Player.aPlayerModel[nCntModel].rot.z += 6.28f;
					}
				}
			}

			g_nCntFramePlayer++;
		}
		else
		{
			g_nCntKeyPlayer++;
			g_nCntFramePlayer = 0;

			for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
			{
				g_Player.aPlayerModel[nCntModel].rotMotion = g_Player.aPlayerModel[nCntModel].rot;
			}
		}
	}
	else
	{
		if (g_Player.aPlayerMotion[g_Player.motionState].nLoop == 0)
		{
			if (g_Player.motionState == PLAYERMOTION_JUMP)
			{
				g_Player.motionState = PLAYERMOTION_AIR;
			}
			else if (g_Player.motionState == PLAYERMOTION_DAMAGE)
			{
				g_Player.motionState = PLAYERMOTION_NORMAL;
			}
			else
			{
				g_Player.motionState = PLAYERMOTION_NORMAL;
			}
		}

		//�L�[�ƃt���[�������Z�b�g
		g_nCntKeyPlayer = 0;
		g_nCntFramePlayer = 0;
	}

	if (g_Player.motionState != g_Player.motionStateOld)
	{
		g_nCntKeyPlayer = 0;
		g_nCntFramePlayer = 0;
		for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
		{
			g_Player.aPlayerModel[nCntModel].pos = g_Player.aPlayerModel[nCntModel].posMotion + g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].pos[nCntModel];
			g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerModel[nCntModel].rotMotion;
			g_Player.aPlayerModel[nCntModel].rot = g_Player.aPlayerMotion[g_Player.motionState].aKeyMotion[g_nCntKeyPlayer].rot[nCntModel];
		}
	}
}

//===========================
//�v���C���[�̊p�x��������
//===========================
void SetRot(void)
{
	float fRotMove, fRotDest, fRotDiff;

	//���݂̊p�x�ƖړI�̊p�x�̍������v�Z
	fRotMove = g_Player.rot.y;
	fRotDest = g_Player.fAngle;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	//���X�ɑ����Ă�
	fRotMove += fRotDiff * 0.2f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_Player.rot.y = fRotMove;
}

//========================================
//�v���C���[�̈ʒu��������
//========================================
void SetPosPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	g_Player.pos = pos;
	g_Player.rot = rot;
}

//========================================
//�v���C���[�̍폜
//========================================
void DeletePlayer(void)
{
	g_Player.bUse = false;
}

//========================================
//�v���C���[�̃_���[�W����
//========================================
void HitPlayer(int nDamage)
{
	if (g_Player.state != PLAYERSTATE_DAMAGE && g_Player.state != PLAYERMOTION_DEATH && g_Player.nLife > 0)
	{//�̗͂����炷

		if (GetGameState() == GAMESTATE_NORMAL)
		{//�ʏ��Ԃł̂ݑ̗͌���
			g_Player.nLife -= nDamage;

			//�_���[�W���Đ�
			PlaySound(SOUND_LABEL_SE_DAMAGE);

			//�X�R�A�̌���
			AddScore(SHOCK_SCORE);

			//�擾�X�R�A�\���ݒ�
			SetPointScore(SHOCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_DAMAGE);

			g_Player.state = PLAYERSTATE_DAMAGE;
			g_Player.motionState = PLAYERMOTION_DAMAGE;
			g_Player.nTimerState = 200;
			g_nCntKeyPlayer = 0;
			g_nCntFramePlayer = 0;

			g_Player.move.x = cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;
			g_Player.move.z = sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;

			SetQuake(10, 0.3f);
		}
	}

	//�v���C���[�̗̑͂��Ȃ��Ȃ���
	if (g_Player.nLife <= 0)
	{
		g_Player.nLife = 0;
		g_Player.nTimerState = 2000;
		g_Player.motionState = PLAYERMOTION_DEATH;
		g_Player.state = PLAYERSTATE_DEATH;
		g_Player.move.x = cosf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;
		g_Player.move.z = sinf(-g_Player.rot.y + D3DX_PI * 0.5f) * LENGTH * 30.0f;
		DeletePlayer();					//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//===========================
//�v���C���[�̃��[�V��������
//===========================
void ReadFilePlayer(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MOTION\\motion_TH.txt", "r");

	//�t�@�C���ǂݍ���
	if (pFile == NULL)
	{
		return;
	}

	do
	{
		fgets(&aText[0], 128, pFile);
	} while (strcmp(aText, "CHARACTERSET\n") != 0);

	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_Player.fMove, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &g_Player.fJump, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_Player.nNumModel, &aText[0], &aText[0]);

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &g_Player.aPlayerModel[nCntModel].nIdxModelParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &g_Player.aPlayerModel[nCntModel].nIdxModelParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_Player.aPlayerModel[nCntModel].posMotion.x, &g_Player.aPlayerModel[nCntModel].posMotion.y, &g_Player.aPlayerModel[nCntModel].posMotion.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_Player.aPlayerModel[nCntModel].rotMotion.x, &g_Player.aPlayerModel[nCntModel].rotMotion.y, &g_Player.aPlayerModel[nCntModel].rotMotion.z);
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
		fscanf(pFile, "%s = %d	%s %s %s %s", &aText[0], &g_Player.aPlayerMotion[nCntMotion].nLoop, &aText[0], &aText[0], &aText[0], &aText[0]);
		fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &g_Player.aPlayerMotion[nCntMotion].nNumKey, &aText[0], &aText[0]);

		for (int nCntKey = 0; nCntKey < g_Player.aPlayerMotion[nCntMotion].nNumKey; nCntKey++)
		{
			fgets(&aText[0], 128, pFile);
			fscanf(pFile, "%s = %d\n", &aText[0], &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].nFrame);

			for (int nCnt = 0; nCnt < g_Player.nNumModel; nCnt++)
			{
				fgets(&aText[0], 128, pFile);
				fscanf(pFile, "%s = %f %f %f", &aText[0], &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].x, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].y, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].pos[nCnt].z);
				fscanf(pFile, "%s = %f %f %f\n", &aText[0], &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].x, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].y, &g_Player.aPlayerMotion[nCntMotion].aKeyMotion[nCntKey].rot[nCnt].z);
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

//========================================
//�v���C���[�̃��[�V����
//========================================
void DebugPlayerMotion(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,580,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "���̃��[�V����; %d\n�O�̃��[�V����; %d :%d", g_Player.motionState, g_Player.motionStateOld, g_Player.state);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_RIGHT, D3DCOLOR_RGBA(200, 200, 200, 255));
}

//===========================
//�v���C���[�̈ړ���������
//===========================
void LimitMovePlayer(void)
{
	if (g_Player.pos.x + PLAYER_RADIUS > (FIELD_RANGE * MESH_U) * 0.5f)
	{//�G���A�E�̐���
		g_Player.pos.x = (FIELD_RANGE * MESH_U) * 0.5f - PLAYER_RADIUS;
	}
	else if (g_Player.pos.x - PLAYER_RADIUS < -(FIELD_RANGE * MESH_U) * 0.5f)
	{//�G���A���̐���
		g_Player.pos.x = -(FIELD_RANGE * MESH_U) * 0.5f + PLAYER_RADIUS;
	}

	if (g_Player.pos.z + PLAYER_RADIUS >(FIELD_RANGE * MESH_V) * 0.5f)
	{//�G���A���̐���
		g_Player.pos.z = (FIELD_RANGE * MESH_V) * 0.5f - PLAYER_RADIUS;
	}
	else if (g_Player.pos.z - PLAYER_RADIUS < -(FIELD_RANGE * MESH_V) * 0.5f)
	{//�G���A��O�̐���
		g_Player.pos.z = -(FIELD_RANGE * MESH_V) * 0.5f + PLAYER_RADIUS;
	}
}

//==========================================
//  ����Ԃ̎擾
//==========================================
PLAYERENVIRONMENT GetEnvironment()
{
	return g_Player.environment;
}

//==========================================
//�����L���O�ł̃v���C���[�̍X�V  
//==========================================
void UpdatePlayerRanking(void)
{
	//�O��̃��[�V������Ԃ�ۑ�
	g_Player.motionStateOld = g_Player.motionState;

	if (GetKeyboardPress(DIK_T) == true &&
		GetKeyboardPress(DIK_K) == true &&
		GetKeyboardPress(DIK_D) == true)
	{//TKD�L�[�������ꂽ��
		g_Player.bEaster = true;
	}

	SetMotionRanking();

	//���[�V�����Ǘ�����
	ModelMotionPlayer();

	//�e�̈ʒu�ݒ�
		SetPositionShadow(g_Player.nIdxShadow,
							D3DXVECTOR3(g_Player.pos.x, 0.1f, g_Player.pos.z),
							g_Player.rot,
							1.0f);
}

//==========================================
//�����L���O�ł̃��[�V�����̍X�V
//==========================================
void SetMotionRanking(void)
{
	if (g_Player.bEaster == true)
	{
		if (g_Player.nEasterTimer < 30)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG6;

			if (g_Player.nEasterTimer == 0)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 90)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG0;

			if (g_Player.nEasterTimer == 30)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 630)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 90)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 690)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;
			
			if (g_Player.nEasterTimer == 630)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 750)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 690)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 810)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;
			
			if (g_Player.nEasterTimer == 750)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 930)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG3;
			
			if (g_Player.nEasterTimer == 810)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 960)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG4;
			
			if (g_Player.nEasterTimer == 930)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1050)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG5;
			
			if (g_Player.nEasterTimer == 960)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1590)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 1050)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1650)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;
			
			if (g_Player.nEasterTimer == 1590)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1710)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;

			if (g_Player.nEasterTimer == 1650)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1770)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG2;

			if (g_Player.nEasterTimer == 1710)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1890)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG3;
			
			if (g_Player.nEasterTimer == 1770)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 1920)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG4;
			
			if (g_Player.nEasterTimer == 1890)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 2010)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG5;
			
			if (g_Player.nEasterTimer == 1920)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 2250)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG1;
			
			if (g_Player.nEasterTimer == 2010)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer < 2370)
		{
			//���[�V������ݒ�
			g_Player.motionState = PLAYERMOTION_EASTEREGG6;

			if (g_Player.nEasterTimer == 2250)
			{
				g_nCntKeyPlayer = 0;
				g_nCntFramePlayer = 0;
			}
		}
		else if (g_Player.nEasterTimer > 2600)
		{
			g_Player.nEasterTimer = 0;
		}

		g_Player.rot.y = 0.0f;
		g_Player.nEasterTimer++;
	}
	else
	{
		//���[�V������ݒ�
		g_Player.motionState = PLAYERMOTION_MOVE;
	}
}

//==========================================
//�v���C���[�̃T�E���h�֌W����
//==========================================
void SoundPlayer(void)
{
	if (g_Player.motionState == PLAYERMOTION_MOVE)
	{
		if (g_nCntKeyPlayer == 3 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_STEP_000);
		}

		if (g_nCntKeyPlayer == 7 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_STEP_000);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_JUMP)
	{
		if (g_nCntKeyPlayer == 1 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_JUMP_000);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_LAND)
	{
		if (g_nCntKeyPlayer == 0 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_LAND);
			SetParticle(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 1.0f, g_Player.pos.z), PARTICLETYPE_LANDED);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_DAMAGE)
	{
		if (g_nCntKeyPlayer == 2 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_LAND);
		}
	}

	if (g_Player.motionState == PLAYERMOTION_DEATH)
	{
		if (g_nCntKeyPlayer == 2 && g_nCntFramePlayer == 0)
		{
			PlaySound(SOUND_LABEL_SE_LAND);
		}
	}
}
