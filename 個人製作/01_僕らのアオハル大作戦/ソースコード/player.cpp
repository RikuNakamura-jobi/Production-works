//===========================
//
//�_�C���N�gX.player�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "player.h"
#include "input.h"
#include "block.h"
#include "effect.h"
#include "particle.h"
#include "goal.h"
#include "game.h"
#include "coin.h"
#include "enemy.h"
#include "stage.h"
#include "start.h"
#include "boss.h"
#include "boost.h"
#include "life.h"
#include "sound.h"

//�v���g�^�C�v�錾

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTexturePlayer[PLAYERTYPE_MAX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;					//���_�����i�[
Player g_player;													//�v���C���[�\����
int g_nCountDeath;
int g_nCountLife;
int g_nCountBoost;
int g_nCountLR;
float g_fLengthPlayer;												//�Ίp���̒���
float g_fAnglePlayer;												//�Ίp���̊p�x
float g_fLengthPlayerMove = 0.0f;									//�Ίp���̈ړ���
bool g_bGoal;

//�e�N�X�`���t�@�C����
const char *c_apFilenamePlayer[PLAYERTYPE_MAX] =
{
	"data\\TEXTURE\\player000.png",
	"data\\TEXTURE\\player001.png",
	"data\\TEXTURE\\player002.png",
	"data\\TEXTURE\\player003.png"
};

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < PLAYERTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenamePlayer[nCnt],
			&g_apTexturePlayer[nCnt]);
	}

	g_player.nCounterAnim = DEFAULT_NUMBER;							//�J�E���^�[������������
	g_player.nPatternAnim = DEFAULT_NUMBER;							//�p�^�[��No.������������
	g_player.pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);				//�ʒu������������
	g_player.posOld = D3DXVECTOR3(100.0f, 100.0f, 0.0f);			//�ʒu������������
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ړ��ʂ�����������
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//����������������
	g_player.fJumpHigh = 18.0f;
	g_player.fJumpG = 0.8f;
	g_player.nLife = 1000;
	g_player.nCounterState = 0;
	g_player.bDisp = true;											//�g�p���Ă��Ȃ���Ԃɂ���
	g_player.bJump = false;											//�g�p���Ă��Ȃ���Ԃɂ���
	g_player.bWall = false;											//�g�p���Ă��Ȃ���Ԃɂ���
	g_player.pBlock = NULL;
	g_player.playertype = PLAYERTYPE_0;
	g_player.state = PLAYERSTATE_WAIT;
	g_bGoal = false;
	g_nCountDeath = 0;
	g_nCountLife = 0;
	g_nCountBoost = 150;
	g_nCountLR = 0;

	//�Ίp���̒������Z�o����
	g_fLengthPlayer = sqrtf((PLAYER_X * PLAYER_X) + (PLAYER_Y * PLAYER_Y)) * 0.5f;

	//�Ίp���̊p�x���Z�o����
	g_fAnglePlayer = atan2f(PLAYER_X, PLAYER_Y);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	
	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 0.25f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	SetLife(g_player.nLife);
	SetBoost(g_nCountBoost);
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < PLAYERTYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_apTexturePlayer[nCnt] != NULL)
		{
			g_apTexturePlayer[nCnt]->Release();
			g_apTexturePlayer[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//===========================
//�v���C���[�̍X�V����
//===========================
void UpdatePlayer(void)
{
	D3DXVECTOR3 playerpos;
	g_player.posOld = g_player.pos;
	int nNumCoin = GetNumCoin();
	int nNumEnemy = GetNumEnemy();
	int nStageType = GetStageType();

	if (g_player.state == PLAYERSTATE_DEATH)
	{
		g_nCountDeath--;
	}

	//�p�^�[��N0.���X�V����
	g_player.nPatternAnim = (g_player.nCounterAnim / 8) % 4;

	if (g_player.state != PLAYERSTATE_DAMAGE && g_player.state != PLAYERSTATE_DEATH && g_player.state != PLAYERSTATE_GOAL && g_player.state != PLAYERSTATE_WAIT)
	{
		g_nCountLife++;

		if (g_nCountLife % 2 == 0)
		{
			g_player.nLife--;
			AddLife(1);
		}
	}

	if (g_player.state != PLAYERSTATE_DAMAGE && g_player.state != PLAYERSTATE_DEATH && g_player.state != PLAYERSTATE_GOAL && g_player.state != PLAYERSTATE_ATTACK && g_player.state != PLAYERSTATE_WAIT)
	{
		if (GetKeyboardTrigger(DIK_1) == true)
		{
			g_player.playertype = PLAYERTYPE_0;
		}
		else if (GetKeyboardTrigger(DIK_2) == true)
		{
			g_player.playertype = PLAYERTYPE_1;
		}
		else if (GetKeyboardTrigger(DIK_3) == true)
		{
			g_player.playertype = PLAYERTYPE_2;
		}
		else if (GetKeyboardTrigger(DIK_4) == true)
		{
			g_player.playertype = PLAYERTYPE_3;
		}

		if (g_player.playertype == PLAYERTYPE_0)
		{
			//�ړ�����
			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
				}

				g_nCountLR = 1;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
				}

				g_nCountLR = 0;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}

			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{
				if (g_player.bJump == false)
				{
					g_player.bJump = true;

					g_player.move.y = g_player.fJumpHigh * -1;
				}
			}

			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_player.nCounterState = 10;
				g_player.state = PLAYERSTATE_ATTACK;

				if (g_nCountLR == 0)
				{
					playerpos.x = g_player.pos.x + 64.0f;
					playerpos.y = g_player.pos.y;
					playerpos.z = g_player.pos.z;

					SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 64.0f, 32.0f, 10);
				}
				else if (g_nCountLR == 1)
				{
					playerpos.x = g_player.pos.x - 64.0f;
					playerpos.y = g_player.pos.y;
					playerpos.z = g_player.pos.z;

					SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 3.14f), 64.0f, 32.0f, 10);
				}

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_ATTACK);
			}
		}
		else if (g_player.playertype == PLAYERTYPE_1)
		{
			//�ړ�����
			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.8f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.8f;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
				}

				g_nCountLR = 1;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.8f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.8f;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
				}

				g_nCountLR = 0;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}

			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{
				if (g_player.bJump == false)
				{
					g_player.bJump = true;

					g_player.move.y = g_player.fJumpHigh * -1 * 0.8f;
				}
			}

			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_player.nCounterState = 10;
				g_player.state = PLAYERSTATE_ATTACK;

				if (g_nCountLR == 0)
				{
					playerpos.x = g_player.pos.x + 96.0f;
					playerpos.y = g_player.pos.y;
					playerpos.z = g_player.pos.z;

					SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 128.0f, 64.0f, 10);
				}
				else if (g_nCountLR == 1)
				{
					playerpos.x = g_player.pos.x - 64.0f;
					playerpos.y = g_player.pos.y;
					playerpos.z = g_player.pos.z;

					SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 3.14f), 128.0f, 64.0f, 10);
				}

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_ATTACK);
			}
		}
		else if (g_player.playertype == PLAYERTYPE_2)
		{
			//�ړ�����
			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 1.5f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 1.5f;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
				}

				g_nCountLR = 1;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 1.5f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 1.5f;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.2f;
				}

				g_nCountLR = 0;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}

			if (GetKeyboardTrigger(DIK_SPACE) == true)
			{
				if (g_player.bJump == false)
				{
					g_player.bJump = true;

					g_player.move.y = g_player.fJumpHigh * -1 * 1.2f;
				}
			}
		}
		else if (g_player.playertype == PLAYERTYPE_3)
		{
			//�ړ�����
			if (GetKeyboardPress(DIK_A) == true)
			{//A�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.5f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.5f;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.05f;
					g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * 0.05f;
				}

				g_nCountLR = 1;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}
			else if (GetKeyboardPress(DIK_D) == true)
			{//D�L�[�������ꂽ��
				if (g_player.bJump == false)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.5f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER * 0.5f;
				}
				else if (g_player.bJump == true)
				{
					g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.05f;
					g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * 0.05f;
				}

				g_nCountLR = 0;

				g_player.nCounterAnim++;				//�J�E���^�[�����Z
			}

			if (GetKeyboardPress(DIK_SPACE) == true && g_nCountBoost > 0)
			{
				g_player.bJump = true;
				g_nCountBoost--;
				AddBoost(1);
				if (g_player.move.y > -4.5f)
				{
					g_player.move.y += g_player.fJumpHigh * -1 * 0.08f;
				}
			}

			if (g_player.bJump == false)
			{
				if (g_nCountBoost < 150)
				{
					g_nCountBoost += 3;
				}
				else if (g_nCountBoost >= 150)
				{
					g_nCountBoost = 150;
				}

				AddBoost(-3);
			}

			if (GetKeyboardTrigger(DIK_RETURN) == true)
			{
				g_player.nCounterState = 10;
				g_player.state = PLAYERSTATE_ATTACK;

				if (g_player.bJump == false)
				{
					if (g_nCountLR == 0)
					{
						playerpos.x = g_player.pos.x + 64.0f;
						playerpos.y = g_player.pos.y - 64.0f;
						playerpos.z = g_player.pos.z;

						SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.57f), 64.0f, 32.0f, 10);
					}
					else if (g_nCountLR == 1)
					{
						playerpos.x = g_player.pos.x - 64.0f;
						playerpos.y = g_player.pos.y - 64.0f;
						playerpos.z = g_player.pos.z;

						SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.57f), 64.0f, 32.0f, 10);
					}
				}
				else if (g_player.bJump == true)
				{
					playerpos.x = g_player.pos.x;
					playerpos.y = g_player.pos.y + 64.0f;
					playerpos.z = g_player.pos.z;

					SetBoss(playerpos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.57f), 64.0f, 32.0f, 10);
				}

				//�T�E���h�̍Đ�
				PlaySound(SOUND_LABEL_SE_ATTACK);
			}
		}

		if (GetKeyboardPress(DIK_A) == true)
		{//A�L�[�������ꂽ��
			g_player.state = PLAYERSTATE_MOVE;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[�������ꂽ��
			g_player.state = PLAYERSTATE_MOVE;
		}

		if (GetKeyboardRelease(DIK_D) == true)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		if (GetKeyboardRelease(DIK_A) == true)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
	}

	g_player.move.y += g_player.fJumpG;

	//�ʒu�X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.rot.z += g_player.move.z;
	g_fLengthPlayer += g_fLengthPlayerMove;

	if (g_player.pBlock != NULL && g_player.bJump == false)
	{
		g_player.pos.x += g_player.pBlock->pos.x - g_player.pBlock->posOld.x;
		g_player.pos.y += g_player.pBlock->pos.y - g_player.pBlock->posOld.y;
	}

	if (g_player.bJump == true)
	{
		g_player.move.x += (0 - g_player.move.x) * 0.01f;
		g_player.move.z += (0 - g_player.move.z) * INE_ROT_PLAYER;
		g_fLengthPlayerMove += (0 - g_fLengthPlayerMove) * INE_SIZE_PLAYER;
	}
	else
	{
		g_player.move.x += (0 - g_player.move.x) * INE_MOVE_PLAYER;
		g_player.move.z += (0 - g_player.move.z) * INE_ROT_PLAYER;
		g_fLengthPlayerMove += (0 - g_fLengthPlayerMove) * INE_SIZE_PLAYER;
	}

	if (g_player.nLife <= 0 && g_player.state != PLAYERSTATE_DEATH)
	{
		g_player.state = PLAYERSTATE_DEATH;
		g_nCountDeath = 90;
		g_player.move.x += 0.0f;
		g_player.move.y += -15.0f;
		g_player.rot.z += 1.57f + (-3.14f * g_nCountLR);
	}

	if ((g_player.pos.y > 800.0f || (g_player.state == PLAYERSTATE_DEATH && g_nCountDeath <= 0)) && g_bGoal == false)
	{
		InitLife();
		InitBlock();
		InitCoin();
		InitPlayer();
		InitEnemy();
		InitGoal();
		InitStart();
		InitStage();

		g_player.state = PLAYERSTATE_NORMAL;
	}

	if (g_player.pos.x < -40.0f)
	{
		g_player.pos.x = 1315.0f;
	}
	if (g_player.pos.x > 1320.0f)
	{
		g_player.pos.x = -35.0f;
	}

	if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.pBlock) == true)
	{
		g_player.bJump = false;
	}
	else
	{
		g_player.bJump = true;
	}

	if (CollisionBlockSide(&g_player.pos, &g_player.posOld, &g_player.move, &g_player.pBlock) == true)
	{
		g_player.bWall = true;
	}
	else
	{
		g_player.bWall = false;
	}
	
	if (nStageType == 2)
	{
		if (nNumEnemy == 0 && g_bGoal == false)
		{
			g_bGoal = true;
			SetGameState(GAMESTATE_END, 60);
		}
	}
	if (nStageType == 4)
	{
		if (nNumEnemy == 0 && g_bGoal == false)
		{
			g_bGoal = true;
			SetGameState(GAMESTATE_END, 60);
		}
	}
	else if (nStageType == 3)
	{
		if (nNumCoin == 0 && g_bGoal == false)
		{
			g_bGoal = true;
			SetGameState(GAMESTATE_END, 60);
		}
	}
	else
	{
		if (CollisionGoal(&g_player.pos) == true && nNumCoin == 0 && nNumEnemy == 0 && g_bGoal == false)
		{
			g_bGoal = true;
			SetGameState(GAMESTATE_END, 60);
		}
	}

	if (g_bGoal == true && g_player.state != PLAYERSTATE_GOAL)
	{
		g_player.state = PLAYERSTATE_GOAL;

		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_GOAL000);
	}

	if (CollisionCoin(&g_player.pos) == true)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_COIN);
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.25f * g_player.nPatternAnim, 0.5f * (g_player.nCounterAnim % 2));
	pVtx[1].tex = D3DXVECTOR2((0.25f * g_player.nPatternAnim) + 0.25f, 0.5f * (g_player.nCounterAnim % 2));
	pVtx[2].tex = D3DXVECTOR2(0.25f * g_player.nPatternAnim, 0.5f * (g_player.nCounterAnim % 2) + 0.5f);
	pVtx[3].tex = D3DXVECTOR2((0.25f * g_player.nPatternAnim) + 0.25f, 0.5f * (g_player.nCounterAnim % 2) + 0.5f);

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.25f);
		break;
	case PLAYERSTATE_WAIT:
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.25f);
		break;
	case PLAYERSTATE_GOAL:
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.25f);
		break;
	case PLAYERSTATE_DEATH:
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.5f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.75f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.75f);
		break;
	case PLAYERSTATE_MOVE:
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR) + (0.125f * g_player.nPatternAnim), 0.25f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR) + (0.125f * g_player.nPatternAnim), 0.25f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR + (0.125f * g_player.nPatternAnim)), 0.5f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR) + (0.125f * g_player.nPatternAnim), 0.5f);
		break;
	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (0.5f * g_nCountLR), 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.25f);

		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;
			g_player.nCounterState = 60;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	case PLAYERSTATE_ATTACK:
		g_player.nCounterState--;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f + (0.5f * g_nCountLR), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f + (0.5f * g_nCountLR), 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.25f + (0.5f * g_nCountLR), 0.25f);

		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0,
		g_pVtxBuffPlayer,
		0,
		sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_apTexturePlayer[g_player.playertype]);

	if (g_player.bDisp == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================
//�v���C���[�̕`�揈��
//===========================
void SetPlayer(D3DXVECTOR3 pPos)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_player.pos = pPos;				//�ʒu������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		{ FVF_VERTEX_2D },
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_player.pos.x + sinf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.y = g_player.pos.y + cosf(g_player.rot.z - D3DX_PI + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.pos.x + sinf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.y = g_player.pos.y + cosf(g_player.rot.z + D3DX_PI - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.pos.x + sinf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.y = g_player.pos.y + cosf(g_player.rot.z - g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.pos.x + sinf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.y = g_player.pos.y + cosf(g_player.rot.z + g_fAnglePlayer) * g_fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[1].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[2].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);
	pVtx[3].col = D3DCOLOR_RGBA(VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR, VERTEX_COLOR);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.25f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 0.25f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//===========================
//�v���C���[�̃q�b�g����
//===========================
void HitPlayer(int nDamage, float move)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	Enemy aENEMY;

	g_player.nLife -= nDamage;

	if (g_player.state != PLAYERSTATE_DAMAGE && g_player.state != PLAYERSTATE_DEATH && g_player.state != PLAYERSTATE_GOAL)
	{
		if (g_player.nLife <= 0 && g_player.state != PLAYERSTATE_DEATH)
		{
			g_player.state = PLAYERSTATE_DEATH;
			g_nCountDeath = 90;
			g_player.move.x += move;
			g_player.move.y += -10.0f;
			g_player.rot.z += 1.57f + (-3.14f * g_nCountLR);

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_HIT001);
		}
		else
		{
			g_player.state = PLAYERSTATE_DAMAGE;
			g_player.nCounterState = 30;
			g_player.move.x = 0.0f;
			g_player.move.y = 0.0f;
			g_player.move.x += move;
			g_player.move.y += -10.0f;

			VERTEX_2D *pVtx;

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

			g_pVtxBuffPlayer->Unlock();

			//�T�E���h�̍Đ�
			PlaySound(SOUND_LABEL_SE_HIT001);
		}
	}
}

//===========================
//�v���C���[��Ԃ̎擾����
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}

//===========================
//wave��Ԏ擾����
//===========================
void SetPlayerState(PLAYERSTATE state, int nCounter)
{
	g_player.state = state;
	g_player.nCounterState = nCounter;
}