//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "enemymanager.h"
#include "renderer.h"
#include "manager.h"
#include "object2D.h"
#include "bg.h"
#include "enemy.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CEnemyManager::CEnemyManager(int nPriority = 0)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_WAVE; nCnt++)
	{
		m_apEnemyWave[nCnt] = NULL;
	}
}

CEnemyManager::~CEnemyManager()
{
}

//=====================================
// 生成処理
//=====================================
CEnemyManager *CEnemyManager::Create(void)
{
	CEnemyManager *pEnemyManager;

	//2Dオブジェクトの生成
	pEnemyManager = new CEnemyManager();

	if (pEnemyManager != NULL)
	{
		//初期化
		if (FAILED(pEnemyManager->Init()))
		{
			return NULL;
		}
	}

	return pEnemyManager;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CEnemyManager::Init(void)
{
	m_apEnemyWave[0] = CEnemySpawn::Create(0, 5);

	for (int nCnt = 0; nCnt < MAX_ENEMY_WAVE; nCnt++)
	{
		if (m_apEnemyWave[nCnt] != NULL)
		{
			m_apEnemyWave[nCnt]->Init();
		}
	}

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CEnemyManager::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_WAVE; nCnt++)
	{
		if (m_apEnemyWave[nCnt] != NULL)
		{
			m_apEnemyWave[nCnt]->Uninit();

			delete m_apEnemyWave[nCnt];
			m_apEnemyWave[nCnt] = NULL;
		}
	}
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CEnemyManager::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_WAVE; nCnt++)
	{
		if (m_apEnemyWave[nCnt] != NULL)
		{
			if (m_apEnemyWave[nCnt]->Update() == 0)
			{
				m_apEnemyWave[nCnt]->Uninit();

				delete m_apEnemyWave[nCnt];

				m_apEnemyWave[nCnt] = CEnemySpawn::Create(nCnt, 5);
				m_apEnemyWave[nCnt]->Init();
			}
		}
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CEnemyManager::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_WAVE; nCnt++)
	{
		if (m_apEnemyWave[nCnt] != NULL)
		{
			m_apEnemyWave[nCnt]->Draw();
		}
	}
}

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CEnemySpawn::CEnemySpawn(int nPriority = 0)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_SPAWN; nCnt++)
	{
		m_apObject2D[nCnt] = NULL;
	}

	m_nNumEnemy = 0;
}

CEnemySpawn::~CEnemySpawn()
{
}

//=====================================
// 生成処理
//=====================================
CEnemySpawn *CEnemySpawn::Create(int nWave, int nEnemySpaen)
{
	CEnemySpawn *pEnemyManager;

	//2Dオブジェクトの生成
	pEnemyManager = new CEnemySpawn();

	if (pEnemyManager != NULL)
	{
		//初期化
		if (FAILED(pEnemyManager->Init()))
		{
			return NULL;
		}

		for (int nCnt = 0; nCnt < nEnemySpaen; nCnt++)
		{
			if (pEnemyManager->m_apObject2D[nCnt] == NULL)
			{
				pEnemyManager->m_apObject2D[nCnt] = CEnemy::Create(D3DXVECTOR3(300.0f * nCnt, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, nWave);
				pEnemyManager->m_apObject2D[nCnt]->SetIdxSpawn(nCnt);
			}
		}

		pEnemyManager->m_nNumEnemy = nEnemySpaen;
	}

	return pEnemyManager;
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CEnemySpawn::Init(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_SPAWN; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			m_apObject2D[nCnt]->Init();
		}
	}

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CEnemySpawn::Uninit(void)
{
	
}

//=====================================
// ポリゴンの更新処理
//=====================================
int CEnemySpawn::Update(void)
{
	return m_nNumEnemy;
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CEnemySpawn::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY_SPAWN; nCnt++)
	{
		if (m_apObject2D[nCnt] != NULL)
		{
			m_apObject2D[nCnt]->Draw();
		}
	}
}