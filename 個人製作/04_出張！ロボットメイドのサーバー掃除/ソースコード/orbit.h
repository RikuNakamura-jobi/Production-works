//=====================================
//
// orbitヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _ORBIT_H_
#define _ORBIT_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------
#define MAX_EDGE	(100)	//辺の最大数
#define NUM_OFFSET	(2)	//オフセットの数

//列挙型定義---------------------------

//クラス定義---------------------------
class COrbit : public CObject
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	COrbit(int nPriority);
	~COrbit();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	static HRESULT Load(void);
	static void Unload(void);

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void SetPos(D3DXVECTOR3 pos) { return; }
	D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void SetMove(D3DXVECTOR3 move) { return; }
	D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void SetRot(D3DXVECTOR3 rot) { return; }
	D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }

	void SetEdge(int nNumEdge) { m_nNumEdge = nNumEdge; }
	void SetCol(D3DXCOLOR col) { m_col = col; }
	void SetposOffset(D3DXVECTOR3 posOffset, int nOffset) { m_posOffset[nOffset] = posOffset; }
	void SetPosPoint(D3DXVECTOR3 posPoint, int nNumEdge, int nOffset) { m_aPosPoint[nNumEdge][nOffset] = posPoint; }
	void SetmtxWorldOffset(D3DXMATRIX mtxWorldOffset, int nOffset) { m_mtxWorldOffset[nOffset] = mtxWorldOffset; }
	void SetMtx(D3DXMATRIX mtx) { m_mtxWorld = mtx; }

	//設定処理
	void SetPositionOffset(D3DXMATRIX mtxWorld);

	//静的メンバ関数
	static COrbit *Create(D3DXMATRIX mtxWorld, D3DXVECTOR3 posOffset1, D3DXVECTOR3 posOffset2, D3DXCOLOR col, int nNumEdge);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	void UpdatePolygon(void);

	//メンバ変数
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					//頂点情報を格納
	static LPDIRECT3DTEXTURE9 m_pTexture;						//テクスチャへのポインタ
	D3DXMATRIX m_mtxWorld;								//マトリックス
	D3DXVECTOR3 m_posOffset[NUM_OFFSET];				//オフセット位置
	D3DXVECTOR3 m_aPosPoint[MAX_EDGE][NUM_OFFSET];		//頂点座標保存用
	D3DXCOLOR m_col;									//色
	D3DXCOLOR m_aColPoint[MAX_EDGE][NUM_OFFSET];		//頂点カラー保存用
	D3DXMATRIX m_mtxWorldOffset[NUM_OFFSET];			//オフセットのマトリックス
	int m_nNumEdge;										//辺の数

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_ORBIT_H_