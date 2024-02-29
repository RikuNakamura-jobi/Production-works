//========================================
//
//2D�V���[�e�B���O�Q�[��(�|�[�Y����)[pause.cpp]
//Author:���v�ԗD��
//
//========================================
#include "main.h"
#include "input.h"
#include "pause.h"
#include "game.h"
#include "fade.h"
#include "option.h"
#include "pause_frame.h"
#include "sound.h"

//==========================================
//  �|���S����ԗ񋓌^��`  kanazaki
//==========================================
typedef enum
{
	POLYGONSTATE_NONE = 0,
	POLYGONSTATE_GRAW,
	POLYGONSTATE_DECLINE,
	POLYGONSTATE_MAX
}POLYGONSTATE;

//==========================================
//  �}�N����`
//  tomoya kanazaki
//==========================================
#define PASUE_BASE_POS (D3DXVECTOR3(640.0f, 325.0f, 0.0f)) //�|���S���̊�ʒu
#define PAUSE_SIZE (D3DXVECTOR3(190.0f, 45.0f, 0.0f)) //�|���S���̊�T�C�Y
#define PAUSE_SPACE (130.0f) //�|���S�����m�̊Ԋu
#define PAUSE_SELECT_WIDTH (PAUSE_SIZE.x * 1.1f) //�I���|���S���̕�
#define PAUSE_SELECT_HEIGHT (PAUSE_SIZE.y * 1.1f) //�I���|���S���̍���
#define PAUSE_NONE_WIDTH (PAUSE_SIZE.x * 0.9f) //��I���|���S���̕�
#define PAUSE_NONE_HEIGHT (PAUSE_SIZE.y * 0.9f) //��I���|���S���̍���
#define PAUSE_BITWEEN (0.01f) //�|���S���̊g�k��
#define PAUSE_GRAW_WIDTH (PAUSE_SELECT_WIDTH * 1.1f) //���荀�ڂ̕�
#define PAUSE_GRAW_HEIGHT (PAUSE_SELECT_HEIGHT * 1.1f) //���荀�ڂ̍���
#define PAUSE_GRAW (0.02f) //�|���S���̐�����

//==========================================
//  �|�[�Y�\���̒�`  kanazaki
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	float fWidth; //�|���S���̕�
	float fHeight; //�|���S���̍���
	D3DXCOLOR col; //���_�J���[
	POLYGONSTATE graw;
}PAUSE;

//�|�[�Y�̃t�@�C������ێ�
const char *c_apFilenamePause[] =
{
	"data\\TEXTURE\\continue000.png",
	"data\\TEXTURE\\restart000.png",
	"data\\TEXTURE\\title003.png",
};

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//���_�o�b�t�@�ւ̃|�C���^
PAUSE_MENU g_PauseMenu;									//�|�[�Y���j���[
PAUSE g_Pause[PAUSE_MENU_MAX];		//�|�[�Y�\����
int aData[PAUSE_MENU_MAX];

//========================================
//�|�[�Y�̏���������
//========================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;									//���_���ւ̃|�C���^
	int nCntPause;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < PAUSE_MENU_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenamePause[nCntTex],
			&g_apTexturePause[nCntTex]
		);
	}

	//�ϐ��̏����� kanazaki
	g_PauseMenu = PAUSE_MENU_CONTENUE;
	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//�|���S�����W��ݒ�
		g_Pause[nCntPause].pos = PASUE_BASE_POS;
		g_Pause[nCntPause].pos.y += PAUSE_SPACE * nCntPause;

		//�I��/��I���̌ʐݒ�
		if (nCntPause == g_PauseMenu) //�I��
		{
			//�|���S����傫���T�C�Y�ɐݒ�
			g_Pause[nCntPause].fWidth = PAUSE_SELECT_WIDTH;
			g_Pause[nCntPause].fHeight = PAUSE_SELECT_HEIGHT;

			//�s�����ɐݒ�
			g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else //��I��
		{
			//�|���S�����������T�C�Y�ɐݒ�
			g_Pause[nCntPause].fWidth = PAUSE_NONE_WIDTH;
			g_Pause[nCntPause].fHeight = PAUSE_NONE_HEIGHT;

			//�������ɐݒ�
			g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		}
		g_Pause[nCntPause].graw = POLYGONSTATE_NONE;

		//sakuma
		aData[nCntPause] = 0;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_Pause[nCntPause].col;
		pVtx[1].col = g_Pause[nCntPause].col;
		pVtx[2].col = g_Pause[nCntPause].col;
		pVtx[3].col = g_Pause[nCntPause].col;

		//�e�N�X�`���̍��W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;								//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

	//�I�v�V�����@�\
	InitOption();

	//�|�[�Y�̘g�̏���������
	InitPauseFrame();
}

//========================================
//�|�[�Y�̏I������
//========================================
void UninitPause(void)
{
	int nCntPause;

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//�e�N�X�`���̔j��
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}

	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	//�I�v�V�����@�\
	UninitOption();

	//�|�[�Y�̘g�̏I������
	UninitPauseFrame();
}

