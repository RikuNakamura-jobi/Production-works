//*****************************************
//
//���f���Ǘ�(model.cpp)
//Author:���R����
//
//*****************************************

//*****************************************
// �C���N���[�h
//*****************************************
#include "main.h"
#include "model.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "book.h"

//*****************************************
// �}�N����`
//*****************************************
#define MAX_STRING	(256)	//�������̍ő�

//*****************************************
// �O���[�o���ϐ��錾
//*****************************************
const char *c_apFilePassObject[OBJECTTYPE_MAX] =
{//X�t�@�C���p�X
	NULL,
	"data\\MODEL\\OBJECT\\catapult_000.x",
	"data\\MODEL\\OBJECT\\reflector_000.x",
	"data\\MODEL\\OBJECT\\reflector_base_000.x",
	"data\\MODEL\\OBJECT\\lever_001.x",
	"data\\MODEL\\OBJECT\\lever_000.x",
	"data\\MODEL\\OBJECT\\torch_000.x",
	"data\\MODEL\\OBJECT\\button_000.x",
	"data\\MODEL\\OBJECT\\pole000.x",
	"data\\MODEL\\OBJECT\\sandpole_base_000.x",
	"data\\MODEL\\OBJECT\\stonepole000.x",
	"data\\MODEL\\OBJECT\\door_000.x",
	"data\\MODEL\\OBJECT\\book_000.x",
	"data\\MODEL\\OBJECT\\rock_frag.x",
	"data\\MODEL\\OBJECT\\rubble_000.x",
	"data\\MODEL\\OBJECT\\statue_002.x",
	"data\\MODEL\\OBJECT\\debris_000.x",
	"data\\MODEL\\OBJECT\\skull_000.x",
	"data\\MODEL\\OBJECT\\skullmt_000.x",
	"data\\MODEL\\OBJECT\\statue_001.x",
	"data\\MODEL\\OBJECT\\statue_004.x",
	"data\\MODEL\\OBJECT\\statue_005.x",
	"data\\MODEL\\OBJECT\\statue_006.x",
	"data\\MODEL\\OBJECT\\statue_007.x",
	"data\\MODEL\\OBJECT\\statue_008.x",
	"data\\MODEL\\OBJECT\\statue_009.x",
	"data\\MODEL\\OBJECT\\sword_001.x",
	"data\\MODEL\\OBJECT\\treasure_box_000.x",
	"data\\MODEL\\OBJECT\\statue_100.x",
	"data\\MODEL\\OBJECT\\statue_101.x",
	"data\\MODEL\\OBJECT\\button_000.x",
};
Model g_aModelObject[OBJECTTYPE_MAX];	//�I�u�W�F�N�g�̃��f�����
Model g_aModelPlayer[MAX_PLAYER_MODEL];	//�v���C���[�̃��f�����
Model g_aModelEnemy[MAX_ENEMY_MODEL];	//�G�̃��f�����
Model g_aModelBook[NUM_BOOK];	//�{�̃��f�����

int g_nNumPlayerParts;	//�v���C���[�̃p�[�c��
int g_nNumEnemyParts;	//�G�l�~�[�̃p�[�c��

//========================================
//����������
//========================================
void InitModel(void)
{
	//�I�u�W�F�N�g���f����񏉊���
	ZeroMemory(&g_aModelObject[0],sizeof(Model) * OBJECTTYPE_MAX);

	//�v���C���[���f����񏉊���
	ZeroMemory(&g_aModelPlayer[0], sizeof(Model) * MAX_PLAYER_MODEL);

	//�G�l�~�[���f����񏉊���
	ZeroMemory(&g_aModelEnemy[0], sizeof(Model) * MAX_ENEMY_MODEL);

	//�{���f����񏉊���
	ZeroMemory(&g_aModelBook[0], sizeof(Model) * NUM_BOOK);

	g_nNumPlayerParts = 0;
	g_nNumEnemyParts = 0;
}

