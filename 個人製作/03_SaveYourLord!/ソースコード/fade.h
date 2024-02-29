//=====================================
//
// bgヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "object2D.h"
#include "scene.h"

//マクロ定義---------------------------
#define SLIDE_SPEED (0.017f)

//列挙型定義---------------------------

//クラス定義---------------------------
class CFade : public CObject2D
{
public:				//外部からアクセス可能

	//フェードの状態
	typedef enum
	{
		STATE_NONE = 0,			//なにもしてない状態
		STATE_IN,				//フェードイン状態
		STATE_OUT,				//フェードアウト状態
		STATE_MAX
	}STATE;

	//種類
	typedef enum
	{
		TYPE_NONE = 0,			//タイプ無し
		TYPE_SLIDE,				//スライド
		TYPE_CURTAIN,			//カーテン
		TYPE_CURTAIN_L,			//カーテン
		TYPE_CURTAIN_R,			//カーテン
		TYPE_OUT,				//敵
		TYPE_MAX
	}TYPE;

	//コンストラクタ・デストラクタ
	CFade(int nPriority);				//デフォルト
	~CFade();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数
	static CFade *Create(TYPE type, CScene::MODE mode);
	static HRESULT Load(void);
	static void Unload(void);
	static int GetState(void) { return m_state; }

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	TYPE m_type;
	int m_timer;
	CScene::MODE m_modeNext;

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		//テクスチャへのポインタ
	static STATE m_state;
	static const char *CFade::m_apFilename[TYPE_MAX];
};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_BG_H_