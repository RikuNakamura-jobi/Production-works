//=====================================
//
// objectヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CObjectX : public CObject
{
public:				//外部からアクセス可能

	struct MODELX
	{
		LPDIRECT3DTEXTURE9 *pTexture;
		LPD3DXMESH pMeshModel;
		LPD3DXBUFFER pBuffMatModel;					//頂点情報を格納
		DWORD dwNumMatModel;
		D3DXVECTOR3 vtxMin;
		D3DXVECTOR3 vtxMax;
	};

	//コンストラクタ・デストラクタ
	CObjectX(int nPriority);
	virtual ~CObjectX();

	//メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//まとめて設定
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

	void BindModel(MODELX *model) { m_model = model; }
	MODELX *GetModel(void) { return m_model; }

	//位置
	void SetPos(D3DXVECTOR3 pos){ m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 *GetPosPointa(void) { return &m_pos; }

	//前の位置
	void SetPosOld(D3DXVECTOR3 posOld) { m_posOld = posOld; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }

	//移動量
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

	//角度
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 *GetRotPointa(void) { return &m_rot; }

	//角度
	void SetMtx(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }

	void SetMtxScale(float mtxScale) { m_mtxScale = mtxScale; }

	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	float GetWidth(void) { return m_fWidth; }

	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	float GetHeight(void) { return m_fHeight; }

	//静的メンバ関数
	static CObjectX *Create(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rot;
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	float m_fWidth;
	float m_fHeight;
	float m_mtxScale;

	MODELX *m_model;

	float m_fHue;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_OBJECTX_H_