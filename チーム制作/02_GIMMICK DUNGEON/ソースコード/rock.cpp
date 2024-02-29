//========================================
//
//2D�A�N�V�����Q�[��(�⏈��)[rock.cpp]
//Author:���v�ԗD��
//
//========================================
//========================
// �C���N���[�h
//========================
#include "main.h"
#include "rock.h"
#include "edit.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "object.h"
#include "enemy.h"
#include "player.h"
#include "stdio.h"
#include "score.h"
#include "particle.h"
#include "wall.h"
#include "ceiling.h"
#include "shadow.h"
#include "orbit.h"
#include "sound.h"
#include "collision.h"
#include "point.h"
#include "point_log.h"

//========================
// �}�N����`
//========================
#define SPEED				(4.0f)					//���x
#define LIFE				(600)					//����
#define RADIUS_PLAYER		(50.0f)					//���a �v���C���[
#define RADIUS_ENEMY		(150.0f)				//���a�@�G
#define DEBUG_CLO_R			(1.0f)					//�f�o�b�O�̐FR
#define DEBUG_CLO_G			(0.0f)					//�f�o�b�O�̐FG
#define DEBUG_CLO_B			(0.5f)					//�f�o�b�O�̐FB
#define DEBUG_CLO_A			(1.0f)					//�f�o�b�O�̐FA
#define DEBUG_TOP			(140)					//�f�o�b�O�̈ʒu
#define ROCK_SCORE			(500)					//�G�Ɋ�𓖂Ă����̃X�R�A	
#define MAX_TEX				(10)					//�e�N�X�`���̐�
#define ROTATE_SPEED		(0.2f)					//��]���x
#define SHADOW_RADIUS		(60.0f)					//�e�̔��a
#define FRAG_NUM			(11)					//��ꂽ�ۂ̔j�Ђ̐�
#define HIT_DAMAGE			(10)					//�G�ɗ^���鎖�̂ł���_���[�W
#define ROLL_SOUND_TIME		(20)					//�]���鉹���Đ�����p�x

//========================
// �v���g�^�C�v�錾
//========================
void CollisionRock(Rock *pRock);				//���f���̓����蔻��
void CollisionRock3(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);	//���f���ƃI�u�W�F�N�g�̓����蔻��
bool CollisionRockWall(int nCntRock);
bool CollisionRockRock(int nCntRock);
void StateChangeRock(void);						//��Ԃ̕ύX
void DebugRock(void);							//���f���̂Ńo�b�N�\��
void RotateRock(Rock *pRock);	//���]��������

//========================
// �O���[�o���ϐ��錾
//========================
Rock g_Rock[MAX_ROCK];											//�\���̂̏��
LPD3DXBUFFER g_pBuffMatRock[ROCKTYPE_MAX];						//�}�e���A���ւ̃o�b�t�@
LPD3DXMESH g_pMeshRock[ROCKTYPE_MAX];							//���b�V���ւ̃|�C���^
DWORD g_dwNumMatRock[ROCKTYPE_MAX];								//�}�e���A���̐�
LPDIRECT3DTEXTURE9 g_apTextureRock[ROCKTYPE_MAX][MAX_TEX];		//�e�N�X�`���ւ̃|�C���^

int g_nNumRock;	//��̑���
int g_RockCounter;	//��ꂽ��̃J�E���^�[
int g_nCntRollSound;	//�]���鉹�̃^�C�}�[

//========================================
//��̏���������
//========================================
void InitRock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	int nCntRock;

	//�O���[�o���ϐ�������
	g_RockCounter = 0;		//��̃J�E���^�[
	g_nNumRock = 0;

	//�e��ϐ��̏�����
	for (nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		ZeroMemory(&g_Rock[nCntRock],sizeof(Rock));
		g_Rock[nCntRock].nIdxShadow = -1;
		g_Rock[nCntRock].nIdxOrbit = -1;
	}

	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	for (nCntRock = 0; nCntRock < ROCKTYPE_MAX; nCntRock++)
	{
		//X�t�@�C���ǂݍ���
		D3DXLoadMeshFromX("data\\MODEL\\OBJECT\\magstone_000.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatRock[nCntRock],
			NULL,
			&g_dwNumMatRock[nCntRock],
			&g_pMeshRock[nCntRock]);

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatRock[nCntRock]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatRock[nCntRock]; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_apTextureRock[nCntRock][nCntMat]);
			}
		}
	}
}

