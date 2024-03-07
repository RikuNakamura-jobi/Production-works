//=====================================
//
// particleヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "objectbillboard.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CParticle : public CObjectBillboard
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CParticle(int nPriority);				//デフォルト
	~CParticle();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, int nLifePtcl, int nNumEffect, int nSpeed, float fWidth, float fHeight);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	int m_nNumEffect;
	int m_nLife;
	int m_nLifePtcl;
	int m_nSpeed;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_PARTICLE_H_