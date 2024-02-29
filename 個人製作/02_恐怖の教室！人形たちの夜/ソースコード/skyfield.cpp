//===========================
//
//�_�C���N�gX.skyfield�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "skyfield.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureSkyField = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkyField = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkyField = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posSkyField;
D3DXVECTOR3 g_rotSkyField;
D3DXMATRIX g_mtxWorldSkyField;		//���[���h�}�g���b�N�X
int g_NumSkyFieldx;
int g_NumSkyFieldy;
int g_NumVtxSkyField;
int g_NumIdxSkyField;

//===========================
//�v���C���[�̏���������
//===========================
void InitSkyField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\sky001.jpg",
		&g_pTextureSkyField);

	g_posSkyField = D3DXVECTOR3(0.0f, -180.0f, 0.0f);
	g_rotSkyField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_NumSkyFieldx = 1;
	g_NumSkyFieldy = 1;

	g_NumVtxSkyField = (g_NumSkyFieldx + 1) * (g_NumSkyFieldy + 1);

	g_NumIdxSkyField = (((g_NumSkyFieldx + 2) * 2) * g_NumSkyFieldy);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_NumVtxSkyField,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkyField,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSkyField->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < g_NumVtxSkyField; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -3600.0f + (18000.0f * (nCnt % (g_NumSkyFieldx + 1)));
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 3600.0f - (18000.0f * (nCnt / (g_NumSkyFieldy + 1)));

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * (nCnt % (g_NumSkyFieldx + 1)), 0.5f * (nCnt / (g_NumSkyFieldy + 1)));

		pVtx++;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSkyField->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_NumIdxSkyField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSkyField,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffSkyField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0, nIdx = 0; nCnt < (g_NumIdxSkyField / 2); nCnt++)
	{
		if (nCnt == (g_NumIdxSkyField / 2) - 1)
		{
			pIdx[nCnt * 2] = g_NumSkyFieldx + nIdx + (nIdx * g_NumSkyFieldx);
		}
		else if (nCnt % (g_NumSkyFieldx + 2) == g_NumSkyFieldx + 1)
		{
			pIdx[nCnt * 2] = g_NumSkyFieldx + nIdx + (nIdx * g_NumSkyFieldx);
			pIdx[(nCnt * 2) + 1] = (g_NumSkyFieldx + 1) * (nIdx + 2);
			nIdx++;
		}
		else
		{
			pIdx[nCnt * 2] = (nCnt - nIdx) + g_NumSkyFieldx + 1;
			pIdx[(nCnt * 2) + 1] = nCnt - nIdx;
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffSkyField->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitSkyField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureSkyField != NULL)
	{
		g_pTextureSkyField->Release();
		g_pTextureSkyField = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSkyField != NULL)
	{
		g_pVtxBuffSkyField->Release();
		g_pVtxBuffSkyField = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffSkyField != NULL)
	{
		g_pIdxBuffSkyField->Release();
		g_pIdxBuffSkyField = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateSkyField(void)
{

}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawSkyField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldSkyField);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_rotSkyField.y, g_rotSkyField.x, g_rotSkyField.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyField, &g_mtxWorldSkyField, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posSkyField.x, g_posSkyField.y, g_posSkyField.z);
	D3DXMatrixMultiply(&g_mtxWorldSkyField, &g_mtxWorldSkyField, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldSkyField);
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffSkyField,
		0,
		sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffSkyField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureSkyField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_NumVtxSkyField, 0, g_NumIdxSkyField - 1);
}