//========================================
// ��̏I������
//========================================
void UninitRock(void)
{
	for (int nCount = 0;nCount < ROCKTYPE_MAX;nCount++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatRock[nCount]; nCntMat++)
		{// �e�N�X�`���̔j��
			if (g_apTextureRock[nCount][nCntMat] != NULL)
			{
				g_apTextureRock[nCount][nCntMat]->Release();
				g_apTextureRock[nCount][nCntMat] = NULL;
			}
		}

		// ���b�V���̔j��
		if (g_pMeshRock[nCount] != NULL)
		{
			g_pMeshRock[nCount]->Release();
			g_pMeshRock[nCount] = NULL;
		}

		// �}�e���A���̔j��
		if (g_pBuffMatRock[nCount] != NULL)
		{
			g_pBuffMatRock[nCount]->Release();
			g_pBuffMatRock[nCount] = NULL;
		}
	}
}

//========================================
// ��̍X�V����
//========================================
void UpdateRock(void)
{
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	Orbit *pOrbit = GetOrbit();

	bool bSound = false; //�]���鉹�̏����ɂ͂��������ǂ���

	for (int nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		if (g_Rock[nCntRock].nType == ROCKTYPE_INVISIBLE)
		{
			if (g_Rock[nCntRock].bUse == true)
			{
				if (g_Rock[nCntRock].bWallInvisible == true)
				{//�ǂɓ���������
					if (g_Rock[nCntRock].nCounter >= 1)
					{//�������
						g_Rock[nCntRock].nCounter = 0;
						DisableRock(&g_Rock[nCntRock]);
					}
					else
					{//�J�E���^�[���Z
						g_Rock[nCntRock].nCounter++;
					}
				}

				if (g_Rock[nCntRock].bUse == true)
				{
					pOrbit[g_Rock[nCntRock].nIdxOrbit].col = D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f);

					while (1)
					{
						// �O��̈ʒu��ۑ�
						g_Rock[nCntRock].posOld = g_Rock[nCntRock].pos;

						// �]��������
						RotateRock(&g_Rock[nCntRock]);

						WindMove(nCntRock);

						if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
						{//�V�䗠��������

							// �ʒu���X�V
							g_Rock[nCntRock].pos += g_Rock[nCntRock].move * g_Rock[nCntRock].fSpeed;
						}
						else
						{
							// �ʒu���X�V
							g_Rock[nCntRock].pos += g_Rock[nCntRock].move * g_Rock[nCntRock].fSpeed;
						}

						// ��Ԃ̕ω�
						StateChangeRock();

						// ���˔Ƃ̓����蔻��
						if (CollisionReflector(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, g_Rock[nCntRock].vtxMax.x, g_Rock[nCntRock].nType) == true)
						{
							// �����̃J�E���g�_�E��
							g_Rock[nCntRock].nLife--;
							break;
						}

						if (CollisionRockInvisible(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].vtxMax.x) == true)
						{
							if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
							{//�V�䗠��������
								pOrbit[g_Rock[nCntRock].nIdxOrbit].col = D3DXCOLOR(1.0f, 0.3f, 0.0f, 1.0f);
							}
							else
							{
								g_Rock[nCntRock].bWallInvisible = true;
								break;
							}
						}

						//�ǂƂ̓����蔻��
						if (CollisionWall(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, 0.0f) == true)
						{
							g_Rock[nCntRock].bWallInvisible = true;
							break;
						}

						if (g_Rock[nCntRock].nLife <= 0)
						{
							g_Rock[nCntRock].bWallInvisible = true;
							break;
						}

						if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
						{//�V�䗠��������

							g_Rock[nCntRock].nLife --;

							//���[���h�}�g���b�N�X�̏�����
							D3DXMatrixIdentity(&g_Rock[nCntRock].mtxWorld);

							//�����𔽉f
							D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rock[nCntRock].rot.y, g_Rock[nCntRock].rot.x, g_Rock[nCntRock].rot.z);
							D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxRot);

							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, g_Rock[nCntRock].pos.x, g_Rock[nCntRock].pos.y, g_Rock[nCntRock].pos.z);
							D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxTrans);

							//�O�Ղ̈ʒu�ݒ�
							SetPositionOffset(g_Rock[nCntRock].mtxWorld, g_Rock[nCntRock].nIdxOrbit);
						}
					}
					
					if (g_Rock[nCntRock].pos.y < CEILING_POSY)
					{//�n�ゾ������

						//�O�Ղ̈ʒu�ݒ�
						SetPositionOffset(g_Rock[nCntRock].mtxWorld, g_Rock[nCntRock].nIdxOrbit);
					}
				}
			}
		}
		else
		{
			if (g_Rock[nCntRock].bUse == true)
			{
				// �O��̈ʒu��ۑ�
				g_Rock[nCntRock].posOld = g_Rock[nCntRock].pos;

				if (bSound == false)
				{//�]���鉹��炷����
					RockRollSound(&g_Rock[nCntRock],&bSound);
				}

				// �]��������
				RotateRock(&g_Rock[nCntRock]);

				if (g_Rock[nCntRock].nCounter >= ROCK_TIME)
				{// �J�E���^�[�̐���
					g_Rock[nCntRock].nCounter = ROCK_TIME;
				}
				else
				{
					// �J�E���^�[�����Z
					g_Rock[nCntRock].nCounter++;
				}

				// ���̈ړ��ʉ��Z����
				WindMove(nCntRock);

				// �ʒu���X�V
				g_Rock[nCntRock].pos += g_Rock[nCntRock].move * g_Rock[nCntRock].fSpeed;

				if (g_Rock[nCntRock].nType == ROCKTYPE_THROW || g_Rock[nCntRock].nType == ROCKTYPE_DROP)
				{
					g_Rock[nCntRock].move.y -= 0.03f;

					if (g_Rock[nCntRock].pos.y < 50.0f)
					{
						g_Rock[nCntRock].move.y = g_Rock[nCntRock].move.y * -0.7f;
						g_Rock[nCntRock].pos.y = 50.0f;
					}
				}

				// ��Ԃ̕ω�
				StateChangeRock();

				// �����̃J�E���g�_�E��
				g_Rock[nCntRock].nLife--;

				// ���˔Ƃ̓����蔻��
				CollisionReflector(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, g_Rock[nCntRock].vtxMax.x, g_Rock[nCntRock].nType);

				//�ǂƂ̓����蔻��
				if (CollisionWall(&g_Rock[nCntRock].pos, &g_Rock[nCntRock].posOld, &g_Rock[nCntRock].move, g_Rock[nCntRock].vtxMax.x) == true)
				{
					DisableRock(&g_Rock[nCntRock]);
				}

				if (CollisionRockRock(nCntRock) == true)
				{
					DisableRock(&g_Rock[nCntRock]);
				}

				//�v���C���[�Ƃ̓����蔻��
				CollisionRockPlayer(nCntRock);

				if (g_Rock[nCntRock].pos.y >= CEILING_POSY)
				{//�V�䗠��������
					SetParticle(
						D3DXVECTOR3(g_Rock[nCntRock].pos.x,
							g_Rock[nCntRock].pos.y - RADIUS_PLAYER,
							g_Rock[nCntRock].pos.z),
						PARTICLETYPE_SANDDUST
					);
				}

				if (g_Rock[nCntRock].pos.y < CEILING_POSY)
				{//�n�ゾ������
					// �e�̈ʒu���X�V����
					SetPositionShadow(g_Rock[nCntRock].nIdxShadow,
						D3DXVECTOR3(g_Rock[nCntRock].pos.x, 0.1f, g_Rock[nCntRock].pos.z),
						D3DXVECTOR3(0.0f, 0.0f, 0.0f),
						1.0f);
				}

				// �������s����
				if (g_Rock[nCntRock].nLife <= 0)
				{//�����������
					DisableRock(&g_Rock[nCntRock]);
				}
			}
		}
	}
}

