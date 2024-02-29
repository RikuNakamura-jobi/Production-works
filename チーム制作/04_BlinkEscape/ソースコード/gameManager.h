//============================================================
//
//	ゲームマネージャーヘッダー [gameManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "retentionManager.h"

//==========================================
//  前方宣言
//==========================================
class CPlayer;
class CMap;
class CObjectGauge2D;
class CObject2D;

//************************************************************
//	クラス定義
//************************************************************
// ゲームマネージャークラス
class CGameManager
{
public:
	// 状態列挙
	typedef enum
	{
		STATE_NORMAL = 0,	// 通常状態
		STATE_START,		// 開始状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// コンストラクタ
	CGameManager();

	// デストラクタ
	~CGameManager();

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void TransitionResult(const CRetentionManager::RESULT result);	// リザルト遷移

	// 静的メンバ関数
	static CGameManager *Create(void);	// 生成
	static HRESULT Release(CGameManager *&prGameManager);	// 破棄
	static CPlayer* GetPlayer(void) { return m_pPlayer; } //プレイヤーの取得
	static CMap* GetMap(void) { return m_pMap; } //世界の取得
	static STATE GetState(void) { return m_state; }

private:

	//メンバ変数
	int m_curtainInterbal; //状態

	//静的メンバ変数
	static CPlayer* m_pPlayer; //プレイヤーのポインタ
	static CMap* m_pMap; //世界のポインタ
	static CObject2D* m_pObject2D[2]; //ゲージのポインタ
	static STATE m_state; //状態

};

#endif	// _GAMEMANAGER_H_
