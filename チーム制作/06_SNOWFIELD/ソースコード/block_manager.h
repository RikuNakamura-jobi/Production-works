//============================================
//
// ブロックマネージャーヘッダー[block_manager.h]
// Author：小原立暉
//
//============================================
#ifndef _BLOCK_MANAGER_H_		// このマクロ定義がされていなかったら
#define _BLOCK_MANAGER_H_		// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "main.h"

//--------------------------------------------
// 前方宣言
//--------------------------------------------
class CBlock;		// ブロック

//--------------------------------------------
// クラス(サンプルマネージャークラス)
//--------------------------------------------
class CBlockManager
{
public:				// 誰でもアクセスできる

	CBlockManager();			// コンストラクタ
	~CBlockManager();			// デストラクタ

	// メンバ関数
	void Regist(CBlock* pThis);		// 登録処理
	void Uninit(void);					// 終了処理
	
	CBlock* GetTop(void);				// 先頭の取得処理
	void Pull(CBlock* pThis);			// リスト構造の引き抜き処理

	// 静的メンバ関数
	static CBlockManager* Create(void);	// 生成処理
	static CBlockManager* Get(void);		// 取得処理

private:			// 自分だけアクセスできる

	// メンバ変数
	CBlock* m_pTop;			// 先頭のオブジェクト
	int m_nNumAll;			// オブジェクトの総数

	// 静的メンバ変数
	static CBlockManager* m_pManager;		// マネージャーの変数
};

#endif