//==========================================
//
//  チームロゴ画面(team_logo.h)
//  Author : Kai Takada
//
//==========================================
#ifndef _TEAM_LOGO_H_
#define _TEAM_LOGO_H_
#include "main.h"

//==========================================
//  タイトル状態の列挙型定義
//==========================================
typedef enum
{
	TEAM_LOGOSTATE_IN = 0,
	TEAM_LOGOSTATE_NORMAL,
	TEAM_LOGOSTATE_OUT,
	TEAM_LOGOSTATE_MAX
}TEAM_LOGOSTATE;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitTeam_logo(void);
void UninitTeam_logo(void);
void UpdateTeam_logo(void);
void DrawTeam_logo(void);

#endif
