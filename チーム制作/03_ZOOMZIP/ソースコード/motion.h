//===================================================
//
//�K�w�\���ɂ�郂�[�V�������� [motion.h]
//Author Ibuki Okusada
//
//===================================================
#ifndef _MOITON_H_
#define _MOITON_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include
#include "model.h"

//===================================================
//�}�N����`
//===================================================
#define MAX_PARTS	(32)	//�p�[�c�̍ő吔
#define MAX_KEY		(64)	//�L�[�̍ő吔
#define MAX_MOTION	(64)	//���[�V�����̍ő吔

//===================================================
//�񋓌^�̒�`
//===================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	//�ҋ@���[�V����
	MOTIONTYPE_WALK,		//�ړ����[�V����
	MOTIONTYPE_ACTION,		//�A�N�V�������[�V����
	MOTIONTYPE_JUMP,		//�W�����v���[�V����
	MOTIONTYPE_LAND,		//���n���[�V����
	MOTIONTYPE_MAX
}MOTIONTYPE;

//===================================================
//�\���̂̒�`
//===================================================
//�L�[
typedef struct
{
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;
}KEY;

//�L�[���
typedef struct
{
	int nFrame;					//�Đ��t���[��
	KEY aKey[MAX_PARTS];		//�p�[�c�̑���
}KEY_INFO;

//���[�V�������
typedef struct
{
	bool bLoop;					//���[�v���邩�ǂ���
	int nNumKey;				//�L�[��
	KEY_INFO aKeyInfo[MAX_KEY];	//�L�[�̍ő吔
}MOTION_INFO;

//�p�[�c�̏��
typedef struct
{
	ModelFileData FileData;		//X�t�@�C���f�[�^
	Model model;				//���f�����
	D3DXVECTOR3 SetPos;			//�����ݒu���W
	D3DXVECTOR3 SetRot;			//�����ݒu����
	int nIndex;					//�����̃C���f�b�N�X�ԍ�
	int nIdexParent;			//�e�̃C���f�b�N�X�ԍ�
}Parts;

//���̃t�@�C�����
typedef struct
{
	MOTION_INFO aMotionInfo[MAX_MOTION];	//���[�V�����̑���
	Parts aParts[MAX_PARTS];			//�p�[�c�̑���
	int nNumParts;						//�p�[�c��
}BodyFileData;

//���g���
typedef struct
{
	BodyFileData FileData;	//�t�@�C�����
	KEY aSet[MAX_PARTS];		//�p�[�c�g�p���
	D3DXMATRIX aSetMtx[MAX_PARTS];	//�p�[�c���
	D3DXVECTOR3 pos;			//���W
	D3DXVECTOR3 rot;			//����
	int nNowFrame;				//���݂̃t���[����
	int nNowMotion;				//���݂̃��[�V�����ԍ�
	int nNowKey;				//���݂̃L�[��
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
}Body;

//===================================================
//�v���g�^�C�v�錾
//===================================================
void InitMotion(void);
void UninitMotion(Body *pBody);
void UpdateMotion(Body *pBody);
void SetMotion(Body *pBody, int nMotionType);

//�O���t�@�C���ǂݍ���
void OpenBodyFile(const char *pFileData, BodyFileData *pBodyFile);
void LoadBodyFile(FILE *pFile, BodyFileData *pBodyFile);
void LoadBodyXFileName(FILE *pFile, BodyFileData *pBodyFile);
void LoadPartsSetData(FILE *pFile, BodyFileData *pBodyFile);
void LoadPartsFileData(Parts *pParts, const char *pFileName);
void LoadMotionData(FILE *pFile, BodyFileData *pBodyFile, int nMotion);
void ResetBodyData(Body * pBody);
void ResetBodyFileData(Body * pBody);

#endif
