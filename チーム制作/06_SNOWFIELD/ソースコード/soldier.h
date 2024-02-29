//============================================
//
// 兵士ヘッダー[soldier.h]
// Author：小原立暉
//
//============================================
#ifndef _SOLDIER_H_					// このマクロ定義がされていなかったら
#define _SOLDIER_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "character.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CMotion;				// モーション

//--------------------------------------------
// クラス(兵士クラス)
//--------------------------------------------
class CSoldier : public CCharacter
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_AI = 0,		// AI(CPU)
		TYPE_PLAYER,		// プレイヤー
		TYPE_MAX			// この列挙型の総数
	};

	// 列挙型定義(モーションの種類)
	enum MOTIONTYPE
	{
		MOTIONTYPE_NEUTRAL = 0,		// 待機モーション
		MOTIONTYPE_MOVE,			// 移動モーション
		MOTIONTYPE_THROW,			// 投げモーション
		MOTIONTYPE_JUMP,			// ジャンプモーション
		MOTIONTYPE_MAX				// この列挙型の総数
	};

	// 列挙型定義(バトル)
	enum BATTLE
	{
		BATTLE_OFF = 0,		// 攻撃側
		BATTLE_DEF,			// 防御側
		BATTLE_MAX			// この列挙型の総数
	};

	// 列挙型定義(状態)
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_THROW,		// 投げ状態
		STATE_MAX			// この列挙型の総数
	};

	CSoldier();				// コンストラクタ
	CSoldier(CObject::TYPE type, PRIORITY priority = PRIORITY_PLAYER);		// オーバーロードコンストラクタ
	virtual ~CSoldier();	// デストラクタ
	void Box(void);			// コンストラクタの箱

	// リスト構造関係
	void SetPrev(CSoldier* pPrev);	// 前のポインタの設定処理
	void SetNext(CSoldier* pNext);	// 後のポインタの設定処理
	CSoldier* GetPrev(void) const;	// 前のポインタの設定処理
	CSoldier* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	virtual HRESULT Init(void);		// 初期化処理
	virtual void Uninit(void);		// 終了処理
	virtual void Update(void) = 0;	// 更新処理
	virtual void Draw(void);		// 描画処理

	virtual void SetData(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle);	// 情報の設定処理

	// セット・ゲット関係
	void SetMove(const D3DXVECTOR3& move);		// 移動量の設定処理
	D3DXVECTOR3 GetMove(void) const;			// 移動量の取得処理

	void SetRotDest(const D3DXVECTOR3& rot);	// 目標の向きの設定処理
	D3DXVECTOR3 GetRotDest(void) const;			// 目標の向きの取得処理

	TYPE GetType(void) const;					// 種類の取得処理

	void SetState(const STATE state);			// 状態の設定処理
	STATE GetState(void) const;					// 状態の取得処理

	void SetSpeed(float fSpeed);				// 速度の設定処理
	float GetSpeed(void) const;					// 速度の取得処理

	BATTLE GetBattle(void) const;				// 陣営の取得処理

	void SetEnableMove(bool bMove);				// 移動状況の設定処理
	bool IsMove(void) const;					// 移動状況の取得処理

	void SetEnableJump(bool bJump);				// ジャンプ状況の設定処理
	bool IsJump(void) const;					// ジャンプ状況の取得処理

	CMotion* GetMotion(void) const;				// モーションの情報の取得処理

	void Hit();

	// 静的メンバ関数
	static CSoldier* Create(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle);	// 生成処理

protected:		// 自分と派生クラスだけアクセスできる

	// メンバ関数
	void Move(void);				// 移動処理
	void Jump(void);				// ジャンプ処理
	void Throw(void);				// 投げる処理
	void ElevationCollision(void);	// 起伏地面の当たり判定処理
	void MagicWall(void);			// 魔法壁の当たり判定処理
	void StateManager(void);		// 状態マネージャー

private:		// 自分だけアクセスできる

	// メンバ変数
	CMotion* m_pMotion;				// モーションの情報

	D3DXVECTOR3 m_move;				// 移動量
	D3DXVECTOR3 m_rotDest;			// 目的の向き
	TYPE m_type;					// 種類
	BATTLE m_battle;				// 攻守
	STATE m_state;					// 状態
	int m_nSnowCount;				// 雪投げカウント
	float m_fSpeed;					// 速度
	bool m_bMove;					// 移動状況
	bool m_bJump;					// ジャンプ状況

	// リスト構造関係
	CSoldier* m_pPrev;	// 前へのポインタ
	CSoldier* m_pNext;	// 次へのポインタ
};

#endif