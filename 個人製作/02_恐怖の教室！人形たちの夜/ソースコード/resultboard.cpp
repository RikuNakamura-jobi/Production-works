//===========================
//
//�_�C���N�gX.resultboard�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "resultboard.h"
#include "camera.h"
#include "result.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureResultBoard = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultBoard = NULL;		//���_�����i�[
D3DXVECTOR3 g_posResultBoard;
D3DXMATRIX g_mtxWorldResultBoard;							//���[���h�}�g���b�N�X

//�e�N�X�`���t�@�C����
const char *c_apFilenameResultBoard[2] =
{
	"data\\TEXTURE\\clear000.png",
	"data\\TEXTURE\\gameover000.png",
};

//===========================
//�v���C���[�̏���������
//===========================
void InitResultBoard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	RESULT result = GetResult();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		c_apFilenameResultBoard[result],
		&g_pTextureResultBoard);

	g_posResultBoard = D3DXVECTOR3(0.0f, 200.0f, 580.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_3D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultBoard,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultBoard->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = -128.0f;
	pVtx[0].pos.y = 72.0f;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 128.0f;
	pVtx[1].pos.y = 72.0f;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = -128.0f;
	pVtx[2].pos.y = -72.0f;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 128.0f;
	pVtx[3].pos.y = -72.0f;
	pVtx[3].pos.z = 0.0f;

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultBoard->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitResultBoard(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureResultBoard != NULL)
	{
		g_pTextureResultBoard->Release();
		g_pTextureResultBoard = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultBoard != NULL)
	{
		g_pVtxBuffResultBoard->Release();
		g_pVtxBuffResultBoard = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateResultBoard(void)
{
	Camera pCamera = GetCamera();

	g_posResultBoard = pCamera.posR;
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawResultBoard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X�擾�p

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//���e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldResultBoard);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&g_mtxWorldResultBoard, NULL, &mtxView);
	g_mtxWorldResultBoard._41 = 0.0f;
	g_mtxWorldResultBoard._42 = 0.0f;
	g_mtxWorldResultBoard._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_posResultBoard.x, g_posResultBoard.y, g_posResultBoard.z);
	D3DXMatrixMultiply(&g_mtxWorldResultBoard, &g_mtxWorldResultBoard, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldResultBoard);
	
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffResultBoard,
		0,
		sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultBoard);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�e����
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//���e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}