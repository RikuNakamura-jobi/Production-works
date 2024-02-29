//============================================
//
// 城ヘッダー[castle.h]
// Author：小原立暉
//
//============================================
#ifndef _CASTLE_H_					// このマクロ定義がされていなかったら
#define _CASTLE_H_					// 2重インクルード防止のマクロを定義する

//********************************************
// インクルードファイル
//********************************************
#include "model.h"

//--------------------------------------------
// クラス(城クラス)
//--------------------------------------------
class CCastle : public CModel
{
public:			// 誰でもアクセスできる

	CCastle();				// コンストラクタ
	~CCastle();				// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// 情報の設定処理

	// 静的メンバ関数
	static CCastle* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// 生成処理

private:		// 自分だけアクセスできる

};

#endif