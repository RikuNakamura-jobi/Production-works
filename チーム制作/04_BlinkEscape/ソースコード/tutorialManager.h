//============================================================
//
//	チュートリアルマネージャーヘッダー [tutorialManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _TUTORIAL_MANAGER_H_
#define _TUTORIAL_MANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	マクロ定義
//************************************************************
#define MAX_TUTORIAL		(2)		//チュートリアルテクスチャの数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;	// オブジェクト2Dクラス

//************************************************************
//	クラス定義
//************************************************************
// チュートリアルマネージャークラス
class CTutorialManager
{
public:
	// 説明列挙
	enum EXPLAIN
	{
		EXPLAIN_NORMAL = 0,	// ボタンテクスチャ
		EXPLAIN_FIRST,		// 説明テクスチャ1
		EXPLAIN_SECOND,		// 説明テクスチャ2
		EXPLAIN_MAX			// この列挙型の総数
	};

	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,	// 何もしない状態
		STATE_NORMAL,	// 通常状態
		STATE_MAX		// この列挙型の総数
	};

	// コンストラクタ
	CTutorialManager();

	// デストラクタ
	~CTutorialManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CTutorialManager *Create(void);	// 生成
	static HRESULT Release(CTutorialManager *&prTutorialManager);	// 破棄

private:
	void UpdateStep(void);		//チュートリアルステップ更新処理

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_apExplain[MAX_TUTORIAL];	// 説明表示の情報
	STATE m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nCounterExplain;	// 説明管理カウンター
};

#endif	// _TUTORIAL_MANAGER_H_
