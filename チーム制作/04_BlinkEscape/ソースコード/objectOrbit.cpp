//============================================================
//
//	オブジェクト軌跡処理 [objectOrbit.cpp]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	インクルードファイル
//************************************************************
#include "objectOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "scene.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "map.h"
#include "pause.h"

//************************************************************
//	静的メンバ変数宣言
//************************************************************
const float CObjectOrbit::mc_aOffset[][MAX_OFFSET]	// オフセットの位置加減量
{
	{ 10.0f, -10.0f },	// プレイヤーオフセット
};

//************************************************************
//	子クラス [CObjectOrbit] のメンバ関数
//************************************************************
//============================================================
//	コンストラクタ
//============================================================
CObjectOrbit::CObjectOrbit()
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_state = STATE_NORMAL;	// 状態
	m_nCounterState = 0;	// 状態管理カウンター
	m_nNumVtx = 0;			// 必要頂点数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_orbit, 0, sizeof(m_orbit));	// 軌跡の情報
}

//============================================================
//	オーバーロードコンストラクタ
//============================================================
CObjectOrbit::CObjectOrbit(const CObject::LABEL label, const int nPriority) : CObject(label, nPriority)
{
	// メンバ変数をクリア
	m_pVtxBuff = NULL;		// 頂点バッファ
	m_state = STATE_NORMAL;	// 状態
	m_nCounterState = 0;	// 状態管理カウンター
	m_nNumVtx = 0;			// 必要頂点数
	m_nTextureID = 0;		// テクスチャインデックス
	memset(&m_orbit, 0, sizeof(m_orbit));	// 軌跡の情報
}

//============================================================
//	デストラクタ
//============================================================
CObjectOrbit::~CObjectOrbit()
{

}

//============================================================
//	初期化処理
//============================================================
HRESULT CObjectOrbit::Init(void)
{
	// メンバ変数を初期化
	m_pVtxBuff		= NULL;			// 頂点バッファ
	m_state			= STATE_NORMAL;	// 状態
	m_nCounterState	= 0;			// 状態管理カウンター
	m_nNumVtx		= 0;			// 必要頂点数
	m_nTextureID	= NONE_IDX;		// テクスチャインデックス

	// 軌跡の情報を初期化
	m_orbit.pPosPoint	= NULL;			// 各頂点座標
	m_orbit.pColPoint	= NULL;			// 各頂点カラー
	m_orbit.pParent		= NULL;			// 親オブジェクト
	m_orbit.posVanish	= VEC3_ZERO;	// 消失開始時の親の位置
	m_orbit.rotVanish	= VEC3_ZERO;	// 消失開始時の親の向き
	m_orbit.nPart		= 1;			// 分割数
	m_orbit.nTexPart	= 1;			// テクスチャ分割数
	m_orbit.bAlpha		= false;		// 透明化状況
	m_orbit.bInit		= false;		// 初期化状況

	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		m_orbit.aPos[nCntOff] = VEC3_ZERO;	// 両端の基準位置
		m_orbit.aCol[nCntOff] = XCOL_WHITE;	// 両端の基準色
		m_orbit.aOffset[nCntOff] = 0.0f;	// 両端のオフセット
	}

	// 長さを設定
	if (FAILED(SetLength(1)))
	{ // 長さの設定に失敗した場合

		// 失敗を返す
		return E_FAIL;
	}

	// 成功を返す
	return S_OK;
}

