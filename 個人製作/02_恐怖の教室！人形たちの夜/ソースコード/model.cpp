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
#include "shadow.h"
#include "bullet.h"
#include "camera.h"
#include "meshwall.h"
#include "particle.h"
#include "effect.h"
#include "explosion.h"
#include <stdio.h>

//�}�N����`
#define MAX_MODEL_MODEL (64)

//�v���g�^�C�v�錾
void SetRotModel(void);

//�O���[�o���ϐ��錾
Model g_aModel[MAX_MODEL_MODEL];
int g_nIdxShadowModel = -1;
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

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		g_aModel[nCnt].state = MODELSTATE_NORMAL;
		g_aModel[nCnt].nTimer = 0;
		g_aModel[nCnt].nLife = 5;
		g_aModel[nCnt].nTimerState = 0;
	}

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelNameModel[g_aModel[nCnt].nType][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModel[nCnt].aModelX.pBuffMatModel,
			NULL,
			&g_aModel[nCnt].aModelX.dwNumMatModel,
			&g_aModel[nCnt].aModelX.pMeshModel);


		//���_���擾
		nNumVtx = g_aModel[nCnt].aModelX.pMeshModel->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCnt].aModelX.pMeshModel->GetFVF());

		//���_�o�b�t�@�����b�N
		g_aModel[nCnt].aModelX.pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= g_aModel[nCnt].aModelX.vtxMaxModel.x)
			{
				g_aModel[nCnt].aModelX.vtxMaxModel.x = vtx.x;
			}
			if (vtx.x <= g_aModel[nCnt].aModelX.vtxMinModel.x)
			{
				g_aModel[nCnt].aModelX.vtxMinModel.x = vtx.x;
			}

			if (vtx.y >= g_aModel[nCnt].aModelX.vtxMaxModel.y)
			{
				g_aModel[nCnt].aModelX.vtxMaxModel.y = vtx.y;
			}
			if (vtx.y <= g_aModel[nCnt].aModelX.vtxMinModel.y)
			{
				g_aModel[nCnt].aModelX.vtxMinModel.y = vtx.y;
			}

			if (vtx.z >= g_aModel[nCnt].aModelX.vtxMaxModel.z)
			{
				g_aModel[nCnt].aModelX.vtxMaxModel.z = vtx.z;
			}
			if (vtx.z <= g_aModel[nCnt].aModelX.vtxMinModel.z)
			{
				g_aModel[nCnt].aModelX.vtxMinModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_aModel[nCnt].aModelX.pMeshModel->UnlockVertexBuffer();

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModel[nCnt].aModelX.pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCnt].aModelX.dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aModel[nCnt].aModelX.apTextureModel[nCntMat]);
			}
		}
	}

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		g_aModel[nCnt].vtxMinModel = g_aModel[nCnt].pos + g_aModel[nCnt].aModelX.vtxMinModel;
		g_aModel[nCnt].vtxMaxModel = g_aModel[nCnt].pos + g_aModel[nCnt].aModelX.vtxMaxModel;

		/*g_aModel[nCnt].fLengthMin = sqrtf((g_aModel[nCnt].aModelX.vtxMinModel.x * g_aModel[nCnt].aModelX.vtxMinModel.x) +
		(g_aModel[nCnt].aModelX.vtxMinModel.y * g_aModel[nCnt].aModelX.vtxMinModel.y) +
		(g_aModel[nCnt].aModelX.vtxMinModel.z * g_aModel[nCnt].aModelX.vtxMinModel.z));
		g_aModel[nCnt].fLengthMax = sqrtf((g_aModel[nCnt].aModelX.vtxMaxModel.x * g_aModel[nCnt].aModelX.vtxMaxModel.x) +
		(g_aModel[nCnt].aModelX.vtxMaxModel.y * g_aModel[nCnt].aModelX.vtxMaxModel.y) +
		(g_aModel[nCnt].aModelX.vtxMaxModel.z * g_aModel[nCnt].aModelX.vtxMaxModel.z));

		g_aModel[nCnt].vtxMinModel.x = g_aModel[nCnt].pos.x + (sinf(g_aModel[nCnt].rot.z) * sinf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMin;
		g_aModel[nCnt].vtxMinModel.y = g_aModel[nCnt].pos.y + (cosf(g_aModel[nCnt].rot.z) * g_aModel[nCnt].fLengthMin);
		g_aModel[nCnt].vtxMinModel.z = g_aModel[nCnt].pos.z + (sinf(g_aModel[nCnt].rot.z) * cosf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMin;

		g_aModel[nCnt].vtxMaxModel.x = g_aModel[nCnt].pos.x + (sinf(g_aModel[nCnt].rot.z) * sinf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMax;
		g_aModel[nCnt].vtxMaxModel.y = g_aModel[nCnt].pos.y + (cosf(g_aModel[nCnt].rot.z) * g_aModel[nCnt].fLengthMax);
		g_aModel[nCnt].vtxMaxModel.z = g_aModel[nCnt].pos.z + (sinf(g_aModel[nCnt].rot.z) * cosf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMax;*/
	}

	g_nIdxShadowModel = SetShadow(30.0f);
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitModel(void)
{
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_aModel[nCnt].aModelX.pMeshModel != NULL)
		{
			g_aModel[nCnt].aModelX.pMeshModel->Release();
			g_aModel[nCnt].aModelX.pMeshModel = NULL;
		}

		//���_�o�b�t�@�̔j��
		if (g_aModel[nCnt].aModelX.pBuffMatModel != NULL)
		{
			g_aModel[nCnt].aModelX.pBuffMatModel->Release();
			g_aModel[nCnt].aModelX.pBuffMatModel = NULL;
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
			if (g_aModel[nCnt].nLife <= 0)
			{
				g_aModel[nCnt].nLife = 5;
				g_aModel[nCnt].bUse = false;
			}

			if (GetKeyboardPress(DIK_K) == true)
			{//W�L�[�������ꂽ��
				g_aModel[nCnt].rot.y -= 0.01f;
			}
			if (GetKeyboardPress(DIK_L) == true)
			{//W�L�[�������ꂽ��
				g_aModel[nCnt].rot.y += 0.01f;
			}

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_nIdxShadowModel, g_aModel[nCnt].pos);
		}
		else
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
		//�_���[�W��Ԕ���
		switch (g_aModel[nCnt].state)
		{
		case MODELSTATE_NORMAL:
			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModel[nCnt].aModelX.pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[nCnt].aModelX.dwNumMatModel; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}

			break;
		case MODELSTATE_DAMAGE:
			g_aModel[nCnt].nTimerState--;

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModel[nCnt].aModelX.pBuffMatModel->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[nCnt].aModelX.dwNumMatModel; nCntMat++)
			{
				pMat->MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				pMat++;
			}

			if (g_aModel[nCnt].nTimerState <= 0)
			{
				g_aModel[nCnt].state = MODELSTATE_NORMAL;
			}
			break;
		}

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

		if (g_aModel[nCnt].bUse == true)
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
void SetModel(void)
{
	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		//x�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aModelNameModel[g_aModel[nCnt].nType][0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModel[nCnt].aModelX.pBuffMatModel,
			NULL,
			&g_aModel[nCnt].aModelX.dwNumMatModel,
			&g_aModel[nCnt].aModelX.pMeshModel);


		//���_���擾
		nNumVtx = g_aModel[nCnt].aModelX.pMeshModel->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aModel[nCnt].aModelX.pMeshModel->GetFVF());

		//���_�o�b�t�@�����b�N
		g_aModel[nCnt].aModelX.pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= g_aModel[nCnt].aModelX.vtxMaxModel.x)
			{
				g_aModel[nCnt].aModelX.vtxMaxModel.x = vtx.x;
			}
			if (vtx.x <= g_aModel[nCnt].aModelX.vtxMinModel.x)
			{
				g_aModel[nCnt].aModelX.vtxMinModel.x = vtx.x;
			}

			if (vtx.y >= g_aModel[nCnt].aModelX.vtxMaxModel.y)
			{
				g_aModel[nCnt].aModelX.vtxMaxModel.y = vtx.y;
			}
			if (vtx.y <= g_aModel[nCnt].aModelX.vtxMinModel.y)
			{
				g_aModel[nCnt].aModelX.vtxMinModel.y = vtx.y;
			}

			if (vtx.z >= g_aModel[nCnt].aModelX.vtxMaxModel.z)
			{
				g_aModel[nCnt].aModelX.vtxMaxModel.z = vtx.z;
			}
			if (vtx.z <= g_aModel[nCnt].aModelX.vtxMinModel.z)
			{
				g_aModel[nCnt].aModelX.vtxMinModel.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_aModel[nCnt].aModelX.pMeshModel->UnlockVertexBuffer();

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModel[nCnt].aModelX.pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModel[nCnt].aModelX.dwNumMatModel; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aModel[nCnt].aModelX.apTextureModel[nCntMat]);
			}
		}
	}

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		g_aModel[nCnt].point = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCnt].state = MODELSTATE_NORMAL;
		g_aModel[nCnt].nTimer = 0;
		g_aModel[nCnt].nLife = 5;
		g_aModel[nCnt].nTimerState = 0;
		g_aModel[nCnt].vtxMinModel = g_aModel[nCnt].pos + g_aModel[g_aModel[nCnt].nType].aModelX.vtxMinModel;
		g_aModel[nCnt].vtxMaxModel = g_aModel[nCnt].pos + g_aModel[g_aModel[nCnt].nType].aModelX.vtxMaxModel;

		/*g_aModel[nCnt].fLengthMin = sqrtf((g_aModel[nCnt].aModelX.vtxMinModel.x * g_aModel[nCnt].aModelX.vtxMinModel.x) +
		(g_aModel[nCnt].aModelX.vtxMinModel.y * g_aModel[nCnt].aModelX.vtxMinModel.y) +
		(g_aModel[nCnt].aModelX.vtxMinModel.z * g_aModel[nCnt].aModelX.vtxMinModel.z));
		g_aModel[nCnt].fLengthMax = sqrtf((g_aModel[nCnt].aModelX.vtxMaxModel.x * g_aModel[nCnt].aModelX.vtxMaxModel.x) +
		(g_aModel[nCnt].aModelX.vtxMaxModel.y * g_aModel[nCnt].aModelX.vtxMaxModel.y) +
		(g_aModel[nCnt].aModelX.vtxMaxModel.z * g_aModel[nCnt].aModelX.vtxMaxModel.z));

		g_aModel[nCnt].vtxMinModel.x = g_aModel[nCnt].pos.x + (sinf(g_aModel[nCnt].rot.z) * sinf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMin;
		g_aModel[nCnt].vtxMinModel.y = g_aModel[nCnt].pos.y + (cosf(g_aModel[nCnt].rot.z) * g_aModel[nCnt].fLengthMin);
		g_aModel[nCnt].vtxMinModel.z = g_aModel[nCnt].pos.z + (sinf(g_aModel[nCnt].rot.z) * cosf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMin;

		g_aModel[nCnt].vtxMaxModel.x = g_aModel[nCnt].pos.x + (sinf(g_aModel[nCnt].rot.z) * sinf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMax;
		g_aModel[nCnt].vtxMaxModel.y = g_aModel[nCnt].pos.y + (cosf(g_aModel[nCnt].rot.z) * g_aModel[nCnt].fLengthMax);
		g_aModel[nCnt].vtxMaxModel.z = g_aModel[nCnt].pos.z + (sinf(g_aModel[nCnt].rot.z) * cosf(g_aModel[nCnt].rot.y)) * g_aModel[nCnt].fLengthMax;*/

		g_aModel[nCnt].bUse = false;
	}

	g_aModel[0].Parent = 0;

	g_nIdxShadowModel = SetShadow(30.0f);
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
bool CollisionModel(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType)
{
	bool bMeshWall;
	int nCntCollision = 0;
	float fRate[4], fOutToPos[4], fOutToPosOld, fOutUnit, fOutRate;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecCross;

	bMeshWall = false;

	for (int nCnt = 0; nCnt < MAX_MODEL_MODEL; nCnt++)
	{
		if (g_aModel[nCnt].bUse == true)
		{
			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				if (nCnt1 == 0)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].vtxMinModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].vtxMinModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].vtxMinModel;
					vecLine.x = g_aModel[nCnt].vtxMaxModel.x - g_aModel[nCnt].vtxMinModel.x;
					vecLine.z = g_aModel[nCnt].vtxMinModel.z - g_aModel[nCnt].vtxMinModel.z;
				}
				else if (nCnt1 == 1)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].vtxMaxModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].vtxMinModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].vtxMinModel;
					vecLine.x = g_aModel[nCnt].vtxMaxModel.x - g_aModel[nCnt].vtxMaxModel.x;
					vecLine.z = g_aModel[nCnt].vtxMaxModel.z - g_aModel[nCnt].vtxMinModel.z;
				}
				else if (nCnt1 == 2)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].vtxMaxModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].vtxMaxModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].vtxMinModel;
					vecLine.x = g_aModel[nCnt].vtxMinModel.x - g_aModel[nCnt].vtxMaxModel.x;
					vecLine.z = g_aModel[nCnt].vtxMaxModel.z - g_aModel[nCnt].vtxMaxModel.z;
				}
				else if (nCnt1 == 3)
				{
					vecToPos.x = pos->x - g_aModel[nCnt].vtxMinModel.x;
					vecToPos.z = pos->z - g_aModel[nCnt].vtxMaxModel.z;
					vecToPosOld = *posOld - g_aModel[nCnt].vtxMinModel;
					vecLine.x = g_aModel[nCnt].vtxMinModel.x - g_aModel[nCnt].vtxMinModel.x;
					vecLine.z = g_aModel[nCnt].vtxMinModel.z - g_aModel[nCnt].vtxMaxModel.z;
				}

				vecMove = *pos - *posOld;
				fOutToPos[nCnt1] = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
				fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate[nCnt1] = fOutRate / fOutUnit;
			}

			if ((fOutToPos[0] < 0.0f && fOutToPos[1] < 0.0f && fOutToPos[2] < 0.0f && fOutToPos[3] < 0.0f) &&
				pos->y <= g_aModel[nCnt].vtxMaxModel.y && pos->y >= g_aModel[nCnt].vtxMinModel.y)
			{
				bMeshWall = true;
			}
		}
	}

	return bMeshWall;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void SetRotModel(void)
{
	float fRotMove, fRotDest, fRotDiff;

	fRotMove = g_aModel[0].rot.y;
	fRotDest = g_aModel[0].point.y;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	fRotMove += fRotDiff * 0.2f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	g_aModel[0].rot.y = fRotMove;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void ReadFileModel(void)
{
	int nNumModel;
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

	fscanf(pFile, "%s = %d\n", &aText[0], &nNumModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		fscanf(pFile, "%s = %s		%s %s\n", &aText[0], &g_aModelNameModel[nCnt][0], &aText[0], &aText[0]);
	}

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	fscanf(pFile, "%s = %d\n", &aText[0], &nNumModel);

	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}

	for (int nCnt = 0; nCnt < nNumModel; nCnt++)
	{
		fscanf(pFile, "%s", &aText[0]);
		fscanf(pFile, "%s = %d", &aText[0], &g_aModel[nCnt].nType);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aModel[nCnt].pos.x, &g_aModel[nCnt].pos.y, &g_aModel[nCnt].pos.z);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &g_aModel[nCnt].rot.x, &g_aModel[nCnt].rot.y, &g_aModel[nCnt].rot.z);
		fscanf(pFile, "%s\n", &aText[0]);

		g_aModel[nCnt].bUse = true;
	}

	fclose(pFile);
}