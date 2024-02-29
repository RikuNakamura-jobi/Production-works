//==========================================================
//
//��Q������ [enemy.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "enemy.h"
#include <stdio.h>
#include "input.h"
#include "debugproc.h"
#include "collision.h"
#include "score.h"
#include "effect.h"
#include "polygon.h"
#include "player.h"
#include "particle.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_ENEMY	(256)		//�G�l�~�[�̍ő吔
#define ENEMY_DELUP	(SCREEN_HEIGHT * -0.3f)		//�G�l�~�[�̍폜��Y���W
#define ENEMY_DELDOWN	(SCREEN_HEIGHT * 1.3f)	//�G�l�~�[�̍폜��Y���W
#define ENEMY_ROTATESPD	(0.01f)
#define ENEMY_METSIZE	(60.0f)					//覐΂̃T�C�Y
#define FST_LVUP	(5)							//1���x���A�b�v�K��l
#define SEC_LVUP	(10)						//2���x���A�b�v�K��l
#define THD_LVUP	(15)						//3���x���A�b�v�K��l
#define FOR_LVUP	(20)						//4���x���A�b�v�K��l
#define FST_SETTIMER	(60 * 6)				//1���x���A�b�v�K��l
#define SEC_SETTIMER	(60 * 5)				//2���x���A�b�v�K��l
#define THD_SETTIMER	(60 * 4)				//3���x���A�b�v�K��l
#define FOR_SETTIMER	(60 * 2)				//4���x���A�b�v�K��l
#define LAST_TIMER		(60)					//���X�g���x��
#define BIED_WIDTH		(50.0f)					//����
#define BIED_HEIGHT		(30.0f)					//������
#define BIED_MOVESPD	(1.7f)					//���ړ���
#define JET_MOVESPD		(3.0f)					//��s�@�ړ���
#define JET_WIDTH		(90.0f)					//��s�@��
#define JET_HEIGHT		(70.0f)					//��s�@����
#define CLOUD_MOVESPD	(1.4f)					//���_�ړ���
#define CLOUD_WIDTH		(70.0f)					//���_��
#define CLOUD_HEIGHT	(70.0f)					//���_����
#define METEOR_MOVESPD	(1.4f)					//覐Έړ���
#define HIT_AREA	(0.9f)						//�����蔻��̔{��
#define NEAR_AREA	(1.6f)						//�j�A�~�X�̔{��
#define NEAR_COUNT	(300)						//�j�A�~�X�̃J�E���g

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void SetpVtxEnemy(Enemy *pEnemy, int nCntEnemy);
void EnemyGenerator(void);
void SetBird(void);
void SetJet(void);
void SetCloud(void);
void SetMeteor(void);

void InitNearMiss(void);
void UninitNearMiss(void);
void UpdateNearMiss(void);
void DrawNearMiss(void);

//==========================================================
//�O���[�o���ϐ�
//==========================================================
Enemy g_aEnemy[MAX_ENEMY] = {};	//�G�l�~�[�̔z��
LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMYTYPE_MAX] = {};	//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;			//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureNearMiss = NULL;		//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNearMiss = NULL;			//���_�o�b�t�@�ւ̃|�C���^
NearMiss g_aNearmiss[MAX_ENEMY];
int g_nEnemySetCnter;			//�G�l�~�[�̐ݒu�J�E���^�[

//==========================================================
//�e�N�X�`���t�@�C����
//==========================================================
const char* c_apFilenameEnemy[ENEMYTYPE_MAX] =
{

	"data\\TEXTURE\\enemy000.png",		//��
	"data\\TEXTURE\\enemy001.png",		//��s�@
	"data\\TEXTURE\\enemy002.png",		//���_
	"data\\TEXTURE\\enemy003.png",		//覐�
};

//==========================================================
//�G�̏���������
//==========================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	ZeroMemory(g_aEnemy, sizeof(g_aEnemy));	//������������

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEnemy[nCntTex],
			&g_apTextureEnemy[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	InitNearMiss();

	//AddScore(40);
}

//==========================================================
//�G�̏I������
//==========================================================
void UninitEnemy(void)
{
	//�e�N�X�`���̏I������
	for (int nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}

	UninitNearMiss();
}

