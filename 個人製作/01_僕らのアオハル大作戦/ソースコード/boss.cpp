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

//�}�N����`
#define MAX_BOSS (248)			//�{�X�U���͈͂̍ő吔

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
	bool bCollision;			//�g�p���Ă��邩�ǂ���
}BOSS;

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
		"data\\TEXTURE\\JOBICciaoBlack.png",
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
		g_aBoss[nCntBoss].bCollision = true;							//�g�p���Ă��Ȃ���Ԃɂ���

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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

			if (g_aBoss[nCntBoss].nLife <= 0)
			{
				g_aBoss[nCntBoss].bUse = false;					//�g�p���ĂȂ���Ԃɂ���
			}
		}
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
void SetBoss(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 rot, float fRadiusX, float fRadiusY, int nLife)
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

			g_aBoss[nCntBoss].bCollision = true;
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
bool CollisionBoss(D3DXVECTOR3 *pPos)
{
	bool bHit = false;
	float fcos[4];
	D3DXVECTOR3 BossSize[4];
	D3DXVECTOR3 Vec;
	D3DXVECTOR3 VecPB;
	D3DXVECTOR3 ObjPos;

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		BossSize[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	Vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VecPB = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ObjPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true && g_aBoss[nCntBoss].bCollision == true)
		{
			//���_���W�̐ݒ�
			BossSize[0].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[0].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - D3DX_PI + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[0].z = 0.0f;

			BossSize[1].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[1].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + D3DX_PI - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[1].z = 0.0f;

			BossSize[2].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[2].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z - g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[2].z = 0.0f;

			BossSize[3].x = g_aBoss[nCntBoss].pos.x + sinf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[3].y = g_aBoss[nCntBoss].pos.y + cosf(g_aBoss[nCntBoss].rot.z + g_aBoss[nCntBoss].fAngle) * g_aBoss[nCntBoss].fLength * 1.6f;
			BossSize[3].z = 0.0f;

			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				switch (nCnt1)
				{
				case 0:
					Vec.x = BossSize[2].x - BossSize[0].x;
					Vec.y = BossSize[2].y - BossSize[0].y;

					VecPB.x = pPos->x - BossSize[0].x;
					VecPB.y = pPos->y - BossSize[0].y;
					break;
				case 1:
					Vec.x = BossSize[3].x - BossSize[2].x;
					Vec.y = BossSize[3].y - BossSize[2].y;

					VecPB.x = pPos->x - BossSize[2].x;
					VecPB.y = pPos->y - BossSize[2].y;
					break;
				case 2:
					Vec.x = BossSize[1].x - BossSize[3].x;
					Vec.y = BossSize[1].y - BossSize[3].y;

					VecPB.x = pPos->x - BossSize[3].x;
					VecPB.y = pPos->y - BossSize[3].y;
					break;
				case 3:
					Vec.x = BossSize[0].x - BossSize[1].x;
					Vec.y = BossSize[0].y - BossSize[1].y;

					VecPB.x = pPos->x - BossSize[1].x;
					VecPB.y = pPos->y - BossSize[1].y;
					break;
				}

				fcos[nCnt1] = ((Vec.x * VecPB.x) + (Vec.y * VecPB.y)) / (sqrtf((Vec.x * Vec.x) + (Vec.y * Vec.y)) * sqrtf((VecPB.x * VecPB.x) + (VecPB.y * VecPB.y)));
			}

			if (fcos[0] > 0.0f &&
				fcos[1] > 0.0f &&
				fcos[2] > 0.0f &&
				fcos[3] > 0.0f)
			{
				bHit = true;

				g_aBoss[nCntBoss].bCollision = false;
			}
		}
	}

	return bHit;
}