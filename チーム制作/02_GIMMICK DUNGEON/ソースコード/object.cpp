//========================================
//
//3D�Q�[��(�I�u�W�F�N�g����)[object.cpp]
//Author:���v�ԗD��,������(�ꕔ)
//
//========================================

//=============================
// �C���N���[�h
//=============================
#include "main.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "player.h"
#include "edit.h"
#include "rock.h"
#include "billboard.h"
#include "ceiling.h"
#include "enemy.h"
#include "stdio.h"
#include "score.h"
#include "shadow.h"
#include "particle.h"
#include "tutorial.h"
#include "effect.h"
#include "sound.h"
#include "collision.h"
#include "game.h"
#include "point.h"
#include "count.h"
#include "model.h"

//=============================
// �}�N����`
//=============================
// �I�u�W�F�N�g�S��
#define NUM_BLOCK	(10)	//�e�N�X�`���̐�
#define ROT_SPEED (0.04f)	//��]���鑬�x
#define ROCK_LIMIT		(5)		//�X�g�b�p�[��������܂ł̉�ꂽ��̐�
#define OBJ_GLAVITY		(0.48f)	//�I�u�W�F�N�g�ɂ�����d��
#define SET_SHADOW	(-1)	//�e�̐ݒ�
#define SHADOW_SCALE_CCL		(1.4f)	//�e�ɓn���{��(��)
#define SHADOW_SCALE_SQR		(1.8f)	//�e�ɓn���{��(��`)
#define COLLISION_RATE		(1.1f)	//�����蔻��̔{��(�u�x��)
#define SHIFT_SOUND_TIME	(5)	//���炷�����Ȃ�p�x
#define DAMAGE_SCALE	(0.3f)	//�_���[�W���������锻��T�C�Y

//���˔�
#define REF_LENGTH			(29.0f)	//���t���N�^�[�ƃv���C���[�̋���

//���ˑ�
#define INJECT_TIME	(300)	//�₪���˂����Ԋu

//���o�[
#define MOVE_WINDLEVER (0.2f)	//���̈ړ���

//�{�^��
#define BUTTON_BILLBOARD (80.0f)	//�r���{�[�h��+y���W
#define BUTTON_PUSH_DEPTH	(50.0f)	//�{�^���̒��ݍ��ޗ�

// ���̒�
#define POLE_SCALE_SPEED	(0.001f)	//���̒��̐������x
#define SANDPOLE_DAMAGE		(150)	//�Β����^����_���[�W
#define ROTATE_SPEED		(0.05f)	//���̒����|���X�s�[�h
#define COLLISION_HEIGHT	(250.0f)	//�����̃I�t�Z�b�g�̍���
#define SAND_POLE_SCORE		(2200)		//���̒��̃X�R�A
#define PARTICLE_TIME		(2)	//�p�[�e�B�N�����o���p�x
#define SANDPOLE_FRAG		(20)	//�j�Ђ̐�

// �Β�
#define STONE_SCALE_SPEED	(0.0007f)	//�Β��̐������x
#define COLLISION_BELOW		(100.0f)	//�Β��̃I�t�Z�b�g�̒Ⴓ
#define STONEPOLE_DAMAGE			(150)	//�Β����^����_���[�W
#define FALL_SPEED			(2.0f)	//�Β��̗������x
#define POLE_SCORE			(2500)	//�Β��̃X�R�A
#define STONE_POLE_RADIUS	(100.0f)	//�Β��̓����蔻��̔��a
#define STONEPOLE_FRAG		(10)	//�j�Ђ̐�

// ��̌���
#define ROCKFRAG_SCALE_SPEED	(0.006f)	//�j�Ђ̃T�C�Y�������x
#define ROCKFRAG_SPEED	(2.0f)	//�j�Ђ̃����_������x
#define ROCKFRAG_GLAVITY		(0.1f)	//�I�u�W�F�N�g�ɂ�����d��
#define FIELD_REFLECT		(0.6f)	//�n�ʂł͂˂��ۂ�move�ւ̔��˗�

//�����L���O�Ŏg�p
#define SETOBJ_ROOP		(50)	//�����L���O�ł̃Z�b�g�t���[����
#define SETOBJ_PERCENT		(100)	//�����L���O�ł̊m��
#define RANKING_SETOBJ		(D3DXVECTOR3( 1000.0f, 0.0f, 200.0f ))	//�����L���O���̃Z�b�g�ʒu

#define SKULL_PER (15.0f)	//���W���̊m��
#define SKULLMT_PER (15.0f + SKULL_PER)	//���W���̎R�̊m��
#define STATUE_1_PER (5.0f + SKULLMT_PER)	//����(���)�̊m��
#define STATUE_3_PER (5.0f + STATUE_1_PER)	//����(���l�T���X)�̊m��
#define STATUE_4_PER (5.0f + STATUE_3_PER)	//����(��)�̊m��
#define STATUE_5_PER (5.0f + STATUE_4_PER)	//����()�̊m��
#define STATUE_6_PER (5.0f + STATUE_5_PER)	//����()�̊m��
#define STATUE_7_PER (5.0f + STATUE_6_PER)	//����()�̊m��
#define STATUE_8_PER (5.0f + STATUE_7_PER)	//����()�̊m��
#define TREASUREBOX_PER (15.0f + STATUE_8_PER)	//�󔠂̊m��
#define STATUE_100_PER (2.0f + TREASUREBOX_PER)	//����(����L)�̊m��
#define STATUE_101_PER (2.0f + STATUE_100_PER)	//����(�W���b�N�I�[)�̊m��
//#define SWORD_PER (15.0f + STATUE_101_PER)	//���̊m��

//=============================
// �񋓌^�錾
//=============================
//�{�^���̏��
typedef enum
{
	BUTONSTATE_NONE = 0,	//�������Ă��Ȃ����
	BUTONSTATE_FORWARD,		//�O�i���
	BUTONSTATE_WAIT,	//�ҋ@���
	BUTONSTATE_MAX
}BUTTONSTATE;

//=============================
// �v���g�^�C�v�錾
//=============================
void StateChangeObject(void);						//��Ԃ̕ύX
void SetRockObject(int nCntObject);			//�₪���ɏo�鏈��
void BillboardUse(Object *pObject);
void RotNormalize(D3DXVECTOR3 *pRot);
void ButtonMove(int nCntObject);
void ButtonPoleMove(int nCntObject);
void CollisionRockObject(int nCntObject);
void UpdatePosOffset(int nCntObject);
void UpdateRockFrag(int nCntObject);

//=============================
// �O���[�o���ϐ��錾
//=============================
Object g_Object[MAX_OBJECT];
D3DXMATERIAL g_typeMat[COLTYPE_MAX] = {};				//�g�p�F�̊Ǘ�
BUTTONSTATE g_buttonState;								//�{�^���̏��
BUTTONSTATE g_buttonStatePole;							//���΃{�^���̏��
bool g_bOpenRock;			//����g�p���Ă��邩�ǂ���
bool g_bPoleAttack;			//���΂��g�p���Ă��邩�ǂ���
int g_nRoopSetTime;			//�����L���O���̃I�u�W�F�N�g�����̃��[�v����
bool g_bButtonSound;

//========================================
//�I�u�W�F�N�g�̏���������
//========================================
void InitObject(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			
	
	//�ϐ��錾
	int nCntObject;

	g_bOpenRock = true;			//����g�p���Ă��Ȃ�
	g_bPoleAttack = true;			//����g�p���Ă��Ȃ�
	g_buttonState = BUTONSTATE_NONE;
	g_buttonStatePole = BUTONSTATE_FORWARD;
	g_nRoopSetTime = 0;

	//�e��ϐ��̏�����
	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		ZeroMemory(&g_Object[nCntObject],sizeof(Object));
		g_Object[nCntObject].nIdxshadow = SET_SHADOW;
	}

	//�F�̗p��(������)
	{
		g_typeMat[COLTYPE_NORMAL].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_NORMAL].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_NORMAL].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_typeMat[COLTYPE_DAMAGE].MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_DAMAGE].MatD3D.Ambient = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_DAMAGE].MatD3D.Emissive = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);

		g_typeMat[COLTYPE_DEATH].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_DEATH].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_typeMat[COLTYPE_DEATH].MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_typeMat[COLTYPE_EDIT].MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_EDIT].MatD3D.Ambient = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);
		g_typeMat[COLTYPE_EDIT].MatD3D.Emissive = D3DXCOLOR(0.8f, 0.3f, 0.3f, 0.8f);

		g_typeMat[COLTYPE_DELETETARGET].MatD3D.Diffuse = D3DXCOLOR(0.9f, 0.0f, 0.0f, 0.8f);
		g_typeMat[COLTYPE_DELETETARGET].MatD3D.Ambient = D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f);
		g_typeMat[COLTYPE_DELETETARGET].MatD3D.Emissive = D3DXCOLOR(0.9f, 0.0f, 0.0f, 1.0f);
	}
}

//========================================
// �I�u�W�F�N�g�̏I������
//========================================
void UninitObject(void)
{
}

