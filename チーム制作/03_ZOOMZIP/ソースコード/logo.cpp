//==========================================================
//
//�`�[�����S��ʏ��� [logo.h]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"    //�쐬����main.h���C���N���[�h����
#include "logo.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//==========================================================
//�}�N����`
//==========================================================
#define LOGO_X			(SCREEN_WIDTH * 0.5f)	//���SX���W
#define LOGO_Y			(SCREEN_HEIGHT * -0.2f)	//���SY���W
#define LOGO_MOVEDY		(SCREEN_HEIGHT * 0.5f)	//���S�ړ���Y���W
#define LOGO_WIDTH		(SCREEN_WIDTH * 0.2f)	//���S��
#define LOGO_HEIGHT		(SCREEN_HEIGHT * 0.2f)	//���S����
#define LOGO_FADECNT	(120)					//�J�ڃJ�E���g
#define LOGO_ROTATECNT	(80)					//��]�J�E���^�[
#define LOGO_ROTATESPD	(D3DX_PI * 0.1f)		//��]���x
#define MAX_LENGTH		(500.0f)				//�ő咷��
#define LOGO_LENGSPD	(4.0f)					//�����������x
#define LOGO_YMOVESPD	((LOGO_MOVEDY - LOGO_Y) * (float)(1.0f / LOGO_ROTATECNT))	//y���W�ړ���
#define SUBLOGO_COLSPD	(35)					//�F�ω��^�C�}�[
#define SUBLOGO_X		(SCREEN_WIDTH * 0.2f)	//�T�u���SX���W
#define SUBLOGO_Y		(SCREEN_HEIGHT * 0.8f)	//�T�u���SY���W
#define SUBLOGO_WIDTH	(SCREEN_WIDTH * 0.2f)	//���S��
#define SUBLOGO_HEIGHT	(SCREEN_HEIGHT * 0.25f)	//���S����
#define SUBLOGO_SPACEX	(SCREEN_WIDTH * 0.2f)	//���S���X�y�[�X

//==========================================================
//�\���̂̒�`
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXCOLOR col;		//�F
	float fAngle;		//�p�x
	float fLength;		//�Ίp���̒���
	float fHeight;		//����
	float fWidth;		//��
	int nMoveCounter;	//�ړ��J�E���^�[
}Logo;

