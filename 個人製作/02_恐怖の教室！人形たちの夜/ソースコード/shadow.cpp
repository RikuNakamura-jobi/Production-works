//===========================
//
//�_�C���N�gX.shadow�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "shadow.h"

//�}�N����`
#define MAX_SHADOW (256)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;					//���_�����i�[
Shadow g_aShadow[MAX_SHADOW];

//===========================
//�v���C���[�̏���������
//===========================
void InitShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&g_pTextureShadow);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_aShadow[nCnt].posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].bTrue = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -30.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 30.0f;

		pVtx[1].pos.x = 30.0f;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 30.0f;

		pVtx[2].pos.x = -30.0f;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = -30.0f;

		pVtx[3].pos.x = 30.0f;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = -30.0f;

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateShadow(void)
{

}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aShadow[nCnt].mtxWorldShadow);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			g_aShadow[nCnt].rotShadow.y, g_aShadow[nCnt].rotShadow.x, g_aShadow[nCnt].rotShadow.z);
		D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorldShadow, &g_aShadow[nCnt].mtxWorldShadow, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			g_aShadow[nCnt].posShadow.x, g_aShadow[nCnt].posShadow.y, g_aShadow[nCnt].posShadow.z);
		D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorldShadow, &g_aShadow[nCnt].mtxWorldShadow, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCnt].mtxWorldShadow);

		if (g_aShadow[nCnt].bTrue == true)
		{
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffShadow,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===========================
//�v���C���[�̐ݒ菈��
//===========================
int SetShadow(float rad)
{
	int nCntShadow = -1;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bTrue == false)
		{
			g_aShadow[nCntShadow].posShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aShadow[nCntShadow].rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//���_���W�̐ݒ�
			pVtx[0].pos.x = -rad;
			pVtx[0].pos.y = 0.0f;
			pVtx[0].pos.z = rad;

			pVtx[1].pos.x = rad;
			pVtx[1].pos.y = 0.0f;
			pVtx[1].pos.z = rad;

			pVtx[2].pos.x = -rad;
			pVtx[2].pos.y = 0.0f;
			pVtx[2].pos.z = -rad;

			pVtx[3].pos.x = rad;
			pVtx[3].pos.y = 0.0f;
			pVtx[3].pos.z = -rad;

			//���_���W�̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_aShadow[nCntShadow].bTrue = true;

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//===========================
//�v���C���[�̐ݒ菈��
//===========================
void SetShadowMat(void)
{
	//D3DXMATRIX mtxShadow;
	//D3DLIGHT9 light;
	//D3DXVECTOR4 posLight;
	//D3DXVECTOR3 pos, normal;
	//D3DXPLANE plane;

	//pDevice->GetLight(0, &light);
	//posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

	//pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXPlaneFromPointNormal(&plane, &pos, &normal);

	////���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&mtxShadow);

	//D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
	//D3DXMatrixMultiply(&mtxShadow, &g_player.mtxWorld, &mtxShadow);

	////���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	//for (int nCntMat = 0; nCntMat < (int)g_dwNumMatPlayer; nCntMat++)
	//{
	//	//�}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	//�e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, NULL);

	//	//���f��(�p�[�c)�̕`��
	//	g_pMeshPlayer->DrawSubset(nCntMat);
	//}
}

//===========================
//�v���C���[�̈ʒu�ݒ菈��
//===========================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].posShadow.x = pos.x;
	g_aShadow[nIdxShadow].posShadow.z = pos.z;
}

//===========================
//�v���C���[�̈ʒu�ݒ菈��
//===========================
void SetShadowFalse(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bTrue = false;
}