//============================================================
//	終了処理
//============================================================
void CObjectOrbit::Uninit(void)
{
	// 各頂点座標の破棄
	if (m_orbit.pPosPoint != NULL)
	{ // 各頂点座標が使用中の場合

		// メモリ開放
		delete[] m_orbit.pPosPoint;
		m_orbit.pPosPoint = NULL;
	}

	// 各頂点カラーの破棄
	if (m_orbit.pColPoint != NULL)
	{ // 各頂点カラーが使用中の場合

		// メモリ開放
		delete[] m_orbit.pColPoint;
		m_orbit.pColPoint = NULL;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// オブジェクト軌跡を破棄
	Release();
}

//============================================================
//	更新処理
//============================================================
void CObjectOrbit::Update(void)
{

}

//============================================================
//	描画処理
//============================================================
void CObjectOrbit::Draw(void)
{
	// 変数を宣言
	D3DXVECTOR3 posParent = VEC3_ZERO;	// 親の位置
	D3DXVECTOR3 rotParent = VEC3_ZERO;	// 親の向き
	bool bUpdate = true;	// 更新状況

	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ
	CTexture *pTexture = CManager::GetTexture();						// テクスチャへのポインタ

	if (CManager::GetScene()->GetMode() == CScene::MODE_GAME)
	{ // モードがゲームの場合

		if (CSceneGame::GetPause()->IsPause())
		{ // ポーズ中の場合

			// 更新しない状況にする
			bUpdate = false;
		}
	}
	
	if (m_state != STATE_NONE)
	{ // 何もしない状態ではない場合

		if (bUpdate)
		{ // 更新する状況の場合

			//------------------------------------------------
			//	状態ごとの設定
			//------------------------------------------------
			switch (m_state)
			{ // 状態ごとの処理
			case STATE_NORMAL:	// 通常状態

				// 親の位置を設定
				posParent = m_orbit.pParent->GetPosition();

				// 親の向きを設定
				rotParent = m_orbit.pParent->GetRotation();

				break;

			case STATE_VANISH:	// 消失状態

				// 親の消失位置を設定
				posParent = m_orbit.posVanish;

				// 親の消失向きを設定
				rotParent = m_orbit.rotVanish;

				// カウンターを加算
				if (m_nCounterState < (m_nNumVtx / MAX_OFFSET) + 1)
				{ // カウンターが軌跡が伸び切る時間より小さい場合

					// カウンターを加算
					m_nCounterState++;
				}
				else
				{ // カウンターが軌跡が伸び切る時間以上の場合

					// カウンターを補正
					m_nCounterState = 0;

					// 状態を設定
					m_state = STATE_NONE;	// 何もしない状態
				}

				break;

			default:	// 例外状態
				assert(false);
				break;
			}

			//------------------------------------------------
			//	オフセットの初期化
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
			{ // オフセットの数分繰り返す

				// 基準位置を設定
				m_orbit.aPos[nCntOff].x = posParent.x + sinf(rotParent.z - HALF_PI) * m_orbit.aOffset[nCntOff];
				m_orbit.aPos[nCntOff].y = posParent.y + cosf(rotParent.z - HALF_PI) * m_orbit.aOffset[nCntOff];
				m_orbit.aPos[nCntOff].z = 0.0f;
			}

			//------------------------------------------------
			//	頂点座標と頂点カラーの情報をずらす
			//------------------------------------------------
			for (int nCntVtx = m_nNumVtx - 1; nCntVtx >= MAX_OFFSET; nCntVtx--)
			{ // 維持する頂点の最大数分繰り返す (オフセット分は含まない)

				// 頂点情報をずらす
				m_orbit.pPosPoint[nCntVtx] = m_orbit.pPosPoint[nCntVtx - MAX_OFFSET];
				m_orbit.pColPoint[nCntVtx] = m_orbit.pColPoint[nCntVtx - MAX_OFFSET];
			}

			//------------------------------------------------
			//	最新の頂点座標と頂点カラーの情報を設定
			//------------------------------------------------
			for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
			{ // オフセットの数分繰り返す

				// 頂点座標の設定
				m_orbit.pPosPoint[nCntOff] = m_orbit.aPos[nCntOff];

				// 頂点カラーの設定
				m_orbit.pColPoint[nCntOff] = m_orbit.aCol[nCntOff];
			}
		}

		//----------------------------------------------------
		//	頂点座標と頂点カラーの情報を初期化
		//----------------------------------------------------
		if (!m_orbit.bInit)
		{ // 初期化済みではない場合

			for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
			{ // 維持する頂点の最大数分繰り返す

				// 頂点座標の設定
				m_orbit.pPosPoint[nCntVtx] = m_orbit.aPos[nCntVtx % MAX_OFFSET];

				// 頂点カラーの設定
				m_orbit.pColPoint[nCntVtx] = m_orbit.aCol[nCntVtx % MAX_OFFSET];
			}

			// 初期化済みにする
			m_orbit.bInit = true;
		}

		//----------------------------------------------------
		//	ポリゴンの描画
		//----------------------------------------------------
		// 頂点情報の設定
		SetVtx();

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetTexture(m_nTextureID));

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumVtx - 2);
	}
}

//============================================================
//	状態取得処理
//============================================================
int CObjectOrbit::GetState(void)
{
	// 状態を返す
	return m_state;
}

