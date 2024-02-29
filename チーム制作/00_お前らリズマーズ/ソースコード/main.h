//===========================
//
//�_�C���N�gX.main�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <time.h>
#include "d3dx9.h"						//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	//�r���h���̌x���p�}�N��
#include "dinput.h"						//���͏����ɕK�v
#include "xaudio2.h"

//���C�u�����̃����N
#pragma comment(lib, "d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib, "d3dx9.lib")		//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib, "dxguid.lib")		//�_�C���N�gX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "winmm.lib")		//�V�X�e�������̎擾�ɕK�v
#pragma comment(lib, "dinput8.lib")		//���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH (1280)				//�E�B���h�E�̕�
#define SCREEN_HEIGHT (720)				//�E�B���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define DEFAULT_NUMBER (0)				//�������p�̐�
#define VERTEX_COLOR (255)				//���_�J���[
#define TEXTUER_O_X (0.0f)				//���_�̃e�N�X�`����x���W
#define TEXTUER_O_Y (0.0f)				//���_�̃e�N�X�`����y���W
#define TEXTUER_TEN_X (0.2f)			//10�����̃e�N�X�`����x���W
#define TEXTUER_TEN_Y (0.5f)			//10�����̃e�N�X�`����y���W
#define FRAME_TEN (10)					//10�t���[��
#define PLAYER_WIDTH_R (980.0f)			//player�̓����镝�E
#define PLAYER_WIDTH_L (300.0f)			//player�̓����镝��
#define PLAYER_HEIGHT_U (0.0f)			//player�̓����鍂����
#define PLAYER_HEIGHT_D (720.0f)		//player�̓����鍂����
#define PLAYER_X (211.2f)				//player��x�T�C�Y
#define PLAYER_Y (352.0f)				//player��y�T�C�Y
#define PERIOD_PLAYER (3)				//player�̎���
#define SWITCH_PLAYER (5)				//player�̏㉺�؊�
#define SPEED_PLAYER (1.5f)				//player�̑��x�{��
#define MOVE_ANGLE_F (1.0f)				//�O�ړ��̊p�x�{��
#define MOVE_ANGLE_DIAF (0.75f)			//�΂ߑO�ړ��̊p�x�{��
#define MOVE_ANGLE_SIDE (0.5f)			//���ړ��̊p�x�{��
#define MOVE_ANGLE_DIAB (0.25f)			//�΂ߌ��ړ��̊p�x�{��
#define MOVE_ANGLE_B (0.0f)				//���ړ��̊p�x�{��
#define ROT_PLAYER (0.02f)				//��]�̈ړ���
#define MOVE_SIZE_PLAYER (0.5f)			//�T�C�Y�̈ړ���
#define INE_MOVE_PLAYER (0.15f)			//�v���C���[�̈ړ��̊���������
#define INE_ROT_PLAYER (0.1f)			//�v���C���[�̉�]�̊���������
#define INE_SIZE_PLAYER (0.1f)			//�v���C���[�̃T�C�Y�̊���������
#define BULLET_SPEED (8.0f)				//�e�̑��x
#define ENEMY_X (159.5f)				//player��x�T�C�Y
#define ENEMY_Y (136.4f)				//player��y�T�C�Y
#define SHOT_INTERVAL_PLAYER (8)		//�v���C���[�̒e�̔��ˊ��o

//��ʃ��[�h�̎��
typedef enum
{
	MODE_TITLE = 0,		//�^�C�g�����
	MODE_TUTORIAL,		//�`���[�g���A�����
	MODE_GAME,			//�Q�[�����
	MODE_RESULT,		//���U���g���
	MODE_RANKING,		//�����L���O���
	MODE_MAX,
}MODE;

//���_���̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;					//���_���W
	float rhw;							//���W�ϊ��p�W��
	D3DCOLOR col;						//���_�J���[
	D3DXVECTOR2 tex;					//�e�N�X�`�����W
}VERTEX_2D;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif // !_MAIN_H_