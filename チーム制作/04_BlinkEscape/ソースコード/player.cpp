//==========================================
//
//  プレイヤークラス(player.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "player.h"
#include "manager.h"
#include "sceneGame.h"
#include "gameManager.h"
#include "debugproc.h"
#include "input.h"
#include "texture.h"
#include "objectOrbit.h"
#include "map.h"
#include "score.h"
#include "sound.h"

//==========================================
//  コンストラクタ
//==========================================
CPlayer::CPlayer() : CObject2D(LABEL_PLAYER)
{
	m_bMiss = false;
	m_bMove = false;
	m_pOrbit = NULL;
	m_posNext = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================
//  デストラクタ
//==========================================
CPlayer::~CPlayer()
{

}

//==========================================
//  初期化処理
//==========================================
HRESULT CPlayer::Init(void)
{
	// 軌跡の生成
	m_pOrbit = CObjectOrbit::Create(this, XCOL_BLUE, CObjectOrbit::OFFSET_PLAYER, 10);
	m_pOrbit->SetLabel(LABEL_PLAYER);

	//次の地点を取得
	m_posNext = CGameManager::GetMap()->GetHeightNext();

	// サウンドの再生
	CManager::GetSound()->Play(CSound::LABEL_SE_CURTAIN);	// 決定音00

	//初期化
	return CObject2D::Init();
}

//==========================================
//  終了処理
//==========================================
void CPlayer::Uninit(void)
{
	//終了
	CObject2D::Uninit();
}

//==========================================
//  更新処理
//==========================================
void CPlayer::Update(void)
{
#ifdef _DEBUG
	if (CManager::GetKeyboard()->GetTrigger(DIK_F5))
	{
		m_bMiss = !m_bMiss;
	}
#endif

	//現在の地点を取得
	SetPosition(CGameManager::GetMap()->GetHeightMin());

	//次の地点を取得
	m_posNext = CGameManager::GetMap()->GetHeightNext();

	//現在の座標を取得
	D3DXVECTOR3 pos = GetPosition();

	//移動処理
	Move(pos);

	//回転処理
	Rotation();
	
	//色が変わる
	if (m_bMiss)
	{
		SetColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	}
	else
	{
		SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (GetPosition().y >= SCREEN_HEIGHT + GetScaling().y)
	{
		m_bMiss = true;
	}

	//更新
	CObject2D::Update();

	//デバッグ表示
	CManager::GetDebugProc()->Print("プレイヤーいるよ\n");
	CManager::GetDebugProc()->Print("プレイヤー座標 : ( %f, %f )\n", pos.x, pos.y);
}

//==========================================
//  描画処理
//==========================================
void CPlayer::Draw(void)
{
	//描画
	CObject2D::Draw();
}

//==========================================
//  移動情報の取得
//==========================================
void CPlayer::GetVecInfo(D3DXVECTOR3* posCurrent, D3DXVECTOR3* posNext, D3DXVECTOR3* vecMove)
{
	*posCurrent = GetPosition();
	*posNext = m_posNext;
	*vecMove = m_vecMove;
}

//==========================================
//  生成処理
//==========================================
CPlayer* CPlayer::Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize, const D3DXVECTOR3& rRot, const D3DXCOLOR& rCol)
{
	//オブジェクトを生成
	CPlayer* pPlayer = nullptr;

	//メモリを確保
	if (pPlayer == nullptr)
	{
		pPlayer = new CPlayer;

		if (pPlayer != nullptr)
		{
			//初期化処理
			pPlayer->Init();

			//各データの設定
			pPlayer->SetPosition(rPos);
			pPlayer->SetScaling(rSize);
			pPlayer->SetRotation(rRot);
			pPlayer->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			pPlayer->BindTexture(CManager::GetTexture()->Regist("data\\TEXTURE\\player000.png"));
		}
	}

	return pPlayer;
}

//==========================================
//  移動
//==========================================
void CPlayer::Move(D3DXVECTOR3 pos)
{
#ifdef _DEBUG
	if (CManager::GetKeyboard()->GetTrigger(DIK_W))
	{
		m_posNext = pos + D3DXVECTOR3(0.0f, -200.0f, 0.0f);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_S))
	{
		m_posNext = pos + D3DXVECTOR3(0.0f, 200.0f, 0.0f);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_A))
	{
		m_posNext = pos + D3DXVECTOR3(-200.0f, 0.0f, 0.0f);
	}
	if (CManager::GetKeyboard()->GetTrigger(DIK_D))
	{
		m_posNext = pos + D3DXVECTOR3(200.0f, 0.0f, 0.0f);
	}
#endif

	//移動ベクトルを算出
	m_vecMove = m_posNext - pos;

	if (m_posNext.y >= 0.0f)
	{
		m_bMove = true;
	}
	else
	{
		m_bMove = false;
	}

	//移動
	if (!m_bMiss)
	{
		if (m_bMove)
		{
			if ((CManager::GetKeyboard()->GetTrigger(DIK_SPACE) || CManager::GetPad()->GetTrigger(CInputPad::KEY_A) || CManager::GetMouse()->GetTrigger(CInputMouse::KEY_LEFT)) && CGameManager::GetState() == CGameManager::STATE_NORMAL)
			{
				//移動先を設定
				SetPosition(m_posNext);

				//次の地点を取得
				m_posNext = CGameManager::GetMap()->GetHeightNext();

				//スコアの加算
				CSceneGame::GetScore()->Add((int)-m_vecMove.y);

				// サウンドの再生
				CManager::GetSound()->Play(CSound::LABEL_SE_MOVE);	// 決定音00

				//雑魚を殺す
				CGameManager::GetMap()->DeleteMin();
			}
		}
	}
}

//==========================================
//  回転
//==========================================
void CPlayer::Rotation()
{
	//角度を取得
	D3DXVECTOR3 rot = GetRotation();

	//移動ベクトルの角度を算出
	float fAngle = atan2f(-m_vecMove.x, -m_vecMove.y);
	rot.z = fAngle;

	//角度を適用
	SetRotation(rot);

	//デバッグ表示
	CManager::GetDebugProc()->Print("プレイヤー向き : %f\n", rot.z);
}
