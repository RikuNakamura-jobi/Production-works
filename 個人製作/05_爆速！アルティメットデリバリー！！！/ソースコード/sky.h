//=====================================
//
// bgmultiヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------
#define MAX_SKY (5)						//多重スクロール用背景の最大数
#define SKY_LENGTH (20000.0f)			//多重スクロール用背景の最大数

//列挙型定義---------------------------

//クラス定義---------------------------
class CObject3D;
class CSkybox;
class CSky : public CObject
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CSky(int nPriority);				//デフォルト
	virtual ~CSky();

	//メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void) override;

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	void SetPos(D3DXVECTOR3 pos){ m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }

	//静的メンバ関数
	static CSky *Create(void);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CObject3D *m_apObject3D[MAX_SKY];
	CSkybox *m_SkyBox;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	float m_fWidth;
	float m_fHeight;

	//静的メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_SKY];		//テクスチャへのポインタ
	static const char *m_apFilename[MAX_SKY];			//テクスチャの名前

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_SKY_H_