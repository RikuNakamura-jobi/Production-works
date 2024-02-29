//==========================================
//
//  �J�E���g�_�E������(countdown.cpp)
//  Author : Kai Takada
//
//==========================================
#include "countdown.h"
#include "game.h"
#include "input.h"

//==========================================
//  �}�N����`
//==========================================
#define CD_TEX_MAX (2) //�e�N�X�`����
#define COUNTDOWN_CNT_MAX (3)	//�ő�J�E���g
#define COUNTDOWN_POS (D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f,0.0f)) //���b�Z�[�W�̒��S���W
#define COUNTDOWN_ALPHA_SCALE (0.05f) //�A���t�@�l�̌�����
#define SIZE_LOOP (0.03f) //�|���S���̊g�嗦
#define COUNTDOWN_POLYGON_WIDTH (300.0f) //�|���S���̕�
#define COUNTDOWN_POLYGON_WIDTH1 (150.0f) //�|���S���̕�1
#define COUNTDOWN_POLYGON_HEIGHT (150.0f) //�|���S���̍���
#define SIZE_RATIO (COUNTDOWN_POLYGON_WIDTH / COUNTDOWN_POLYGON_HEIGHT) //�����ƕ��̔䗦
#define FREEZE_TIME (30) //�ꎞ��~�t���[��

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	D3DXCOLOR col; //�|���S���J���[
	float fWidth; //�|���S���̕�
	float fHeight; //�|���S���̍���
	float fScale; //�|���S���̊g�嗦
	int nNumCnt; //�����ڂ�
}COUNTDOWN;

//==========================================
//  �e�N�X�`���p�X�z��錾
//==========================================
const char *c_apFilenameCD[CD_TEX_MAX] =
{
	"data/TEXTURE/countdown_number_000.png",
	"data/TEXTURE/countdown_go_000.png",
};

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureCountdown[CD_TEX_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCountdown = NULL;
COUNTDOWN g_countdown;
int g_nFreezeFrame;
bool g_bSkip;

//==========================================
//  ����������
//==========================================
void InitCountdown()
{
	//�ϐ��̏�����
	g_countdown.pos = COUNTDOWN_POS;
	g_countdown.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_countdown.fWidth = COUNTDOWN_POLYGON_WIDTH1;
	g_countdown.fHeight = COUNTDOWN_POLYGON_HEIGHT;
	g_countdown.fScale = 0.0f;
	g_countdown.nNumCnt = COUNTDOWN_CNT_MAX;
	
	g_nFreezeFrame = FREEZE_TIME;
	g_bSkip = false;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCountdown,
		NULL
	);

	for (int nCntTex = 0; nCntTex < CD_TEX_MAX; nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameCD[nCntTex],
			&g_pTextureCountdown[nCntTex]
		);
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

	
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//rhw�̐ݒ�
		pVtx[nCnt].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = g_countdown.col;
	}

	//���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffCountdown->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitCountdown()
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCountdown != NULL)
	{
		g_pVtxBuffCountdown->Release();
		g_pVtxBuffCountdown = NULL;
	}

	for (int nCntTex = 0; nCntTex < CD_TEX_MAX; nCntTex++)
	{
		if (g_pTextureCountdown[nCntTex] != NULL)
		{
			g_pTextureCountdown[nCntTex]->Release();
			g_pTextureCountdown[nCntTex] = NULL;
		}
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateCountdown()
{
	if ((GetKeyboardTrigger(DIK_RETURN) ||
		GetKeyboardTrigger(DIK_SPACE) ||
		GetJoyPadButtonTrigger(KEY_A, 0) ||
		GetJoyPadButtonTrigger(KEY_B, 0))
		&& g_bSkip == false)
	{
		g_countdown.nNumCnt = 0;
		g_countdown.fScale = 0.0f;
		g_countdown.col.a = 1.0f;
		g_nFreezeFrame = FREEZE_TIME;
	}

	if (g_nFreezeFrame >= FREEZE_TIME || g_nFreezeFrame < 0)
	{//�t���[�Y���ȊO
		g_countdown.fScale += SIZE_LOOP;
	}

	if (g_countdown.fScale >= 1.0f)
	{//�K��l�ȏ�
		g_nFreezeFrame--;

		if (g_nFreezeFrame <= 0)
		{//�t���[�Y���ȊO
			g_countdown.col.a -= COUNTDOWN_ALPHA_SCALE;
			g_nFreezeFrame = -1;
		}
	}

	if (g_countdown.col.a <= 0.0f)
	{//�����ɂȂ����猳�ʂ�
		g_countdown.nNumCnt--;
		g_countdown.fScale = 0.0f;
		g_countdown.col.a = 1.0f;
		g_nFreezeFrame = FREEZE_TIME;
	}

	if (g_countdown.nNumCnt <= 0)
	{
		g_bSkip = true;
	}

	//������
	switch (g_countdown.nNumCnt)
	{
	case 0:
		g_countdown.fWidth = COUNTDOWN_POLYGON_WIDTH;
		break;

	default:
		g_countdown.fWidth = COUNTDOWN_POLYGON_WIDTH1;
		break;
	}
	
	//�Q�[���X�^�[�g
	if (g_countdown.nNumCnt == -1)
	{
		SetGameState(GAMESTATE_NORMAL, 0);
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffCountdown->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̍X�V
	pVtx[0].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y - (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_countdown.pos.x - (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_countdown.pos.x + (g_countdown.fWidth * g_countdown.fScale), g_countdown.pos.y + (g_countdown.fHeight * g_countdown.fScale), 0.0f);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//���_�J���[�̍X�V
		pVtx[nCnt].col = g_countdown.col;
	}

	if (g_countdown.nNumCnt == 0)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	else
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_countdown.nNumCnt * 0.1f + 0.1f, 1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffCountdown->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawCountdown()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCountdown, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	switch (g_countdown.nNumCnt)
	{
	case 0:
		pDevice->SetTexture(0, g_pTextureCountdown[1]);
		break;
	default:
		pDevice->SetTexture(0, g_pTextureCountdown[0]);
		break;
	}

	//���S�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//Z�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
