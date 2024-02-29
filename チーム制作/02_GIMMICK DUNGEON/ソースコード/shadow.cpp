//==================================================================================================
//
//shadow.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "shadow.h"
#include "input.h"

//�}�N����`
#define NUM_TEX					(2)											//�e�N�X�`����
#define SHADOW_TEX_FILE			"data\\TEXTURE\\shadow000.jpg"		//�e�N�X�`���t�@�C����
#define SHADOW_TEX_FILE2		"data\\TEXTURE\\shadow001.jpg"		//�e�N�X�`���t�@�C����
#define SHADOW_WIDTH			(50)										//��
#define SHADOW_HEIGHT			(50)										//����
#define MAX_SHADOW				(256)										//�e�̍ő吔

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureShadow[NUM_TEX] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Shadow g_aShadow[MAX_SHADOW];						//�\���̂̏��

//==================================================================================================
//����������
//==================================================================================================
void InitShadow(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEX_FILE,
		&g_apTextureShadow[0]);

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEX_FILE2,
		&g_apTextureShadow[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	//���擾
	Shadow *pShadow = GetShadow();

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		//�ϐ��̏�����
		ZeroMemory(pShadow, sizeof(Shadow));

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//�|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitShadow(void)
{
	for (int nCntTex = 0;nCntTex < NUM_TEX;nCntTex++)
	{//�e�N�X�`���j��
		if (g_apTextureShadow[nCntTex] != NULL)
		{
			g_apTextureShadow[nCntTex]->Release();
			g_apTextureShadow[nCntTex] = NULL;
		}
	}
	

	if (g_pVtxBuffShadow != NULL)
	{//���_�o�b�t�@�|�C���^�̔j��
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateShadow(void)
{

}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawShadow(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	//���Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���擾
	Shadow *pShadow = GetShadow();

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++,pShadow++)
	{
		if (pShadow->bUse == true)
		{//�g�p���Ă���Ȃ�
		 //���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply(&pShadow->mtxWorld, &pShadow->mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pShadow->mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_apTextureShadow[pShadow->type]);

			//�|���S���`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntShadow, 2);

		}
	}

	//���Z�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==================================================================================================
//�ݒ菈��
//==================================================================================================
int SetShadow(float fRadius,SHADOWTYPE type, float fWidth, float fHeight ,D3DXVECTOR3 rot)
{
	//�ϐ��錾
	int nCntShadow = -1;

	//���擾
	Shadow *pShadow = GetShadow();

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_SHADOW; nCount++, pShadow++)
	{
		if (pShadow->bUse == false)
		{//�g�p���Ă��Ȃ���ԂȂ�

			//���a���
			pShadow->rot = rot;
			pShadow->fRadius = fRadius;
			pShadow->fWidth = fWidth;
			pShadow->fHeight = fHeight;
			pShadow->type = type;

			//���_���W�̐ݒ�
			if (type == SHADOWTYPE_SQUARE)
			{
				pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
				pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
				pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
				pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(-pShadow->fRadius, 0.0f, pShadow->fRadius);
				pVtx[1].pos = D3DXVECTOR3(pShadow->fRadius, 0.0f, pShadow->fRadius);
				pVtx[2].pos = D3DXVECTOR3(-pShadow->fRadius, 0.0f, -pShadow->fRadius);
				pVtx[3].pos = D3DXVECTOR3(pShadow->fRadius, 0.0f, -pShadow->fRadius);
			}

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			nCntShadow = nCount;

			//�g�p��Ԃɂ���
			pShadow->bUse = true;

			//for���𔲂���
			break;
		}

		//�|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}

//==================================================================================================
//�ʒu�ݒ菈��
//==================================================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale)
{
	//�g�p������
	if (nIdxShadow == -1)
	{
		return;
	}

	//�ϐ��錾
	float fRadius, fHeight, fWidth;

	//�ʒu�E�����̎󂯎��
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].rot = rot;

	//���_���̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	//�|�C���^��i�߂�
	pVtx += 4 * nIdxShadow;

	//���_���W�̐ݒ�
	switch (g_aShadow[nIdxShadow].type)
	{
	case SHADOWTYPE_CIRCLE:
		fRadius = g_aShadow[nIdxShadow].fRadius * fScale;

		pVtx[0].pos = D3DXVECTOR3(-fRadius, 0.0f, fRadius);
		pVtx[1].pos = D3DXVECTOR3(fRadius, 0.0f, fRadius);
		pVtx[2].pos = D3DXVECTOR3(-fRadius, 0.0f, -fRadius);
		pVtx[3].pos = D3DXVECTOR3(fRadius, 0.0f, -fRadius);
		break;

	case SHADOWTYPE_SQUARE:
		fHeight = g_aShadow[nIdxShadow].fHeight * fScale;
		fWidth = g_aShadow[nIdxShadow].fWidth * fScale;

		pVtx[0].pos = D3DXVECTOR3(-fWidth, 0.0f, fHeight);
		pVtx[1].pos = D3DXVECTOR3(fWidth, 0.0f, fHeight);
		pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, -fHeight);
		pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, -fHeight);
		break;
	}
	
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//==================================================================================================
//���擾����
//==================================================================================================
Shadow *GetShadow(void)
{
	return &g_aShadow[0];
}

//==================================================================================================
//�e�L�������Ǘ�
//==================================================================================================
void EnableShadow(int nIdxShadow, bool bUse)
{
	g_aShadow[nIdxShadow].bUse = bUse;
}

//==================================================================================================
//�e�̃f�o�b�O����
//==================================================================================================
void DebugShadow()
{
	RECT rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };	//�ꏊ
	char aStr[1][256];					//��
	LPD3DXFONT Font = GetFont();		//���C���̂��
	int nUseCnter = 0;

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == true)
		{
			nUseCnter++;
		}
	}
	
	//������ɑ��
	sprintf(&aStr[0][0], "�e�̐��F%d", nUseCnter);

	for (int nCntDebug = 0; nCntDebug < 1; nCntDebug++)
	{
		Font->DrawText(NULL, &aStr[nCntDebug][0], -1, &rect, DT_RIGHT, D3DXCOLOR(0.7f, 0.0f, 0.7f, 1.0f));
		rect.top += 20;
	}
}
