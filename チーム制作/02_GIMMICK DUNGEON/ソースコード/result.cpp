//==========================================
//
//  ���U���g���(result.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "result.h"
#include "input.h"
#include "fade.h"
#include "result_message.h"
#include "effect_2D.h"
#include "particle_2D.h"
#include "sound.h"

//==========================================
//  �񋓌^��`
//==========================================
typedef enum
{
	RESULT_RETRY = 0, //�X�^�[�g����
	RESULT_RANKING, //�`���[�g���A������
	RESULT_TITLE, //�^�C�g���ɖ߂�
	RESULT_MAX
}RESULTPATTERN;

//==========================================
//  �}�N����`
//==========================================
#define TOP_MENU (470.0f) //��ԏ�̃|���S����Y���W
#define RESULT_WIDTH (240.0f) //�|���S���̕�
#define RESULT_HEIGHT (45.0f) //�|���S���̍���
#define RESULT_ALPHA (0.01f) //1f�ŕω�����A���t�@�l
#define RANKING_FROM_RESULT (900) //���u�Ń����L���O�ɑJ�ڂ��鎞��
#define RESULT_WIDHT_NONE (RESULT_WIDTH * 0.9) //��I���|���S���̕�
#define RESULT_HEIGHT_NONE (RESULT_HEIGHT * 0.9) //��I���|���S���̍���
#define RESULT_WIDHT_SELECT (RESULT_WIDTH * 1.1f) //�I���|���S���̕�
#define RESULT_HEIGHT_SELECT (RESULT_HEIGHT * 1.1f) //�I���|���S���̍���
#define RESULT_BITWEEN (0.07f) //�|���S���̊e�k��
#define RESULT_GRAW (0.05f) //�|���S���̊g�嗦

