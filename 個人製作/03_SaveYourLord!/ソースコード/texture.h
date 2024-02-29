//=============================================================================
//
// 
// Author :中村陸
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//マクロ定義---------------------------
#define MAX_TEXTURE (256)

//列挙型定義---------------------------

//サウンドクラス
class CTexture
{
public:

	CTexture();
	~CTexture();

	//メンバ関数
	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);

	//静的メンバ関数

private:

	//メンバ関数

	//メンバ変数
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];

	//静的メンバ変数
	static int m_nNumAll;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_TEXTURE_H_
