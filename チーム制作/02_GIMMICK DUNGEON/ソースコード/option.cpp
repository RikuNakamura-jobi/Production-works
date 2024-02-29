//==========================================
//
//  �Q�[�����ݒ�(option.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "option.h"
#include "input.h"

//==========================================
//  �}�N����`
//==========================================
#define MOUSE_SENS_DEFAULT (D3DXVECTOR3(0.001f, 0.001f, 0.0f)) //�}�E�X���x�̏����ݒ�
#define MOUSE_SENS_SCALE (0.0001f) //���x�ݒ�̊�l

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 temp; //�ۑ��p�ϐ�
	D3DXVECTOR3 pass; //�ϐ��擾�p
	bool bReverse_X; //X���̔��]
	bool bReverse_Y; //Y���̔��]
}MOUSE_SENS;

//==========================================
//  �O���[�o���ϐ�
//==========================================
MOUSE_SENS g_MouseSens;

//==========================================
//  ����������
//==========================================
void InitOption()
{
	//�ϐ��̏�����
	g_MouseSens.temp = MOUSE_SENS_DEFAULT;
	g_MouseSens.pass = g_MouseSens.temp;
	g_MouseSens.bReverse_X = false;
	g_MouseSens.bReverse_Y = false;
}

//==========================================
//  �I������
//==========================================
void UninitOption()
{

}

//==========================================
//  �X�V����
//==========================================
void UpdateOption()
{
	//�}�E�X���x�̕␳�l��ݒ肷��
	SetMouseSensCorrection();
}

//==========================================
//  �`�揈��
//==========================================
void DrawOption()
{

}

//==========================================
//  �}�E�X�̊��x�ݒ�
//==========================================
void SetMouseSensCorrection()
{
	//���[�J���ϐ��錾
	float fWheel = GetMouseMove().z;

	//���x�̏㉺
	if (fWheel > 0.0f)
	{
		g_MouseSens.temp += D3DXVECTOR3(MOUSE_SENS_SCALE, MOUSE_SENS_SCALE, 0.0f);
	}
	if (fWheel < 0.0f)
	{
		g_MouseSens.temp -= D3DXVECTOR3(MOUSE_SENS_SCALE, MOUSE_SENS_SCALE, 0.0f);
	}

	//�Œ�l�̕␳
	if (g_MouseSens.temp.x < MOUSE_SENS_SCALE)
	{
		g_MouseSens.temp.x = MOUSE_SENS_SCALE;
	}
	if (g_MouseSens.temp.y < MOUSE_SENS_SCALE)
	{
		g_MouseSens.temp.y = MOUSE_SENS_SCALE;
	}

	//�ݒ肳�ꂽ���x�̎擾
	g_MouseSens.pass = g_MouseSens.temp;

	//���]�؂�ւ�
	if (GetMouseTrigger(MOUSEBUTTON_LEFT))
	{
		g_MouseSens.bReverse_X = !g_MouseSens.bReverse_X;
	}
	if (GetMouseTrigger(MOUSEBUTTON_RIGHT))
	{
		g_MouseSens.bReverse_Y = !g_MouseSens.bReverse_Y;
	}

	//���͂̔��]
	if (g_MouseSens.bReverse_X)
	{
		g_MouseSens.pass.x *= -1.0f;
	}
	if (g_MouseSens.bReverse_Y)
	{
		g_MouseSens.pass.y *= -1.0f;
	}
}

//==========================================
//  �␳���x�̎擾
//==========================================
D3DXVECTOR3 GetMouseSensCorrection()
{
	return g_MouseSens.pass;
}