//============================================================
//	生成処理
//============================================================
CObjectOrbit *CObjectOrbit::Create
(
	CObject *pParent,		// 親オブジェクト
	const D3DXCOLOR& rCol,	// 色
	const OFFSET offset,	// オフセット
	const int nPart,		// 分割数
	const int nTexPart,		// テクスチャ分割数
	const bool bAlpha		// 透明化状況
)
{
	// ポインタを宣言
	CObjectOrbit *pObjectOrbit = NULL;		// オブジェクト軌跡生成用

	if (pObjectOrbit == NULL)
	{ // 使用されていない場合

		// メモリ確保
		pObjectOrbit = new CObjectOrbit;	// オブジェクト軌跡
	}
	else { assert(false); return NULL; }	// 使用中

	if (pObjectOrbit != NULL)
	{ // 確保に成功している場合

		// オブジェクト軌跡の初期化
		if (FAILED(pObjectOrbit->Init()))
		{ // 初期化に失敗した場合

			// メモリ開放
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// 失敗を返す
			return NULL;
		}

		// 親のオブジェクトを設定
		pObjectOrbit->SetParent(pParent);

		// 色を設定
		pObjectOrbit->SetColor(rCol);

		// オフセットを設定
		pObjectOrbit->SetOffset(offset);

		// テクスチャ分割数を設定
		pObjectOrbit->SetTexPart(nTexPart);

		// 透明化状況を設定
		pObjectOrbit->SetEnableAlpha(bAlpha);

		// 長さを設定
		if (FAILED(pObjectOrbit->SetLength(nPart)))
		{ // 長さの設定に失敗した場合

			// メモリ開放
			delete pObjectOrbit;
			pObjectOrbit = NULL;

			// 失敗を返す
			return NULL;
		}

		// 確保したアドレスを返す
		return pObjectOrbit;
	}
	else { assert(false); return NULL; }	// 確保失敗
}

//============================================================
//	テクスチャ割当処理
//============================================================
void CObjectOrbit::BindTexture(const int nTextureID)
{
	// テクスチャインデックスを代入
	m_nTextureID = nTextureID;
}

//============================================================
//	状態の設定処理
//============================================================
void CObjectOrbit::SetState(const STATE state)
{
	if (state == m_state && state != STATE_NORMAL)
	{ // 設定する状態が現在の状態且つ、設定する状態が通常状態の場合

		// 処理を抜ける
		return;
	}

	// 引数の状態を設定
	m_state = state;

	switch (m_state)
	{ // 状態ごとの処理
	case STATE_NONE:	// 何もしない状態

		// 無し

		break;

	case STATE_NORMAL:	// 通常状態

		// 初期化していない状態にする
		m_orbit.bInit = false;

		break;

	case STATE_VANISH:	// 消失状態

		// 現在の親の位置を消失する位置に設定
		m_orbit.posVanish = m_orbit.pParent->GetPosition();

		// 現在の親の向きを消失する向きに設定
		m_orbit.rotVanish = m_orbit.pParent->GetRotation();

		break;

	default:	// 例外状態
		assert(false);
		break;
	}
}

//============================================================
//	親オブジェクトの設定処理
//============================================================
void CObjectOrbit::SetParent(CObject *pParent)
{
	// 引数の親オブジェクトを設定
	m_orbit.pParent = pParent;
}

//============================================================
//	色の設定処理
//============================================================
void CObjectOrbit::SetColor(const D3DXCOLOR& rCol)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		// 引数の色を設定
		m_orbit.aCol[nCntOff] = rCol;
	}
}

//============================================================
//	オフセットの設定処理
//============================================================
void CObjectOrbit::SetOffset(const OFFSET offset)
{
	for (int nCntOff = 0; nCntOff < MAX_OFFSET; nCntOff++)
	{ // オフセットの数分繰り返す

		// 引数のオフセットを設定
		m_orbit.aOffset[nCntOff] = mc_aOffset[(int)offset][nCntOff];
	}
}

//============================================================
//	テクスチャ分割数の設定処理
//============================================================
void CObjectOrbit::SetTexPart(const int nTexPart)
{
	// 例外処理
	assert(nTexPart > 0);	// 0以下エラー

	// 引数のテクスチャ分割数を設定
	m_orbit.nTexPart = nTexPart;
}

//============================================================
//	透明化状況の設定処理
//============================================================
void CObjectOrbit::SetEnableAlpha(const bool bAlpha)
{
	// 引数の透明化状況を設定
	m_orbit.bAlpha = bAlpha;
}

//============================================================
//	初期化状況の設定処理
//============================================================
void CObjectOrbit::SetEnableInit(const bool bInit)
{
	// 引数の初期化状況を設定
	m_orbit.bInit = bInit;
}

