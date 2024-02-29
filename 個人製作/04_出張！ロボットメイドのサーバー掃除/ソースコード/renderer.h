//=====================================
//
// rendererヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CRenderer
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CRenderer();				//デフォルト
	~CRenderer();

	//メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	LPDIRECT3D9 m_pD3D;							//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;				//Direct3Dデバイスへのポインタ

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_RENDERER_H_