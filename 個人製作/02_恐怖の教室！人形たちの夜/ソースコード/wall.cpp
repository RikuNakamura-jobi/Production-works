//===========================
//
//�_�C���N�gX.wall�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "wall.h"
#include "input.h"

//�}�N����`
#define MAX_WALL (4)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;					//���_�����i�[
Wall g_aWall[MAX_WALL];

//===========================
//�v���C���[�̏���������
//===========================
void InitWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\block000.jpg",
		&g_pTextureWall);

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (nCnt == 0)
		{
			g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 1800.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
		else if (nCnt == 1)
		{
			g_aWall[nCnt].pos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 1.57f, 0.0f);
		}
		else if (nCnt == 2)
		{
			g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, -1800.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
		}
		else if (nCnt == 3)
		{
			g_aWall[nCnt].pos = D3DXVECTOR3(-1800.0f, 0.0f, 0.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, -1.57f, 0.0f);
		}

		g_aWall[nCnt].bUse = true;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = -1800.0f;
		pVtx[0].pos.y = 600.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 1800.0f;
		pVtx[1].pos.y = 600.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = -1800.0f;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 1800.0f;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = 0.0f;

		if (nCnt == 0 || nCnt == 1)
		{
			//���_���W�̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}
		else if (nCnt == 2 || nCnt == 3)
		{
			//���_���W�̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}

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
	g_pVtxBuffWall->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitWall(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateWall(void)
{
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{
			if (GetKeyboardPress(DIK_O) == true)
			{//W�L�[�������ꂽ��
				g_aWall[nCnt].rot.y += 0.01f;
			}
			if (GetKeyboardPress(DIK_P) == true)
			{//W�L�[�������ꂽ��
				g_aWall[nCnt].rot.y -= 0.01f;
			}
		}
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCnt].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_aWall[nCnt].rot.y, g_aWall[nCnt].rot.x, g_aWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_aWall[nCnt].pos.x, g_aWall[nCnt].pos.y, g_aWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0,
				g_pVtxBuffWall,
				0,
				sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}

bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove)
{
	bool bLand = false;

	if ((pPos->x > 1800.0f || pPos->x < -1800.0f ||
		pPos->z > 1800.0f || pPos->z < -1800.0f||
		pPos->y < 0.0f) &&
		pPos->y < 600.0f)
	{
		bLand = true;
	}

	return bLand;
}