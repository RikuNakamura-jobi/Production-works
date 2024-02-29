//==========================================================
//
//�t�F�[�h����
//Author ����������
//
//==========================================================
#include "fade.h"

//�}�N����`
#define NUM_TEX		(2)		//�e�N�X�`���̐�
#define WIDTH_FADE	(1280.0f * 10.0f)	//���̒���
#define HEIGHT_FADE	(720.0f * 10.0f)	//�c�̒���
#define ROTATE_LENGTH	(1500.0f)		//��]�t�F�[�h�̑Ίp���̒���

//�v���g�^�C�v�錾
void NormalFade(void);
void RotateFade(void);

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTextureFade[NUM_TEX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	//���_�o�b�t�@�ւ̃|�C���^
FADE g_fade;									//�t�F�[�h�̏��
Fade g_aFade;								//�t�F�[�h�̏��
MODE g_modeNext;								//���̉��(���[�h)
float g_fLengthFade;		//�Ίp���̒���
float g_fAngleFade;			//�Ίp���̊p�x
float g_fOldLengthFade;		//�Ίp���̒����ۑ��p

//==========================================================
//�t�F�[�h�̏���������
//==========================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_pTextureFade[FADETYPE_NORMAL]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\fade_rotate.png",
		&g_pTextureFade[FADETYPE_ROTATE]);

	g_aFade.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//�ʒu
	g_aFade.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_aFade.nType = FADETYPE_ROTATE;
	g_aFade.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����|���S���ɂ��Ă���

	g_fade = FADE_IN;	//�t�F�[�h�C����Ԃ�
	g_modeNext = modeNext;	//���̉�ʂ�ݒ�

	//�Ίp���̒������Z�o����
	g_fLengthFade = sqrtf(WIDTH_FADE * WIDTH_FADE + HEIGHT_FADE * HEIGHT_FADE) * 0.5f;
	g_fOldLengthFade = g_fLengthFade;

	//�Ίp���̊p�x���Z�o����
	g_fAngleFade = atan2f(WIDTH_FADE, HEIGHT_FADE);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_aFade.nType == FADETYPE_NORMAL)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	}
	else if (g_aFade.nType == FADETYPE_ROTATE)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.z = 0.0f;
	}

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[1].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);
	pVtx[2].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);
	pVtx[3].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);
	pVtx[0].col = D3DXCOLOR(g_aFade.col.r, g_aFade.col.g, g_aFade.col.b, g_aFade.col.a);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(g_modeNext);
}

//==========================================================
//�t�F�[�h�̏I������
//==========================================================
void UninitFade(void)
{
	for (int nCntFade = 0; nCntFade < NUM_TEX; nCntFade++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureFade[nCntFade] != NULL)
		{
			g_pTextureFade[nCntFade]->Release();
			g_pTextureFade[nCntFade] = NULL;
		}
	}

	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//==========================================================
//�t�F�[�h�̍X�V����
//==========================================================
void UpdateFade(void)
{
	if (g_aFade.nType == FADETYPE_NORMAL)
	{
		//�ʏ�t�F�[�h
		NormalFade();
	}
	else if (g_aFade.nType == FADETYPE_ROTATE)
	{
		//��]�t�F�[�h
		RotateFade();
	}

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	if (g_aFade.nType == FADETYPE_NORMAL)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	}
	else if (g_aFade.nType == FADETYPE_ROTATE)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.75f) * g_fLengthFade;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + -D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aFade.pos.x + sinf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.y = g_aFade.pos.y + cosf(g_aFade.rot.z + D3DX_PI * 0.25f) * g_fLengthFade;
		pVtx[3].pos.z = 0.0f;
	}

	//���_�J���[�̐ݒ�
	pVtx[1].col = g_aFade.col;
	pVtx[2].col = g_aFade.col;
	pVtx[3].col = g_aFade.col;
	pVtx[0].col = g_aFade.col;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//==========================================================
