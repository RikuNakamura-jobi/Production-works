//=====================================
//
// bgヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"
#include "object2D.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CBg : public CObject2D
{
public:				//外部からアクセス可能

	enum TEXTURE
	{
		TEXTURE_RESULT = 0,				//なにもしてない状態
		TEXTURE_TUTORIAL_KEY0,			//フェードイン状態
		TEXTURE_TUTORIAL_KEY1,			//フェードイン状態
		TEXTURE_TUTORIAL_KEY2,			//フェードイン状態
		TEXTURE_TUTORIAL_PAD0,			//フェードイン状態
		TEXTURE_TUTORIAL_PAD1,			//フェードイン状態
		TEXTURE_TUTORIAL_PAD2,			//フェードイン状態
		TEXTURE_START,					//フェードイン状態
		TEXTURE_READY,					//フェードイン状態
		TEXTURE_GO,						//フェードイン状態
		TEXTURE_SUCCESS,				//フェードイン状態
		TEXTURE_FAILED,					//フェードイン状態
		TEXTURE_TIME,					//フェードイン状態
		TEXTURE_SCORE,					//フェードイン状態
		TEXTURE_COMBO,					//フェードイン状態
		TEXTURE_MAX
	};

	//コンストラクタ・デストラクタ
	CBg(int nPriority);				//デフォルト
	~CBg();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	TEXTURE GetTextureType(void) { return m_textureType; }

	void SetTextureType(TEXTURE texture);

	//静的メンバ関数
	static CBg *Create(TEXTURE texture);
	static CBg *CreateMin(D3DXVECTOR3 pos, float width, float height,TEXTURE texture);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	TEXTURE m_textureType;

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[TEXTURE_MAX];		//テクスチャへのポインタ
	static const char *m_apFilename[TEXTURE_MAX];			//テクスチャの名前

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_BG_H_