//==================================================================================================
//
//shadow.h
//Author:���R����
//
//==================================================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_

//���
typedef enum
{
	SHADOWTYPE_CIRCLE = 0,						//�~�`
	SHADOWTYPE_SQUARE,							//��`
	SHADOWTYPE_MAX
}SHADOWTYPE;

//�\���̐錾
typedef struct
{
	D3DXVECTOR3 pos;							//�ʒu
	D3DXVECTOR3 rot;							//����
	D3DXMATRIX mtxWorld;						//���[���h�}�g���b�N�X
	SHADOWTYPE type;							//���
	float fRadius;								//���a
	float fWidth;								//x��
	float fHeight;								//z��
	bool bUse;									//�g�p���Ă��邩�ǂ���
}Shadow;

//�v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(float fRadius, SHADOWTYPE type,float fWidth, float fHeight, D3DXVECTOR3 rot);
void SetPositionShadow(int nIdxShadow,D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fScale);
Shadow *GetShadow(void);
void EnableShadow(int nIdxShadow, bool bUse);
void DebugShadow(void);

#endif
