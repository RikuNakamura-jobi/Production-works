//=====================================
//
// player�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "objectX.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//���O��Ԑ錾---------------------
namespace PlayerSpeed
{
	const float SPEED_DASH = -10.0f;			//X�����̃G���A��
	const float SPEED_BOOST = -40.0f;			//Y�����̃G���A��
	const float SPEED_TURN = -0.6f;				//Y�����̃G���A��
	const float SPEED_WALK = -2.0f;				//Y�����̃G���A��
	const float SPEED_MAX = -30.0f;				//Y�����̃G���A��
	const float SPEED_EFFECT = -8.0f;			//Y�����̃G���A��
	const float SPEED_EFFECT_BOOST = -20.0f;	//Y�����̃G���A��
	const float SPEED_BOOSTON = -1.0f;			//Y�����̃G���A��
	const float SPEED_DECELERATION = 0.2f;		//Y�����̃G���A��
	const float SPEED_JUMP = 25.0f;				//Y�����̃G���A��
}
//�N���X��`---------------------------
class COrbit;
class CModel;
class CMotion;
class CHook;
class CDeliveryarrow;
class CPlayer : public CObject
{
public:				//�O������A�N�Z�X�\

	enum MOTION
	{
		MOTION_NORMAL = 0,			//�ҋ@���[�V����
		MOTION_MOVE,				//�ҋ@���[�V����
		MOTION_DASH,				//�ړ����[�V����
		MOTION_TURN,				//�ړ����[�V����
		MOTION_WALL,				//�A�N�V�������[�V����
		MOTION_HOOK,				//�A�N�V�������[�V����
		MOTION_AIR,					//�A�N�V�������[�V����
		MOTION_DROP,				//�A�N�V�������[�V����
		MOTION_ACTION,				//�A�N�V�������[�V����
		MOTION_MAX
	};

	enum STATE
	{
		STATE_NORMAL = 0,		//�ʏ�
		STATE_KICK,
		STATE_HIT,
		STATE_DAMAGE,
		STATE_MAX
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CPlayer(int nPriority);				//�f�t�H���g
	~CPlayer();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	//�ʒu
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//�O�̈ʒu
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	//�ړ���
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//�p�x
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//�ړ���
	void SetSpeedDest(float SpeedDest) { m_SpeedDest = SpeedDest; }
	float GetSpeedDest(void) { return m_SpeedDest; }

	void SetSpeed(float Speed) { m_Speed = Speed; }
	float GetSpeed(void) { return m_Speed; }

	int GetEnegy(void) { return m_nEnergy; }
	bool GetDash(void) { return m_bDash; }
	bool GetAir(void) { return m_bAir; }
	bool GetWall(void) { return m_bWall; }

	bool GetControl(void) { return m_bControl; }
	void SetControl(bool bCon) { m_bControl = bCon; }

	CModel *GetModel(int nIdx) { return m_apModel[nIdx]; }

	//�ÓI�����o�֐�
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);
	void SetRot(D3DXVECTOR3 *rot);
	void Control(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlPad(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlMove(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlMotion(D3DXVECTOR3 move);

	static void ReadFilePlayer(void);

	//�����o�ϐ�
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_rotMove;
	D3DXVECTOR3 m_vecWall;
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	CModel *m_apModel[32];
	CMotion *m_pMotion;
	STATE m_state;
	COrbit *m_orbit[4];
	CDeliveryarrow *m_Arrow;
	CHook *m_Hook;

	bool m_bSave;
	bool m_bAir;
	bool m_bShot;
	bool m_bDash;
	bool m_bWall;
	bool m_bTurn;
	bool m_bBoost;
	bool m_bControl;
	int m_nEasterTimer;
	int m_nShotTimer;
	int m_nCombo;
	int m_nEnergy;
	int m_nPalTimer;
	int m_nHookSoundTimer;
	int m_nWindCounter;
	float m_Speed;
	float m_SpeedDest;
	float m_fHue;

	//�ÓI�����o�ϐ� 
	static int m_nNumModel;
	static CModel *m_apModelOrigin[32];
	static CMotion *m_pMotionOrigin;

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_PLAYER_H_