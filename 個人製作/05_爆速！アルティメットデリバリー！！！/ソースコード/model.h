//=====================================
//
// modelヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CModel
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CModel();
	~CModel();

	//メンバ関数
	HRESULT Init(char *pFilename);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//位置
	void SetPos(D3DXVECTOR3 pos){ m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	void SetPosDef(D3DXVECTOR3 posDef) { m_posDef = posDef; }
	D3DXVECTOR3 GetPosDef(void) { return m_posDef; }

	//角度
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetRotDef(D3DXVECTOR3 rotDef) { m_rotDef = rotDef; }
	D3DXVECTOR3 GetRotDef(void) { return m_rotDef; }

	//親
	void SetParent(CModel *pModel) { m_pParent = pModel; }

	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	LPD3DXBUFFER GetBuffMat(void) { return m_pBuffMatModel; }

	//静的メンバ関数
	static CModel *Create(char *pFilename);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	LPDIRECT3DTEXTURE9 *m_pTexture;
	LPD3DXMESH m_pMeshModel;
	LPD3DXBUFFER m_pBuffMatModel;					//頂点情報を格納
	DWORD m_dwNumMatModel;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posDef;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_rotDef;
	D3DXMATRIX m_mtxWorld;				//ワールドマトリックス

	CModel *m_pParent;

	float m_fHue;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_MODEL_H_