//==========================================================
//�O���[�o���ϐ�
//==========================================================
LPDIRECT3DTEXTURE9 g_apTextureLogo[LOGOTYPE_MAX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;				//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogoBg = NULL;			//���_�o�b�t�@�ւ̃|�C���^
int g_nLogoFadeCounter = 0;									//���S�J�ڃJ�E���^�[
Logo g_aLogo[LOGOTYPE_MAX];	//���S�̏��z��

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void SetpVtxLogoBg(void);
void SetpVtxLogo(void);
void DrawLogoBg(void);
Logo *GetLogo(void);
void AppearLogo(void);
void ChangeLogoCola(Logo *pLogo);

//==========================================================
//�e�N�X�`���t�@�C����
//==========================================================
const char* c_apFilenameLogo[LOGOTYPE_MAX] =
{
	"data\\TEXTURE\\team_logo000.png",		//���S��
	"data\\TEXTURE\\team_logo001.png",		//���S�̃R�s�[
	"data\\TEXTURE\\team_logo002.png",		//���S�̃R�s�[(2)
	"data\\TEXTURE\\team_logo003.png",		//���S�̃R�s�[(3)
	"data\\TEXTURE\\team_logo004.png",		//���S�̃R�s�[(4)
};

//==========================================================
//�`�[�����S�̏���������
//==========================================================
void InitLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾
	Logo *pLogo = GetLogo();	//���S�̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameLogo[nCnt],
			&g_apTextureLogo[nCnt]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGOTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogoBg,
		NULL);

	//��񏉊���
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX - 1; nCnt++, pLogo++)
	{
		pLogo->pos = D3DXVECTOR3(SUBLOGO_X + nCnt * SUBLOGO_SPACEX, SUBLOGO_Y, 0.0f);
		pLogo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pLogo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pLogo->fHeight = SUBLOGO_HEIGHT;
		pLogo->fWidth = SUBLOGO_WIDTH;
		pLogo->fAngle = 0.0f;
		pLogo->fLength = 0.0f;
		pLogo->nMoveCounter;

		//�Ίp���̒������Z�o����
		pLogo->fLength = sqrtf(SUBLOGO_WIDTH * SUBLOGO_WIDTH + SUBLOGO_HEIGHT * SUBLOGO_HEIGHT) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		pLogo->fAngle = atan2f(SUBLOGO_WIDTH, SUBLOGO_HEIGHT);
	}

	pLogo->pos = D3DXVECTOR3(LOGO_X, LOGO_Y, 0.0f);
	pLogo->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pLogo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pLogo->fHeight = LOGO_HEIGHT;
	pLogo->fWidth = LOGO_WIDTH;
	pLogo->fAngle = 0.0f;
	pLogo->fLength = 0.0f;
	pLogo->nMoveCounter;

	//�Ίp���̒������Z�o����
	pLogo->fLength = sqrtf(LOGO_WIDTH * LOGO_WIDTH + LOGO_HEIGHT * LOGO_HEIGHT) * 0.5f;

	//�Ίp���̊p�x���Z�o����
	pLogo->fAngle = atan2f(LOGO_WIDTH, LOGO_HEIGHT);

	//�w�i�ݒ�
	SetpVtxLogoBg();

	//���S�ݒ�
	SetpVtxLogo();
}

//==========================================================
//�`�[�����S�̏I������
//==========================================================
void UninitLogo(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		if (g_apTextureLogo[nCnt] != NULL)
		{
			g_apTextureLogo[nCnt]->Release();
			g_apTextureLogo[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLogoBg != NULL)
	{
		g_pVtxBuffLogoBg->Release();
		g_pVtxBuffLogoBg = NULL;
	}
}

//==========================================================
//�`�[�����S�̍X�V����
//==========================================================
void UpdateLogo(void)
{
	if (g_nLogoFadeCounter >= LOGO_FADECNT ||
		GetKeyboardTrigger(DIK_RETURN) == true || 
		GetGamepadTrigger(BUTTON_A, 0) == true || 
		GetGamepadTrigger(BUTTON_START,0) == true)
	{//�J�E���^�[�K��l�A�܂��̓{�^������
		g_nLogoFadeCounter = 0;	//�J�E���^�[���Z�b�g
		SetFade(MODE_TITLE);	//�^�C�g���֑J��
		PlaySound(SOUND_LABEL_SEBUTTON);
	}

	if (g_aLogo[LOGOTYPE_COPY002].col.a >= 1.0f)
	{//�S�Ẵ��S���s�����ɂȂ����ꍇ
		g_aLogo[LOGOTYPE_COPY003].nMoveCounter++;

		if (g_aLogo[LOGOTYPE_COPY003].nMoveCounter <= LOGO_ROTATECNT)
		{//�ړ��J�E���^�[����]�J�E���g��菭�Ȃ��ꍇ

			g_aLogo[LOGOTYPE_COPY003].rot.z += LOGO_ROTATESPD;	//��]
			g_aLogo[LOGOTYPE_COPY003].pos.y += LOGO_YMOVESPD;	//Y���W�ړ�
			g_aLogo[LOGOTYPE_COPY003].col.a += (1.0f * (float)(1.0f / LOGO_ROTATECNT));	//�s�����ɋ߂Â���

			if (g_aLogo[LOGOTYPE_COPY003].rot.z >= D3DX_PI)
			{//�p�x���E�������ꍇ
				g_aLogo[LOGOTYPE_COPY003].rot.z = -D3DX_PI;	//���]����
			}
		}
		else
		{
			g_aLogo[LOGOTYPE_COPY003].fLength += LOGO_LENGSPD;

			if (g_aLogo[LOGOTYPE_COPY003].fLength > MAX_LENGTH)
			{//�������������ꍇ
				g_aLogo[LOGOTYPE_COPY003].fLength = MAX_LENGTH;
				g_nLogoFadeCounter++;
			}
		}
	}
	else
	{
		//�s��������
		AppearLogo();
	}

	//���S�̒��_���ݒ�
	SetpVtxLogo();
}

//==========================================================
//�`�[�����S�̕`�揈��
//==========================================================
void DrawLogo(void)
{
	//�w�i�`��
	DrawLogoBg();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureLogo[nCnt]);

		//�|���S���̕`��(�J�n���_�ړ�)
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}