//========================================
// �I�u�W�F�N�g�̍X�V����
//========================================
void UpdateObject(void)
{
	//���擾
	int *pHitRockCouter = GetHitRockNum();
	Enemy *pEnemy = GetEnemy();
	
	//�����擾
	D3DXVECTOR3 posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDiffLength = 0.0f;

	if (*pHitRockCouter >= ROCK_LIMIT)
	{//��萔�₪��ꂽ�^�C�~���O�ŃX�g�b�p�[��������

		g_buttonState = BUTONSTATE_FORWARD;

		g_bOpenRock = false;

		*pHitRockCouter = 0;
	}

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{//�g�p����Ă����ԂȂ�
			//�O��̈ʒu��ۑ�
			g_Object[nCntObject].posOld = g_Object[nCntObject].pos;

			posDiff = g_Object->pos - GetPlayer()->pos;
			fDiffLength = D3DXVec3Length(&posDiff);

			if (g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR &&
				g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR_BASE &&
				g_Object[nCntObject].nType != OBJECTTYPE_SANDPOLE_BASE)
			{//�����蔻����s��Ȃ��I�u�W�F�N�g�̂ݍs��
				if (g_Object[nCntObject].rotate.x == 0)
				{// ��]���ĂȂ��Ƃ��ɔ��肷��
					//��Ƃ̓����蔻��
					CollisionRockObject(nCntObject);
				}
			}

			//��]�ʂ̑��
			g_Object[nCntObject].rot += g_Object[nCntObject].rotate;

			//�ړ��ʂ̉��Z
			g_Object[nCntObject].pos += g_Object[nCntObject].move;

			switch (g_Object[nCntObject].nType)
			{
			case OBJECTTYPE_CATAPULT:
				//�₪���ɏo��
				if (GetGameState() != GAMESTATE_END)
				{
					SetRockObject(nCntObject);
				}
				break;
			case OBJECTTYPE_REFLECTOR:
				
				break;
			case OBJECTTYPE_LEVER_BAR:

				break;
			case OBJECTTYPE_TORCH:
				//�r���{�[�h�̈ʒu�ݒ菈��
				SetPositionBillboard(g_Object[nCntObject].nIdxBillboard, D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 80.0f, g_Object[nCntObject].pos.z));
				break;
			case OBJECTTYPE_BUTTON:
				//�{�^���̍X�V
				ButtonMove(nCntObject);
				break;
			case OBJECTTYPE_BUTTON_POLE:
				//�{�^���̍X�V
				ButtonPoleMove(nCntObject);
				break;
			case OBJECTTYPE_ROCK_FRAG:
				//��̌��Ђ̍X�V
				UpdateRockFrag(nCntObject);
				break;

				//���̒�========================================
			case OBJECTTYPE_SANDPOLE:

				if (g_Object[nCntObject].nCounterEff % PARTICLE_TIME == 0)
				{//���Ԋu�Ńp�[�e�B�N���ݒ�
					//�p�[�e�B�N���̐ݒ�
					SetParticle(D3DXVECTOR3(g_Object[nCntObject].pos.x,
						g_Object[nCntObject].pos.y + 500.0f,
						g_Object[nCntObject].pos.z),
						PARTICLETYPE_SANDPOLE);

					//�J�E���^�[�̃��Z�b�g
					g_Object[nCntObject].nCounterEff = 0;
				}

				//�G�t�F�N�g�̃J�E���^�[���Z
				g_Object[nCntObject].nCounterEff++;

				//�X�P�[���̊g��
				g_Object[nCntObject].fScale += POLE_SCALE_SPEED;

				if (g_Object[nCntObject].fScale >= 1.0f)
				{//�X�P�[���̐���
					g_Object[nCntObject].fScale = 1.0f;
				}

				if (g_Object[nCntObject].rot.x >= D3DX_PI * 0.5f)
				{//�|�ꂫ�����珉����Ԃɖ߂�

					//�����̗ʂɂ���ĉ��ʂ�ς���
					SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);

					//�j�󂳂�鉹�̍Đ�
					PlaySound(SOUND_LABEL_SE_CRASH);

					//�j�Ђ̐ݒ�
					SetBreakObj(
						D3DXVECTOR3
						(
							g_Object[nCntObject].mtxWorldCollision._41,
							g_Object[nCntObject].mtxWorldCollision._42,
							g_Object[nCntObject].mtxWorldCollision._43
						), 
						SANDPOLE_FRAG);
					
					//�f�t�H���g�̏�Ԃɖ߂�
					g_Object[nCntObject].rot.x = 0;
					g_Object[nCntObject].rotate = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
					g_Object[nCntObject].fScale = 0.0f;
				}

				//�G�Ƃ̓����蔻��
				if (g_Object[nCntObject].rotate.x != 0)
				{//��]���Ă��鎞
					//�ʒu�̍������擾
					D3DXVECTOR3 PosDiff = pEnemy->pos - D3DXVECTOR3(g_Object[nCntObject].mtxWorldCollision._41, g_Object[nCntObject].mtxWorldCollision._42, g_Object[nCntObject].mtxWorldCollision._43);

					//�����̋������擾
					float fLength = D3DXVec3Length(&PosDiff);
					float fRadEnemy = ENEMY_RADIUS;

					//�}�̕␳
					fLength = fLength * fLength;
					fRadEnemy = fRadEnemy * fRadEnemy;

					if (fLength <= fRadEnemy + g_Object[nCntObject].vtxMax.x * g_Object[nCntObject].vtxMax.x)
					{
						//�����̗ʂɂ���ĉ��ʂ�ς���
						SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

						//�U�����Đ�
						PlaySound(SOUND_LABEL_SE_ATTACK_000);

						//�j�Аݒ�
						SetBreakObj(g_Object[nCntObject].pos, SANDPOLE_FRAG);

						if (g_Object[nCntObject].fScale > DAMAGE_SCALE)
						{
							//�G�q�b�g����
							HitEnemy((int)(SANDPOLE_DAMAGE * g_Object[nCntObject].fScale), 0);
							//�X�R�A�̉��Z
							AddScore(SAND_POLE_SCORE);
							//�擾�X�R�A�\���ݒ�
							SetPointScore(SAND_POLE_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITSANDPOLE);
						}
						else
						{
							HitEnemy(0, 0);
						}

						//���̔��肪�Ԃ������珉����Ԃɖ߂�
						g_Object[nCntObject].rot.x = 0;
						g_Object[nCntObject].rotate = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
						g_Object[nCntObject].fScale = 0.0f;
					}

				}

				//�I�t�Z�b�g�̈ʒu�ݒ�
				UpdatePosOffset(nCntObject);
				break;
				//���̒�========================================

				//�Β�========================================
			case OBJECTTYPE_STONEPOLE:
				
				//�I�t�Z�b�g�̈ʒu�ݒ�
				UpdatePosOffset(nCntObject);

				//�X�P�[���̊g��
				g_Object[nCntObject].fScale += STONE_SCALE_SPEED;

				if (g_Object[nCntObject].fScale >= 1.0f)
				{//�X�P�[���̐���
					g_Object[nCntObject].fScale = 1.0f;
				}

				if (g_Object[nCntObject].move.y != 0)
				{//�ړ��ʂ̉��Z
					g_Object[nCntObject].move.y -= OBJ_GLAVITY;
				}

				//�G�Ƃ̓����蔻��==========
				//�ʒu�̍������擾
				D3DXVECTOR3 PosDiff =
					pEnemy->pos -
					D3DXVECTOR3
					(
						g_Object[nCntObject].mtxWorldCollision._41,
						g_Object[nCntObject].mtxWorldCollision._42,
						g_Object[nCntObject].mtxWorldCollision._43
					);

				//�����̋������擾
				float fLength = D3DXVec3Length(&PosDiff);
				float fRadEnemy = ENEMY_RADIUS;

				//�}�̕␳
				fLength = fLength * fLength;
				fRadEnemy = fRadEnemy * fRadEnemy;

				if (g_Object[nCntObject].move.y != 0)
				{
					if (fLength <= fRadEnemy + STONE_POLE_RADIUS)
					{
						//�����̗ʂɂ���ĉ��ʂ�ς���
						SetVolume(SOUND_LABEL_SE_ATTACK_000, 1.0f - fDiffLength / ROLLSOUND_RANGE);

						//�U�����Đ�
						PlaySound(SOUND_LABEL_SE_ATTACK_000);

						//�j�Аݒ�
						SetBreakObj(g_Object[nCntObject].pos, STONEPOLE_FRAG);

						if (g_Object[nCntObject].fScale > DAMAGE_SCALE)
						{
							//�G�q�b�g����
							HitEnemy((int)(STONEPOLE_DAMAGE * g_Object[nCntObject].fScale), 0);
							//�X�R�A�̑���
							AddScore(POLE_SCORE);
							//�擾�X�R�A�\���ݒ�
							SetPointScore(POLE_SCORE, D3DXVECTOR3(1000.0f, 400.0f, 0.0f), D3DXVECTOR3(1000.0f, 300.0f, 0.0f), POINTTYPE_SLIDE, 0.5f, LOGTYPE_HITSTONE);
						}
						else
						{
							HitEnemy(0, 0);
						}

						//���̔��肪�Ԃ������珉����Ԃɖ߂�
						g_Object[nCntObject].pos = g_Object[nCntObject].posBlock;
						g_Object[nCntObject].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
						g_Object[nCntObject].fScale = 0.0f;
					}
					else if (g_Object[nCntObject].mtxWorldCollision._42 < 0)
					{//�Β�����������������Ƃɖ߂�

						//�����̗ʂɂ���ĉ��ʂ�ς���
						SetVolume(SOUND_LABEL_SE_CRASH, 1.0f - fDiffLength / ROLLSOUND_RANGE);

						//�j�󂳂�鉹�̍Đ�
						PlaySound(SOUND_LABEL_SE_CRASH);

						//�j�Ђ̐ݒ�
						SetBreakObj(g_Object[nCntObject].pos, STONEPOLE_FRAG);

						//�f�t�H���g�̏�Ԃɖ߂�
						g_Object[nCntObject].pos = g_Object[nCntObject].posBlock;
						g_Object[nCntObject].move = D3DXVECTOR3{ 0.0f, 0.0f, 0.0f };
						g_Object[nCntObject].fScale = 0.0f;
					}
				}
				
				//====================

				break;
				//�Β�========================================
			}

			//�����_�̐ݒ�
			SetPoint(nCntObject);

			//��Ԃ̕ύX
			StateChangeObject();

			//�����̐��퉻
			RotNormalize(&g_Object[nCntObject].rot);

			if (g_Object[nCntObject].nType == OBJECTTYPE_ROCK_FRAG)
			{
				//�e�̈ʒu�ݒ�
				D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				rotShadow.y = g_Object[nCntObject].rot.y;

				SetPositionShadow(g_Object[nCntObject].nIdxshadow,
					D3DXVECTOR3(g_Object[nCntObject].pos.x, 0.1f, g_Object[nCntObject].pos.z),
					rotShadow,
					g_Object[nCntObject].fScale);
			}
			else
			{
				//�e�̈ʒu�ݒ�
				D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				rotShadow.y = g_Object[nCntObject].rot.y;

				SetPositionShadow(g_Object[nCntObject].nIdxshadow,
					D3DXVECTOR3(g_Object[nCntObject].pos.x, 0.1f, g_Object[nCntObject].pos.z),
					rotShadow,
					1.0f);
			}


		}
	}
}

