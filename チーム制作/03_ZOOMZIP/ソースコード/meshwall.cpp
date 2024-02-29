//==========================================================
//
//���b�V���E�H�[������ [meshwall.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "meshwall.h"
#include "input.h"
#include "texture.h"
#include "fileload.h"

//==========================================================
//�}�N����`
//==========================================================
#define TEXTUREFILE_DATA	"data\\TEXTURE\\field000.jpg"		//�e�N�X�`���f�[�^
#define MAX_FIELD		(28)			//�t�B�[���h�̒��_��
#define FIELD_WIDTH		(3)				//�t�B�[���h�̕�
#define FIELD_HEIGHT	(3)				//�t�B�[���h�̍���
#define FIELD_TYOUTEN	(4 * 4)			//���_��
#define FIELD_WIDTHWIDTH	(100.0f)	//������
#define FIELD_HEIGHTHEIGHT	(100.0f)	//���s����

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void CreateVertexWallBuff(void);
void CreateIndexWallBuff(void);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdexBuffMeshWall = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshWall;				//�ʒu
D3DXVECTOR3 g_rotMeshWall;				//����
D3DXMATRIX g_mtxWorldMeshWall;			//���[���h�}�g���b�N�X
int g_nMeshWallVertex;					//���_�̎g�p���m�F
int g_nMeshWallIndex;					//�C���f�b�N�X�̎d�l���m�F
MeshWall g_aMeshWall[MAX_MESHWALL];		//���b�V���E�H�[���̏��

//==========================================================
//���b�V���E�H�[���̏���������
//==========================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e��ϐ��̏�����
	g_posMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		g_aMeshWall[nCntWall].Polygon.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].Polygon.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMeshWall[nCntWall].Polygon.fHeight = 0.0f;
		g_aMeshWall[nCntWall].Polygon.fWidth = 0.0f;
		g_aMeshWall[nCntWall].nHeightMesh = 0;
		g_aMeshWall[nCntWall].nWidthMesh = 0;
		g_aMeshWall[nCntWall].nVertex = 0;
		g_aMeshWall[nCntWall].nStartIndex = 0;
		g_aMeshWall[nCntWall].bUse = false;
		
	}

	//�ǂݍ��ݐ��f�[�^�̏�����
	g_nMeshWallVertex = 0;
	g_nMeshWallIndex = 0;
}

//==========================================================
//���b�V���E�H�[���̏I������
//==========================================================
void UninitMeshWall(void)
{
	//���_�o�b�t�@�̔p��
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̔p��
	if (g_pIdexBuffMeshWall != NULL)
	{
		g_pIdexBuffMeshWall->Release();
		g_pIdexBuffMeshWall = NULL;
	}

}

//==========================================================
//���b�V���E�H�[���̍X�V����
//==========================================================
void UpdateMeshWall(void)
{
	int nVertex = 0;

	////���b�V���E�H�[���̍ő吔�m�F
	//for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	//{
	//	if (g_aMeshWall[nCntWall].bUse == true)
	//	{//�g�p���Ă���ꍇ
	//		SceltonWall(nVertex, g_aMeshWall[nCntWall]);
	//	}

	//	nVertex += g_aMeshWall[nCntWall].nVertex;

	//	if (nVertex >= g_nMeshWallVertex)
	//	{//�������𒴂����ꍇ
	//		break;
	//	}
	//}
}

//==========================================================
//���b�V���E�H�[���̕`�揈��
//==========================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	int nCntIndex = 0;
	int aOrderIndex[MAX_MESHWALL] = {};	//�ԍ�
	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		aOrderIndex[nCntWall] = nCntWall;
	}

	//���Ԋm�F
	//OrderWall(&aOrderIndex[0]);

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{

		if (g_aMeshWall[aOrderIndex[nCntWall]].bUse == true)
		{//�g�p����Ă���Ƃ�
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.rot.y, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.rot.x, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.rot.z);
			D3DXMatrixMultiply(&g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.pos.x, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.pos.y, g_aMeshWall[aOrderIndex[nCntWall]].Polygon.pos.z);
			D3DXMatrixMultiply(&g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall[aOrderIndex[nCntWall]].Polygon.mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffMeshWall,
				0,
				sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdexBuffMeshWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, *SetTexture(g_aMeshWall[aOrderIndex[nCntWall]].nTextype));

			//���b�V���E�H�[���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshWall[aOrderIndex[nCntWall]].nVertex,		//�p�ӂ������_��
				g_aMeshWall[aOrderIndex[nCntWall]].nStartIndex,							//�C���f�b�N�X�J�n�_
				g_aMeshWall[aOrderIndex[nCntWall]].nIndex - 2	//�`�悷��v���~�e�B�u��
			);

			//nCntIndex += g_aMeshWall[nCntWall].nIndex;
		}
	}
}

