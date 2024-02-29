//===========================
//
//�_�C���N�gX.meshwall�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "meshwall.h"
#include "input.h"

//�}�N����`
#define MAX_MESHWALL (4)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshWall[MAX_MESHWALL] = {};						//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;					//���_�����i�[
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Meshwall g_meshWall[MAX_MESHWALL] = {};
int g_nNumVtx;
int g_nNumIdx;

const char *c_apFilenameWall[MAX_MESHWALL] =
{
	"data\\TEXTURE\\aoharuBG002.png",
	"data\\TEXTURE\\aoharuBG001.png",
	"data\\TEXTURE\\aoharuBG003.png",
	"data\\TEXTURE\\aoharuBG000.png"
};

//===========================
//�v���C���[�̏���������
//===========================
void InitMeshWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameWall[nCnt],
			&g_pTextureMeshWall[nCnt]);
	}

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		g_meshWall[nCnt].rot.y = 0.0f + (1.57f * nCnt);

		if (g_meshWall[nCnt].rot.y >= 3.14f)
		{
			g_meshWall[nCnt].rot.y -= 6.28f;
		}
		else if (g_meshWall[nCnt].rot.y <= -3.14f)
		{
			g_meshWall[nCnt].rot.y += 6.28f;
		}

		g_meshWall[nCnt].pos.x = 1800.0f * sinf(g_meshWall[nCnt].rot.y);
		g_meshWall[nCnt].pos.y;
		g_meshWall[nCnt].pos.z = 1800.0f * cosf(g_meshWall[nCnt].rot.y);

		g_meshWall[nCnt].Numx = 2;
		g_meshWall[nCnt].Numy = 2;
		g_meshWall[nCnt].NumVtx = (g_meshWall[nCnt].Numx + 1) * (g_meshWall[nCnt].Numy + 1);
		g_meshWall[nCnt].NumIdx = (((g_meshWall[nCnt].Numx + 2) * 2) * g_meshWall[nCnt].Numy);
		g_meshWall[nCnt].fWallLength = 2699.0f;

		g_meshWall[nCnt].posVec[0].x = g_meshWall[nCnt].pos.x + (-2699.0f * cosf(g_meshWall[nCnt].rot.y));
		g_meshWall[nCnt].posVec[0].y = 0.0f;
		g_meshWall[nCnt].posVec[0].z = g_meshWall[nCnt].pos.z + (2699.0f * sinf(g_meshWall[nCnt].rot.y));

		g_meshWall[nCnt].posVec[1].x = g_meshWall[nCnt].pos.x + ((-2699.0f + (2700.0f * g_meshWall[nCnt].Numx)) * cosf(g_meshWall[nCnt].rot.y));
		g_meshWall[nCnt].posVec[1].y = 0.0f;
		g_meshWall[nCnt].posVec[1].z = g_meshWall[nCnt].pos.z + ((2699.0f - (2700.0f * g_meshWall[nCnt].Numx)) * sinf(g_meshWall[nCnt].rot.y));
	}

	g_nNumVtx = 0;
	g_nNumIdx = 0;

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		g_nNumVtx += g_meshWall[nCnt].NumVtx;
		g_nNumIdx += g_meshWall[nCnt].NumIdx;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVtx,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt1 = 0; nCnt1 < MAX_MESHWALL; nCnt1++)
	{
		for (int nCnt = 0; nCnt < g_meshWall[nCnt1].NumVtx; nCnt++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos.x = -2699.0f + (2700.0f * (nCnt % (g_meshWall[nCnt1].Numx + 1)));
			pVtx[0].pos.y = 1800.0f - (900.0f * (nCnt / (g_meshWall[nCnt1].Numy + 1)));
			pVtx[0].pos.z = 0.0f;

			//���_���W�̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.5f, -0.5f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx[0].tex.x = (1.0f / (float)(g_meshWall[nCnt1].Numx)) * (float)(nCnt % (g_meshWall[nCnt1].Numx + 1));
			pVtx[0].tex.y = (1.0f / (float)(g_meshWall[nCnt1].Numy)) * (float)(nCnt / (g_meshWall[nCnt1].Numy + 1));

			pVtx++;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshWall->Unlock();

	//�C���f�b�N�X�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	WORD *pIdx;				//�C���f�b�N�X���ւ̃|�C���^

							//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	int nVtxCnt = 0;
	int nIdxCnt = 0;

	for (int nCnt1 = 0; nCnt1 < MAX_MESHWALL; nCnt1++)
	{
		for (int nCnt = 0, nIdx = 0; nCnt < (g_meshWall[nCnt1].NumIdx / 2); nCnt++)
		{
			if (nCnt == (g_meshWall[nCnt1].NumIdx / 2) - 1)
			{
				pIdx[0] = g_meshWall[nCnt1].Numx + nIdx + (g_meshWall[nCnt1].Numx * nIdx) + nVtxCnt;
				pIdx += 2;
			}
			else if (nCnt % (g_meshWall[nCnt1].Numx + 2) == g_meshWall[nCnt1].Numx + 1)
			{
				pIdx[0] = g_meshWall[nCnt1].Numx + nIdx + (g_meshWall[nCnt1].Numx * nIdx) + nVtxCnt;
				pIdx[1] = (g_meshWall[nCnt1].Numx + 1) * (nIdx + 2) + nVtxCnt;
				nIdx++;
				pIdx += 2;
			}
			else
			{
				pIdx[0] = (nCnt - nIdx) + g_meshWall[nCnt1].Numx + 1 + nVtxCnt;
				pIdx[1] = nCnt - nIdx + nVtxCnt;
				pIdx += 2;
			}
		}

		nVtxCnt += g_meshWall[nCnt1].NumVtx;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshWall->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitMeshWall(void)
{
	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureMeshWall[nCnt] != NULL)
		{
			g_pTextureMeshWall[nCnt]->Release();
			g_pTextureMeshWall[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateMeshWall(void)
{
	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		g_meshWall[nCnt].pos.x = g_meshWall[nCnt].fWallLength * sinf(g_meshWall[nCnt].rot.y);
		g_meshWall[nCnt].pos.y;
		g_meshWall[nCnt].pos.z = g_meshWall[nCnt].fWallLength * cosf(g_meshWall[nCnt].rot.y);

		g_meshWall[nCnt].posVec[0].x = g_meshWall[nCnt].pos.x + (-2699.0f * cosf(g_meshWall[nCnt].rot.y));
		g_meshWall[nCnt].posVec[0].y = 0.0f;
		g_meshWall[nCnt].posVec[0].z = g_meshWall[nCnt].pos.z + (2699.0f * sinf(g_meshWall[nCnt].rot.y));

		g_meshWall[nCnt].posVec[1].x = g_meshWall[nCnt].pos.x + ((-2699.0f + (2700.0f * g_meshWall[nCnt].Numx)) * cosf(g_meshWall[nCnt].rot.y));
		g_meshWall[nCnt].posVec[1].y = 0.0f;
		g_meshWall[nCnt].posVec[1].z = g_meshWall[nCnt].pos.z + ((2699.0f - (2700.0f * g_meshWall[nCnt].Numx)) * sinf(g_meshWall[nCnt].rot.y));
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawMeshWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

										//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_meshWall[nCnt].mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_meshWall[nCnt].rot.y, g_meshWall[nCnt].rot.x, g_meshWall[nCnt].rot.z);
		D3DXMatrixMultiply(&g_meshWall[nCnt].mtxWorld, &g_meshWall[nCnt].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_meshWall[nCnt].pos.x, g_meshWall[nCnt].pos.y, g_meshWall[nCnt].pos.z);
		D3DXMatrixMultiply(&g_meshWall[nCnt].mtxWorld, &g_meshWall[nCnt].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_meshWall[nCnt].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0,
			g_pVtxBuffMeshWall,
			0,
			sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshWall);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMeshWall[nCnt]);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_meshWall[nCnt].NumVtx, 0, g_meshWall[nCnt].NumIdx - 1);
	}

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===========================
//�v���C���[�̕`�揈��
//===========================
bool CollisionMeshWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move)
{
	bool bMeshWall;
	int nCntCollision = 0;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fDot;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecMoveRef, vecToCross, vecCrossToPos;

	bMeshWall = false;

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		vecToPos = *pos - g_meshWall[nCnt].posVec[0];
		vecToPosOld = *posOld - g_meshWall[nCnt].posVec[0];
		vecMove = *posOld - *pos;
		vecLine = (g_meshWall[nCnt].posVec[1]) - (g_meshWall[nCnt].posVec[0]);
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
			pos->x = g_meshWall[nCnt].posVec[0].x + (vecLine.x * fRate);
			pos->z = g_meshWall[nCnt].posVec[0].z + (vecLine.z * fRate);
			/*move->x = vecMoveRef.x;
			move->z = vecMoveRef.z;*/
			bMeshWall = true;
		}
	}

	return bMeshWall;
}