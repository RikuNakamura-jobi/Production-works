//===========================
//
//�_�C���N�gX.bullet�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "sound.h"
#include "life.h"
#include "effect.h"
#include "game.h"

//�}�N����`
#define MAX_BULLET (256)											//�e�̍ő吔

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;												//�ʒu
	D3DXVECTOR3 move;												//�ړ���
	D3DXVECTOR3 rot;												//����
	int nLife;														//����
	BULLETTYPE type;												//�e�̃^�C�v
	D3DXCOLOR col;													//�F
	bool bUse;														//�g�p���Ă��邩�ǂ���
}Bullet;

//�v���g�^�C�v�錾
void CollisionEnemy(Bullet *pBullet);								//�G�Ƃ̓����蔻��
void CollisionPlayer(Bullet *pBullet);								//�v���C���[�Ƃ̓����蔻��

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					//���_�����i�[
Bullet g_aBullet[MAX_BULLET];										//�e�̏��
float g_fLengthBullet;												//�Ίp���̒���
float g_fAngleBullet;												//�Ίp���̊p�x
float g_fLengthHalfBullet;											//���a�̒���
int g_shotEnemy[MAX_ENEMY];											//�ˌ����Ă���G

//===========================
//�e�̏���������
//===========================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet002.png",
		&g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].bUse = false;									//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//�Ίp���̒������Z�o����
	g_fLengthBullet = sqrtf((16.0f * 16.0f) + (16.0f * 16.0f)) * 0.5f;

	//�Ίp���̊p�x���Z�o����
	g_fAngleBullet = atan2f(16.0f, 16.0f);

	g_fLengthHalfBullet = 8.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
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
	g_pVtxBuffBullet->Unlock();
}