//========================================
// �₪�]���鉹�̊Ǘ�
//========================================
void RockRollSound(Rock *pRock,bool *bSound)
{
	//���擾
	Player *pPlayer = GetPlayer();
	bool bNear;

	// ������������ʒu�ɂ��邩�̓����蔻��
	bNear = SphereCollision(pRock->pos,pPlayer->pos,ROLLSOUND_RANGE,0.0f);

	if (bNear)
	{//������������͈͂Ȃ�
		if (g_nCntRollSound == 0)
		{
			//�����擾
			D3DXVECTOR3 posDiff = pRock->pos - pPlayer->pos;
			float fDiffLength;

			fDiffLength = D3DXVec3Length(&posDiff);

			//�����̗ʂɂ���ĉ��ʂ�ς���
			SetVolume(SOUND_LABEL_SE_ROLL, (1.0f - fDiffLength / ROLLSOUND_RANGE) * 1.5f);

			//�]���鉹�Đ�
			PlaySound(SOUND_LABEL_SE_ROLL);

			//�J�E���^�[���Z�b�g
			g_nCntRollSound = ROLL_SOUND_TIME;
		}

		//�J�E���^�[���Z
		g_nCntRollSound--;

		// ������������͈͂ɓ������̂�`����
		*bSound = true;
	}
}