//========================================
//�I�u�W�F�N�g�̕`�揈��
//========================================
void DrawObject(void)
{
	//���擾
	Model *pModelObject = GetModelobject();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^
	
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Object[nCntObject].mtxWorld);

			if (g_Object[nCntObject].nType == OBJECTTYPE_SANDPOLE
				|| g_Object[nCntObject].nType == OBJECTTYPE_STONEPOLE
				|| g_Object[nCntObject].nType == OBJECTTYPE_ROCK_FRAG)
			{//�X�P�[���̕ύX
				g_Object[nCntObject].mtxWorld._11 = g_Object[nCntObject].fScale;
				g_Object[nCntObject].mtxWorld._22 = g_Object[nCntObject].fScale;
				g_Object[nCntObject].mtxWorld._33 = g_Object[nCntObject].fScale;
			}

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Object[nCntObject].rot.y, g_Object[nCntObject].rot.x, g_Object[nCntObject].rot.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld, &g_Object[nCntObject].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y, g_Object[nCntObject].pos.z);

			D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorld, &g_Object[nCntObject].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCntObject].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)pModelObject[g_Object[nCntObject].nType].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pModelObject[g_Object[nCntObject].nType].dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				switch (g_Object[nCntObject].state)
				{
				case OBJECTSTATE_NORMAL:
					if (g_Object[nCntObject].nIdxEdit == GetSelectIdxEdit())
					{
						pDevice->SetMaterial(&g_typeMat[COLTYPE_DELETETARGET].MatD3D);
					}
					else
					{
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					}
					break;

				case OBJECTSTATE_DAMEGE:
					pDevice->SetMaterial(&g_Object[nCntObject].RedMat.MatD3D);
					break;
				}
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, pModelObject[g_Object[nCntObject].nType].apTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				pModelObject[g_Object[nCntObject].nType].pMesh->DrawSubset(nCntMat);
			}
			//�ۑ����Ă����}�e���A����߂� 
			pDevice->SetMaterial(&matDef);
		}
	}
}

//========================================
//�I�u�W�F�N�g�̐ݒ菈��
//========================================
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	//���擾
	Model *pModelObject = GetModelobject();

	int nCntObject;
	float fAvrRadius = 0.0f;	//���ϒl�Z�o�p
	float fWidth, fHeight;
	SHADOWTYPE type;

	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == false)
		{//�e���g�p����ĂȂ�

			//�ϐ�������
			ZeroMemory(&g_Object[nCntObject],sizeof(Object));
			g_Object[nCntObject].nIdxshadow = -1;

			//�����󂯎��
			g_Object[nCntObject].pos = pos;
			g_Object[nCntObject].rot = rot;
			g_Object[nCntObject].rotBase = rot;
			g_Object[nCntObject].nType = nType;
			g_Object[nCntObject].bUse = true;						//�g�p���Ă����Ԃɂ���
			g_Object[nCntObject].vtxMax = pModelObject[nType].vtxMax;
			g_Object[nCntObject].vtxMin = pModelObject[nType].vtxMin;

			g_Object[nCntObject].posBlock = g_Object[nCntObject].pos;

			switch (nType)
			{
			case OBJECTTYPE_BUTTON: //�{�^���̈������߂Ă����ݒ�
				g_Object[nCntObject].pos.z += BUTTON_PUSH_DEPTH;
				break;

			case OBJECTTYPE_BUTTON_POLE: //�{�^���̈������߂Ă����ݒ�
				g_Object[nCntObject].pos.y += BUTTON_PUSH_DEPTH;
				break;

			case OBJECTTYPE_TORCH: //�����̃r���{�[�h�ݒ�
				g_Object[nCntObject].nIdxBillboard = SetBillboard(BILLBOARDTYPE_FIRE, g_Object[nCntObject].pos);
				break;

			case OBJECTTYPE_SANDPOLE: //�����̃I�t�Z�b�g�ݒ�
				g_Object[nCntObject].posCollision = D3DXVECTOR3{ 0.0f, COLLISION_HEIGHT, 0.0f };
				break;

			case OBJECTTYPE_STONEPOLE: //�Β��̃I�t�Z�b�g�ݒ�
				g_Object[nCntObject].posCollision = D3DXVECTOR3{ 0.0f, -COLLISION_BELOW, 0.0f };
				break;

			case OBJECTTYPE_ROCK_FRAG: //��̌��Ђ̃I�t�Z�b�g�ݒ�
				g_Object[nCntObject].fScale = ((float)(rand() % 501) / 1000.0f) + 0.5f;
				g_Object[nCntObject].move = D3DXVECTOR3(sinf((float)((rand() % 629 - 314) / 100.0f)) * ROCKFRAG_SPEED,
					cosf((float)((rand() % 629 - 314) / 100.0f)) * ROCKFRAG_SPEED,
					cosf((float)((rand() % 629 - 314) / 100.0f)) * ROCKFRAG_SPEED);
				break;
			}

			//�����_�̐ݒ�
			SetPoint(nCntObject);

			//�C���f�b�N�X�̐ݒ�
			SetEditIndex(nCntObject);

			//�e�̐ݒ�
			if (pos.y < CEILING_POSY)
			{
				if (nType != OBJECTTYPE_STONEPOLE)
				{
					if (g_Object[nCntObject].nType == OBJECTTYPE_CATAPULT ||
						g_Object[nCntObject].nType == OBJECTTYPE_BUTTON ||
						g_Object[nCntObject].nType == OBJECTTYPE_REFLECTOR ||
						g_Object[nCntObject].nType == OBJECTTYPE_SANDPOLE_BASE ||
						g_Object[nCntObject].nType == OBJECTTYPE_DOOR ||
						g_Object[nCntObject].nType == OBJECTTYPE_TREASUREBOX ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_0 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_1 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_4 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_5 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_6 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_7 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_8 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_100 ||
						g_Object[nCntObject].nType == OBJECTTYPE_STATUE_101)
					{
						type = SHADOWTYPE_SQUARE;

							fWidth = g_Object[nCntObject].vtxMax.x * SHADOW_SCALE_SQR;
							fHeight = g_Object[nCntObject].vtxMax.z * SHADOW_SCALE_SQR;
					}
					else
					{
						g_Object[nCntObject].vtxMax.x <= g_Object[nCntObject].vtxMax.z ?
							fAvrRadius = g_Object[nCntObject].vtxMax.z * SHADOW_SCALE_CCL : 
							fAvrRadius = g_Object[nCntObject].vtxMax.x * SHADOW_SCALE_CCL;
						type = SHADOWTYPE_CIRCLE;
						fWidth = 0.0f;
						fHeight = 0.0f;
					}

					D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					rotShadow.y = g_Object[nCntObject].rot.y;

					g_Object[nCntObject].nIdxshadow = SetShadow(fAvrRadius,	type, fWidth, fHeight, rotShadow);
				}
			}

			break;
		}
	}
}

//========================================
//�I�u�W�F�N�g�̏�������
//========================================
void DeleteObject(int nIdx)
{
	g_Object[nIdx].bUse = false;
	g_Object[nIdx].pos = { 0.0f,0.0f,0.0f };
	g_Object[nIdx].rot = { 0.0f,0.0f,0.0f };
	g_Object[nIdx].fScale = 1.0f;

	//�e�̍폜
	EnableShadow(g_Object[nIdx].nIdxshadow, false);
}

//========================================
//��Ԃ̕ύX
//========================================
void StateChangeObject(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		g_Object[nCntObject].posOld = g_Object[nCntObject].pos;

		if (g_Object[nCntObject].bUse == true)
		{
			switch (g_Object[nCntObject].state)
			{
			case OBJECTSTATE_NORMAL:
				break;

			case OBJECTSTATE_DAMEGE:
				g_Object[nCntObject].nCounterState--;

				if (g_Object[nCntObject].nCounterState <= 0)
				{
					g_Object[nCntObject].state = OBJECTSTATE_NORMAL;
				}
				break;
			}
		}
	}
}

