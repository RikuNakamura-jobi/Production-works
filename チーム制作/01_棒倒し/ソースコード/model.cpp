//===========================
//
//�_�C���N�gX.model�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include <stdio.h>

//�}�N����`
#define MAX_MODEL_MODEL (128)

//�O���[�o���ϐ��錾
Model g_aModel[MAX_MODEL_MODEL];
Model g_aModelSet[MAX_MODEL_MODEL];
Model g_aModelSetTutorial[MAX_MODEL_MODEL];
ModelX g_aModelX[MAX_MODEL_MODEL];
int g_nIdxShadowModel = -1;
int g_nNumModel;
int g_nNumModeltutorial;
int g_nNumSetModel;
int g_nNumSetModeltutorial;
char g_aModelNameModel[32][128];

//===========================
//�v���C���[�̏���������
//===========================
void InitModel(void)
{
	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < g_nNumModel; nCnt++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelNameModel[nCnt][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelX[nCnt].pBuffMatModel,
			NULL,
			&g_aModelX[nCnt].dwNumMatModel,
			&g_aModelX[nCnt].pMeshModel);

		//���_���擾
		nNumVtx = g_aModelX[nCnt].pMeshModel->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModelX[nCnt].pMeshModel->GetFVF());

		//���_�o�b�t�@�����b�N
		g_aModelX[nCnt].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= g_aModelX[nCnt].vtxMaxModel.x)
			{
				g_aModelX[nCnt].vtxMaxModel.x = vtx.x;
			}
			if (vtx.x <= g_aModelX[nCnt].vtxMinModel.x)
			{
				g_aModelX[nCnt].vtxMinModel.x = vtx.x;
			}

			if (vtx.y >= g_aModelX[nCnt].vtxMaxModel.y)
			{
				g_aModelX[nCnt].vtxMaxModel.y = vtx.y;
			}
			if (vtx.y <= g_aModelX[nCnt].vtxMinModel.y)
			{
				g_aModelX[nCnt].vtxMinModel.y = vtx.y;
			}

			if (vtx.z >= g_aModelX[nCnt].vtxMaxModel.z)
			{
				g_aModelX[nCnt].vtxMaxModel.z = vtx.z;
			}
			if (vtx.z <= g_aModelX[nCnt].vtxMinModel.z)
			{
				g_aModelX[nCnt].vtxMinModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_aModelX[nCnt].pMeshModel->UnlockVertexBuffer();

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModelX[nCnt].pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelX[nCnt].dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aModelX[nCnt].apTextureModel[nCntMat]);
			}
		}
	}
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_aModelX[nCnt].pMeshModel != NULL)
		{
			g_aModelX[nCnt].pMeshModel->Release();
			g_aModelX[nCnt].pMeshModel = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_aModelX[nCnt].pBuffMatModel != NULL)
		{
			g_aModelX[nCnt].pBuffMatModel->Release();
			g_aModelX[nCnt].pBuffMatModel = NULL;
		}
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse == true)
		{

		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aModel[nCnt].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aModel[nCnt].rot.y, g_aModel[nCnt].rot.x, g_aModel[nCnt].rot.z);
		D3DXMatrixMultiply(&g_aModel[nCnt].mtxWorld, &g_aModel[nCnt].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_aModel[nCnt].pos.x, g_aModel[nCnt].pos.y, g_aModel[nCnt].pos.z);
		D3DXMatrixMultiply(&g_aModel[nCnt].mtxWorld, &g_aModel[nCnt].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCnt].mtxWorld);

		if (g_aModel[nCnt].bDraw == true)
		{
			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���̃f�[�^�̃|�C���g���擾
			pMat = (D3DXMATERIAL*)g_aModel[nCnt].aModelX.pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[nCnt].aModelX.dwNumMatModel; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aModel[nCnt].aModelX.apTextureModel[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aModel[nCnt].aModelX.pMeshModel->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nType)
{
	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse == false)
		{
			g_aModel[nCnt].pos = pos;
			g_aModel[nCnt].move = move;
			g_aModel[nCnt].rot = rot;
			g_aModel[nCnt].nType = nType;

			for (int nCnt1 = 0; nCnt1 < 8; nCnt1++)
			{
				g_aModel[nCnt].aModelX.apTextureModel[nCnt1] = g_aModelX[g_aModel[nCnt].nType].apTextureModel[nCnt1];
			}

			g_aModel[nCnt].aModelX.dwNumMatModel = g_aModelX[g_aModel[nCnt].nType].dwNumMatModel;
			g_aModel[nCnt].aModelX.pBuffMatModel = g_aModelX[g_aModel[nCnt].nType].pBuffMatModel;
			g_aModel[nCnt].aModelX.pMeshModel = g_aModelX[g_aModel[nCnt].nType].pMeshModel;
			g_aModel[nCnt].aModelX.vtxMinModel = g_aModel[nCnt].pos + g_aModelX[g_aModel[nCnt].nType].vtxMinModel;
			g_aModel[nCnt].aModelX.vtxMaxModel = g_aModel[nCnt].pos + g_aModelX[g_aModel[nCnt].nType].vtxMaxModel;

			g_aModel[nCnt].bUse = true;
			g_aModel[nCnt].bDraw = true;

			break;
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ResetModel(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		g_aModelSet[MAX_MODEL_MODEL].move = {};
		g_aModel[nCnt].bUse = false;
		g_aModel[nCnt].bDraw = false;
	}

	for (int nCnt = 0; nCnt < g_nNumSetModel; nCnt++)
	{
		SetModel(g_aModelSet[nCnt].pos, g_aModelSet[nCnt].move, g_aModelSet[nCnt].rot, g_aModelSet[nCnt].nType);
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ResetModelTutorial(void)
{
	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		g_aModelSetTutorial[MAX_MODEL_MODEL].move = {};
		g_aModel[nCnt].bUse = false;
		g_aModel[nCnt].bDraw = false;
	}

	for (int nCnt = 0; nCnt < g_nNumSetModeltutorial; nCnt++)
	{
		SetModel(g_aModelSetTutorial[nCnt].pos, g_aModelSetTutorial[nCnt].move, g_aModelSetTutorial[nCnt].rot, g_aModelSetTutorial[nCnt].nType);
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
Model *GetModel(void)
{
	return &g_aModel[0];
}

//===========================
//�v���C���[�̕`�揈��
//===========================
int CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType)
{
	int nMeshWall;
	int nCntCollision = 0;
	float fRate[4], fOutToPos[4], fOutToPosOld, fOutUnit, fOutRate;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecCross;

	nMeshWall = -1;

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse == true)
		{
			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				if (nCnt1 == 0)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].aModelX.vtxMinModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].aModelX.vtxMinModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].aModelX.vtxMinModel;
					vecLine.x = g_aModel[nCnt].aModelX.vtxMaxModel.x - g_aModel[nCnt].aModelX.vtxMinModel.x;
					vecLine.z = g_aModel[nCnt].aModelX.vtxMinModel.z - g_aModel[nCnt].aModelX.vtxMinModel.z;
				}
				else if (nCnt1 == 1)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].aModelX.vtxMaxModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].aModelX.vtxMinModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].aModelX.vtxMinModel;
					vecLine.x = g_aModel[nCnt].aModelX.vtxMaxModel.x - g_aModel[nCnt].aModelX.vtxMaxModel.x;
					vecLine.z = g_aModel[nCnt].aModelX.vtxMaxModel.z - g_aModel[nCnt].aModelX.vtxMinModel.z;
				}
				else if (nCnt1 == 2)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].aModelX.vtxMaxModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].aModelX.vtxMaxModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].aModelX.vtxMinModel;
					vecLine.x = g_aModel[nCnt].aModelX.vtxMinModel.x - g_aModel[nCnt].aModelX.vtxMaxModel.x;
					vecLine.z = g_aModel[nCnt].aModelX.vtxMaxModel.z - g_aModel[nCnt].aModelX.vtxMaxModel.z;
				}
				else if (nCnt1 == 3)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].aModelX.vtxMinModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].aModelX.vtxMaxModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].aModelX.vtxMinModel;
					vecLine.x = g_aModel[nCnt].aModelX.vtxMinModel.x - g_aModel[nCnt].aModelX.vtxMinModel.x;
					vecLine.z = g_aModel[nCnt].aModelX.vtxMinModel.z - g_aModel[nCnt].aModelX.vtxMaxModel.z;
				}

				vecMove = *pos - *posOld;
				fOutToPos[nCnt1] = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
				fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate[nCnt1] = fOutRate / fOutUnit;
			}

			if ((fOutToPos[0] < 0.0f && fOutToPos[1] < 0.0f && fOutToPos[2] < 0.0f && fOutToPos[3] < 0.0f) &&
				pos->y <= g_aModel[nCnt].aModelX.vtxMaxModel.y && pos->y >= g_aModel[nCnt].aModelX.vtxMinModel.y)
			{
				if (g_aModel[nCnt].nType != 6 && g_aModel[nCnt].nType != 0)
				{
					g_aModel[nCnt].rot.x = -1.57f;
					g_aModel[nCnt].bUse = false;
				}
				else if (g_aModel[nCnt].nType == 0)
				{
					g_aModel[nCnt].rot.x = -1.57f;
				}

				nMeshWall = g_aModel[nCnt].nType;
			}
		}
	}

	return nMeshWall;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ReadFileModel(void)
{
	char aText[128];
	FILE *pFile = fopen("data\\MODEL\\model.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < 11; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_nNumModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_nNumModel; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aModelNameModel[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_nNumSetModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_nNumSetModel; nCnt++)
	{
		fscanf(pFile, "%s", &aText[0]);
		fscanf(pFile, "%s = %d", &aText[0], &g_aModelSet[nCnt].nType);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aModelSet[nCnt].pos.x, &g_aModelSet[nCnt].pos.y, &g_aModelSet[nCnt].pos.z);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aModelSet[nCnt].rot.x, &g_aModelSet[nCnt].rot.y, &g_aModelSet[nCnt].rot.z);
		fscanf(pFile, "%s\n", &aText[0]);

		g_aModelSet[nCnt].bUse = true;
	}

	fclose(pFile);

	pFile = fopen("data\\MODEL\\modeltutorial.txt", "r");

	if (pFile == NULL)
	{
		return;
	}

	for (int nCnt = 0; nCnt < 11; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_nNumModeltutorial);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_nNumModeltutorial; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aModelNameModel[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &g_nNumSetModeltutorial);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < g_nNumSetModeltutorial; nCnt++)
	{
		fscanf(pFile, "%s", &aText[0]);
		fscanf(pFile, "%s = %d", &aText[0], &g_aModelSetTutorial[nCnt].nType);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aModelSetTutorial[nCnt].pos.x, &g_aModelSetTutorial[nCnt].pos.y, &g_aModelSetTutorial[nCnt].pos.z);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aModelSetTutorial[nCnt].rot.x, &g_aModelSetTutorial[nCnt].rot.y, &g_aModelSetTutorial[nCnt].rot.z);
		fscanf(pFile, "%s\n", &aText[0]);

		g_aModelSet[nCnt].bUse = true;
	}

	fclose(pFile);
}