//========================================
// �I������
//========================================
void UninitModel(void)
{
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{//�I�u�W�F�N�g���f���I������
		UninitXFile(&g_aModelObject[nCntObject]);
	}

	for (int nCntPart = 0;nCntPart < MAX_PLAYER_MODEL;nCntPart++)
	{//�v���C���[���f���I������
		UninitXFile(&g_aModelPlayer[nCntPart]);
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_MODEL; nCntEnemy++)
	{//�G���f���I������
		UninitXFile(&g_aModelEnemy[nCntEnemy]);
	}

	for (int nCntBook = 0; nCntBook < NUM_BOOK; nCntBook++)
	{//�{���f���I������
		UninitXFile(&g_aModelBook[nCntBook]);
	}
}

//========================================
// �ǂݍ��ݏ����܂Ƃ�
//========================================
void LoadAllModel(void)
{
	//���f���ǂݍ���
	LoadObjectModel();

	//�v���C���[���f���ǂݍ���
	LoadPlayerModel();

	//�G���f���ǂݍ���
	LoadEnemyModel();

	//�{�ǂݍ���
	LoadBookModel();
}

//========================================
// �I�u�W�F�N�g�̃��f�����擾
//========================================
void LoadObjectModel(void)
{
	for (int nCntObject = 0; nCntObject < OBJECTTYPE_MAX; nCntObject++)
	{//���f���ǂݍ���
		if (c_apFilePassObject[nCntObject] != NULL)
		{
			LoadModel(&g_aModelObject[nCntObject], c_apFilePassObject[nCntObject]);
		}
	}
}

//========================================
// �{�̃��f�����擾
//========================================
void LoadBookModel(void)
{
	//�ϐ��錾
	const char *acBookFilePass[NUM_BOOK] = 
	{
		"data\\MODEL\\OBJECT\\book_001.x",
		"data\\MODEL\\OBJECT\\book_002.x",
	};

	for (int nCntBook = 0; nCntBook < NUM_BOOK; nCntBook++)
	{//���f���ǂݍ���
		if (acBookFilePass[nCntBook] != NULL)
		{
			LoadModel(&g_aModelBook[nCntBook], acBookFilePass[nCntBook]);
		}
	}
}

//========================================
// �v���C���[�̃��f�����擾
//========================================
void LoadPlayerModel(void)
{
	//�ϐ��錾
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	char acFilenamePlayer[MAX_PLAYER_MODEL][MAX_STRING];

	//�t�@�C�����烂�f����ǂݍ���
	FILE *pFile = fopen("data\\MOTION\\motion_TH.txt", "r");

	while (1)
	{
		//�����ǂݍ���
		fscanf(pFile, "%s", &cTemp[0]);

		//�t�@�C�����ǂݍ���=========================================
		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{//���f�����ǂݍ���
			//"="�ǂݍ���
			fscanf(pFile, "%s", &cTemp[0]);

			//���f�����ǂݍ���
			fscanf(pFile, "%d", &g_nNumPlayerParts);

			for (int nCntFile = 0; nCntFile < g_nNumPlayerParts;)
			{//�t�@�C�����ǂݍ���
				//�����ǂݍ���
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "MODEL_FILENAME") == 0)
				{//�t�@�C�����ǂݍ���
					//"="�ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					//�t�@�C�����擾
					fscanf(pFile, "%s", &acFilenamePlayer[nCntFile][0]);

					//X�t�@�C���̓ǂݍ���
					LoadModel(&g_aModelPlayer[nCntFile], acFilenamePlayer[nCntFile]);

					//�ǂݍ��񂾃��f�����J�E���^���Z
					nCntFile++;
				}
			}
		}
		//�t�@�C�����ǂݍ���=========================================

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			break;
		}
	}
}

