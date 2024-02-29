//===========================
//
//�_�C���N�gX.menu�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "menu.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//�}�N����`
#define NUM_MENU (4)											//�^�C�g����ʂ̐�

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureMenu[NUM_MENU] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_VtxBuffMenu = NULL;					//���_�����i�[
int g_nCounterMenuState;										//��ԊǗ��J�E���^�[
int nMenuSelect;
float g_fCntMenu = 0.0f;										//�^�C�g���̓_�ŗp�ϐ�
bool g_MenuEnd;												//�I������

//===========================
//�^�C�g����ʂ̏���������
//===========================
void InitMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntMenu;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\aoharuBG002.png",
		&g_apTextureMenu[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\menu001.png",
		&g_apTextureMenu[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\menu002.png",
		&g_apTextureMenu[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\menu003.png",
		&g_apTextureMenu[3]);

	g_nCounterMenuState = 120;
	g_fCntMenu = 0.0f;
	nMenuSelect = 0;
	g_MenuEnd = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_MENU,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_VtxBuffMenu,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffMenu->Unlock();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM007);
}

//===========================
//�^�C�g����ʂ̏I������
//===========================
void UninitMenu(void)
{
	int nCntMenu;

	//�T�E���h�̒�~
	StopSound();

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureMenu[nCntMenu] != NULL)
		{
			g_apTextureMenu[nCntMenu]->Release();
			g_apTextureMenu[nCntMenu] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_VtxBuffMenu != NULL)
	{
		g_VtxBuffMenu->Release();
		g_VtxBuffMenu = NULL;
	}
}

//===========================
//�^�C�g����ʂ̍X�V����
//===========================
void UpdateMenu(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	FADE fade = GetFade();
	int nCntMenu;

	if (g_MenuEnd == false)
	{
		col.a = (sinf(g_fCntMenu) + 1.0f) / 2;

		g_fCntMenu += 0.05f;
	}
	else
	{
		col.a = (sinf(g_fCntMenu) + 1.0f) / 2;

		g_fCntMenu += 0.5f;
	}

	//���̓J�[�\���ړ�
	if (GetKeyboardTrigger(DIK_S) == true && g_MenuEnd == false)
	{
		nMenuSelect++;
	}

	if (GetKeyboardTrigger(DIK_W) == true && g_MenuEnd == false)
	{
		nMenuSelect += 2;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_VtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		if (nCntMenu == (nMenuSelect % 3) + 1)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
		}
		else if (nCntMenu >= 1)
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
		}
		else
		{
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_VtxBuffMenu->Unlock();

	if (GetKeyboardTrigger(DIK_RETURN) == true && g_MenuEnd == false)
	{
		g_MenuEnd = true;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_DECISION002);
	}

	if (g_MenuEnd == true)
	{
		g_nCounterMenuState--;

		if (g_nCounterMenuState <= 0 && fade == FADE_NONE)
		{
			if (nMenuSelect % 3 == 0)
			{
				//���[�h�ݒ�
				SetFade(MODE_SELECT);
			}
			else if (nMenuSelect % 3 == 1)
			{
				//���[�h�ݒ�
				SetFade(MODE_TUTORIAL);
			}
			else if(nMenuSelect % 3 == 2)
			{
				//���[�h�ݒ�
				SetFade(MODE_TITLE);
			}
		}
	}
}

//===========================
//�^�C�g����ʂ̕`�揈��
//===========================
void DrawMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntMenu;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_VtxBuffMenu,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntMenu = 0; nCntMenu < NUM_MENU; nCntMenu++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureMenu[nCntMenu]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMenu * 4, 2);
	}
}