//========================================
// ���]��������
//========================================
void RotateRock(Rock *pRock)
{
	// ����ړ������Ɍ����鏈��
	pRock->rot.y = atan2f(pRock->move.x, pRock->move.z);

	if (pRock->nType != ROCKTYPE_INVISIBLE)
	{
		// �₪X���œ]���鏈��
		pRock->rot.x += ROTATE_SPEED;
	}
}

//========================================
// �����������
//========================================
void DisableRock(Rock *pRock)
{
	//���擾
	Player *pPlayer = GetPlayer();

	//�����擾
	D3DXVECTOR3 posDiff = pRock->pos - pPlayer->pos;
	float fDiffLength;

	fDiffLength = D3DXVec3Length(&posDiff);

	//�g�p���Ă��Ȃ���Ԃɂ���
	pRock->bUse = false;

	if (SphereCollision(pRock->pos, pPlayer->pos, ROLLSOUND_RANGE, 0) == true && pRock->nType != ROCKTYPE_INVISIBLE)
	{//�v���C���[�����̕�������͈͓���������

		//�����̗ʂɂ���ĉ��ʂ�ς���
		SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);

		PlaySound(SOUND_LABEL_SE_CRASH);
	}

	if (pRock->nType == ROCKTYPE_INVISIBLE)
	{
		//�O�Ղ̍폜
		EnableOrbit(pRock->nIdxOrbit);
	}
	else
	{
		if (pRock->pos.y < CEILING_POSY)
		{
			//�e�̍폜
			EnableShadow(pRock->nIdxShadow, false);
			//�p�[�e�B�N���̐ݒ�0
			SetBreakObj(pRock->pos, FRAG_NUM);
		}

		//�⑍�����Z
		g_nNumRock--;
	}
}

//========================================
// ��̕`�揈��
//========================================
void DrawRock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Rock[nCntRock].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rock[nCntRock].rot.y, g_Rock[nCntRock].rot.x, g_Rock[nCntRock].rot.z);

		D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Rock[nCntRock].pos.x, g_Rock[nCntRock].pos.y, g_Rock[nCntRock].pos.z);

		D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Rock[nCntRock].mtxWorld);


		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatRock[g_Rock[nCntRock].nType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatRock[g_Rock[nCntRock].nType]; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			switch (g_Rock[nCntRock].state)
			{

			case ROCKSTATE_NORMAL:
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				break;

			case ROCKSTATE_DAMEGE:
				pDevice->SetMaterial(&g_Rock[nCntRock].RedMat.MatD3D);
				break;
			}
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureRock[g_Rock[nCntRock].nType][nCntMat]);

			if (g_Rock[nCntRock].bUse && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
			{
				//���f��(�p�[�c)�̕`��
				g_pMeshRock[g_Rock[nCntRock].nType]->DrawSubset(nCntMat);
			}
		}
		//�ۑ����Ă����}�e���A����߂� 
		pDevice->SetMaterial(&matDef);
	}

#ifdef _DEBUG
	//��̃f�o�b�O�\��
	//DebugRock();
#endif
}

