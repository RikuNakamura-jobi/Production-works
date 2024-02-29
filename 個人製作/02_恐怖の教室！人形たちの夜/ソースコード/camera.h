//===========================
//
//�_�C���N�gX.camera�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posV;			//���_
	D3DXVECTOR3 posR;			//�����_
	D3DXVECTOR3 posVDest;		//�ړI�̎��_
	D3DXVECTOR3 posRDest;		//�ړI�̒����_
	D3DXVECTOR3 posVOld;		//�ړI�̎��_
	D3DXVECTOR3 posROld;		//�ړI�̒����_
	D3DXVECTOR3 vecU;			//������x�N�g��
	D3DXVECTOR3 rot;			//�p�x
	D3DXVECTOR3 rotOld;			//�p�x
	D3DXMATRIX mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			//�r���[�}�g���b�N�X
}Camera;

//�v���g�^�C�v�錾
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void UpdateCameraTitle(void);
void SetCamera(void);
Camera GetCamera(void);
void RotCamera(void);
void RotCameraTitle(void);

#endif // !_CAMERA_H_