//============================================
//
// 土嚢ヘッダー[sandbag.h]
// Author：小原立暉
//
//============================================
#ifndef _SANDBAG_H_					// このマクロ定義がされていなかったら
#define _SANDBAG_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"

//--------------------------------------------
// クラス(土嚢クラス)
//--------------------------------------------
class CSandbag : public CModel
{
public:			// 誰でもアクセスできる

	CSandbag();				// コンストラクタ
	~CSandbag();				// デストラクタ

	// リスト構造関係
	void SetPrev(CSandbag* pPrev);	// 前のポインタの設定処理
	void SetNext(CSandbag* pNext);	// 後のポインタの設定処理
	CSandbag* GetPrev(void) const;	// 前のポインタの設定処理
	CSandbag* GetNext(void) const;	// 次のポインタの設定処理

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 情報の設定処理

	// 静的メンバ関数
	static CSandbag* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);	// 生成処理

private:		// 自分だけアクセスできる

	// リスト構造関係
	CSandbag* m_pPrev;	// 前へのポインタ
	CSandbag* m_pNext;	// 次へのポインタ
};

#endif