//==========================================================
//
//�|�[�Y�`�揈��
//Author ����ɐ�
//
//==========================================================
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "game.h"

//�}�N����`
#define NUM_PAUSE	(4)										//�|�[�Y��ʃe�N�X�`���̎��
#define BUTTON_PAUSE	(3)									//�|�[�Y�{�^���̐�
#define MAX_PAUSE	(5)										//�|�[�Y��ʂɕ`�悷��|���S����
#define PAUSE_BGDATA		"data\\TEXTURE\\pausebg000.png"	//�|�[�Y�w�i�e�N�X�`��
#define PAUSE_CONTINUEDATA	"data\\TEXTURE\\pause003.png"	//�Q�[����ʂɖ߂�e�N�X�`��
#define PAUSE_RETRYDATA		"data\\TEXTURE\\pause004.png"	//�Q�[������蒼���e�N�X�`��
#define PAUSE_QUITDATA		"data\\TEXTURE\\pause005.png"	//�^�C�g���ɖ߂�e�N�X�`��
#define PAUSE_BUTTONWIDTH	(120.0f)						//�|�[�Y��ʃ{�^����
#define PAUSE_BUTTONHEIGHT	(40.0f)							//�|�[�Y��ʃ{�^������
#define PAUSE_BUTTONX		(640.0f)						//�|�[�Y���X���W
#define PAUSE_BUTTONY		(280.0f)						//�|�[�Y���Y���W
#define PAUSE_BUTTONSPACE	(90.0f)							//�|�[�Y��ʏc�X�y�[�X
#define PAUSE_MOVECOLA		(-0.02f)						//���t���[���ς��{�^���̓����x
#define PAUSE_BGX			(640.0f)						//�w�ix���W
#define PAUSE_BGY			(360.0f)						//�w�iy���W
#define PAUSE_BGSIZE		(200.0f)						//�T�C�Y

//�|�[�Y��ʕK�v���񋓌^
typedef enum
{
	PAUSE_FADE = 0,		//�w��̃|�[�Y
	PAUSE_BG,			//�w��̔w�i
	PAUSE_CONTINUE,		//�Q�[����ʂ֖߂�
	PAUSE_RETRY,		//�Q�[������蒼��
	PAUSE_QUIT,			//�^�C�g����ʂ֖߂�
	PAUSE_MAX
}PAUSE;

//�v���g�^�C�v�錾
void SetPauseScreen(int nCntpause);

//�O���[�o���錾
LPDIRECT3DTEXTURE9 g_pTexturePause[NUM_PAUSE] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//���_�o�b�t�@�ւ̃|�C���^
int g_pauseMenu;									//�|�[�Y���j���[
D3DXCOLOR g_ButtonColor[BUTTON_PAUSE];					//�{�^�����Ƃ̃J���[
bool g_bButtonPress;									//�|�[�Y��ʂŌ��肵�����ǂ���
bool g_bButtonPause;									//�|�[�Y��ʂőI����ύX�������ǂ���
float g_bButtonCola;									//�����x��ݒ肷��

//�|�[�Y�̏���������
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�|�[�Y���j���[�I����������
	g_pauseMenu = PAUSEMENU_CONTINUE;
	g_bButtonPress = false;				//�{�^���F����������
	g_bButtonPause = false;				//�ύX��Ԃ�������
	g_bButtonCola = PAUSE_MOVECOLA;

	//�e�N�X�`���̓ǂݍ���
	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		switch (nCntPause)
		{
		case 0:
			D3DXCreateTextureFromFile(pDevice, PAUSE_BGDATA, &g_pTexturePause[nCntPause]);
			break;
		case 1:
			D3DXCreateTextureFromFile(pDevice, PAUSE_CONTINUEDATA, &g_pTexturePause[nCntPause]);
			break;
		case 2:
			D3DXCreateTextureFromFile(pDevice, PAUSE_RETRYDATA, &g_pTexturePause[nCntPause]);
			break;
		case 3:
			D3DXCreateTextureFromFile(pDevice, PAUSE_QUITDATA, &g_pTexturePause[nCntPause]);
			break;
		}
	}

	//�F�̐ݒ�
	for (int nCntPause = 0; nCntPause < BUTTON_PAUSE; nCntPause++)
	{
		g_ButtonColor[nCntPause] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);	//�����J���[��ݒ�
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * MAX_PAUSE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	//�e�|���S���̏�����
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		SetPauseScreen(nCntPause);
	}
}

