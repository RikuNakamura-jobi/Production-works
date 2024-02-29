//==========================================
//
//  �t�B�[���h�̐���(field.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "field.h"

//==========================================
//  �}�N����`
//==========================================
#define FIELD_POS (D3DXVECTOR3(0.0f, 600.0f, -1650.0f)) //���̈ʒu
#define FIELD_SIZE (150.0f) //�t�B�[���h�̑傫��
#define FIELD_SIZE_Z (1700.0f) //�t�B�[���h�̑傫��

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;		//�e�N�X�`���ւ̃|�C���^ 
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX g_FieldmtxWorld;		//���[���h�}�g���b�N�X

//==========================================
//  ����������
//==========================================
void InitField()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/field_000.jpg",
		&g_pTextureField
	);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField,
		NULL
	);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, FIELD_SIZE);
	pVtx[1].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, FIELD_SIZE);
	pVtx[2].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, -FIELD_SIZE_Z);
	pVtx[3].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, -FIELD_SIZE_Z);

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffField->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitField()
{
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateField()
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawField()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_FieldmtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_FieldmtxWorld, &g_FieldmtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, FIELD_POS.x, FIELD_POS.y, FIELD_POS.z);
	D3DXMatrixMultiply(&g_FieldmtxWorld, &g_FieldmtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_FieldmtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureField);

	//�|���S���̕`��
	pDevice->DrawPrimitive
	(
		D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,									//�`�悷��ŏ��̃C���f�b�N�X
		2
	);												//�`�悷��v���~�e�B�u��

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
