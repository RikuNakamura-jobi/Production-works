//=====================================
//
// 
// Author:������
//
//=====================================
#include "useful.h"
#include "manager.h"
#include "debugproc.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------

D3DXCOLOR useful::HSLtoRGB(float Hue)
{
	D3DXCOLOR colRGB = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float max = 255.0f;
	float min = 0.0f;

	while (1)
	{
		if (Hue >= 360.0f)
		{
			Hue -= 360.0f;
		}
		else if (Hue < 0.0f)
		{
			Hue += 360.0f;
		}
		else
		{
			break;
		}
	}

	if (Hue >= 300.0f)
	{
		colRGB.r = max;
		colRGB.g = min;
		colRGB.b = max * ((360.0f - Hue) / 60.0f);
	}
	else if (Hue >= 240.0f)
	{
		colRGB.r = max * ((Hue - 240.0f) / 60.0f);
		colRGB.g = min;
		colRGB.b = max;
	}
	else if (Hue >= 180.0f)
	{
		colRGB.r = min;
		colRGB.g = max * ((240.0f - Hue) / 60.0f);
		colRGB.b = max;
	}
	else if (Hue >= 120.0f)
	{
		colRGB.r = min;
		colRGB.g = max;
		colRGB.b = max * ((Hue - 120.0f) / 60.0f);
	}
	else if (Hue >= 60.0f)
	{
		colRGB.r = max * ((120.0f - Hue) / 60.0f);
		colRGB.g = max;
		colRGB.b = min;
	}
	else if (Hue >= 0.0f)
	{
		colRGB.r = max;
		colRGB.g = max * (Hue / 60.0f);
		colRGB.b = min;
	}

	colRGB.r /= max;
	colRGB.g /= max;
	colRGB.b /= max;
	
	return colRGB;
}

//========================================
//3������Ԃł̍s��ɂ���]���W�ϊ��֐�
//(�C�ӂ̓_����̃I�t�Z�b�g�ʒu���p�x�Ƌ����ŕϊ�)
//========================================
D3DXVECTOR3 useful::PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset)
{
	D3DXVECTOR3 posAnswer;
	D3DXMATRIX mtxO, mtxAnswer;
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRotModel, mtxTransModel, mtxPalent;		//�v�Z�p�}�g���b�N�X

	//�p�[�c�̃��[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxO);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		posO.x, posO.y, posO.z);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxTransModel);

	mtxPalent = mtxO;

	//�p�[�c�̃��[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxAnswer);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		3.14f, 3.14f, 3.14f);
	D3DXMatrixMultiply(&mtxO, &mtxO, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel,
		offset.x, offset.y, offset.z);
	D3DXMatrixMultiply(&mtxAnswer, &mtxAnswer, &mtxTransModel);

	//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&mtxAnswer,
		&mtxAnswer,
		&mtxPalent);

	posAnswer.x = mtxAnswer._41;
	posAnswer.y = mtxAnswer._42;
	posAnswer.z = mtxAnswer._43;

	return posAnswer;
}