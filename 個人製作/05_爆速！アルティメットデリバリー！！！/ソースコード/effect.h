//=====================================
//
// effectヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "objectbillboard.h"
#include "object2D.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CEffect
{
public:				//外部からアクセス可能
	
	//コンストラクタ・デストラクタ
	CEffect() {};				//デフォルト
	~CEffect() {};

	static HRESULT Load(void);
	static void Unload(void);

	static LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }

private:			//外部からアクセス不可能

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
};

class CEffect3D : public CObjectBillboard
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CEffect3D(int nPriority);				//デフォルト
	~CEffect3D();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight, bool bCenter = true);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	int m_nCounterAnim;											//アニメーションカウンター
	int m_nPatternAnim;											//アニメーションパターンNo.
	int m_nLife;
	int m_nLifeMax;
};

class CEffect2D : public CObject2D
{
public:				//外部からアクセス可能

					//コンストラクタ・デストラクタ
	CEffect2D(int nPriority);				//デフォルト
	~CEffect2D();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CEffect2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight, bool bCenter = true);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

					//メンバ関数

					//メンバ変数
	int m_nCounterAnim;											//アニメーションカウンター
	int m_nPatternAnim;											//アニメーションパターンNo.
	int m_nLife;
	int m_nLifeMax;
};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_EFFECT_H_