//==========================================================
//�G�̍X�V����
//==========================================================
void UpdateEnemy(void)
{
	Enemy *pEnemy = GetEnemy();	//�G�l�~�[�̃|�C���^
	int a = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�g�p����Ă���ꍇ
			a++;
			pEnemy->pos += pEnemy->move;	//�ړ��ʑ���

			g_aEnemy[nCntEnemy].nNear--;

			if (g_aEnemy[nCntEnemy].nNear < 0)
			{
				g_aEnemy[nCntEnemy].nNear = 0;
			}

			if (pEnemy->nType != ENEMYTYPE_METEOR)
			{
				if (pEnemy->nType != ENEMYTYPE_CLOUD)
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.6f, 1.0f, 0.6f, 1.0f), 1, 5, g_aEnemy[nCntEnemy].fWidth);
				}
				else
				{
					SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 1, 5, g_aEnemy[nCntEnemy].fWidth);
				}
			}
			else
			{
				SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(0.7f, 0.4f, 0.4f, 1.0f), 2, 5, g_aEnemy[nCntEnemy].fWidth);
			}

			if (pEnemy->pos.y > ENEMY_DELDOWN ||
				
				pEnemy->pos.y < ENEMY_DELUP ||
				pEnemy->pos.x > SCREEN_WIDTH * 1.1f ||
				pEnemy->pos.x < -SCREEN_WIDTH * 0.1f)
			{//�폜���W�ɂ���ꍇ
				pEnemy->bUse = false;	//�g�p����Ă��Ȃ���Ԃɂ���
			}
			else
			{
				switch (pEnemy->nType)
				{
				case ENEMYTYPE_METEOR:

					//覐Ή�]
					if (pEnemy->nRot == 0)
					{
						pEnemy->rot.z += ENEMY_ROTATESPD;

						if (pEnemy->rot.z >= D3DX_PI)
						{
							pEnemy->rot.z += -D3DX_PI * 2;
						}
					}
					else
					{
						pEnemy->rot.z -= ENEMY_ROTATESPD;

						if (pEnemy->rot.z <= -D3DX_PI)
						{
							pEnemy->rot.z += D3DX_PI * 2;
						}
					}
					break;
				}

				//���_���
				SetpVtxEnemy(pEnemy, nCntEnemy);
			}
		}
	}

	//�G�l�~�[�o���ݒ�
	EnemyGenerator();

	PrintDebugProc("�G�̐�%d\n", a);

	UpdateNearMiss();
}

//==========================================================
//�G�̕`�揈��
//==========================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	Enemy *pEnemy = GetEnemy();		//�G�l�~�[�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{//�g�p����Ă���ꍇ

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntEnemy * 4,									//�v���~�e�B�u�i�|���S���̐��j
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	DrawNearMiss();
}

//==========================================================
//�G�̏����擾
//==========================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==========================================================
//�G�̐ݒ�
//==========================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fWidth, float fHeight, int nType)
{
	Enemy *pEnemy = GetEnemy();		//�G�l�~�[�̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{//�g�p����Ă���ꍇ
			pEnemy->pos = pos;
			pEnemy->rot = rot;
			pEnemy->move = move;
			pEnemy->nType = nType;
			pEnemy->nTexCounter = 0;
			pEnemy->nTexAnim = 0;
			pEnemy->fWidth = fWidth;
			pEnemy->fHeight = fHeight;
			pEnemy->nRot = rand() % 2;

			//�Ίp���̒������Z�o����
			pEnemy->fLength = sqrtf(pEnemy->fWidth * pEnemy->fWidth + pEnemy->fHeight * pEnemy->fHeight) * 0.5f;

			//�Ίp���̊p�x���Z�o����
			pEnemy->fAngle = atan2f(pEnemy->fWidth, pEnemy->fHeight);

			//���_���ݒ�
			SetpVtxEnemy(pEnemy, nCntEnemy);

			pEnemy->bUse = true;
			break;
		}
	}
}

