//===========================================
//
// 旗取得UIヘッダー[acquire_UI.h]
// Author 小原立暉
//
//===========================================
#ifndef _ACQUIRE_UI_H_
#define _ACQUIRE_UI_H_

//-------------------------------------------
// インクルードファイル
//-------------------------------------------
#include "object.h"

//-------------------------------------------
// 前方宣言
//-------------------------------------------
class CBillboard;		// ビルボード

//-------------------------------------------
// クラス定義(旗取得UI)
//-------------------------------------------
class CAcquireUI : public CObject
{
public:		// 誰でもアクセスできる

	// 列挙型定義(ポリゴンの種類)
	enum TYPE
	{
		TYPE_GAGE = 0,		// ゲージ
		TYPE_METER,			// メーター
		TYPE_MAX			// この列挙型の総数
	};

	CAcquireUI();					// コンストラクタ
	~CAcquireUI();			// デストラクタ

	// メンバ関数
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理

	void SetData(const D3DXVECTOR3 pos);				// 情報の設定処理

	// セット・ゲット関係
	CBillboard* GetPolygon(const TYPE type);			// ポリゴンの情報の取得処理

	// 静的メンバ関数
	static CAcquireUI* Create(const D3DXVECTOR3 pos);	// 生成処理

private:		// 自分だけアクセスできる

	// メンバ関数

	// メンバ変数
	CBillboard* m_apUI[TYPE_MAX];		// ビルボードの情報
};

#endif