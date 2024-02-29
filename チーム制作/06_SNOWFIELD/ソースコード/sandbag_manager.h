//============================================
//
// 土嚢マネージャーヘッダー[sandbag_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _SANDBAG_MANAGER_H_		// このマクロ定義がされていなかったら
#define _SANDBAG_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CSandbag;		// ブロック

//--------------------------------------------
// クラス(サンプルマネージャークラス)
//--------------------------------------------
class CSandbagManager
{
public:				// 誰でもアクセスできる

	CSandbagManager();			// コンストラクタ
	~CSandbagManager();			// デストラクタ

	// メンバ関数
	void Regist(CSandbag* pThis);		// 登録処理
	void Uninit(void);					// 終了処理
	
	CSandbag* GetTop(void);				// 先頭の取得処理
	void Pull(CSandbag* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CSandbagManager* Create(void);	// 生成処理
	static CSandbagManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CSandbag* m_pTop;			// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CSandbagManager* m_pManager;		// マネージャーの変数
};

#endif