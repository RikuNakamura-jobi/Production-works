//===========================
//
//�_�C���N�gX.enemy�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//�}�N����`
#define MAX_ENEMY_MODEL (64)
#define MAX_MOTION_KEY (64)
#define MAX_POINT (32)			//�G�l�~�[�̈ړ��|�C���g�̍ő吔
#define ENEMY_LIFE (500) //�����̗�
#define MAX_PATTERN (8)			//�G�l�~�[�̈ړ��p�^�[���̍ő吔
#define ENEMY_RADIUS	(350.0f)	//�����蔻��̋��̑傫��
#define ENEMY_SPEED (0.3f)				//���x�␳�̒l

//�G�l�~�[�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,			//�ʏ���
	ENEMYSTATE_SEARCH,				//���G���
	ENEMYSTATE_LOST,				//�����������
	ENEMYSTATE_FIND,				//�������
	ENEMYSTATE_TRACK,				//�ǐՏ��
	ENEMYSTATE_SLEEP,				//�������
	ENEMYSTATE_STANDUP,				//�N���オ����
	ENEMYSTATE_DAMAGE,				//�_���[�W���
	ENEMYSTATE_COUNTER,				//�������
	ENEMYSTATE_JUMPATTACK,			//�W�����v�U�����
	ENEMYSTATE_DEATH,				//���S���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�G�l�~�[�̎��
typedef enum
{
	ENEMYMOTION_NORMAL = 0,			//�j���[�g�������[�V����
	ENEMYMOTION_MOVE,				//�ړ����[�V����
	ENEMYMOTION_SHAKE,				//�U�艺�낵���[�V����
	ENEMYMOTION_PIERCE,				//�˂��h�����[�V����
	ENEMYMOTION_STEP,				//�����݃��[�V����
	ENEMYMOTION_THROW,				//���΂����[�V����
	ENEMYMOTION_OVERLOOK,			//���n�����[�V����
	ENEMYMOTION_DAMAGE,				//��e���[�V����
	ENEMYMOTION_DOWN,				//�_�E�����[�V����
	ENEMYMOTION_BACKJUMP,			//�o�b�N�W�����v���[�V����
	ENEMYMOTION_JUMPATTACK,			//�W�����v�U�����[�V����
	ENEMYMOTION_FIND,				//�������[�V����
	ENEMYMOTION_DEATH,				//���S���[�V����
	ENEMYMOTION_JUMPATTACK_LIGHT,	//��W�����v�U�����[�V����
	ENEMYMOTION_SLEEP,				//�x�����[�V����
	ENEMYMOTION_STANDUP,			//�����オ�胂�[�V����
	ENEMYMOTION_MAX
}ENEMYMOTION;

//�G�l�~�[�̍\����
typedef struct
{
	D3DXVECTOR3 point[MAX_POINT];		//����n�_
	int nNumPoint;
	int posCount = 0;					//�ړ���̃|�C���g�ԍ�
}MovePattern;

//�G�l�~�[�̍\����
typedef struct
{
	D3DXVECTOR3 pos[MAX_ENEMY_MODEL];
	D3DXVECTOR3 rot[MAX_ENEMY_MODEL];
	int nFrame;
}KeyMotion;

//�G�l�~�[�̍\����
typedef struct
{
	KeyMotion aKeyMotion[MAX_MOTION_KEY];
	int nLoop;
	int nNumKey;
	int nKey;
}EnemyMotion;

//�G�l�~�[�̃��f���\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posMotion;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotMotion;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 apTextureEnemy[8] = {};
	LPD3DXMESH pMeshEnemy = NULL;
	LPD3DXBUFFER pBuffMatEnemy = NULL;					//���_�����i�[
	DWORD dwNumMatEnemy = 0;
	D3DXVECTOR3 vtxMinEnemy;
	D3DXVECTOR3 vtxMaxEnemy;
	int nIdxModelParent;
	char cModelName[128];
}EnemyX;

//�G�l�~�[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 posOld;					//�O�̈ʒu
	D3DXVECTOR3 move;					//�ړ���
	D3DXVECTOR3 rot;					//�p�x
	D3DXVECTOR3 vtxMin;
	D3DXVECTOR3 vtxMax;
	D3DXMATRIX mtxWorld;				//���[���h�}�g���b�N�X
	D3DXVECTOR3 pointMove;				//�ړ��n�_
	ENEMYSTATE state;					//���
	ENEMYSTATE stateOld;					//���
	ENEMYMOTION motionState;
	ENEMYMOTION motionStateOld;
	EnemyMotion aEnemyMotion[ENEMYMOTION_MAX];
	EnemyX aEnemyX[MAX_ENEMY_MODEL];						//���f�����
	MovePattern MovePattern[MAX_PATTERN];
	int nTimerMove;						//�ړ��^�C�}�[
	int nFindCount;						//��������
	int nAttackCount;					//�U���ڍs����
	int nLife;							//�̗�
	int nNumModel;						//���f���̐�
	int nIdxGauge;						//�Q�[�W�̔ԍ�
	int nIdxShadow;						//�e�̔ԍ�
	int nIdxOrbit;						//�O�Ղ̔ԍ�
	int nCntKey;
	int nCntFrame;
	int nNumPattern;
	int nPattern;
	int nAttackTime;
	int nHitRock;
	float vecL;							//���̊O��
	float vecR;							//�E�̊O��
	float fLength;						//�v���C���[�Ƃ̋���
	float fSpeed;						//�G�l�~�[�̑���
	bool bFind;							//�����t���O
	bool bUse;							//�g�p�t���O
	bool bDown;							//�g�p�t���O
	bool bDeath;						//���S�t���O
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nType);
Enemy *GetEnemy(void);
bool CollisionEnemy(D3DXVECTOR3 *posMin, D3DXVECTOR3 *posMax);
bool CollisionEnemyAttack(D3DXVECTOR3 *pos, D3DXVECTOR3 *rot);
bool CollisionEnemyPoint(D3DXVECTOR3 *pos);
void ReadFileEnemy(void);
void SetPosEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitEnemy(int nDamage, int nCntEnemy);
bool GetEnemyUse(void);
float OuterProduct(D3DXVECTOR3 vecA, D3DXVECTOR3 vecB, D3DXVECTOR3 vecC);
void DrawEnemyDebug(void);
void DebugEnemyPos(void);
void DebugEnemyPoint(void);
void DebugEnemyOuter(void);
void DebugEnemyFind(void);
void DebugEnemyLength(void);
void DebugEnemyRot(void);
void DebugEnemyMotion(void);
void DebugEnemyState(void);

#endif // !_ENEMY_H_