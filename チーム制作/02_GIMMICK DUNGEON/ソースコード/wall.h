//========================================
//
//3D���f���̕ǃ|���S������[wall.h]
//Author:Kai Takada
//
//========================================
#ifndef _WALL_H_								//���̃}�N����`������ĂȂ�������
#define _WALL_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

#define MAX_WALL (8)			//�ő�ǔz�u��

//�ǂ̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	float fHeight;				//����
	float fWidth;				//��
	float fDivX;				//�e�N�X�`��������x
	float fDivY;				//�e�N�X�`��������z
	D3DXVECTOR3 aVtxpos[4];		//�l���_�̍��W
	bool bUse;					//�g�p���Ă��邩
	int nIdx;					//�C���f�b�N�X
}Wall;

//�v���g�^�C�v�錾
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
void DeleteWall(int nIdx);
Wall *GetWall(void);
int GetUseWall(void);
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength);
void UpdateWallRanking(float moveX);

#endif