//========================================
//�I�u�W�F�N�g�Ƃ̓����蔻��
//========================================
bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump)
{
	//�ϐ��錾
	int nCntObject;
	int *pHitRockCouter = GetHitRockNum();
	Billboard *pBillboard = GetBillboard();
	float fConv = 0.0f;

	bool bLand = false;			//���n�������ǂ���

	for (nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true
			&& (g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR
			&& g_Object[nCntObject].nType != OBJECTTYPE_LEVER_BAR
			&& g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR_BASE
			&& g_Object[nCntObject].nType != OBJECTTYPE_ROCK_FRAG))
		{

			if ((g_Object[nCntObject].rot.y <= D3DX_PI * 0.51f && g_Object[nCntObject].rot.y >= D3DX_PI * 0.49f ) ||
				(g_Object[nCntObject].rot.y >= D3DX_PI * -0.51f && g_Object[nCntObject].rot.y <= D3DX_PI * -0.49f))
			{//90�x��������z��x�����ւ���
				fConv = g_Object[nCntObject].vtxMin.x;
				g_Object[nCntObject].vtxMin.x = g_Object[nCntObject].vtxMin.z;
				g_Object[nCntObject].vtxMin.z = fConv;

				fConv = g_Object[nCntObject].vtxMax.x;
				g_Object[nCntObject].vtxMax.x = g_Object[nCntObject].vtxMax.z;
				g_Object[nCntObject].vtxMax.z = fConv;
			}

			if (pPos->y <= g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y &&
				pPos->y + vtxMax.y >= g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMin.y)
			{//�㉺�͈͓̔��ɂ���Ƃ�
				if (pPosOld->x + vtxMax.z <= g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
					&& pPos->x + vtxMax.z > g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
					&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
					&& pPos->z + vtxMin.z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z)
				{//�E�̔���
					pPos->x = g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x - vtxMax.z - 0.05f;
				}
				if (pPosOld->x - vtxMax.z >= g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
					&& pPos->x - vtxMax.z < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
					&& pPos->z - vtxMax.z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z
					&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z)
				{//���̔���
					pPos->x = g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x + vtxMax.z + 0.05f;
				}
				if (pPosOld->z + vtxMax.z <= g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
					&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
					&& pPos->x + vtxMax.x > g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
					&& pPos->x - vtxMax.x < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x)
				{//�O�̔���
					pPos->z = g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z - vtxMax.z - 0.05f;

					if (g_buttonState == BUTONSTATE_WAIT && g_Object[nCntObject].nType == OBJECTTYPE_BUTTON)
					{//�ҋ@��Ԃł̂݉�����
						g_Object[nCntObject].pos.z += 1.0f;
					
						if (g_bOpenRock == false)
						{
							if (g_Object[nCntObject].nCntSound <= 0)
							{
								PlaySound(SOUND_LABEL_SE_SHIFT);
								g_Object[nCntObject].nCntSound = SHIFT_SOUND_TIME;
							}
							else
							{
								g_Object[nCntObject].nCntSound--;
							}
						}
					}

					if (g_Object[nCntObject].pos.z >= g_Object[nCntObject].posBlock.z + BUTTON_PUSH_DEPTH)
					{//�ړ�����
						g_Object[nCntObject].pos.z = g_Object[nCntObject].posBlock.z + BUTTON_PUSH_DEPTH;

						g_bOpenRock = true;
						*pHitRockCouter = 0;
						if (g_bButtonSound == false)
						{							
							PlaySound(SOUND_LABEL_SE_SHIFT);
							g_bButtonSound = true;
						}

						//�`���[�g���A����i�߂�
						TUTORIALSTATE *tutorial = GetTutorialState();
						if (*tutorial == TUTORIALSTATE_BUTTON && GetMode() == MODE_TUTORIAL)
						{
							*tutorial = TUTORIALSTATE_JUMP;

							SetCount(D3DXVECTOR3(640.0f, 120.0, 0.0f), 100.0f, 2);
							PlaySound(SOUND_LABEL_SE_BUTTON_004);

							for (int nCntButton = 0; nCntButton < MAX_OBJECT; nCntButton++)
							{
								switch (g_Object[nCntButton].nType)
								{
								case OBJECTTYPE_BUTTON:
									pBillboard[g_Object[nCntButton].nIdxBillboard].bUse = false;
									break;
								}
							}
						}
					}
				}

				if (pPosOld->z - vtxMax.z >= g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z
					&& pPos->z - vtxMax.z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z
					&& pPos->x - vtxMax.x < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
					&& pPos->x + vtxMax.x > g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x)
				{//���̔���
					pPos->z = g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z + vtxMax.z + 0.05f;

				}
			}
			if (pPos->x + vtxMax.x >= g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMin.x
				&& pPos->x < g_Object[nCntObject].pos.x + g_Object[nCntObject].vtxMax.x
				&& pPos->z + vtxMax.z > g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMin.z
				&& pPos->z < g_Object[nCntObject].pos.z + g_Object[nCntObject].vtxMax.z)
			{
				if (pPosOld->y >= g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y
					&& pPos->y < g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y)
				{
					bLand = true;
					pPos->y = g_Object[nCntObject].pos.y + g_Object[nCntObject].vtxMax.y;
					pMove->y = 0.0f;
					if (g_Object[nCntObject].nType == OBJECTTYPE_TORCH)
					{
						HitPlayer(1);
					}

					if (g_buttonStatePole == BUTONSTATE_WAIT && g_Object[nCntObject].nType == OBJECTTYPE_BUTTON_POLE)
					{//�ҋ@��Ԃł̂݉�����
						g_Object[nCntObject].pos.y -= 1.0f;

						if (g_bPoleAttack)
						{
							if (g_Object[nCntObject].nCntSound <= 0)
							{
								PlaySound(SOUND_LABEL_SE_SHIFT);
								g_Object[nCntObject].nCntSound = SHIFT_SOUND_TIME;
							}
							else
							{
								g_Object[nCntObject].nCntSound--;
							}
						}

						if (g_Object[nCntObject].pos.y <= g_Object[nCntObject].posBlock.y && g_bPoleAttack)
						{//�ړ�����
							g_Object[nCntObject].pos.y = g_Object[nCntObject].posBlock.y;

							g_bPoleAttack = false;

							Enemy *enemy = GetEnemy();
							D3DXVECTOR3 posEnemy = enemy->pos;
							posEnemy.y += 490.0f;

							SetRock
							(
								posEnemy,
								D3DXVECTOR3(0.0f,0.0f,0.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								LIFE_ROCK,
								ROCKTYPE_DROP
							);
						}
					}
				}
			}
		}
	}
	return bLand;
}

//========================================
// �I�u�W�F�N�g�̃q�b�g����
//========================================
void HitObject(int nDamage,int nCntObject)
{
	//�̗͂����炷
	g_Object[nCntObject].nLife -= nDamage;
	
	//�v���C���[�̗̑͂��Ȃ��Ȃ���
	if (g_Object[nCntObject].nLife == 0)
	{
		DeleteObject(nCntObject);
	}
	else
	{//�̗͂��c���Ă���
		g_Object[nCntObject].state = OBJECTSTATE_DAMEGE;
		g_Object[nCntObject].nCounterState = 5;
	}
}

//========================================
//�I�u�W�F�N�g�̎擾
//========================================
Object *ObjectGet(void)
{
	return &g_Object[0];
}

//========================================
//�w��̃I�u�W�F�N�g�̃��b�V���̎擾
//========================================
LPD3DXMESH ObjectMeshGet(OBJECTTYPE type)
{
	//���擾
	Model *pModelObject = GetModelobject();

	return pModelObject[type].pMesh;
}

//========================================
//�w��̃I�u�W�F�N�g�̃}�e���A���̎擾
//========================================
LPD3DXBUFFER ObjectBuffmatGet(OBJECTTYPE type)
{
	//���擾
	Model *pModelObject = GetModelobject();

	return pModelObject[type].pBuffMat;
}

//========================================
//�w��̃I�u�W�F�N�g�̃}�e���A�����̎擾
//========================================
DWORD ObjectdwNummatGet(OBJECTTYPE type)
{
	//���擾
	Model *pModelObject = GetModelobject();

	return pModelObject[type].dwNumMat;
}

//==============================
//�I�u�W�F�N�g�̎g�p���擾����
//==============================
int GetUseObject(void)
{
	int nNumUse = 0;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (g_Object[nCnt].bUse == true)
		{
			nNumUse++;
		}
	}

	return nNumUse;
}

//========================================
//�₪���ŏo��
//========================================
void SetRockObject(int nCntObject)
{
	if (g_bOpenRock == true)
	{
		g_Object[nCntObject].nCounter++;

		if ((g_Object[nCntObject].nCounter % INJECT_TIME) == 0)
		{//��莞�Ԃ��ƂɊ�𔭎˂���
			SetRock
			(
				D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 50.0f, g_Object[nCntObject].pos.z),
				D3DXVECTOR3
				(
					sinf(g_Object[nCntObject].rot.y) * -SPEED_ROCK,
					0.0f,
					cosf(g_Object[nCntObject].rot.y)  * -SPEED_ROCK
				),
				g_Object[nCntObject].rot,
				LIFE_ROCK,
				ROCKTYPE_GIMMICK
			);
			g_Object[nCntObject].RockCounter++;
		}

		if (g_bOpenRock == false)
		{
			g_Object[nCntObject].nCounter = 1;
		}
	}

	if (g_Object[nCntObject].pos.y >= GetCeilingPos().y)
	{//�V���荂���ꍇ
		SetRock
		(
			D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 50.0f, g_Object[nCntObject].pos.z),
			D3DXVECTOR3
			(
				sinf(g_Object[nCntObject].rot.y) * -SPEED_ROCK * 1.0f,
				0.0f,
				cosf(g_Object[nCntObject].rot.y)  * -SPEED_ROCK * 1.0f
			),
			g_Object[nCntObject].rot,
			180,
			ROCKTYPE_INVISIBLE
		);
	}
	else
	{
		SetRock
		(
			D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 50.0f, g_Object[nCntObject].pos.z),
			D3DXVECTOR3
			(
				sinf(g_Object[nCntObject].rot.y) * -SPEED_ROCK * 1.0f,
				0.0f,
				cosf(g_Object[nCntObject].rot.y)  * -SPEED_ROCK * 1.0f
			),
			g_Object[nCntObject].rot,
			5,
			ROCKTYPE_INVISIBLE
		);
	}
}

//========================================
//�r���{�[�h�̏o��
//========================================
void BillboardUse(Object *pObject)
{
	Player *pPlayer = GetPlayer();				//�v���C���[�̃|�C���^
	Billboard *pBillboard = GetBillboard();
	//int nCntObject;

	float fLength;		//2�_�Ԃ̒���

	if (pObject->bBillboard == false)
	{
		//�~�̓����蔻��
		fLength = (pPlayer->pos.x - pObject->pos.x) * (pPlayer->pos.x - pObject->pos.x)
			+ (pPlayer->pos.z - pObject->pos.z) * (pPlayer->pos.z - pObject->pos.z);

		if (fLength < (200.0f * 200.0f))
		{
			//�r���{�[�h�̐ݒ菈��
			pObject->nIdxBillboard = SetBillboard(BILLBOARDTYPE_ICON,pObject->pos);
			pObject->bBillboard = true;
			//GetKeyboardTrigger
			if (GetKeyboardTrigger(DIK_M) == true)
			{
				g_Object[pObject->nIdxBillboard].rot.y += D3DX_PI * 0.2f;
			}
		}
	}

	if (pObject->bBillboard == true)
	{
		//�~�̓����蔻��
		fLength = (pPlayer->pos.x - pObject->pos.x) * (pPlayer->pos.x - pObject->pos.x)
			+ (pPlayer->pos.z - pObject->pos.z) * (pPlayer->pos.z - pObject->pos.z);

		if (fLength > (200.0f * 200.0f))
		{
			//�r���{�[�h�̐ݒ菈��
			pBillboard[pObject->nIdxBillboard].bUse = false;
			pObject->bBillboard = false;
		}
	}
}