//==========================================================
//���_���ݒ�
//==========================================================
void SetpVtxEnemy(Enemy *pEnemy, int nCntEnemy)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	float fTexU = 1.0f;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nCntEnemy * 4;

	//���_���W�̐ݒ�
	pVtx[0].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + (-D3DX_PI + pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[0].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + (-D3DX_PI + pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[1].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + (D3DX_PI - pEnemy->fAngle)) * pEnemy->fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + -pEnemy->fAngle) * pEnemy->fLength;
	pVtx[2].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + -pEnemy->fAngle) * pEnemy->fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = pEnemy->pos.x + sinf(pEnemy->rot.z + pEnemy->fAngle) * pEnemy->fLength;
	pVtx[3].pos.y = pEnemy->pos.y + cosf(pEnemy->rot.z + pEnemy->fAngle) * pEnemy->fLength;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	switch (pEnemy->nType)
	{
	case ENEMYTYPE_BIED:
		if (pEnemy->move.x > 0.0f)
		{
			pEnemy->nTexAnim = 1;
		}
		else
		{
			pEnemy->nTexAnim = 0;
		}

		fTexU = 0.5f;	//�T�C�Y�ݒ�
		break;
	case ENEMYTYPE_JET:
		if (pEnemy->move.x > 0.0f)
		{
			pEnemy->nTexAnim = 1;
		}
		else
		{
			pEnemy->nTexAnim = 0;
		}

		fTexU = 0.5f;	//�T�C�Y�ݒ�
		break;
	default:
		fTexU = 1.0f;	//�T�C�Y�ݒ�
		break;
	}

	pVtx[0].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim + fTexU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexU * pEnemy->nTexAnim + fTexU, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//==========================================================
//�G�l�~�[�̏o���ݒ�
//==========================================================
void EnemyGenerator(void)
{
	int nType = ENEMYTYPE_MAX;	//�o��������

	g_nEnemySetCnter += rand() % 10;	//�����_���ŃJ�E���g�𑝉�

	if (GetNowScore() < FST_LVUP)
	{//�������x��
		if (g_nEnemySetCnter >= FST_SETTIMER)
		{//�o���^�C�}�[�𒴂����ꍇ
			nType = rand() % ENEMYTYPE_JET;	//���o��
			g_nEnemySetCnter = 0;	//�J�E���^�[���Z�b�g
		}
	}
	else if (GetNowScore() >= FST_LVUP && 
		GetNowScore() < SEC_LVUP)
	{//1���x��
		if (g_nEnemySetCnter >= FST_SETTIMER)
		{//�o���^�C�}�[�𒴂����ꍇ
			nType = rand() % ENEMYTYPE_CLOUD;	//��s�@�܂ŏo��
			g_nEnemySetCnter = 0;	//�J�E���^�[���Z�b�g
		}
	}
	else if (GetNowScore() >= SEC_LVUP &&
		GetNowScore() < THD_LVUP)
	{//2���x��
		if (g_nEnemySetCnter >= SEC_SETTIMER)
		{//�o���^�C�}�[�𒴂����ꍇ
			nType = rand() % ENEMYTYPE_METEOR;	//���_�܂ŏo��
			g_nEnemySetCnter = 0;	//�J�E���^�[���Z�b�g
		}
	}
	else if (GetNowScore() >= THD_LVUP &&
		GetNowScore() < FOR_LVUP)
	{//3���x��
		if (g_nEnemySetCnter >= THD_SETTIMER)
		{//�o���^�C�}�[�𒴂����ꍇ
			nType = rand() % ENEMYTYPE_MAX;	//覐΂܂ŏo��
			g_nEnemySetCnter = 0;	//�J�E���^�[���Z�b�g
		}
	}
	else if (GetNowScore() >= FOR_LVUP)
	{//4���x���ȍ~
		if (g_nEnemySetCnter >= FOR_SETTIMER)
		{//�o���^�C�}�[�𒴂����ꍇ
			nType = rand() % ENEMYTYPE_MAX;	//覐΂܂ŏo��
			g_nEnemySetCnter = 0;	//�J�E���^�[���Z�b�g
		}
	}
	else
	{
		if (g_nEnemySetCnter >= LAST_TIMER)
		{//�o���^�C�}�[�𒴂����ꍇ
			nType = rand() % ENEMYTYPE_MAX;	//覐΂܂ŏo��
			g_nEnemySetCnter = 0;	//�J�E���^�[���Z�b�g
		}
	}

	//�K��l�ł���ΐݒu����
	if (nType != -1)
	{//��ސݒ�ς�
		switch (nType)
		{
		case ENEMYTYPE_BIED:
			SetBird();
			break;
		case ENEMYTYPE_JET:
			SetJet();
			break;
		case ENEMYTYPE_CLOUD:
			SetCloud();
			break;
		case ENEMYTYPE_METEOR:
			SetMeteor();
			break;
		default:
			break;
		}
	}
}