//���U���g�̃t�@�C������ێ�
const char *c_apFilenameResult[] =
{
	"data\\TEXTURE\\retry001.png",
	"data\\TEXTURE\\ranking000.png",
	"data\\TEXTURE\\title001.png",
};

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //���S���W
	float height; //�|���S���̍���
	float width; //�|���S���̕�
	D3DXCOLOR col; //�|���S���J���[
	bool bSelect;
}RESULT;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureResult[RESULT_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;
RESULT g_Result[RESULT_MAX];
RESULTTYPE g_resultType;
float g_fResultAlpha;
int g_nResultState;
int g_nResultTimer;

//==========================================
//  ����������
//==========================================
void InitResult()
{
	//�ϐ��̏�����
	g_nResultState = RESULT_MAX;
	g_resultType = GetResult();
	g_fResultAlpha = RESULT_ALPHA;
	g_nResultTimer = 0;
	for (int nCntBuff = 0; nCntBuff < RESULT_MAX; nCntBuff++)
	{
		g_Result[nCntBuff].pos = D3DXVECTOR3
		(
			SCREEN_WIDTH * 0.75f,
			TOP_MENU + ((RESULT_HEIGHT * 2.0f) * nCntBuff),
			0.0f
		);
		if (nCntBuff == g_nResultState)
		{
			g_Result[nCntBuff].width = RESULT_WIDHT_SELECT;
			g_Result[nCntBuff].height = RESULT_HEIGHT_SELECT;
		}
		else
		{
			g_Result[nCntBuff].width = RESULT_WIDHT_NONE;
			g_Result[nCntBuff].height = RESULT_HEIGHT_NONE;
		}
		g_Result[nCntBuff].bSelect = false;
		g_Result[nCntBuff].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * RESULT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < RESULT_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameResult[nCntTex],
			&g_pTextureResult[nCntTex]
		);
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntBuff = 0; nCntBuff < RESULT_MAX; nCntBuff++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhw�̐ݒ�
			pVtx[nCnt].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_Result[nCntBuff].col;
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffResult->Unlock();

	//���b�Z�[�W�̏�����
	InitResultMessage();

	//�p�[�e�B�N���̕`��
	InitParticle2D();

	//�G�t�F�N�g�̕`��
	InitEffect2D();

	//�T�E���h�̒�~
	StopSound();

	//�T�E���h�Đ� =
	if (g_resultType == RESULTTYPE_WIN)
	{//����
		PlaySound(SOUND_LABEL_BGM004);
	}
	else
	{//�s�k
		PlaySound(SOUND_LABEL_BGM005);
	}
}

//==========================================
//  �I������
//==========================================
void UninitResult()
{
	//�T�E���h�̒�~
	StopSound();

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{
		if (g_pTextureResult[nCnt] != NULL)
		{
			g_pTextureResult[nCnt]->Release();
			g_pTextureResult[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	//���b�Z�[�W�̏I��
	UninitResultMessage();

	//�p�[�e�B�N���̕`��
	UninitParticle2D();

	//�G�t�F�N�g�̕`��
	UninitEffect2D();
}

//==========================================
//  �X�V����
//==========================================
void UpdateResult()
{
	if (GetResultState())
	{
		//��ԑJ��
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
		{
			PlaySound(SOUND_LABEL_SE_BUTTON_001);

			switch (g_nResultState)
			{
			case RESULT_RETRY:
				SetFade(MODE_GAME);
				break;
			case RESULT_RANKING:
				SetFade(MODE_RANKING);
				break;
			case RESULT_TITLE:
				SetFade(MODE_TITLE);
				break;
			}
		}

		//���u�^�C�}�[�̍X�V
		g_nResultTimer++;

		//�I����Ԃ̍X�V
		if (SelectResult())
		{
			if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_S) ||
				GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0) ||
				GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0)
				)
			{
				g_nResultState = RESULT_RETRY;

				PlaySound(SOUND_LABEL_SE_BUTTON_003);
			}
		}
		else
		{
			if (GetFade() == FADE_NONE)
			{
				if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
				{
					g_nResultState += (RESULT_MAX - 1);

					//�I����Ԃ̕␳
					g_nResultState %= RESULT_MAX;

					//���u�^�C�}�[�̃��Z�b�g
					g_nResultTimer = 0;

					PlaySound(SOUND_LABEL_SE_BUTTON_003);
				}
				if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
				{
					g_nResultState++;

					//�I����Ԃ̕␳
					g_nResultState %= RESULT_MAX;

					//���u�^�C�}�[�̃��Z�b�g
					g_nResultTimer = 0;

					PlaySound(SOUND_LABEL_SE_BUTTON_003);
				}
			}
		}

		//���j���[���ڂ̓_��
		if (g_Result[g_nResultState].col.a < 0.0f || g_Result[g_nResultState].col.a > 1.0f)
		{
			g_fResultAlpha *= -1.0f;
		}
		g_Result[g_nResultState].col.a += g_fResultAlpha;

		//��I�����ڂ̓����x�����Z�b�g����
		for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
		{
			if (nCnt == g_nResultState) //�I����
			{
				//�Ώۂ̃|���S�����g�傷��
				if (g_Result[nCnt].width < RESULT_WIDHT_SELECT)
				{
					g_Result[nCnt].width += g_Result[nCnt].width * RESULT_BITWEEN;
				}
				if (g_Result[nCnt].height < RESULT_HEIGHT_SELECT)
				{
					g_Result[nCnt].height += g_Result[nCnt].height * RESULT_BITWEEN;
				}
			}
			else //��I����
			{
				//�Ώۂ̃|���S�����k������
				if (g_Result[nCnt].width > RESULT_WIDHT_NONE)
				{
					g_Result[nCnt].width -= g_Result[nCnt].width * RESULT_BITWEEN;
				}
				if (g_Result[nCnt].height > RESULT_HEIGHT_NONE)
				{
					g_Result[nCnt].height -= g_Result[nCnt].height * RESULT_BITWEEN;
				}

				g_Result[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		//�t�F�[�h���̋���
		if (g_nResultState != RESULT_MAX)
		{
			if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
			{
				g_Result[g_nResultState].bSelect = true;
			}
			if (g_Result[g_nResultState].bSelect)
			{
				g_Result[g_nResultState].width += g_Result[g_nResultState].width * RESULT_GRAW;
				g_Result[g_nResultState].height += g_Result[g_nResultState].height * RESULT_GRAW;
				if (g_Result[g_nResultState].col.a > 0.0f)
				{
					g_Result[g_nResultState].col.a -= g_Result[g_nResultState].col.a * RESULT_GRAW * 3;
				}
				else
				{
					g_Result[g_nResultState].col.a = 0.0f;
				}
			}
		}

		//���_�o�b�t�@�̌Ăяo��
		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N
		g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		for (int nCntBuff = 0; nCntBuff < RESULT_MAX; nCntBuff++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y - g_Result[nCntBuff].height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x - g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Result[nCntBuff].pos.x + g_Result[nCntBuff].width, g_Result[nCntBuff].pos.y + g_Result[nCntBuff].height, 0.0f);

			//���_�J���[�̐ݒ�
			for (int nCnt = 0; nCnt < 4; nCnt++)
			{
				pVtx[nCnt].col = g_Result[nCntBuff].col;
			}

			//���_�f�[�^��i�߂�
			pVtx += 4;
		}

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffResult->Unlock();

		//�^�C�g���ɋ�������
		if (g_nResultTimer >= RANKING_FROM_RESULT)
		{
			SetFade(MODE_TITLE);
		}
	}

	//���b�Z�[�W�̍X�V
	UpdateResultMessage();

	//�p�[�e�B�N���̕`��
	UpdateParticle2D();

	//�G�t�F�N�g�̕`��
	UpdateEffect2D();
}

//==========================================
//  �`�揈��
//==========================================
void DrawResult()
{
	if (GetResultState())
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResult[nCnt]);

			//���j���[���ڂ̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//���b�Z�[�W�̕`��
	DrawResultMessage();

	//�p�[�e�B�N���̕`��
	DrawParticle2D();

	//�G�t�F�N�g�̕`��
	DrawEffect2D();
}

//==========================================
//  ��I���̏��
//==========================================
bool SelectResult()
{
	return g_nResultState == RESULT_MAX ? true : false;
}

//==========================================
//  ���ʂ̐ݒ�
//==========================================
void SetResult(RESULTTYPE type)
{
	g_resultType = type;
}

//==========================================
//  ���ʂ̎擾
//==========================================
RESULTTYPE GetResult()
{
	return g_resultType;
}