//===========================
//�����_�̐ݒ菈��
//===========================
void SetPoint(int nNum)
{
	float fAngle, fLength;

	if (g_Object[nNum].nType != OBJECTTYPE_LEVER_BAR)
	{
		//�Ίp���̒������Z�o����
		fLength = sqrtf((float)pow(g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x, 2) + (float)pow(g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z, 2)) * 0.5f;

		//�Ίp���̊p�x���Z�o����
		fAngle = atan2f(g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x, g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z);

		//����̊�_�v�Z
		g_Object[nNum].point[0].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y + fAngle) * fLength;
		g_Object[nNum].point[0].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[0].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + fAngle) * fLength;

		g_Object[nNum].point[1].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y - fAngle) * fLength;
		g_Object[nNum].point[1].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[1].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y - fAngle) * fLength;

		g_Object[nNum].point[2].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y - D3DX_PI + fAngle) * fLength;
		g_Object[nNum].point[2].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[2].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y - D3DX_PI + fAngle) * fLength;

		g_Object[nNum].point[3].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y + D3DX_PI - fAngle) * fLength;
		g_Object[nNum].point[3].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[3].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + D3DX_PI - fAngle) * fLength;
	}
	else if (g_Object[nNum].nType == OBJECTTYPE_LEVER_BAR)
	{
		//�Ίp���̒������Z�o����
		fLength = sqrtf((float)pow((g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x), 2) + (float)pow(g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z, 2));

		//�Ίp���̊p�x���Z�o����
		fAngle = atan2f((g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x), g_Object[nNum].vtxMax.z - g_Object[nNum].vtxMin.z);

		//����̊�_�v�Z
		g_Object[nNum].point[0].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y + (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);
		g_Object[nNum].point[0].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[0].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);

		g_Object[nNum].point[1].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y - (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);
		g_Object[nNum].point[1].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMax.y;
		g_Object[nNum].point[1].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y - (D3DX_PI * 0.5f)) * (g_Object[nNum].vtxMax.x - g_Object[nNum].vtxMin.x);

		g_Object[nNum].point[2].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y  + D3DX_PI + fAngle) * fLength * 0.95f;
		g_Object[nNum].point[2].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[2].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y + D3DX_PI  + fAngle) * fLength * 0.95f;

		g_Object[nNum].point[3].x = g_Object[nNum].pos.x + sinf(g_Object[nNum].rot.y  + D3DX_PI - fAngle) * fLength * 0.95f;
		g_Object[nNum].point[3].y = g_Object[nNum].pos.y + g_Object[nNum].vtxMin.y;
		g_Object[nNum].point[3].z = g_Object[nNum].pos.z + cosf(g_Object[nNum].rot.y  + D3DX_PI - fAngle) * fLength * 0.95f;
	}
}

//===========================
//���˔̓����蔻�蔽�ˏ���
//===========================
bool CollisionReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength, int type)
{
	//�����擾
	D3DXVECTOR3 posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fDiffLength = 0.0f;

	bool bMeshWall;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fDot, fRock, fLengthRock, fOuterRef[2], fRotRock;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef, posCol;

	bMeshWall = false;
	bool bNear;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		posDiff = g_Object->pos - GetPlayer()->pos;
		fDiffLength = D3DXVec3Length(&posDiff);

		if (g_Object[nCnt].bUse == true && g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR)
		{
			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				//����p�x�N�g���v�Z
				if (nCnt1 == 3)
				{
					//��̊p�x
					fRock = atan2f(g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//���ʂ̃x�N�g��
					vecLine.x = g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z;

					//��_�Ɗ�̈ʒu�̃x�N�g��
					vecToPos.x = (pos->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;

					//��_�Ɗ�̑O�̈ʒu�̃x�N�g��
					vecToPosOld.x = (posOld->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;
				}
				else
				{
					//��̊p�x
					fRock = atan2f(g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//���ʂ̃x�N�g��
					vecLine.x = g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z;

					//��_�Ɗ�̈ʒu�̃x�N�g��
					vecToPos.x = (pos->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;

					//��_�Ɗ�̑O�̈ʒu�̃x�N�g��
					vecToPosOld.x = (posOld->x + (fLength * sinf(fRock))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fLength * cosf(fRock))) - g_Object[nCnt].point[nCnt1].z;
				}

				//�ړ��ʃx�N�g��
				vecMove = *pos - *posOld;

				//���ʂƊ�̈ʒu�̊O��
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
				fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

				//��_�v�Z
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				//��̒��S�ƃ|�C���g�̋����v�Z
				fLengthRock = sqrtf((float)pow(g_Object[nCnt].point[nCnt1].x - pos->x, 2) + (float)pow(g_Object[nCnt].point[nCnt1].z - pos->z, 2));

				//�p�ɓ������Ă���
				if (fLengthRock < fLength)
				{
					if (pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y && pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y)
					{
						//��̊p�x
						fRock = atan2f(pos->x - g_Object[nCnt].point[nCnt1].x, pos->z - g_Object[nCnt].point[nCnt1].z);

						//��̈ʒu�ړ�
						pos->x = g_Object[nCnt].point[nCnt1].x + (sinf(fRock) * fLength);
						pos->z = g_Object[nCnt].point[nCnt1].z + (cosf(fRock) * fLength);

						//�@���x�N�g���v�Z
						vecNor.x = pos->x - g_Object[nCnt].point[nCnt1].x;
						vecNor.y = 0.0f;
						vecNor.z = pos->z - g_Object[nCnt].point[nCnt1].z;

						//�@���x�N�g���P�ʉ�
						D3DXVec3Normalize(&vecNor, &vecNor);

						//���������傫��
						fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

						//���������x�N�g���v�Z
						vecDot = (vecNor * fDot) * 2;

						//�ړ��ʃx�N�g���v�Z
						vecMoveRef = vecMove + vecDot;

						//�ړ��ʃx�N�g���P�ʉ�
						D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

						//���������|�C���g�����̈ړ������Ɠ��������|�C���g�Ǝ��̃|�C���g�̊O�όv�Z
						if (nCnt1 == 0)
						{
							fOuterRef[0] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[3]);
						}
						else
						{
							fOuterRef[0] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[nCnt1 - 1]);
						}

						//���������|�C���g�����̈ړ������Ɠ��������|�C���g�ƑO�̃|�C���g�̊O�όv�Z
						if (nCnt1 == 3)
						{
							fOuterRef[1] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[0]);
						}
						else
						{
							fOuterRef[1] = OuterProduct(g_Object[nCnt].point[nCnt1], g_Object[nCnt].point[nCnt1] + vecMoveRef, g_Object[nCnt].point[nCnt1 + 1]);
						}

						//�ړ��������I�u�W�F�N�g���������Ă邩�ǂ���
						if (fOuterRef[0] < 0.0f && fOuterRef[1] > 0.0f)
						{
							//�ǂ���̒������߂���
							if (-fOuterRef[0] < fOuterRef[1])
							{
								//�p�x�v�Z
								if (nCnt1 == 3)
								{
									fRotRock = atan2f(g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z);
								}
								else
								{
									fRotRock = atan2f(g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z);
								}

								//�ړ���������
								vecMoveRef.x = (-sinf(fRotRock) * vecMoveRef.z) + (cosf(fRotRock) * vecMoveRef.x);
								vecMoveRef.z = (sinf(fRotRock) * vecMoveRef.x) + (cosf(fRotRock) * vecMoveRef.z);
							}
							else
							{
								//�p�x�v�Z
								if (nCnt1 == 0)
								{
									fRotRock = atan2f(g_Object[nCnt].point[3].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[3].z - g_Object[nCnt].point[nCnt1].z);
								}
								else
								{
									fRotRock = atan2f(g_Object[nCnt - 1].point[nCnt1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 - 1].z - g_Object[nCnt].point[nCnt1].z);
								}

								//�ړ���������
								vecMoveRef.x = (-sinf(fRotRock) * vecMoveRef.z) + (cosf(fRotRock) * vecMoveRef.x);
								vecMoveRef.z = (sinf(fRotRock) * vecMoveRef.x) + (cosf(fRotRock) * vecMoveRef.z);
							}
						}

						//�ړ��ʃx�N�g���P�ʉ�
						D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

						if (type == ROCKTYPE_INVISIBLE)
						{
							//�ړ��ʑ��
							*move = vecMoveRef * SPEED_ROCK * 1.0f;
						}
						else
						{
							//�ړ��ʑ��
							*move = vecMoveRef * SPEED_ROCK;
						
							//���擾
							Player *pPlayer = GetPlayer();

							// ������������ʒu�ɂ��邩�̓����蔻��
							bNear = SphereCollision(g_Object[nCnt].pos, pPlayer->pos, ROLLSOUND_RANGE, 0.0f);

							if (bNear)
							{//������������͈͂Ȃ�

								//�����̗ʂɂ���ĉ��ʂ�ς���
								SetVolume(SOUND_LABEL_SE_COLROCK, 1.0f - fDiffLength / ROLLSOUND_RANGE);

								PlaySound(SOUND_LABEL_SE_COLROCK);
							}
						}

						bMeshWall = true;

						break;
					}
				}

				//���ʏՓ˔���
				if (fOutToPos < 0.0f && fOutToPosOld > 0.0f && fRate < 1.0f && fRate > 0.0f)
				{
					if (pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y && pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y)
					{
						//��̈ʒu�ړ�
						pos->x = g_Object[nCnt].point[nCnt1].x + (vecLine.x * fRate) - (fLength * sinf(fRock));
						pos->z = g_Object[nCnt].point[nCnt1].z + (vecLine.z * fRate) - (fLength * cosf(fRock));

						//�@���x�N�g���v�Z
						vecNor.x = vecLine.z;
						vecNor.y = 0.0f;
						vecNor.z = -vecLine.x;

						//�@���x�N�g���P�ʉ�
						D3DXVec3Normalize(&vecNor, &vecNor);

						//���������傫��
						fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

						//���������x�N�g���v�Z
						vecDot = (vecNor * fDot) * 2;

						//�ړ��ʃx�N�g���v�Z
						vecMoveRef = vecMove + vecDot;

						//�ړ��ʃx�N�g���P�ʉ�(�K�v��������Ȃ�)
						D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

						if (type == ROCKTYPE_INVISIBLE)
						{
							//�ړ��ʑ��
							*move = vecMoveRef * SPEED_ROCK * 1.0f;
						}
						else
						{
							//�ړ��ʑ��
							*move = vecMoveRef * SPEED_ROCK;
						
							//���擾
							Player *pPlayer = GetPlayer();

							// ������������ʒu�ɂ��邩�̓����蔻��
							bNear = SphereCollision(g_Object[nCnt].pos, pPlayer->pos, ROLLSOUND_RANGE, 0.0f);

							if (bNear)
							{//������������͈͂Ȃ�

							 //�����̗ʂɂ���ĉ��ʂ�ς���
								SetVolume(SOUND_LABEL_SE_COLROCK, 1.0f - fDiffLength / ROLLSOUND_RANGE);

								PlaySound(SOUND_LABEL_SE_COLROCK);
							}
						}

						bMeshWall = true;
					}
				}
			}
		}
	}

	return bMeshWall;
}

