//=====================================
//
// baselifeヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _BASELIFE_H_
#define _BASELIFE_H_

#include "main.h"
#include "object2D.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CBaselife : public CObject2D
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CBaselife(int nPriority);				//デフォルト
	~CBaselife();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetLife(void) { return m_nLife; }
	void SetLife(int nLife) { m_nLife = nLife; }
	void AddLife(int nLife) { m_nLife += nLife; }

	//静的メンバ関数
	static CBaselife *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	int m_nLife;

	//メンバ変数

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_BASELIFE_H_