//===========================
//
//�_�C���N�gX.enemy�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "fade.h"
#include "particle.h"
#include "effect.h"
#include "boss.h"
#include "life.h"
#include "sound.h"

//�}�N����`
#define NUM_ENEMY (6)												//�G�̎��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffEnemy = NULL;						//���_�����i�[
Enemy g_aEnemy[MAX_ENEMY];											//�G�̏��
int g_nNumEnemy;													//�G�̐�
int g_nCounterShotTime = 0;											//�G�������n�߂�܂ł̎���
int g_nTimeStop;													//�G�̃X�g�b�v�p�ϐ�
int g_nCountLREnemy;													//�G�̃X�g�b�v�p�ϐ�
float g_fEnemyMove;													//��~���̈ړ��ʊi�[�ϐ�
float g_fEnemyColorTimer;											//�G�̐F�ύX�^�C�}�[�ϐ�
float g_fEnemyTimer;											//�G�̐F�ύX�^�C�}�[�ϐ�
int g_nBossShot = 0;												//�{�X�̍U�����ʕϐ�

//===========================
//�G�̏���������
//===========================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy000.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy000.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy001.png",
		&g_apTextureEnemy[3]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy002.png",
		&g_apTextureEnemy[4]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Enemy003.png",
		&g_apTextureEnemy[5]);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fLength = 0.0f;
		g_aEnemy[nCntEnemy].fAngle = 0.0f;
		g_aEnemy[nCntEnemy].fLengthMove = 0.0f;
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nMoveType = 0;
		g_aEnemy[nCntEnemy].nMoveTime = 90;
		g_aEnemy[nCntEnemy].nLife = 1;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].pBlock = NULL;
		g_aEnemy[nCntEnemy].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���

		//�Ίp���̒������Z�o����
		g_aEnemy[nCntEnemy].fLength = sqrtf((64.0f * 64.0f) + (64.0f * 64.0f)) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		g_aEnemy[nCntEnemy].fAngle = atan2f(64.0f, 64.0f);
	}

	g_nNumEnemy = 0;
	g_nCounterShotTime = 10;
	g_nTimeStop = 0;
	g_nBossShot = 0;
	g_fEnemyColorTimer = 0.0f;
	g_fEnemyTimer = 0.0f;
	g_nCountLREnemy = 0;

	srand((unsigned int)time(0));

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffEnemy,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI +g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z -g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z +g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z +g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
		pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffEnemy->Unlock();
}

//===========================
//�G�̏I������
//===========================
void UninitEnemy(void)
{
	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < NUM_ENEMY; nCntEnemy++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffEnemy != NULL)
	{
		g_VtxBuffEnemy->Release();
		g_VtxBuffEnemy = NULL;
	}
}

