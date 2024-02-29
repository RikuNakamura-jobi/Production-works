//========================================
//
//2D�V���[�e�B���O�Q�[��[main.cpp]
//Author:���v�ԗD��
//
//========================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "fade.h"
#include "team_logo.h"
#include "title.h"
#include "game.h"
#include "tutorial.h"
#include "result.h"
#include "ranking.h"
#include "camera.h"
#include "menu.h"
#include "player.h"
#include "enemy.h"
#include "object.h"
#include "orbit.h"
#include "shadow.h"
#include "sound.h"
#include "fog.h"
#include "option.h"
#include "model.h"
#include "score.h"
#include "effect.h"

//�}�N����`
#define CLASS_NAME				"WindowClass"					//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME				"3D�Q�[��"						//�E�C���h�E�̖��O(�L���v�V�����ɕ\��)
#define ID_BUTTON_FINISH		(101)							//�I���{�^����ID
#define ID_TIMER				(131)							//�^�C�}�[��ID
#define TIMER_INTERVAL			(1000/60)						//�^�C�}�[�̔����Ԋu(�~���b)

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstsnce, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);
void DrawMove(void);
void DrawRot(void);
void DrawPos(void);
void DrawModelPos(void);
void DrawEnemyState(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;										//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;							//Direc3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;										//�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;											//FPS�J�E���g
bool g_bDispDebug;										//�f�o�b�O�\����ON/OFF
#ifdef _DEBUG
MODE g_mode = MODE_LOGO;										//���݂̃��[�h
#else
MODE g_mode = MODE_LOGO;										//���݂̃��[�h
#endif

//========================================
//���C���֐�
//========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex
    {
        sizeof(WNDCLASSEX),											//WNDCLASSEX�̃������T�C�Y
        CS_CLASSDC,													//�E�C���h�E�̃X�^�C��
        WindowProc,													//�E�C���h�E�̃v���V�[�W��
        0,															//0�ɂ���(�ʏ�͎g�p���Ȃ�)
        0,															//0�ɂ���(�ʏ�͎g�p���Ȃ�)
        hInstance,													//�C���X�^���X�n���h��
        LoadIcon(NULL,IDI_APPLICATION),								//�^�X�N�o�[�̃A�C�R��
        LoadCursor(NULL,IDC_ARROW),									//�}�E�X�J�[�\��
        (HBRUSH)(COLOR_WINDOW + 1),									//�N���C�A���g�̈�̔w�i�F
        NULL,														//���j���[�o�[
        CLASS_NAME,													//�E�C���h�N���X�̖��O
        LoadIcon(NULL,IDI_APPLICATION),								//�t�@�C���̃A�C�R��
    };

    HWND hWnd;														//�E�C���h�E�n���h��(���ʎq)
    MSG msg;														//���b�Z�[�W���i�[����ϐ�
    DWORD dwCurrentTime;											//���ݎ���
    DWORD dwExecLastTime;											//�Ō�ɏ�����������
    DWORD dwFrameCount;												//�t���[���J�E���g
    DWORD dwFPSLastTime;											//�Ō�ɂ��v����������

    RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };				//��ʃT�C�Y�̍\����

    //�E�C���h�E�N���X�̓o�^
    RegisterClassEx(&wcex);

    AdjustWindowRect(&rect, WS_EX_OVERLAPPEDWINDOW, FALSE);

    //�E�C���h�E�𐶐�
    hWnd = CreateWindowEx(0,										//�g���E�C���h�E�X�^�C��
        CLASS_NAME,													//�E�C���h�E�N���X�̖��O
        WINDOW_NAME,												//�E�C���h�E�̖��O
        WS_OVERLAPPEDWINDOW,										//�E�C���h�E�̃X�^�C��
        CW_USEDEFAULT,												//�E�C���h�E�̍���X���W
        CW_USEDEFAULT,												//�E�C���h�E�̍���Y���W
        (rect.right - rect.left),									//�E�C���h�E�̕�
        (rect.bottom - rect.top),									//�E�C���h�E�̍���
        NULL,														//�e�E�C���h�E�̃n���h��
        NULL,														//���j���[�n���h���܂��͎q�E�C���h�EID
        hInstance,													//�C���X�^���X�n���h��
        NULL);														//�E�C���h�E�쐬�f�[�^

    //����������
