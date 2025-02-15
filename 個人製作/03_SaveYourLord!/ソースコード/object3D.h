//=====================================
//
// objectヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CObject3D : public CObject
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CObject3D(int nPriority);
	virtual ~CObject3D();

	//メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexturePolygon = pTexture; }

	//まとめて設定
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	void SetPosSize(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void SetPosSizeX(D3DXVECTOR3 pos, D3DXVECTOR3 size);

	//位置
	void SetPos(D3DXVECTOR3 pos){ m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	//前の位置
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	//移動量
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//角度
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	//大きさ
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }

	//色
	void SetCol(D3DXCOLOR col) { m_col = col; }
	D3DXCOLOR GetCol(void) { return m_col; }

	//テクスチャ座標
	D3DXVECTOR2 *GetTexPos(void) { return &m_texPos[0]; }

	//幅
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }

	//高さ
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }

	//高さ
	float GetColHeight(D3DXVECTOR3 pos, D3DXVECTOR3 posOld, D3DXVECTOR3 *move);

	LPDIRECT3DVERTEXBUFFER9 *GetBuff(void) { return &m_pVtxBuff; }
	void SetBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff =  pVtxBuff; }

	LPDIRECT3DTEXTURE9 *GetTex(void) { return &m_pTexturePolygon; }
	void SetTex(LPDIRECT3DTEXTURE9 pTexture) { m_pTexturePolygon = pTexture; }

	//静的メンバ関数
	static CObject3D *Create(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					//頂点情報を格納
	LPDIRECT3DTEXTURE9 m_pTexturePolygon;				//テクスチャへのポインタ
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	D3DXCOLOR m_col;
	D3DXVECTOR2 m_texPos[4];
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス

	float m_fWidth;
	float m_fHeight;
	float m_fLength;
	float m_fAngle;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_OBJECT3D_H_