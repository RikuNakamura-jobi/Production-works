//==================================================================================================
//
//�ėp�����蔻�菈��
//Author:������
//
//==================================================================================================
#include "main.h"
#include "collision.h"
#include "calculation.h"
#include "effect.h"

//==================================================================================================
//�~�̓����蔻��(x.z����)
//==================================================================================================
bool CollisionCircle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//�ϐ��錾
	bool bHit = false;

	if (hypotf(posA.x - posB.x, posA.z - posB.z) < fabsf(length))
	{//�~�̐ڐG����
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//�~�̓����蔻��(2D)
//==================================================================================================
bool CollisionCircle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//�ϐ��錾
	bool bHit = false;

	if (hypotf(posA.x - posB.x, posA.y - posB.y) < fabsf(length))
	{//�~�̐ڐG����
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//���̓����蔻��
//==================================================================================================
bool CollisionSphere(D3DXVECTOR3 posA, D3DXVECTOR3 posB, float length)
{
	//�ϐ��錾
	bool bHit = false;
	D3DXVECTOR3 posDiff;

	//�ʒu�̍����v�Z
	posDiff = posA - posB;

	if (fabsf(D3DXVec3Length(&posDiff)) < fabsf(length))
	{//���̐ڐG����
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//�ȉ~�̓����蔻��(x.z����)
//==================================================================================================
bool CollisionEllipse(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//�ϐ��錾
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;
	float fAxisTemp;

	if (axisL < axisS)
	{
		fAxisTemp = axisL;
		axisL = axisS;
		axisS = fAxisTemp;
	}

	//�œ_�̌v�Z
	posFocusA.x = posA.x + (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.z = posA.x + (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x + (cosf(rot.y + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.z = posA.x + (sinf(rot.y + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	if (axisL * 2 > hypotf(posFocusA.x - posB.x, posFocusA.z - posB.z) + hypotf(posFocusB.x - posB.x, posFocusB.z - posB.z))
	{//�ȉ~�̐ڐG����
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//�ȉ~�̓����蔻��(2D)
//==================================================================================================
bool CollisionEllipse2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//�ϐ��錾
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;
	float fAxisTemp;

	if (axisL < axisS)
	{
		fAxisTemp = axisL;
		axisL = axisS;
		axisS = fAxisTemp;
	}

	//�œ_�̌v�Z
	posFocusA.x = posA.x + (cosf(rot.z) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.y = posA.y + (sinf(rot.z) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x + (cosf(rot.z + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.y = posA.y + (sinf(rot.z + D3DX_PI) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	
	//��̏œ_�Ƃ̋����̘a�����̒l�ȉ����Ƃ�Ƃ����肷��
	if (axisL * 2 > hypotf(posFocusA.x - posB.x, posFocusA.y - posB.y) + hypotf(posFocusB.x - posB.x, posFocusB.y - posB.y))
	{//�ȉ~�̐ڐG����
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//�ȉ~���̓����蔻��(������)
//==================================================================================================
bool CollisionOval(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	//�ϐ��錾
	bool bHit = false;
	D3DXVECTOR3 posFocusA, posFocusB;

	//�œ_�̌v�Z
	posFocusA.x = posA.x + (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusA.z = posA.x + (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	posFocusB.x = posA.x - (cosf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));
	posFocusB.z = posA.x - (sinf(rot.y) * sqrtf((float)pow(axisL, 2) - (float)pow(axisS, 2)));

	if (hypotf(posA.x - posB.x, posA.z - posB.z) < hypotf(posFocusA.x - posB.x, posFocusA.z - posB.z) + hypotf(posFocusB.x - posB.x, posFocusB.z - posB.z))
	{//���̐ڐG����
		bHit = true;
	}

	return bHit;
}

//==================================================================================================
//��`�̓����蔻��
//==================================================================================================
bool CollisionRectangle(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width)
{
	bool bRec;
	bRec = false;

	return bRec;
}

//==================================================================================================
//��`�̓����蔻��(2D)
//==================================================================================================
bool CollisionRectangle2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float height, float width)
{
	bool bRec;
	bRec = false;

	D3DXVECTOR3 point[4];
	float fAngle, fLength;
	int nRec = 0;

	//�Ίp���̒������Z�o����
	fLength = hypotf(width, height) * 0.5f;

	//�Ίp���̊p�x���Z�o����
	fAngle = atan2f(width, height);

	//���_���W�̐ݒ�
	point[0].x = posA.x + (sinf(rot.z - D3DX_PI + fAngle) * fLength);
	point[0].y = posA.y + (cosf(rot.z - D3DX_PI + fAngle) * fLength);
	point[0].z = 0.0f;

	point[1].x = posA.x + (sinf(rot.z - fAngle) * fLength);
	point[1].y = posA.y + (cosf(rot.z - fAngle) * fLength);
	point[1].z = 0.0f;

	point[2].x = posA.x + (sinf(rot.z + fAngle) * fLength);
	point[2].y = posA.y + (cosf(rot.z + fAngle) * fLength);
	point[2].z = 0.0f;

	point[3].x = posA.x + (sinf(rot.z + D3DX_PI - fAngle) * fLength);
	point[3].y = posA.y + (cosf(rot.z + D3DX_PI - fAngle) * fLength);
	point[3].z = 0.0f;

	for (int nCntRectangle = 0; nCntRectangle < 4; nCntRectangle++)
	{
		if (nCntRectangle == 3)
		{
			if (OuterProduct2D(point[nCntRectangle], posB, point[0]) > 0.0f)
			{
				nRec++;
			}
		}
		else
		{
			if (OuterProduct2D(point[nCntRectangle], posB, point[nCntRectangle + 1]) > 0.0f)
			{
				nRec++;
			}
		}
	}

	if (nRec == 2)
	{
		bRec = true;
	}

	return bRec;
}

//==================================================================================================
//���̓����蔻��(������)
//==================================================================================================
bool CollisionSquare(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 rot, float axisL, float axisS)
{
	bool bSquare;
	bSquare = false;

	return bSquare;
}