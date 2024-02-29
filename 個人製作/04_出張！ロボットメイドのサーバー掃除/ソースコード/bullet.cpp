//=====================================
//
// 
// Author:中村陸
//
//=====================================
#include "bullet.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "effect.h"
#include "particle.h"
#include "score.h"
#include "orbit.h"
#include "lockon.h"
#include "collision.h"
#include "enemy.h"
#include "enemymanager.h"
#include "debugproc.h"
#include "scene.h"
#include "sound.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------

//構造体定義---------------------------

//プロトタイプ宣言---------------------

//静的メンバ変数宣言-------------------
CObjectX::MODELX CBullet::m_model = {};
D3DXCOLOR CBullet::m_colorOrbit = D3DXCOLOR(0.7f, 0.3f, 0.9f, 1.0f);

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CBullet::CBullet(int nPriority = 5) : CObjectX(nPriority)
{
	memset(&m_modelTemp, 0, sizeof(MODELX));
	m_nLife = 0;
	m_pow = 0.3f;
	m_orbit = NULL;
	m_type = TYPE_NORMAL;
}

CBullet::~CBullet()
{
}

//=====================================
// 生成処理
//=====================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, int nWave, int nSpawn, float fWidth, float fHeight, int nLife, TYPE type)
{
	CBullet *pObjectBullet;

	//2Dオブジェクトの生成
	pObjectBullet = new CBullet();

	if (pObjectBullet != NULL)
	{
		pObjectBullet->Set(pos, rot, fWidth, fHeight);

		pObjectBullet->SetModel(m_model);
		pObjectBullet->BindModel(pObjectBullet->GetModel());

		pObjectBullet->m_nWave = nWave;
		pObjectBullet->m_nSpawn = nSpawn;
		pObjectBullet->m_type = type;

		//初期化
		if (FAILED(pObjectBullet->Init()))
		{
			pObjectBullet->Release();
		}

		pObjectBullet->SetMove(move);

		pObjectBullet->m_nLife = nLife;
	}

	return pObjectBullet;
}

//=====================================
// ポリゴンのテクスチャ処理
//=====================================
HRESULT CBullet::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->Get()->GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;

	//xファイルの読み込み
	D3DXLoadMeshFromX("data/MODEL/Bullet000.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model.pBuffMatModel,
		NULL,
		&m_model.dwNumMatModel,
		&m_model.pMeshModel);

	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//頂点数取得
	nNumVtx = m_model.pMeshModel->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(m_model.pMeshModel->GetFVF());

	//頂点バッファをロック
	m_model.pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		if (vtx.x >= m_model.vtxMax.x)
		{
			m_model.vtxMax.x = vtx.x;
		}
		if (vtx.x <= m_model.vtxMin.x)
		{
			m_model.vtxMin.x = vtx.x;
		}

		if (vtx.y >= m_model.vtxMax.y)
		{
			m_model.vtxMax.y = vtx.y;
		}
		if (vtx.y <= m_model.vtxMin.y)
		{
			m_model.vtxMin.y = vtx.y;
		}

		if (vtx.z >= m_model.vtxMax.z)
		{
			m_model.vtxMax.z = vtx.z;
		}
		if (vtx.z <= m_model.vtxMin.z)
		{
			m_model.vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	m_model.pMeshModel->UnlockVertexBuffer();

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_model.pBuffMatModel->GetBufferPointer();

	m_model.pTexture = new LPDIRECT3DTEXTURE9[m_model.dwNumMatModel];

	for (int nCntMat = 0; nCntMat < (int)m_model.dwNumMatModel; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_model.pTexture[nCntMat]);
		}
		else
		{
			m_model.pTexture[nCntMat] = NULL;
		}
	}

	return S_OK;
}
void CBullet::Unload(void)
{
	//テクスチャの破棄
	if (m_model.pMeshModel != NULL)
	{
		m_model.pMeshModel->Release();
		m_model.pMeshModel = NULL;
	}

	//頂点バッファの破棄
	if (m_model.pBuffMatModel != NULL)
	{
		m_model.pBuffMatModel->Release();
		m_model.pBuffMatModel = NULL;
	}

	//頂点バッファの破棄
	if (m_model.pTexture != NULL)
	{
		delete[] m_model.pTexture;
		m_model.pTexture = NULL;
	}
}