//==========================================================
//���b�V���E�H�[���̐ݒ�
//==========================================================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType)
{
	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aMeshWall[nCntWall].Polygon.pos = pos;
			g_aMeshWall[nCntWall].Polygon.rot = rot;

			if (g_aMeshWall[nCntWall].Polygon.rot.y > D3DX_PI || g_aMeshWall[nCntWall].Polygon.rot.y < -D3DX_PI)
			{//-3.14�`3.14�͈̔͊O�̏ꍇ
				if (g_aMeshWall[nCntWall].Polygon.rot.y > D3DX_PI)
				{
					g_aMeshWall[nCntWall].Polygon.rot.y += (-D3DX_PI * 2);
				}
				else if (g_aMeshWall[nCntWall].Polygon.rot.y < -D3DX_PI)
				{
					g_aMeshWall[nCntWall].Polygon.rot.y += (D3DX_PI * 2);
				}
			}

			g_aMeshWall[nCntWall].Polygon.fHeight = fHeight;
			g_aMeshWall[nCntWall].Polygon.fWidth = fWidth;
			g_aMeshWall[nCntWall].nHeightMesh = nHeight;
			g_aMeshWall[nCntWall].nWidthMesh = nWidth;
			g_aMeshWall[nCntWall].nVertex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh + 1);
			g_aMeshWall[nCntWall].bUse = true;
			g_aMeshWall[nCntWall].nIndex = (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nHeightMesh) * 2 + (g_aMeshWall[nCntWall].nHeightMesh - 1) * 2;
			g_aMeshWall[nCntWall].nTextype = nType;
			g_aMeshWall[nCntWall].nStartIndex = g_nMeshWallIndex;

			//���_���ƃC���f�b�N�X������
			g_nMeshWallIndex += g_aMeshWall[nCntWall].nIndex;
			g_nMeshWallVertex += g_aMeshWall[nCntWall].nVertex;
			break;
		}
	}
}

//==========================================================
//���b�V���E�H�[���̃C���f�b�N�X�ԍ��ݒ�
//==========================================================
void SetIndexMeshWall(int nCntWall)
{
	WORD *pIdx;	//�C���f�b�N�X���̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N�����_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdexBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	pIdx += g_nMeshWallIndex;

	for (int nCntIdx = 0; nCntIdx < g_aMeshWall[nCntWall].nIndex / 2; nCntIdx++)
	{
		pIdx[nCntIdx * 2] = (nCntIdx + g_aMeshWall[nCntWall].nWidthMesh + 1)
			- (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 2))
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));

		pIdx[nCntIdx * 2 + 1] = nCntIdx
			+ (nCntIdx % (g_aMeshWall[nCntWall].nWidthMesh + 2) / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].nWidthMesh + 1))
			- (nCntIdx / (g_aMeshWall[nCntWall].nWidthMesh + 2));

		pIdx[nCntIdx * 2] += g_nMeshWallVertex;

		pIdx[nCntIdx * 2 + 1] += g_nMeshWallVertex;
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdexBuffMeshWall->Unlock();

	g_nMeshWallIndex += g_aMeshWall[nCntWall].nIndex;
}

