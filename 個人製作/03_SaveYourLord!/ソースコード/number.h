//=====================================
//
// numberヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "object2D.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CNumber : public CObject2D
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CNumber(int nPriority);				//デフォルト
	~CNumber();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetNumber(void) { return m_nNumber; }
	void SetNumber(int nNumber) { m_nNumber = nNumber; }

	//静的メンバ関数
	static CNumber *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	int m_nNumber;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_NUMBER_H_