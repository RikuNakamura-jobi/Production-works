//===========================
//
//�_�C���N�gX.skyceiling�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "skyceiling.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureSkyCeiling = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkyCeiling = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkyCeiling = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posSkyCeiling;
D3DXVECTOR3 g_rotSkyCeiling;
D3DXMATRIX g_mtxWorldSkyCeiling;		//���[���h�}�g���b�N�X
int g_NumSkyCeilingx;
int g_NumSkyCeilingy;
int g_NumVtxSkyCeiling;
int g_NumIdxSkyCeiling;

//===========================
//�v���C���[�̏���������
//===========================
void InitSkyCeiling(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky001.jpg",
		&g_pTextureSkyCeiling);

	g_posSkyCeiling = D3DXVECTOR3(0.0f, 3600.0f, 0.0f);
	g_rotSkyCeiling = D3DXVECTOR3(3.14f, 0.0f, 0.0f);
	g_NumSkyCeilingx = 1;
	g_NumSkyCeilingy = 1;

	g_NumVtxSkyCeiling = (g_NumSkyCeilingx + 1) * (g_NumSkyCeilingy + 1);

	g_NumIdxSkyCeiling = (((g_NumSkyCeilingx + 2) * 2) * g_NumSkyCeilingy);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxSkyCeiling,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkyCeiling,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSkyCeiling->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxSkyCeiling; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -3600.0f + (18000.0f * (nCnt % (g_NumSkyCeilingx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 3600.0f - (18000.0f * (nCnt / (g_NumSkyCeilingy + 1)));

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumSkyCeilingx + 1)), 0.5f * (nCnt / (g_NumSkyCeilingy + 1)));

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSkyCeiling->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxSkyCeiling,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSkyCeiling,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffSkyCeiling->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxSkyCeiling / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxSkyCeiling / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumSkyCeilingx + nIdx + (nIdx * g_NumSkyCeilingx);
		}
		else if (nCnt % (g_NumSkyCeilingx + 2) == g_NumSkyCeilingx + 1)
		{
			pIdx[nCnt * 2] = g_NumSkyCeilingx + nIdx + (nIdx * g_NumSkyCeilingx);
			pIdx[(nCnt * 2) + 1] = (g_NumSkyCeilingx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumSkyCeilingx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffSkyCeiling->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitSkyCeiling(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureSkyCeiling != NULL)
	{
		g_pTextureSkyCeiling->Release();
		g_pTextureSkyCeiling = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSkyCeiling != NULL)
	{
		g_pVtxBuffSkyCeiling->Release();
		g_pVtxBuffSkyCeiling = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffSkyCeiling != NULL)
	{
		g_pIdxBuffSkyCeiling->Release();
		g_pIdxBuffSkyCeiling = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateSkyCeiling(void)
{

}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawSkyCeiling(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldSkyCeiling);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotSkyCeiling.y, g_rotSkyCeiling.x, g_rotSkyCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyCeiling, &g_mtxWorldSkyCeiling, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posSkyCeiling.x, g_posSkyCeiling.y, g_posSkyCeiling.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyCeiling, &g_mtxWorldSkyCeiling, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSkyCeiling);
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffSkyCeiling,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffSkyCeiling);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureSkyCeiling);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxSkyCeiling, 0, g_NumIdxSkyCeiling - 1);
}