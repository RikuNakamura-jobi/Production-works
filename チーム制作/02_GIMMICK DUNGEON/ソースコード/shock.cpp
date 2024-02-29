//===========================
//
//�_�C���N�gX.shock�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "shock.h"
#include "input.h"
#include "camera.h"
#include "camera.h"
#include "meshwall.h"
#include <stdio.h>
#include "score.h"
#include "game.h"
#include "point.h"
#include "point_log.h"
#include "tutorial.h"
#include "sound.h"
#include "count.h"

//�}�N����`
#define MAX_SHOCK (32)
#define SHOCK_SCORE (300)			//��щz�������̃X�R�A

//�O���[�o���ϐ��錾
Shock g_aShock[MAX_SHOCK];
ShockX g_aShockX;

//===========================
//�Ռ��g�̏���������
//===========================
void InitShock(void)
{
	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATERIAL *pMat;

	//x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\shock_000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aShockX.pBuffMatShock,
		NULL,
		&g_aShockX.dwNumMatShock,
		&g_aShockX.pMeshShock);

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		g_aShock[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShock[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShock[nCnt].nLife = 0;
		g_aShock[nCnt].fRadiusMax = 0.0f;
		g_aShock[nCnt].fRadiusMin = 0.0f;
		g_aShock[nCnt].fRadSpeed = 0.0f;
		g_aShock[nCnt].bUse = false;

		//���_���擾
		nNumVtx = g_aShockX.pMeshShock->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		dwSizeFVF = D3DXGetFVFVertexSize(g_aShockX.pMeshShock->GetFVF());

		//���_�o�b�t�@�����b�N
		g_aShockX.pMeshShock->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (vtx.x >= g_aShockX.vtxMaxShock.x)
			{
				g_aShockX.vtxMaxShock.x = vtx.x;
			}
			if (vtx.x <= g_aShockX.vtxMinShock.x)
			{
				g_aShockX.vtxMinShock.x = vtx.x;
			}

			if (vtx.y >= g_aShockX.vtxMaxShock.y)
			{
				g_aShockX.vtxMaxShock.y = vtx.y;
			}
			if (vtx.y <= g_aShockX.vtxMinShock.y)
			{
				g_aShockX.vtxMinShock.y = vtx.y;
			}

			if (vtx.z >= g_aShockX.vtxMaxShock.z)
			{
				g_aShockX.vtxMaxShock.z = vtx.z;
			}
			if (vtx.z <= g_aShockX.vtxMinShock.z)
			{
				g_aShockX.vtxMinShock.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;
		}

		//���_�o�b�t�@���A�����b�N
		g_aShockX.pMeshShock->UnlockVertexBuffer();

		//�}�e���A�����ɑ΂���|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aShockX.pBuffMatShock->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aShockX.dwNumMatShock; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aShockX.apTextureShock[nCntMat]);
			}
		}

		g_aShock[nCnt].mtxScale = g_aShock[nCnt].mtxWorld;
	}
}

//===========================
//�Ռ��g�̏I������
//===========================
void UninitShock(void)
{
	//���b�V���̔j��
	if (g_aShockX.pMeshShock != NULL)
	{
		g_aShockX.pMeshShock->Release();
		g_aShockX.pMeshShock = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_aShockX.pBuffMatShock != NULL)
	{
		g_aShockX.pBuffMatShock->Release();
		g_aShockX.pBuffMatShock = NULL;
	}

	for (int nCntTex = 0; nCntTex < NUM_TEX_SHOCK;nCntTex++)
	{//�e�N�X�`���̔j��
		if (g_aShockX.apTextureShock[nCntTex] != NULL)
		{
			g_aShockX.apTextureShock[nCntTex]->Release();
			g_aShockX.apTextureShock[nCntTex] = NULL;
		}
	}
}

//===========================
//�Ռ��g�̍X�V����
//===========================
void UpdateShock(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		if (g_aShock[nCnt].bUse == true)
		{
			//�O��̍ő�E�ŏ����a��ۑ�
			g_aShock[nCnt].fRadiusMaxOld = g_aShock[nCnt].fRadiusMax;
			g_aShock[nCnt].fRadiusMinOld = g_aShock[nCnt].fRadiusMin;

			g_aShock[nCnt].nLife--;

			g_aShock[nCnt].fRadiusMax += g_aShockX.vtxMaxShock.x * g_aShock[nCnt].fRadSpeed;
			g_aShock[nCnt].fRadiusMin += g_aShockX.vtxMaxShock.x * g_aShock[nCnt].fRadSpeed;

			g_aShock[nCnt].mtxScale._11 += g_aShock[nCnt].fRadSpeed;
			g_aShock[nCnt].mtxScale._33 += g_aShock[nCnt].fRadSpeed;

			if (g_aShock[nCnt].nLife < 0)
			{
				g_aShock[nCnt].bUse = false;
			}
		}
	}
}

