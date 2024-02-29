//========================================  
//
//  ビルボード処理(billboard.h)
//  Author : 佐久間優香 / 金崎朋弥
//
//========================================  
#ifndef _BILLBOARD_H_ //このマクロ定義がされていなかったら
#define _BILLBOARD_H_ //2重インクルード防止のマクロを定義する

//==========================================
//  列挙型の定義
//==========================================
//ビルボードの種類
typedef enum
{
	BILLBOARDTYPE_ICON = 0,
	BILLBOARDTYPE_FIRE,
	BILLBOARDTYPE_ALERT,
	BILLBOARDTYPE_GUIDE,
	BILLBOARDTYPE_MAX
}BILLBOARDTYPE;

//==========================================
//  構造体の定義
//==========================================
//ビルボードの構造体
typedef struct
{
	D3DXVECTOR3 pos; //位置
	bool bUse; //使用しているかどうか
	int nType; //種類
	int nCnterAnim; //アニメーションカウンター
	int nPatternAnim; //アニメーションパターン
	float fMoveDist;	//移動距離
	bool bMoveState;	//移動状態
	D3DXMATRIX mtxWorld; //ワールドマトリックス
}Billboard;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
int SetBillboard(int nType, D3DXVECTOR3 pos);
void SetPositionBillboard(int nIdxBillboard, D3DXVECTOR3 pos);
Billboard *GetBillboard(void);

#endif
