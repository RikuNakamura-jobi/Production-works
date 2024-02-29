//==========================================
//
//  ���j���[����(menu_item.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "menu.h"
#include "menu_item.h"
#include "input.h"

//==========================================
//  �񋓌^��`
//==========================================
typedef enum
{
	ITEM_START = 0, //�X�^�[�g����
	ITEM_TUTORIAL, //�`���[�g���A������
	ITEM_QUIT, //�^�C�g���ɖ߂�
	ITEM_MAX
}ITEMPATTERN; //���j���[����

//==========================================
//  �}�N����`
//==========================================
#define TOP_POS (380.0f) //��ԏ�̃|���S����Y���W
#define ITEM_WIDTH (350.0f) //�|���S�����̊�l
#define ITEM_HEIGHT (55.0f) //�|���S�����̊�l
#define ITEM_WIDHT_NONE (ITEM_WIDTH * 0.9) //��I���|���S���̕�
#define ITEM_HEIGHT_NONE (ITEM_HEIGHT * 0.9) //��I���|���S���̍���
#define ITEM_WIDHT_SELECT (ITEM_WIDTH * 1.1) //��I���|���S���̕�
#define ITEM_HEIGHT_SELECT (ITEM_HEIGHT * 1.1) //��I���|���S���̍���
#define POLYGON_ALPHA (0.01f) //1f�ŕω�����A���t�@�l
#define ITEM_BITWEEN (0.07f) //�|���S���̊g�k��
#define ITEM_GRAW (0.05f) //�|���S���̊g�k��

//���j���[�̃t�@�C������ێ�
const char *c_apFilenameMenu[] =
{
	"data\\TEXTURE\\gamestart000.png",
	"data\\TEXTURE\\tutorial000.png",
	"data\\TEXTURE\\quit001.png",
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
	bool bSelect; //�I�����
}ITEM;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureMenuItem[ITEM_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenuItem = NULL;
int g_nState;
ITEM g_Item[ITEM_MAX];
float g_fItemAlpha;

//==========================================
//  ����������
//==========================================
void InitMenuItem()
{
	//���[�J���ϐ��錾
	int nState = GetMenuState();
	g_nState = ITEM_START;

	//�ϐ��̏�����
	for (int nCntBuff = 0; nCntBuff < ITEM_MAX; nCntBuff++)
	{
		g_Item[nCntBuff].pos = D3DXVECTOR3
		(
			SCREEN_WIDTH * 0.5f,
			TOP_POS + ((ITEM_HEIGHT * 2.0f) * nCntBuff),
			0.0f
		);
		if (nCntBuff == nState)
		{
			g_Item[nCntBuff].width = ITEM_WIDHT_SELECT;
			g_Item[nCntBuff].height = ITEM_HEIGHT_SELECT;
		}
		else
		{
			g_Item[nCntBuff].width = ITEM_WIDHT_NONE;
			g_Item[nCntBuff].height = ITEM_HEIGHT_NONE;
		}
		g_Item[nCntBuff].bSelect = false;
		g_Item[nCntBuff].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	g_fItemAlpha = POLYGON_ALPHA;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * ITEM_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMenuItem,
		NULL
	);

	//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < ITEM_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile
		(
			pDevice,
			c_apFilenameMenu[nCntTex],
			&g_pTextureMenuItem[nCntTex]
		);
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMenuItem->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntBuff = 0; nCntBuff < ITEM_MAX; nCntBuff++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//rhw�̐ݒ�
			pVtx[nCnt].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = g_Item[nCntBuff].col;
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
	g_pVtxBuffMenuItem->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMenuItem()
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		if (g_pTextureMenuItem[nCnt] != NULL)
		{
			g_pTextureMenuItem[nCnt]->Release();
			g_pTextureMenuItem[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMenuItem != NULL)
	{
		g_pVtxBuffMenuItem->Release();
		g_pVtxBuffMenuItem = NULL;
	}

}

//==========================================
//  �X�V����
//==========================================
void UpdateMenuItem()
{
	//���j���[�̑I����Ԃ��擾
	int nState = GetMenuState();

	//���j���[���ڂ̓_��
	if (g_Item[nState].col.a < 0.0f || g_Item[nState].col.a > 1.0f)
	{
		g_fItemAlpha *= -1.0f;
	}
	g_Item[nState].col.a += g_fItemAlpha;

	//�I���A��I�����̎d�l
	if (nState != MENUSTATE_FADE)
	{
		for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
		{
			if (nCnt == nState) //�I����
			{
				//�Ώۂ̃|���S�����g�傷��
				if (g_Item[nCnt].width < ITEM_WIDHT_SELECT)
				{
					g_Item[nCnt].width += g_Item[nCnt].width * ITEM_BITWEEN;
				}
				if (g_Item[nCnt].height < ITEM_HEIGHT_SELECT)
				{
					g_Item[nCnt].height += g_Item[nCnt].height * ITEM_BITWEEN;
				}
			}
			else //��I����
			{
				//�Ώۂ̃|���S�����k������
				if (g_Item[nCnt].width > ITEM_WIDHT_NONE)
				{
					g_Item[nCnt].width -= g_Item[nCnt].width * ITEM_BITWEEN;
				}
				if (g_Item[nCnt].height > ITEM_HEIGHT_NONE)
				{
					g_Item[nCnt].height -= g_Item[nCnt].height * ITEM_BITWEEN;
				}

				//���_�J���[��ffffff�ŌŒ肷��
				g_Item[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
	
	//�t�F�[�h���̋���
	//if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_START, 0) || GetMouseTrigger(MOUSEBUTTON_LEFT))
	if(GetEnterFrag())
	{
		//�I������t���O
		g_Item[g_nState].bSelect = true;
	}

	if(g_Item[g_nState].bSelect)
	{
		//�|���S���̊g��
		g_Item[g_nState].width += g_Item[g_nState].width * ITEM_GRAW;
		g_Item[g_nState].height += g_Item[g_nState].height * ITEM_GRAW;

		//�s�����x�̕ύX
		if (g_Item[g_nState].col.a > 0.0f)
		{
			g_Item[g_nState].col.a -= g_Item[g_nState].col.a * ITEM_GRAW * 3;
		}
		else
		{
			g_Item[g_nState].col.a = 0.0f;
		}
	}
	else
	{
		//�I����Ԃ�ύX
		if (GetKeyboardTrigger(DIK_W) || GetStickTriggerL(STICK_UP, 0) || GetJoyPadCrossTrigger(CROSS_UP, 0))
		{
			g_nState += ITEM_MAX - 1;
			g_nState %= ITEM_MAX;
		}
		if (GetKeyboardTrigger(DIK_S) || GetStickTriggerL(STICK_DOWN, 0) || GetJoyPadCrossTrigger(CROSS_DOWN, 0))
		{
			g_nState += 1;
			g_nState %= ITEM_MAX;
		}
	}

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMenuItem->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	for (int nCntBuff = 0; nCntBuff < ITEM_MAX; nCntBuff++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y - g_Item[nCntBuff].height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x - g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Item[nCntBuff].pos.x + g_Item[nCntBuff].width, g_Item[nCntBuff].pos.y + g_Item[nCntBuff].height, 0.0f);

		//���_�J���[�̐ݒ�
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			pVtx[nCnt].col = g_Item[nCntBuff].col;
		}

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMenuItem->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawMenuItem()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMenuItem, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMenuItem[nCnt]);

		//���j���[���ڂ̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
