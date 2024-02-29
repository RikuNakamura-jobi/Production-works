//==================================================================================================
//
//�l�p�v�Z����
//Author:������
//
//==================================================================================================
#include "main.h"
#include "calculation.h"

//========================================
//�O�ύ��E����p�֐�(�_A����L�т�2�̃x�N�g��(AB��AC)�̊O��)
//�_B������AC�̍��ɂ���Ƃ��̓v���X�A�_B������AC�̉E�ɂ���Ƃ��̓}�C�i�X�̒l��Ԃ�������ɂ���ꍇ0��Ԃ�
//========================================
float OuterProduct(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	D3DXVECTOR3 vecLine1, vecLine2;
	float vecAnswer;

	//�_AB�̃x�N�g��
	vecLine1.x = posB.x - posA.x;
	vecLine1.z = posB.z - posA.z;

	//�_AC�̃x�N�g��
	vecLine2.x = posC.x - posA.x;
	vecLine2.z = posC.z - posA.z;

	//�v�Z�����x�N�g��
	vecAnswer = (vecLine1.z * vecLine2.x) - (vecLine1.x * vecLine2.z);

	return vecAnswer;
}

//========================================
//�O�ύ��E����p�֐�(�_A����L�т�2�̃x�N�g��(AB��AC)�̊O��)
//�_B������AC�̍��ɂ���Ƃ��̓v���X�A�_B������AC�̉E�ɂ���Ƃ��̓}�C�i�X�̒l��Ԃ�������ɂ���ꍇ0��Ԃ�
//========================================
float OuterProduct2D(D3DXVECTOR3 posA, D3DXVECTOR3 posB, D3DXVECTOR3 posC)
{
	D3DXVECTOR3 vecLine1, vecLine2;
	float vecAnswer;

	//�_AB�̃x�N�g��
	vecLine1.x = posB.x - posA.x;
	vecLine1.y = posB.y - posA.y;

	//�_AC�̃x�N�g��
	vecLine2.x = posC.x - posA.x;
	vecLine2.y = posC.y - posA.y;

	//�v�Z�����x�N�g��
	vecAnswer = (vecLine1.y * vecLine2.x) - (vecLine1.x * vecLine2.y);

	return vecAnswer;
}

//========================================
//3������Ԃł̍s��ɂ���]���W�ϊ��֐�
//(�C�ӂ̓_����̃I�t�Z�b�g�ʒu���p�x�Ƌ����ŕϊ�)
//========================================
D3DXVECTOR3 PosRelative(D3DXVECTOR3 posO, D3DXVECTOR3 rot, float length)
{
	D3DXVECTOR3 posAnswer;

	posAnswer.x = posO.x + (cosf(rot.y) * cosf(rot.z) * length);
	posAnswer.y = posO.y + (-sinf(rot.x) * sinf(rot.z) * length);
	posAnswer.z = posO.z + (-sinf(rot.y) * cosf(rot.x) * length);

	return posAnswer;
}

//==================================================
//�_�ƒ����̋��������߂�֐�
//==================================================
float LengthPToL(D3DXVECTOR3 point, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2)
{
	D3DXVECTOR3 lineCoe;		//�����̌W��
	float den, num;				//����ƕ��q
	float answer;				//����

	lineCoe.x = (linePoint2.z - linePoint1.z) / (linePoint2.x - linePoint1.x);
	lineCoe.y = -1.0f;
	lineCoe.z = (lineCoe.x * linePoint1.x) - linePoint1.z;

	den = hypotf(lineCoe.x, lineCoe.y);
	num = fabsf((lineCoe.x * point.x) + (lineCoe.y * point.z) + lineCoe.z);

	answer = num / den;

	return answer;
}

//==================================================
//3������ԏ��2�_���Ȃ�������rot�����߂�֐�(�\��n)
//A����B��B����A�Ō��ʂ����]���܂�
//==================================================