//========================================
//��̐ݒ菈��
//========================================
void SetRock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot,int nLife, int nType)
{
	int nCntRock;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	for (nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		if (g_Rock[nCntRock].bUse == false)
		{//�g�p����ĂȂ��Ȃ�
			//���ݒ�
			g_Rock[nCntRock].pos = pos;							//�ʒu
			g_Rock[nCntRock].move = move;						//�ړ���
			g_Rock[nCntRock].rot = rot;							//����
			g_Rock[nCntRock].nLife = nLife;						//����
			g_Rock[nCntRock].nType = nType;						//���
			g_Rock[nCntRock].bUse = true;						//�g�p���Ă����Ԃɂ���
			g_Rock[nCntRock].bWallInvisible = false;
			g_Rock[nCntRock].nCounter = 0;
			g_Rock[nCntRock].fSpeed = SPEED;
			
			//���f���̓����蔻��
			CollisionRock(&g_Rock[nCntRock]);

			//�e�̐ݒ�
			if (pos.y < CEILING_POSY && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
			{
				g_Rock[nCntRock].nIdxShadow = SetShadow(SHADOW_RADIUS, SHADOWTYPE_CIRCLE, 0.0f, 0.0f,g_Rock[nCntRock].rot);
				SetPositionShadow(g_Rock[nCntRock].nIdxShadow, D3DXVECTOR3(g_Rock[nCntRock].pos.x,0.1f, g_Rock[nCntRock].pos.z), g_Rock[nCntRock].rot,1.0f);
			}

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Rock[nCntRock].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Rock[nCntRock].rot.y, g_Rock[nCntRock].rot.x, g_Rock[nCntRock].rot.z);

			D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Rock[nCntRock].pos.x, g_Rock[nCntRock].pos.y, g_Rock[nCntRock].pos.z);

			D3DXMatrixMultiply(&g_Rock[nCntRock].mtxWorld, &g_Rock[nCntRock].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Rock[nCntRock].mtxWorld);

			if (g_Rock[nCntRock].nType == ROCKTYPE_INVISIBLE)
			{
				if (g_Rock[nCntRock].pos.y >= CEILING_POSY/* && (rand() % 2) == 0*/)
				{//�V�䗠��������

					 // �O�Ղ̐ݒ�
					g_Rock[nCntRock].nIdxOrbit = SetOrbit(g_Rock[nCntRock].mtxWorld, D3DXVECTOR3(-10.0f, -100.0f, 0.0f), D3DXVECTOR3(10.0f, -100.0f, 0.0f), D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), 180);
				}

				if (g_Rock[nCntRock].pos.y < CEILING_POSY)
				{//�n�ゾ������

					// �O�Ղ̐ݒ�
					g_Rock[nCntRock].nIdxOrbit = SetOrbit(g_Rock[nCntRock].mtxWorld, D3DXVECTOR3(0.0f, -10.0f, 0.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXCOLOR(0.0f, 0.3f, 1.0f, 1.0f), 5);
				}
			}
			else
			{//�s���̊�͉��Z���Ȃ�
				//�������Z
				g_nNumRock++;
			}
			
			break;
		}
	}
}

//========================================
//��Ԃ̕ύX
//========================================
void StateChangeRock(void)
{
	for (int nCntModel = 0; nCntModel < MAX_ROCK; nCntModel++)
	{
		if (g_Rock[nCntModel].bUse == true)
		{
			switch (g_Rock[nCntModel].state)
			{
			case ROCKSTATE_NORMAL:
				break;

			case ROCKSTATE_DAMEGE:
				g_Rock[nCntModel].nCounterState--;

				if (g_Rock[nCntModel].nCounterState <= 0)
				{
					g_Rock[nCntModel].state = ROCKSTATE_NORMAL;
				}
				break;
			}
		}
	}
}