//=====================================
// ポリゴンの初期化処理
//=====================================
HRESULT CBullet::Init(void)
{
	CObjectX::Init();

	D3DXMATRIX mtxTemp;
	D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxTemp);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxTemp, &mtxTemp, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxTemp, &mtxTemp, &mtxTrans);

	if (m_nWave != -1 && m_nSpawn != -1)
	{
		if (m_type == TYPE_EFFECT)
		{
			m_orbit = COrbit::Create(mtxTemp, D3DXVECTOR3(0.0f, 8.0f, 0.0f), D3DXVECTOR3(0.0f, -8.0f, 0.0f), m_colorOrbit, 15);
		}
		else
		{
			m_orbit = COrbit::Create(mtxTemp, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_colorOrbit, 3);
		}
	}
	else
	{
		m_orbit = COrbit::Create(mtxTemp, D3DXVECTOR3(0.0f, 2.0f, 0.0f), D3DXVECTOR3(0.0f, -2.0f, 0.0f), m_colorOrbit, 15);
	}

	SetType(TYPE_BULLET);

	return S_OK;
}

//=====================================
// ポリゴンの終了処理
//=====================================
void CBullet::Uninit(void)
{
	m_orbit->Uninit();

	CObjectX::Uninit();
}

//=====================================
// ポリゴンの更新処理
//=====================================
void CBullet::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = pos;
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	m_nLife--;

	if (m_nWave != -1 && m_nSpawn != -1)
	{
		if (CManager::Get()->Get()->GetScene()->GetEnemyManager() != NULL)
		{
			if (CManager::Get()->Get()->GetScene()->GetEnemyManager()->GetEnemyWave(m_nWave) != NULL)
			{
				CEnemy *pEnemy = CManager::Get()->Get()->GetScene()->GetEnemyManager()->GetEnemyWave(m_nWave)->GetEnemy(m_nSpawn);

				if (pEnemy != NULL)
				{
					if (pEnemy->GetLockon() != NULL)
					{
						float fRotMove, fRotDest, fRotDiff;
						float length = D3DXVec3Length(&move);
						float epos = pEnemy->GetPos().x;

						if (m_nLife < 198)
						{
							m_pow = 1.0f;
						}

						if (m_type == TYPE_EFFECT)
						{
							if (m_nLife < 198)
							{
								m_nLife -= 4;
							}

							fRotMove = atan2f(move.x, move.z);
							fRotDest = atan2f(pEnemy->GetLockon()->GetPos().x - pos.x, pEnemy->GetLockon()->GetPos().z - pos.z);
							fRotDiff = fRotDest - fRotMove;

							if (fRotDiff > D3DX_PI)
							{
								fRotDiff -= (D3DX_PI * 2);
							}
							else if (fRotDiff < -D3DX_PI)
							{
								fRotDiff += (D3DX_PI * 2);
							}

							fRotMove += fRotDiff * m_pow;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							move.x = sinf(fRotMove) * length;
							move.z = cosf(fRotMove) * length;

							fRotMove = atan2f(move.x, move.z);
							fRotDest = atan2f(pEnemy->GetLockon()->GetPos().x - pos.x, pEnemy->GetLockon()->GetPos().y - pos.y);
							fRotDiff = fRotDest - fRotMove;

							if (fRotDiff > D3DX_PI)
							{
								fRotDiff -= (D3DX_PI * 2);
							}
							else if (fRotDiff < -D3DX_PI)
							{
								fRotDiff += (D3DX_PI * 2);
							}

							fRotMove += fRotDiff * m_pow;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							move.y = cosf(fRotMove) * length;
						}
						else
						{
							fRotMove = atan2f(move.x, move.y);
							fRotDest = atan2f(pEnemy->GetLockon()->GetPos().x - pos.x, pEnemy->GetLockon()->GetPos().y - pos.y);
							fRotDiff = fRotDest - fRotMove;

							if (fRotDiff > D3DX_PI)
							{
								fRotDiff -= (D3DX_PI * 2);
							}
							else if (fRotDiff < -D3DX_PI)
							{
								fRotDiff += (D3DX_PI * 2);
							}

							fRotMove += fRotDiff * m_pow;

							if (fRotMove > D3DX_PI)
							{
								fRotMove -= (D3DX_PI * 2);
							}
							else if (fRotMove < -D3DX_PI)
							{
								fRotMove += (D3DX_PI * 2);
							}

							move.x = sinf(fRotMove) * length;
							move.y = cosf(fRotMove) * length;
							move.z = 0.0f;
							pos.z = 0.0f;
						}
					}
				}
			}
		}
	}

	//移動処理
	pos += move;

	//CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 15, 32.0f, 32.0f);

	/*Reflect(&pos, &posOld, &rot, &move, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f));
	Reflect(&pos, &posOld, &rot, &move, D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));
	Reflect(&pos, &posOld, &rot, &move, D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f));
	Reflect(&pos, &posOld, &rot, &move, D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));*/
	//Reflect(&pos, &posOld, &rot, &move, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f));

	if (Collision(&pos, &posOld, &move, &rot) == false)
	{
		SetMove(move);
		SetRot(rot);
		SetPos(pos);

		CObjectX::Update();

		//CEffect::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 3, 8.0f, 8.0f);

		if (m_orbit != NULL)
		{
			D3DXMATRIX mtxTemp;
			D3DXMATRIX mtxRot, mtxTrans;		//計算用マトリックス

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxTemp);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				GetRot().y, GetRot().x, GetRot().z);
			D3DXMatrixMultiply(&mtxTemp, &mtxTemp, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans,
				GetPos().x, GetPos().y, GetPos().z);
			D3DXMatrixMultiply(&mtxTemp, &mtxTemp, &mtxTrans);

			m_orbit->SetPositionOffset(mtxTemp);
		}

		if (m_nLife <= 0)
		{
			Uninit();
		}
	}
}

