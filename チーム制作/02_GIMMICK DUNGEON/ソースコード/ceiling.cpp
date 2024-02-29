//==================================
//
//�V��̏���(ceiling.cpp)
// Author: Kai Takada
//
//==================================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "ceiling.h"
#include "meshfield.h"

//==========================================
//�}�N����`
//==========================================
#define CEILING_TEX_U (300.0f)	//�e�N�X�`���̒���x
#define CEILING_TEX_V (300.0f)	//�e�N�X�`���̒���y

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureCeiling = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCeiling = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posCeiling;							//�ʒu
D3DXVECTOR3 g_rotCeiling;							//�ړ���
D3DXMATRIX g_mtxWorldCeiling;						//���[���h�}�g���b�N�X
float g_fHeightCeiling,g_fWidthCeiling;	//�c��

//==================================
//�V��̏���������
//==================================
void InitCeiling(void)
{
	//�e�N�X�`��������
	float fDivU, fDivV;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",				//�e�N�X�`���̃t�@�C����
		&g_pTextureCeiling);

	//�ϐ��̏�����
	g_posCeiling = D3DXVECTOR3(0.0f, CEILING_POSY, 0.0f);
	g_rotCeiling = D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f);
	g_fHeightCeiling = FIELD_RANGE * MESH_V;
	g_fWidthCeiling = FIELD_RANGE * MESH_U;
	fDivU = g_fWidthCeiling / CEILING_TEX_U;
	fDivV = g_fHeightCeiling / CEILING_TEX_V;
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCeiling,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCeiling->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, g_fHeightCeiling);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, g_fHeightCeiling);
	pVtx[2].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, -g_fHeightCeiling);
	pVtx[3].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, -g_fHeightCeiling);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);	//(x,y,z)
	pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
	pVtx[1].tex = D3DXVECTOR2(fDivU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fDivV);
	pVtx[3].tex = D3DXVECTOR2(fDivU, fDivV);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCeiling->Unlock();
}

//==================================
//�V��̏I������
//==================================
void UninitCeiling(void)
{
	if (g_pTextureCeiling != NULL)
	{
		g_pTextureCeiling->Release();
		g_pTextureCeiling = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffCeiling != NULL)
	{
		g_pVtxBuffCeiling->Release();
		g_pVtxBuffCeiling = NULL;					//�o�b�t�@�̔j��
	}
}

//==================================
//�V��̍X�V����
//==================================
void UpdateCeiling(void)
{

}

//==================================
//�V��̕`�揈��
//==================================
void DrawCeiling(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldCeiling);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotCeiling.y, g_rotCeiling.x, g_rotCeiling.z);

	D3DXMatrixMultiply(&g_mtxWorldCeiling, &g_mtxWorldCeiling, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posCeiling.x, g_posCeiling.y, g_posCeiling.z);

	D3DXMatrixMultiply(&g_mtxWorldCeiling, &g_mtxWorldCeiling, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldCeiling);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCeiling, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCeiling);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�`�悷��ŏ��̃C���f�b�N�X
		2);												//�`�悷��v���~�e�B�u��
}

//==================================
//�V��̐ݒu����
//==================================
void SetPosCeiling(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	//�e�N�X�`��������
	float fDivU, fDivV;

	g_posCeiling = pos;
	g_fWidthCeiling = fWidth;
	g_fHeightCeiling = fHeight;

	fDivU = fWidth / CEILING_TEX_U;
	fDivV = fHeight / CEILING_TEX_V;

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCeiling->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, g_fHeightCeiling);		//(x,y,z)
	pVtx[1].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, g_fHeightCeiling);
	pVtx[2].pos = D3DXVECTOR3(-g_fWidthCeiling, 0.0f, -g_fHeightCeiling);
	pVtx[3].pos = D3DXVECTOR3(g_fWidthCeiling, 0.0f, -g_fHeightCeiling);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
	pVtx[1].tex = D3DXVECTOR2(fDivU, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, fDivV);
	pVtx[3].tex = D3DXVECTOR2(fDivU, fDivV);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCeiling->Unlock();
}

//==================================
//�V��̈ʒu�擾����
//==================================
D3DXVECTOR3 GetCeilingPos(void)
{
	return g_posCeiling;
}