//===========================
//�e�̏I������
//===========================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===========================
//�e�̍X�V����
//===========================
void UpdateBullet(void)
{
	int nCntBullet;
	VERTEX_2D *pVtx;
	D3DXCOLOR col = (255, 255, 255, 255);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{//�G�̒ʏ�e
				CollisionPlayer(&g_aBullet[nCntBullet]);
				if (g_shotEnemy[nCntBullet] == 0 || g_shotEnemy[nCntBullet] == 4)
				{
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.6f), 12.0f, 6);
				}
				else if (g_shotEnemy[nCntBullet] == 1)
				{
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.6f), 12.0f, 6);
				}
				else if (g_shotEnemy[nCntBullet] == 2)
				{
					SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
				}
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{//�v���C���[�̒ʏ�e
				CollisionEnemy(&g_aBullet[nCntBullet]);
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_HOMING)
			{//�v���C���[�̃z�[�~���O�e
				float fRotMove, fRotDest, fRotDiff;
				Enemy *pEnemy = GetEnemy();					//�G�̎擾

				for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++, pEnemy++)
				{
					if (pEnemy->bUse != false)
					{
						fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
						fRotDest = atan2f(pEnemy->pos.x - g_aBullet[nCntBullet].pos.x, pEnemy->pos.y - g_aBullet[nCntBullet].pos.y);
						fRotDiff = fRotDest - fRotMove;

						if (fRotDiff > D3DX_PI)
						{
							fRotDiff -= (D3DX_PI * 2);
						}
						else if (fRotDiff < -D3DX_PI)
						{
							fRotDiff += (D3DX_PI * 2);
						}

						if (g_aBullet[nCntBullet].nLife > 80)
						{
							fRotMove += fRotDiff * 0.0f;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 0.3f);
							g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 0.3f);
						}
						else if (g_aBullet[nCntBullet].nLife == 80)
						{
							fRotMove += fRotDiff * 1.0f;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							g_aBullet[nCntBullet].move.x = sinf(fRotMove);
							g_aBullet[nCntBullet].move.y = cosf(fRotMove);
						}
						else if (g_aBullet[nCntBullet].nLife < 80)
						{
							fRotMove += fRotDiff * 0.0f;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 2.0f);
							g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 2.0f);
						}

						CollisionEnemy(&g_aBullet[nCntBullet]);
						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f), 12.0f, 20);
					}
				}
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY_HOMING)
			{//�G�̃z�[�~���O�e
				float fRotMove, fRotDest, fRotDiff;
				Player *pPlayer = GetPlayer();

				for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
				{
					fRotMove = atan2f(g_aBullet[nCntBullet].move.x, g_aBullet[nCntBullet].move.y);
					fRotDest = atan2f(pPlayer->pos.x - g_aBullet[nCntBullet].pos.x, pPlayer->pos.y - g_aBullet[nCntBullet].pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= (D3DX_PI * 2);
					}
					else if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += (D3DX_PI * 2);
					}

					if (g_shotEnemy[nCntBullet] == 2)
					{
						if (g_aBullet[nCntBullet].nLife >= 50)
						{
							fRotMove += fRotDiff * 0.0003f;
						}
						else
						{
							fRotMove += fRotDiff * 0.0f;
						}
						
						if (fRotMove > D3DX_PI)
						{
							fRotMove -= (D3DX_PI * 2);
						}
						else if (fRotMove < -D3DX_PI)
						{
							fRotMove += (D3DX_PI * 2);
						}

						g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 0.9f);
						g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 0.9f);

						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
					}
					else if (g_shotEnemy[nCntBullet] == 6)
					{
						if (g_aBullet[nCntBullet].nLife == 110)
						{
							fRotMove += fRotDiff * 1.0f;
						}
						else if (g_aBullet[nCntBullet].nLife == 60)
						{
							fRotMove += fRotDiff * 1.0f;
						}
						else if (g_aBullet[nCntBullet].nLife == 40)
						{
							fRotMove += fRotDiff * 1.0f;
						}
						else
						{
							fRotMove += fRotDiff * 0.0f;
						}

						if (fRotMove > D3DX_PI)
						{
							fRotMove -= (D3DX_PI * 2);
						}
						else if (fRotMove < -D3DX_PI)
						{
							fRotMove += (D3DX_PI * 2);
						}

						g_aBullet[nCntBullet].move.x = sinf(fRotMove) * (BULLET_SPEED * 0.9f);
						g_aBullet[nCntBullet].move.y = cosf(fRotMove) * (BULLET_SPEED * 0.9f);
						g_aBullet[nCntBullet].rot.z = fRotMove;

						SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.6f), 12.0f, 6);
					}

					CollisionPlayer(&g_aBullet[nCntBullet]);
				}
			}

			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBullet[nCntBullet].col;
			pVtx[1].col = g_aBullet[nCntBullet].col;
			pVtx[2].col = g_aBullet[nCntBullet].col;
			pVtx[3].col = g_aBullet[nCntBullet].col;

			if ((pVtx[0].pos.y >= PLAYER_HEIGHT_D || pVtx[1].pos.x <= PLAYER_WIDTH_L || pVtx[2].pos.x >= PLAYER_WIDTH_R || pVtx[3].pos.y <= PLAYER_HEIGHT_U) && g_aBullet[nCntBullet].type != BULLETTYPE_HOMING && g_aBullet[nCntBullet].type != BULLETTYPE_ENEMY_HOMING)
			{
				g_aBullet[nCntBullet].bUse = false;					//�g�p���ĂȂ���Ԃɂ���
			}

			g_aBullet[nCntBullet].nLife -= 1;

			//�����ɂ��e�̏�������
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

				if (g_shotEnemy[nCntBullet] == 2 || g_shotEnemy[nCntBullet] == 3 || g_shotEnemy[nCntBullet] == 6 || g_shotEnemy[nCntBullet] == 7 || g_shotEnemy[nCntBullet] == 11)
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f), 4);
				}
				else
				{
					SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 4);
				}

				g_aBullet[nCntBullet].bUse = false;					//�g�p���ĂȂ���Ԃɂ���
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();
}

//===========================
//�e�̕`�揈��
//===========================
void DrawBullet(void)
{
	int nCntBullet;

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffBullet,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g���Ă���
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBullet, 2);
		}
	}
}

//===========================
//�e�̐ݒ菈��
//===========================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, D3DXCOLOR col, int nEnemyType)
{
	int nCntBullet;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			g_aBullet[nCntBullet].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI + g_fAngleBullet) * g_fLengthBullet;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI - g_fAngleBullet) * g_fLengthBullet;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_fAngleBullet) * g_fLengthBullet;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_fAngleBullet) * g_fLengthBullet;
			pVtx[3].pos.z = 0.0f;

			g_shotEnemy[nCntBullet] = nEnemyType;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].bUse = true;						//�g�p���Ă����Ԃɂ���

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();
}