//�|�[�Y��ʂɔz�u����|���S���̏�����
void SetPauseScreen(int nCntPause)
{
	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	pVtx += nCntPause * 4;	//�|�C���^��i�߂�

	switch (nCntPause)
	{
	case PAUSE_FADE:	//�|�[�Y
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		break;
	case PAUSE_BG:
		pVtx[0].pos = D3DXVECTOR3(PAUSE_BGX - PAUSE_BGSIZE, PAUSE_BGY - PAUSE_BGSIZE, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_BGX + PAUSE_BGSIZE, PAUSE_BGY - PAUSE_BGSIZE, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_BGX - PAUSE_BGSIZE, PAUSE_BGY + PAUSE_BGSIZE, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_BGX + PAUSE_BGSIZE, PAUSE_BGY + PAUSE_BGSIZE, 0.0f);
		break;
	default:
		pVtx[0].pos = D3DXVECTOR3(PAUSE_BUTTONX - PAUSE_BUTTONWIDTH, PAUSE_BUTTONY - PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSE_BUTTONX + PAUSE_BUTTONWIDTH, PAUSE_BUTTONY - PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSE_BUTTONX - PAUSE_BUTTONWIDTH, PAUSE_BUTTONY + PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSE_BUTTONX + PAUSE_BUTTONWIDTH, PAUSE_BUTTONY + PAUSE_BUTTONHEIGHT + ((nCntPause - PAUSE_CONTINUE) * PAUSE_BUTTONSPACE), 0.0f);
		break;
	}

	if (nCntPause == PAUSE_FADE)
	{
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);
	}
	else if (nCntPause == PAUSE_BG)
	{
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
	}
	else
	{
		pVtx[0].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
		pVtx[1].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
		pVtx[2].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
		pVtx[3].col = g_ButtonColor[nCntPause - PAUSE_CONTINUE];
	}

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//�|�[�Y�̏I������
void UninitPause(void)
{
	//�e�N�X�`���̔p��
	for (int nCntPause = 0; nCntPause < NUM_PAUSE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//�|�[�Y�̍X�V����
void UpdatePause(void)
{
	if (g_bButtonPress == true)
	{//�����ꂽ��Ԃ̏ꍇ
		return;
	}

	if (g_bButtonPause == false)
	{
		for (int nCntPause = 0; nCntPause < BUTTON_PAUSE; nCntPause++)
		{
			if (g_pauseMenu == nCntPause)
			{
				g_ButtonColor[nCntPause] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				g_ButtonColor[nCntPause] = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			}
		}
		g_bButtonPause = true;
	}

	g_ButtonColor[g_pauseMenu].a += g_bButtonCola;
	if (g_ButtonColor[g_pauseMenu].a >= 1.0f || g_ButtonColor[g_pauseMenu].a <= 0.0f)
	{
		g_bButtonCola *= -1.0f;
	}

	if (GetKeyboardRepeat(DIK_W) || GetKeyboardTrigger(DIK_W) || GetGamepadTrigger(BUTTON_UP, 0) == true)
	{//��ړ��L�[�����͂��ꂽ

		g_pauseMenu = (g_pauseMenu - 1 + BUTTON_PAUSE) % BUTTON_PAUSE ;
		g_bButtonPause = false;
		g_bButtonCola = PAUSE_MOVECOLA;
	}
	else if (GetKeyboardRepeat(DIK_S) || GetKeyboardTrigger(DIK_S) || GetGamepadTrigger(BUTTON_DOWN, 0) == true)
	{//���ړ��L�[�����͂��ꂽ
		
		g_pauseMenu = (g_pauseMenu + 1) % BUTTON_PAUSE;
		g_bButtonPause = false;
		g_bButtonCola = PAUSE_MOVECOLA;
	}
	else if (GetKeyboardTrigger(DIK_RETURN) || GetGamepadTrigger(BUTTON_A, 0) == true)
	{//����L�[�������ꂽ
		switch (g_pauseMenu)
		{
		case PAUSEMENU_CONTINUE:
			SetEnablePause(true);
			break;
		case PAUSEMENU_RETRY:
			SetFadeMode(FADEMODE_NORMAL);
			SetFade(MODE_GAME);
			break;
		case PAUSEMENU_QUIT:
			SetFadeMode(FADEMODE_NORMAL);
			SetFade(MODE_TITLE);
			break;
		}
		if (g_pauseMenu != PAUSEMENU_CONTINUE)
		{
			g_bButtonPress = true;		//�����ꂽ��ԂɕύX
		}
	}

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * PAUSE_CONTINUE;	//�|�C���^���{�^���܂Ői�߂Ă���

	for (int nCntPause = 0; nCntPause < BUTTON_PAUSE; nCntPause++)
	{
		pVtx[0].col = g_ButtonColor[nCntPause];
		pVtx[1].col = g_ButtonColor[nCntPause];
		pVtx[2].col = g_ButtonColor[nCntPause];
		pVtx[3].col = g_ButtonColor[nCntPause];

		pVtx += 4;	//�|�C���^���{�^���܂Ői�߂Ă���
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//�|�[�Y�̕`�揈��
void DrawPause(void)
{

	PDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffPause,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (nCntPause == PAUSE_FADE)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);
		}
		else
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePause[nCntPause - 1]);
		}

		//�|�[�Y�̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntPause * 4,
			2	//���_���\���̂̃T�C�Y
		);
	}
}