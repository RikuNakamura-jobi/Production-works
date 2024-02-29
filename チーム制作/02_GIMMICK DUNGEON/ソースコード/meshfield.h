//==================================================================================================
//
//meshfield.h
//Author:���R����
//
//==================================================================================================

#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//�}�N����`
#define FIELD_RANGE					(30.0f)									//���b�V���̈�ӂ̒���
#define MESH_U						(100)											//���̃u���b�N��
#define MESH_V						(100)											//�c�̃u���b�N��
#define MESH_TUTORIAL_U				(50.0f)											//���̃u���b�N��
#define MESH_TUTORIAL_V				(50.0f)											//�c�̃u���b�N��

//�\���̐錾
typedef struct
{
	D3DXVECTOR3 pos;						//�ʒu
	D3DXVECTOR3 rot;						//����
	D3DXMATRIX mtxWorld;					//���[���h�}�g���b�N�X
	bool bUse;								//�g�p��
	int nNumIdx;							//�C���f�b�N�X��
	int nNumVtx;							//���_��
}MeshField;

//�v���g�^�C�v�錾
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void UpdateMeshFieldRanking(float moveX);
void SetMeshField(D3DXVECTOR3 pos);

#endif