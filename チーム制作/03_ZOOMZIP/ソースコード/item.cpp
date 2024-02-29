//==========================================================
//
//�A�C�e������ [item.cpp]
//Author:����������
//
//==========================================================
#include "item.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "particle.h"

//�}�N����`
#define NUM_ITEM	(3)			//�A�C�e���̐�
#define WIDTH_ITEM	(30.0f)		//��
#define HEIGHT_ITEM	(30.0f)		//����
#define MOVE		(0.5f)		//�ړ���
#define MOVE_JUMP	(-10.0f)	//���˂鍂��
#define APPEAR_TIME	(60 * 10)	//�o������

//�v���g�^�C�v�錾
void ControlItem(int nCntItem);
void ScreenItem(int nCntItem);
void CollisionItem(void);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureItem[NUM_ITEM] = {};			//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Item g_aItem[MAX_ITEM];									//�A�C�e���̏��
int g_nItemAppearCounter;		//�o������
int g_nItemType;			//�A�C�e���̎��
float g_fItemRandPos;			//�����_���̈ʒu

//==========================================================
//�A�C�e���̏���������
//==========================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\item001.png",
		&g_pTextureItem[ITEMSTATE_DEL]);		//�S�폜

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\item000.png",
		&g_pTextureItem[ITEMSTATE_JET]);		//�W�F�b�g�p�b�N

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\item002.png",
		&g_pTextureItem[ITEMSTATE_UP]);			//���_�A�b�v

	//�ϐ�������
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;			//�O��̈ʒu
		g_aItem[nCntItem].posKeep = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O��̈ʒu
		g_aItem[nCntItem].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aItem[nCntItem].moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O��̈ړ���
		g_aItem[nCntItem].bUse = true;			//�g�p���Ă邩
		g_aItem[nCntItem].bDisp = true;			//�`�悷�邩
		g_aItem[nCntItem].nType = -1;			//���
		g_aItem[nCntItem].nSwitchLR = 1;		//���E�؂�ւ�
		g_aItem[nCntItem].fWidth = WIDTH_ITEM;	//��
		g_aItem[nCntItem].fHeight = HEIGHT_ITEM;	//����
		g_aItem[nCntItem].fMoveRand = (float)(rand() % 5) * 0.1f;	//�ړ���
	}

	g_nItemAppearCounter = 0;		//�o�����Ԃ�������
	g_fItemRandPos = 0;				//�o���ꏊ��ݒ�
	g_nItemType = -1;				//�A�C�e���̎��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);

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
	g_pVtxBuffItem->Unlock();

}

