//===========================
//
//�_�C���N�gX.shadow�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _SHADOW_H_
#define _SHADOW_H_

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 posShadow;
	D3DXVECTOR3 rotShadow;
	D3DXMATRIX mtxWorldShadow;		//���[���h�}�g���b�N�X
	bool bTrue;
}Shadow;

//�v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(float rad);
void SetShadowMat(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void SetShadowFalse(int nIdxShadow);

#endif // !_SHADOW_H_