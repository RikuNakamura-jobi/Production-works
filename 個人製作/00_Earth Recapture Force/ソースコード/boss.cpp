//===========================
//
//�_�C���N�gX.boss�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "boss.h"
#include "player.h"
#include "enemy.h"
#include "sound.h"
#include "life.h"

//�}�N����`
#define MAX_BOSS (248)											//�{�X�U���͈͂̍ő吔

//�{�X�U���͈͍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXCOLOR col;				//�F
	D3DXVECTOR3 rot;			//����
	float fRadiusX;				//X����
	float fRadiusY;				//Y����
	float fLength;				//�Ίp���̒���
	float fAngle;				//�Ίp���̊p�x
	int nLife;					//����
	int nType;					//���
	bool bUse;					//�g�p���Ă��邩�ǂ���
}BOSS;

//�v���g�^�C�v�錾
void CollisionPlayerBoss(int nCntBoss);				//�v���C���[�Ƃ̓����蔻��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBoss = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoss = NULL;		//���_�����i�[
BOSS g_aBoss[MAX_BOSS];								//�{�X�U���͈͂̏��
int g_nCntBossSound1;								//���ʉ��̑��d����1
int g_nCntBossSound2;								//���ʉ��̑��d����2
int g_nCntBossSound3;								//���ʉ��̑��d����3

//===========================
//�{�X�U���͈͂̏���������
//===========================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBoss;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\boss001.png",
		&g_pTextureBoss);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].fRadiusX = 10.0f;
		g_aBoss[nCntBoss].fRadiusY = 10.0f;
		g_aBoss[nCntBoss].fAngle = 0.0f;
		g_aBoss[nCntBoss].fLength = 0.0f;
		g_aBoss[nCntBoss].nLife = 10;
		g_aBoss[nCntBoss].bUse = false;									//�g�p���Ă��Ȃ���Ԃɂ���

		//�Ίp���̒������Z�o����
		g_aBoss[nCntBoss].fLength = sqrtf((g_aBoss[nCntBoss].fRadiusX * g_aBoss[nCntBoss].fRadiusX) + (g_aBoss[nCntBoss].fRadiusY * g_aBoss[nCntBoss].fRadiusY)) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		g_aBoss[nCntBoss].fAngle = atan2f(g_aBoss[nCntBoss].fRadiusX, g_aBoss[nCntBoss].fRadiusY);
	}

	g_nCntBossSound1 = 0;
	g_nCntBossSound2= 0;
	g_nCntBossSound3 = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSS,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoss,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[0].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[1].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[2].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[3].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;											//���_���W�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBoss->Unlock();
}

