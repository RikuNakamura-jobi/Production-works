//===========================
//
//�_�C���N�gX.skywall�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "skywall.h"
#include "input.h"

//�}�N����`
#define MAX_SKYWALL (4)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureSkywall[MAX_SKYWALL] = {};						//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkywall = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSkywall = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Skywall g_Skywall[MAX_SKYWALL];
int g_nNumVtxSky;
int g_nNumIdxSky;

const char *c_apFilenameSky[MAX_SKYWALL] =
{
	"data\\TEXTURE\\sky001.jpg",
	"data\\TEXTURE\\sky001.jpg",
	"data\\TEXTURE\\sky001.jpg",
	"data\\TEXTURE\\sky001.jpg"
};

//===========================
//�v���C���[�̏���������
//===========================
void InitSkywall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSky[nCnt],
			&g_pTextureSkywall[nCnt]);
	}

	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		g_Skywall[nCnt].rot.y = 0.0f + (1.57f * nCnt);

		if (g_Skywall[nCnt].rot.y > 3.14f)
		{
			g_Skywall[nCnt].rot.y -= 6.28f;
		}
		else if (g_Skywall[nCnt].rot.y <= -3.14f)
		{
			g_Skywall[nCnt].rot.y += 6.28f;
		}

		g_Skywall[nCnt].pos.x = 3600.0f * sinf(g_Skywall[nCnt].rot.y);
		g_Skywall[nCnt].pos.y;
		g_Skywall[nCnt].pos.z = 3600.0f * cosf(g_Skywall[nCnt].rot.y);

		g_Skywall[nCnt].Numx = 2;
		g_Skywall[nCnt].Numy = 2;
		g_Skywall[nCnt].NumVtx = (g_Skywall[nCnt].Numx + 1) * (g_Skywall[nCnt].Numy + 1);
		g_Skywall[nCnt].NumIdx = (((g_Skywall[nCnt].Numx + 2) * 2) * g_Skywall[nCnt].Numy);
		g_Skywall[nCnt].fWallLength = 3600.0f;

		g_Skywall[nCnt].posVec[0].x = g_Skywall[nCnt].pos.x + (-3600.0f * cosf(g_Skywall[nCnt].rot.y));
		g_Skywall[nCnt].posVec[0].y = 0.0f;
		g_Skywall[nCnt].posVec[0].z = g_Skywall[nCnt].pos.z + (3600.0f * sinf(g_Skywall[nCnt].rot.y));

		g_Skywall[nCnt].posVec[1].x = g_Skywall[nCnt].pos.x + ((-3600.0f + (7200.0f * g_Skywall[nCnt].Numx)) * cosf(g_Skywall[nCnt].rot.y));
		g_Skywall[nCnt].posVec[1].y = 0.0f;
		g_Skywall[nCnt].posVec[1].z = g_Skywall[nCnt].pos.z + ((3600.0f - (7200.0f * g_Skywall[nCnt].Numx)) * sinf(g_Skywall[nCnt].rot.y));
	}

	g_nNumVtxSky = 0;
	g_nNumIdxSky = 0;

	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		g_nNumVtxSky += g_Skywall[nCnt].NumVtx;
		g_nNumIdxSky += g_Skywall[nCnt].NumIdx;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVtxSky,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkywall,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSkywall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_SKYWALL; nCnt1++)
	{
		for (int nCnt = 0; nCnt < g_Skywall[nCnt1].NumVtx; nCnt++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos.x = -3600.0f + (5400.0f * (nCnt % (g_Skywall[nCnt1].Numx + 1)));
			pVtx[0].pos.y = 3600.0f - (3600.0f * (nCnt / (g_Skywall[nCnt1].Numy + 1)));
			pVtx[0].pos.z = 0.0f;

			//���_���W�̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.5f, -0.5f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			pVtx[0].tex.x = (1.0f / (float)(g_Skywall[nCnt1].Numx)) * (float)(nCnt % (g_Skywall[nCnt1].Numx + 1));
			pVtx[0].tex.y = (1.0f / (float)(g_Skywall[nCnt1].Numy)) * (float)(nCnt / (g_Skywall[nCnt1].Numy + 1));

			pVtx++;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSkywall->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIdxSky,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSkywall,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffSkywall->Lock(0, 0, (void**)&pIdx, 0);

	int nVtxCnt = 0;
	int nIdxCnt = 0;

	for (int nCnt1 = 0; nCnt1 < MAX_SKYWALL; nCnt1++)
	{
		for (int nCnt = 0, nIdx = 0; nCnt < (g_Skywall[nCnt1].NumIdx / 2); nCnt++)
		{
			if (nCnt == (g_Skywall[nCnt1].NumIdx / 2) - 1)
			{
				pIdx[0] = g_Skywall[nCnt1].Numx + nIdx + (g_Skywall[nCnt1].Numx * nIdx) + nVtxCnt;
				pIdx += 2;
			}
			else if (nCnt % (g_Skywall[nCnt1].Numx + 2) == g_Skywall[nCnt1].Numx + 1)
			{
				pIdx[0] = g_Skywall[nCnt1].Numx + nIdx + (g_Skywall[nCnt1].Numx * nIdx) + nVtxCnt;
				pIdx[1] = (g_Skywall[nCnt1].Numx + 1) * (nIdx + 2) + nVtxCnt;
				nIdx++;
				pIdx += 2;
			}
			else
			{
				pIdx[0] = (nCnt - nIdx) + g_Skywall[nCnt1].Numx + 1 + nVtxCnt;
				pIdx[1] = nCnt - nIdx + nVtxCnt;
				pIdx += 2;
			}
		}

		nVtxCnt += g_Skywall[nCnt1].NumVtx;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffSkywall->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitSkywall(void)
{
	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSkywall[nCnt] != NULL)
		{
			g_pTextureSkywall[nCnt]->Release();
			g_pTextureSkywall[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSkywall != NULL)
	{
		g_pVtxBuffSkywall->Release();
		g_pVtxBuffSkywall = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffSkywall != NULL)
	{
		g_pIdxBuffSkywall->Release();
		g_pIdxBuffSkywall = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateSkywall(void)
{
	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		if (g_Skywall[nCnt].rot.y > 3.14f)
		{
			g_Skywall[nCnt].rot.y -= 6.28f;
		}
		else if (g_Skywall[nCnt].rot.y <= -3.14f)
		{
			g_Skywall[nCnt].rot.y += 6.28f;
		}

		g_Skywall[nCnt].pos.x = g_Skywall[nCnt].fWallLength * sinf(g_Skywall[nCnt].rot.y);
		g_Skywall[nCnt].pos.y;
		g_Skywall[nCnt].pos.z = g_Skywall[nCnt].fWallLength * cosf(g_Skywall[nCnt].rot.y);

		g_Skywall[nCnt].posVec[0].x = g_Skywall[nCnt].pos.x + (-3600.0f * cosf(g_Skywall[nCnt].rot.y));
		g_Skywall[nCnt].posVec[0].y = 0.0f;
		g_Skywall[nCnt].posVec[0].z = g_Skywall[nCnt].pos.z + (3600.0f * sinf(g_Skywall[nCnt].rot.y));

		g_Skywall[nCnt].posVec[1].x = g_Skywall[nCnt].pos.x + ((-3600.0f + (1800.0f * g_Skywall[nCnt].Numx)) * cosf(g_Skywall[nCnt].rot.y));
		g_Skywall[nCnt].posVec[1].y = 0.0f;
		g_Skywall[nCnt].posVec[1].z = g_Skywall[nCnt].pos.z + ((3600.0f - (1800.0f * g_Skywall[nCnt].Numx)) * sinf(g_Skywall[nCnt].rot.y));
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawSkywall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Skywall[nCnt].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_Skywall[nCnt].rot.y, g_Skywall[nCnt].rot.x, g_Skywall[nCnt].rot.z);
		D3DXMatrixMultiply(&g_Skywall[nCnt].mtxWorld, &g_Skywall[nCnt].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_Skywall[nCnt].pos.x, g_Skywall[nCnt].pos.y, g_Skywall[nCnt].pos.z);
		D3DXMatrixMultiply(&g_Skywall[nCnt].mtxWorld, &g_Skywall[nCnt].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Skywall[nCnt].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffSkywall,
			0,
			sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffSkywall);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSkywall[nCnt]);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_Skywall[nCnt].NumVtx, 0, g_Skywall[nCnt].NumIdx - 1);
	}

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
bool CollisionSkywall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move)
{
	bool bSkywall;
	int nCntCollision = 0;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fDot;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecMoveRef, vecToCross, vecCrossToPos;

	bSkywall = false;

	for (int nCnt = 0; nCnt < MAX_SKYWALL; nCnt++)
	{
		vecToPos = *pos - g_Skywall[nCnt].posVec[0];
		vecToPosOld = *posOld - g_Skywall[nCnt].posVec[0];
		vecMove = *posOld - *pos;
		vecLine = (g_Skywall[nCnt].posVec[1]) - (g_Skywall[nCnt].posVec[0]);
		fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
		fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);
		fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
		fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
		fRate = fOutRate / fOutUnit;

		vecNor.x = vecLine.x * cosf(-1.57f);
		vecNor.y = 0.0f;
		vecNor.z = vecLine.z * sinf(-1.57f);

		D3DXVec3Normalize(&vecNor, &vecNor);

		fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);
		vecMoveRef.x = vecMove.x + ((vecNor.x * fDot) * 2);
		vecMoveRef.z = vecMove.z + ((vecNor.z * fDot) * 2);

		if (fOutToPos <= 0.0f && fOutToPos * fOutToPosOld <= 0.0f && fRate > 0.0f && fRate < 1.0f)
		{
			pos->x = g_Skywall[nCnt].posVec[0].x + (vecLine.x * fRate);
			pos->z = g_Skywall[nCnt].posVec[0].z + (vecLine.z * fRate);
			/*move->x = vecMoveRef.x;
			move->z = vecMoveRef.z;*/
			bSkywall = true;
		}
	}

	return bSkywall;
}