//==========================================================
//���̏o���ݒ�
//==========================================================
void SetBird(void)
{
	int nRand = 0;	//�����_���p

	Enemy enemy;	//�G�l�~�[�̏��p
	ZeroMemory(&enemy, sizeof(enemy));	//������������

	//���E�ǂ��炩��ݒ肷��
	nRand = rand() % 2;

	if (nRand == 0)
	{
		enemy.pos.x = SCREEN_WIDTH * 1.1f;
		enemy.move.x = -BIED_MOVESPD;
	}
	else
	{
		enemy.pos.x = SCREEN_WIDTH * -0.1f;
		enemy.move.x = BIED_MOVESPD;
	}

	int nY = (int)((float)SCREEN_HEIGHT + (float)SCREEN_HEIGHT * 0.1f);
	enemy.pos.y = (float)(rand() % nY) + SCREEN_HEIGHT * 0.1f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, BIED_WIDTH, BIED_HEIGHT, ENEMYTYPE_BIED);
}

//==========================================================
//��s�@�̏o���ݒ�
//==========================================================
void SetJet(void)
{
	int nRand = 0;	//�����_���p

	Enemy enemy;	//�G�l�~�[�̏��p
	ZeroMemory(&enemy, sizeof(enemy));	//������������

	//���E�ǂ��炩��ݒ肷��
	nRand = rand() % 2;

	if (nRand == 0)
	{
		enemy.pos.x = SCREEN_WIDTH * 1.1f;
		enemy.move.x = -JET_MOVESPD;
	}
	else
	{
		enemy.pos.x = SCREEN_WIDTH * -0.1f;
		enemy.move.x = JET_MOVESPD;
	}

	int nY = (int)((float)SCREEN_HEIGHT + (float)SCREEN_HEIGHT * 0.1f);
	enemy.pos.y = (float)(rand() % nY) + SCREEN_HEIGHT * 0.1f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, JET_WIDTH, JET_HEIGHT, ENEMYTYPE_JET);
}

//==========================================================
//�_�̏o���ݒ�
//==========================================================
void SetCloud(void)
{
	int nRand = 0;	//�����_���p

	Enemy enemy;	//�G�l�~�[�̏��p
	ZeroMemory(&enemy, sizeof(enemy));	//������������

	//���E�ǂ��炩��ݒ肷��
	nRand = rand() % 2;

	if (nRand == 0)
	{
		enemy.pos.x = SCREEN_WIDTH * 1.1f;
		enemy.move.x = -CLOUD_MOVESPD;
	}
	else
	{
		enemy.pos.x = SCREEN_WIDTH * -0.1f;
		enemy.move.x = CLOUD_MOVESPD;
	}

	int nY = (int)((float)SCREEN_HEIGHT + (float)SCREEN_HEIGHT * 0.1f);
	enemy.pos.y = (float)(rand() % nY) + SCREEN_HEIGHT * 0.1f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, CLOUD_WIDTH, CLOUD_HEIGHT, ENEMYTYPE_CLOUD);
}

//==========================================================
//覐΂̏o���ݒ�
//==========================================================
void SetMeteor(void)
{
	Enemy enemy;
	ZeroMemory(&enemy, sizeof(enemy));	//������������

	enemy.pos.x = (float)(rand() % SCREEN_WIDTH);
	enemy.pos.y = ENEMY_DELUP;
	enemy.move.y = METEOR_MOVESPD;
	enemy.rot.z = 0.0f;

	SetEnemy(enemy.pos, enemy.rot, enemy.move, ENEMY_METSIZE, ENEMY_METSIZE, ENEMYTYPE_METEOR);
}

