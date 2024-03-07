//=====================================
//
// playerヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "objectX.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//名前空間宣言---------------------
namespace PlayerSpeed
{
	const float SPEED_DASH = -10.0f;			//X方向のエリア数
	const float SPEED_BOOST = -40.0f;			//Y方向のエリア数
	const float SPEED_TURN = -0.6f;				//Y方向のエリア数
	const float SPEED_WALK = -2.0f;				//Y方向のエリア数
	const float SPEED_MAX = -30.0f;				//Y方向のエリア数
	const float SPEED_EFFECT = -8.0f;			//Y方向のエリア数
	const float SPEED_EFFECT_BOOST = -20.0f;	//Y方向のエリア数
	const float SPEED_BOOSTON = -1.0f;			//Y方向のエリア数
	const float SPEED_DECELERATION = 0.2f;		//Y方向のエリア数
	const float SPEED_JUMP = 25.0f;				//Y方向のエリア数
}
//クラス定義---------------------------
class COrbit;
class CModel;
class CMotion;
class CHook;
class CDeliveryarrow;
class CPlayer : public CObject
{
public:				//外部からアクセス可能

	enum MOTION
	{
		MOTION_NORMAL = 0,			//待機モーション
		MOTION_MOVE,				//待機モーション
		MOTION_DASH,				//移動モーション
		MOTION_TURN,				//移動モーション
		MOTION_WALL,				//アクションモーション
		MOTION_HOOK,				//アクションモーション
		MOTION_AIR,					//アクションモーション
		MOTION_DROP,				//アクションモーション
		MOTION_ACTION,				//アクションモーション
		MOTION_MAX
	};

	enum STATE
	{
		STATE_NORMAL = 0,		//通常
		STATE_KICK,
		STATE_HIT,
		STATE_DAMAGE,
		STATE_MAX
	};

	//コンストラクタ・デストラクタ
	CPlayer(int nPriority);				//デフォルト
	~CPlayer();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	//位置
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//前の位置
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	//移動量
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//角度
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//移動量
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

	//静的メンバ関数
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);
	void SetRot(D3DXVECTOR3 *rot);
	void Control(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlPad(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlMove(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlMotion(D3DXVECTOR3 move);

	static void ReadFilePlayer(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_rotMove;
	D3DXVECTOR3 m_vecWall;
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
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

	//静的メンバ変数 
	static int m_nNumModel;
	static CModel *m_apModelOrigin[32];
	static CMotion *m_pMotionOrigin;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_PLAYER_H_