//==========================================================
//
//X�t�@�C�����f������ [model.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "fileload.h"
#include "meshwall.h"
#include "debugproc.h"
#include "result.h"

//==========================================================
//�}�N����`
//==========================================================
#define STATE_TXT		"STATESET"		//�t�@�C�����m�F
#define END_STATE_TXT	"END_STATESET"	//�t�@�C�����m�F
#define WEIGHT_TXT		"WEIGHT"		//�d���m�F����
#define SKELTONLENGTH	(0.9f)			//���������苗���{��
#define RANKING_MODELSET	(-900.0f)	//���f���ݒ�ʒu
#define PUSH_MOVE		(0.8f)			//�����Ă���ۂ̈ړ��ʌ���
#define POWER_MOVE		(0.98f)			//power�������̈ړ��ʌ���
#define SKELTON_ALPHA	(0.2f)			//���߃J���[
#define SKELTON_ALPMOVE	(0.02f)			//���߈ړ���

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void SetVtxSize(int nCntModel);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
Model g_aModel[MAX_MODEL];				//���f�����
ModelFileData g_aModelFileData[MAX_MODELFILE];	//���f���t�@�C�����

//==========================================================
//���f���̏���������
//==========================================================
void InitModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//�f�o�C�X�ւ̃|�C���^

	//�e��ϐ��̏�����
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aModel[nCntModel].nShadow = -1;
		g_aModel[nCntModel].fMoveY = 0.0f;
		g_aModel[nCntModel].nType = 0;
		g_aModel[nCntModel].nShadow = -1;
		g_aModel[nCntModel].bUse = false;
		g_aModel[nCntModel].pModelFiledata = NULL;
	}

	//���f���f�[�^�ő�l�ŏ��l������
	for (int nCntModelFile = 0; nCntModelFile < MAX_MODELFILE; nCntModelFile++)
	{
		g_aModelFileData[nCntModelFile].vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		g_aModelFileData[nCntModelFile].vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		g_aModelFileData[nCntModelFile].nType = MODELTYPE_OBJECT;
	}
}

//==========================================================
//���f���̏I������
//==========================================================
void UninitModel(void)
{
	for (int nCntModelFile = 0; nCntModelFile < MAX_MODELFILE; nCntModelFile++)
	{
		//���b�V���̔p��
		if (g_aModelFileData[nCntModelFile].pMesh != NULL)
		{
			g_aModelFileData[nCntModelFile].pMesh->Release();
			g_aModelFileData[nCntModelFile].pMesh = NULL;
		}

		//�}�e���A���̔p��
		if (g_aModelFileData[nCntModelFile].pBuffMat != NULL)
		{
			g_aModelFileData[nCntModelFile].pBuffMat->Release();
			g_aModelFileData[nCntModelFile].pBuffMat = NULL;
		}

		//�e�N�X�`���̔p��
		for (int nCntTexture = 0; nCntTexture < MAX_MODELTEX; nCntTexture++)
		{
			if (g_aModelFileData[nCntModelFile].apTexture[nCntTexture] != NULL)
			{
				g_aModelFileData[nCntModelFile].apTexture[nCntTexture]->Release();
				g_aModelFileData[nCntModelFile].apTexture[nCntTexture] = NULL;
			}
		}
	}

}

//==========================================================
//���f���̍X�V����
//==========================================================
void UpdateModel(void)
{

}

//==========================================================
//���f���̕`�揈��
//==========================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{//�g�p����Ă���ꍇ
				//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aModel[nCntModel].pModelFiledata->pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].pModelFiledata->dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aModel[nCntModel].pModelFiledata->apTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aModel[nCntModel].pModelFiledata->pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
//���f���̐ݒ�
//==========================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSetShadow)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{//�g�p����Ă��Ȃ��ꍇ
			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].rot = rot;
			g_aModel[nCntModel].nType = nType;
			g_aModel[nCntModel].pModelFiledata = GetModelData(g_aModel[nCntModel].nType);
			g_aModel[nCntModel].bUse = true;
			g_aModel[nCntModel].fMoveY = 0.0f;

			break;
		}
	}
}

//==========================================================
//���f���f�[�^���擾
//==========================================================
ModelFileData *GetModelData(int nModelType)
{
	return &g_aModelFileData[nModelType];
}

