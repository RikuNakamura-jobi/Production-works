//=====================================
//
// objectヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//マクロ定義---------------------------
#define MAX_OBJECT (1024)
#define ALL_PRIORITY (8)

//列挙型定義---------------------------

//クラス定義---------------------------
class CCollider;
class CObject
{
public:				//外部からアクセス可能

	//種類
	enum TYPE
	{
		TYPE_NONE = 0,			//タイプ無し
		TYPE_PLAYER,			//プレイヤー
		TYPE_ENEMY,				//敵
		TYPE_BULLET,			//弾
		TYPE_BG,				//背景
		TYPE_BGMULTI,			//多重背景
		TYPE_EFFECT,			//エフェクト
		TYPE_PARTICLE,			//パーティクル
		TYPE_BLOCK,				//ブロック
		TYPE_NUMBER,			//数字
		TYPE_SCORE,				//スコア
		TYPE_IETM,
		TYPE_LOCKON,
		TYPE_FADE,
		TYPE_SKY,
		TYPE_DELIVERYPOINT,
		TYPE_INVISIBLE,
		TYPE_MAX
	};

	//コンストラクタ・デストラクタ
	CObject();				//デフォルト
	CObject(int nPriority);
	virtual ~CObject();

	//メンバ関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	virtual void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight) = 0;
	virtual void SetPos(D3DXVECTOR3 pos) = 0;
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual void SetMove(D3DXVECTOR3 move) = 0;
	virtual D3DXVECTOR3 GetMove(void) = 0;
	virtual void SetRot(D3DXVECTOR3 rot) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual void SetWidth(float fWidth) { return; }
	virtual float GetWidth(void) { return 0.0f; }
	virtual void SetHeight(float fHeight) { return; }
	virtual float GetHeight(void) { return 0.0f; }
	virtual void Damage(int damage) { return; }
	virtual int GetLife(void) { return 0; }
	virtual void SetLockon(void) { return; }

	int GetID(void) { return m_nID; }
	void SetID(int nID) { m_nID = nID; }

	int GetPriority(void) { return m_nPriority; }
	void SetPriority(int nPriority) { m_nPriority = nPriority; }

	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return  m_type; }
	
	CObject *GetObjectNext(void) { return m_pNext; }
	void SetObjectNext(CObject *pNext) { m_pNext = pNext; }

	CObject *GetObjectPrev(void) { return m_pPrev; }
	void SetObjectPrev(CObject *pPrev) { m_pPrev = pPrev; }

	CCollider *GetCollider(void) { return m_pCollider; }
	void SetCollider(CCollider *pCollider) { m_pCollider = pCollider; }

	//静的メンバ関数
	static void ReleaseAll(void);
	static void ReleaseTYPE(TYPE type);
	static void ReleaseTYPEN(TYPE type);
	static void UpdateAll(void);
	static void DrawAll(void);

	static CObject *GetObjectTop(int nPriority) { return m_apTop[nPriority]; }

protected:			//子ならアクセス可能(使わない)
	void Release(void);

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CCollider *m_pCollider;
	int m_nID;									//自分自身のID
	int m_nPriority;							//自分自身のID
	TYPE m_type;

	CObject *m_pPrev;
	CObject *m_pNext;
	bool m_bDeath;

	//静的メンバ変数
	static int m_nNumAll;						//オブジェクト総数

	static CObject *m_apTop[ALL_PRIORITY];
	static CObject *m_apCur[ALL_PRIORITY];

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_OBJECT_H_