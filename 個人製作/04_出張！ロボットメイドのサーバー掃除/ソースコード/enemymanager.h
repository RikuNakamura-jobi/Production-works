//=====================================
//
// enemymanagerヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

#include "main.h"
#include "object.h"

//マクロ定義---------------------------
#define MAX_ENEMY_WAVE (1)			//多重スクロール用背景の最大数
#define MAX_ENEMY_SPAWN (36)			//多重スクロール用背景の最大数

//列挙型定義---------------------------

//クラス定義---------------------------
class CEnemy;
class CEnemySpawn
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CEnemySpawn(int nPriority);				//デフォルト
	virtual ~CEnemySpawn();

	//メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual int Update(void);
	virtual void Draw(void);

	void addNumEnemy(int nNumEnemy) { m_nNumEnemy += nNumEnemy; }
	int GetNumEnemy(void) { return m_nNumEnemy; }

	CEnemy *GetEnemy(int nIdxEnemy) { return m_apObject2D[nIdxEnemy]; }
	void SetEnemy(int nIdxEnemy, CEnemy *enemy) { m_apObject2D[nIdxEnemy] = enemy; }

	//静的メンバ関数
	static CEnemySpawn *Create(int nWave, int nEnemySpaen);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CEnemy *m_apObject2D[MAX_ENEMY_SPAWN];
	int m_nNumEnemy;
	int m_nWave;

	//静的メンバ変数

};

class CEnemyManager
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CEnemyManager(int nPriority);				//デフォルト
	virtual ~CEnemyManager();

	//メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	CEnemySpawn *GetEnemyWave(int nWave) { return m_apEnemyWave[nWave]; }

	//静的メンバ関数
	static CEnemyManager *Create(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CEnemySpawn *m_apEnemyWave[MAX_ENEMY_WAVE];

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_ENEMYMANAGER_H_