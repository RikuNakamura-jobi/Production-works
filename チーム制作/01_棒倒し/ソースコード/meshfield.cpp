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
MESHFIELD g_meshField;

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

	g_meshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.Numx = 1;
	g_meshField.Numy = 1;

	g_meshField.NumVtx = (g_meshField.Numx + 1) * (g_meshField.Numy + 1);

	g_meshField.NumIdx = (((g_meshField.Numx + 2) * 2) * g_meshField.Numy);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_meshField.NumVtx,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_meshField.NumVtx; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -150.0f + (300.0f * (nCnt % (g_meshField.Numx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 1800.0f - (18000.0f * (nCnt / (g_meshField.Numy + 1)));

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_meshField.Numx + 1)), 0.5f * (nCnt / (g_meshField.Numy + 1)));

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_meshField.NumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_meshField.NumIdx / 2); nCnt++)
	{
		if (nCnt == (g_meshField.NumIdx / 2) - 1)
		{
			pIdx[nCnt * 2] = g_meshField.Numx + nIdx + (nIdx * g_meshField.Numx);
		}
		else if (nCnt % (g_meshField.Numx + 2) == g_meshField.Numx + 1)
		{
			pIdx[nCnt * 2] = g_meshField.Numx + nIdx + (nIdx * g_meshField.Numx);
			pIdx[(nCnt * 2) + 1] = (g_meshField.Numx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_meshField.Numx + 1;
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
	D3DXMatrixIdentity(&g_meshField.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_meshField.rot.y, g_meshField.rot.x, g_meshField.rot.z);
	D3DXMatrixMultiply(&g_meshField.mtxWorld, &g_meshField.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_meshField.pos.x, g_meshField.pos.y, g_meshField.pos.z);
	D3DXMatrixMultiply(&g_meshField.mtxWorld, &g_meshField.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_meshField.mtxWorld);
	
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
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_meshField.NumVtx, 0, g_meshField.NumIdx - 1);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void SetMeshField(void)
{
	g_meshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_meshField.Numx = 3;
	g_meshField.Numy = 3;

	g_meshField.NumVtx = (g_meshField.Numx + 1) * (g_meshField.Numy + 1);

	g_meshField.NumIdx = (((g_meshField.Numx + 2) * 2) * g_meshField.Numy);
}