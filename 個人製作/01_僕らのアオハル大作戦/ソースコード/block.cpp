//===========================
//
//�_�C���N�gX.block�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "block.h"
#include "player.h"

//�}�N����`
#define MAX_BLOCK (256)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBlock[BLOCKTYPE_MAX] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffBlock = NULL;			//���_�����i�[
Block g_aBlock[MAX_BLOCK];								//�G�̏��
float g_fMoveCount;

//�e�N�X�`���t�@�C����
const char *c_apFilenameBlock[BLOCKTYPE_MAX] =
{
	"data\\TEXTURE\\block000.png",
	"data\\TEXTURE\\block001.png",
	"data\\TEXTURE\\block000.png",
	"data\\TEXTURE\\block004.png",
	"data\\TEXTURE\\block005.png"
};

//===========================
//�G�̏���������
//===========================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < BLOCKTYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameBlock[nCnt],
			&g_apTextureBlock[nCnt]);
	}

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].posInit = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBlock[nCntBlock].blocktype = BLOCKTYPE_0;
		g_aBlock[nCntBlock].fHeight = 80.0f;
		g_aBlock[nCntBlock].fWidth = 80.0f;
		g_aBlock[nCntBlock].nBlockBreak = 120;
		g_aBlock[nCntBlock].bUse = false;					//�g�p���Ă��Ȃ���Ԃɂ���
		g_aBlock[nCntBlock].bPlayer = false;
	}

	g_fMoveCount = 0.0f;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
		pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
		pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
		pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aBlock[nCntBlock].col;
		pVtx[1].col = g_aBlock[nCntBlock].col;
		pVtx[2].col = g_aBlock[nCntBlock].col;
		pVtx[3].col = g_aBlock[nCntBlock].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.01f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.01f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBlock->Unlock();
}

//===========================
//�G�̏I������
//===========================
void UninitBlock(void)
{
	for (int nCnt = 0; nCnt < BLOCKTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBlock[nCnt] != NULL)
		{
			g_apTextureBlock[nCnt]->Release();
			g_apTextureBlock[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffBlock != NULL)
	{
		g_VtxBuffBlock->Release();
		g_VtxBuffBlock = NULL;
	}
}

//===========================
//�G�̍X�V����
//===========================
void UpdateBlock(void)
{
	int nTimerBlock = (int)(g_fMoveCount * 100);
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	g_fMoveCount += 0.01f;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].nBlockBreak += 1;
			g_aBlock[nCntBlock].posOld = g_aBlock[nCntBlock].pos;

			if (g_aBlock[nCntBlock].nBlockBreak >= 20 && g_aBlock[nCntBlock].nBlockBreak < 120)
			{
				g_aBlock[nCntBlock].move.y += 0.5f;
			}
			else if (g_aBlock[nCntBlock].nBlockBreak == 120)
			{
				g_aBlock[nCntBlock].move.y = 0.0f;
				g_aBlock[nCntBlock].pos = g_aBlock[nCntBlock].posInit;
			}

			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;
			pVtx[3].pos.z = 0.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBlock[nCntBlock].col;
			pVtx[1].col = g_aBlock[nCntBlock].col;
			pVtx[2].col = g_aBlock[nCntBlock].col;
			pVtx[3].col = g_aBlock[nCntBlock].col;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffBlock->Unlock();
}

//===========================
//�G�̕`�揈��
//===========================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBlock[g_aBlock[nCntBlock].blocktype]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlock * 4, 2);
		}
	}
}

//===========================
//�G�̐ݒ菈��
//===========================
void SetBlock(D3DXVECTOR3 pos, int nType)
{
	int nCntBlock;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].posOld = pos;
			g_aBlock[nCntBlock].posInit = pos;
			g_aBlock[nCntBlock].nType = nType;
			g_aBlock[nCntBlock].bUse = true;

			if (nType == 1)
			{
				g_aBlock[nCntBlock].blocktype = BLOCKTYPE_1;
			}
			if (nType == 2)
			{
				g_aBlock[nCntBlock].blocktype = BLOCKTYPE_2;
			}
			else if (nType == 3)
			{
				g_aBlock[nCntBlock].blocktype = BLOCKTYPE_3;
			}
			else if (nType == 4)
			{
				g_aBlock[nCntBlock].blocktype = BLOCKTYPE_4;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[0].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
			pVtx[1].pos.y = g_aBlock[nCntBlock].pos.y;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBlock[nCntBlock].pos.x;
			pVtx[2].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth;
			pVtx[3].pos.y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight;
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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.01f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.01f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}

		pVtx += 4;
	}

	g_VtxBuffBlock->Unlock();
}

