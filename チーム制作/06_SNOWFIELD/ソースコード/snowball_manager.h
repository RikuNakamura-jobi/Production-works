//============================================
//
// 雪玉マネージャーヘッダー[snowball_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _SNOWBALL_MANAGER_H_		// このマクロ定義がされていなかったら
#define _SNOWBALL_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CSnowBall;		// 雪玉

//--------------------------------------------
// クラス(サンプルマネージャークラス)
//--------------------------------------------
class CSnowBallManager
{
public:				// 誰でもアクセスできる

	CSnowBallManager();			// コンストラクタ
	~CSnowBallManager();			// デストラクタ

	// メンバ関数
	void Regist(CSnowBall* pThis);		// 登録処理
	void Uninit(void);					// 終了処理
	
	CSnowBall* GetTop(void);				// 先頭の取得処理
	void Pull(CSnowBall* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CSnowBallManager* Create(void);	// 生成処理
	static CSnowBallManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CSnowBall* m_pTop;	// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CSnowBallManager* m_pManager;		// マネージャーの変数
};

#endif