//========================================
//�|�[�Y�̍X�V����
//========================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;									//���_���ւ̃|�C���^
	int nCntPause;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (g_Pause[nCntPause].graw == POLYGONSTATE_NONE)
		{
			if (nCntPause == g_PauseMenu) //�I��
			{
				//�|���S����傫���T�C�Y�ɐݒ�
				if (g_Pause[nCntPause].fWidth < PAUSE_SELECT_WIDTH)
				{
					g_Pause[nCntPause].fWidth += g_Pause[nCntPause].fWidth * PAUSE_BITWEEN;
				}
				if (g_Pause[nCntPause].fHeight < PAUSE_SELECT_HEIGHT)
				{
					g_Pause[nCntPause].fHeight += g_Pause[nCntPause].fHeight * PAUSE_BITWEEN;
				}

				//�s�����ɐݒ�
				g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else //��I��
			{
				//�|���S�����������T�C�Y�ɐݒ�
				if (g_Pause[nCntPause].fWidth > PAUSE_NONE_WIDTH)
				{
					g_Pause[nCntPause].fWidth -= g_Pause[nCntPause].fWidth * PAUSE_BITWEEN;
				}
				if (g_Pause[nCntPause].fHeight > PAUSE_NONE_HEIGHT)
				{
					g_Pause[nCntPause].fHeight -= g_Pause[nCntPause].fHeight * PAUSE_BITWEEN;
				}

				//�������ɐݒ�
				g_Pause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			}
		}

		if (GetKeyboardTrigger(DIK_P) == true)
		{
			g_PauseMenu = PAUSE_MENU_CONTENUE;
			aData[nCntPause] = 0;
		}

		if (GetFade() == FADE_NONE)
		{
			//�L�[�������ꂽ�Ƃ�
			if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
			{//W�L�[�������ꂽ

				if (aData[nCntPause] >= 1)
				{
					aData[nCntPause]--;
				}
				else if (aData[nCntPause] == 0)
				{
					aData[nCntPause] = 2;
				}

				PlaySound(SOUND_LABEL_SE_BUTTON_003);
			}
			if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
			{//S�L�[�������ꂽ

				if (aData[nCntPause] == 2)
				{
					aData[nCntPause] = 0;
				}
				else if (aData[nCntPause] >= 0)
				{
					aData[nCntPause]++;
				}
				PlaySound(SOUND_LABEL_SE_BUTTON_003);
			}
		}

		switch (aData[nCntPause])
		{
		case PAUSE_MENU_CONTENUE:
			g_PauseMenu = PAUSE_MENU_CONTENUE;
			break;

		case PAUSE_MENU_RETRY:
			g_PauseMenu = PAUSE_MENU_RETRY;
			break;

		case PAUSE_MENU_QUIT:
			g_PauseMenu = PAUSE_MENU_QUIT;
			break;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y - g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x - g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Pause[nCntPause].pos.x + g_Pause[nCntPause].fWidth, g_Pause[nCntPause].pos.y + g_Pause[nCntPause].fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_Pause[nCntPause].col;
		pVtx[1].col = g_Pause[nCntPause].col;
		pVtx[2].col = g_Pause[nCntPause].col;
		pVtx[3].col = g_Pause[nCntPause].col;

		pVtx += 4;								//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���肳�ꂽ���ڂ��g��
	if (g_Pause[g_PauseMenu].graw == POLYGONSTATE_GRAW)
	{
		if (g_Pause[g_PauseMenu].fWidth < PAUSE_GRAW_WIDTH && g_Pause[g_PauseMenu].fHeight < PAUSE_GRAW_HEIGHT)
		{
			g_Pause[g_PauseMenu].fWidth += g_Pause[g_PauseMenu].fWidth * PAUSE_GRAW;
			g_Pause[g_PauseMenu].fHeight += g_Pause[g_PauseMenu].fHeight * PAUSE_GRAW;
		}
		else
		{
			g_Pause[g_PauseMenu].graw = POLYGONSTATE_DECLINE;
		}
	}

	//���肳�ꂽ���ڂ��k��
	if (g_Pause[g_PauseMenu].graw == POLYGONSTATE_DECLINE)
	{
		if (g_Pause[g_PauseMenu].fWidth > PAUSE_SELECT_WIDTH)
		{
			g_Pause[g_PauseMenu].fWidth -= g_Pause[g_PauseMenu].fWidth * PAUSE_GRAW;
		}
		if (g_Pause[g_PauseMenu].fHeight > PAUSE_SELECT_HEIGHT)
		{
			g_Pause[g_PauseMenu].fHeight -= g_Pause[g_PauseMenu].fHeight * PAUSE_GRAW;
		}
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	if (GetFade() == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ��Ƃ�
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
		{//����L�[(ENTER�L�[)�������ꂽ�Ƃ�

			PlaySound(SOUND_LABEL_SE_BUTTON_001);

			switch (g_PauseMenu)
			{
			case PAUSE_MENU_CONTENUE:
				SetEnablePause(false);
				break;

			case PAUSE_MENU_RETRY:
				if (GetMode() == MODE_GAME)
				{
					SetFade(MODE_GAME);
				}
				if (GetMode() == MODE_TUTORIAL)
				{
					SetFade(MODE_TUTORIAL);
				}
				break;

			case PAUSE_MENU_QUIT:
				// ���[�h�ݒ�(���U���g��ʂɈڍs)
				SetFade(MODE_TITLE);
				break;
			}

			//�|���S���̊g�k��ݒ�
			if (g_PauseMenu != PAUSE_MENU_CONTENUE)
			{
				g_Pause[g_PauseMenu].graw = POLYGONSTATE_GRAW;
			}
		}
	}

	//�I�v�V�����@�\
	UpdateOption();

	//�|�[�Y�̘g�̍X�V����
	UpdatePauseFrame();

}

//========================================
//�|�[�Y�̕`�揈��
//========================================
void DrawPause(void)
{
	//�|�[�Y�̘g�̕`�揈��
	DrawPauseFrame();

	int nCntPause;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);
	}

	//�I�v�V�����@�\
	DrawOption();
}