//===========================
//�v���C���[�Ƃ̓����蔻��
//===========================
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, Block **pBlock)
{
	bool bLand = false;
	Player *pPlayer = GetPlayer();

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if ((pPos->x + 24.0f > g_aBlock[nCntBlock].pos.x && pPos->x - 24.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth) && (pPos->y + 32.0f > g_aBlock[nCntBlock].pos.y && pPosOld->y + 32.0f <= g_aBlock[nCntBlock].pos.y) && pPos->y + 32.0f < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight)
			{
				if (g_aBlock[nCntBlock].nType != 4)
				{
					bLand = true;
					pPos->y = g_aBlock[nCntBlock].pos.y - 32.0f;
					pMove->y = 0.0f;

					if (g_aBlock[nCntBlock].nType == 1)
					{
						HitPlayer(1000, -5.0f);
					}

					if (pBlock != NULL)
					{
						*pBlock = &g_aBlock[nCntBlock];
					}
				}

				if (g_aBlock[nCntBlock].nType == 2 && g_aBlock[nCntBlock].nBlockBreak >= 120 && pPlayer->state != PLAYERSTATE_WAIT)
				{
					g_aBlock[nCntBlock].nBlockBreak = 0;
				}

				g_aBlock[nCntBlock].bPlayer = true;
			}
			else if ((pPos->x + 24.0f > g_aBlock[nCntBlock].pos.x && pPos->x - 24.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth) && (pPos->y - 32.0f < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight && pPosOld->y - 32.0f >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight) && pPos->y - 32.0f > g_aBlock[nCntBlock].pos.y)
			{
				if (g_aBlock[nCntBlock].nType != 3)
				{
					pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight + 32.0f;
					pMove->y = 0.0f;
				}

				g_aBlock[nCntBlock].bPlayer = true;
			}
			else
			{
				g_aBlock[nCntBlock].bPlayer = false;
			}
		}
	}

	return bLand;
}

//===========================
//�v���C���[�Ƃ̓����蔻�艡
//===========================
bool CollisionBlockSide(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, Block **pBlock)
{
	bool bWall = false;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			if ((pPos->y + 30.0f > g_aBlock[nCntBlock].pos.y && pPos->y - 30.0f < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight) && (pPos->x + 25.0f > g_aBlock[nCntBlock].pos.x && pPosOld->x + 25.0f <= g_aBlock[nCntBlock].pos.x) && pPos->x + 25.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth)
			{
				if (g_aBlock[nCntBlock].nType != 3 && g_aBlock[nCntBlock].nType != 4)
				{
					bWall = true;
					pPos->x = g_aBlock[nCntBlock].pos.x - 25.0f;
					pMove->x = 0.0f;

					if (pBlock != NULL)
					{
						*pBlock = &g_aBlock[nCntBlock];
					}

					if (pMove->y < 0.0f)
					{
						pMove->y += 1.0f;
					}
				}

				g_aBlock[nCntBlock].bPlayer = true;
			}
			else if ((pPos->y + 30.0f > g_aBlock[nCntBlock].pos.y && pPos->y - 30.0f < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight) && (pPos->x - 25.0f < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth && pPosOld->x - 25.0f >= g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth) && pPos->x - 25.0f > g_aBlock[nCntBlock].pos.x)
			{
				if (g_aBlock[nCntBlock].nType != 3 && g_aBlock[nCntBlock].nType != 4)
				{
					bWall = true;
					pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fHeight + 25.0f;
					pMove->x = 0.0f;

					if (pBlock != NULL)
					{
						*pBlock = &g_aBlock[nCntBlock];
					}

					if (pMove->y < 0.0f)
					{
						pMove->y += 1.0f;
					}
				}

				g_aBlock[nCntBlock].bPlayer = true;
			}
			else
			{
				g_aBlock[nCntBlock].bPlayer = false;
			}
		}
	}

	return bWall;
}