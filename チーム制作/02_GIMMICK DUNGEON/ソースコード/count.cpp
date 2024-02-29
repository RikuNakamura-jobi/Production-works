//==========================================
//
//  �J�E���g�̏���(count.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "count.h"
#include "polygon.h"
#include "tutorial.h"
#include "sound.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_DIGIT (1) //�ő�̌���
#define MAX_POLYGON (1 + MAX_DIGIT * 2) //�X���b�V���̃|���S�� + �ő包���̐��l�|���S�� * 2

//==========================================
//  �e�N�X�`�����x��
//==========================================
typedef enum
{
	TEX_SLASH = 0, 
	TEX_NUM, 
	TEX_MAX
}TEXTURE_COUNT_LABEL;

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //�ʒu
	float fSize; //�T�C�Y
	int nMax; //�J�E���g�̍ő�l
	int nCountCurrent; //���݂̃J�E���g
	bool bClear; //�B�����
}COUNT;

//==========================================
//  �e�N�X�`���ꗗ
//==========================================
const char *c_apTextureCount[TEX_MAX] =
{
	"data/TEXTURE/slash_000.png", //�X���b�V��
	"data/TEXTURE/number001.png" //���l
};

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureCount[TEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCount = NULL;
COUNT g_Count;

//==========================================
//  ����������
//==========================================
void InitCount()
{
	//�ϐ��̏�����
	ZeroMemory(&g_Count, sizeof(COUNT));

	//���_�o�b�t�@�̏�����
	g_pVtxBuffCount = Init_2D_Polygon(MAX_POLYGON);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apTextureCount[nCnt],
			&g_pTextureCount[nCnt]);
	}
}

//==========================================
//  �I������
//==========================================
void UninitCount()
{
	//���_�o�b�t�@�̔j��
	UninitPolygon(&g_pVtxBuffCount);

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		if (g_pTextureCount[nCnt] != NULL)
		{
			g_pTextureCount[nCnt]->Release();
			g_pTextureCount[nCnt] = NULL;
		}
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateCount()
{
	//���[�J���ϐ��錾
	int aTex[MAX_DIGIT];
	int nCntCount, nCalc;

	//�ő吔�̎擾
	nCalc = g_Count.nCountCurrent;

	//�e�N�X�`�����W�̌v��
	for (nCntCount = 0; nCntCount < MAX_DIGIT; nCntCount++)
	{
		aTex[nCntCount] = nCalc % 10;
		nCalc /= 10;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffCount->Lock(0, 0, (void**)&pVtx, 0);

	//�Œ�̒l��ݒ肩��Ȃ�
	pVtx += 4 * (MAX_DIGIT + 1);

	//�ݒ菈��
	for (nCntCount = 0; nCntCount < MAX_DIGIT; nCntCount++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Count.pos.x - ((float)(nCntCount + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y - g_Count.fSize, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Count.pos.x - ((float)nCntCount * (g_Count.fSize * 0.6f)), g_Count.pos.y - g_Count.fSize, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Count.pos.x - ((float)(nCntCount + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Count.pos.x - ((float)nCntCount * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTex[nCntCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aTex[nCntCount] * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTex[nCntCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aTex[nCntCount] * 0.1f) + 0.1f, 1.0f);

		//���_�f�[�^�̃|�C���^��4�i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffCount->Unlock();

	//�J�E���g�̒B������
	if (g_Count.nMax <= g_Count.nCountCurrent)
	{
		g_Count.bClear = true;
	}

	//�`���[�g���A����i�߂�
	TUTORIALSTATE *tutorial = GetTutorialState();
	if (*tutorial == TUTORIALSTATE_JUMP && g_Count.bClear && GetMode() == MODE_TUTORIAL)
	{
		*tutorial = TUTORIALSTATE_ENEMY;

		PlaySound(SOUND_LABEL_SE_BUTTON_004);

		//�J�E���^��ݒ�
		SetCount(D3DXVECTOR3(640.0f, 120.0, 0.0f), 100.0f, 3);
	}
	else if (*tutorial == TUTORIALSTATE_ENEMY && g_Count.bClear)
	{
		if (GetCount())
		{
			*tutorial = TUTORIALSTATE_END;

			PlaySound(SOUND_LABEL_SE_BUTTON_004);
		}
	}
}

//==========================================
//  �`�揈��
//==========================================
void DrawCount()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCount, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		if (nCnt == 0)
		{
			pDevice->SetTexture(0, g_pTextureCount[TEX_SLASH]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureCount[TEX_NUM]);
		}

		//���j���[���ڂ̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//==========================================
//  �ݒ菈��
//==========================================
void SetCount(D3DXVECTOR3 pos, float size, int max)
{
	//���[�J���ϐ��錾
	int aTex[MAX_DIGIT];
	int nCntCount, nCalc;

	//�������̕ۑ�
	g_Count.pos = pos;
	g_Count.fSize = size;
	g_Count.nMax = max;

	//���݃J�E���g�̏�����
	g_Count.nCountCurrent = 0;
	g_Count.bClear = false;

	//�ő吔�̎擾
	nCalc = g_Count.nMax;

	//�e�N�X�`�����W�̌v��
	for (nCntCount = MAX_DIGIT - 1; nCntCount >= 0; nCntCount--)
	{
		aTex[nCntCount] = nCalc % 10;
		nCalc /= 10;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffCount->Lock(0, 0, (void**)&pVtx, 0);

	//�ݒ菈��
	for (nCntCount = 0; nCntCount <= MAX_DIGIT; nCntCount++)
	{
		if (nCntCount == 0)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Count.pos.x - g_Count.fSize * 0.3f, g_Count.pos.y - g_Count.fSize, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Count.pos.x + g_Count.fSize * 0.3f, g_Count.pos.y - g_Count.fSize, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Count.pos.x - g_Count.fSize * 0.3f, g_Count.pos.y + g_Count.fSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Count.pos.x + g_Count.fSize * 0.3f, g_Count.pos.y + g_Count.fSize, 0.0f);
		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Count.pos.x + ((float)(nCntCount - 1) * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Count.pos.x + ((float)((nCntCount - 1) + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Count.pos.x + ((float)(nCntCount - 1) * (g_Count.fSize * 0.6f)), g_Count.pos.y + g_Count.fSize, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Count.pos.x + ((float)((nCntCount - 1) + 1.0f) * (g_Count.fSize * 0.6f)), g_Count.pos.y + g_Count.fSize, 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(aTex[nCntCount - 1] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((aTex[nCntCount - 1] * 0.1f) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aTex[nCntCount - 1] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((aTex[nCntCount - 1] * 0.1f) + 0.1f, 1.0f);
		}

		//���_�f�[�^�̃|�C���^��4�i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
 	g_pVtxBuffCount->Unlock();
}

//==========================================
//  ���Z����
//==========================================
void AddCount(int add)
{
	g_Count.nCountCurrent += add;

	PlaySound(SOUND_LABEL_SE_BUTTON_004);
}

//==========================================
//  �J�E���g�̏�Ԃ��擾
//==========================================
bool GetCount()
{
	return g_Count.bClear;
}
