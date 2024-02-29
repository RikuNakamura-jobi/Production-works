//============================================================
//
//	ステージヘッダー [stage.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _STAGE_H_
#define _STAGE_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "main.h"

//************************************************************
//	クラス定義
//************************************************************
// ステージクラス
class CStage
{
public:
	// 制限列挙
	typedef enum
	{
		LIMIT_BOX = 0,	// 矩形範囲
		LIMIT_CIRCLE,	// 円範囲
		LIMIT_MAX		// この列挙型の総数
	}LIMIT;

	// コンストラクタ
	CStage();

	// デストラクタ
	~CStage();

	// ステージ範囲構造体
	typedef struct
	{
		LIMIT mode;			// 制限モード
		D3DXVECTOR3 center;	// 中心座標
		float fNear;		// 制限位置 (前)
		float fFar;			// 制限位置 (後)
		float fRight;		// 制限位置 (右)
		float fLeft;		// 制限位置 (左)
		float fRadius;		// 制限位置 (半径)
		float fField;		// 制限位置 (地面)
	}StageLimit;

	// メンバ関数
	HRESULT Init(void);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void SetStageLimit(const StageLimit& rLimit);	// ステージ範囲設定
	StageLimit GetStageLimit(void) const;			// ステージ範囲取得
	void LimitPosition(D3DXVECTOR3& rPos, const float fRadius);						// 位置補正
	bool LandPosition(D3DXVECTOR3& rPos, D3DXVECTOR3& rMove, const float fHeight);	// 範囲外着地

	// 静的メンバ関数
	static CStage *Create(void);				// 生成
	static HRESULT Release(CStage *&prStage);	// 破棄

private:
	// 静的メンバ関数
	static void LoadSetup(CStage *pStage);	// セットアップ

	// メンバ変数
	StageLimit m_stageLimit;	// 範囲情報
};

#endif	// _STAGE_H_
