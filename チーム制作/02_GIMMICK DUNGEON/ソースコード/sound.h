//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 佐久間優香 / 高山桃也
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM000 = 0,				// タイトル
	SOUND_LABEL_BGM001,					// 非戦闘
	SOUND_LABEL_BGM002,					// 発見
	SOUND_LABEL_BGM003,					// ランキング
	SOUND_LABEL_BGM004,					// リザルト-勝利(仮)
	SOUND_LABEL_BGM005,					// リザルト-敗北(仮)
	SOUND_LABEL_SE_BUTTON_000,			// メニュー項目の切り替え音
	SOUND_LABEL_SE_BUTTON_001,			// 決定音
	SOUND_LABEL_SE_BUTTON_002,			// タイトル画面の決定音
	SOUND_LABEL_SE_BUTTON_003,			// リザルトなどの遷移音
	SOUND_LABEL_SE_BUTTON_004,			// リザルトなどの遷移音
	SOUND_LABEL_SE_STEP_000,			// プレイヤーの足音
	SOUND_LABEL_SE_STEP_001,			// ゴーレムの足音
	SOUND_LABEL_SE_JUMP_000,			// プレイヤーのジャンプ音
	SOUND_LABEL_SE_JUMP_001,			// ゴーレムのジャンプ音
	SOUND_LABEL_SE_ATTACK_000,			// 斧を叩きつける音
	SOUND_LABEL_SE_ATTACK_001,			// 攻撃予告音
	SOUND_LABEL_SE_LAND,				// プレイヤーの着地音
	SOUND_LABEL_SE_ROLL,				// マグストーンが転がる音
	SOUND_LABEL_SE_SHIFT,				// 石をずらす音
	SOUND_LABEL_SE_CRASH,				// マグストーンが砕ける音
	SOUND_LABEL_SE_DAMAGE,				// ダメージ音
	SOUND_LABEL_SE_VICTORY,				// 勝利の音
	SOUND_LABEL_SE_RESULT,				// リザルトの音
	SOUND_LABEL_SE_COLROCK,				// 岩と反射板の反射音
	SOUND_LABEL_SE_ADDSCORE_000,		// スコア加算音
	SOUND_LABEL_SE_ADDSCORE_001,		// スコア加算音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// サウンドのフェード状態
//*****************************************************************************
typedef enum
{
	FADESTATE_SOUND_NONE = 0,	//何もしていない状態
	FADESTATE_SOUND_IN,	//フェードイン状態
	FADESTATE_SOUND_OUT,	//フェードアウト状態
	FADESTATE_SOUND_MAX
}FADESTATE_SOUND;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
void UpdateSound(void);

HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

void SetVolume(SOUND_LABEL label,float fVolume);
void SetFadeSound(SOUND_LABEL LabelNow, SOUND_LABEL LabelNext, float fSpeed);
#endif