#ifdef _DEBUG
    if (FAILED(Init(hInstance, hWnd, TRUE)))
    {//���������������s�����ꍇ
        return -1;
    }
#else
    if (FAILED(Init(hInstance, hWnd, FALSE)))
    {//���������������s�����ꍇ
        return -1;
    }
#endif //_DEBUG

    //����\��ݒ�
    timeBeginPeriod(1);
    dwCurrentTime = 0;												//������
    dwExecLastTime = timeGetTime();									//���ݎ������擾(�ۑ�)

    //�E�C���h�E�̕\��
    ShowWindow(hWnd, nCmdShow);										//�E�C���h�E�̕\����Ԃ�ݒ�
    UpdateWindow(hWnd);												//�N���C�A���g�̈���X�V
    dwFrameCount = 0;
    dwFPSLastTime = timeGetTime();

    srand((unsigned int)time(0));//�����̏�����

    //���b�Z�[�W���[�v
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                //���b�Z�[�W�̐ݒ�
                TranslateMessage(&msg);								//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
                DispatchMessage(&msg);								//�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
            }
        }
        else
        {//DirectX�̏���
            dwCurrentTime = timeGetTime();							//���ݎ������擾

            if ((dwCurrentTime - dwFPSLastTime) >= 500)
            {//0.5�b�o��
                //FPS���v��
                g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

                dwFPSLastTime = dwCurrentTime;						//FPS���v�������Ƃ��̎�����ۑ�
                dwFrameCount = 0;									//�t���[���J�E���g���N���A
            }
            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {//60����1�b�o��
                dwExecLastTime = dwCurrentTime;						//�����J�n�̎���[���ݎ���]��ۑ�

                //�X�V����
                Update();

                //�`�揈��
                Draw();

                dwFrameCount++;										//�t���[���J�E���g�����Z
            }
        }
    }
    //�I������
    Uninit();

    //�E�C���h�E�N���X�̓o�^������
    UnregisterClass(CLASS_NAME, wcex.hInstance);
    return(int)msg.wParam;

}

//========================================
//�E�C���h�E�v���V�[�W��
//========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int nID;														//�Ԃ�l���i�[

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:												//�L�[�����̃��b�Z�[�W
        switch (wParam)
        {
        case VK_ESCAPE:												//[ESC]�L�[�������ꂽ

            //nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
            //if (nID == IDYES)
            //{

                //�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
                DestroyWindow(hWnd);
            //}
            break;
        }
        break;

    case WM_COMMAND:												//�R�}���h���s�̃��b�Z�[�W
        if (LOWORD(wParam) == ID_BUTTON_FINISH)
        {
            nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
            if (nID == IDYES)
            {

                //�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
                DestroyWindow(hWnd);
            }
        }
        break;

    case WM_CLOSE:

        //nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);

        //if (nID == IDYES)
        //{
            //�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
            DestroyWindow(hWnd);
        //}
        //else
        //{
            return 0;												//0��Ԃ��Ȃ��ƏI�����Ă��܂�
        //}

        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);				//����̏�����Ԃ�
}

//========================================
//����������
//========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    D3DDISPLAYMODE d3ddm;											//�f�B�X�v���C���[�h
    D3DPRESENT_PARAMETERS d3dpp;									//�v���[���e�[�V�����p�����[�^

    //Direct3D�I�u�W�F�N�g�̐���
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
    ZeroMemory(&d3dpp, sizeof(d3dpp));								//�p�����[�^�̃[���N���A

    d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʃT�C�Y(��)
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʃT�C�Y(����)
    d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
    d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
    d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@�𐶐�
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@�Ƃ���16bit���g��
    d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//���t���b�V�����[�g
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

    //Direct3D�f�o�C�X�̐���
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &g_pD3DDevice)))
    {
        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp,
            &g_pD3DDevice)))
        {
            if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,

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

    //�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

    //�f�o�b�O�\���p�t�H���g�̐���
    D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Teminal", &g_pFont);

    //�L�[�{�[�h�̏���������
    if (FAILED(InitDevice(hInstance, hWnd)))
    {
        return E_FAIL;
    }
	InitOption();

    //�T�E���h�̏���������
    InitSound(hWnd);

	//���f�����̏�����
	InitModel();

	//���f���ǂݍ���
	LoadAllModel();

	//���̏�����
	InitFog();

    //�t�F�[�h�̐ݒ�
    InitFade(g_mode);

    //�����L���O�̓ǂݍ���
    ResetRanking();

#ifdef _DEBUG	//�f�o�b�O�\�L�����邩�ǂ���
	g_bDispDebug = false;
#else
	g_bDispDebug = false;
#endif

    return S_OK;
}

