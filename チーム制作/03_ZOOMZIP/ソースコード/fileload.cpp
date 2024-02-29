//==========================================================
//
//�O���t�@�C���ǂݍ��ݏ��� [fileload.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "fileload.h"
#include <stdio.h>
#include <string.h>
#include "texture.h"
#include "meshfield.h"
#include "meshwall.h"
#include "model.h"
#include "result.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_FILESTRING	(256)	//�t�@�C���̕����ő�
#define GAME_TXTFILE		"data\\TXT\\game.txt"		//�Q�[����ʂŎg�p����ǂݍ��ݗp�t�@�C��
#define TUTORIAL_TXTFILE	"data\\TXT\\tutorial.txt"	//�`���[�g���A����ʂŎg�p����ǂݍ��ݗp�t�@�C��
#define CHECKTXT			"data\\TXT\\check.txt"		//�f�o�b�O�p�m�F�}�b�v
#define TITLE_TXTFILE		"data\\TXT\\title.txt"		//�^�C�g����ʗp�ǂݍ��݃t�@�C��
#define GAMEOVERTXTFILE		"data\\TXT\\gameover.txt"	//�Q�[���I�[�o�[�ǂݍ��݃t�@�C��
#define GAMECLEARTXTFILE	"data\\TXT\\result.txt"	//�Q�[���N���A�ǂݍ��݃t�@�C��
#define RANKINGTXTFILE		"data\\TXT\\ranking.txt"	//�����L���O��ʗp�ǂݍ��݃t�@�C��

#define CHACKTXT_NUM	(0)		//�e�L�X�g�t�@�C���؂�ւ��B[0]���Q�[���p,[1]���`�F�b�N�p

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void LoadTxtFileData(FILE *pFile);
void LoadTextureNum(FILE *pFile);
void LoadTextureFileName(FILE *pFile);
void LoadModelNum(FILE *pFile);
void LoadModelFileName(FILE *pFile);

//==========================================================
//�O���[�o���ϐ��錾
//==========================================================
char g_aTextureFileName[MAX_TEXTUREFILE][MAX_FILESTRING];	//�e�N�X�`���f�[�^�̃t�@�C�����i�[�p
char g_aModelFileName[MAX_MODELFILE][MAX_FILESTRING];		//���f���f�[�^�̃t�@�C�����i�[�p
int g_nTextureNum;		//�ǂݍ��񂾃e�N�X�`������ێ�
int g_nModelNum;		//�ǂݍ��񂾃��f������ێ�

//==========================================================
//�t�@�C���ǂݍ��ݏ���������
//==========================================================
void InitFileLoad(void)
{
	//�e�N�X�`���t�@�C����������
	memset(&g_aTextureFileName, '\0', sizeof(g_aTextureFileName));

	//���f���t�@�C����������
	memset(&g_aModelFileName, '\0', sizeof(g_aModelFileName));

	//�ǂݍ��ݐ�������
	g_nTextureNum = 0;		//�e�N�X�`����
	g_nModelNum = 0;		//���f����
}

//==========================================================
//�t�@�C���ǂݍ��ݏI������
//==========================================================
void UninitFileLoad(void)
{

}

//==========================================================
//�t�@�C���ǂݍ��ݍX�V����
//==========================================================
void UpdateFileLoad(void)
{

}

//==========================================================
//�z�u�e�L�X�g�t�@�C���ǂݍ��ݏ���
//==========================================================
void SetFileLoad(void)
{

}

//==========================================================
//�Q�[�����[�h�p�ǂݍ��݃t�@�C��
//==========================================================
void GametxtFileLoad(void)
{
	FILE *pFile;

	switch (GetMode())
	{
	case MODE_TITLE:	//�^�C�g�����
		pFile = fopen(TITLE_TXTFILE, "r");
		break;
	case MODE_TUTORIAL:	//�`���[�g���A�����
		pFile = fopen(TUTORIAL_TXTFILE, "r");
		break;
	case MODE_RESULT:	//���U���g���
	
			pFile = fopen(GAMECLEARTXTFILE, "r");
		break;
	case MODE_RANKING:	//�����L���O���
		pFile = fopen(RANKINGTXTFILE, "r");
		break;
	default:	//�Q�[�����
#if CHACKTXT_NUM
		pFile = fopen(CHECKTXT, "r");
#else
		pFile = fopen(GAME_TXTFILE, "r");
#endif 
		break;
	}

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		char aStr[256];

		//�J�n�����܂œǂݍ���
		while (1)
		{
			//�e�L�X�g�ǂݍ���
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], START_SCRIPTTXT) == 0)
			{//�X�N���v�g�J�n�̕������m�F�ł����ꍇ
			 //�e�f�[�^�̓ǂݍ��݊J�n
				LoadTxtFileData(pFile);

				break;
			}
			else if (nResult == EOF)
			{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
				break;
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}

	//���b�V���t�B�[���h�o�b�t�@����
	CreateFieldBuff();

	//���b�V���E�H�[���o�b�t�@����
	CreateWallBuff();

	//���f���t�@�C���ǂݍ���
	LoadModelFile(g_nModelNum);

}

