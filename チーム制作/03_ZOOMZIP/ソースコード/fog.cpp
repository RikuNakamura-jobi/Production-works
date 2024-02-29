//==========================================================
//
//�t�H�O���� [fog.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "fog.h"
#include "result.h"

//==========================================================
//�O���[�o���ϐ�
//==========================================================
Fog g_Fog;		//�t�H�O�̏��

//==========================================================
//�t�H�O�̏���������
//==========================================================
void InitFog(void)
{
	//��񏉊���
	switch (GetMode())
	{
	case MODE_TUTORIAL:
		g_Fog.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f);	//�F
		g_Fog.fStart = 1000.0f;	//�J�n�_
		g_Fog.fEnd = 1700.0f;	//�I���_
		g_Fog.fDensity = 0.01f;	//�t�H�O���x
		break;
	case MODE_GAME:
		g_Fog.col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.1f);	//�F
		g_Fog.fStart = 2000.0f;	//�J�n�_
		g_Fog.fEnd = 2700.0f;	//�I���_
		g_Fog.fDensity = 0.01f;	//�t�H�O���x
		break;
	case MODE_RESULT:
		//if (*GetResultState() == RESULTSTATE_CLEAR)
		//{//�N���A���
		//	g_Fog.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		//	g_Fog.fStart = 1000.0f;	//�J�n�_
		//	g_Fog.fEnd = 3000.0f;	//�I���_
		//	g_Fog.fDensity = 0.01f;	//�t�H�O���x
		//}
		//else if(*GetResultState() == RESULTSTATE_GAMEOVER)
		//{//�Q�[���I�[�o�[���
			g_Fog.col = D3DXCOLOR(0.05f, 0.05f, 0.05f, 0.5f);	//�F
			g_Fog.fStart = 200.0f;	//�J�n�_
			g_Fog.fEnd = 1000.0f;	//�I���_
			g_Fog.fDensity = 0.01f;	//�t�H�O���x
		//}
		break;
	}
}

//==========================================================
//�t�H�O�̏I������
//==========================================================
void UninitFog(void)
{

}

//==========================================================
//�t�H�O�̍X�V����
//==========================================================
void UpdateFog(void)
{

}

//==========================================================
//�t�H�O�̕`�揈��
//==========================================================
void DrawFog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾

	Fog fog = g_Fog;	//�t�H�O�̏��i�[�p

	//�t�H�O��L���ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//�t�H�O�J���[�ݒ�
	pDevice->SetRenderState(D3DRS_FOGCOLOR, fog.col);

	//�o�[�e�b�N�X�t�H�O(���`����)���g�p
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);

	//�t�H�O�͈͐ݒ�
	pDevice->SetRenderState(D3DRS_FOGSTART, *((LPDWORD)(&fog.fStart)));
	pDevice->SetRenderState(D3DRS_FOGEND, *((LPDWORD)(&fog.fEnd)));

	//�s�N�Z���t�H�O(�w����2��)���g�p
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);

	//�t�H�O���x�ݒ�
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *((LPDWORD)(&fog.fDensity)));

	//�͈̓x�[�X�̃t�H�O���g�p
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

}

//==========================================================
//�t�H�O�̏�ԃ��Z�b�g
//==========================================================
void ResetStateFog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾

	//�t�H�O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);

	//�͈̓x�[�X�̃t�H�O��s�g�p
	pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
}