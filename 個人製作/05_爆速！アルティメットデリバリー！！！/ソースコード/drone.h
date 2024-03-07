//=====================================
//
// droneヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _DRONE_H_
#define _DRONE_H_

#include "main.h"
#include "objectX.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CDrone : public CObjectX
{
public:				//外部からアクセス可能

	enum TYPE
	{
		TYPE_NONE = 0,			//なにもしてない状態
		TYPE_MAX
	};

	//コンストラクタ・デストラクタ
	CDrone(int nPriority);				//デフォルト
	~CDrone();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	//静的メンバ関数
	static CDrone *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, CDrone::TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	MODELX m_modelTemp;

	float m_fHue;
	float m_fHueOrg;

	//静的メンバ変数
	static MODELX m_model[TYPE_MAX];
	static const char *m_apFilename[TYPE_MAX];			//テクスチャの名前

	TYPE m_type;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_DRONE_H_