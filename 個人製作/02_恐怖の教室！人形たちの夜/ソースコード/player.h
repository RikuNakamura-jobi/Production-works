//===========================
//
//�_�C���N�gX.player�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//�}�N����`
#define MAX_PLAYER_MODEL (32)
#define MAX_MOTION_KEY (32)

//�v���C���[�̎��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,			//�o�����
	PLAYERSTATE_NORMAL,				//�ʏ���
	PLAYERSTATE_MOVE,				//�ړ����
	PLAYERSTATE_DAMAGE,				//�_���[�W���
	PLAYERSTATE_SHOT,				//�ˌ����
	PLAYERSTATE_DEATH,				//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̎��
typedef enum
{
	PLAYERMOTION_NORMAL = 0,		//�ҋ@���[�V����
	PLAYERMOTION_MOVE,				//�ړ����[�V����
	PLAYERMOTION_ACTION,			//�A�N�V�������[�V����
	PLAYERMOTION_TITLE,				//�W�����v���[�V����
	PLAYERMOTION_CLEAR,				//���n���[�V����
	PLAYERMOTION_DEATH,				//���n���[�V����
	PLAYERMOTION_RAND,				//���n���[�V����
	PLAYERMOTION_MAX
}PLAYERMOTION;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 apTexturePlayer[8] = {};
	LPD3DXMESH pMeshPlayer = NULL;
	LPD3DXBUFFER pBuffMatPlayer = NULL;					//���_�����i�[
	DWORD dwNumMatPlayer = 0;
	int nIdxModelParent;
}PlayerModel;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_PLAYER_MODEL];
	D3DXVECTOR3 rot[MAX_PLAYER_MODEL];
	int nFrame;
}KeyMotion;

//�v���C���[�̍\����
typedef struct
{
	KeyMotion aKeyMotion[MAX_MOTION_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}PlayerMotion;

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
	PLAYERSTATE state;
	PLAYERMOTION motionState;
	PLAYERMOTION motionStateOld;
	PlayerModel aPlayerModel[MAX_PLAYER_MODEL];
	PlayerMotion aPlayerMotion[PLAYERMOTION_MAX];
	int nNumModel;
	int nLife;
	int nTimerState;
	float fJump;
	float fMove;
	float fAngle;
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void InitPlayerTitle(void);
void InitPlayerResult(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void UpdatePlayerResult(void);
void DrawPlayer(void);
Player *GetPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFilePlayer(void);

#endif // !_PLAYER_H_