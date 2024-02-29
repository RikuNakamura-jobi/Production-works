//=====================================
//
// bgヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "object3D.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CField : public CObject3D
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CField(int nPriority);				//デフォルト
	~CField();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_FIELD_H_