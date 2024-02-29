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

	typedef enum
	{
		TEXTURE_RESULT = 0,		//なにもしてない状態
		TEXTURE_TUTORIAL_KEY_MOVE,		//フェードイン状態
		TEXTURE_TUTORIAL_KEY_SHOT,		//フェードイン状態
		TEXTURE_TUTORIAL_KEY_SAVE,		//フェードイン状態
		TEXTURE_TUTORIAL_PAD_MOVE,		//フェードイン状態
		TEXTURE_TUTORIAL_PAD_SHOT,		//フェードイン状態
		TEXTURE_TUTORIAL_PAD_SAVE,		//フェードイン状態
		TEXTURE_TUTORIAL_GAME,			//フェードイン状態
		TEXTURE_SUCCESS,				//フェードイン状態
		TEXTURE_FAILED,					//フェードイン状態
		TEXTURE_START,					//フェードイン状態
		TEXTURE_MAX
	}TEXTURE;

	//コンストラクタ・デストラクタ
	CBg(int nPriority);				//デフォルト
	~CBg();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	TEXTURE GetTextureType(void) { return m_textureType; }

	//静的メンバ関数
	static CBg *Create(TEXTURE texture);
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