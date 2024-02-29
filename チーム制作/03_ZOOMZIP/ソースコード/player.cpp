//==========================================================
//
//�v���C���[���� [player.cpp]
//Author:����������
//
//==========================================================
#include "player.h"
#include "input.h"
#include "debugproc.h"
#include "sound.h"
#include "score.h"
#include "enemy.h"
#include "game.h"

//�}�N����`
#define POS_X			(1200.0f)	//�v���C���[�̈ʒuX
#define POS_Y			(360.0f)	//�v���C���[�̈ʒuY
#define ANGLE_UP		(0.75f)		//��̊p�x
#define ANGLE_DOWN		(0.25f)		//���̊p�x
#define MOVE			(0.5f)		//�v���C���[�̈ړ���
#define MOVE_JUMP		(-0.7f)		//�W�����v�̍���
#define COL_UP			(20.0f)					//��̓����蔻��
#define COL_DOWN		(SCREEN_HEIGHT - COL_UP)	//���̓����蔻��
#define COL_L			(100.0f)					//���̓����蔻��
#define COL_R			(SCREEN_WIDTH - COL_L)		//�E�̓����蔻��
#define TEX_DIV			(4)					//�e�N�X�`���̕�����
#define TEX_POSX		(1.0f / TEX_DIV)	//�e�N�X�`�����WX
#define TEX_POSY		(1.0f)				//�e�N�X�`�����WY

//�v���g�^�C�v�錾
void ControlPlayer(void);
void ScreenPlayer(void);
void CollisionHLPlayer(void);
void CollisionLRPlayer(void);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffplayer = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Player g_aPlayer;									//�v���C���[�̏��

//==========================================================
//����������
//==========================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\player000.png",
		&g_pTexturePlayer);

	//�ϐ�������
	g_aPlayer.pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);	//�ʒu
	g_aPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O��̈ʒu
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
	g_aPlayer.moveOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O��̈ړ���
	g_aPlayer.nSwitchLR = 1;		//�v���C���[�̍��E�؂�ւ�
	g_aPlayer.nCntAnim = 0;			//�A�j���[�V�����J�E���^�[
	g_aPlayer.nPatternAnim = 0;		//�A�j���[�V����No.
	g_aPlayer.nCntGravity = 10;		//�d�̓J�E���^�[
	g_aPlayer.fSpeed = 0.0f;		//�ړ���
	g_aPlayer.fJump = 0.0f;			//�W�����v��
	g_aPlayer.fLength = 0.0f;		//�Ίp���̒���
	g_aPlayer.fAngle = 0.0f;		//�Ίp���̊p�x
	g_aPlayer.fWidth = WIDTH_PLAYER;		//��
	g_aPlayer.fHeight = HEIGHT_PLAYER;		//����
	g_aPlayer.bJump = false;		//�W�����v���Ă邩
	g_aPlayer.bUse = true;			//�g�p���Ă邩
	g_aPlayer.bDisp = true;			//�`�悷�邩
	g_aPlayer.state = PLAYER_APPEAR;	//�o����Ԃɂ���

	//�Ίp���̒������Z�o����
	g_aPlayer.fLength = sqrtf(g_aPlayer.fWidth * g_aPlayer.fWidth + g_aPlayer.fHeight * g_aPlayer.fHeight) * 0.5f;

	//�Ίp���̊p�x���Z�o����
	g_aPlayer.fAngle = atan2f(g_aPlayer.fWidth, g_aPlayer.fHeight);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffplayer,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 
	pVtx[1].rhw = 
	pVtx[2].rhw = 
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffplayer->Unlock();

}

//==========================================================
//�I������
//==========================================================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffplayer != NULL)
	{
		g_pVtxBuffplayer->Release();
		g_pVtxBuffplayer = NULL;
	}
}

//==========================================================
//�X�V����
//==========================================================
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	if (g_aPlayer.bUse == true && g_aPlayer.bDisp == true)
	{//�g�p���Ă���

		g_aPlayer.posOld = g_aPlayer.pos;		//���݂̈ʒu
		g_aPlayer.nCntAnim++;		//�A�j���[�V�����J�E���^�[���Z

		//�v���C���[�̑���
		ControlPlayer();

		//�ʒu���X�V
		g_aPlayer.pos.x += g_aPlayer.move.x;
		g_aPlayer.pos.y += g_aPlayer.move.y;

		//�ړ��ʂ��X�V
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * 0.1f;

		//�������X�V
		g_aPlayer.rot.z += g_aPlayer.move.z;

		//�㉺�̓����蔻��
		CollisionHLPlayer();

		//���E�̓����蔻��
		CollisionLRPlayer();

		//�G�Ƃ̓����蔻��
		if (CollisionEnemy(g_aPlayer.pos, g_aPlayer.posOld) == true)
		{
			g_aPlayer.bUse = false;
			SetGameState(GAMESTATE_END, 0);

			PlaySound(SOUND_LABEL_DAMAGE);
		}

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffplayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y - g_aPlayer.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPlayer.pos.x - g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPlayer.pos.x + g_aPlayer.fWidth, g_aPlayer.pos.y + g_aPlayer.fHeight, 0.0f);

		if ((g_aPlayer.nCntAnim % 30) == 0)
		{//��莞�Ԍo��

			if (g_aPlayer.nSwitchLR == 1)
			{
				//�A�j���[�V����No.�ύX
				g_aPlayer.nPatternAnim = (g_aPlayer.nPatternAnim + 1) % TEX_DIV;

				if (g_aPlayer.nPatternAnim == 2)
				{
					g_aPlayer.nPatternAnim -= 2;
				}
			}
			else if (g_aPlayer.nSwitchLR == -1)
			{
				//�A�j���[�V����No.�ύX
				g_aPlayer.nPatternAnim = (g_aPlayer.nPatternAnim + 1) % TEX_DIV;

				if (g_aPlayer.nPatternAnim == 0)
				{
					g_aPlayer.nPatternAnim += 2;
				}
			}

			//�e�N�X�`�����W�̍Đݒ�
			pVtx[0].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY);
			pVtx[1].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX + TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY);
			pVtx[2].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY + TEX_POSY);
			pVtx[3].tex = D3DXVECTOR2((g_aPlayer.nPatternAnim % TEX_DIV) * TEX_POSX + TEX_POSX, (g_aPlayer.nPatternAnim / 1) * TEX_POSY + TEX_POSY);
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffplayer->Unlock();
	}

	PrintDebugProc("�W�����v�F[SPACE] / [A]\n");
}

