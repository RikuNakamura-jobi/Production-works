//============================================
//
// ブロックヘッダー[block.h]
// Author：小原立暉
//
//============================================
#ifndef _BLOCK_H_					// このマクロ定義がされていなかったら
#define _BLOCK_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"

//--------------------------------------------
// クラス(ブロッククラス)
//--------------------------------------------
class CBlock : public CModel
{
public:			// 誰でもアクセスできる

	// 列挙型定義(種類)
	enum TYPE
	{
		TYPE_TREE = 0,		// 木
		TYPE_MAX			// この列挙型の総数
	};

	CBlock();				// コンストラクタ
	~CBlock();				// デストラクタ

	// リスト構造関係
	void SetPrev(CBlock* pPrev);	// 前のポインタの設定処理
	void SetNext(CBlock* pNext);	// 後のポインタの設定処理
	CBlock* GetPrev(void) const;	// 前のポインタの設定処理
	CBlock* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);		// 情報の設定処理

	// 静的メンバ関数
	static CBlock* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const TYPE type);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ変数
	TYPE m_type;		// 種類

	// リスト構造関係
	CBlock* m_pPrev;	// 前へのポインタ
	CBlock* m_pNext;	// 次へのポインタ
};

#endif