//=====================================
// ポリゴンの描画処理
//=====================================
void CBullet::Draw(void)
{
	//CObjectX::Draw();
}

//=====================================
// 弾の当たり判定処理
//=====================================
bool CBullet::Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 *rot)
{
	if (m_nWave != -1 && m_nSpawn != -1)
	{
		if (CManager::Get()->Get()->GetScene()->GetEnemyManager() != NULL)
		{
			if (CManager::Get()->Get()->GetScene()->GetEnemyManager()->GetEnemyWave(m_nWave) != NULL)
			{
				CEnemy *pEnemy = CManager::Get()->Get()->GetScene()->GetEnemyManager()->GetEnemyWave(m_nWave)->GetEnemy(m_nSpawn);

				if (pEnemy != NULL)
				{
					if (pEnemy->GetLockon() != NULL && m_type != TYPE::TYPE_EFFECT)
					{
						if (pEnemy->GetCollider()->CollisionSquareTrigger(*pos) == true)
						{
							/*CExplosion::Create(*pos, GetRot(), 64.0f, 64.0f);*/

							pEnemy->Damage(1);

							if (pEnemy->GetLife() <= 0)
							{
								CParticle::Create(*pos, GetRot(), D3DXCOLOR(0.8f, 0.2f, 0.1f, 1.0f), 10, 5, 15, 15, 32.0f, 32.0f);
								CManager::Get()->GetScene()->GetScore()->AddScore(500);

								pEnemy->Uninit();

								CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
							}

							Uninit();

							return true;
						}
					}
				}
			}
		}
	}
	else
	{
		for (int nCntPri = 0; nCntPri < ALL_PRIORITY; nCntPri++)
		{
			if (nCntPri != 4)
			{
				CObject *pObj;

				pObj = GetObjectTop(nCntPri);

				while (pObj != NULL)
				{
					CObject *pObjNext = pObj->GetObjectNext();

					CObject::TYPE type;

					//種類取得
					type = pObj->GetType();

					if (type == TYPE_ENEMY)
					{
						if (pObj->GetCollider()->CollisionSquareTrigger(*pos) == true)
						{
							/*CExplosion::Create(*pos, GetRot(), 64.0f, 64.0f);*/

							pObj->Damage(1);

							if (pObj->GetLife() <= 0)
							{
								CParticle::Create(*pos, GetRot(), D3DXCOLOR(0.8f, 0.2f, 0.1f, 1.0f), 10, 5, 15, 15, 32.0f, 32.0f);
								CManager::Get()->GetScene()->GetScore()->AddScore(300);

								pObj->Uninit();

								CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
							}

							Uninit();

							return true;
						}
					}

					pObj = pObjNext;
				}
			}
		}
	}

	return false;
}

