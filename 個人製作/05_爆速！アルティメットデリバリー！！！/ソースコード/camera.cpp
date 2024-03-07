//=============================================================================
//
// 
// Author :中村陸
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "scene.h"
#include "debugproc.h"

using namespace PlayerSpeed;

//=====================================
// コンストラクタ・デストラクタ
//=====================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_cameraLength = 0.0f;
	m_fViewBack = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

CCamera::~CCamera()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(void)
{
	//3人称
	m_posV = D3DXVECTOR3(0.0f, 500.0f, -400.0f);
	m_posR = D3DXVECTOR3(0.0f, 500.0f, 200.0f);
	m_posVDest = D3DXVECTOR3(0.0f, 500.0f, -400.0f);
	m_posRDest = D3DXVECTOR3(0.0f, 500.0f, 200.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.785f, 1.67f);
	m_rotOld = D3DXVECTOR3(0.0f, 3.14f, 1.57f);

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	Rot();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	CInput *input = CManager::Get()->Get()->GetInputKeyboard();
	CInput *inputPad = CManager::Get()->Get()->GetInputPad();
	CInput *inputMouse = CManager::Get()->Get()->GetInputMouse();

	float RotStick = 0.0f;
	float lengthStick = 0.0f;

	if (inputPad != NULL)
	{
		RotStick = atan2f((float)inputPad->GetRStickLRPress(), (float)inputPad->GetRStickUDPress());
		lengthStick = hypotf((float)inputPad->GetRStickLRPress(), (float)inputPad->GetRStickUDPress());
	}
	
	CManager::Get()->Get()->GetDebugProc()->Print("Rスティックの距離: %f\n", lengthStick);

	m_rotOld = m_rot;

	if (input->GetPress(DIK_J) == true)
	{//Aキーが押された時
		//m_rot.y -= 0.04f;
	}
	if (input->GetPress(DIK_L) == true)
	{//Dキーが押された時
		//m_rot.y += 0.04f;
	}
	if (input->GetPress(DIK_I) == true)
	{//Aキーが押された時
		//m_rot.z -= 0.04f;
	}
	if (input->GetPress(DIK_K) == true)
	{//Dキーが押された時
		//m_rot.z += 0.04f;
	}

	if (inputPad != NULL)
	{
		if (lengthStick > 10.0f)
		{
			m_rot.y += (float)inputPad->GetRStickLRPress() * 0.001f * 0.05f;
			m_rot.z += (float)inputPad->GetRStickUDPress() * 0.001f * 0.05f;
		}
	}

	lengthStick = hypotf((float)inputMouse->GetRStickLRPress(), (float)inputMouse->GetRStickUDPress());

	if (lengthStick > 10.0f)
	{
		m_rot.y += (float)inputMouse->GetRStickLRPress() * 0.001f * 0.05f;
		m_rot.z += (float)inputMouse->GetRStickUDPress() * 0.001f * 0.05f;
	}

	if (inputPad != NULL)
	{
		if (inputPad->GetButtonPress(9))
		{
			m_fViewBack += (D3DX_PI - m_fViewBack) * 0.4f;
		}
		else
		{
			m_fViewBack += (0.0f - m_fViewBack) * 0.4f;
		}
	}

	Rot();
}

//===========================
//プレイヤーの描画処理
//===========================
void CCamera::Set(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->Get()->GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f, 180000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW,
		&m_mtxView);
}

//===========================
//プレイヤーの描画処理
//===========================
void CCamera::Rot(void)
{
	//m_cameraLength = 0.0f;
	CPlayer *player = CManager::Get()->Get()->GetScene()->GetPlayer();

	m_posVOld = m_posV;
	m_posROld = m_posR;

	if (m_rot.y > 3.14f)
	{
		m_rot.y -= 6.28f;
	}
	else if (m_rot.y < -3.14f)
	{
		m_rot.y += 6.28f;
	}

	if (m_rot.z < 0.4f)
	{
		m_rot.z = 0.4f;
	}
	else if (m_rot.z > 3.1f)
	{
		m_rot.z = 3.1f;
	}

	float speedLength = player->GetSpeed();
	float speedLengthDest = player->GetSpeedDest();

	if (speedLength < SPEED_MAX)
	{
		speedLength = SPEED_MAX;
	}
	
	if (speedLengthDest == SPEED_TURN)
	{
		m_cameraLength = 150.0f + (speedLength * -35.0f);
	}
	else if (speedLengthDest < SPEED_DASH)
	{
		m_cameraLength = 50.0f + (speedLength * -20.0f);
	}
	else if (speedLengthDest == SPEED_DASH)
	{
		m_cameraLength += (250.0f - m_cameraLength);
	}
	else
	{
		m_cameraLength = 250.0f;
	}
	
	CManager::Get()->Get()->GetDebugProc()->Print("カメラ距離: %f\n", m_cameraLength);

	m_posRDest.x = player->GetPos().x - (sinf(0.0f) * 0.0f);
	m_posRDest.y = player->GetPos().y + 40.0f;
	m_posRDest.z = player->GetPos().z - (cosf(0.0f) * 0.0f);

	m_posVDest.x = m_posRDest.x - (sinf(m_rot.z) * sinf(m_rot.y + m_fViewBack)) * m_cameraLength;
	m_posVDest.y = m_posRDest.y + 0.0f - (cosf(m_rot.z) * m_cameraLength);
	m_posVDest.z = m_posRDest.z - (sinf(m_rot.z) * cosf(m_rot.y + m_fViewBack)) * m_cameraLength;

	m_posR.x += (m_posRDest.x - m_posR.x) * 0.7f;
	m_posR.y += (m_posRDest.y - m_posR.y) * 0.7f;
	m_posR.z += (m_posRDest.z - m_posR.z) * 0.7f;

	m_posV.x += (m_posVDest.x - m_posV.x) * 0.7f;
	m_posV.y += (m_posVDest.y - m_posV.y) * 0.7f;
	m_posV.z += (m_posVDest.z - m_posV.z) * 0.7f;

	float fRotMove, fRotDest, fRotDiff;
	fRotMove = m_rot.y;
	fRotDest = player->GetRot().y + D3DX_PI;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	fRotMove += fRotDiff * 0.04f;

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	if (speedLengthDest != SPEED_WALK && speedLengthDest != 0.0f && !player->GetAir() && !player->GetWall())
	{
		m_rot.y = fRotMove;
	}
}