//==========================================================
//�A�C�e���̏I������
//==========================================================
void UninitItem(void)
{
	//�e�N�X�`���̔j��
	for (int nCntItem = 0; nCntItem < NUM_ITEM; nCntItem++)
	{
		if (g_pTextureItem[nCntItem] != NULL)
		{
			g_pTextureItem[nCntItem]->Release();
			g_pTextureItem[nCntItem] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//==========================================================
//�A�C�e���̍X�V����
//==========================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	g_nItemAppearCounter++;				//�o�����Ԃ����Z

	if ((g_nItemAppearCounter % APPEAR_TIME) == 0)
	{//��莞�Ԍo��

		g_fItemRandPos = (float)(rand() % SCREEN_WIDTH + 0);		//�o���ꏊ�ݒ�
		g_nItemType = rand() % ITEMSTATE_UP;	//�A�C�e���̎�ސݒ�

		//�A�C�e���̐ݒ�
		SetItem(D3DXVECTOR3(g_fItemRandPos, SCREEN_HEIGHT, 0.0f), g_nItemType);
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse = true && g_aItem[nCntItem].bDisp == true)
		{//�g�p���Ă���

			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;		//���݂̈ʒu

			//�A�C�e���̑���
			ControlItem(nCntItem);

			//�ʒu���X�V
			g_aItem[nCntItem].pos.x += g_aItem[nCntItem].move.x;
			g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;

			//�ړ��ʂ��X�V
			g_aItem[nCntItem].move.x += (0.0f - g_aItem[nCntItem].move.x) * 0.1f;

			//�������X�V
			g_aItem[nCntItem].rot.z += g_aItem[nCntItem].move.z;

			//��ʊO����
			ScreenItem(nCntItem);

			CollisionItem();

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//==========================================================
//�A�C�e���̑���
//==========================================================
void ControlItem(int nCntItem)
{
	if (g_aItem[nCntItem].bJump == false)
	{//�W�����v���ĂȂ��Ƃ�

		//��ɏグ��
		g_aItem[nCntItem].move.y = MOVE_JUMP;

		g_aItem[nCntItem].bJump = true;		//�W�����v���Ă��Ԃɂ���
	}
	else if(g_aItem[nCntItem].bJump == true)
	{//�W�����v���ĂȂ��Ƃ�

		g_aItem[nCntItem].move.y += 0.1f;	//�d�͉��Z

	}

	if (g_aItem[nCntItem].posKeep.x <= SCREEN_WIDTH / 2)
	{//������荶�̏ꍇ

		//�E�Ɉړ�
		g_aItem[nCntItem].move.x += MOVE + g_aItem[nCntItem].fMoveRand;
	}
	else if (g_aItem[nCntItem].posKeep.x > SCREEN_WIDTH / 2)
	{//�������E�̏ꍇ

		//���Ɉړ�
		g_aItem[nCntItem].move.x -= MOVE - g_aItem[nCntItem].fMoveRand;
	}
}

//==========================================================
//��ʊO�ɏo�����̏���
//==========================================================
void ScreenItem(int nCntItem)
{
	//��ʊO�ɏo���ꍇ
	if ((g_aItem[nCntItem].pos.x - WIDTH_ITEM + 5.0f <= 0) ||
		(g_aItem[nCntItem].pos.x + WIDTH_ITEM - 5.0f >= SCREEN_WIDTH))
	{//���E�ɏo���ꍇ

		//�g�p���ĂȂ���Ԃɂ���
		g_aItem[nCntItem].bDisp = false;
		g_aItem[nCntItem].bUse = false;
	}
}

//==========================================================
//�A�C�e���̓����蔻��
//==========================================================
void CollisionItem(void)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//��Q���̏��擾
	Enemy *pEnemy = GetEnemy();

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].pos.x >= pPlayer->pos.x - WIDTH_PLAYER &&
			g_aItem[nCntItem].pos.x <= pPlayer->pos.x + WIDTH_PLAYER &&
			g_aItem[nCntItem].pos.y >= pPlayer->pos.y - HEIGHT_PLAYER &&
			g_aItem[nCntItem].pos.y <= pPlayer->pos.y + HEIGHT_PLAYER)
		{
			switch (g_aItem[nCntItem].nType)
			{
			case ITEMSTATE_DEL:		//��Q���S�폜

				//��Q���폜
				for (int nCntEnemy = 0; nCntEnemy < 256; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�g�p���Ă���
						SetParticle(pEnemy->pos, D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 0, 10, 50.0f);
						pEnemy->bUse = false;		//�g�p���ĂȂ���Ԃɂ���
					}
				}

				break;

			case ITEMSTATE_JET:		//�W�F�b�g�p�b�N

				AddScore(3);

				break;

			case ITEMSTATE_UP:		//���_�A�b�v

				AddScore(1);

				break;
			}

			//�A�C�e���폜
			g_aItem[nCntItem].bUse = false;
			g_aItem[nCntItem].bDisp = false;

			break;
		}
	}
}

//==========================================================
//�A�C�e���̕`�揈��
//==========================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true && g_aItem[nCntItem].bDisp == true)
		{
			switch (g_aItem[nCntItem].nType)
			{
			case ITEMSTATE_DEL:		//��Q���S�폜

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureItem[ITEMSTATE_DEL]);

				break;

			case ITEMSTATE_JET:		//�W�F�b�g�p�b�N

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureItem[ITEMSTATE_JET]);

				break;

			case ITEMSTATE_UP:		//���_�A�b�v

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureItem[ITEMSTATE_UP]);

				break;
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				4 * nCntItem,									//�v���~�e�B�u�i�|���S���̐��j
				2);												//�`�悷��v���~�e�B�u��
		}
	}
}

//==========================================================
//�A�C�e���̐ݒ菈��
//==========================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{//�g�p���ĂȂ��Ƃ�

			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aItem[nCntItem].posOld = g_aItem[nCntItem].pos;
			g_aItem[nCntItem].posKeep = g_aItem[nCntItem].pos;
			g_aItem[nCntItem].nType = nType;
			g_aItem[nCntItem].bUse = true;
			g_aItem[nCntItem].bDisp = true;
			g_aItem[nCntItem].bJump = false;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y - g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + g_aItem[nCntItem].fWidth, g_aItem[nCntItem].pos.y + g_aItem[nCntItem].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//==========================================================
//�A�C�e���̏��
//==========================================================
Item *GetItem(void)
{
	return &g_aItem[0];
}