//========================================
//��̍ő�E�ŏ��l�擾
//========================================
void CollisionRock(Rock *pRock)
{
	int nNumVtx;				//���_��
	DWORD dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;				//���_�o�b�t�@�ւ̃|�C���^

	//���_�����擾
	nNumVtx = g_pMeshRock[pRock->nType]->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshRock[pRock->nType]->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_pMeshRock[pRock->nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntModel = 0; nCntModel < nNumVtx; nCntModel++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//���_���W�̑��

		if (vtx.x < pRock->vtxMin.x)
		{
			pRock->vtxMin.x = vtx.x;
		}
		if (vtx.x > pRock->vtxMax.x)
		{
			pRock->vtxMax.x = vtx.x;
		}

		if (vtx.y < pRock->vtxMin.y)
		{
			pRock->vtxMin.y = vtx.y;
		}
		if (vtx.y > pRock->vtxMax.y)
		{
			pRock->vtxMax.y = vtx.y;
		}

		if (vtx.z < pRock->vtxMin.z)
		{
			pRock->vtxMin.z = vtx.z;
		}
		if (vtx.z > pRock->vtxMax.z)
		{
			pRock->vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}
	pRock->vtxMax;
	pRock->vtxMin;

	//���_�o�b�t�@���A�����b�N
	g_pMeshRock[pRock->nType]->UnlockVertexBuffer();
}

//========================================
//��ƃv���C���[�̓����蔻��
//========================================
void CollisionRockPlayer(int nCntRock)
{
	//���擾
	Player *pPlayer = GetPlayer();
	bool bHit = false;

	if (pPlayer->motionState != PLAYERMOTION_DAMAGE)
	{//�v���C���[���_���[�W���[�V�����łȂ��Ƃ�
		bHit = SphereCollision(g_Rock[nCntRock].pos, pPlayer->pos, PLAYER_RADIUS, g_Rock[nCntRock].vtxMax.x);

		if (bHit)
		{//�����������ǂ���

			//�v���C���[�̌�������Ɍ�����
			pPlayer->rot.y = atan2f(pPlayer->pos.x - g_Rock[nCntRock].pos.x, pPlayer->pos.z - g_Rock[nCntRock].pos.z);

			if (pPlayer->motionState != PLAYERMOTION_DAMAGE)
			{//�v���C���[����Ɍ�����
				pPlayer->fAngle = pPlayer->rot.y;
			}

			//�v���C���[�Ƀ_���[�W��^����
			HitPlayer(2);

			//��j��
			DisableRock(&g_Rock[nCntRock]);

			//��j�󐔃J�E���g
			g_RockCounter++;
		}
	}
}

//========================================
//��ƃG�l�~�[�̓����蔻��
//========================================
void CollisionRockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pPoint, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump)
{
	//�ϐ��錾
	int nCntRock;
	float fLengthS;		//2�_�Ԃ̒���(����)
	float fLengthH;		//2�_�Ԃ̒���(����)
	float fLengthEnemy;

	for (nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++)
	{
		if (g_Rock[nCntRock].bUse == true && g_Rock[nCntRock].nType != ROCKTYPE_THROW && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
		{
			//�~�̓����蔻��(��)
			fLengthS = sqrtf((pPos->x - g_Rock[nCntRock].pos.x) * (pPos->x - g_Rock[nCntRock].pos.x)
				+ (pPos->z - g_Rock[nCntRock].pos.z) * (pPos->z - g_Rock[nCntRock].pos.z));

			//�~�̓����蔻��(�c)
			fLengthH = sqrtf((pPos->y - g_Rock[nCntRock].pos.y) * (pPos->y - g_Rock[nCntRock].pos.y)
				+ fLengthS * fLengthS);

			//�G�̔��a������
			fLengthEnemy = (RADIUS_ENEMY) * (RADIUS_ENEMY);

			//������p����g��
			float posCol;
			if (g_Rock[nCntRock].nType == ROCKTYPE_DROP)
			{
				posCol = 350.0f;
			}
			else
			{
				posCol = 0.0f;
			}

			if (fLengthH * fLengthH < fLengthEnemy && pPos->y < g_Rock[nCntRock].pos.y + g_Rock[nCntRock].vtxMax.y && pPos->y > g_Rock[nCntRock].pos.y + g_Rock[nCntRock].vtxMin.y - posCol)
			{
				//�����������
				DisableRock(&g_Rock[nCntRock]);

				*pPoint = g_Rock[nCntRock].pos;

				//��ꂽ��̃J�E���g
				g_RockCounter++;

				if (g_Rock[nCntRock].nType == ROCKTYPE_DROP)
				{
					//�G�l�~�[�̃q�b�g����
					HitEnemy(HIT_DAMAGE * 20, nCntRock);

					//�X�R�A�̉��Z
					AddScore(ROCK_SCORE * 20);

					//�擾�X�R�A�\���ݒ�
					SetPointScore(ROCK_SCORE * 20, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITROCK);
				}
				else
				{
					//�G�l�~�[�̃q�b�g����
					HitEnemy(HIT_DAMAGE, nCntRock);

					//�X�R�A�̉��Z
					AddScore(ROCK_SCORE);

					//�擾�X�R�A�\���ݒ�
					SetPointScore(ROCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITROCK);
				}
			}
		}
	}
}

//========================================
//��ƃI�u�W�F�N�g�̓����蔻��(������)
//========================================
void CollisionRock3(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	Object *pObject = ObjectGet();				//�I�u�W�F�N�g�̎擾
	Player *pPlayer = GetPlayer();
	int nCntRock;								//�J�E���^�[

	for (nCntRock = 0; nCntRock < MAX_OBJECT; nCntRock++, pObject++)
	{
		if (pObject->bUse == true && pObject->nType != OBJECTTYPE_REFLECTOR)
		{//�u���b�N���g���Ă���
			if (pPosOld->x + vtxMax.x <= pObject->pos.x + pObject->vtxMin.x
				&& pPos->x + vtxMax.x > pObject->pos.x + pObject->vtxMin.x
				&& pPos->z + vtxMax.z > pObject->pos.z + pObject->vtxMin.z
				&& pPos->z + vtxMin.z < pObject->pos.z + pObject->vtxMax.z)
			{//�E�̔���
				pPos->x = pObject->pos.x + pObject->vtxMin.x - vtxMax.x;
			}
			if (pPosOld->x - vtxMax.z >= pObject->pos.x + pObject->vtxMax.x
				&& pPos->x - vtxMax.z < pObject->pos.x + pObject->vtxMax.x
				&& pPos->z - vtxMax.z < pObject->pos.z + pObject->vtxMax.z
				&& pPos->z + vtxMax.z > pObject->pos.z + pObject->vtxMin.z)
			{//���̔���
				pPos->x = pObject->pos.x + pObject->vtxMax.x + vtxMax.z + 0.05f;
			}
			if (pPosOld->z + vtxMax.z <= pObject->pos.z + pObject->vtxMin.z
				&& pPos->z + vtxMax.z > pObject->pos.z + pObject->vtxMin.z
				&& pPos->x + vtxMax.z > pObject->pos.x + pObject->vtxMin.x
				&& pPos->x - vtxMax.z < pObject->pos.x + pObject->vtxMax.x)
			{//�O�̔���
				pPos->z = pObject->pos.z + pObject->vtxMin.z - vtxMax.z;
			}
			if (pPosOld->z - vtxMax.z >= pObject->pos.z + pObject->vtxMax.z
				&& pPos->z - vtxMax.z < pObject->pos.z + pObject->vtxMax.z
				&& pPos->x - vtxMax.z < pObject->pos.x + pObject->vtxMax.x
				&& pPos->x + vtxMax.z > pObject->pos.x + pObject->vtxMin.x)
			{//���̔���
				pPos->z = pObject->pos.z + pObject->vtxMax.z + vtxMax.z + 0.05f;
			}
		}
	}
}

//========================================
//��ƕǂ̓����蔻��
//========================================
bool CollisionRockWall(int nCntRock)
{
	bool bWall;

	bWall = false;

	//�ǂ̏������炤
	Wall *pWall = GetWall();

	D3DXVECTOR3 Wallpos0;		//����pos
	D3DXVECTOR3 Wallpos1;		//�E��pos
	D3DXVECTOR3 vecToPos;		//pos0��pos1�̍����i�ʒu�֌W�j
	D3DXVECTOR3 vecLine;		//�ǂ̒���
	D3DXVECTOR3 vecMove;		//�e�̃x�N�g��
	float fRate;		//��_�̊���

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pWall++)
	{
		if (pWall->bUse == true)
		{
			switch (nCntWall)
			{
			case 0:
				Wallpos0 = D3DXVECTOR3(-1500.0f, 0.0f, 1500.0f);
				Wallpos1 = D3DXVECTOR3(1500.0f, 0.0f, 1500.0f);
				break;

			case 1:
				Wallpos0 = D3DXVECTOR3(1500.0f, 0.0f, -1500.0f);
				Wallpos1 = D3DXVECTOR3(-1500.0f, 0.0f, -1500.0f);
				break;

			case 2:
				Wallpos0 = D3DXVECTOR3(-1500.0f, 0.0f, -1500.0f);
				Wallpos1 = D3DXVECTOR3(-1500.0f, 0.0f, 1500.0f);
				break;

			case 3:
				Wallpos0 = D3DXVECTOR3(1500.0f, 0.0f, 1500.0f);
				Wallpos1 = D3DXVECTOR3(1500.0f, 0.0f, -1500.0f);
				break;
			}

			//Wallpos0 = pWall->aVtxpos[0];
			//Wallpos1 = pWall->aVtxpos[1];

			vecLine = Wallpos1 - Wallpos0;
			vecToPos = g_Rock[nCntRock].pos - Wallpos0;
			vecMove = g_Rock[nCntRock].pos - g_Rock[nCntRock].posOld;

			fRate = (((vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z))
				/ ((vecLine.z * vecMove.x) - (vecLine.x * vecMove.z)));

			if ((vecLine.z * vecToPos.x)
				- (vecLine.x * vecToPos.z) <= 0.0f
				&& fRate >= 0.0f && fRate <= 1.0f)
			{
				D3DXVECTOR3 posCross = vecLine * fRate;	//��_�Ɉʒu�����炷

				bWall = true;

				if (g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE)
				{
					DisableRock(&g_Rock[nCntRock]);
				}
				break;
			}
		}
	}

	return bWall;
}

