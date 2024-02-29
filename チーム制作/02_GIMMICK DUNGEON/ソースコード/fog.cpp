//==========================================
//
//  �����̐���(fog.cpp)
//  Autnor : Tomoya Kanazaki
//
//==========================================
#include "fog.h"
#include "player.h"
#include "input.h"

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
FOGDATA g_fog;

//==========================================
//  ����������
//==========================================
void InitFog()
{
	//�ϐ��̏�����
	ZeroMemory(&g_fog, sizeof(FOGDATA));
	g_fog.bUse = false;
	g_fog.col = FOG_COLOR_SAND;
	g_fog.fStart = FOG_START_TITLE;
	g_fog.fEnd = FOG_END_TITLE;
}

//==========================================
//  �I������
//==========================================
void UninitFog()
{

}

//==========================================
//  �X�V����
//==========================================
void UpdateFog()
{
	//�g�p��Ԃ̍X�V
	if (GetEnvironment() == PLAYERENVIRONMENT_CLOUD)
	{
		g_fog.bUse = true;
	}
	else
	{
		//g_fog.bUse = false;
	}

#ifdef _DEBUG
	if (GetKeyboardPress(DIK_3))
	{
		g_fog.fStart += 0.5f;
	}
	if (GetKeyboardPress(DIK_4))
	{
		g_fog.fStart -= 0.5f;
	}
	if (GetKeyboardPress(DIK_5))
	{
		g_fog.fEnd += 0.5f;
	}
	if (GetKeyboardPress(DIK_6))
	{
		g_fog.fEnd -= 0.5f;
	}
#endif
}

//==========================================
//  �ݒ菈��
//==========================================
void SetFog()
{
	if (g_fog.bUse)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//����L����
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

		//���̐F��ݒ�
		pDevice->SetRenderState(D3DRS_FOGCOLOR, g_fog.col);

		//���̏�Ԃ�ݒ�
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

		//���̔����͈͂�ݒ�
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&g_fog.fStart));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&g_fog.fEnd));
	}
}

//==========================================
//  �I���I�t�؂�ւ����� takayama
//==========================================
void EnableFog(bool bUse)
{
	g_fog.bUse = bUse;
}

//==========================================
//  �|�C���^�擾���� takayama
//==========================================
FOGDATA *GetFog(void)
{
	return &g_fog;
}