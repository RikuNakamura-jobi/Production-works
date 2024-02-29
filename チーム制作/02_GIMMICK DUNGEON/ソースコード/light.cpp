//========================================
//
//2D�A�N�V�����Q�[��(���C�g����)[item.cpp]
//Author:���v�ԗD��
//
//========================================
#include "main.h"
#include "light.h"
#include "player.h"
#include "input.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_LIGHT (4) //���C�g�̎g�p��
#define LIGHT_BRIGHT	(0.9f)	//���C�g�̖��邳

//�O���[�o���ϐ��錾
D3DLIGHT9 g_light[MAX_LIGHT];				//���C�g�̏��

//========================================
//���C�g�̏���������
//========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXVECTOR3 vecDir;									//�ݒ�p�����x�N�g��
	int nCntLight;

	for (nCntLight = 0; nCntLight < MAX_LIGHT;nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		//���C�g�̎�ނ�ݒ�
		g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//���C�g�̎�ސݒ�
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g�̊g�U���̐ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(LIGHT_BRIGHT, LIGHT_BRIGHT, LIGHT_BRIGHT, 1.0f);

			//���C�g�̕����ݒ�
			vecDir = D3DXVECTOR3(-1.4f, 0.24f, -2.21f);
			D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
			g_light[nCntLight].Direction = vecDir;

			//���C�g�ݒ�
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//���C�g�L����
			pDevice->LightEnable(nCntLight, TRUE);
			
		}
		else if (nCntLight == 1)
		{
			//���C�g�̎�ސݒ�
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g�̊g�U���̐ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(LIGHT_BRIGHT, LIGHT_BRIGHT, LIGHT_BRIGHT, 1.0f);

			//���C�g�̕����ݒ�
			vecDir = D3DXVECTOR3(1.42f, -0.8f, 0.08f);
			D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
			g_light[nCntLight].Direction = vecDir;

			//���C�g�ݒ�
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//���C�g�L����
			pDevice->LightEnable(nCntLight, TRUE);
			
		}
		else if (nCntLight == 2)
		{
			//���C�g�̎�ސݒ�
			g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			//���C�g�̊g�U���̐ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(LIGHT_BRIGHT, LIGHT_BRIGHT, LIGHT_BRIGHT, 1.0f);

			//���C�g�̕����ݒ�
			vecDir = D3DXVECTOR3(-0.59f, -0.8f, 1.55f);
			D3DXVec3Normalize(&vecDir, &vecDir);		//�x�N�g�����K��
			g_light[nCntLight].Direction = vecDir;

			//���C�g�ݒ�
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//���C�g�L����
			pDevice->LightEnable(nCntLight, TRUE);
			
		}
		else if (nCntLight == 3)
		{
			//���C�g���|�C���g���C�g�ɐݒ�
			g_light[nCntLight].Type = D3DLIGHT_POINT;

			//���C�g�̊g�U���̐ݒ�
			g_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			//���C�g���v���C���[�ɒǏ]������
			g_light[nCntLight].Position = GetPlayer()->pos;
			g_light[nCntLight].Position.y = 0.0f;

			//���C�g�̃T�C�Y��ݒ肷��
			g_light[nCntLight].Range = 500.0f;

			//�t�H�[���I�t�ݒ�
			g_light[nCntLight].Falloff = 1.0f;

			//���C�g�ݒ�
			pDevice->SetLight(nCntLight, &g_light[nCntLight]);

			//���C�g�L����
			//pDevice->LightEnable(nCntLight, TRUE);
		}
	}

}

//========================================
//���C�g�̏I������
//========================================
void UninitLight(void)
{

}

//========================================
//���C�g�̍X�V����
//========================================
void UpdateLight(void)
{
	//�|�C���g���C�g���v���C���[�ɒǏ]������
	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		if (g_light[nCnt].Type == D3DLIGHT_POINT)
		{
			LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

			g_light[nCnt].Position.x = GetPlayer()->pos.x;
			g_light[nCnt].Position.z = GetPlayer()->pos.z;

			if (GetKeyboardPress(DIK_UP))
			{
				g_light[nCnt].Range++;
			}
			if (GetKeyboardPress(DIK_DOWN))
			{
				g_light[nCnt].Range--;
			}
			if (GetKeyboardPress(DIK_LEFT))
			{
				g_light[nCnt].Position.y++;
			}
			if (GetKeyboardPress(DIK_RIGHT))
			{
				g_light[nCnt].Position.y--;
			}

			//���C�g�ݒ�
			pDevice->SetLight(nCnt, &g_light[nCnt]);

			//���C�g�̎g�p/�s�g�p
			//if (GetKeyboardTrigger(DIK_RETURN))
			//{
			//	pDevice->LightEnable(nCnt, TRUE);
			//}
			//if (GetKeyboardTrigger(DIK_RSHIFT))
			//{
			//	pDevice->LightEnable(nCnt, FALSE);
			//}
		}
	}
}

//========================================
//�v���C���[�̃��[�V����
//========================================
void DebugLIGHT(void)
{
	LPD3DXFONT pFont = GetFont();
	RECT rect4 = { 0,100,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "light.Range : %.2f\nlight.pos : ( %.2f : %.2f : %.2f )", g_light[3].Range, g_light[3].Position.x, g_light[3].Position.y, g_light[3].Position.z);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect4, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}