//========================================
//��ƕǂ̓����蔻��
//========================================
bool CollisionRockRock(int nCntThrow)
{
	bool bRock;

	bRock = false;

	for (int nCntRock = 0; nCntRock < MAX_WALL; nCntRock++)
	{
		if (g_Rock[nCntRock].bUse == true && g_Rock[nCntRock].nType != ROCKTYPE_INVISIBLE && nCntRock != nCntThrow)
		{
			if (SphereCollision(g_Rock[nCntRock].pos, g_Rock[nCntThrow].pos, g_Rock[nCntRock].vtxMax.x * 2.0f, g_Rock[nCntThrow].vtxMax.x * 2.0f) == true)
			{
				DisableRock(&g_Rock[nCntRock]);
				bRock = true;
			}
		}
	}

	return bRock;
}

//========================================
//��̃q�b�g����
//========================================
void HitRock(int nDamage,int nCntRock)
{
	//�̗͂����炷
	g_Rock[nCntRock].nLife -= nDamage;

	//�v���C���[�̗̑͂��Ȃ��Ȃ���
	if (g_Rock[nCntRock].nLife == 0)
	{
		g_Rock[nCntRock].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���
	}
	else
	{//�̗͂��c���Ă���
		g_Rock[nCntRock].state = ROCKSTATE_DAMEGE;
		g_Rock[nCntRock].nCounterState = 5;
	}
}

