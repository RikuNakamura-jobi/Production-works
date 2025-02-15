//=======================================
//
//リザルト処理[result.cpp]
//Author Ibuki Okusada
//
//=======================================
#include "result.h"   //作成したresult.hをインクルードする
#include "input.h"
#include "fade.h"
#include "sound.h"

//=======================================
//マクロ定義
//=======================================

//=======================================
//列挙型の定義
//=======================================

//=======================================
//構造体の定義
//=======================================

//=======================================
//プロトタイプ宣言
//=======================================

//=======================================
//グローバル変数宣言
//=======================================

//=======================================
//テクスチャファイル名
//=======================================

//========================================
//リザルトの初期化処理
//========================================
void InitResult(void)
{

	PlaySound(SOUND_LABEL_BGMCLEAR);
}

//========================================
//リザルトの終了処理
//========================================
void UninitResult(void)
{
	StopSound();
}

//========================================
//リザルトの更新処理
//========================================
void UpdateResult(void)
{
#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//遷移キーが押された
		SetFade(MODE_RANKING);
		PlaySound(SOUND_LABEL_SEBUTTON);
	}
#endif
}

//========================================
//リザルトの描画処理
//========================================
void DrawResult(void)
{

}

//=======================================
//状態の設定
//=======================================
void SetResultMode(RESULTSTATE Result)
{
	
}

//=======================================
//リザルトの状態確認
//=======================================
void ResultJudge(void)
{
	
}

//=======================================
//ゲームクリア画面の情報生成
//=======================================
void CreateResultClear(void)
{
	
}
