//==================================
//
//�V��̏���(ceiling.h)
// Author: Kai Takada
//
//==================================
#ifndef _CEILING_H_								//���̃}�N����`������ĂȂ�������
#define _CEILING_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================
//  �}�N����`
//==========================================
#define CEILING_POSY			(500.0f)				//�V��̍���

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitCeiling(void);
void UninitCeiling(void);
void UpdateCeiling(void);
void DrawCeiling(void);
void SetPosCeiling(D3DXVECTOR3 pos, float fHeight, float fWidth);
D3DXVECTOR3 GetCeilingPos(void);

#endif
