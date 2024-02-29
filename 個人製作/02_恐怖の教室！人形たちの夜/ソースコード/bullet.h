//===========================
//
//ダイレクトX.bulletヘッダー
//Author:中村　陸
//
//===========================
#ifndef _BULLET_H_
#define _BULLET_H_

//弾の構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	D3DXCOLOR col;
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	int nLife;
	int nType;
	int nIdxShadow;
	bool bTrue;
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 point, D3DXVECTOR3 rot , D3DXCOLOR col, int nLife, int nType);

#endif // !_BULLET_H_