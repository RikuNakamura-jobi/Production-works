//==========================================================
//
//��Q������ [enemy.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_H_	 //���̃}�N����`����`����Ă��Ȃ�������
#define _ENEMY_H_    //2�A�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"    //�쐬����main.h���C���N���[�h����
#include "polygon.h"

//==========================================================
//�񋓌^�̒�`
//==========================================================
typedef enum
{
	ENEMYTYPE_BIED = 0,	//��
	ENEMYTYPE_JET,		//��s�@
	ENEMYTYPE_CLOUD,	//���_
	ENEMYTYPE_METEOR,	//覐�
	ENEMYTYPE_MAX
}ENEMYTYPE;

//==========================================================
//�\���̂̒�`
//==========================================================
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col;		//�F
	float fAngle;		//�p�x
	float fLength;		//�Ίp���̒���
	float fHeight;		//����
	float fWidth;		//��
	bool bUse;			//�g�p���Ă��邩�ǂ���
	int nType;			//���
	int nTexCounter;	//�e�N�X�`���J�E���^�[
	int nTexAnim;		//�e�N�X�`�����W
	int nRot;			//������
	int nNear;			//�j�A�~�X�������ǂ���
}Enemy;

typedef struct
{
	polygon Polygon;
	D3DXVECTOR3 move;	//�ړ���
	bool bUse;			//�g�p���Ă��邩�ǂ���
	int nLife;
}NearMiss;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, float fWidth, float fHeight, int nType);
bool CollisionEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 posOld);
void SetNearMiss(void);

#endif
