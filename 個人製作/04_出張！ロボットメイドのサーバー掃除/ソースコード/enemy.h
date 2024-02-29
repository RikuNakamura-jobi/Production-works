//=====================================
//
// enemyヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "objectX.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CLockon;
class CEnemy : public CObjectX
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CEnemy(int nPriority);				//デフォルト
	~CEnemy();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	void SetLockon(void);
	CLockon *GetLockon(void) { return m_lockon; }

	void Damage(int damage) { m_nLife -= damage; }
	int GetLife(void) { return m_nLife; }

	void SetIdxSpawn(int idx){ m_nIdxSpawn = idx; }

	//静的メンバ関数
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, int nWave);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	int m_nCounter;											//行動カウンター
	int m_nPattern;											//行動パターンNo.
	int m_nWave;
	int m_nLife;
	int m_nIdxSpawn;
	CLockon *m_lockon;

	MODELX m_modelTemp;

	//静的メンバ変数
	static MODELX m_model;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_ENEMY_H_