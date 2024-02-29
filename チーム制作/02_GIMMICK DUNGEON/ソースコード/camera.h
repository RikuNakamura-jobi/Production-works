//========================================
//
//2D�V���[�e�B���O�Q�[��(�J�����擾����)[getitem.h]
//Author:���v�ԗD��
//
//========================================
#ifndef _CAMERA_H_							//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_							//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//==========================================
//  �\���̒�`
//==========================================
//�J�����\����
typedef struct
{
	D3DXVECTOR3 posV;						//���_
	D3DXVECTOR3 posR;						//�����_
	D3DXVECTOR3 vecU;						//������x�N�g��
	D3DXMATRIX mtxProjection;				//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;						//�r���[�}�g���b�N�X
	D3DXVECTOR3 rot;						//����
	D3DXVECTOR3 posVDest;					//�ړI�̎��_
	D3DXVECTOR3 posRDest;					//�ړI�̒����_
	int nQuakeTime;							//�h��̎���
	float fQuakeSize;						//�h��̑傫��
	float posRHeight;						//�����_�̍���
	bool bBehind;							//���ɓ�������
}Camera;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);
void Move(void);			//�ړI�̒����_��ݒ�(�ړ�)
void RotMove(void);
void Rot(void);				//�ړI�̒����_��ݒ�(��]) 
void RotPlayer(void);		//���_���v���C���[�̌��ɓ�����
void SetQuake(int time, float size);	//�J�������h���
void DrawDebugCamera(LPD3DXFONT pFont);
void GameEndCamera(int nResult);

void ControlCamera(void);	//�J�������쏈��
#endif
