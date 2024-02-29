//===========================
//
//�_�C���N�gX.Meshfield�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "meshfield.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshField;
D3DXVECTOR3 g_rotMeshField;
D3DXMATRIX g_mtxWorldMeshField;		//���[���h�}�g���b�N�X
int g_NumMeshFieldx;
int g_NumMeshFieldy;
int g_NumVtxMeshField;
int g_NumIdxMeshField;

//===========================
//�v���C���[�̏���������
//===========================
void InitMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\field000.png",
		&g_pTextureMeshField);

	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_NumMeshFieldx = 3;
	g_NumMeshFieldy = 3;

	g_NumVtxMeshField = (g_NumMeshFieldx + 1) * (g_NumMeshFieldy + 1);

	g_NumIdxMeshField = (((g_NumMeshFieldx + 2) * 2) * g_NumMeshFieldy);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxMeshField,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshField; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -2700.0f + (1800.0f * (nCnt % (g_NumMeshFieldx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 2700.0f - (1800.0f * (nCnt / (g_NumMeshFieldy + 1)));

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumMeshFieldx + 1)), 0.5f * (nCnt / (g_NumMeshFieldy + 1)));

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxMeshField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxMeshField / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxMeshField / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumMeshFieldx + nIdx + (nIdx * g_NumMeshFieldx);
		}
		else if (nCnt % (g_NumMeshFieldx + 2) == g_NumMeshFieldx + 1)
		{
			pIdx[nCnt * 2] = g_NumMeshFieldx + nIdx + (nIdx * g_NumMeshFieldx);
			pIdx[(nCnt * 2) + 1] = (g_NumMeshFieldx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumMeshFieldx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshField->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateMeshField(void)
{

}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffMeshField,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxMeshField, 0, g_NumIdxMeshField - 1);
}