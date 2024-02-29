//===========================
//
//�_�C���N�gX.main�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "fade.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "billboard.h"
#include "meshfield.h"
#include "meshwall.h"
#include "ui.h"
#include "model.h"
#include "sound.h"

//�}�N����`
#define CLASS_NAME "WindowClass"												//�E�B���h�E�N���X�̖��O
#define WINDOW_NAME "3D GAME"										//�E�B���h�E�̖��O

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawOpe(void);

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;						//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;						//�t�H���g�ւ̃|�C���^
MODE g_mode = MODE_TITLE;
int g_nCountFPS = 0;							//FPS�J�E���g
bool g_bDispDebug = false;						//�f�o�b�O�\����ON/OFF

//===========================
//���C���֐�
//===========================
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hinstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),										//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,												//�E�B���h�E�̃X�^�C��
		WindowProc,												//�E�B���h�E�v���V�[�W��
		0,														//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,														//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hinstance,												//�C���X�^���X�n���h��
		LoadIcon(NULL, IDI_APPLICATION),						//�^�X�N�o�[�A�C�R��
		LoadCursor(NULL, IDC_ARROW),							//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),								//�N���C�A���g�̈�̔w�i�F
		NULL,													//���j���[�o�[
		CLASS_NAME,												//�E�B���h�E�N���X�̖��O
		LoadIcon(NULL, IDI_APPLICATION),						//�t�@�C���̃A�C�R��
	};

	HWND hWnd;													//�E�B���h�E�n���h��(���ʎq)
	MSG msg;													//���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;										//���ݎ���
	DWORD dwExecLastTime;										//�Ō�ɏ�����������
	DWORD dwFrameCount;											//�t���[���J�E���g
	DWORD dwFPSLastTime;										//�Ō��FPS���v����������
	
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E���쐬
	hWnd = CreateWindowEx
	(0,															//�g���E�B���h�E�X�^�C��
		CLASS_NAME,												//�E�B���h�E�N���X�̖��O
		WINDOW_NAME,											//�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,									//�E�B���h�E�X�^�C��
		CW_USEDEFAULT,											//�E�B���h�E�̍���X���W
		CW_USEDEFAULT,											//�E�B���h�E�̍���Y���W
		(rect.right - rect.left),								//�E�B���h�E�̕�
		(rect.bottom - rect.top),								//�E�B���h�E�̍���
		NULL,													//�V�E�B���h�E�̃n���h��
		NULL,													//���j���[�n���h���܂��͎q�E�B���h�EID
		hinstance,												//�C���X�^���X�n���h��
		NULL);													//�E�B���h�E�쐬�f�[�^

	//����������
	if (FAILED(Init(hinstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;								//������
	dwExecLastTime = timeGetTime();								//���ݎ������擾
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);									//�E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);											//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//Windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);									//���z���b�Z�[�W�𕶎����b�Z�[�W�ɕϊ�
				DispatchMessage(&msg);									//�E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̕`�揈��
			dwCurrentTime = timeGetTime();								//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;							//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;							//�����̊J�n��ۑ�

				//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;											//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//===========================
//�E�B���h�E�v���V�[�W��
//===========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;													//�Ԃ�l���i�[

	switch (uMsg)
	{
	case WM_DESTROY:											//�E�B���h�E�j���̃��b�Z�[�W
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:											//�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:												//�{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//===========================
//����������
//===========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;					//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;			//�v���[���e�[�V�����p�����[�^

	//3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//�����̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;									//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;									//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3dpp.BackBufferFormat;						//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;												//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;								//�_�u���o�b�t�@�̐؊�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;									//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;								//�f�v�X�o�b�t�@�Ƃ���16bit�g��
	d3dpp.Windowed = bWindow;												//�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;				//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;				//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice))
	{
		if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice))
		{
			if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���[�X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�L�[�{�[�h�̏���������
	if (FAILED(InitDevice(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	ReadFilePlayer();
	ReadFileModel();

	//�T�E���h�̏���������
	InitSound(hWnd);

	//�f�o�b�O�\���p�t�H���g�̍쐬
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	InitPlayer();
	InitModel();

	//�t�F�[�h�̐ݒ�
	InitFade(g_mode);
	InitUi();
	InitMeshField();
	InitMeshWall();

	return S_OK;
}

//===========================
//�I������
//===========================
void Uninit(void)
{
	//UI�̏�����
	UninitUi();

	//�|���S���̏�����
	UninitMeshField();

	//�|���S���̏�����
	UninitMeshWall();

	//���f���̏�����
	UninitPlayer();

	//���f���̏�����
	UninitModel();

	//�^�C�g����ʂ̏I������
	UninitTitle();

	//�Q�[����ʂ̏I������
	UninitGame();

	//���U���g��ʂ̏I������
	UninitResult();

	//�t�F�[�h�̏I������
	UninitFade();

	//�T�E���h�̏I������
	UninitSound();

	//�L�[�{�[�h�̏I������
	UninitDevice();

	//�f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//===========================
//�X�V����
//===========================
void Update(void)
{
	//�L�[�{�[�h�̍X�V����
	UpdateDevice();

	switch (g_mode)
	{
	case MODE_TITLE:
		//�Q�[����ʂ̍X�V����
		UpdateTitle();
		break;
	case MODE_TUTORIAL:
		//�Q�[����ʂ̍X�V����
		UpdateTutorial();
		break;
	case MODE_GAME:
		//�Q�[����ʂ̍X�V����
		UpdateGame();
		break;
	case MODE_RESULT:
		//���U���g��ʂ̍X�V����
		UpdateResult();
		break;
	case MODE_RANKING:
		//���U���g��ʂ̏I������
		UpdateRanking();
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();

	if (GetKeyboardTrigger(DIK_F1) == true)
	{
		g_bDispDebug = g_bDispDebug ^ 1;
	}
}

//===========================
//�`�揈��
//===========================
void Draw(void)
{
	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������Ƃ�
		switch (g_mode)
		{
		case MODE_TITLE:
			//�Q�[����ʂ̕`�揈��
			DrawTitle();
			break;
		case MODE_TUTORIAL:
			//�Q�[����ʂ̍X�V����
			DrawTutorial();
			break;
		case MODE_GAME:
			//�Q�[����ʂ̕`�揈��
			DrawGame();
			break;
		case MODE_RESULT:
			//���U���g��ʂ̕`�揈��
			DrawResult();
			break;
		case MODE_RANKING:
			//���U���g��ʂ̏I������
			DrawRanking();
			break;
		}

		if (g_bDispDebug == false)
		{
#ifdef _DEBUG

			//�f�o�b�O�\��
			DrawFPS();

#endif // _DEBUG

#ifndef _DEBUG

			//����̕\��
			//DrawOpe();

#endif // _DEBUG
		}

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	DrawFade();

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//===========================
//���[�h�̐ݒ�
//===========================
void SetMode(MODE mode)
{
	//���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		//�^�C�g����ʂ̏I������
		UninitTitle();
		break;
	case MODE_TUTORIAL:
		//�Q�[����ʂ̍X�V����
		UninitTutorial();
		break;
	case MODE_GAME:
		//�Q�[����ʂ̏I������
		UninitGame();
		break;
	case MODE_RESULT:
		//���U���g��ʂ̏I������
		UninitResult();
		break;
	case MODE_RANKING:
		//���U���g��ʂ̏I������
		UninitRanking();
		break;
	}

	g_mode = mode;

	//�V������ʂ̏���������
	switch (mode)
	{
	case MODE_TITLE:
		//�^�C�g����ʂ̏���������
		InitTitle();
		break;
	case MODE_TUTORIAL:
		//�Q�[����ʂ̍X�V����
		InitTutorial();
		break;
	case MODE_GAME:
		InitUi();
		//�Q�[����ʂ̏���������
		InitGame();
		break;
	case MODE_RESULT:
		//���U���g��ʂ̏���������
		InitResult();
		break;
	case MODE_RANKING:
		//���U���g��ʂ̏I������
		InitRanking();
		break;
	}
}

//===========================
//�f�o�b�O�\��
//===========================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�ɕ`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================
//����\��
//===========================
void DrawOpe(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	//������ɑ��
	wsprintf(&aStr[0], "�ړ�\n��:W\n��:S\n��:A\n�E:D\n\n�ˌ�(�A��):space\n\n���_����(���E):[Q][E]\n���_����(�㉺):[T][B]\n\n�I��:escape", g_nCountFPS);

	//�e�L�X�g�ɕ`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//===========================
//�f�o�C�X�̎擾
//===========================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//===========================
//�t�H���g�̎擾
//===========================
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}

//===========================
//���[�h�̎擾����
//===========================
MODE GetMode(void)
{
	return g_mode;
}