//==========================================================
//�G�l�~�[�̏o���ݒ�
//==========================================================
bool CollisionEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posOld)
{
	bool bEnemy;
	bEnemy = false;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			switch (g_aEnemy[nCntEnemy].nType)
			{
			case ENEMYTYPE_BIED:
				if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * HIT_AREA, g_aEnemy[nCntEnemy].fHeight * HIT_AREA) == true)
				{//�Փ˂����Ƃ�
					bEnemy = true;
				}
				else if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA) == true &&
					CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//�j�A�~�X�̎�
					AddScore(1);		//�X�R�A���Z
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			case ENEMYTYPE_JET:
				if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * HIT_AREA * 0.8f, g_aEnemy[nCntEnemy].fHeight * HIT_AREA * 0.9f) == true)
				{//�Փ˂����Ƃ�
					bEnemy = true;
				}
				else if (CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA * 0.8f, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA * 0.9f) == true &&
					CollisionEllipse2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].rot, g_aEnemy[nCntEnemy].fWidth * NEAR_AREA * 0.8f, g_aEnemy[nCntEnemy].fHeight * NEAR_AREA * 0.9f) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//�j�A�~�X�̎�
					AddScore(1);		//�X�R�A���Z
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			case ENEMYTYPE_CLOUD:
				if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight * HIT_AREA * 0.9f) == true)
				{//�Փ˂����Ƃ�
					bEnemy = true;
				}
				else if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA * 0.9f) == true &&
					CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA * 0.9f) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//�j�A�~�X�̎�
					AddScore(1);		//�X�R�A���Z
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			case ENEMYTYPE_METEOR:
				if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight * HIT_AREA) == true)
				{//�Փ˂����Ƃ�
					bEnemy = true;
				}
				else if (CollisionCircle2D(g_aEnemy[nCntEnemy].pos, posOld, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA) == true &&
					CollisionCircle2D(g_aEnemy[nCntEnemy].pos, pos, g_aEnemy[nCntEnemy].fHeight* NEAR_AREA) == false &&
					g_aEnemy[nCntEnemy].nNear == 0)
				{//�j�A�~�X�̎�
					AddScore(1);		//�X�R�A���Z
					g_aEnemy[nCntEnemy].nNear = NEAR_COUNT;
				}
				break;
			}
		}
	}

	return bEnemy;
}

//==========================================================
//�j�A�~�X�|���S���̏�����
//==========================================================
void InitNearMiss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	ZeroMemory(g_aNearmiss, sizeof(g_aNearmiss));	//������������

													//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\score000.png",
			&g_pTextureNearMiss);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNearMiss,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNearMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		g_aNearmiss[nCntNear].bUse = false;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNearMiss->Unlock();
}

//==========================================================
//�j�A�~�X�|���S���̏I������
//==========================================================
void UninitNearMiss(void)
{
	if (g_pTextureNearMiss != NULL)
	{
		g_pTextureNearMiss->Release();
		g_pTextureNearMiss = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffNearMiss != NULL)
	{
		g_pVtxBuffNearMiss->Release();
		g_pVtxBuffNearMiss = NULL;
	}
}

//==========================================================
//�j�A�~�X�|���S���̍X�V����
//==========================================================
void UpdateNearMiss(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNearMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		if (g_aNearmiss[nCntNear].bUse == true)
		{//�g�p����Ă���ꍇ
			g_aNearmiss[nCntNear].Polygon.pos += g_aNearmiss[nCntNear].move;

			g_aNearmiss[nCntNear].nLife--;

			if (g_aNearmiss[nCntNear].nLife <= 0)
			{
				g_aNearmiss[nCntNear].nLife = 0;
				g_aNearmiss[nCntNear].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNearMiss->Unlock();
}

//==========================================================
//�j�A�~�X�|���S���̕`�揈��
//==========================================================
void DrawNearMiss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffNearMiss, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		if (g_aNearmiss[nCntNear].bUse == true)
		{//�g�p����Ă���ꍇ

		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureNearMiss);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntNear * 4,									//�v���~�e�B�u�i�|���S���̐��j
				2);												//�`�悷��v���~�e�B�u��
		}
	}
}

//==========================================================
//�j�A�~�X�|���S���̐ݒ菈��
//==========================================================
void SetNearMiss(void)
{
	Player *player = GetPlayer();
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNearMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNear = 0; nCntNear < MAX_ENEMY; nCntNear++)
	{
		if (g_aNearmiss[nCntNear].bUse == false)
		{//�g�p����Ă���ꍇ
			g_aNearmiss[nCntNear].Polygon.pos.x = player->pos.x + 30.0f;
			g_aNearmiss[nCntNear].Polygon.pos.y = player->pos.y - 30.0f;
			g_aNearmiss[nCntNear].Polygon.fWidth = 30.0f;
			g_aNearmiss[nCntNear].Polygon.fHeight = 20.0f;
			g_aNearmiss[nCntNear].move.y = -0.5f;
			g_aNearmiss[nCntNear].bUse = true;
			g_aNearmiss[nCntNear].nLife = 40;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y - g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x - g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aNearmiss[nCntNear].Polygon.pos.x + g_aNearmiss[nCntNear].Polygon.fWidth, g_aNearmiss[nCntNear].Polygon.pos.y + g_aNearmiss[nCntNear].Polygon.fHeight, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw =
				pVtx[1].rhw =
				pVtx[2].rhw =
				pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNearMiss->Unlock();
}