//===========================
//�G�Ƃ̓����蔻��
//===========================
void CollisionEnemy(Bullet *pBullet)
{
	Enemy *pEnemy = GetEnemy();					//�G�̎擾
	int nCntEnemy;
	float fLength1, fLength2, fLengthEnemy1, fLengthEnemy2;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			//�����Z�o
			if (pEnemy->nType >= 0 && pEnemy->nType <= 2)
			{
				fLengthEnemy1 = pEnemy->pos.y + cosf(pEnemy->rot.z) * pEnemy->fLength * 0.457f;
				fLengthEnemy2 = pEnemy->pos.y + cosf(pEnemy->rot.z + D3DX_PI) * pEnemy->fLength * 0.457f;
				fLength1 = sqrtf(((pBullet->pos.x - pEnemy->pos.x) * (pBullet->pos.x - pEnemy->pos.x)) + ((pBullet->pos.y - fLengthEnemy1) * (pBullet->pos.y - fLengthEnemy1))) * 0.5f;
				fLength2 = sqrtf(((pBullet->pos.x - pEnemy->pos.x) * (pBullet->pos.x - pEnemy->pos.x)) + ((pBullet->pos.y - fLengthEnemy2) * (pBullet->pos.y - fLengthEnemy2))) * 0.5f;
			}
			else if (pEnemy->nType >= 3 && pEnemy->nType <= 11)
			{
				fLength1 = sqrtf(((pBullet->pos.x - pEnemy->pos.x) * (pBullet->pos.x - pEnemy->pos.x)) + ((pBullet->pos.y - pEnemy->pos.y) * (pBullet->pos.y - pEnemy->pos.y))) * 0.5f;
			}

			//�����蔻��
			if (pEnemy->nType == 3)
			{
				if (fLength1 <= g_fLengthHalfBullet + 40.96f)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType == 7)
			{
				if (fLength1 <= g_fLengthHalfBullet + 40.0f)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType == 11)
			{
				if (fLength1 <= g_fLengthHalfBullet + 48.0f)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType == 12)
			{
				if (pEnemy->pos.x + 128 >= pBullet->pos.x && pEnemy->pos.x - 128 <= pBullet->pos.x && pEnemy->pos.y - 128 <= pBullet->pos.y && pEnemy->pos.y + 128 >= pBullet->pos.y)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType >= 0 && pEnemy->nType <= 2)
			{
				if (fLength1 <= g_fLengthHalfBullet + 8.0f || fLength2 <= g_fLengthHalfBullet + 8.0f)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType >= 4 && pEnemy->nType <= 6)
			{
				if (fLength1 <= g_fLengthHalfBullet + 16.0f)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
			else if (pEnemy->nType >= 8 && pEnemy->nType <= 10)
			{
				if (fLength1 <= g_fLengthHalfBullet + 16.0f)
				{
					//�G�̃q�b�g����
					HitEnemy(nCntEnemy, 1);

					//�T�E���h�̍Đ�
					PlaySound(SOUND_LABEL_SE_HIT);

					pBullet->bUse = false;
				}
			}
		}
	}
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
void CollisionPlayer(Bullet *pBullet)
{
	Player *pPlayer = GetPlayer();					//�G�̎擾
	bool bHardMode = GetHardMode();
	float fLength;

	//�����Z�o
	fLength = sqrtf(((pBullet->pos.x - pPlayer->pos.x) * (pBullet->pos.x - pPlayer->pos.x)) + ((pBullet->pos.y - pPlayer->pos.y) * (pBullet->pos.y - pPlayer->pos.y))) * 0.5f;

	//�����蔻��
	if (pPlayer->state == PLAYERSTATE_NORMAL)
	{
		if (fLength <= g_fLengthHalfBullet + 15.0f)
		{
			if (bHardMode == true)
			{
				//�G�̃q�b�g����
				HitPlayer(5);
				AddLife(5);
			}
			else
			{
				//�G�̃q�b�g����
				HitPlayer(1);
				AddLife(1);
			}

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_HIT);

			pBullet->bUse = false;
		}
	}
}