//========================================
//
//3D���f���̃G�f�B�b�g�@�\[edit.h]
//Author:Kai Takada
//
//========================================
#ifndef _EDIT_H_								//���̃}�N����`������ĂȂ�������
#define _EDIT_H_								//��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitEdit(void);
void UninitEdit(void);
void UpdateEdit(void);
void DrawEdit(void);
void SetEditIndex(int nIdx);
int GetSelectIdxEdit(void);
void LoadEdit(void);	//�G�f�B�b�g���e�̃��[�h

#endif
