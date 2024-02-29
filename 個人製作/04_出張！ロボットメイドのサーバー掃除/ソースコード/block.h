//=====================================
//
// blockヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectX.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CBlock : public CObjectX
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CBlock(int nPriority);				//デフォルト
	~CBlock();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	//静的メンバ関数
	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	MODELX m_modelTemp;

	//静的メンバ変数
	static MODELX m_model;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_BLOCK_H_