//===========================
//�{�X�U���͈͂̏I������
//===========================
void UninitBoss(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBoss != NULL)
	{
		g_pTextureBoss->Release();
		g_pTextureBoss = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//===========================
//�{�X�U���͈͂̍X�V����
//===========================
void UpdateBoss(void)
{
	int nCntBoss;
	VERTEX_2D *pVtx;
	Enemy *aEnemy = GetEnemy();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)
		{
			g_aBoss[nCntBoss].nLife -= 1;

			//�U������
			if (g_aBoss[nCntBoss].nType == 0)
			{
				if (g_aBoss[nCntBoss].nLife == 20)
				{
					if (g_nCntBossSound1 == 0)
					{
						//�T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_KATANA001);

						g_nCntBossSound1++;
					}

					CollisionPlayerBoss(nCntBoss);
					g_aBoss[nCntBoss].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
				else if (g_aBoss[nCntBoss].nLife < 20)
				{
					CollisionPlayerBoss(nCntBoss);
					g_aBoss[nCntBoss].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			else if (g_aBoss[nCntBoss].nType == 1)
			{
				if (g_aBoss[nCntBoss].nLife < 20)
				{
					g_aBoss[nCntBoss].fRadiusX -= 1.5f;
				}
				else if (g_aBoss[nCntBoss].nLife < 40)
				{
					CollisionPlayerBoss(nCntBoss);
					g_aBoss[nCntBoss].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				}
				else if (g_aBoss[nCntBoss].nLife == 40)
				{
					if (g_nCntBossSound2 == 0)
					{
						//�T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_BEAM001);

						g_nCntBossSound2++;
					}

					CollisionPlayerBoss(nCntBoss);
					g_aBoss[nCntBoss].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
				}
			}
			else if (g_aBoss[nCntBoss].nType == 2)
			{
				if (g_aBoss[nCntBoss].nLife > 60)
				{
					g_aBoss[nCntBoss].fRadiusX += 1.5f;
				}
				else if (g_aBoss[nCntBoss].nLife == 60)
				{
					if (g_nCntBossSound3 == 0)
					{
						//�T�E���h�̍Đ�
						PlaySound(SOUND_LABEL_SE_DORAGON001);

						g_nCntBossSound3++;
					}

					CollisionPlayerBoss(nCntBoss);
					g_aBoss[nCntBoss].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				}
				else if (g_aBoss[nCntBoss].nLife > 20)
				{
					CollisionPlayerBoss(nCntBoss);
					g_aBoss[nCntBoss].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				}
				else if (g_aBoss[nCntBoss].nLife <= 20)
				{
					g_aBoss[nCntBoss].fRadiusX -= 6.0f;
				}
			}

			//�Ίp���̒������Z�o����
			g_aBoss[nCntBoss].fLength = sqrtf((g_aBoss[nCntBoss].fRadiusX * g_aBoss[nCntBoss].fRadiusX) + (g_aBoss[nCntBoss].fRadiusY * g_aBoss[nCntBoss].fRadiusY)) * 0.5f;

			//�Ίp���̊p�x���Z�o����
			g_aBoss[nCntBoss].fAngle = atan2f(g_aBoss[nCntBoss].fRadiusX, g_aBoss[nCntBoss].fRadiusY);

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[0].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[1].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[2].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[3].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBoss[nCntBoss].col;
			pVtx[1].col = g_aBoss[nCntBoss].col;
			pVtx[2].col = g_aBoss[nCntBoss].col;
			pVtx[3].col = g_aBoss[nCntBoss].col;

			if (g_aBoss[nCntBoss].nLife <= 0)
			{
				g_nCntBossSound1 = 0;
				g_nCntBossSound2 = 0;
				g_nCntBossSound3 = 0;
				g_aBoss[nCntBoss].bUse = false;					//�g�p���ĂȂ���Ԃɂ���
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffBoss->Unlock();
}

//===========================
//�{�X�U���͈͂̕`�揈��
//===========================
void DrawBoss(void)
{
	int nCntBoss;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffBoss,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBoss);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)
		{//�{�X�U���͈͂��g���Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBoss, 2);
		}
	}
}

//===========================
//�{�X�U���͈͂̐ݒ菈��
//===========================
void SetBoss(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, float fRadiusX, float fRadiusY, int nLife, int nType)
{
	int nCntBoss;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == false)
		{
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].rot = rot;
			g_aBoss[nCntBoss].nLife = nLife;
			g_aBoss[nCntBoss].nType = nType;
			g_aBoss[nCntBoss].fRadiusX = fRadiusX;
			g_aBoss[nCntBoss].fRadiusY = fRadiusY;
			g_aBoss[nCntBoss].col = col;

			//�Ίp���̒������Z�o����
			g_aBoss[nCntBoss].fLength = sqrtf((g_aBoss[nCntBoss].fRadiusX * g_aBoss[nCntBoss].fRadiusX) + (g_aBoss[nCntBoss].fRadiusY * g_aBoss[nCntBoss].fRadiusY)) * 0.5f;

			//�Ίp���̊p�x���Z�o����
			g_aBoss[nCntBoss].fAngle = atan2f(g_aBoss[nCntBoss].fRadiusX, g_aBoss[nCntBoss].fRadiusY);

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[0].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[1].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[2].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[3].pos.y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBoss[nCntBoss].bUse = true;						//�g�p���Ă����Ԃɂ���

			break;
		}

		pVtx += 4;
	}

	g_pVtxBuffBoss->Unlock();
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
void CollisionPlayerBoss(int nCntBoss)
{
	Player *pPlayer = GetPlayer();					//�G�̎擾
	float fLength, fData1, fData2, fData3, fData4, fData5, fData6;

	//�����Z�o
	fData1 = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
	fData2 = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
	fData3 = ((g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength));
	fData4 = ((g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength));
	fData5 = (float)sqrt((((fData2 - fData1) / (fData4 - fData3)) * ((fData2 - fData1) / (fData4 - fData3))) + 1);
	fData6 = ((((fData2 - fData1) / (fData4 - fData3)) * pPlayer->pos.x) + (pPlayer->pos.y * -1.0f) + (fData1 - (((fData2 - fData1) / (fData4 - fData3)) * fData3)));
	
	if (fData6 < 0)
	{
		fData6 = fData6 * -1.0f;
	}

	fLength = fData6 / fData5;

	//�����蔻��
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		if (fLength <= 15.0f)
		{
			//�G�̃q�b�g����
			HitPlayer(5);
			AddLife(5);
		}
	}

	//�����Z�o
	fData1 = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
	fData2 = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
	fData3 = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
	fData4 = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength;
	fData5 = (float)sqrt((((fData2 - fData1) / (fData4 - fData3)) * ((fData2 - fData1) / (fData4 - fData3))) + 1);
	fData6 = ((((fData2 - fData1) / (fData4 - fData3)) * pPlayer->pos.x) + (pPlayer->pos.y * -1.0f) + (fData1 - (((fData2 - fData1) / (fData4 - fData3)) * fData3)));

	if (fData6 < 0)
	{
		fData6 = fData6 * -1.0f;
	}

	fLength = fData6 / fData5;

	//�����蔻��
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		if (fLength <= 15.0f)
		{
			//�G�̃q�b�g����
			HitPlayer(5);
			AddLife(5);
		}
	}

	//�����蔻��
	if (g_aBoss[nCntBoss].nType == 2)
	{
		if (pPlayer->state == PLAYERSTATE_NORMAL)
		{
			if ((g_aBoss[nCntBoss].pos.x - pPlayer->pos.x <= 15.0f + (g_aBoss[nCntBoss].fRadiusX / 2) && pPlayer->pos.x <= g_aBoss[nCntBoss].pos.x) || (pPlayer->pos.x  - g_aBoss[nCntBoss].pos.x<= 15.0f + (g_aBoss[nCntBoss].fRadiusX / 2) && pPlayer->pos.x > g_aBoss[nCntBoss].pos.x))
			{
				//�G�̃q�b�g����
				HitPlayer(5);
				AddLife(5);
			}
		}
	}
}