//===========================
//�G�̍X�V����
//===========================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (CollisionBoss(&g_aEnemy[nCntEnemy].pos) == true && g_aEnemy[nCntEnemy].state == ENEMYSTATE_NORMAL)
			{
				HitEnemy(nCntEnemy, 1);
			}

			if (pPlayer->state != PLAYERSTATE_WAIT)
			{
				CollisionPlayerENEMY(nCntEnemy);

				if (g_aEnemy[nCntEnemy].nType == 0)
				{
					g_aEnemy[nCntEnemy].nMoveTime--;

					if (g_aEnemy[nCntEnemy].nMoveTime >= 0)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime > -90)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime == -90)
					{
						g_aEnemy[nCntEnemy].nMoveTime = 90;
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == 1)
				{
					g_aEnemy[nCntEnemy].nMoveTime--;

					if (g_aEnemy[nCntEnemy].nMoveTime >= 0)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime > -90)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime == -90)
					{
						g_aEnemy[nCntEnemy].nMoveTime = 90;
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == 2)
				{
					if (g_aEnemy[nCntEnemy].pos.x < pPlayer->pos.x)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].pos.x > pPlayer->pos.x)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].pos.x == pPlayer->pos.x)
					{
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else if (g_aEnemy[nCntEnemy].nType == 3)
				{
					if (g_aEnemy[nCntEnemy].pos.x < pPlayer->pos.x)
					{
						if (g_aEnemy[nCntEnemy].move.x < 0.0f)
						{
							g_aEnemy[nCntEnemy].move.x += 0.06f;
						}

						g_nCountLREnemy = 1;
						g_aEnemy[nCntEnemy].move.x += 0.05f;
					}
					else if (g_aEnemy[nCntEnemy].pos.x > pPlayer->pos.x)
					{
						if (g_aEnemy[nCntEnemy].move.x > 0.0f)
						{
							g_aEnemy[nCntEnemy].move.x -= 0.06f;
						}

						g_nCountLREnemy = 0;
						g_aEnemy[nCntEnemy].move.x -= 0.05f;
					}

					g_aEnemy[nCntEnemy].move.y -= 0.8f;
				}
				else if (g_aEnemy[nCntEnemy].nType == 4)
				{
					g_aEnemy[nCntEnemy].nMoveTime++;
					g_fEnemyTimer += 0.01f;

					if (g_aEnemy[nCntEnemy].nMoveTime >= 0 && g_aEnemy[nCntEnemy].nMoveTime < 200)
					{
						g_fEnemyTimer = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime >= 200 && g_aEnemy[nCntEnemy].nMoveTime < 500)
					{
						g_aEnemy[nCntEnemy].move.x += sinf(g_fEnemyTimer * -1.0f) * 0.1f;
						g_aEnemy[nCntEnemy].move.y += 0.07f;
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime >= 500 && g_aEnemy[nCntEnemy].nMoveTime < 700)
					{
						g_fEnemyTimer = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime >= 700 && g_aEnemy[nCntEnemy].nMoveTime < 1000)
					{
						g_aEnemy[nCntEnemy].move.x += sinf(g_fEnemyTimer) * 0.1f;
						g_aEnemy[nCntEnemy].move.y += 0.07f;
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime == 1000)
					{
						g_aEnemy[nCntEnemy].nMoveTime = 0;
					}

					g_aEnemy[nCntEnemy].move.y -= 0.8f;
				}
				else if (g_aEnemy[nCntEnemy].nType == 5)
				{
					g_aEnemy[nCntEnemy].nMoveTime++;
					g_fEnemyTimer += 0.01f;

					if (g_aEnemy[nCntEnemy].nMoveTime >= 0 && g_aEnemy[nCntEnemy].nMoveTime < 200)
					{
						g_fEnemyTimer = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime >= 200 && g_aEnemy[nCntEnemy].nMoveTime < 500)
					{
						g_aEnemy[nCntEnemy].move.x += sinf(g_fEnemyTimer * -1.0f) * 0.2f;
						g_aEnemy[nCntEnemy].move.y += 0.07f;
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime >= 500 && g_aEnemy[nCntEnemy].nMoveTime < 700)
					{
						g_fEnemyTimer = 0;
						g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime >= 700 && g_aEnemy[nCntEnemy].nMoveTime < 1000)
					{
						g_aEnemy[nCntEnemy].move.x += sinf(g_fEnemyTimer) * 0.2f;
						g_aEnemy[nCntEnemy].move.y += 0.07f;
					}
					else if (g_aEnemy[nCntEnemy].nMoveTime == 1000)
					{
						g_aEnemy[nCntEnemy].nMoveTime = 0;
					}

					g_aEnemy[nCntEnemy].move.y -= 0.8f;
				}

				g_aEnemy[nCntEnemy].move.y += 0.8f;

				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
			}
			

			if (CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, &g_aEnemy[nCntEnemy].pBlock) == true)
			{
			
			}

			if (CollisionBlockSide(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, &g_aEnemy[nCntEnemy].pBlock) == true)
			{

			}

			VERTEX_2D *pVtx;

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += (4 * nCntEnemy);

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[3].pos.z = 0.0f;

			if (g_aEnemy[nCntEnemy].nCounterState <= 0)
			{
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
				g_aEnemy[nCntEnemy].nCounterState = 0;

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				g_aEnemy[nCntEnemy].nCounterState--;
			}

			if (g_aEnemy[nCntEnemy].nType == 3)
			{
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((0.5f * g_nCountLREnemy), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((0.5f * g_nCountLREnemy) + 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((0.5f * g_nCountLREnemy), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((0.5f * g_nCountLREnemy) + 0.5f, 1.0f);
			}
			else
			{
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			g_VtxBuffEnemy->Unlock();
		}
	}
}

//===========================
//�G�̕`�揈��
//===========================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffEnemy,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//===========================
//�G�̐ݒ菈��
//===========================
void SetEnemy(D3DXVECTOR3 pos, int nType, int nMoveType, int nMoveTime)
{
	int nCntEnemy;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nMoveTime -= nMoveTime;

			if (nType == 3)
			{
				g_aEnemy[nCntEnemy].nLife = 4;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;

				//�Ίp���̒������Z�o����
				g_aEnemy[nCntEnemy].fLength = sqrtf((96.0f * 96.0f) + (96.0f * 96.0f)) * 0.5f;

				//�Ίp���̊p�x���Z�o����
				g_aEnemy[nCntEnemy].fAngle = atan2f(96.0f, 96.0f);
			}
			else if (nType == 4)
			{
				g_aEnemy[nCntEnemy].nLife =8;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;
				g_aEnemy[nCntEnemy].nMoveTime = 0;

				//�Ίp���̒������Z�o����
				g_aEnemy[nCntEnemy].fLength = sqrtf((128.0f * 128.0f) + (128.0f * 128.0f)) * 0.5f;

				//�Ίp���̊p�x���Z�o����
				g_aEnemy[nCntEnemy].fAngle = atan2f(128.0f, 128.0f);
			}
			else if (nType == 5)
			{
				g_aEnemy[nCntEnemy].nLife = 8;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;
				g_aEnemy[nCntEnemy].nMoveTime = 0;

				//�Ίp���̒������Z�o����
				g_aEnemy[nCntEnemy].fLength = sqrtf((128.0f * 128.0f) + (128.0f * 128.0f)) * 0.5f;

				//�Ίp���̊p�x���Z�o����
				g_aEnemy[nCntEnemy].fAngle = atan2f(128.0f, 128.0f);
			}
			else
			{
				g_aEnemy[nCntEnemy].nLife = 2;
				g_aEnemy[nCntEnemy].nMoveType = nMoveType;

				//�Ίp���̒������Z�o����
				g_aEnemy[nCntEnemy].fLength = sqrtf((64.0f * 64.0f) + (64.0f * 64.0f)) * 0.5f;

				//�Ίp���̊p�x���Z�o����
				g_aEnemy[nCntEnemy].fAngle = atan2f(64.0f, 64.0f);
			}

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - D3DX_PI + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + D3DX_PI - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z - g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x + sinf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y + cosf(g_aEnemy[nCntEnemy].rot.z + g_aEnemy[nCntEnemy].fAngle) * g_aEnemy[nCntEnemy].fLength;
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			if (g_aEnemy[nCntEnemy].nType == 3)
			{
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
			}
			else
			{
				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			g_aEnemy[nCntEnemy].bUse = true;						//�g�p���Ă����Ԃɂ���

			g_nNumEnemy++;

			break;
		}
		pVtx += 4;
	}

	g_VtxBuffEnemy->Unlock();
}

