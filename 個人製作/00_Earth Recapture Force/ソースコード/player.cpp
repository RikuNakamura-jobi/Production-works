//===========================
//
//�_�C���N�gX.player�t�@�C��
//Author:�����@��
//
//===========================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "explosion.h"
#include "sound.h"
#include "game.h"
#include "life.h"
#include "remain.h"
#include "effect.h"
#include "particle.h"

//�v���g�^�C�v�錾
void BuffPlayer(void);
void ShotPlayer(void);
void ConPlayerPad(void);

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;					//���_�����i�[
Player g_player;													//�v���C���[�\����
float g_fLengthPlayer;												//�Ίp���̒���
float g_fAnglePlayer;												//�Ίp���̊p�x
float g_fLengthPlayerMove = 0.0f;									//�Ίp���̈ړ���
int g_CounterMissile;												//�~�T�C���J�E���^�[
int g_nMissileNumber;												//�~�T�C���̐�

//===========================
//�v���C���[�̏���������
//===========================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Player002.png",
		&g_pTexturePlayer);

	g_player.nCounterAnim = DEFAULT_NUMBER;							//�J�E���^�[������������
	g_player.nPatternAnim = DEFAULT_NUMBER;							//�p�^�[��No.������������
	g_player.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);				//�ʒu������������
	g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ړ��ʂ�����������
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//����������������
	g_player.nLife = 5;
	g_player.nRemaining = 3;
	g_player.state = PLAYERSTATE_APPEAR;
	g_player.nCounterState = 60;
	g_player.bDisp = true;											//�g�p���Ă��Ȃ���Ԃɂ���
	g_CounterMissile = 0;
	g_nMissileNumber = 10;

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
	pVtx[0].tex = D3DXVECTOR2(TEXTUER_O_X, TEXTUER_O_Y);
	pVtx[1].tex = D3DXVECTOR2(1.0f, TEXTUER_O_Y);
	pVtx[2].tex = D3DXVECTOR2(TEXTUER_O_X, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//===========================
//�v���C���[�̏I������
//===========================
void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
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
	Enemy aENEMY;
	D3DXVECTOR3 playerpos;
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	int nData = rand() % 21 - 10;

	g_player.nCounterAnim++;				//�J�E���^�[�����Z

	switch (g_player.state)
	{//��Ԕ���
	case PLAYERSTATE_APPEAR:
		g_player.nCounterState--;
		if (g_player.nCounterState % 6 == 0)
		{
			g_player.bDisp = g_player.bDisp ^ 1;
		}
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		break;
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_DAMAGE:
		break;
	case PLAYERSTATE_WAIT:
		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.nCounterState = 60;
			g_player.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);				//�ʒu������������
			g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//�ړ��ʂ�����������
			g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//����������������
			g_player.nLife = 5;
			BuffPlayer();
			InitLife();
			SetLife(5);
			AddRemain(1);
			g_player.bDisp = true;
			g_player.state = PLAYERSTATE_APPEAR;
		}
		return;
		break;
	case PLAYERSTATE_DEATH:
		return;
		break;
	}


	if (GetKeyboardRepeat(DIK_SPACE) == true)
	{//�ˌ�
		ShotPlayer();
	}

	//�~�T�C���֌W
	g_CounterMissile++;

	if (g_CounterMissile % 120 == 0 && g_nMissileNumber < 10)
	{
		g_nMissileNumber++;
	}

	if (g_nMissileNumber > 0)
	{
		if (GetKeyboardTrigger(DIK_B) == true)
		{
			/*g_nMissileNumber -= 2;

			posBullet.x = (g_player.pos.x + sinf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
			posBullet.y = (g_player.pos.y + cosf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
			posBullet.z = g_player.pos.z;

			moveBullet.x = sinf(g_player.rot.z + D3DX_PI - 0.25f) * BULLET_SPEED;
			moveBullet.y = cosf(g_player.rot.z + D3DX_PI - 0.25f) * BULLET_SPEED;
			moveBullet.z = 0.0f;
			SetBullet(posBullet, moveBullet, 100, BULLETTYPE_HOMING, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 0);

			posBullet.x = (g_player.pos.x + sinf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
			posBullet.y = (g_player.pos.y + cosf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
			posBullet.z = g_player.pos.z;

			moveBullet.x = sinf(g_player.rot.z + D3DX_PI) * BULLET_SPEED;
			moveBullet.y = cosf(g_player.rot.z + D3DX_PI) * BULLET_SPEED;
			moveBullet.z = 0.0f;
			SetBullet(posBullet, moveBullet, 100, BULLETTYPE_HOMING, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f), 0);*/
		}
	}

	//�p�^�[��N0.���X�V����
	g_player.nPatternAnim = ((g_player.nCounterAnim + 1) / PERIOD_PLAYER) % FRAME_TEN;

	//�ړ�����
	if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[�������ꂽ��
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_DIAF) * SPEED_PLAYER;
			g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_DIAF) * SPEED_PLAYER;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_DIAB) * SPEED_PLAYER;
			g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_DIAB) * SPEED_PLAYER;
		}
		else
		{
			g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
			g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[�������ꂽ��
		if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_DIAF) * SPEED_PLAYER;
			g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_DIAF) * SPEED_PLAYER;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_DIAB) * SPEED_PLAYER;
			g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_DIAB) * SPEED_PLAYER;
		}
		else
		{
			g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
			g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_SIDE) * SPEED_PLAYER;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[�������ꂽ��
		g_player.move.x += sinf(D3DX_PI * MOVE_ANGLE_F) * SPEED_PLAYER;
		g_player.move.y += cosf(D3DX_PI * MOVE_ANGLE_F) * SPEED_PLAYER;
	}
	else if (GetKeyboardPress(DIK_S) == true && GetKeyboardPress(DIK_A) != true)
	{//S�L�[�������ꂽ��
		g_player.move.x += sinf(-D3DX_PI * MOVE_ANGLE_B) * SPEED_PLAYER;
		g_player.move.y += cosf(-D3DX_PI * MOVE_ANGLE_B) * SPEED_PLAYER;
	}

	ConPlayerPad();

	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		playerpos.x = (g_player.pos.x + sinf(g_player.rot.z + 0.4f) * g_fLengthPlayer * 1.0f);
		playerpos.y = (g_player.pos.y + sinf(g_player.rot.z + 0.6f) * g_fLengthPlayer * 1.0f);
		playerpos.z = g_player.pos.z;
		SetParticle(playerpos, D3DXCOLOR(1.0f, 0.3f, 0.01f, 1.0f), 10.0f, 6, 0, 314, D3DXVECTOR3 (0.0f, 7.0f, 0.0f), 100);

		playerpos.x = (g_player.pos.x + sinf(g_player.rot.z - 0.4f) * g_fLengthPlayer * 1.0f);
		playerpos.y = (g_player.pos.y + sinf(g_player.rot.z + 0.6f) * g_fLengthPlayer * 1.0f);
		playerpos.z = g_player.pos.z;
		SetParticle(playerpos, D3DXCOLOR(1.0f, 0.3f, 0.01f, 1.0f), 10.0f, 6, 0, 314, D3DXVECTOR3 (0.0f, 7.0f, 0.0f), 100);
	}

	BuffPlayer();
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
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_player.bDisp == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//===========================
//���_�o�b�t�@�̒�������
//===========================
void BuffPlayer(void)
{
	//�ʒu�X�V
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.rot.z += g_player.move.z;
	g_fLengthPlayer += g_fLengthPlayerMove;

	g_player.move.x += (0 - g_player.move.x) * INE_MOVE_PLAYER;
	g_player.move.y += (0 - g_player.move.y) * INE_MOVE_PLAYER;
	g_player.move.z += (0 - g_player.move.z) * INE_ROT_PLAYER;
	g_fLengthPlayerMove += (0 - g_fLengthPlayerMove) * INE_SIZE_PLAYER;

	//�ړ��ʍX�V
	if (g_player.pos.x  > PLAYER_WIDTH_R - 32.0f)
	{
		g_player.pos.x = PLAYER_WIDTH_R - 32.0f;
		g_player.move.x = 0.0f;
	}
	if (g_player.pos.x < PLAYER_WIDTH_L + 32.0f)
	{
		g_player.pos.x = PLAYER_WIDTH_L + 32.0f;
		g_player.move.x = 0.0f;
	}
	if (g_player.pos.y < PLAYER_HEIGHT_U + 32.0f)
	{
		g_player.pos.y = PLAYER_HEIGHT_U + 32.0f;
		g_player.move.y = 0.0f;
	}
	if (g_player.pos.y > PLAYER_HEIGHT_D - 32.0f)
	{
		g_player.pos.y = PLAYER_HEIGHT_D - 32.0f;
		g_player.move.y = 0.0f;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYERSTATE_NORMAL:
		break;
	case PLAYERSTATE_DAMAGE:
		g_player.nCounterState--;

		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		break;
	}

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//===========================
//�v���C���[�̎ˌ�����
//===========================
void ShotPlayer(void)
{
	D3DXVECTOR3 posBullet;
	D3DXVECTOR3 moveBullet;
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.7f);

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_SE_SHOT002);

	posBullet.x = (g_player.pos.x + sinf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.y = (g_player.pos.y + cosf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.z = g_player.pos.z;

	moveBullet.x = sinf(g_player.rot.z + D3DX_PI) * BULLET_SPEED;
	moveBullet.y = cosf(g_player.rot.z + D3DX_PI) * BULLET_SPEED;
	moveBullet.z = 0.0f;
	SetBullet(posBullet, moveBullet, 100, BULLETTYPE_PLAYER, col, 0);

	posBullet.x = (g_player.pos.x + sinf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.y = (g_player.pos.y + cosf(g_player.rot.z + 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.z = g_player.pos.z;

	moveBullet.x = sinf(g_player.rot.z + D3DX_PI - 0.25f) * BULLET_SPEED;
	moveBullet.y = cosf(g_player.rot.z + D3DX_PI - 0.25f) * BULLET_SPEED;
	moveBullet.z = 0.0f;
	SetBullet(posBullet, moveBullet, 100, BULLETTYPE_PLAYER, col, 0);

	posBullet.x = (g_player.pos.x + sinf(g_player.rot.z - 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.y = (g_player.pos.y + cosf(g_player.rot.z - 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.z = g_player.pos.z;

	moveBullet.x = sinf(g_player.rot.z + D3DX_PI) * BULLET_SPEED;
	moveBullet.y = cosf(g_player.rot.z + D3DX_PI) * BULLET_SPEED;
	moveBullet.z = 0.0f;
	SetBullet(posBullet, moveBullet, 100, BULLETTYPE_PLAYER, col, 0);

	posBullet.x = (g_player.pos.x + sinf(g_player.rot.z - 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.y = (g_player.pos.y + cosf(g_player.rot.z - 2.5f) * g_fLengthPlayer * 0.4f);
	posBullet.z = g_player.pos.z;

	moveBullet.x = sinf(g_player.rot.z + D3DX_PI + 0.25f) * BULLET_SPEED;
	moveBullet.y = cosf(g_player.rot.z + D3DX_PI + 0.25f) * BULLET_SPEED;
	moveBullet.z = 0.0f;
	SetBullet(posBullet, moveBullet, 100, BULLETTYPE_PLAYER, col, 0);
}

//===========================
//�v���C���[��Ԃ̎擾����
//===========================
Player *GetPlayer(void)
{
	return &g_player;
}

//===========================
//�v���C���[�̃q�b�g����
//===========================
void HitPlayer(int nDamage)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	Enemy aENEMY;

	g_player.nLife -= nDamage;

	if (g_player.nLife > 5)
	{
		g_player.nLife = 5;
	}

	if (g_player.nLife <= 0)
	{
		//�T�E���h�̍Đ�
		PlaySound(SOUND_LABEL_SE_EXPLOSION);

		SetExplosion(g_player.pos, col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(10.0f, 20.0f, 0.0f), col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(-20.0f, 20.0f, 0.0f), col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(20.0f, -10.0f, 0.0f), col, 0);
		SetExplosion(g_player.pos + D3DXVECTOR3(-30.0f, -20.0f, 0.0f), col, 0);

		if (g_player.nRemaining <= 0)
		{
			g_player.state = PLAYERSTATE_DEATH;
			g_player.bDisp = false;
			SetGameState(GAMESTATE_END, 60);
		}
		else
		{
			g_player.nRemaining--;
			g_player.nCounterState = 60;
			g_player.state = PLAYERSTATE_WAIT;
			g_player.bDisp = false;
		}
	}
	else
	{
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;

		VERTEX_2D *pVtx;

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_pVtxBuffPlayer->Unlock();
	}
}

//===========================
//�v���C���[�̉񕜏���
//===========================
void AddPlayer(int nLife, int nRemain)
{
	g_player.nLife += nLife;
	if (g_player.nLife > 5)
	{
		g_player.nLife = 5;
	}

	if (g_player.nRemaining < 3)
	{
		g_player.nRemaining += nRemain;
	}
}

void ConPlayerPad(void)
{
	int nLStickLR = GetPadLStickLRPress();
	int nLStickUD = GetPadLStickUDPress();
	float fLength;
	float fAngle = atan2f((float)nLStickLR, (float)nLStickUD);

	//�Ίp���̒������Z�o����
	fLength = sqrtf(((float)nLStickLR * (float)nLStickLR) + ((float)nLStickUD * (float)nLStickUD)) * 0.5f;

	nLStickUD = nLStickUD * -1;

	if (GetPadButtonRepeat(5) == true)
	{//�ˌ�
		ShotPlayer();
	}

	//�ړ�����
	if (fLength != 0)
	{
		g_player.move.x += sinf(fAngle) * SPEED_PLAYER;
		g_player.move.y += cosf(fAngle) * SPEED_PLAYER;
	}
}
