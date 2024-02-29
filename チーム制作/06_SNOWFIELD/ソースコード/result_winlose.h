//===================================
//
// リザルトヘッダー[result_winlose.h]
// Author 小原立暉
//
//===================================
#ifndef _RESULT_WINLOSE_H_
#define _RESULT_WINLOSE_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CWinLose)
//-----------------------------------
class CWinLose : public CObject2D
{
public:			// 誰でもアクセスできる

	CWinLose();				// コンストラクタ
	~CWinLose();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);					// 情報の設定処理

	// 静的メンバ関数
	static CWinLose* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

};

#endif