//===========================
//�G�̏��擾����
//===========================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//===========================
//�G�̃q�b�g����
//===========================
void HitEnemy(int nCntEnemy, int nDamage)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Player *pPlayer = GetPlayer();

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{
		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		SetEffect(g_aEnemy[nCntEnemy].pos, col, 64.0f, 40);

		g_nNumEnemy--;

		if (g_nNumEnemy == 0)
		{

		}

		g_aEnemy[nCntEnemy].nMoveTime = 0;

		g_aEnemy[nCntEnemy].bUse = false;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_HIT001);
	}
	else
	{
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 10;

		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_VtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (4 * nCntEnemy);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_VtxBuffEnemy->Unlock();

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_HIT001);
	}
}

//===========================
//�G�̑S�Ŕ��菈��
//===========================
int GetNumEnemy(void)
{
	return g_nNumEnemy;
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
void CollisionPlayerENEMY(int nCntEnemy)
{
	Player *pPlayer = GetPlayer();					//�G�̎擾
	float fLength1, fMove;

	fLength1 = sqrtf(((pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x) * (pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x)) + ((pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y) * (pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y))) * 0.5f;

	if (pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x >= 0.0f)
	{
		fMove = 10.0f;
	}
	else
	{
		fMove = -10.0f;
	}

	if (pPlayer->state != PLAYERSTATE_DAMAGE && pPlayer->state != PLAYERSTATE_DEATH && pPlayer->state != PLAYERSTATE_GOAL)
	{
		if (g_aEnemy[nCntEnemy].nType == 3)
		{
			if (fLength1 <= 36.0f)
			{
				//�G�̃q�b�g����
				HitPlayer(100, fMove);
				
				AddLife(100);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 3)
		{
			if (fLength1 <= 42.0f)
			{
				//�G�̃q�b�g����
				HitPlayer(100, fMove);

				AddLife(100);
			}
		}
		else if (g_aEnemy[nCntEnemy].nType == 4 || g_aEnemy[nCntEnemy].nType == 5)
		{
			if (fLength1 <= 42.0f)
			{
				//�G�̃q�b�g����
				HitPlayer(100, fMove);

				AddLife(100);
			}
		}
		else
		{
			if (fLength1 <= 24.0f)
			{
				//�G�̃q�b�g����
				HitPlayer(100, fMove);

				AddLife(100);
			}
		}
	}
}