//==========================================
//
//  ���U���g���b�Z�[�W(result_message.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "result_message.h"
#include "result.h"
#include "particle_2D.h"
#include "sound.h"

//==========================================
//  �}�N����`
//==========================================
#define MESSAGE_POS_LEFT (D3DXVECTOR3(415.0f, 260.0f, 0.0f)) //���S���W
#define MESSAGE_POS_RIGHT (D3DXVECTOR3(865.0f, 260.0f, 0.0f)) //���S���W
#define MESSAGE_WIDTH (225.0f) //����
#define MESSAGE_HEIGHT (180.0f) //����
#define POLYGON_MOVE (30.2f) //�|���S���̈ړ���
#define POLYGON_COLOR (0.03f) //�|���S���̐F

//���ʂ̃t�@�C������ێ�
const char *c_apFilenameResultMessege[RESULTTYPE_MAX] =
{
	"data\\TEXTURE\\clear000.png",
	"data\\TEXTURE\\failed000.png"
};

//==========================================
//  ���U���g�|���S���\����
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	D3DXCOLOR col; //�F
}RESULT;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureResultMessage[RESULTTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultMessage = NULL;
RESULT g_result[2];
bool g_bSet;

//==========================================
//  ����������
//==========================================
void InitResultMessage()
{
	//�ϐ��̏�����
	switch (GetResult())
	{
	case RESULTTYPE_WIN:
		//���S���W�̐ݒ�
		g_result[0].pos = D3DXVECTOR3(-MESSAGE_WIDTH, MESSAGE_POS_LEFT.y, 0.0f);
		g_result[1].pos = D3DXVECTOR3(SCREEN_WIDTH + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y, 0.0f);
		//�F�̐ݒ�
		g_result[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_result[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case RESULTTYPE_LOSE:
		g_result[0].pos = MESSAGE_POS_LEFT;
		g_result[1].pos = MESSAGE_POS_RIGHT;
		//�F�̐ݒ�
		g_result[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_result[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		break;
	default:
		break;
	}

	g_bSet = false;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultMessage,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0;nCntTex < RESULTTYPE_MAX;nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameResultMessege[nCntTex],
			&g_pTextureResultMessage[nCntTex]
		);
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhw�̐ݒ�
			pVtx[nCnt].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_result[nCntPolygon].col;
		}

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResultMessage->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitResultMessage()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultMessage != NULL)
	{
		g_pVtxBuffResultMessage->Release();
		g_pVtxBuffResultMessage = NULL;
	}

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < RESULTTYPE_MAX; nCntTex++)
	{
		if (g_pTextureResultMessage[nCntTex] != NULL)
		{
			g_pTextureResultMessage[nCntTex]->Release();
			g_pTextureResultMessage[nCntTex] = NULL;
		}
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateResultMessage()
{
	//����p���W�̕ۑ�
	float fOldPos[2] = { g_result[0].pos.x , g_result[1].pos.x };

	//���W��␳����
	if (g_result[0].pos.x < MESSAGE_POS_LEFT.x)
	{
		g_result[0].pos.x += POLYGON_MOVE;
	}
	if (g_result[0].pos.x >= MESSAGE_POS_LEFT.x)
	{
		g_result[0].pos.x = MESSAGE_POS_LEFT.x;
	}
	if (g_result[1].pos.x > MESSAGE_POS_RIGHT.x)
	{
		g_result[1].pos.x -= POLYGON_MOVE;
	}
	if (g_result[1].pos.x <= MESSAGE_POS_RIGHT.x)
	{
		g_result[1].pos.x = MESSAGE_POS_RIGHT.x;
	}

	//�p�[�e�B�N���𔭐�����
	if (fOldPos[0] < MESSAGE_POS_LEFT.x && fOldPos[1] > MESSAGE_POS_RIGHT.x)
	{
		if (g_result[0].pos.x >= MESSAGE_POS_LEFT.x || g_result[1].pos.x <= MESSAGE_POS_RIGHT.x)
		{
			//SE�Đ�
			PlaySound(SOUND_LABEL_SE_RESULT);

			//�p�[�e�B�N������
			SetParticle2D(D3DXVECTOR3(MESSAGE_POS_LEFT.x + MESSAGE_WIDTH, MESSAGE_POS_LEFT.y, 0.0f), PARTICLETYPE_2D_RESULT);
		}
	}

	//�F��␳����
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (g_result[nCnt].col.a <= 1.0f)
		{
			g_result[nCnt].col.a += POLYGON_COLOR;
		}
	}

	if (g_result[0].pos.x == MESSAGE_POS_LEFT.x && g_result[1].pos.x == MESSAGE_POS_RIGHT.x  && g_result[0].col.a >= 1.0f && g_result[1].col.a >= 1.0f)
	{
		g_bSet = true;
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < 2; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y - MESSAGE_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x - MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_result[nCntPolygon].pos.x + MESSAGE_WIDTH, g_result[nCntPolygon].pos.y + MESSAGE_HEIGHT, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_result[nCntPolygon].col;
		}

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResultMessage->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawResultMessage()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultMessage, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultMessage[GetResult()]);

	//���S�̕`��
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//==========================================
//  ���b�Z�[�W�̏�Ԃ̎擾
//==========================================
bool GetResultState()
{
	return g_bSet;
}