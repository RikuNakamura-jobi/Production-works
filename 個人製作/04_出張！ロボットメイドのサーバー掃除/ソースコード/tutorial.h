//=====================================
//
// tutorialヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "objectbillboard.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CTutorialBillboard : public CObjectBillboard
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CTutorialBillboard(int nPriority);				//デフォルト
	~CTutorialBillboard();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CTutorialBillboard *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[5];		//テクスチャへのポインタ
	static const char *m_apFilename[5];				//テクスチャの名前

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_LOCKON_H_