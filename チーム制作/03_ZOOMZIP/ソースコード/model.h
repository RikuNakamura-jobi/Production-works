//==========================================================
//
//X�t�@�C�����f������ [model.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include
#include <stdio.h>

//==========================================================
//�}�N����`
//==========================================================
#define MAX_MODELTEX	(64)		//X�t�@�C���e�N�X�`���i�[�ő吔
#define MAX_MODEL		(1024)		//�z�u�ł��郂�f���̍ő吔
#define MAX_MODELFILE	(256)		//�ǂݍ��߂郂�f���̍ő吔
#define CATCH_LENGTH	(115.0f)	//�����̔���͈�
#define GRAB_HEIGHT		(100.0f)	//�͂ލ���
#define GRAB_LENGTH		(150.0f)	//�͂߂鍂��

//==========================================================
//�񋓌^�̒�`
//==========================================================
//���f���^�C�v
typedef enum
{
	MODELTYPE_OBJECT = 0,		//��Q��
	MODELTYPE_BLOCK,			//���������\
	MODELTYPE_RAIL,				//���[��
	MODELTYPE_RACK,				//����
	MODELTYPE_DECO,				//����
	MODELTYPE_ITEM,				//�A�C�e��
	MODELTYPE_CONTAINER,		//�R���e�i
	MODELTYPE_BOX,				//������R���e�i
	MODELTYPE_LIFTUP,			//�������u
	MODELTYPE_CRANECTRL,		//�N���[�������
	MODELTYPE_MAX
}MODELTYPE;

//���f���^�C�v
typedef enum
{
	WEIGHT_LIGHT = 0,		//�y��(�d��1)
	WEIGHT_HEAVY,			//�d��(�d��2)
	WEIGHT_OBJECT,			//�����Ȃ�
	WEIGHT_MAX
}WEIGHT;

//==========================================================
//�\���̂̒�`
//==========================================================
//���f���t�@�C���f�[�^
typedef struct
{
	LPD3DXMESH pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
	LPDIRECT3DTEXTURE9 apTexture[MAX_MODELTEX] = {};	//�e�N�X�`���̃|�C���^
	DWORD dwNumMat = 0;					//�}�e���A���̐�
	D3DXVECTOR3 vtxMin;					//�ŏ��l
	D3DXVECTOR3 vtxMax;					//�ő�l
	int nType;							//�^�C�v
}ModelFileData;

//���f�����(model.cpp)
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 posOld;					//�O��̈ʒu
	D3DXVECTOR3 rot;					//����
	float fMoveY;						//�d��
	D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
	ModelFileData *pModelFiledata;		//���f���t�@�C���f�[�^�̃|�C���^
	int nShadow;						//�g�p���Ă���e�̔ԍ�
	int nType;							//���
	bool bUse;							//�g�p���Ă��邩�ǂ���
}Model;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
ModelFileData *GetModelData(int nModelType);
void LoadModelFile(int nMaxModelFile);
void LoadModelFileData(FILE *pFile);
void LoadModelData(FILE *pFile);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nSetShadow);
Model *GetModel(void);
void DeleteModel(int nCntModel);

#endif