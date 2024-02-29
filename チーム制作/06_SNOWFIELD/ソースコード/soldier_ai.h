//============================================
//
// 兵士AIヘッダー[soldier_ai.h]
// Author：小原立暉
//
//============================================
#ifndef _SOLDIER_AI_H_					// このマクロ定義がされていなかったら
#define _SOLDIER_AI_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "soldier.h"

//--------------------------------------------
// クラス(兵士プレイヤークラス)
//--------------------------------------------
class CSoldierAI : public CSoldier
{
public:			// 誰でもアクセスできる

	// 列挙型定義(戦闘AI)
	enum BATTLEAI
	{
		BATTLEAI_ATTACK = 0,	// 攻撃的
		BATTLEAI_GUARD,			// 守備的
		BATTLEAI_MAX			// この列挙型の総数
	};

	// 列挙型定義(戦闘AI)
	enum SITUATION
	{
		SITUATION_ASSAULT = 0,	// 突撃
		SITUATION_SHOOT,		// 攻撃
		SITUATION_GUARD,		// 守備
		SITUATION_ESCAPE,		// 逃走
		SITUATION_WAIT,			// 待機
		SITUATION_MAX			// この列挙型の総数
	};

	CSoldierAI();			// コンストラクタ
	~CSoldierAI();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const TYPE type, const BATTLE battle);		// 情報の設定処理

private:		// 自分だけアクセスできる

	// メンバ関数
	void AI(void);	//AIのまとめ

	// 移動など
	void AIMove(void);
	void AIShoot(void);

	// 行動選択
	void AIOffense(void);
	void AIOffenseAttack(void);
	void AIOffenseGuard(void);
	void AIDefense(void);
	void AIDefenseAttack(void);
	void AIDefenseGuard(void);

	// メンバ変数
	BATTLEAI m_BattleAI;
	SITUATION m_Situation;
	D3DXVECTOR3 m_AimPos;
	int m_nCountJudge;
	int m_nNumNearPlayer;
	int m_nNumNearEnemy;
	float m_fTargetRot;
};

#endif