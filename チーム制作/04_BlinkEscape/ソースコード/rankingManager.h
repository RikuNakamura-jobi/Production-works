//============================================================
//
//	ランキングマネージャーヘッダー [resultManager.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _RANKINGMANAGER_H_
#define _RANKINGMANAGER_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	前方宣言
//************************************************************
#define MAX_RANKKING	(5)			//ランキング数
#define NUM_DIGIT		(8)			//スコアの桁数
#define NUM_RANKING		(MAX_RANKKING * NUM_DIGIT)	// ランキング表示に必要なポリゴン数

//************************************************************
//	前方宣言
//************************************************************
class CObject2D;		// オブジェクト2Dクラス
class CScore;			// スコアオブジェクトクラス

//************************************************************
//	クラス定義
//************************************************************
// ランキングマネージャークラス
class CRankingManager
{
public:
	// テクスチャ列挙
	typedef enum
	{
		TEXTURE_LENGTH = 0,		// 距離テクスチャ
		TEXTURE_SCORE,			// スコア表示テクスチャ
		TEXTURE_MAX				// この列挙型の総数
	}TEXTURE;

	// 状態列挙
	typedef enum
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_FADEIN,		// フェードイン状態
		STATE_RANKING,		// ランキング表示状態
		STATE_SCORE_WAIT,	// スコア表示待機状態
		STATE_SCORE,		// スコア表示状態
		STATE_WAIT,			// 遷移待機状態
		STATE_MAX			// この列挙型の総数
	}STATE;

	// コンストラクタ
	CRankingManager();

	// デストラクタ
	~CRankingManager();

	// メンバ関数
	HRESULT Init(void);		// 初期化
	HRESULT Uninit(void);	// 終了
	void Update(void);		// 更新

	// 静的メンバ関数
	static CRankingManager *Create(void);	// 生成
	static HRESULT Release(CRankingManager *&prRankingManager);	// 破棄

private:
	// メンバ関数
	void UpdateFade(void);		// フェードイン
	void UpdateRanking(void);	// ランキング表示
	void UpdateScore(void);		// スコア表示
	void UpdateBack(void);		// 遷移決定
	void ChangeAlpha(void);		// 透明度変更
	void SkipStaging(void);		// 演出スキップ
	void SetTexRanking(void);	// ランキング表示のテクスチャ設定
	void Load(void);			// ランキングの読み込み処理
	void Set(void);				// ランキングの設定処理
	bool UpdateDrawWait(const int nWait);	// 表示待機

	// 静的メンバ変数
	static const char *mc_apTextureFile[];	// テクスチャ定数

	// メンバ変数
	CObject2D *m_pRanking;	// ランキング背景の情報
	//CObject2D *m_pScoreLogo;	// スコアロゴの情報
	CObject2D *m_pFade;		// フェードの情報
	CScore *m_pScore[MAX_RANKKING];		// スコアの情報
	STATE m_state;			// 状態
	int m_nCounterState;	// 状態管理カウンター
	int m_nCounterAlpha;	// ランキングの透明度変更間隔
	int m_nScore[MAX_RANKKING];			// ランキング
	int m_nRankUpdate;		// ランキング更新番号
	float m_fScale;			// ポリゴン拡大率

};

#endif	// _RESULTMANAGER_H_
