//===========================
//
//�_�C���N�gX.life�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "life.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;								//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;							//���_�����i�[
D3DXVECTOR3 g_posLife;													//�ʒu
int g_nLife;															//�̗͂̒l

//===========================
//�̗͂̏���������
//===========================
void InitLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\life001.png",
		&g_pTextureLife);

	g_posLife = D3DXVECTOR3(1000.0f, 170.0f, 0.0f);
	g_nLife = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posLife.x;
	pVtx[0].pos.y = g_posLife.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posLife.x + 65.0f;
	pVtx[1].pos.y = g_posLife.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posLife.x;
	pVtx[2].pos.y = g_posLife.y + 65.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posLife.x + 65.0f;
	pVtx[3].pos.y = g_posLife.y + 65.0f;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

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


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//===========================
//�̗͂̏I������
//===========================
void UninitLife(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//===========================
//�̗͂̍X�V����
//===========================
void UpdateLife(void)
{
	
}

//===========================
//�̗͂̕`�揈��
//===========================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffLife,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0 , 2);
}

//===========================
//�̗͂̐ݒ菈��
//===========================
void SetLife(int nLife)
{
	g_nLife = nLife;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posLife.x;
	pVtx[0].pos.y = g_posLife.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[1].pos.y = g_posLife.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posLife.x;
	pVtx[2].pos.y = g_posLife.y + 65.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[3].pos.y = g_posLife.y + 65.0f;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f * g_nLife, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.2f * g_nLife, 1.0f);

	g_pVtxBuffLife->Unlock();
}

//===========================
//�̗͂̒ǉ�����
//===========================
void AddLife(int nValue)
{
	g_nLife -= nValue;

	if (g_nLife > 5)
	{
		g_nLife = 5;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_posLife.x;
	pVtx[0].pos.y = g_posLife.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[1].pos.y = g_posLife.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posLife.x;
	pVtx[2].pos.y = g_posLife.y + 65.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posLife.x + 13.0f * g_nLife;
	pVtx[3].pos.y = g_posLife.y + 65.0f;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.2f * g_nLife, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.2f * g_nLife, 1.0f);

	g_pVtxBuffLife->Unlock();
}

//===========================
//�̗͂̎擾����
//===========================
int GetLife(void)
{
	return g_nLife;
}