//==================================================================================================
//
//meshfield.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "meshfield.h"
#include "input.h"

//�}�N����`
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\field_000.jpg"				//�e�N�X�`���t�@�C����
#define SPLIT_TEX					(20)										//�e�N�X�`��������
#define MAX_MESHFIELD				(3)										//�ő吔

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;									//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;								//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;								//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MeshField g_MeshField[MAX_MESHFIELD];											//�\���̂̏��

//==================================================================================================
//����������
//==================================================================================================
void InitMeshField(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MESHFIELD_TEX_FILE,
		&g_pTextureMeshField);

	for (int nCntMeshField = 0; nCntMeshField < MAX_MESHFIELD; nCntMeshField++)
	{
		g_MeshField[nCntMeshField].nNumVtx = (MESH_U + 1) * (MESH_V + 1);
		g_MeshField[nCntMeshField].bUse = false;

		//������������
		g_MeshField[nCntMeshField].rot = { 0.0f,0.0f,0.0f };

		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_MeshField[nCntMeshField].nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffMeshField,
			NULL);

		//���_���̃|�C���^
		VERTEX_3D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

		int nCountV, nCountU;

		for (nCountV = 0; nCountV < MESH_V + 1; nCountV++)
		{//���_���W�̐ݒ�
			for (nCountU = 0; nCountU < MESH_U + 1; nCountU++)
			{
				//���_���W
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.x = (nCountU - ((MESH_U) * 0.5f)) * FIELD_RANGE;
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.z = (((MESH_U) * 0.5f) - nCountV) * FIELD_RANGE;
				pVtx[nCountV * (MESH_U + 1) + nCountU].pos.y = 0.0f;

				//�e�N�X�`�����W
				pVtx[nCountV * (MESH_U + 1) + nCountU].tex = D3DXVECTOR2
				(
					((float)SPLIT_TEX / MESH_U) * nCountU,
					((float)SPLIT_TEX / MESH_V) * nCountV
				);

				//�@���x�N�g���̐ݒ�
				pVtx[nCountV * (MESH_U + 1) + nCountU].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			}
		}

		for (int nCnt = 0; nCnt < g_MeshField[nCntMeshField].nNumVtx; nCnt++)
		{
			//���_�J���[�̐ݒ�
			pVtx[nCnt].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffMeshField->Unlock();

		g_MeshField[nCntMeshField].nNumIdx = (MESH_U + 1) * (MESH_V + 1) + ((MESH_V + 1) - 2) * (MESH_U + 1) + (((MESH_V + 1) - 2) * 2);

		//�C���f�b�N�X�o�b�t�@�̐���
		pDevice->CreateIndexBuffer(sizeof(WORD) * g_MeshField[nCntMeshField].nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&g_pIdxBuffMeshField,
			NULL);

		//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
		WORD *pIdx;

		//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
		g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

		for (int nCount = 0; nCount < g_MeshField[nCntMeshField].nNumIdx / 2; nCount++)
		{//�C���f�b�N�X����
			if (
				nCount % ((MESH_U + 1) + (MESH_U + 2) * (nCount / (MESH_U + 2))) == 0
				&& nCount != 0
				)
			{
				pIdx[nCount * 2 + 1] = ((MESH_U + 1) * 2) + (MESH_U + 1) * (nCount / (MESH_U + 2));

				pIdx[nCount * 2] = MESH_U + (MESH_U + 1) * (nCount / (MESH_U + 2));
			}
			else
			{
				pIdx[nCount * 2 + 1] = nCount - (nCount / (MESH_U + 2));

				pIdx[nCount * 2] = (nCount - (nCount / (MESH_U + 2))) + (MESH_U + 1);
			}
		}

		//�C���f�b�N�X�o�b�t�@���A�����b�N
		g_pIdxBuffMeshField->Unlock();
	}

	switch (GetMode())
	{
	case MODE_RANKING:
		SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetMeshField(D3DXVECTOR3(1000.0f, 0.0f, 0.0f));
		SetMeshField(D3DXVECTOR3(2000.0f, 0.0f, 0.0f));
		break;

	case MODE_TITLE:
	case MODE_MENU:
		break;


	default:
		SetMeshField(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	}
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitMeshField(void)
{
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	if (g_pVtxBuffMeshField != NULL)
	{//���_�o�b�t�@�|�C���^�̔j��
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	if (g_pIdxBuffMeshField != NULL)
	{//�C���f�b�N�X�o�b�t�@�|�C���^�̔j��
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateMeshField(void)
{
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawMeshField(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntDraw = 0; nCntDraw < MAX_MESHFIELD; nCntDraw++)
	{
		if (g_MeshField[nCntDraw].bUse == true)
		{
			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_MeshField[nCntDraw].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_MeshField[nCntDraw].rot.y, g_MeshField[nCntDraw].rot.x, g_MeshField[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_MeshField[nCntDraw].mtxWorld, &g_MeshField[nCntDraw].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_MeshField[nCntDraw].pos.x, g_MeshField[nCntDraw].pos.y, g_MeshField[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_MeshField[nCntDraw].mtxWorld, &g_MeshField[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshField[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshField);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureMeshField);

			//�|���S���`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshField[nCntDraw].nNumVtx, 0, g_MeshField[nCntDraw].nNumIdx - 2);
		}
	}

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}

//==================================================================================================
//�����L���O�ł̍X�V����
//==================================================================================================
void UpdateMeshFieldRanking(float moveX)
{
	for (int nCntRank = 0; nCntRank < MAX_MESHFIELD; nCntRank++)
	{
		if (g_MeshField[nCntRank].bUse == true)
		{
			g_MeshField[nCntRank].pos.x += moveX;

			if (g_MeshField[nCntRank].pos.x <= -1000.0f)
			{
				g_MeshField[nCntRank].pos.x = 2000.0f;
			}
		}
	}
}

//==================================================================================================
//�ݒu����
//==================================================================================================
void SetMeshField(D3DXVECTOR3 pos)
{
	for (int nCntRank = 0; nCntRank < MAX_MESHFIELD; nCntRank++)
	{
		if (g_MeshField[nCntRank].bUse == false)
		{
			g_MeshField[nCntRank].pos = pos;
			g_MeshField[nCntRank].bUse = true;
			break;
		}
	}
}