//========================================
//�I������
//========================================
void Uninit(void)
{
    //����\��߂�
    timeEndPeriod(1);

	//���f�����I��
	UninitModel();

	//���̏I������
	UninitFog();

	//�^�C�g����ʂ̏I������
	UninitTeam_logo();
	
	//�^�C�g����ʂ̏I������
	UninitTitle();

    //�`���[�g���A���̏I������
    UninitTutorial();

    //�Q�[����ʂ̏I������
    UninitGame();

    //���U���g��ʂ̏I������
    UninitResult();

    //�����L���O��ʂ̏I������
    UninitRanking();

    //�t�F�[�h�̏I������
    UninitFade();

    //�T�E���h�̏I������
    UninitSound();

    //���̓f�o�C�X�̏I������
    UninitDevice();
	UninitOption();

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

//========================================
//�X�V����
//========================================
void Update(void)
{
    //���̓f�o�C�X�̍X�V����
    UpdateDevice();

    switch (g_mode)
    {
	case MODE_LOGO:
		UpdateTeam_logo();									//���S���
		break;
	
	case MODE_TITLE:
        UpdateTitle();									//�^�C�g�����
        break;

    case MODE_TUTORIAL:
        UpdateTutorial();									//�`���[�g���A�����
        break;

    case MODE_GAME:
        UpdateGame();									//�Q�[�����
        break;

    case MODE_RANKING:
        UpdateRanking();								//�����L���O���
        break;
    }

	if (GetKeyboardTrigger(DIK_F1) == true)
	{//�f�o�b�O�\�L�؂�ւ�
		g_bDispDebug = (g_bDispDebug == false) ? true : false;
	}

	// ���̍X�V����
	UpdateSound();

	// ���̍X�V����
	UpdateFog();
    
	// �t�F�[�h�̍X�V����
    UpdateFade();
}

//========================================
//�`�揈��
//========================================
void Draw(void)
{
    //��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
    g_pD3DDevice->Clear(0, NULL,
        (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
        D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

    //�`��J�n
    if (SUCCEEDED(g_pD3DDevice->BeginScene()))
    {//�`��J�n�����������ꍇ

		//���̐ݒ�
		SetFog();

        switch (g_mode)
        {
		case MODE_LOGO:
			DrawTeam_logo();								//���S���
			break;
		
		case MODE_TITLE:
            DrawTitle();									//�^�C�g�����
            break;

        case MODE_TUTORIAL:
            DrawTutorial();									//�`���[�g���A�����
            break;

        case MODE_GAME:
            DrawGame();										//�Q�[�����
            break;

        case MODE_RANKING:
            DrawRanking();									//�����L���O���
            break;
        }

        //�t�F�[�h�̕`�揈��
        DrawFade();

		g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		g_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, (DWORD)0.66f);

		if (g_bDispDebug)
		{//�f�o�b�O�\�L
			//�ړ���
			DrawMove();

			//FPS�̕\��
			DrawFPS();

			//����
			DrawRot();

			//�ʒu
			DrawPos();

			//���f���̈ʒu
			DrawModelPos();

			//�G�̏��
			DrawEnemyState();

			//�J�����̏��
			DrawDebugCamera(g_pFont);

			//�I�u�W�F�N�g���
			DebugObject();

			//�G�̏��
			DrawEnemyDebug();

			//�v���C���[���[�V�������
			DebugPlayerMotion();

			//�X�R�A���
			DebugScore();

			//�G�t�F�N�g���
			DebugEffect();
		}

        //�`��I��
        g_pD3DDevice->EndScene();
    }

    //�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//========================================
//�f�o�C�X�̎擾
//========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
    return g_pD3DDevice;
}

//========================================
//FPS�̕\��
//========================================
void DrawFPS(void)
{
    RECT rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    //������ɑ��
    wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

    //�e�L�X�g�̕`��
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//�������
//========================================
void DrawMove(void)
{
    RECT rect1 = { 0,370,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[6][256];
	int nNumString;

	if (GetPhotoMode())
	{
		//������ɑ��
		wsprintf(&aStr[0][0], "�����ړ�:[WASD]\n");
		wsprintf(&aStr[1][0], "��    ��:[LSHIFT]\n");
		wsprintf(&aStr[2][0], "�㉺�ړ�:[TB]\n");
		wsprintf(&aStr[3][0], "���E��]:[QE]\n");
		wsprintf(&aStr[4][0], "�㉺�ړ�:[YN]\n");
		wsprintf(&aStr[5][0], "�t�H�g���[�h�؂�ւ�:[RCTRL]\n");
		nNumString = 6;
	}
	else
	{
		//������ɑ��
		wsprintf(&aStr[0][0], "�ړ�:[WASD]");
		wsprintf(&aStr[1][0], "�t�H�g���[�h�؂�ւ�:[RCTRL]\n");
		nNumString = 2;
	}

	for (int nCntText = 0;nCntText < nNumString;nCntText++)
	{
		//�e�L�X�g�̕`��
		g_pFont->DrawText(NULL, &aStr[nCntText][0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

		rect1.top += 20;
	}
}

//========================================
//�������
//========================================
void DrawRot(void)
{
    Camera *pCamera = GetCamera();
    RECT rect3 = { 0,200,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    sprintf(&aStr[0], "�J�����̊p�x;%0.3f %0.3f %0.3f", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);

    //�e�L�X�g�̕`��
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//�������
//========================================
void DrawPos(void)
{
    Camera *pCamera = GetCamera();
    RECT rect4 = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    sprintf(&aStr[0], "�����_�̈ʒu;%0.3f %0.3f %0.3f", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

    //�e�L�X�g�̕`��
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//�v���C���[�̏�ԃf�o�b�O�\��
//========================================
void DrawModelPos(void)
{
    Player *pPlayer = GetPlayer();
    RECT rect4 = { 0,220,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    float fAngle = atan2f(pPlayer->move.x, pPlayer->move.z);

    sprintf(&aStr[0], "�v���C���[�̗̑́F[%d] \n�v���C���[�̈ʒu�F[%.2f,%.2f,%.2f]\n�v���C���[�̌����F[%.2f,%.2f,%.2f]\n�ړ�����[%.2f]",
        pPlayer->nLife,pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z, pPlayer->rot.x, pPlayer->rot.y, pPlayer->rot.z, fAngle);

    //�e�L�X�g�̕`��
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//�G�̏�ԃf�o�b�O�\��
//========================================
void DrawEnemyState(void)
{
    Enemy *pEnemy = GetEnemy();
    RECT rect4 = { 0,350,SCREEN_WIDTH,SCREEN_HEIGHT };
    char aStr[256];

    sprintf(&aStr[0], "�G�̗̑́F[%d]", pEnemy->nLife);

    //�e�L�X�g�̕`��
    g_pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//========================================
//���[�h�̐ݒ�
//========================================
void SetMode(MODE mode)
{
    //���݂̉��(���[�h)�̏I������
    switch (g_mode)
    {
	case MODE_LOGO:
		UninitTeam_logo();									//���S���
		break;
	
	case MODE_TITLE:
        UninitTitle();										//�^�C�g�����
        break;

    case MODE_TUTORIAL:
        UninitTutorial();									//�`���[�g���A�����
        break;

    case MODE_GAME:
        UninitGame();										//�Q�[�����
        break;

    case MODE_RANKING:
        UninitRanking();									//�����L���O���
        break;
    }

    //a <-What?!
    g_mode = mode;

    //�V�������(���[�h)�̏���������
    switch (g_mode)
    {
	case MODE_LOGO:
		InitTeam_logo();									//���S���
		break;
	
	case MODE_TITLE:
        InitTitle();										//�^�C�g�����
        break;

    case MODE_TUTORIAL:
        InitTutorial();										//�`���[�g���A�����
        break;

    case MODE_GAME:
        InitGame();											//�Q�[�����
        break;

    case MODE_RANKING:
        InitRanking();											//�����L���O���
        break;
    }
}

//========================================
//���[�h�̎擾
//========================================
MODE GetMode(void)
{
    return g_mode;
}

//========================================
//�t�H���g�̎擾
//========================================
LPD3DXFONT GetFont(void)
{
    return g_pFont;
}