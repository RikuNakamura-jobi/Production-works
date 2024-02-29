//============================================================
//
//	ランキング画面ヘッダー [sceneRanking.h]
//	Author：佐藤根詩音
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _SCENE_RANKING_H_
#define _SCENE_RANKING_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"
#include "scene.h"

//************************************************************
//	前方宣言
//************************************************************
class CRankingManager;	// ランキングマネージャークラス
class CBg;				// 背景クラス

//************************************************************
//	クラス定義
//************************************************************
// ランキング画面クラス
class CSceneRanking : public CScene
{
public:
	// コンストラクタ
	CSceneRanking(const MODE mode);

	// デストラクタ
	~CSceneRanking();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	HRESULT Uninit(void) override;	// 終了
	void Update(void) override;		// 更新
	void Draw(void) override;		// 描画

	// 静的メンバ関数
	static CRankingManager *GetRankingManager(void);	// ランキングマネージャー取得
	static CBg *GetBg(void);	// 背景取得

private:
	// 静的メンバ変数
	static CRankingManager *m_pRankingManager;	// ランキングマネージャー
	static CBg *m_pBg;	// 背景オブジェクト
};

#endif	// _SCENE_RESULT_H_
