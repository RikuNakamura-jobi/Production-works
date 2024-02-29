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

//クラス定義---------------------------
class CModel;
class CMotion;
class CPlayer : public CObject
{
public:				//外部からアクセス可能

	typedef enum
	{
		MOTION_NORMAL = 0,			//待機モーション
		MOTION_SHOT,				//待機モーション
		MOTION_MOVE,				//移動モーション
		MOTION_MOVE_SHOT,			//アクションモーション
		MOTION_MOVE_SHOT_BACK,		//アクションモーション
		MOTION_MOVE_SHOT_R,			//アクションモーション
		MOTION_MOVE_SHOT_L,			//アクションモーション
		MOTION_JUMP,				//ジャンプモーション
		MOTION_JUMP_SHOT,			//着地モーション
		MOTION_KICK,				//空中モーション
		MOTION_MAX
	}MOTION;

	typedef enum
	{
		TYPE_NORMAL = 0,		//通常
		TYPE_SAVEDATA,
		TYPE_MAX
	}TYPE;

	typedef enum
	{
		STATE_NORMAL = 0,		//通常
		STATE_KICK,
		STATE_HIT,
		STATE_DAMAGE,
		STATE_MAX
	}STATE;

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

	void SetSave(bool save) { m_bSave = save; }
	bool GetSave(void) { return m_bSave; }

	void SetSaveType(TYPE save) { m_Type = save; }
	TYPE GetSaveType(void) { return m_Type; }

	int GetEnegy(void) { return m_nEnergy; }

	CModel *GetModel(int nIdx) { return m_apModel[nIdx]; }

	void SetSave(CPlayer *pPlayerSave);
	void GetSave(CPlayer *pPlayerSave);

	//静的メンバ関数
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move);
	void SetRot(D3DXVECTOR3 *rot);
	void Easter(void);
	void Control(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlPad(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth);
	void ControlMotion(D3DXVECTOR3 move);

	static void ReadFilePlayer(void);

	//メンバ変数
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_rotDest;
	D3DXVECTOR3 m_rotMove;
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	CModel *m_apModel[32];
	CMotion *m_pMotion;
	TYPE m_Type;
	STATE m_state;

	bool m_bSave;
	bool m_bAir;
	bool m_bShot;
	int m_nEasterTimer;
	int m_nShotTimer;
	int m_nCombo;
	int m_nEnergy;

	//静的メンバ変数 
	static int m_nNumModel;
	static CModel *m_apModelOrigin[32];
	static CModel *m_apModelSave[32];
	static CMotion *m_pMotionOrigin;
	static CMotion *m_pMotionSave;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_PLAYER_H_