//===========================
//
//�_�C���N�gX.enemy�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//�G�̎��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,			//�ʏ���
	ENEMYSTATE_DAMAGE,				//�_���[�W���
	ENEMYSTATE_MAX
}ENEMYSTATE;

//�v���C���[�̍\����
typedef struct
{
	LPDIRECT3DTEXTURE9 apTextureEnemy[8] = {};
	LPD3DXMESH pMeshEnemy = NULL;
	LPD3DXBUFFER pBuffMatEnemy = NULL;					//���_�����i�[
	DWORD dwNumMatEnemy = 0;
	D3DXVECTOR3 vtxMinEnemy;
	D3DXVECTOR3 vtxMaxEnemy;
}EnemyX;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posFile;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 point;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 rotFile;
	D3DXVECTOR3 vtxMinEnemy;
	D3DXVECTOR3 vtxMaxEnemy;
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	ENEMYSTATE state;
	D3DXCOLOR col[8];
	EnemyX aEnemyX;
	int Parent;
	int nTimer;
	int nMoveTimer;
	int nMoveTime;
	int nLife;
	int nLifeFile;
	int nLifeCounter;
	int nType;
	int nTypeFile;
	int nTimerState;
	float fLengthMin;
	float fLengthMax;
	float fRot;
	bool bUse;
}Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void InitEnemyGame(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nLife, int nType);
Enemy *GetEnemy(void);
bool CollisionEnemy(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, int nType);
void ReadFileEnemy(void);

#endif // !_ENEMY_H_