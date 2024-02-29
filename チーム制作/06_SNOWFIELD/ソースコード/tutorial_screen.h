//===================================
//
// チュートリアルスクリーンヘッダー[tutorial_screen.h]
// Author 小原立暉
//
//===================================
#ifndef _TUTORIAL_SCREEN_H_
#define _TUTORIAL_SCREEN_H_

//***********************************
// インクルードファイル
//***********************************
#include "object2D.h"

//-----------------------------------
// クラス定義(CTutorialScreen)
//-----------------------------------
class CTutorialScreen : public CObject2D
{
public:			// 誰でもアクセスできる

	CTutorialScreen();		// コンストラクタ
	~CTutorialScreen();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(void);				// 情報の設定処理

	// 静的メンバ関数
	static CTutorialScreen* Create(void);	// 生成処理

private:		// 自分だけアクセスできる

};

#endif