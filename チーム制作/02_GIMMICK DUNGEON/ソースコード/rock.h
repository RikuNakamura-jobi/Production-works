//========================================
//
//2D�V���[�e�B���O�Q�[��(�A�C�e���擾����)[getitem.h]
//Author:���v�ԗD��
//
//========================================
#ifndef _ROCK_H_							//���̃}�N����`������Ă��Ȃ�������
#define _ROCK_H_							//2�d�C���N���[�h�h�~�̃}�N�����`����v

#include"main.h"

//========================
// �}�N����`
//========================
#define MAX_ROCK	(256)							//���f���̍ő吔
#define SPEED_ROCK	(2.3f)							//��̃X�s�[�h
#define LIFE_ROCK	(6000)							//��̎���
#define ROCK_TIME	(30)							//��̖��G����
#define ROLLSOUND_RANGE		(900.0f)				//�]���鉹�̕�������͈�

//========================
// �񋓌^��`
//========================
// �}�O�X�g�[���̎��
typedef enum
{
	ROCKTYPE_GIMMICK = 0,					//�]������
	ROCKTYPE_THROW,						//��΂��z
	ROCKTYPE_INVISIBLE,		//�s���^�C�v
	ROCKTYPE_DROP,		//�����^�C�v
	ROCKTYPE_MAX
}ROCKTYPE;

// ���f���̎��
typedef enum
{
	ROCKSTATE_NORMAL = 0,						//�ʏ�
	ROCKSTATE_DAMEGE,						//�_���[�W���
	ROCKSTATE_DEATH,						//���S���
	ROCKSTATE_MAX
}ROCKSTATE;

//========================
// �\���̒�`
//========================
// ���f���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;									//�ʒu
	D3DXVECTOR3 posOld;									//�ʒu
	D3DXVECTOR3 move;									//�ړ���
	D3DXVECTOR3 rot;									//����
	D3DXVECTOR3 vtxMin;									//�ŏ�
	D3DXVECTOR3 vtxMax;									//�ő�
	D3DXMATRIX mtxWorld;								//���[���h�}�g���b�N�X
	float fSpeed;										//����
	int nCounter;										//�J�E���^�[
	int nType;											//���
	bool bUse;											//�g�p���Ă��邩�ǂ���
	bool bWallInvisible;								//�ǂɓ����������ǂ���
	ROCKSTATE state;									//���
	int nCounterState;									//��ԊǗ��J�E���^�[
	D3DXMATERIAL RedMat;								//�}�e���A���f�[�^�ւ̃|�C���^
	int nLife;											//�̗�
	int nldxModelParent;								//�e���f���̃C���f�b�N�X��
	int nIdxEditModel;									//�Ώۂ̃G�f�B�b�g�̃C���f�b�N�X(�ԍ�)
	int nHitRockCounter;									//��̃J�E���^�[
	int nCntSound;										//����炷�J�E���^�[
	int nIdxShadow;										//�e�̔ԍ�
	int nIdxOrbit;										//�O�Ղ̔ԍ�
}Rock;

//========================
// �v���g�^�C�v�錾
//========================
//��{����
void InitRock(void);
void UninitRock(void);
void UpdateRock(void);
void DrawRock(void);

//�����蔻��
void CollisionRockPlayer(int nCntRock);
void CollisionRockEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pPoint, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump);
void HitRock(int nDamage, int nCntRock);

//�ݒ菈��
void SetRock(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, int nType);
void DisableRock(Rock *pRock);	//�����������

//���擾����
Rock *GetRock(void);
int *GetHitRockNum(void);

//���̑�
void RockRollSound(Rock *pRock,bool *bSound);
void PlaySoundRockCrash(Rock *pRock);

#endif