//===========================
//��ƃI�u�W�F�N�g�̓����蔻��
//===========================
void CollisionRockObject(int nCntObject)
{
	//���̎擾
	Rock *pRock = GetRock();

	//�ϐ��錾
	float fAveMax;	//�ő咸�_�̕���
	D3DXVECTOR3 PosDiff;
	bool bHit;

	for (int nCntRock = 0; nCntRock < MAX_ROCK; nCntRock++, pRock++)
	{
		if (pRock->bUse && pRock->nCounter >= ROCK_TIME)
		{//�g�p���Ă����ԂȂ�
			//�������擾
			PosDiff = pRock->pos - g_Object[nCntObject].pos;

			//�I�u�W�F�N�g�̍ő咸�_���畽�ς��Z�o
			fAveMax = (g_Object[nCntObject].vtxMax.x + g_Object[nCntObject].vtxMax.z) / 2;

			//���̓����蔻�菈��
			bHit = SphereCollision(g_Object[nCntObject].pos, pRock->pos, fAveMax * COLLISION_RATE, pRock->vtxMax.x * COLLISION_RATE);

			if (bHit)
			{//���肪����������
				float fAngle = 0.0f;

				switch (g_Object[nCntObject].nType)
				{
				case OBJECTTYPE_SANDPOLE:
					//���̒���|��============
					//�p�x���Z�o
					fAngle = atan2f(PosDiff.x, PosDiff.z);

					//����|�����Ɍ�����
					g_Object[nCntObject].rot.y = fAngle + D3DX_PI;

					//��]�ʂɑ��
					g_Object[nCntObject].rotate.x = ROTATE_SPEED;
					//========================

					//���j�󂷂�
					DisableRock(pRock);
					break;

				case OBJECTTYPE_STONEPOLE:
					//�Β��𗎂Ƃ�
					g_Object[nCntObject].move.y = -FALL_SPEED;
					break;

				case OBJECTTYPE_ROCK_FRAG:
					break;

				default:

					//�����
					DisableRock(pRock);

					break;
				}
			}
		}
	}
}

//===========================
//�����Ȋ�ƃI�u�W�F�N�g�̓����蔻��
//===========================
bool CollisionRockInvisible(D3DXVECTOR3 *pos, float *vtxMax)
{
	//�ϐ��錾
	float fAveMax;	//�ő咸�_�̕���
	bool bHit;

	bHit = false;

	for (int nCntObject = 0; nCntObject < MAX_ROCK; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{
			if (g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR &&
				g_Object[nCntObject].nType != OBJECTTYPE_REFLECTOR_BASE &&
				g_Object[nCntObject].nType != OBJECTTYPE_SANDPOLE_BASE &&
				g_Object[nCntObject].nType != OBJECTTYPE_ROCK_FRAG &&
				g_Object[nCntObject].nType != OBJECTTYPE_CATAPULT)
			{
				//�I�u�W�F�N�g�̍ő咸�_���畽�ς��Z�o
				fAveMax = (g_Object[nCntObject].vtxMax.x + g_Object[nCntObject].vtxMax.z) / 2;

				//���̓����蔻�菈��
				if (SphereCollision(g_Object[nCntObject].pos, *pos, fAveMax, *vtxMax) == true)
				{
					bHit = true;
				}
			}
		}
	}

	return bHit;
}

