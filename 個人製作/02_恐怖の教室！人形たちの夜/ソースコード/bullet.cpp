//===========================
//
//�_�C���N�gX.bullet�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "bullet.h"
#include "shadow.h"
#include "wall.h"
#include "meshwall.h"
#include "effect.h"
#include "explosion.h"
#include "particle.h"
#include "model.h"
#include "enemy.h"
#include "player.h"

//�}�N����`
#define MAX_BULLET (8192)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;					//���_�����i�[
Bullet g_aBullet[MAX_BULLET];

//===========================
//�v���C���[�̏���������
//===========================
void InitBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet002.png",
		&g_pTextureBullet);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		g_aBullet[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aBullet[nCnt].nLife = 300;
		g_aBullet[nCnt].nIdxShadow = 1;
		g_aBullet[nCnt].bTrue = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -5.0f;
		pVtx[0].pos.y = 5.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 5.0f;
		pVtx[1].pos.y = 5.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = -5.0f;
		pVtx[2].pos.y = -5.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 5.0f;
		pVtx[3].pos.y = -5.0f;
		pVtx[3].pos.z = 0.0f;

		//���_���W�̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aBullet[nCnt].col;
		pVtx[1].col = g_aBullet[nCnt].col;
		pVtx[2].col = g_aBullet[nCnt].col;
		pVtx[3].col = g_aBullet[nCnt].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateBullet(void)
{
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bTrue == true)
		{
			g_aBullet[nCnt].posOld = g_aBullet[nCnt].pos;
			g_aBullet[nCnt].nLife--;

			g_aBullet[nCnt].pos += g_aBullet[nCnt].move;

			//�e�̈ʒu��ݒ�
			SetPositionShadow(g_aBullet[nCnt].nIdxShadow, g_aBullet[nCnt].pos);

			if (g_aBullet[nCnt].nType == 1)
			{
				SetEffect(g_aBullet[nCnt].pos, g_aBullet[nCnt].col, 10.0f, 7);
			}

			if (g_aBullet[nCnt].nLife <= 0)
			{
				g_aBullet[nCnt].bTrue = false;
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
			}

			if (CollisionMeshWall(&g_aBullet[nCnt].pos, &g_aBullet[nCnt].posOld, &g_aBullet[nCnt].move) == true)
			{
				SetExplosion(g_aBullet[nCnt].pos, g_aBullet[nCnt].col, 0);
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
				g_aBullet[nCnt].bTrue = false;
			}

			if (CollisionModel(&g_aBullet[nCnt].pos, &g_aBullet[nCnt].posOld, &g_aBullet[nCnt].move, g_aBullet[nCnt].nType) == true)
			{
				SetExplosion(g_aBullet[nCnt].pos, g_aBullet[nCnt].col, 0);
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
				g_aBullet[nCnt].bTrue = false;
			}

			if (CollisionEnemy(&g_aBullet[nCnt].pos, &g_aBullet[nCnt].posOld, &g_aBullet[nCnt].move, g_aBullet[nCnt].nType) == true)
			{
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
				g_aBullet[nCnt].bTrue = false;
			}

			if (CollisionPlayer(&g_aBullet[nCnt].pos, &g_aBullet[nCnt].posOld, &g_aBullet[nCnt].move, g_aBullet[nCnt].nType) == true)
			{
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
				g_aBullet[nCnt].bTrue = false;
			}

			if (g_aBullet[nCnt].pos.y < 0.0f)
			{
				SetExplosion(g_aBullet[nCnt].pos, g_aBullet[nCnt].col, 0);
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
				g_aBullet[nCnt].bTrue = false;
			}

			if (g_aBullet[nCnt].pos.y > 1800.0f)
			{
				SetExplosion(g_aBullet[nCnt].pos, g_aBullet[nCnt].col, 0);
				SetShadowFalse(g_aBullet[nCnt].nIdxShadow);
				g_aBullet[nCnt].bTrue = false;
			}
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X�擾�p

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 100);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bTrue == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorld, NULL, &mtxView);
			g_aBullet[nCnt].mtxWorld._41 = 0.0f;
			g_aBullet[nCnt].mtxWorld._42 = 0.0f;
			g_aBullet[nCnt].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffBullet,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//===========================
//�v���C���[�̐ݒ菈��
//===========================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 point, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, int nType)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bTrue == false)
		{
			g_aBullet[nCnt].pos = pos;
			g_aBullet[nCnt].col = col;
			g_aBullet[nCnt].posOld = g_aBullet[nCnt].pos;
			g_aBullet[nCnt].nLife = nLife;
			g_aBullet[nCnt].nType = nType;
			g_aBullet[nCnt].nIdxShadow = SetShadow(10.0f);
			g_aBullet[nCnt].bTrue = true;
			
			if (nType == 1)
			{
				g_aBullet[nCnt].move.x = (sinf(rot.z - 0.01f) * sinf(rot.y)) * 50.0f;
				g_aBullet[nCnt].move.y = cosf(rot.z - 0.01f) * 50.0f;
				g_aBullet[nCnt].move.z = (sinf(rot.z - 0.01f) * cosf(rot.y)) * 50.0f;
			}
			else if (nType == 2)
			{
				g_aBullet[nCnt].move.x = (sinf(rot.z - 0.01f) * sinf(rot.y)) * 20.0f;
				g_aBullet[nCnt].move.y = cosf(rot.z - 0.01f) * 20.0f;
				g_aBullet[nCnt].move.z = (sinf(rot.z - 0.01f) * cosf(rot.y)) * 20.0f;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos.x = -2.5f;
			pVtx[0].pos.y = 2.5f;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = 2.5f;
			pVtx[1].pos.y = 2.5f;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = -2.5f;
			pVtx[2].pos.y = -2.5f;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = 2.5f;
			pVtx[3].pos.y = -2.5f;
			pVtx[3].pos.z = 0.0f;

			//���_���W�̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aBullet[nCnt].col = col;
			pVtx[1].col = g_aBullet[nCnt].col = col;
			pVtx[2].col = g_aBullet[nCnt].col = col;
			pVtx[3].col = g_aBullet[nCnt].col = col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			break;
		}
			pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}