//===========================
//�Ռ��g�̕`�揈��
//===========================
void DrawShock(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aShock[nCnt].mtxWorld);

		g_aShock[nCnt].mtxWorld._11 = g_aShock[nCnt].mtxScale._11;
		g_aShock[nCnt].mtxWorld._33 = g_aShock[nCnt].mtxScale._33;

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aShock[nCnt].rot.y, g_aShock[nCnt].rot.x, g_aShock[nCnt].rot.z);
		D3DXMatrixMultiply(&g_aShock[nCnt].mtxWorld, &g_aShock[nCnt].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_aShock[nCnt].pos.x, g_aShock[nCnt].pos.y, g_aShock[nCnt].pos.z);
		D3DXMatrixMultiply(&g_aShock[nCnt].mtxWorld, &g_aShock[nCnt].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aShock[nCnt].mtxWorld);

		if (g_aShock[nCnt].bUse == true)
		{
			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���̃f�[�^�̃|�C���g���擾
			pMat = (D3DXMATERIAL*)g_aShockX.pBuffMatShock->GetBufferPointer();

			pMat->MatD3D.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
			pMat->MatD3D.Emissive = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);

			for (int nCntMat = 0; nCntMat < (int)g_aShockX.dwNumMatShock; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aShockX.apTextureShock[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aShockX.pMeshShock->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//===========================
//�Ռ��g�̐ݒ菈��
//===========================
void SetShock(D3DXVECTOR3 pos, int life, float speed)
{
	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		if (g_aShock[nCnt].bUse == false)
		{
			g_aShock[nCnt].pos = pos;
			g_aShock[nCnt].nLife = life;
			g_aShock[nCnt].fRadiusMax = g_aShockX.vtxMaxShock.x + 10.0f;
			g_aShock[nCnt].fRadiusMin = g_aShockX.vtxMaxShock.x - 10.0f;
			g_aShock[nCnt].fRadSpeed = speed;
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShock[nCnt].mtxWorld);
			g_aShock[nCnt].mtxScale = g_aShock[nCnt].mtxWorld;
			g_aShock[nCnt].bUse = true;

			break;
		}
	}
}

//===========================
//�Ռ��g�̎擾����
//===========================
Shock *GetShock(void)
{
	return &g_aShock[0];
}

//===========================
//�Ռ��g�̓����蔻�菈��
//===========================
bool CollisionShock(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot,bool bJump)
{
	bool bMeshWall;
	float fLength;
	float fLengthOld;

	bMeshWall = false;

	for (int nCnt = 0; nCnt < MAX_SHOCK; nCnt++)
	{
		if (g_aShock[nCnt].bUse == true)
		{
			//�v���C���[�ƃG�l�~�[�̋����v�Z
			fLength = hypotf(pos->x - g_aShock[nCnt].pos.x,pos->z - g_aShock[nCnt].pos.z);

			fLengthOld = hypotf(posOld->x - g_aShock[nCnt].pos.x, posOld->z - g_aShock[nCnt].pos.z);

			if (fLength < g_aShock[nCnt].fRadiusMax && fLength > g_aShock[nCnt].fRadiusMin && pos->y <= g_aShock[nCnt].pos.y + g_aShockX.vtxMaxShock.y)
			{
				if (fabsf(fLength - g_aShock[nCnt].fRadiusMax) <= fabsf(fLength - g_aShock[nCnt].fRadiusMin))
				{
					rot->y = atan2f(pos->x - g_aShock[nCnt].pos.x, pos->z - g_aShock[nCnt].pos.z);
				}
				else if (fabsf(fLength - g_aShock[nCnt].fRadiusMax) > fabsf(fLength - g_aShock[nCnt].fRadiusMin))
				{
					rot->y = atan2f(g_aShock[nCnt].pos.x - pos->x, g_aShock[nCnt].pos.z - pos->z);
				}

				bMeshWall = true;
			}
			if (pos->y >= g_aShock[nCnt].pos.y + g_aShockX.vtxMaxShock.y
				&& fLength < g_aShock[nCnt].fRadiusMax
				&& fLengthOld > g_aShock[nCnt].fRadiusMaxOld)
			{
				if (bJump == true)
				{
					//�X�R�A�̑���
					AddScore(SHOCK_SCORE);

					//�擾�X�R�A�\���ݒ�
					SetPointScore(SHOCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_DODGE_SHOCK);
				}

				if (GetMode() == MODE_TUTORIAL)
				{
					AddCount(1);
				}
			}
			if (pos->y >= g_aShock[nCnt].pos.y + g_aShockX.vtxMaxShock.y
				&& fLength > g_aShock[nCnt].fRadiusMin
				&& fLengthOld < g_aShock[nCnt].fRadiusMinOld)
			{//��������O��
				if (bJump == true)
				{
					//�X�R�A�̑���
					AddScore(SHOCK_SCORE);

					//�擾�X�R�A�\���ݒ�
					SetPointScore(SHOCK_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_DODGE_SHOCK);
				}
			}
		}
	}

	return bMeshWall;
}