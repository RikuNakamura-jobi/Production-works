//==========================================
//
//  �O�Ղ̏���
//  Author : ���R����
//
//==========================================
#include "orbit.h"
#include "effect.h"
#include "input.h"

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureOrbit = NULL;		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOrbit = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXMATRIX g_OrbitmtxWorld;		//���[���h�}�g���b�N�X
Orbit g_aOrbit[MAX_ORBIT];	//�\���̂̏��

//==========================================
//  ����������
//==========================================
void InitOrbit()
{
	//�ϐ�������
	ZeroMemory(&g_aOrbit[0],sizeof(Orbit) * MAX_ORBIT);

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile
	(
		pDevice,
		"data/TEXTURE/orbit_000.png",
		&g_pTextureOrbit
	);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * MAX_EDGE * NUM_OFFSET * MAX_ORBIT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOrbit,
		NULL
	);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0;nCntVtx < MAX_EDGE * NUM_OFFSET * MAX_ORBIT;nCntVtx++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f * (nCntVtx % NUM_OFFSET));

		pVtx += 1;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOrbit->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitOrbit()
{
	if (g_pTextureOrbit != NULL)
	{//�e�N�X�`���j��
		g_pTextureOrbit->Release();
		g_pTextureOrbit = NULL;
	}
	if (g_pVtxBuffOrbit != NULL)
	{//���_���j��
		g_pVtxBuffOrbit->Release();
		g_pVtxBuffOrbit = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateOrbit(void)
{
}

//==========================================
//  �|���S���X�V����
//==========================================
void UpdateOrbitPolygon(int nCntOrbit)
{
	//�ϐ��錾
	int nCntOffset;
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DXVECTOR3 posTemp[NUM_OFFSET];

	//���擾
	Orbit *pOrbit = GetOrbit();

	//�������Ƃ���܂Ń|�C���^��i�߂�
	pOrbit += nCntOrbit;

	//�ۑ��������W�����炷==========
	for (int nCntVtx = 1; nCntVtx < pOrbit->nNumEdge; nCntVtx++)
	{
		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{
			//��O�̍��W�ɂ����
			pOrbit->aPosPoint[nCntVtx - 1][nCntOffset] = pOrbit->aPosPoint[nCntVtx][nCntOffset];
		}
	}

	for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
	{
		//���݂̃t���[���̃I�t�Z�b�g�ʒu��ۑ�
		pOrbit->aPosPoint[pOrbit->nNumEdge - 1][nCntOffset] =
		{
			pOrbit->mtxWorld[nCntOffset]._41,
			pOrbit->mtxWorld[nCntOffset]._42,
			pOrbit->mtxWorld[nCntOffset]._43
		};
	}
	//�ۑ��������W�����炷==========

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffOrbit->Lock(0, 0, (void**)&pVtx, 0);

	//�ŏ��̃|�C���^�܂Ői�߂�
	pVtx += MAX_EDGE * NUM_OFFSET * nCntOrbit;

	for (int nCntVtx = 0; nCntVtx < pOrbit->nNumEdge; nCntVtx++)
	{//�ӂ��Ƃ̒��_���W�ݒ�

		for (nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{//�I�t�Z�b�g�̐����ݒ�

			 //���_���W�̐ݒ�
			pVtx[nCntOffset].pos = pOrbit->aPosPoint[nCntVtx][nCntOffset];

			//���_�J���[�̐ݒ�
			pVtx[nCntOffset].col = D3DXCOLOR(pOrbit->col.r, pOrbit->col.g, pOrbit->col.b, (float)nCntVtx / pOrbit->nNumEdge);
		}

		//�|�C���^��i�߂�
		pVtx += NUM_OFFSET;
	}
}

//==========================================
//  �`�揈��
//==========================================
void DrawOrbit()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���擾
	Orbit *pOrbit = GetOrbit();

	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

	//�J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_OrbitmtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_OrbitmtxWorld, &g_OrbitmtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&g_OrbitmtxWorld, &g_OrbitmtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_OrbitmtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffOrbit, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	for (int nCntOrbit = 0;nCntOrbit < MAX_ORBIT;nCntOrbit++)
	{
		if (g_aOrbit[nCntOrbit].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureOrbit);

			//�|���S���̕`��
			pDevice->DrawPrimitive
			(
				D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				MAX_EDGE * NUM_OFFSET * nCntOrbit,		//�`�悷��ŏ��̃C���f�b�N�X
				g_aOrbit[nCntOrbit].nNumEdge + (g_aOrbit[nCntOrbit].nNumEdge - 2)
			);												//�`�悷��v���~�e�B�u��
		}
	}

	//�J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================
//  �ݒ菈��
//==========================================
int SetOrbit(D3DXMATRIX mtxWorld, D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col,int nNumEdge)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//�ϐ��錾
	int nCounterOrbit = -1;

	//���擾
	Orbit *pOrbit = GetOrbit();

	for (int nCntOrbit = 0;nCntOrbit < MAX_ORBIT;nCntOrbit++,pOrbit++)
	{
		if (pOrbit->bUse == false)
		{
			//�F�̑��
			pOrbit->col = col;

			//�I�t�Z�b�g�̑��
			pOrbit->posOffset[0] = posOffset1;
			pOrbit->posOffset[1] = posOffset2;

			//�ӂ̐��̑��
			pOrbit->nNumEdge = nNumEdge;

			pOrbit->mtxWorld[0]._41 = posOffset1.x + mtxWorld._41;
			pOrbit->mtxWorld[0]._42 = posOffset1.y + mtxWorld._42;
			pOrbit->mtxWorld[0]._43 = posOffset1.z + mtxWorld._43;
			pOrbit->mtxWorld[1]._41 = posOffset2.x + mtxWorld._41;
			pOrbit->mtxWorld[1]._42 = posOffset2.y + mtxWorld._42;
			pOrbit->mtxWorld[1]._43 = posOffset2.z + mtxWorld._43;

			for (int nCntVtx = 0; nCntVtx < pOrbit->nNumEdge; nCntVtx++)
			{
				pOrbit->aPosPoint[nCntVtx][0].x = posOffset1.x + mtxWorld._41;
				pOrbit->aPosPoint[nCntVtx][0].y = posOffset1.y + mtxWorld._42;
				pOrbit->aPosPoint[nCntVtx][0].z = posOffset1.z + mtxWorld._43;
				pOrbit->aPosPoint[nCntVtx][1].x = posOffset2.x + mtxWorld._41;
				pOrbit->aPosPoint[nCntVtx][1].y = posOffset2.y + mtxWorld._42;
				pOrbit->aPosPoint[nCntVtx][1].z = posOffset2.z + mtxWorld._43;
			}

			UpdateOrbitPolygon(nCntOrbit);

			//�C���f�b�N�X�̑��
			nCounterOrbit = nCntOrbit;

			//�g�p��Ԃɂ���
			pOrbit->bUse = true;

			break;
		}
	}

	return nCounterOrbit;
}

//==========================================
//  �ʒu�ݒ菈��
//==========================================
void SetPositionOffset(D3DXMATRIX mtxWorld,int nIdxOrbit)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���擾����
	Orbit *pOrbit = GetOrbit();

	//�|�C���^��i�߂�
	pOrbit += nIdxOrbit;

	if (pOrbit->bUse)
	{
		for (int nCntOffset = 0; nCntOffset < NUM_OFFSET; nCntOffset++)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pOrbit->mtxWorld[nCntOffset]);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, pOrbit->posOffset[nCntOffset].x, pOrbit->posOffset[nCntOffset].y, pOrbit->posOffset[nCntOffset].z);
			D3DXMatrixMultiply(&pOrbit->mtxWorld[nCntOffset], &pOrbit->mtxWorld[nCntOffset], &mtxTrans);

			//�}�g���b�N�X���������킹��
			D3DXMatrixMultiply(&pOrbit->mtxWorld[nCntOffset], &pOrbit->mtxWorld[nCntOffset], &mtxWorld);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pOrbit->mtxWorld[nCntOffset]);
		}
	}

	//�|���S���X�V����
	UpdateOrbitPolygon(nIdxOrbit);
}

//==================================================
//�@�O�Ղ̎g�p��Ԑ؂�ւ�
//==================================================
void EnableOrbit(int nIdxOrbit, bool bUse)
{
	//�g�p��ԑ��
	g_aOrbit[nIdxOrbit].bUse = bUse;

	//���g�����Z�b�g
	ZeroMemory(&g_aOrbit[nIdxOrbit],sizeof(Orbit));
}

//==================================================
//�@���擾
//==================================================
Orbit *GetOrbit(void)
{
	return &g_aOrbit[0];
}

//==================================================
//�@�O�Տ���
//==================================================
void EnableOrbit(int nIdxOrbit)
{
	//�ϐ�������
	ZeroMemory(&g_aOrbit[nIdxOrbit], sizeof(Orbit));
}