//==========================================================
//�t�@�C�����e�ǂݍ��ݏ���
//==========================================================
void LoadTxtFileData(FILE *pFile)
{
	char aStr[256];	//�]���ȕ��͓ǂݍ��ݗp

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		//�ǂݍ��ݓ��e�m�F
		if (strcmp(&aStr[0], TEXTURENUM_TXT) == 0)
		{//�g�p����e�N�X�`�������̏ꍇ
			LoadTextureNum(pFile);
		}
		else if (strcmp(&aStr[0], MODELNUM_TXT) == 0)
		{//�g�p���郂�f�����̏ꍇ
			LoadModelNum(pFile);
		}

		//�t�@�C�����m�F
		if (strcmp(&aStr[0], TEXTUREFILENAME_TXT) == 0)
		{//�e�N�X�`���t�@�C�����̏ꍇ
			LoadTextureFileName(pFile);
		}
		else if (strcmp(&aStr[0], MODELFILENAME_TXT) == 0)
		{//���f���t�@�C�����̏ꍇ
			LoadModelFileName(pFile);
		}
		else if (strcmp(&aStr[0], MODELFILE_TXT) == 0)
		{//���f���t�@�C�����̏ꍇ
			LoadModelFileData(pFile);
		}

		//�z�u���m�F
		if (strcmp(&aStr[0], WALLSET_TXT) == 0)
		{//�ǔz�u�̏ꍇ
			LoadMeshWallData(pFile);
		}
		else if (strcmp(&aStr[0], FIELDSET_TXT) == 0)
		{//���z�u�̏ꍇ
			LoadMeshFieldData(pFile);
		}
		else if (strcmp(&aStr[0], MODELSET_TXT) == 0)
		{//���f���z�u�̏ꍇ
			LoadModelData(pFile);
		}
		else if (strcmp(&aStr[0], ITEMSET_TXT) == 0)
		{//�p�[�c�z�u�̏ꍇ
			//LoadItemData(pFile);
		}
		else if (strcmp(&aStr[0], PARTSSET_TXT) == 0)
		{//�p�[�c�z�u�̏ꍇ
			//LoadPartsData(pFile);
		}


		//�I���m�F
		if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//�X�N���v�g�J�n�̕������m�F�ł����ꍇ
			break;
		}
		else if (nResult == EOF)
		{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
			break;
		}
	}
}

//==========================================================
//�e�N�X�`�������擾
//==========================================================
void LoadTextureNum(FILE *pFile)
{
	char aStr[4];	//�]���ȕ��͓ǂݍ��ݗp

	fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
	fscanf(pFile, "%d", &g_nTextureNum);	//�g�p�����ǂݍ���
}

//==========================================================
//�e�N�X�`���t�@�C�����擾
//==========================================================
void LoadTextureFileName(FILE *pFile)
{
	char aStr[4];	//�]���ȕ��͓ǂݍ��ݗp

	fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���

	for (int nCntTexture = 0; nCntTexture < g_nTextureNum; nCntTexture++)
	{
		if (g_aTextureFileName[nCntTexture][0] == '\0')
		{//�t�@�C����������ꏊ���g���Ă��Ȃ��ꍇ
			fscanf(pFile, "%s", &g_aTextureFileName[nCntTexture][0]);	//(=)�ǂݍ���
			break;
		}
	}
}

//==========================================================
//���f�����擾
//==========================================================
void LoadModelNum(FILE *pFile)
{
	char aStr[4];	//�]���ȕ��͓ǂݍ��ݗp

	fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
	fscanf(pFile, "%d", &g_nModelNum);	//�g�p���ǂݍ���
}

//==========================================================
//���f���t�@�C�����擾
//==========================================================
void LoadModelFileName(FILE *pFile)
{
	char aStr[4];	//�]���ȕ��͓ǂݍ��ݗp

	fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���

	for (int nCntModel = 0; nCntModel < g_nModelNum; nCntModel++)
	{
		if (g_aModelFileName[nCntModel][0] == '\0')
		{//�t�@�C����������ꏊ���g���Ă��Ȃ��ꍇ
			fscanf(pFile, "%s", &g_aModelFileName[nCntModel][0]);	//�t�@�C�����ǂݍ���
			break;
		}
	}
}

//==========================================================
//�e�N�X�`���ǂݍ��ݐ��擾
//==========================================================
int GetTextureCnt(void)
{
	return g_nTextureNum;
}

//==========================================================
//�e�N�X�`���t�@�C�����擾
//==========================================================
const char *pTextureFileName(int nCntTxture)
{
	return &g_aTextureFileName[nCntTxture][0];
}

//==========================================================
//���f���ǂݍ��ݐ��擾
//==========================================================
int GetModelCnt(void)
{
	return g_nModelNum;
}

//==========================================================
//���f���t�@�C�����擾
//==========================================================
const char *pModelFileName(int nCntModel)
{
	return &g_aModelFileName[nCntModel][0];
}