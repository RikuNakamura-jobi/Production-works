//=====================================
//
// bulletヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
#include "objectX.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class COrbit;
class CLockon;
class CBullet : public CObjectX
{
public:				//外部からアクセス可能

	typedef enum
	{
		TYPE_NORMAL = 0,		//通常
		TYPE_EFFECT,
		TYPE_MAX
	}TYPE;

	//コンストラクタ・デストラクタ
	CBullet(int nPriority);				//デフォルト
	~CBullet();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	//静的メンバ関数
	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nWave, int nSpawn, float fWidth, float fHeight, int nLife, TYPE type);

	static HRESULT Load(void);
	static void Unload(void);

	static void color(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	void Reflect(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, D3DXVECTOR3 posWall1, D3DXVECTOR3 posWall2);
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *rot);

	//メンバ変数
	int m_nLife;
	int m_nWave;
	int m_nSpawn;
	float m_pow;
	MODELX m_modelTemp;
	COrbit *m_orbit;
	TYPE m_type;

	//静的メンバ変数
	static MODELX m_model;
	static D3DXCOLOR m_colorOrbit;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_BULLET_H_