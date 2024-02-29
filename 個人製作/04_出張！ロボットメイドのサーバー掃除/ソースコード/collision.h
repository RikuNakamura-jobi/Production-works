//=============================================================================
//
// 
// Author :中村陸
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//マクロ定義---------------------------

//列挙型定義---------------------------
class CCollider;
class CCollision
{
public:

	CCollision();
	~CCollision();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	CCollider *GetTop(void) { return m_pTop; }
	void SetTop(CCollider *collider) { m_pTop = collider; }

	CCollider *GetCur(void) { return m_pCur; }
	void SetCur(CCollider *collider) { m_pCur = collider; }

	int GetNumAll(void) { return m_nNumAll; }
	void SetNumAll(int nNumAll) { m_nNumAll = nNumAll; }

	//静的メンバ関数
	static CCollision *Get(void);
	static HRESULT Release(void);

private:

	//メンバ関数

	//メンバ変数
	CCollider *m_pTop;
	CCollider *m_pCur;

	int m_nNumAll;						//オブジェクト総数

	//静的メンバ変数
	static CCollision *m_pCollision;

};

class CCollider
{
public:

	typedef enum
	{
		TYPE_NONE = 0,			//タイプ無し
		TYPE_POINT,			//プレイヤー
		TYPE_BOX,			//プレイヤー
		TYPE_MAX
	}TYPE;

	CCollider();
	~CCollider();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	CCollider *GetPrev(void) { return m_pPrev; }
	void SetPrev(CCollider *collider) { m_pPrev = collider; }

	CCollider *GetNext(void) { return m_pNext; }
	void SetNext(CCollider *collider) { m_pNext = collider; }

	bool GetDeath(void) { return m_bDeath; }
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }

	void SetType(TYPE type) { m_type = type; }

	bool CollisionSquare(D3DXVECTOR3 *posTarget, D3DXVECTOR3 posTargetOld, D3DXVECTOR3 *move);			//箱の当たり判定(押し戻しあり)
	bool CollisionSquareTrigger(D3DXVECTOR3 posTarget);													//箱の当たり判定(内外判定のみ)

	//静的メンバ関数
	static CCollider *Create(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin);

private:

	//メンバ関数
	D3DXVECTOR3 PosRelativeMtx(D3DXVECTOR3 posO, D3DXVECTOR3 rot, D3DXVECTOR3 offset);

	//メンバ変数
	D3DXVECTOR3 *m_pos;
	D3DXVECTOR3 *m_rot;
	D3DXVECTOR3 m_offsetMax;
	D3DXVECTOR3 m_offsetMin;

	int m_nID;									//自分自身のID
	int m_nPriority;							//自分自身のID
	TYPE m_type;

	CCollider *m_pPrev;
	CCollider *m_pNext;

	bool m_bDeath;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_COLLISION_H_
