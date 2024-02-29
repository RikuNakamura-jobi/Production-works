//============================================
//
// ゲームヘッダー[game.h]
// Author：小原立暉
//
//============================================
#ifndef _GAME_H_			//このマクロ定義がされていなかったら
#define _GAME_H_			//2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "scene.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CFlag;			// 旗
class COffRespawn;		// 攻撃側の復活地点
class CCastle;			// 城

//--------------------------------------------
// クラス(ゲームクラス)
//--------------------------------------------
class CGame : public CScene
{
public:						// 誰でもアクセスできる

	// ゲームの進行状態
	enum STATE
	{
		STATE_START = 0,	// 開始状態
		STATE_PLAY,			// プレイ状態
		STATE_TIMEUP,		// タイムアップ状態
		STATE_OFFWIN,		// 攻撃側の勝ち
		STATE_MAX			// この列挙型の総数
	};

	CGame();				// コンストラクタ
	~CGame();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const MODE mode);		// 情報の設定処理

	// 静的メンバ関数(セット・ゲット)
	static void SetState(const STATE state);		// ゲームの進行状態の設定処理
	static STATE GetState(void);					// ゲームの進行状態の取得処理

	static CFlag *GetFlag(void);						// 旗の取得処理
	static COffRespawn *GetRespawn(void);			// リスポーン位置の取得処理
	static CCastle *GetCastle(void);					// 城の取得処理

private:					// 自分だけアクセスできる

	// メンバ関数
	void Transition(void);		// 遷移処理

	// 静的メンバ変数(最初から必要な物を配置しておく)
	static CFlag* m_pFlag;				// 旗の情報
	static COffRespawn* m_pRespawn;		// 攻撃側の復活地点の情報
	static CCastle* m_pCastle;			// 城の情報
	static STATE m_GameState;			// ゲームの進行状態
	static int m_nFinishCount;			// 終了カウント
};

#endif