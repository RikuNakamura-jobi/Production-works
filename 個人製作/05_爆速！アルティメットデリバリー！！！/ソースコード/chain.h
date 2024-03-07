//=====================================
//
// chainヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _CHAIN_H_
#define _CHAIN_H_

#include "main.h"
#include "objectX.h"
#include <map>

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CModel;
class COrbit;
class CHook : public CObjectX
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CHook(int nPriority);				//デフォルト
	~CHook();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	bool GetBoolShot(void) { return m_bShot; }
	void SetBoolShot(bool Shot) { m_bShot = Shot; }

	bool GetBoolGet(void) { return m_bGet; }
	void SetBoolGet(bool get) { m_bGet = get; }

	bool GetBoolCollision(void) { return m_bCollision; }
	void SetBoolCollision(bool Collision) { m_bCollision = Collision; }

	void ShotHook(D3DXVECTOR3 rot, float speed);

	//静的メンバ関数
	static CHook *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);
	bool CollisionReticle(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);

	//メンバ変数
	MODELX m_modelTemp;
	COrbit *m_orbit[4];
	D3DXVECTOR3 m_posOld;

	bool m_bShot;
	bool m_bGet;
	bool m_bCollision;

	//静的メンバ変数
	static MODELX m_model;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_CHAIN_H_