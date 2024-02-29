//===========================
//
//�_�C���N�gX.Meshceiling�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "meshceiling.h"
#include "game.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshCeiling = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCeiling = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCeiling = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshCeiling;
D3DXVECTOR3 g_rotMeshCeiling;
D3DXCOLOR g_colMeshCeiling;
D3DXMATRIX g_mtxWorldMeshCeiling;		//���[���h�}�g���b�N�X
int g_NumMeshCeilingx;
int g_NumMeshCeilingy;
int g_NumVtxMeshCeiling;
int g_NumIdxMeshCeiling;

//===========================
//�v���C���[�̏���������
//===========================
void InitMeshCeiling(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ceiling000.png",
		&g_pTextureMeshCeiling);

	g_posMeshCeiling = D3DXVECTOR3(0.0f, 1800.0f, 0.0f);
	g_rotMeshCeiling = D3DXVECTOR3(3.14f, 0.0f, 0.0f);
	g_colMeshCeiling = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_NumMeshCeilingx = 3;
	g_NumMeshCeilingy = 3;

	g_NumVtxMeshCeiling = (g_NumMeshCeilingx + 1) * (g_NumMeshCeilingy + 1);

	g_NumIdxMeshCeiling = (((g_NumMeshCeilingx + 2) * 2) * g_NumMeshCeilingy);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxMeshCeiling,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCeiling,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCeiling->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshCeiling; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -2700.0f + (1800.0f * (nCnt % (g_NumMeshCeilingx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 2700.0f - (1800.0f * (nCnt / (g_NumMeshCeilingy + 1)));

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_colMeshCeiling;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumMeshCeilingx + 1)), 0.5f * (nCnt / (g_NumMeshCeilingy + 1)));

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCeiling->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxMeshCeiling,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCeiling,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCeiling->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxMeshCeiling / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxMeshCeiling / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCeilingx + nIdx + (nIdx * g_NumMeshCeilingx);
		}
		else if (nCnt % (g_NumMeshCeilingx + 2) == g_NumMeshCeilingx + 1)
		{
			pIdx[nCnt * 2] = g_NumMeshCeilingx + nIdx + (nIdx * g_NumMeshCeilingx);
			pIdx[(nCnt * 2) + 1] = (g_NumMeshCeilingx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumMeshCeilingx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshCeiling->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitMeshCeiling(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshCeiling != NULL)
	{
		g_pTextureMeshCeiling->Release();
		g_pTextureMeshCeiling = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshCeiling != NULL)
	{
		g_pVtxBuffMeshCeiling->Release();
		g_pVtxBuffMeshCeiling = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCeiling != NULL)
	{
		g_pIdxBuffMeshCeiling->Release();
		g_pIdxBuffMeshCeiling = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateMeshCeiling(void)
{
	if (GetGameState() == GAMESTATE_BOSS || GetGameState() == GAMESTATE_BOSSBATTLE)
	{
		if (g_colMeshCeiling.r >= 0.0f)
		{
			g_colMeshCeiling.r -= 0.01f;
		}
		if (g_colMeshCeiling.g >= 0.0f)
		{
			g_colMeshCeiling.g -= 0.01f;
		}
		if (g_colMeshCeiling.b >= 0.0f)
		{
			g_colMeshCeiling.b -= 0.01f;
		}
	}
	else
	{
		g_colMeshCeiling.r = 1.0f;
		g_colMeshCeiling.g = 1.0f;
		g_colMeshCeiling.b = 1.0f;
	}

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshCeiling->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxMeshCeiling; nCnt++)
	{
		//���_�J���[�̐ݒ�
		pVtx[0].col = g_colMeshCeiling;

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshCeiling->Unlock();
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawMeshCeiling(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshCeiling);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotMeshCeiling.y, g_rotMeshCeiling.x, g_rotMeshCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCeiling, &g_mtxWorldMeshCeiling, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posMeshCeiling.x, g_posMeshCeiling.y, g_posMeshCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldMeshCeiling, &g_mtxWorldMeshCeiling, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshCeiling);
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffMeshCeiling,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCeiling);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCeiling);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxMeshCeiling, 0, g_NumIdxMeshCeiling - 1);
}