//�ʏ�t�F�[�h����
//==========================================================
void NormalFade(void)
{
	if (g_fade != FADE_NONE)
	{//�������Ă��Ȃ���ԈȊO
		if (g_fade == FADE_IN)
		{//�t�F�[�h�C�����
			g_aFade.col.a -= 1.0f / 30;	//�����ɋ߂Â��Ă���

			if (g_aFade.col.a <= 0.0f)
			{//���S�ɓ����ɂȂ����ꍇ
				g_aFade.col.a = 0.0f;	//�����x�𓧖���
				g_fade = FADE_NONE;	//�������Ă��Ȃ���Ԃ�
			}
		}
		else if (g_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���

			g_aFade.col.a += 1.0f / 30;	//�s�����ɋ߂Â��Ă���

			if (g_aFade.col.a >= 1.0f)
			{//���S�ɕs�����ɂȂ����ꍇ
				g_aFade.col.a = 1.0f;	//�����x��s������

				//���[�h�̐ݒ�(���̉�ʂɈڍs)
				SetMode(g_modeNext);

				g_fade = FADE_IN;	//�������Ă��Ȃ���Ԃ�
				g_aFade.nType = FADETYPE_ROTATE;


			}
		}
	}
}

//==========================================================
//��]�t�F�[�h����
//==========================================================
void RotateFade(void)
{
	if (g_fade != FADE_NONE)
	{//�������Ă��Ȃ���ԈȊO
		if (g_fade == FADE_IN)
		{//�t�F�[�h�C�����

			g_fLengthFade += ROTATE_LENGTH;

			if (g_fLengthFade >= 14000.0f && g_aFade.col.a > 0.0f)
			{
				g_aFade.col.a -= 1.0f / 25;	//�����ɋ߂Â��Ă���
			}

			if (g_fLengthFade >= 46000.0f)
			{//���S�ɓ����ɂȂ����ꍇ
				g_aFade.col.a = 0.0f;	//�����x�𓧖���
				g_fade = FADE_NONE;		//�������Ă��Ȃ���Ԃ�
			}

			if (g_fLengthFade + ROTATE_LENGTH >= 46000.0f)
			{
				g_fLengthFade = 46000.0f;
			}
		}
		else if (g_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���

			g_fLengthFade -= ROTATE_LENGTH;

			if (g_fLengthFade <= 901.0f)
			{//���S�ɕs�����ɂȂ����ꍇ
				g_aFade.col.a = 1.0f;	//�����x��s������

				g_fLengthFade = g_fOldLengthFade;		//���̒���

				//���[�h�̐ݒ�(���̉�ʂɈڍs)
				SetMode(g_modeNext);
				g_aFade.nType = FADETYPE_ROTATE;
				g_fade = FADE_IN;	//�������Ă��Ȃ���Ԃ�
			}

			if (g_fLengthFade - ROTATE_LENGTH <= 901.0f)
			{//���S�ɕs�����ɂȂ����ꍇ
				g_fLengthFade = 901.0f;
				g_aFade.nType = FADETYPE_NORMAL;
			}
		}
	}
}

//==========================================================
//�t�F�[�h�̕`�揈��
//==========================================================
void DrawFade(void)
{
	if (g_fade != FADE_NONE)
	{//�������Ă��Ȃ���ԈȊO�̏ꍇ
		PDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̎擾

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(
			0,
			g_pVtxBuffFade,
			0,
			sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		if (g_aFade.nType == FADETYPE_NORMAL)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}
		else if (g_aFade.nType == FADETYPE_ROTATE)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureFade[1]);
		}

		//�t�F�[�h�̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,
			2	//���_���\���̂̃T�C�Y
		);
	}
}

//==========================================================
//�t�F�[�h�̓����x�擾
//==========================================================
FADE GetFade(void)
{
	//�����x��Ԃ�
	return g_fade;
}

//==========================================================
//�t�F�[�h�̏�Ԑݒ�
//==========================================================
void SetFade(MODE modeNext)
{
	if (g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;	//�t�F�[�h�A�E�g��Ԃ�
		g_modeNext = modeNext;	//���̉��(���[�h)��

		g_aFade.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//�����ɐݒ�
	}
}

//==========================================================
//�t�F�[�h�̏��
//==========================================================
Fade *GetFadeInfo(void)
{
	return &g_aFade;
}