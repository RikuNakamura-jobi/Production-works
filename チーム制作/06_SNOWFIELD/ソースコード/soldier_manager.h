//============================================
//
// 兵士マネージャーヘッダー[soldier_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _SOLDIER_MANAGER_H_		// このマクロ定義がされていなかったら
#define _SOLDIER_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CSoldier;		// 雪玉

//--------------------------------------------
// クラス(サンプルマネージャークラス)
//--------------------------------------------
class CSoldierManager
{
public:				// 誰でもアクセスできる

	CSoldierManager();			// コンストラクタ
	~CSoldierManager();			// デストラクタ

	// メンバ関数
	void Regist(CSoldier* pThis);		// 登録処理
	void Uninit(void);					// 終了処理
	
	CSoldier* GetTop(void);				// 先頭の取得処理
	void Pull(CSoldier* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CSoldierManager* Create(void);	// 生成処理
	static CSoldierManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CSoldier* m_pTop;	// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CSoldierManager* m_pManager;		// マネージャーの変数
};

#endif