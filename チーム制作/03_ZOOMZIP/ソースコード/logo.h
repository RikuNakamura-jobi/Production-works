//==========================================================
//
//チームロゴ画面処理 [logo.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _LOGO_H_	//このマクロ定義が定義されていなかったら
#define _LOGO_H_    //2連インクルード防止のマクロを定義する

//==========================================================
//列挙型の定義
//==========================================================
typedef enum
{
	LOGOTYPE_NORMAL = 0,	//ロゴ元
	LOGOTYPE_COPY000,		//ロゴのコピー
	LOGOTYPE_COPY001,		//ロゴのコピー(2)
	LOGOTYPE_COPY002,		//ロゴのコピー(3)
	LOGOTYPE_COPY003,		//ロゴのコピー(4)
	LOGOTYPE_MAX
}LOGOTYPE;

//==========================================================
//プロトタイプ宣言
//==========================================================
void InitLogo(void);
void UninitLogo(void);
void UpdateLogo(void);
void DrawLogo(void);

#endif
