//===========================
//
//�_�C���N�gX.light�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "light.h"

//�}�N����`
#define NUM_LIGHT (3)

//�O���[�o���ϐ��錾
D3DLIGHT9 g_aLight[NUM_LIGHT];		//���C�g�̏��

//===========================
//�v���C���[�̏���������
//===========================
void InitLight(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;		//�ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//���C�g�̊g�U����ݒ�
			g_aLight[nCntLight].Diffuse = D3DXCOLOR(0.7f, 0.5f, 0.5f, 1.0f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
		}
		else if (nCntLight == 1)
		{
			//���C�g�̊g�U����ݒ�
			g_aLight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.0f, 0.0f, 0.65f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
		}
		else if (nCntLight == 2)
		{
			//���C�g�̊g�U����ݒ�
			g_aLight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 0.15f);

			//���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
		}

		D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g���𐳋K������
		g_aLight[nCntLight].Direction = vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);
	}

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitLight(void)
{
	
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdateLight(void)
{

}