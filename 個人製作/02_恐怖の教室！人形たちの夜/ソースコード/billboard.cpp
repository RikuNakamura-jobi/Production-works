//===========================
//
//�_�C���N�gX.billboard�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "billboard.h"
#include "camera.h"
#include "player.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;					//���_�����i�[
D3DXVECTOR3 g_posBillboard;
D3DXMATRIX g_mtxWorldBillboard;		//���[���h�}�g���b�N�X
bool g_bBillboard;
int nBillboardCount;

//�e�N�X�`���t�@�C����
const char *c_apFilenameBoard[2] =
{
	"data\\TEXTURE\\life001.png",
	"data\\TEXTURE\\life001.png",
};

//===========================
//�v���C���[�̏���������
//===========================
void InitBillboard(void)
{
	MODE mode = GetMode();

	g_bBillboard = true;
	nBillboardCount = 0;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		c_apFilenameBoard[1],
		&g_pTextureBillboard);

	g_posBillboard = D3DXVECTOR3(-2200.0f, 35.0f, -300.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = -25.0f;
	pVtx[0].pos.y = 25.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 25.0f;
	pVtx[1].pos.y = 25.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -25.0f;
	pVtx[2].pos.y = -25.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 25.0f;
	pVtx[3].pos.y = -25.0f;
	pVtx[3].pos.z = 0.0f;

	//���_���W�̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBillboard->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitBillboard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateBillboard(void)
{
	if (g_bBillboard == true)
	{
		if (CollisionPlayer(&g_posBillboard, &g_posBillboard, &D3DXVECTOR3(0.0f, 0.0f, -0.0f), 3) == true)
		{
			g_bBillboard = false;
		}
	}
	else
	{
		nBillboardCount++;

		if (nBillboardCount >= 300)
		{
			g_bBillboard = true;
			nBillboardCount = 0;
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X�擾�p

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	if (g_bBillboard == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldBillboard);

		//�r���[�}�g���b�N�X���擾
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
		D3DXMatrixInverse(&g_mtxWorldBillboard, NULL, &mtxView);
		g_mtxWorldBillboard._41 = 0.0f;
		g_mtxWorldBillboard._42 = 0.0f;
		g_mtxWorldBillboard._43 = 0.0f;

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_posBillboard.x, g_posBillboard.y, g_posBillboard.z);
		D3DXMatrixMultiply(&g_mtxWorldBillboard, &g_mtxWorldBillboard, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBillboard);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffBillboard,
			0,
			sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBillboard);


		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}