//========================================
//��̎擾
//========================================
Rock *GetRock(void)
{
	return &g_Rock[0];
}

//========================================
//�₪�����������̎擾
//========================================
int *GetHitRockNum(void)
{
	return &g_RockCounter;
}

//==================================================
//��̃f�o�b�O�\��
//==================================================
void DebugRock(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//�ꏊ
	char aStr[1][256];					//��
	int nNumRock = 0;
	LPD3DXFONT Font = GetFont();		//���C���̂��

	for (int nCnt = 0; nCnt < MAX_ROCK; nCnt++)
	{
		if (g_Rock[nCnt].bUse == true)
		{
			nNumRock++;
		}
	}

	//������ɑ��
	sprintf(&aStr[0][0], "��̐��F%d", nNumRock);

	rect.top += DEBUG_TOP;
	Font->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DXCOLOR(DEBUG_CLO_R, DEBUG_CLO_G, DEBUG_CLO_B, DEBUG_CLO_A));
}

//========================================
//�����Đ����鏈��
//========================================
void PlaySoundRockCrash(Rock *pRock)
{
	//���擾
	Player *pPlayer = GetPlayer();

	//�����擾
	D3DXVECTOR3 posDiff = pRock->pos - pPlayer->pos;
	float fDiffLength;

	fDiffLength = D3DXVec3Length(&posDiff);

	if (SphereCollision(pRock->pos, pPlayer->pos, ROLLSOUND_RANGE, 0) == true && pRock->nType != ROCKTYPE_INVISIBLE)
	{//�v���C���[�����̕�������͈͓���������

		//�����̗ʂɂ���ĉ��ʂ�ς���
		SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);
		
		PlaySound(SOUND_LABEL_SE_CRASH);
	}
}