//=====================================
// 弾の反射処理
//=====================================
void CBullet::Reflect(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, D3DXVECTOR3 posWall1, D3DXVECTOR3 posWall2)
{
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fDot;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef, posCol;

	//板側面のベクトル
	vecLine.x = posWall1.y - posWall2.y;
	vecLine.y = posWall1.x - posWall2.x;

	//基準点と岩の位置のベクトル
	vecToPos.x = pos->y - posWall2.y;
	vecToPos.y = pos->x - posWall2.x;

	vecToPosOld.x = posOld->y - posWall2.y;
	vecToPosOld.y = posOld->x - posWall2.x;

	//移動量ベクトル
	vecMove.x = pos->y - posOld->y;
	vecMove.y = pos->x - posOld->x;

	//側面と岩の位置の外積
	fOutToPos = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);
	fOutToPosOld = (vecLine.y * vecToPosOld.x) - (vecLine.x * vecToPosOld.y);

	//交点計算
	fOutUnit = (vecLine.y * vecMove.x) - (vecLine.x * vecMove.y);
	fOutRate = (vecToPos.y * vecMove.x) - (vecToPos.x * vecMove.y);
	fRate = fOutRate / fOutUnit;

	//側面衝突判定
	if (fOutToPos < 0.0f && fOutToPosOld > 0.0f && fRate < 1.0f && fRate > 0.0f)
	{
		//岩の位置移動
		pos->y = posWall2.y + (vecLine.x * fRate);
		pos->x = posWall2.x + (vecLine.y * fRate);

		//法線ベクトル計算
		vecNor.x = vecLine.y;
		vecNor.y = -vecLine.x;
		vecNor.z = 0.0f;

		//法線ベクトル単位化
		D3DXVec3Normalize(&vecNor, &vecNor);

		//水平方向大きさ
		fDot = (-vecMove.x * vecNor.x) + (-vecMove.y * vecNor.y);

		//水平方向ベクトル計算
		vecDot = (vecNor * fDot) * 2.0f;

		//移動量ベクトル計算
		vecMoveRef = vecMove + vecDot;

		//移動量ベクトル単位化(必要か分からない)
		D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

		//移動量代入
		rot->z += (atan2f(posWall1.x - posWall2.x, posWall1.y - posWall2.y) - atan2f(move->x, move->y)) * 2.0f;
		//*move = vecMoveRef * hypotf(move->x, move->y);
		move->x = sinf(rot->z) * hypotf(move->x, move->y);
		move->y = cosf(rot->z) * hypotf(move->x, move->y);
	}
}

