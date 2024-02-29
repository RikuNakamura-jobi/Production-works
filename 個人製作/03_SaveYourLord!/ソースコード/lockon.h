//=====================================
//
// lockonヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _LOCKON_H_
#define _LOCKON_H_

#include "main.h"
#include "objectbillboard.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CLockon : public CObjectBillboard
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CLockon(int nPriority);				//デフォルト
	~CLockon();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetWidthDef(float fWidth) { m_fWidthDef = fWidth; }
	void SetHeightDef(float fHeight) { m_fHeightDef = fHeight; }

	bool GetLock(void) { return m_bLock; }

	//静的メンバ関数
	static CLockon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	void target(void);

	//メンバ変数
	float m_fWidthDef;
	float m_fHeightDef;
	bool m_bLock;

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_LOCKON_H_