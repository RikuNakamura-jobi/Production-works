//==========================================================
//
//���C������ [main.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "stdio.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "fade.h"
#include "result.h"
#include "ranking.h"
#include "title.h"
#include "tutorial.h"
#include "sound.h"
#include "logo.h"

//�}�N����`
#define WINDOW_NAME	"ZOOMZIP"	//�E�C���h�E�̖��O
#define PCTIME		(1000/60)		//�p�\�R���̎���
#define MAX_STRING	(512)			//�����̍ő吔
#define CLASS_NAME	"WindowClass"	//�E�C���h�E�N���X�̖��O

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;	//Direct3D�I�u�W�F�N�g�̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//Direct3D�f�o�C�X�ւ̃|�C���^
int g_nCountFPS = 0;		//FPS�J�E���^
MODE g_mode = MODE_TEAMLOGO;	//���[�h
int g_bWire = 0;			//���C���[�t���[���\����ON/OFF

//================================================
//���C���֐�
//================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						//�E�C���h�E�̃X�^�C��
		WindowProc,						//�E�C���h�E�v���V�[�W��
		0,								//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,								//0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,						//�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		//�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		//�N���C�A���g�̈�̔w�i�F
		NULL,							//���j���[�o�[
		CLASS_NAME,						//�E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	//�t�@�C���̃A�C�R��
	};

	HWND hWnd;	//�E�C���h�E�n���h��
	MSG msg;	//���b�Z�[�W���i�[����ϐ�

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�`�����w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx
	(
		0,								//�g���E�C���h�E�X�^�C��
		CLASS_NAME,						//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,					//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,			//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,					//�E�C���h�E�̍���x���W
		CW_USEDEFAULT,					//�E�C���h�E�̍���y���W
		(rect.right - rect.left),		//�E�C���h�E�̕�
		(rect.bottom - rect.top),		//�E�C���h�E�̍���
		NULL,							//�e�E�C���h�E�̃n���h��
		NULL,							//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,						//�C���X�^���X�n���h��
		NULL							//�E�C���h�E�쐬�f�[�^
	);

	//����������
	if (FAILED(Init(hInstance, hWnd, FALSE)))
	{//�����������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//������
	dwExecLastTime = timeGetTime();	//���ݎ������擾(�ۑ�)
	dwFrameCount = 0;	//������
	dwFPSLastTime = timeGetTime();	//���ݎ������擾(�ۑ�)

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();	//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
			 //FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;	//�t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;	//�����J�n�̎���[���ݎ���]��ۑ�

												//�X�V����
				Update();

				//�`�揈��
				Draw();

				dwFrameCount++;	//�t���[���J�E���g�����Z
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//================================================
//�E�C���h�E�v���V�[�W��
//================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;					//�Ԃ�l���i�[


	const RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };		//�E�C���h�E�̗̈�(�`)

	switch (uMsg)
	{
	case WM_DESTROY:		//�E�B���h�E�p���̃��b�Z�[�W

							//WM_QUET���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:		//�L�[�����̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE:

			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

			if (nID == IDYES)
			{
				//�E�C���h�E��p������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_COMMAND:	//�R�}���h���s���b�Z�[�W
		break;
	case WM_CLOSE:		//����{�^������
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);

		if (nID == IDYES)
		{//�͂���I�������Ƃ�
		 //�E�C���h�E��p������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{//��������I�������Ƃ�
			return 0;	//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//����̏�����Ԃ�
}

//================================================
//����������
//================================================
LRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

									//DirectX3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						//�o�b�N�o�b�t�@�[�̌`��
	d3dpp.BackBufferCount = 1;									//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE;						//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��


	//Direct3D�f�o�C�X�̍쐬
	if (FAILED(g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
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

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	//�e�N�X�`���X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�C�X�̏�����
	if (FAILED(InitDevice(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�f�o�b�O�\���̏���������
	InitDebugProc();

	//�t�F�[�h�̏�����
	InitFade(g_mode);

	return S_OK;
}

//================================================
//�I������
//================================================
void Uninit(void)
{
	//�f�o�C�X�̏I������
	UninitDevice();

	//�T�E���h�̏I������
	UninitSound();

	//�f�o�b�O�\���̏I������
	UninitDebugProc();

	//�t�F�[�h�̏I������
	UninitFade();

	//Direct3D�f�o�C�X�̔p��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔p��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//================================================
//�X�V����
//================================================
void Update(void)
{
	//�f�o�C�X�̍X�V����
	UpdateDevice();

	//�f�o�b�O�\���̍X�V����
	UpdateDebugProc();

	//���̃��[�h�̍X�V
	switch (g_mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		UpdateTitle();
		break;
	case MODE_TUTORIAL:	//�`���[�g���A�����
		UpdateTutorial();
		break;
	case MODE_GAME:		//�Q�[�����
		UpdateGame();
		break;
	case MODE_RESULT:	//���U���g���
		UpdateResult();
		break;
	case MODE_RANKING:	//�����L���O���
		UpdateRanking();
		break;
	case MODE_TEAMLOGO:	//�`�[�����S���
		UpdateLogo();
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();

	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8�L�[���͂��ꂽ�Ƃ�
		g_bWire = g_bWire ^ 1;

		//�`�惂�[�h�̕ύX
		switch (g_bWire)
		{
		case 1:
			//���C���[�t���[��
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			break;
		case 0:
			//�ʏ�`��
			g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			break;
		}
	}
}

//================================================
//�`�揈��
//================================================
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`�悪���������ꍇ

		switch (g_mode)
		{
		case MODE_TITLE:	//�^�C�g�����
			DrawTitle();
			break;
		case MODE_TUTORIAL:	//�`���[�g���A�����
			DrawTutorial();
			break;
		case MODE_GAME:		//�Q�[�����
			DrawGame();
			break;
		case MODE_RESULT:	//���U���g���
			DrawResult();
			break;
		case MODE_RANKING:	//�����L���O���
			DrawRanking();
			break;
		case MODE_TEAMLOGO:	//�`�[�����S���
			DrawLogo();
			break;
		}

		//�t�F�[�h�̕`�揈��
		DrawFade();

		//�f�o�b�O�\���̕`�揈��
		DrawDebugProc();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//================================================
//�f�o�C�X�̎擾
//================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//================================================
//FPS�̎擾
//================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//================================================
//���[�h�̐ݒ�
//================================================
void SetMode(MODE mode)
{
	//���݂̃��[�h�̏I��
	switch (g_mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		UninitTitle();
		break;
	case MODE_TUTORIAL:	//�`���[�g���A�����
		UninitTutorial();
		break;
	case MODE_GAME:		//�Q�[�����
		UninitGame();
		break;
	case MODE_RESULT:	//���U���g���
		UninitResult();
		break;
	case MODE_RANKING:	//�����L���O���
		UninitRanking();
		break;
	case MODE_TEAMLOGO:	//�`�[�����S���
		UninitLogo();
		break;
	}

	//���̃��[�h�ɕύX
	g_mode = mode;

	//���̃��[�h�̏�����
	switch (mode)
	{
	case MODE_TITLE:	//�^�C�g�����
		InitTitle();
		break;
	case MODE_TUTORIAL:	//�`���[�g���A�����
		InitTutorial();
		break;
	case MODE_GAME:		//�Q�[�����
		InitGame();
		break;
	case MODE_RESULT:	//���U���g���
		InitResult();
		break;
	case MODE_RANKING:	//�����L���O���
		InitRanking();
		break;
	case MODE_TEAMLOGO:	//�`�[�����S���
		InitLogo();
		break;
	}
}

//================================================
//���[�h�̎擾
//================================================
MODE GetMode(void)
{
	return g_mode;
}