//==========================================================
//�v���C���[�̑���
//==========================================================
void ControlPlayer(void)
{
	if (GetKeyboardPress(DIK_SPACE) == true || GetGamepadPress(BUTTON_A, 0) == true)
	{//SPACE�L�[��A�{�^���������ꂽ��

		//�T�E���h�̍Đ�
		//PlaySound(SOUND_LABEL_SE_JUMP000);

		g_aPlayer.nCntGravity = 10;

		//��ɏグ��
		g_aPlayer.move.y += MOVE_JUMP;

		g_aPlayer.bJump = true;		//�W�����v���Ă��Ԃɂ���
	}
	else if (g_aPlayer.nCntGravity > 0 && g_aPlayer.bJump == true)
	{//�d�͂̎��Ԍo���ĂȂ�������

		if (g_aPlayer.nCntGravity > 1)
		{
			g_aPlayer.move.y += 0.5f;		//�d�͉��Z

		}
		else
		{
			g_aPlayer.move.y += (g_aPlayer.move.y * -1) + 0.3f;		//�d�͉��Z

		}

		g_aPlayer.nCntGravity--;		//�J�E���^�[���Z

		if (g_aPlayer.nCntGravity <= 0)
		{
			g_aPlayer.bJump = false;		//�W�����v���ĂȂ���Ԃɂ���
		}
	}
	else
	{
		//�d�͏���
		g_aPlayer.move.y += PLAYER_GRAVITY;
	}

	//���E�ɐi��
	g_aPlayer.move.x -= MOVE * g_aPlayer.nSwitchLR * (g_aPlayer.pos.y * 0.002f);
}

//==========================================================
//��ʊO�ɏo�����̏���
//==========================================================
void ScreenPlayer(void)
{
	//��ʊO�ɏo���ꍇ
	if (g_aPlayer.pos.x - WIDTH_PLAYER + 5.0f <= 0)
	{//���ɏo���ꍇ
		g_aPlayer.pos.x = SCREEN_WIDTH - WIDTH_PLAYER;
		g_aPlayer.pos.y = 660.0f;
	}
	else if (g_aPlayer.pos.x + WIDTH_PLAYER - 5.0f >= SCREEN_WIDTH)
	{//�E�ɏo���ꍇ
		g_aPlayer.pos.x = WIDTH_PLAYER;
		g_aPlayer.pos.y = 660.0f;
	}
}

//==========================================================
//�㉺�̓����蔻��
//==========================================================
void CollisionHLPlayer(void)
{
	if ((g_aPlayer.posOld.y - HEIGHT_PLAYER >= COL_UP && g_aPlayer.pos.y - HEIGHT_PLAYER < COL_UP) ||
		(g_aPlayer.posOld.y + HEIGHT_PLAYER <= COL_DOWN && g_aPlayer.pos.y + HEIGHT_PLAYER > COL_DOWN))
	{//�ォ���������Ă���
		g_aPlayer.bUse = false;
		SetGameState(GAMESTATE_END, 0);
	}
}

//==========================================================
//���E�̓����蔻��Ɖ�������
//==========================================================
void CollisionLRPlayer(void)
{
	if ((g_aPlayer.posOld.x - WIDTH_PLAYER >= COL_L && g_aPlayer.pos.x - WIDTH_PLAYER < COL_L) ||
		(g_aPlayer.posOld.x + WIDTH_PLAYER <= COL_R && g_aPlayer.pos.x + WIDTH_PLAYER > COL_R))
	{//�E�����������Ă���

		g_aPlayer.nSwitchLR *= -1;	//���E�؂�ւ�
		AddScore(1);		//�X�R�A���Z

		if (g_aPlayer.nSwitchLR == 1)
		{
			g_aPlayer.nPatternAnim = 0;
		}
		else if (g_aPlayer.nSwitchLR == -1)
		{
			g_aPlayer.nPatternAnim = 2;
		}
	}
}

//==========================================================
//�`�揈��
//==========================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffplayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aPlayer.bUse == true && g_aPlayer.bDisp == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			0,												//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}

//==========================================================
//�v���C���[�̏��
//==========================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}