//==========================================================
//���b�V���E�H�[����pVtx�ݒ�
//==========================================================
void SetpVtxMeshWall(int nCntWall)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffMeshWall->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	pVtx += g_nMeshWallVertex;

	//���_���W�̐ݒ�(��������E��O�Ɍ������Ē��_����ݒ肷��
	for (int nCntpVtx = 0; nCntpVtx < g_aMeshWall[nCntWall].nVertex; nCntpVtx++)
	{
		//���_���W
		pVtx[nCntpVtx].pos.x = -(g_aMeshWall[nCntWall].Polygon.fWidth * g_aMeshWall[nCntWall].nWidthMesh) + (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1) * (g_aMeshWall[nCntWall].Polygon.fWidth * 2));
		pVtx[nCntpVtx].pos.y = ((g_aMeshWall[nCntWall].Polygon.fHeight * 2) * g_aMeshWall[nCntWall].nHeightMesh) + ((nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1) * (-g_aMeshWall[nCntWall].Polygon.fHeight * 2)));
		pVtx[nCntpVtx].pos.z = 0.0f;

		//�@��
		pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//�F
		pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (g_aMeshWall[nCntWall].nWidthMesh + 1)), 1.0f * (nCntpVtx / (g_aMeshWall[nCntWall].nWidthMesh + 1)));
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshWall->Unlock();

	g_nMeshWallVertex += g_aMeshWall[nCntWall].nVertex;
}

//==========================================================
//���b�V���E�H�[���̏��擾
//==========================================================
MeshWall *GetMeshWall(void)
{
	return &g_aMeshWall[0];
}

//==========================================================
//���b�V���t�B�[���h�̒��_�o�b�t�@����
//==========================================================
void CreateVertexWallBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshWallIndex,	//�K�v�ȃC���f�b�N�X��
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdexBuffMeshWall,
		NULL);
}

//==========================================================
//���b�V���t�B�[���h�̃C���f�b�N�X�o�b�t�@����
//==========================================================
void CreateIndexWallBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nMeshWallVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall,
		NULL);
}

//==========================================================
//���b�V���E�H�[���̏��ǂݍ���
//==========================================================
void LoadMeshWallData(FILE *pFile)
{
	char aStr[256];	//�]���ȕ��͓ǂݍ��ݗp
	MeshWall meshWall;

	//������
	memset(&meshWall, NULL, sizeof(meshWall));

	//�I�������܂œǂݍ���
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//�z�u���m�F
		if (strcmp(&aStr[0], LOAD_TEXTYPE) == 0)
		{//�e�N�X�`��
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &meshWall.nTextype);	//�e�N�X�`�����ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//���W
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.pos.x);	//x���W�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.pos.y);	//y���W�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.pos.z);	//z���W�ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//����
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.rot.x);	//x���W�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.rot.y);	//y���W�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.rot.z);	//z���W�ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_BLOCK) == 0)
		{//����
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &meshWall.nHeightMesh);	//�c���ǂݍ���
			fscanf(pFile, "%d", &meshWall.nWidthMesh);		//�����ǂݍ���

		}
		else if (strcmp(&aStr[0], LOAD_SIZE) == 0)
		{//�T�C�Y
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.fHeight);//�c���ǂݍ���
			fscanf(pFile, "%f", &meshWall.Polygon.fWidth);	//�����ǂݍ���
		}

		//�I��
		if (strcmp(&aStr[0], ENDWALLSET_TXT) == 0)
		{//�I������
			break;
		}
	}

	if (meshWall.Polygon.rot.y > 180.0f)
	{//180�x�������ꍇ
		meshWall.Polygon.rot.y += -360.0f;
	}
	else if (meshWall.Polygon.rot.y < -180.0f)
	{//-180�x�������ꍇ
		meshWall.Polygon.rot.y += 360.0f;
	}

	//�ǂ̔z�u
	SetMeshWall(meshWall.Polygon.pos, D3DXToRadian(meshWall.Polygon.rot),
		meshWall.Polygon.fHeight, meshWall.Polygon.fWidth,
		meshWall.nHeightMesh, meshWall.nWidthMesh, meshWall.nTextype);
}

//==========================================================
//���b�V���E�H�[���̐����֘A
//==========================================================
void CreateWallBuff(void)
{

	//���_�o�b�t�@����
	CreateVertexWallBuff();

	//�C���f�b�N�X�o�b�t�@����
	CreateIndexWallBuff();

	//��񏉊���
	g_nMeshWallVertex = 0;
	g_nMeshWallIndex = 0;

	for (int nCntWall = 0; nCntWall < MAX_MESHWALL; nCntWall++)
	{
		if (g_aMeshWall[nCntWall].bUse == true)
		{//�g�p����Ă���ꍇ
		 //�C���f�b�N�X�̐ݒ�
			SetIndexMeshWall(nCntWall);

			//���_���W�̐ݒ�
			SetpVtxMeshWall(nCntWall);
		}
	}
}