//==========================================================
//�`�[�����S�w�i�̒��_���ݒ�
//==========================================================
void SetpVtxLogoBg(void)
{
	VERTEX_2D * pVtx;   //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffLogoBg->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLogoBg->Unlock();
}

//==========================================================
//�`�[�����S�̒��_���ݒ�
//==========================================================
void SetpVtxLogo(void)
{
	Logo *pLogo = GetLogo();	//���S�̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffLogo->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++, pLogo++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + (-D3DX_PI + pLogo->fAngle)) * pLogo->fLength;
		pVtx[0].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + (-D3DX_PI + pLogo->fAngle)) * pLogo->fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + (D3DX_PI - pLogo->fAngle)) * pLogo->fLength;
		pVtx[1].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + (D3DX_PI - pLogo->fAngle)) * pLogo->fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + -pLogo->fAngle) * pLogo->fLength;
		pVtx[2].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + -pLogo->fAngle) * pLogo->fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = pLogo->pos.x + sinf(pLogo->rot.z + pLogo->fAngle) * pLogo->fLength;
		pVtx[3].pos.y = pLogo->pos.y + cosf(pLogo->rot.z + pLogo->fAngle) * pLogo->fLength;
		pVtx[3].pos.z = 0.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = pLogo->col;
		pVtx[1].col = pLogo->col;
		pVtx[2].col = pLogo->col;
		pVtx[3].col = pLogo->col;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLogo->Unlock();
}

//==========================================================
//�`�[�����S�w�i�̕`�揈��
//==========================================================
void DrawLogoBg(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLogoBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================
//�`�[�����S�̏��擾
//==========================================================
Logo *GetLogo(void)
{
	return &g_aLogo[0];
}

//==========================================================
//�`�[�����S�̏o������
//==========================================================
void AppearLogo(void)
{
	Logo *pLogo = GetLogo();	//���S�̃|�C���^

	for (int nCntLogo = 0; nCntLogo < LOGOTYPE_MAX - 1; nCntLogo++, pLogo++)
	{
		if (nCntLogo == 0 && pLogo->col.a < 1.0f)
		{//�J�E���g0��
			ChangeLogoCola(pLogo);	//�F�ύX
		}
		else if (nCntLogo != 0)
		{
			if ((pLogo - 1)->col.a >= 1.0f && pLogo->col.a < 1.0f)
			{//��O���s���������݂̂��s�����ł͂Ȃ��ꍇ
				ChangeLogoCola(pLogo);	//�F�ύX
			}
		}
	}
}

//==========================================================
//�`�[�����S�̕s��������
//==========================================================
void ChangeLogoCola(Logo *pLogo)
{
	if (pLogo->col.a < 1.0f)
	{//�F�����S�ɕs�����ł͂Ȃ��ꍇ
		pLogo->col.a += 1.0f * (float)(1.0f / SUBLOGO_COLSPD);	//�s�����ɋ߂Â���

		if (pLogo->col.a > 1.0f)
		{//�F���E�𒴂���
			pLogo->col.a = 1.0f;	//�s������
		}
	}
}