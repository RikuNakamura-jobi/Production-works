//==================================================================================================
//
//collision.h
//Author:������
//
//==================================================================================================

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//================================
// �v���g�^�C�v�錾
//================================
bool CollisionCircle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length);											//�~�̓����蔻��
bool CollisionCircle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length);										//�~(2D)�̓����蔻��
bool CollisionSphere(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length);											//���̓����蔻��
bool CollisionEllipse(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);			//�ȉ~�̓����蔻��
bool CollisionEllipse2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);			//�ȉ~(2D)�̓����蔻��
bool CollisionOval(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);				//�ȉ~���̓����蔻��
bool CollisionRectangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width);			//��`�̓����蔻��
bool CollisionRectangle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width);		//��`(2D)�̓����蔻��
bool CollisionSquare(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS);			//���̓����蔻��

#endif