//========================================
// �G�l�~�[�̃��f�����擾
//========================================
void LoadEnemyModel(void)
{
	//�ϐ��錾
	char cTemp[MAX_STRING];
	int nCntMotion = 0;
	char acFilenameEnemy[MAX_ENEMY_MODEL][MAX_STRING];

	//�t�@�C�����烂�f����ǂݍ���
	FILE *pFile = fopen("data\\MOTION\\motion_golem.txt", "r");

	while (1)
	{
		//�����ǂݍ���
		fscanf(pFile, "%s", &cTemp[0]);

		//�t�@�C�����ǂݍ���=========================================
		if (strcmp(cTemp, "NUM_MODEL") == 0)
		{//���f�����ǂݍ���
		 //"="�ǂݍ���
			fscanf(pFile, "%s", &cTemp[0]);

			//���f�����ǂݍ���
			fscanf(pFile, "%d", &g_nNumEnemyParts);

			for (int nCntFile = 0; nCntFile < g_nNumEnemyParts;)
			{//�t�@�C�����ǂݍ���
			 //�����ǂݍ���
				fscanf(pFile, "%s", &cTemp[0]);

				if (strcmp(cTemp, "MODEL_FILENAME") == 0)
				{//�t�@�C�����ǂݍ���
				 //"="�ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					//�t�@�C�����擾
					fscanf(pFile, "%s", &acFilenameEnemy[nCntFile][0]);

					//X�t�@�C���̓ǂݍ���
					LoadModel(&g_aModelEnemy[nCntFile], acFilenameEnemy[nCntFile]);

					//�ǂݍ��񂾃��f�����J�E���^���Z
					nCntFile++;
				}
			}
		}
		//�t�@�C�����ǂݍ���=========================================

		if (strcmp(cTemp, "END_SCRIPT") == 0)
		{
			break;
		}
	}
}

//========================================
// ���f���̓ǂݍ���(�ėp)
//========================================
void LoadModel(Model *pModel,const char *pFilePass)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//�v�Z�p���[�J���ϐ��錾
	int nNumVtx;	//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	
	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL *pMat;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pFilePass,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModel->pBuffMat,
		NULL,
		&pModel->dwNumMat,
		&pModel->pMesh);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)pModel->pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pModel->dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&pModel->apTexture[nCntMat]);
		}
	}

	//�ő�E�ŏ����_���擾================================================
	//���_�����擾
	nNumVtx = pModel->pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(pModel->pMesh->GetFVF());

	//���_�o�b�t�@�̃��b�N
	pModel->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntObject = 0; nCntObject < nNumVtx; nCntObject++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//���_���W�̑��

		if (vtx.x < pModel->vtxMin.x)
		{//X���̍ŏ�
			pModel->vtxMin.x = vtx.x;
		}
		if (vtx.x > pModel->vtxMax.x)
		{//X���̍ő�
			pModel->vtxMax.x = vtx.x;
		}

		if (vtx.y < pModel->vtxMin.y)
		{//Y���̍ŏ�
			pModel->vtxMin.y = vtx.y;
		}
		if (vtx.y > pModel->vtxMax.y)
		{//Y���̍ő�
			pModel->vtxMax.y = vtx.y;
		}

		if (vtx.z < pModel->vtxMin.z)
		{//Z���̍ŏ�
			pModel->vtxMin.z = vtx.z;
		}
		if (vtx.z > pModel->vtxMax.z)
		{//Z���̍ő�
			pModel->vtxMax.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	pModel->pMesh->UnlockVertexBuffer();
	//�ő�E�ŏ����_���擾================================================
}

//========================================
// �I�u�W�F�N�g���f�����擾
//========================================
Model *GetModelobject(void)
{
	return &g_aModelObject[0];
}

//========================================
// �{���f�����擾
//========================================
Model *GetModelBook(void)
{
	return &g_aModelBook[0];
}

//========================================
// �v���C���[���f�����擾
//========================================
Model *GetModelPlayer(void)
{
	return &g_aModelPlayer[0];
}

//========================================
// �v���C���[���f�����擾
//========================================
Model *GetModelEnemy(void)
{
	return &g_aModelEnemy[0];
}

//========================================
// �v���C���[���f�����擾
//========================================
int GetNumPlayerPart(void)
{
	return g_nNumPlayerParts;
}

//========================================
// �v���C���[���f�����擾
//========================================
int GetNumEnemyPart(void)
{
	return g_nNumEnemyParts;
}

//========================================
// �ėp�I������
//========================================
void UninitXFile(Model *pModel)
{
	if (pModel->pBuffMat != NULL)
	{//�}�e���A���̔j��
		pModel->pBuffMat->Release();
		pModel->pBuffMat = NULL;
	}

	if (pModel->pMesh != NULL)
	{//���b�V���̔j��
		pModel->pMesh->Release();
		pModel->pMesh = NULL;
	}

	for (int nCntTex = 0; nCntTex < NUM_TEX; nCntTex++)
	{//�e�N�X�`���̔j��
		if (pModel->apTexture[nCntTex] != NULL)
		{
			pModel->apTexture[nCntTex]->Release();
			pModel->apTexture[nCntTex] = NULL;
		}
	}
}