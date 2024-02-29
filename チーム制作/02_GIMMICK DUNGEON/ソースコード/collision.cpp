//==================================================================================================
//
//collision.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"

//==================================================================================================
//�@���̓����蔻��
//==================================================================================================
bool SphereCollision(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget, float width, float TargetWidth)
{
	//�ϐ��錾
	bool bHit = false;
	D3DXVECTOR3 posDiff;
	float fLengthBet;	//�����̒���
	float fLengthWidth = width + TargetWidth;	//�����̒���

	//�ڕW�̒l�ƌ��݂̒l�̍������Z�o
	posDiff = posTarget - pos;

	//�����𒼐������ɕϊ�
	fLengthBet = D3DXVec3Length(&posDiff);

	//�v���X�E�}�C�i�X�̕␳
	fLengthBet = fLengthBet * fLengthBet;
	fLengthWidth = fLengthWidth * fLengthWidth;

	if (fLengthBet <= fLengthWidth)
	{// �����ڐG���Ă��邩�̔���
		bHit = true;
	}

	return bHit;
}