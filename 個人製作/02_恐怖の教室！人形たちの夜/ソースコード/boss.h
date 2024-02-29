//===========================
//
//�_�C���N�gX.boss�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _BOSS_H_
#define _BOSS_H_

//�}�N����`
#define MAX_BOSS_MODEL (32)
#define MAX_MOTIONBOSS_KEY (32)

//�v���C���[�̎��
typedef enum
{
	BOSSSTATE_APPEAR = 0,			//�o�����
	BOSSSTATE_NORMAL,				//�ʏ���
	BOSSSTATE_MOVE,				//�ړ����
	BOSSSTATE_DAMAGE,				//�_���[�W���
	BOSSSTATE_SHOT,				//�ˌ����
	BOSSSTATE_DEATH,				//���S���
	BOSSSTATE_MAX
}BOSSSTATE;

//�v���C���[�̎��
typedef enum
{
	BOSSMOTION_NORMAL = 0,		//�ҋ@���[�V����
	BOSSMOTION_MOVE,				//�ړ����[�V����
	BOSSMOTION_ACTION,			//�A�N�V�������[�V����
	BOSSMOTION_JUMP,				//�W�����v���[�V����
	BOSSMOTION_RAND,				//���n���[�V����
	BOSSMOTION_MAX
}BOSSMOTION;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 apTextureBoss[8] = {};
	LPD3DXMESH pMeshBoss = NULL;
	LPD3DXBUFFER pBuffMatBoss = NULL;					//���_�����i�[
	DWORD dwNumMatBoss = 0;
	int nIdxModelParent;
}BossModel;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_BOSS_MODEL];
	D3DXVECTOR3 rot[MAX_BOSS_MODEL];
	int nFrame;
}KeyMotion;

//�v���C���[�̍\����
typedef struct
{
	KeyMotion aKeyMotion[MAX_MOTIONBOSS_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}BossMotion;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 point;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	D3DXVECTOR3 vtxMinModel;
	D3DXVECTOR3 vtxMaxModel;
	BOSSSTATE state;
	BOSSMOTION motionState;
	BOSSMOTION motionStateOld;
	BossModel aBossModel[MAX_BOSS_MODEL];
	BossMotion aBossMotion[BOSSMOTION_MAX];
	int nNumModel;
	int nLife;
	int nTimerState;
	float fJump;
	float fMove;
	float fAngle;
}Boss;

//�v���g�^�C�v�錾
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
Boss *GetBoss(void);
bool CollisionBoss(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFileBoss(void);

#endif // !_BOSS_H_