void CBullet::color(void)
{
	CInput *input = CManager::Get()->Get()->GetInputKeyboard();

	if (input->GetPress(DIK_LSHIFT) == true)
	{
		if (input->GetPress(DIK_Y) == true)
		{//Aキーが押された時
			m_colorOrbit.r += 0.01f;

			if (m_colorOrbit.r > 1.0f)
			{
				m_colorOrbit.r = 1.0f;
			}
		}
		if (input->GetPress(DIK_H) == true)
		{//Aキーが押された時
			m_colorOrbit.r -= 0.01f;

			if (m_colorOrbit.r < 0.0f)
			{
				m_colorOrbit.r = 0.0f;
			}
		}

		if (input->GetPress(DIK_U) == true)
		{//Aキーが押された時
			m_colorOrbit.g += 0.01f;

			if (m_colorOrbit.g > 1.0f)
			{
				m_colorOrbit.g = 1.0f;
			}
		}
		if (input->GetPress(DIK_J) == true)
		{//Aキーが押された時
			m_colorOrbit.g -= 0.01f;

			if (m_colorOrbit.g < 0.0f)
			{
				m_colorOrbit.g = 0.0f;
			}
		}

		if (input->GetPress(DIK_I) == true)
		{//Aキーが押された時
			m_colorOrbit.b += 0.01f;

			if (m_colorOrbit.b > 1.0f)
			{
				m_colorOrbit.b = 1.0f;
			}
		}
		if (input->GetPress(DIK_K) == true)
		{//Aキーが押された時
			m_colorOrbit.b -= 0.01f;

			if (m_colorOrbit.b < 0.0f)
			{
				m_colorOrbit.b = 0.0f;
			}
		}

		if (input->GetPress(DIK_O) == true)
		{//Aキーが押された時
			m_colorOrbit.a += 0.01f;

			if (m_colorOrbit.a > 1.0f)
			{
				m_colorOrbit.a = 1.0f;
			}
		}
		if (input->GetPress(DIK_L) == true)
		{//Aキーが押された時
			m_colorOrbit.a -= 0.01f;

			if (m_colorOrbit.a < 0.0f)
			{
				m_colorOrbit.a = 0.0f;
			}
		}
	}
	else
	{
		if (input->GetTrigger(DIK_Y) == true)
		{//Aキーが押された時
			m_colorOrbit.r += 0.01f;

			if (m_colorOrbit.r > 1.0f)
			{
				m_colorOrbit.r = 1.0f;
			}
		}
		if (input->GetTrigger(DIK_H) == true)
		{//Aキーが押された時
			m_colorOrbit.r -= 0.01f;

			if (m_colorOrbit.r < 0.0f)
			{
				m_colorOrbit.r = 0.0f;
			}
		}

		if (input->GetTrigger(DIK_U) == true)
		{//Aキーが押された時
			m_colorOrbit.g += 0.01f;

			if (m_colorOrbit.g > 1.0f)
			{
				m_colorOrbit.g = 1.0f;
			}
		}
		if (input->GetTrigger(DIK_J) == true)
		{//Aキーが押された時
			m_colorOrbit.g -= 0.01f;

			if (m_colorOrbit.g < 0.0f)
			{
				m_colorOrbit.g = 0.0f;
			}
		}

		if (input->GetTrigger(DIK_I) == true)
		{//Aキーが押された時
			m_colorOrbit.b += 0.01f;

			if (m_colorOrbit.b > 1.0f)
			{
				m_colorOrbit.b = 1.0f;
			}
		}
		if (input->GetTrigger(DIK_K) == true)
		{//Aキーが押された時
			m_colorOrbit.b -= 0.01f;

			if (m_colorOrbit.b < 0.0f)
			{
				m_colorOrbit.b = 0.0f;
			}
		}

		if (input->GetTrigger(DIK_O) == true)
		{//Aキーが押された時
			m_colorOrbit.a += 0.01f;

			if (m_colorOrbit.a > 1.0f)
			{
				m_colorOrbit.a = 1.0f;
			}
		}
		if (input->GetTrigger(DIK_L) == true)
		{//Aキーが押された時
			m_colorOrbit.a -= 0.01f;

			if (m_colorOrbit.a < 0.0f)
			{
				m_colorOrbit.a = 0.0f;
			}
		}
	}

	CManager::Get()->GetDebugProc()->Print("弾の軌跡の色: %f %f %f %f\n", m_colorOrbit.r, m_colorOrbit.g, m_colorOrbit.b, m_colorOrbit.a);
}