//===========================
//���˔̓����蔻���]����
//===========================
bool PushReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float *rot, bool jump)
{
	//���擾
	Camera *pCamara = GetCamera();
	Player *pPlayer = GetPlayer();
	Billboard *pBillboard = GetBillboard();
	
	//�ϐ��錾
	bool bReflector;
	int nInnerCnt = 0;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fRef, fLengthPlayer, fRotRef, fRefLength;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef;

	bReflector = false;

	for (int nCnt = 0; nCnt < MAX_OBJECT; nCnt++)
	{
		if (g_Object[nCnt].bUse == true && (g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR || g_Object[nCnt].nType == OBJECTTYPE_LEVER_BAR))
		{
			if (g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR)
			{
				fRefLength = REF_LENGTH;
			}
			else if (g_Object[nCnt].nType == OBJECTTYPE_LEVER_BAR)
			{
				fRefLength = 23.0f;
			}

			for (int nCnt1 = 0; nCnt1 < 4; nCnt1++)
			{
				//����p�x�N�g���v�Z
				if (nCnt1 == 3)
				{
					//�v���C���[�̊p�x
					fRef = atan2f(g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//��_�ƃv���C���[�̈ʒu�̃x�N�g��
					vecToPos.x = (pos->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//��_�ƃv���C���[�̑O�̈ʒu�̃x�N�g��
					vecToPosOld.x = (posOld->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//���ʂ̃x�N�g��
					vecLine.x = g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z;
				}
				else
				{
					//�v���C���[�̊p�x
					fRef = atan2f(g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x, g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z) - (D3DX_PI * 0.5f);

					//��_�ƃv���C���[�̈ʒu�̃x�N�g��
					vecToPos.x = (pos->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPos.z = (pos->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//��_�ƃv���C���[�̑O�̈ʒu�̃x�N�g��
					vecToPosOld.x = (posOld->x + (fRefLength * sinf(fRef))) - g_Object[nCnt].point[nCnt1].x;
					vecToPosOld.z = (posOld->z + (fRefLength * cosf(fRef))) - g_Object[nCnt].point[nCnt1].z;

					//���ʂ̃x�N�g��
					vecLine.x = g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x;
					vecLine.z = g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z;
				}

				//�ړ��ʃx�N�g��
				vecMove = *pos - *posOld;

				//���ʂƃv���C���[�̈ʒu�̊O��
				fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
				fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

				//��_�v�Z
				fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
				fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
				fRate = fOutRate / fOutUnit;

				//�v���C���[�̒��S�ƃ|�C���g�̋����v�Z
				fLengthPlayer = hypotf(g_Object[nCnt].point[nCnt1].x - pos->x, g_Object[nCnt].point[nCnt1].z - pos->z);

				if (fOutToPos < 0.0f)
				{
					nInnerCnt++;
				}

				//���ʏՓ˔���
				if (fOutToPos < 0.0f && fOutToPosOld > 0.0f && fRate < 1.0f && fRate > 0.0f)
				{
					if (pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y - 40.0f)
					{
						//���������̂�bool�^��true�ɂ���
						bReflector = true;

						if (jump == false)
						{
							if (g_Object[nCnt].nType == OBJECTTYPE_REFLECTOR)
							{
								//���ʏՓ˔���
								if (fRate > 0.6f)
								{
									//���˔�]
									g_Object[nCnt].rot.y -= ROT_SPEED * fRate;

									//�J������]
									pCamara->rot.y += ROT_SPEED * fRate;

									if (g_Object[nCnt].nCntSound == 0)
									{
										//�΂����炷���Đ�
										PlaySound(SOUND_LABEL_SE_SHIFT);

										//�J�E���g���Z�b�g
										g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
									}

									//�J�E���g���Z
									g_Object[nCnt].nCntSound--;
								}
								else if (fRate < 0.4f)
								{
									//���˔�]
									g_Object[nCnt].rot.y += ROT_SPEED * (1.0f - fRate);

									//�J������]
									pCamara->rot.y -= ROT_SPEED * (1.0f - fRate);
									
									if (g_Object[nCnt].nCntSound == 0)
									{
										//�΂����炷���Đ�
										PlaySound(SOUND_LABEL_SE_SHIFT);

										//�J�E���g���Z�b�g
										g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
									}

									//�J�E���g���Z
									g_Object[nCnt].nCntSound--;
								}

								TUTORIALSTATE *tutorial = GetTutorialState();
								if (*tutorial == TUTORIALSTATE_REFLECTOR && GetMode() == MODE_TUTORIAL)
								{//�{�^���̃`���[�g���A���ɐi��
									*tutorial = TUTORIALSTATE_BUTTON;

									g_bOpenRock = false;

									g_buttonState = BUTONSTATE_FORWARD;

									PlaySound(SOUND_LABEL_SE_BUTTON_004);
								
									for (int nCntButton = 0; nCntButton < MAX_OBJECT; nCntButton++)
									{
										if (g_Object[nCntButton].nType == OBJECTTYPE_BUTTON)
										{
											g_Object[nCntButton].nIdxBillboard = SetBillboard(BILLBOARDTYPE_GUIDE,
												D3DXVECTOR3(g_Object[nCntButton].pos.x,
													g_Object[nCntButton].pos.y + BUTTON_BILLBOARD,
													g_Object[nCntButton].pos.z));
											break;
										}
									}
								}
							}
							else if (g_Object[nCnt].nType == OBJECTTYPE_LEVER_BAR)
							{
								for (int nCntBar = 0; nCntBar < MAX_OBJECT; nCntBar++)
								{
									if (g_Object[nCntBar].nType == OBJECTTYPE_LEVER_BAR)
									{
										if (nCnt1 == 1)
										{
											//���˔�]
											g_Object[nCntBar].rot.y -= (ROT_SPEED * 0.4f) * fRate;

											if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
											{
												//�J������]
												pCamara->rot.y += (ROT_SPEED * 0.4f) * fRate;
											}

											if (g_Object[nCnt].nCntSound == 0)
											{
												//�΂����炷���Đ�
												PlaySound(SOUND_LABEL_SE_SHIFT);

												//�J�E���g���Z�b�g
												g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
											}

											//�J�E���g���Z
											g_Object[nCnt].nCntSound--;
										}
										else if (nCnt1 == 3)
										{
											//���˔�]
											g_Object[nCntBar].rot.y += (ROT_SPEED * 0.4f) * (1.0f - fRate);

											if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
											{
												//�J������]
												pCamara->rot.y -= (ROT_SPEED * 0.4f) * (1.0f - fRate);
											}

											if (g_Object[nCnt].nCntSound == 0)
											{
												//�΂����炷���Đ�
												PlaySound(SOUND_LABEL_SE_SHIFT);

												//�J�E���g���Z�b�g
												g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
											}

											//�J�E���g���Z
											g_Object[nCnt].nCntSound--;
										}
										else
										{
											//���ʏՓ˔���
											if (fRate > 0.6f)
											{
												//���˔�]
												g_Object[nCntBar].rot.y -= (ROT_SPEED * 0.4f) * fRate;

												if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
												{
													//�J������]
													pCamara->rot.y += (ROT_SPEED * 0.4f) * fRate;
												}

												if (g_Object[nCnt].nCntSound == 0)
												{
													//�΂����炷���Đ�
													PlaySound(SOUND_LABEL_SE_SHIFT);

													//�J�E���g���Z�b�g
													g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
												}

												//�J�E���g���Z
												g_Object[nCnt].nCntSound--;
											}
											else if (fRate < 0.4f)
											{
												//���˔�]
												g_Object[nCntBar].rot.y += (ROT_SPEED * 0.4f) * (1.0f - fRate);

												if (g_Object[nCntBar].rot.y <= D3DX_PI * 0.15f && g_Object[nCntBar].rot.y >= -D3DX_PI * 0.15f && nCntBar == nCnt)
												{
													//�J������]
													pCamara->rot.y -= (ROT_SPEED * 0.4f) * (1.0f - fRate);
												}

												if (g_Object[nCnt].nCntSound == 0)
												{
													//�΂����炷���Đ�
													PlaySound(SOUND_LABEL_SE_SHIFT);

													//�J�E���g���Z�b�g
													g_Object[nCnt].nCntSound = SHIFT_SOUND_TIME;
												}

												//�J�E���g���Z
												g_Object[nCnt].nCntSound--;
											}
										}

										fRotRef = g_Object[nCntBar].rot.y - g_Object[nCntBar].rotBase.y;

										if (fRotRef > D3DX_PI)
										{
											fRotRef -= (D3DX_PI * 2);
										}
										else if (fRotRef <= -D3DX_PI)
										{
											fRotRef += (D3DX_PI * 2);
										}
										
										if (fRotRef > D3DX_PI * 0.15f)
										{
											g_Object[nCntBar].rot.y = g_Object[nCntBar].rotBase.y + D3DX_PI * 0.15f;
										}

										if (fRotRef < -D3DX_PI * 0.15f)
										{
											g_Object[nCntBar].rot.y = g_Object[nCntBar].rotBase.y - D3DX_PI * 0.15f;
										}
									}
								}
							}
						}

						SetPoint(nCnt);

						//����p�x�N�g���v�Z
						if (nCnt1 == 3)
						{
							//���ʂ̃x�N�g��
							vecLine.x = g_Object[nCnt].point[0].x - g_Object[nCnt].point[nCnt1].x;
							vecLine.z = g_Object[nCnt].point[0].z - g_Object[nCnt].point[nCnt1].z;
						}
						else
						{
							//���ʂ̃x�N�g��
							vecLine.x = g_Object[nCnt].point[nCnt1 + 1].x - g_Object[nCnt].point[nCnt1].x;
							vecLine.z = g_Object[nCnt].point[nCnt1 + 1].z - g_Object[nCnt].point[nCnt1].z;
						}

						//�@���x�N�g���v�Z
						vecNor.x = vecLine.z;
						vecNor.y = 0.0f;
						vecNor.z = -vecLine.x;

						//�@���x�N�g���P�ʉ�
						D3DXVec3Normalize(&vecNor, &vecNor);

						//�ړ��ʑ��
						move->x = 0.0f;
						move->z = 0.0f;

						if (jump == false)
						{
							*rot = atan2f(vecNor.x, vecNor.z);
						}

						//�v���C���[�̈ʒu�ړ�
						pos->x = g_Object[nCnt].point[nCnt1].x + (vecLine.x * fRate) + vecNor.x + (fRefLength * sinf(atan2f(vecNor.x, vecNor.z)));
						pos->z = g_Object[nCnt].point[nCnt1].z + (vecLine.z * fRate) + vecNor.z + (fRefLength * cosf(atan2f(vecNor.x, vecNor.z)));

						break;
					}
					
				}

				//�p�ɓ������Ă���
				if (fLengthPlayer < fRefLength)
				{
					if (pos->y < g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && pos->y > g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y - 40.0f)
					{
						if (jump == false)
						{
							//�v���C���[�̊p�x
							fRef = atan2f(pos->x - g_Object[nCnt].point[nCnt1].x, pos->z - g_Object[nCnt].point[nCnt1].z);

							float fMoveSize;

							fMoveSize = hypotf(move->x, move->z);

							//�v���C���[�̈ʒu�ړ�
							pos->x = g_Object[nCnt].point[nCnt1].x + (sinf(fRef) * (fRefLength + 0.1f));
							pos->z = g_Object[nCnt].point[nCnt1].z + (cosf(fRef) * (fRefLength + 0.1f));

							//�@���x�N�g���v�Z
							vecNor.x = pos->x - g_Object[nCnt].point[nCnt1].x;
							vecNor.y = 0.0f;
							vecNor.z = pos->z - g_Object[nCnt].point[nCnt1].z;

							//�@���x�N�g���P�ʉ�
							D3DXVec3Normalize(&vecNor, &vecNor);

							if (OuterProduct(g_Object[nCnt].point[nCnt1], *posOld, *pos) < 0.0f)
							{
								vecNor.x = -vecNor.z;
								vecNor.y = 0.0f;
								vecNor.z = vecNor.x;
							}
							else
							{
								vecNor.x = vecNor.z;
								vecNor.y = 0.0f;
								vecNor.z = -vecNor.x;
							}

							//�@���x�N�g���P�ʉ�
							D3DXVec3Normalize(&vecNor, &vecNor);

							move->x = vecNor.x * fMoveSize;
							move->z = vecNor.z * fMoveSize;

							break;
						}
					}
				}
			}

			if (nInnerCnt == 4 && pos->y <= g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && posOld->y >= g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y && pos->y >= g_Object[nCnt].pos.y + g_Object[nCnt].vtxMin.y)
			{
				pos->y = g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y;
				posOld->y = g_Object[nCnt].pos.y + g_Object[nCnt].vtxMax.y;
				move->y = 0.0f;

				if (pPlayer->motionState != PLAYERMOTION_JUMP)
				{
					pPlayer->bJump = false;
				}
			}

			nInnerCnt = 0;
		}
	}

	return bReflector;
}

//==================================
//���̈ړ��ʏ���
//==================================
void WindMove(int nCntRock)
{
	Rock *pRock = GetRock();
	int nNumBar = 0;

	//�V�䗠�̊�Ɉړ��ʂ�^����
	for (int nCntObject = 0; nCntObject < MAX_ROCK; nCntObject++)
	{
		if (g_Object[nCntObject].nType == OBJECTTYPE_LEVER_BAR)
		{
			g_Object[nCntObject].WindMove.x = (g_Object[nCntObject].rot.y - g_Object[nCntObject].rotBase.y) * -MOVE_WINDLEVER;

			if (pRock[nCntRock].bUse == true && pRock[nCntRock].pos.y >= GetCeilingPos().y)
			{//�V���荂���ꍇ
				if (nNumBar == 0)
				{
					pRock[nCntRock].move.x += g_Object[nCntObject].WindMove.x * 1.0f;
				}
			}

			nNumBar++;
		}
	}
}

//==================================================
//�I�u�W�F�N�g��rot�̏C������
//==================================================
void RotNormalize(D3DXVECTOR3 *pRot)
{
	if (pRot->y > D3DX_PI)
	{
		pRot->y -= D3DX_PI * 2;
	}
	else if (pRot->y < -D3DX_PI)
	{
		pRot->y += D3DX_PI * 2;
	}
}

//========================================
//�{�^���̓���
//========================================
void ButtonMove(int nCntObject)
{
	if (g_buttonState == BUTONSTATE_FORWARD)
	{
		//���̈ʒu�ɖ߂낤�Ƃ���
		g_Object[nCntObject].pos.z += (g_Object[nCntObject].posBlock.z - g_Object[nCntObject].pos.z) * 0.1f;

		//���ȏ�őҋ@��ԂɂȂ�
		if ((g_Object[nCntObject].posBlock.z - g_Object[nCntObject].pos.z) < -3)
		{
			g_buttonState = BUTONSTATE_WAIT;
			g_bButtonSound = false;
		}
	}
}

//========================================
//�{�^���̓���
//========================================
void ButtonPoleMove(int nCntObject)
{
	if (g_buttonStatePole == BUTONSTATE_FORWARD)
	{
		//���̈ʒu�ɖ߂낤�Ƃ���
		g_Object[nCntObject].pos.y += (g_Object[nCntObject].posBlock.y - g_Object[nCntObject].pos.y) * 0.1f;

		//���ȏ�őҋ@��ԂɂȂ�
		if ((g_Object[nCntObject].posBlock.y - g_Object[nCntObject].pos.y) < -3.0f)
		{
			g_buttonStatePole = BUTONSTATE_WAIT;
			g_bButtonSound = false;
		}
	}
}

//==================================================
//�I�u�W�F�N�g�̃f�o�b�O�\��
//==================================================
void DebugObject(void)
{
	RECT rect = { 0,80,SCREEN_WIDTH,SCREEN_HEIGHT };	//�ꏊ
	char aStr[2][256];					//��
	LPD3DXFONT Font = GetFont();		//���C���̂��

	//������ɑ��
	sprintf(&aStr[0][0], "�I�u�W�F�N�g�̐��F%d", GetUseObject());
	sprintf(&aStr[1][0], "�X�g�b�p�[�󋵁F%d", (int)g_bOpenRock);

	for (int nCntDebug = 0; nCntDebug < 2; nCntDebug++)
	{
		Font->DrawText(NULL, &aStr[nCntDebug][0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		rect.top += 20;
	}
}

//==================================================
//�_�ƒ����̋��������߂�֐�
//==================================================
float LengthPointToLine(D3DXVECTOR3 point, D3DXVECTOR3 linePoint1, D3DXVECTOR3 linePoint2)
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
//�I�t�Z�b�g�̈ʒu���X�V���鏈��
//==================================================
void UpdatePosOffset(int nCntObject)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Object[nCntObject].mtxWorldCollision);
	
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Object[nCntObject].posCollision.x, g_Object[nCntObject].posCollision.y, g_Object[nCntObject].posCollision.z);
	D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorldCollision, &g_Object[nCntObject].mtxWorldCollision, &mtxTrans);

	//�}�g���b�N�X���������킹��
	D3DXMatrixMultiply(&g_Object[nCntObject].mtxWorldCollision, &g_Object[nCntObject].mtxWorldCollision, &g_Object[nCntObject].mtxWorld);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Object[nCntObject].mtxWorldCollision);
}

//==================================================
//���Ђ̈ړ�����
//==================================================
void UpdateRockFrag(int nCntObject)
{
	//�ړ��ʂ�pos�ɉ��Z
	g_Object[nCntObject].move.y -= ROCKFRAG_GLAVITY;
	g_Object[nCntObject].pos += g_Object[nCntObject].move;
	g_Object[nCntObject].fScale -= ROCKFRAG_SCALE_SPEED;
	
	if (g_Object[nCntObject].pos.y + (g_Object[nCntObject].vtxMin.y * g_Object[nCntObject].fScale) < 0.0f)
	{//�n�ʂŔ���
		g_Object[nCntObject].move.x *= FIELD_REFLECT;
		g_Object[nCntObject].move.y *= -(FIELD_REFLECT + 0.1f);
		g_Object[nCntObject].move.z *= FIELD_REFLECT;
	}

	//if (g_Object[nCntObject].pos.x + (g_Object[nCntObject].vtxMax.x * g_Object[nCntObject].fScale) >= 1500.0f ||
	//	g_Object[nCntObject].pos.x + (g_Object[nCntObject].vtxMin.x * g_Object[nCntObject].fScale) <= -1500.0f)
	//{//x�ǂŔ���
	//	g_Object[nCntObject].move.x *= -1;
	//}
	//else if (g_Object[nCntObject].pos.z + (g_Object[nCntObject].vtxMax.z * g_Object[nCntObject].fScale) >= 1500.0f ||
	//	g_Object[nCntObject].pos.z + (g_Object[nCntObject].vtxMin.z * g_Object[nCntObject].fScale) <= -1500.0f)
	//{//z�ǂŔ���
	//	g_Object[nCntObject].move.z *= -1;
	//}

	if (g_Object[nCntObject].fScale <= 0)
	{//�T�C�Y�ō폜
		DeleteObject(nCntObject);
	}
}

//========================================
//��������ꂽ���Z�b�g
//========================================
void SetBreakObj(D3DXVECTOR3 pos , int nNum)
{
	SetParticle(pos, PARTICLETYPE_SMOG);
	SetParticle(pos, PARTICLETYPE_SPARK);

	for (int nCntFrag = 0; nCntFrag < nNum; nCntFrag++)
	{
		SetObject(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBJECTTYPE_ROCK_FRAG);
	}
}

//========================================
//�����L���O�ł̃I�u�W�F�N�g�̍X�V
//========================================
void UpdateObjectRanking(float moveX)
{
	float fRand = 0.0f;	//�����_�����ʕێ�
	int nObjectType = OBJECTTYPE_SKULL;	//�����_�����ʕێ�
	int nTorchCntr = 0;	//���ݏ����J�E���g
	D3DXVECTOR3 Setrot;	//�ݒu���镨�̌���
	D3DXVECTOR3 TorchPos[3];	//�����L���O�̏����ʒu
	D3DXVECTOR3 TorchVtxmtx[3];	//�����L���O�̏����T�C�Y

	//���擾
	Model *pModelObject = GetModelobject();

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (g_Object[nCntObject].bUse == true)
		{
			//�ړ�����
			g_Object[nCntObject].posOld = g_Object[nCntObject].pos;
			g_Object[nCntObject].move.x = moveX;
			g_Object[nCntObject].pos.x += g_Object[nCntObject].move.x;

			if (g_Object[nCntObject].pos.x <= -1000.0f)
			{//����or���[�v
				switch (g_Object[nCntObject].nType)
				{
				case OBJECTTYPE_TORCH:
					g_Object[nCntObject].pos.x = 2000.0f;
					break;
				default:
					DeleteObject(nCntObject);
					break;
				}
			}

			//�e�̈ʒu�ݒ�
			D3DXVECTOR3 rotShadow = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			rotShadow.y = g_Object[nCntObject].rot.y;

			SetPositionShadow(g_Object[nCntObject].nIdxshadow,
				D3DXVECTOR3(g_Object[nCntObject].pos.x, 0.1f, g_Object[nCntObject].pos.z),
				rotShadow,
				1.0f);

			if (g_Object[nCntObject].nType == OBJECTTYPE_TORCH)
			{
				//�r���{�[�h�̈ʒu�ݒ菈��
				SetPositionBillboard(g_Object[nCntObject].nIdxBillboard,
					D3DXVECTOR3(g_Object[nCntObject].pos.x, g_Object[nCntObject].pos.y + 80.0f, g_Object[nCntObject].pos.z));

				TorchVtxmtx[nTorchCntr] = g_Object[nCntObject].vtxMax;
				TorchPos[nTorchCntr] = g_Object[nCntObject].pos;
				nTorchCntr++;
			}
		}
	}

	if (g_nRoopSetTime >= SETOBJ_ROOP && rand() % 5 == 0)
	{//�����Ń����_���Z�b�g
		fRand = (float)(rand() % 1000 / 10.0f);		// %�ȍ~��100�ɂȂ�悤��

		if (fRand <= SKULL_PER)
		{//���W��
			nObjectType = OBJECTTYPE_SKULL;
		}
		else if (fRand <= SKULLMT_PER)
		{//���W���̎R
			nObjectType = OBJECTTYPE_SKULLMT;
		}
		else if (fRand <= STATUE_1_PER)
		{//���H����
			nObjectType = OBJECTTYPE_STATUE_1;
		}
		else if (fRand <= STATUE_3_PER)
		{//���l�T���X����
			nObjectType = OBJECTTYPE_STATUE_3;
		}
		else if (fRand <= STATUE_4_PER)
		{//������
			nObjectType = OBJECTTYPE_STATUE_4;
		}
		else if (fRand <= STATUE_5_PER)
		{//����������
			nObjectType = OBJECTTYPE_STATUE_5;
		}
		else if (fRand <= STATUE_6_PER)
		{//���h������
			nObjectType = OBJECTTYPE_STATUE_6;
		}
		else if (fRand <= STATUE_7_PER)
		{//������
			nObjectType = OBJECTTYPE_STATUE_7;
		}
		else if (fRand <= STATUE_8_PER)
		{//������
			nObjectType = OBJECTTYPE_STATUE_8;
		}
		else if (fRand <= TREASUREBOX_PER)
		{//��
			nObjectType = OBJECTTYPE_TREASUREBOX;
		}
		else if (fRand <= STATUE_100_PER)
		{//����L����
			nObjectType = OBJECTTYPE_STATUE_100;
		}
		else if (fRand <= STATUE_101_PER)
		{//�W���b�N�I�[����
			nObjectType = OBJECTTYPE_STATUE_101;
		}
		else
		{//��
			nObjectType = OBJECTTYPE_SWORD;
		}


		switch (nObjectType)
		{
		case OBJECTTYPE_STATUE_1:
		case OBJECTTYPE_STATUE_3:
		case OBJECTTYPE_STATUE_4:
		case OBJECTTYPE_STATUE_5:
		case OBJECTTYPE_STATUE_6:
		case OBJECTTYPE_STATUE_7:
		case OBJECTTYPE_STATUE_8:
		case OBJECTTYPE_STATUE_100:
		case OBJECTTYPE_STATUE_101:
			Setrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			break;
		default:
			Setrot = D3DXVECTOR3(0.0f, (float)(rand() % 629 - 314) / 100.0f, 0.0f);
			break;
		}

		if ((TorchPos[0].x + TorchVtxmtx[0].x <= RANKING_SETOBJ.x - pModelObject[nObjectType].vtxMax.x ||
			 TorchPos[0].x - TorchVtxmtx[0].x >= RANKING_SETOBJ.x + pModelObject[nObjectType].vtxMax.x) &&
			(TorchPos[1].x + TorchVtxmtx[1].x <= RANKING_SETOBJ.x - pModelObject[nObjectType].vtxMax.x ||
			 TorchPos[1].x - TorchVtxmtx[1].x >= RANKING_SETOBJ.x + pModelObject[nObjectType].vtxMax.x) &&
			(TorchPos[2].x + TorchVtxmtx[2].x <= RANKING_SETOBJ.x - pModelObject[nObjectType].vtxMax.x ||
			 TorchPos[2].x - TorchVtxmtx[2].x >= RANKING_SETOBJ.x + pModelObject[nObjectType].vtxMax.x))
		{//�����ɂ��Ԃ��Ă��Ȃ�������

			SetObject(RANKING_SETOBJ,
					Setrot,
					nObjectType);

			g_nRoopSetTime = 0;
		}
	}

	g_nRoopSetTime++;
}