//============================================================
//	長さの設定処理
//============================================================
HRESULT CObjectOrbit::SetLength(const int nPart)
{
	// ポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// 必要頂点数を求める
	m_nNumVtx = nPart * MAX_OFFSET;

	//--------------------------------------------------------
	//	各頂点座標の破棄・生成
	//--------------------------------------------------------
	// 各頂点座標の破棄
	if (m_orbit.pPosPoint != NULL)
	{ // 各頂点座標が使用中の場合

		// メモリ開放
		delete[] m_orbit.pPosPoint;
		m_orbit.pPosPoint = NULL;
	}

	// 各頂点座標の情報を設定
	if (m_orbit.pPosPoint == NULL)
	{ // 非使用中の場合

		// 各頂点座標のメモリ確保
		m_orbit.pPosPoint = new D3DXVECTOR3[m_nNumVtx];

		if (m_orbit.pPosPoint != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_orbit.pPosPoint, 0, sizeof(D3DXVECTOR3) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	//--------------------------------------------------------
	//	各頂点カラーの破棄・生成
	//--------------------------------------------------------
	// 各頂点カラーの破棄
	if (m_orbit.pColPoint != NULL)
	{ // 各頂点カラーが使用中の場合

		// メモリ開放
		delete[] m_orbit.pColPoint;
		m_orbit.pColPoint = NULL;
	}

	// 各頂点カラーの情報を設定
	if (m_orbit.pColPoint == NULL)
	{ // 非使用中の場合

		// 各頂点カラーのメモリ確保
		m_orbit.pColPoint = new D3DXCOLOR[m_nNumVtx];

		if (m_orbit.pColPoint != NULL)
		{ // 確保に成功した場合

			// メモリクリア
			memset(m_orbit.pColPoint, 0, sizeof(D3DXCOLOR) * m_nNumVtx);
		}
		else { assert(false); return E_FAIL; }	// 確保失敗
	}
	else { assert(false); return E_FAIL; }	// 使用中

	//--------------------------------------------------------
	//	頂点バッファの破棄・生成
	//--------------------------------------------------------
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{ // 頂点バッファが使用中の場合

		// メモリ開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 頂点バッファの情報を設定
	if (m_pVtxBuff == NULL)
	{ // 非使用中の場合

		// 頂点バッファの生成
		if (FAILED(pDevice->CreateVertexBuffer
		( // 引数
			sizeof(VERTEX_2D) * m_nNumVtx,	// 必要頂点数
			D3DUSAGE_WRITEONLY,	// 使用方法
			FVF_VERTEX_2D,		// 頂点フォーマット
			D3DPOOL_MANAGED,	// メモリの指定
			&m_pVtxBuff,		// 頂点バッファへのポインタ
			NULL
		)))
		{ // 頂点バッファの生成に失敗した場合

			// 失敗を返す
			assert(false);
			return E_FAIL;
		}
	}
	else { assert(false); return E_FAIL; }	// 使用中

	// 頂点情報の設定
	SetVtx();

	// 成功を返す
	return S_OK;
}

//============================================================
//	頂点情報の設定処理
//============================================================
void CObjectOrbit::SetVtx(void)
{
	// ポインタを宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	if (m_pVtxBuff != NULL)
	{ // 軌跡の頂点バッファが使用中の場合

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVtx = 0; nCntVtx < m_nNumVtx; nCntVtx++)
		{ // 維持する頂点の最大数分繰り返す

			// 頂点座標の設定
			pVtx[0].pos = m_orbit.pPosPoint[nCntVtx];

			// rhw の設定
			pVtx[0].rhw = 1.0f;

			// 頂点カラーの設定
			if (m_orbit.bAlpha)
			{ // 透明にしていく場合

				pVtx[0].col = D3DXCOLOR
				( // 引数
					m_orbit.pColPoint[nCntVtx].r,	// r
					m_orbit.pColPoint[nCntVtx].g,	// g
					m_orbit.pColPoint[nCntVtx].b,	// b
					m_orbit.pColPoint[nCntVtx].a - (m_orbit.pColPoint[nCntVtx].a / (m_nNumVtx * 0.5f)) * (nCntVtx / 2)	// a
				);
			}
			else
			{ // 透明度を変えない場合

				pVtx[0].col = m_orbit.pColPoint[nCntVtx];
			}

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			( // 引数
				(1.0f / (float)m_orbit.nTexPart) * (nCntVtx / 2),	// u
				1.0f * -((nCntVtx % 2) - 1)							// v
			);

			// 頂点データのポインタを進める
			pVtx += 1;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
	else { assert(false); }	// 非使用中
}
