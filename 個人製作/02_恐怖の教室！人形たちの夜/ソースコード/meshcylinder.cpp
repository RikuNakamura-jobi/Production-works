//===========================
//
//�_�C���N�gX.Meshcylinder�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "meshcylinder.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshCylinder;
D3DXVECTOR3 g_rotMeshCylinder;
D3DXMATRIX g_mtxWorldMeshCylinder;		//���[���h�}�g���b�N�X
int g_NumMeshCylinderx;
int g_NumMeshCylindery;
int g_NumVtxMeshCylinder;
int g_NumIdxMeshCylinder;

//===========================
//�v���C���[�̏���������
//===========================
void InitMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureMeshCylinder);

	g_posMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshCylinder = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_NumMeshCylinderx = 3;
	g_NumMeshCylindery = 3;

	g_NumVtxMeshCylinder = (g_NumMeshCylinderx + 1) * (g_NumMeshCylindery + 1);

	g_NumIdxMeshCylinder = (((g_NumMeshCylinderx + 2) * 2) * g_NumMeshCylindery);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxMeshCylinder,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshCylinder; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -1800.0f + (1800.0f * (nCnt % (g_NumMeshCylinderx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 1800.0f - (1800.0f * (nCnt / (g_NumMeshCylindery + 1)));

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumMeshCylinderx + 1)), 0.5f * (nCnt / (g_NumMeshCylindery + 1)));

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCylinder->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxMeshCylinder,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxMeshCylinder / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxMeshCylinder / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCylinderx + nIdx + (nIdx * g_NumMeshCylinderx);
		}
		else if (nCnt % (g_NumMeshCylinderx + 2) == g_NumMeshCylinderx + 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCylinderx + nIdx + (nIdx * g_NumMeshCylinderx);
			pIdx[(nCnt * 2) + 1] = (g_NumMeshCylinderx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumMeshCylinderx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCylinder->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitMeshCylinder(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateMeshCylinder(void)
{

}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshCylinder);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCylinder, &g_mtxWorldMeshCylinder, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCylinder);
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffMeshCylinder,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxMeshCylinder, 0, g_NumIdxMeshCylinder - 1);
}