//==========================================================
//�t�@�C���ǂݍ���
//==========================================================
void LoadModelFile(int nMaxModelFile)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�t�@�C�������ǂݍ���
	for (int nCntModelFile = 0; nCntModelFile < nMaxModelFile; nCntModelFile++)
	{
		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(pModelFileName(nCntModelFile),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aModelFileData[nCntModelFile].pBuffMat,
			NULL,
			&g_aModelFileData[nCntModelFile].dwNumMat,
			&g_aModelFileData[nCntModelFile].pMesh);

		//�����蔻��T�C�Y�擾
		SetVtxSize(nCntModelFile);

		D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aModelFileData[nCntModelFile].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_aModelFileData[nCntModelFile].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�e�N�X�`���t�@�C�������݂��Ă���
			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &g_aModelFileData[nCntModelFile].apTexture[nCntMat]);
			}
		}
	}
}

//==========================================================
//X�t�@�C���ő�l�ŏ��l�擾
//==========================================================
void SetVtxSize(int nCntModel)
{
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�̃|�C���^

	//���_�����擾
	nNumVtx = g_aModelFileData[nCntModel].pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_aModelFileData[nCntModel].pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	g_aModelFileData[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		if (vtx.x < g_aModelFileData[nCntModel].vtxMin.x)
		{//�ŏ��l�����l���������ꍇ
			g_aModelFileData[nCntModel].vtxMin.x = vtx.x;
		}
		else if (vtx.x > g_aModelFileData[nCntModel].vtxMax.x)
		{//�ő�l�����l���傫���ꍇ
			g_aModelFileData[nCntModel].vtxMax.x = vtx.x;
		}

		if (vtx.z < g_aModelFileData[nCntModel].vtxMin.z)
		{//�ŏ��l�����l���������ꍇ
			g_aModelFileData[nCntModel].vtxMin.z = vtx.z;
		}
		else if (vtx.z > g_aModelFileData[nCntModel].vtxMax.z)
		{//�ő�l�����l���傫���ꍇ
			g_aModelFileData[nCntModel].vtxMax.z = vtx.z;
		}

		if (vtx.y < g_aModelFileData[nCntModel].vtxMin.y)
		{//�ŏ��l�����l���������ꍇ
			g_aModelFileData[nCntModel].vtxMin.y = vtx.y;
		}
		else if (vtx.y > g_aModelFileData[nCntModel].vtxMax.y)
		{//�ő�l�����l���傫���ꍇ
			g_aModelFileData[nCntModel].vtxMax.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_aModelFileData[nCntModel].pMesh->UnlockVertexBuffer();
}

//==========================================================
//���f���^�C�v���ǂݍ���
//==========================================================
void LoadModelFileData(FILE *pFile)
{
	int nNumCnt = 0;	//���݂̃t�@�C���ԍ�
	char aStr[256];
	do
	{
			//�e�L�X�g�ǂݍ���
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], STATE_TXT) == 0)
			{//�X�N���v�g�J�n�̕������m�F�ł����ꍇ
				while (1)
				{
					nResult = fscanf(pFile, "%s", &aStr[0]);

					if (strcmp(&aStr[0], LOAD_MODELTYPE) == 0)
					{//���f�����
						fscanf(pFile, "%s", &aStr[0]);	//=
						fscanf(pFile, "%d", &g_aModelFileData[nNumCnt].nType);		//��ގ擾
					}
					else if (strcmp(&aStr[0], END_STATE_TXT) == 0)
					{//�I������	
						nNumCnt++;	//���̃t�@�C����
						break;
					}
					else if (nResult == EOF)
					{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
						nNumCnt = GetModelCnt();
						break;
					}
				}
			}
			else if (nResult == EOF)
			{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
				break;
			}
	}while (nNumCnt < GetModelCnt());	//���f���t�@�C�������܂œǂݍ���
}

//==========================================================
//���f���z�u�f�[�^�ǂݍ���
//==========================================================
void LoadModelData(FILE *pFile)
{
	char aStr[256];	//�]���ȕ��͓ǂݍ��ݗp
	Model Model;
	int nType = 0;	//�g�p���郂�f���ԍ�

	//������
	memset(&Model, NULL, sizeof(Model));

	//�I�������܂œǂݍ���
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);
		//�z�u���m�F
		if (strcmp(&aStr[0], LOAD_MODELTYPE) == 0)
		{//���f�����
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &Model.nType);	//�g�p���郂�f���ԍ��ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//���W
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &Model.pos.x);	//x���W�ǂݍ���
			fscanf(pFile, "%f", &Model.pos.y);	//y���W�ǂݍ���
			fscanf(pFile, "%f", &Model.pos.z);	//z���W�ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//����
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%f", &Model.rot.x);	//x���W�ǂݍ���
			fscanf(pFile, "%f", &Model.rot.y);	//y���W�ǂݍ���
			fscanf(pFile, "%f", &Model.rot.z);	//z���W�ǂݍ���
		}
		else if (strcmp(&aStr[0], LOAD_SHADOW) == 0)
		{//�e
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &Model.nShadow);	//�e�ǂݍ���
		}

		//�I��
		if (strcmp(&aStr[0], ENDMODELSET_TXT) == 0)
		{//�I������
			break;
		}
	}

	SetModel(Model.pos, D3DXToRadian(Model.rot), Model.nType, Model.nShadow);
}

//==========================================================
//���f���̏��擾
//==========================================================
Model *GetModel(void)
{
	return &g_aModel[0];
}

//==========================================================
//�w��ԍ����f�����g�p����Ă��Ȃ���Ԃɂ���
//==========================================================
void DeleteModel(int nCntModel)
{
	g_aModel[nCntModel].bUse = false;			//�g�p����Ă��Ȃ���Ԃɂ���
	g_aModel[nCntModel].pModelFiledata = NULL;	//���f���̃f�[�^��NULL�ɂ���
}

//==========================================================
//���f���̌����ɂ�铖���蔻��T�C�Y�̒���
//==========================================================
void RotSetVtxSize(D3DXVECTOR3 *SetVtxMax, D3DXVECTOR3 *SetVtxMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, float fRot)
{
	//�����ɂ���ĕύX����
	if (fRot == 0.0f || fRot == -0.0f)
	{//������
		//�ő�l���f
		SetVtxMax->x = vtxMax.x;	//x���W
		SetVtxMax->y = vtxMax.y;	//y���W
		SetVtxMax->z = vtxMax.z;	//z���W

		//�ŏ��l���f
		SetVtxMin->x = vtxMin.x;	//x���W
		SetVtxMin->y = vtxMin.y;	//y���W
		SetVtxMin->z = vtxMin.z;	//z���W
	}
	if (fRot == D3DX_PI * 0.5f)
	{//������
		//�ő�l���f
		SetVtxMax->x = vtxMax.z;	//x���W
		SetVtxMax->y = vtxMax.y;	//y���W
		SetVtxMax->z = -vtxMin.x;	//z���W

		//�ŏ��l���f
		SetVtxMin->x = vtxMin.z;	//x���W
		SetVtxMin->y = vtxMin.y;	//y���W
		SetVtxMin->z = -vtxMax.x;	//z���W
	}
	else if (fRot == -D3DX_PI * 0.5f)
	{//�E����
	 //�ő�l���f
		SetVtxMax->x = -vtxMin.z;	//x���W
		SetVtxMax->y = vtxMax.y;	//y���W
		SetVtxMax->z = vtxMax.x;	//z���W

		//�ŏ��l���f
		SetVtxMin->x = -vtxMax.z;	//x���W
		SetVtxMin->y = vtxMin.y;	//y���W
		SetVtxMin->z = vtxMin.x;	//z���W
	}
	else if (fRot == -D3DX_PI || fRot == D3DX_PI)
	{//���]���Ă���ꍇ
		//�����ȊO�̍ő�l���ŏ��l�ɂ���
		SetVtxMax->x = -vtxMin.x;	//x���W
		SetVtxMax->y = vtxMax.y;	//y���W
		SetVtxMax->z = -vtxMin.z;	//z���W

		//�����ȊO�̍ŏ��l���ő�l�ɂ���
		SetVtxMin->x = -vtxMax.x;	//x���W
		SetVtxMin->y = vtxMin.y;	//y���W
		SetVtxMin->z = -vtxMax.z;	//z���W
	}
	else
	{//����ȊO�̏ꍇ
		//�ő�l���f
		SetVtxMax->x = vtxMax.x;	//x���W
		SetVtxMax->y = vtxMax.y;	//y���W
		SetVtxMax->z = vtxMax.z;	//z���W

		//�ŏ��l���f
		SetVtxMin->x = vtxMin.x;	//x���W
		SetVtxMin->y = vtxMin.y;	//y���W
		SetVtxMin->z = vtxMin.z;	//z���W
	}
}