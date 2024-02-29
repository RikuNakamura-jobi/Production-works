//==========================================================
//
//���C�g���� [light.cpp]
//Author Ibuki Okusada
//
//==========================================================
#include "light.h"

//==========================================================
//�O���[�o���ϐ�
//==========================================================
D3DLIGHT9 g_alight[NUM_LIGHT];	//���C�g�̏��
int g_nSelectLight;		//�I�𒆃��C�g

//==========================================================
//���C�g�̏���������
//==========================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾
	D3DXVECTOR3 vecDir;		//�ݒ�ύX�p�x�N�g��

	g_nSelectLight = 0;

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_alight[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_alight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		//���C�g�̊g�U����ݒ�
		switch (nCntLight)
		{
		case 0:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			break;
		case 2:
			g_alight[nCntLight].Diffuse = D3DXCOLOR(0.45f, 0.45f, 0.45f, 1.0f);
			break;
		}

		//���C�g�̕�����ݒ�
		switch (nCntLight)
		{
		case 0:
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;
		case 1:
			vecDir = D3DXVECTOR3(0.8f, 0.23f, -0.55f);
			break;
		case 2:
			vecDir = D3DXVECTOR3(-0.8f, -0.21f, -0.44f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);	//�x�N�g���𐳋K������
		g_alight[nCntLight].Direction = vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_alight[nCntLight]);

		//���C�g��L��������
		pDevice->LightEnable(nCntLight, TRUE);
	}

}

//==========================================================
//���C�g�̏I������
//==========================================================
void UninitLight(void)
{

}

//==========================================================
//���C�g�̍X�V����
//==========================================================
void UpdateLight(void)
{



}

//==========================================================
//���C�g�̐F�ύX
//==========================================================
void ChangeColLight(void)
{


}


//==========================================================
//���C�g�̎擾
//==========================================================
D3DLIGHT9 *GetLight(void)
{
	return &g_alight[0];
}

//==========================================================
//�I�𒆃��C�g�ԍ��擾
//==========================================================
int GetSelectLight(void)
{
	return g_nSelectLight;
}