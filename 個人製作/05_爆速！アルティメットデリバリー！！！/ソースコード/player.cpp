//=====================================
//
// 
// Author:������
//
//=====================================
#include "player.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "score.h"
#include "effect.h"
#include "camera.h"
#include "model.h"
#include "motion.h"
#include "object3D.h"
#include "camera.h"
#include "field.h"
#include "deliveryarrow.h"
#include "scene.h"
#include "particle.h"
#include "chain.h"
#include "sound.h"
#include "combo.h"
#include "collision.h"
#include "debugproc.h"
#include "orbit.h"
#include "useful.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

//�ÓI�����o�ϐ��錾-------------------
int CPlayer::m_nNumModel = 0;
CModel *CPlayer::m_apModelOrigin[32] = {};
CMotion *CPlayer::m_pMotionOrigin = NULL;

using namespace PlayerSpeed;

//=====================================
// �R���X�g���N�^�E�f�X�g���N�^
//=====================================
CPlayer::CPlayer(int nPriority = 4) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.785f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.785f, 0.0f);
	m_rotMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecWall = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bSave = false;
	m_nEasterTimer = 0;
	m_pMotion = nullptr;
	m_orbit[0] = nullptr;
	m_orbit[1] = nullptr;
	m_Arrow = nullptr;
	m_Hook = nullptr;
	m_bAir = true;
	m_bShot = false;
	m_bDash = false;
	m_bWall = false;
	m_bTurn = false;
	m_bBoost = false;
	m_bControl = false;
	m_nShotTimer = 0;
	m_nPalTimer = 0;
	m_state = STATE_NORMAL;
	m_nCombo = 0;
	m_nEnergy = 0;
	m_nHookSoundTimer = 0;
	m_nWindCounter = 0;
	m_Speed = 0.0f;
	m_SpeedDest = 0.0f;
	m_fHue = 0.0f;

	for (int nCntModel = 0; nCntModel < 32; nCntModel++)
	{
		m_apModel[nCntModel] = nullptr;
	}
}

CPlayer::~CPlayer()
{
}

//=====================================
// ��������
//=====================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pObjectPlayer;

	//2D�I�u�W�F�N�g�̐���
	pObjectPlayer = new CPlayer();

	if (pObjectPlayer != nullptr)
	{
		//������
		if (FAILED(pObjectPlayer->Init()))
		{
			pObjectPlayer->Release();

			return nullptr;
		}

		pObjectPlayer->Set(pos, rot, 0.0f, 0.0f);
	}

	return pObjectPlayer;
}

//=====================================
// �|���S���̃e�N�X�`������
//=====================================
HRESULT CPlayer::Load(void)
{
	ReadFilePlayer();

	return S_OK;
}
void CPlayer::Unload(void)
{
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (m_apModelOrigin[nCnt] != nullptr)
		{
			m_apModelOrigin[nCnt]->Uninit();

			delete m_apModelOrigin[nCnt];
			m_apModelOrigin[nCnt] = nullptr;
		}
	}

	if (m_pMotionOrigin != nullptr)
	{
		delete m_pMotionOrigin;
		m_pMotionOrigin = nullptr;
	}
}

//=====================================
// �v���C���[�̏���������
//=====================================
HRESULT CPlayer::Init(void)
{
	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		m_apModel[nCnt] = m_apModelOrigin[nCnt];
	}

	if (m_pMotionOrigin != nullptr)
	{
		m_pMotion = m_pMotionOrigin;
		m_pMotion->Set(MOTION_NORMAL);
	}

	SetType(TYPE_PLAYER);
	SetCollider(CCollider::Create(&m_pos, &m_rot, D3DXVECTOR3(100.0f, 100.0f, 100.0f), D3DXVECTOR3(-100.0f, -5.0f, -100.0f), CCollider::TAG_PLAYER));
	m_nEnergy = 10;
	m_rotDest.z = -2.335f;

	D3DXMATRIX mtxTemp = GetModel(16)->GetMtxWorld();
	mtxTemp._41 = 0.0f;
	mtxTemp._42 = 0.0f;
	mtxTemp._43 = 0.0f;
	m_orbit[0] = COrbit::Create(mtxTemp, D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(-2.0f, -0.0f, -0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100);

	mtxTemp = GetModel(17)->GetMtxWorld();
	mtxTemp._41 = 0.0f;
	mtxTemp._42 = 0.0f;
	mtxTemp._43 = 0.0f;
	m_orbit[1] = COrbit::Create(mtxTemp, D3DXVECTOR3(2.0f, 0.0f, 0.0f), D3DXVECTOR3(-2.0f, -0.0f, -0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100);

	/*mtxTemp = GetModel(16)->GetMtxWorld();
	m_orbit[2] = COrbit::Create(mtxTemp, D3DXVECTOR3(3.0f, 0.0f, 0.0f), D3DXVECTOR3(-3.0f, -0.0f, -0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100);

	mtxTemp = GetModel(17)->GetMtxWorld();
	m_orbit[3] = COrbit::Create(mtxTemp, D3DXVECTOR3(3.0f, 0.0f, 0.0f), D3DXVECTOR3(-3.0f, -0.0f, -0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 100);*/

	m_Arrow = CDeliveryarrow::Create(GetPos(), GetRot(), 0.0f, 0.0f);

	m_Hook = CHook::Create(GetPos(), GetRot());

	return S_OK;
}

//=====================================
// �v���C���[�̏I������
//=====================================
void CPlayer::Uninit(void)
{
	if (m_pMotion != nullptr)
	{
		m_pMotion = nullptr;
	}

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel] = nullptr;
	}

	Release();
}

//=====================================
// �v���C���[�̍X�V����
//=====================================
void CPlayer::Update(void)
{
	CInput *input = CManager::Get()->Get()->GetInputKeyboard();
	CInput *inputPad = CManager::Get()->Get()->GetInputPad();
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = pos;
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	float fHeight = GetHeight();
	float fWidth = GetWidth();
	m_bShot = false;

	if (m_bControl)
	{
		Control(&pos, &posOld, &rot, &move, &fHeight, &fWidth);

		if (inputPad != nullptr)
		{
			ControlPad(&pos, &posOld, &rot, &move, &fHeight, &fWidth);
		}
	}
	else
	{
		m_SpeedDest = 0.0f;
	}

	if (m_bAir && !m_bWall)
	{
		move.y -= 0.65f;
	}

	ControlMove(&pos, &posOld, &rot, &move, &fHeight, &fWidth);

	if (m_state == STATE_KICK)
	{
		rot.x -= 2.0f;

		if (m_pMotion->IsFinish())
		{
			move *= 0.0f;
		}
	}

	pos += move;

	if (m_state != STATE_KICK)
	{
		move.x += (0 - move.x) * 0.15f;
		move.z += (0 - move.z) * 0.15f;

		rot.x = 0.0f;
	}

	if (m_state == STATE_DAMAGE)
	{
		m_nEasterTimer++;

		if (m_nEasterTimer > 180)
		{
			m_state = STATE_NORMAL;
			m_nEasterTimer = 0;
		}
	}

	if (m_state == STATE_HIT)
	{
		m_nEasterTimer++;

		if (m_nEasterTimer > 10)
		{
			m_state = STATE_NORMAL;
			m_nEasterTimer = 0;
		}

		move.y -= 1.0f;
	}

	if (pos.y <= 0.0f)
	{
		pos.y = 0.0f;
		move.y = 0.0f;
		m_bAir = false;
		m_bWall = false;

		if (m_bBoost)
		{
			m_bBoost = false;
			m_bControl = true;
			m_SpeedDest = SPEED_BOOST;
			CSound::PlaySound(CSound::SOUND_LABEL_SE_BOOST);
		}
	}
	else
	{
		if (!m_bWall)
		{
			m_bAir = true;
		}

		if (pos.y <= -300.0f)
		{
			pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}

	if (m_state != STATE_HIT)
	{
		m_nPalTimer--;

		if (m_nPalTimer < 0)
		{
			m_nPalTimer = 0;
		}

		Collision(&pos, &posOld, &move);
	}

	if (pos.x < -4000.0f)
	{
		pos.x = -4000.0f;
	}
	if (pos.z < -4000.0f)
	{
		pos.z = -4000.0f;
	}

	if (pos.x > 76000.0f)
	{
		pos.x = 76000.0f;
	}
	if (pos.z > 76000.0f)
	{
		pos.z = 76000.0f;
	}

	if (CManager::Get()->Get()->GetScene()->GetField() != nullptr)
	{
		pos.y = CManager::Get()->Get()->GetScene()->GetField()->GetColHeight(pos, posOld, &move);
	}

	CManager::Get()->Get()->GetDebugProc()->Print("�v���C���[��pos: %f, %f, %f\n", pos.x, pos.y, pos.z);
	CManager::Get()->Get()->GetDebugProc()->Print("�v���C���[��move: %f, %f, %f\n", move.x, move.y, move.z);

	if (!m_bWall)
	{
		SetRot(&rot);
	}
	else
	{
		rot.y = atan2f(m_vecWall.x, m_vecWall.z) + D3DX_PI;
		m_rotDest.z = atan2f(m_vecWall.x, m_vecWall.z) + D3DX_PI;
		m_rotMove.y = atan2f(m_vecWall.x, m_vecWall.z) + D3DX_PI;
	}

	SetPos(pos);
	SetRot(rot);
	SetMove(move);
	SetHeight(fHeight);
	SetWidth(fWidth);

	D3DXVECTOR3 posEffect;

	CManager::Get()->Get()->GetScene()->GetCamera()->SetPos(pos);

	ControlMotion(move);

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Update();
	}

	m_pMotion->Update();
}

//=====================================
// �v���C���[�̕`�揈��
//=====================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::Get()->Get()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;		//�v�Z�p�}�g���b�N�X
	int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if ((m_nEasterTimer / 10) % 2 == 0 || m_state != STATE_DAMAGE)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			if (nCntModel == 16 || nCntModel == 17)
			{
				D3DXMATERIAL *pMat;
				//�}�e���A���̃f�[�^�̃|�C���g���擾
				pMat = (D3DXMATERIAL*)m_apModel[nCntModel]->GetBuffMat()->GetBufferPointer();

				if (combo > 5)
				{
					pMat[0].MatD3D.Diffuse = useful::HSLtoRGB(m_fHue);
					pMat[0].MatD3D.Emissive = useful::HSLtoRGB(m_fHue);
				}
				else
				{
					pMat[0].MatD3D.Diffuse = useful::HSLtoRGB(20.0f);
					pMat[0].MatD3D.Emissive = useful::HSLtoRGB(20.0f);
				}
			}

			m_apModel[nCntModel]->Draw();
		}
	}

	m_Hook->Draw();

	if (combo > 5)
	{
		if (m_orbit[0] != nullptr)
		{
			D3DXMATRIX mtxTemp = GetModel(16)->GetMtxWorld();
			m_orbit[0]->SetPositionOffset(mtxTemp, useful::HSLtoRGB(m_fHue));
		}

		if (m_orbit[1] != nullptr)
		{
			D3DXMATRIX mtxTemp = GetModel(17)->GetMtxWorld();
			m_orbit[1]->SetPositionOffset(mtxTemp, useful::HSLtoRGB(m_fHue));
		}

		m_fHue += 4.0f;
	}
	else
	{
		if (m_orbit[0] != nullptr)
		{
			D3DXMATRIX mtxTemp = GetModel(16)->GetMtxWorld();
			m_orbit[0]->SetPositionOffset(mtxTemp, useful::HSLtoRGB(20.0f));
		}

		if (m_orbit[1] != nullptr)
		{
			D3DXMATRIX mtxTemp = GetModel(17)->GetMtxWorld();
			m_orbit[1]->SetPositionOffset(mtxTemp, useful::HSLtoRGB(20.0f));
		}
	}
}

//=====================================
// �v���C���[�̑��쏈��
//=====================================
void CPlayer::Control(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth)
{
	CInput *input = CManager::Get()->Get()->GetInputKeyboard();
	CInput *inputMouse = CManager::Get()->Get()->GetInputMouse();
	CInput *inputPad = CManager::Get()->Get()->GetInputPad();

	D3DXVECTOR3 rotCamera = CManager::Get()->Get()->GetScene()->GetCamera()->GetRot();
	D3DXVECTOR3 rotCameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotCameraDest.z = atan2f(CManager::Get()->Get()->GetScene()->GetCamera()->GetPosV().z - CManager::Get()->Get()->GetScene()->GetCamera()->GetPosR().z, CManager::Get()->Get()->GetScene()->GetCamera()->GetPosV().x - CManager::Get()->Get()->GetScene()->GetCamera()->GetPosR().x);

	float RotKey = 0.0f;
	int NumInputKey = 0;

	if (inputPad != nullptr)
	{
		if (inputPad->GetAll())
		{
			return;
		}
	}

	if (!m_Hook->GetBoolCollision())
	{
		//�ړ�����
		if (input->GetPress(DIK_A) == true)
		{//D�L�[�������ꂽ��

			RotKey -= D3DX_PI * 0.5f;
			NumInputKey++;

		}
		if (input->GetPress(DIK_W) == true)
		{//A�L�[�������ꂽ��

			if (NumInputKey > 0)
			{
				RotKey -= D3DX_PI * 1.0f;
			}
			else
			{
				RotKey += D3DX_PI * 1.0f;
			}

			NumInputKey++;

		}
		if (input->GetPress(DIK_S) == true)
		{//D�L�[�������ꂽ��

			RotKey += D3DX_PI * 0.0f;
			NumInputKey++;

		}
		if (input->GetPress(DIK_D) == true)
		{//D�L�[�������ꂽ��

			RotKey += D3DX_PI * 0.5f;
			NumInputKey++;

		}

		if (NumInputKey > 0)
		{//A�L�[�������ꂽ��

			if (NumInputKey > 1)
			{
				RotKey *= 0.5f;
			}

			if (input->GetPress(DIK_LSHIFT))
			{
				if (m_SpeedDest < SPEED_DASH)
				{
					m_SpeedDest += SPEED_DECELERATION;
				}
				else
				{
					m_SpeedDest = SPEED_DASH;
				}

				m_rotDest.z = rotCamera.y - RotKey;

				m_bDash = true;
			}
			else
			{
				if (m_SpeedDest < SPEED_WALK)
				{
					m_SpeedDest += SPEED_DECELERATION;
				}
				else
				{
					m_SpeedDest = SPEED_WALK;
				}

				m_rotDest.z = rotCamera.y - RotKey;

				m_bDash = false;
				m_bTurn = false;
			}
		}
		else
		{
			m_SpeedDest = 0.0f;
			//m_bWall = false;
			m_bDash = false;
			m_bTurn = false;
		}

		if (inputMouse->GetTrigger(1) == true && m_bAir == false && m_Speed >= SPEED_EFFECT_BOOST)
		{
			m_bTurn = true;
		}

		if (inputMouse->GetPress(1) == true && m_bAir == false && m_bTurn)
		{
			if (m_bDash)
			{
				m_SpeedDest = SPEED_TURN;

				m_bDash = false;
			}
		}
		else
		{
			if (m_bDash && m_bAir == false && m_bTurn)
			{
				m_SpeedDest = SPEED_TURN;

				m_bDash = false;

				if (m_Speed >= SPEED_BOOSTON)
				{
					m_SpeedDest = SPEED_BOOST;
					m_bTurn = false;
					m_bBoost = true;

					CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(1);
					int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
					CManager::Get()->Get()->GetScene()->GetScore()->AddScore(1000 * combo);

					CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
					CSound::PlaySound(CSound::SOUND_LABEL_SE_BOOST);
				}
			}
			else
			{
				m_rotMove = *rot;
			}
		}

		if (input->GetTrigger(DIK_SPACE) == true)
		{
			if (m_bAir)
			{
				move->y = -150.0f; 
			}
			else
			{
				move->y = SPEED_JUMP;
				m_bTurn = false;

				if (m_bWall)
				{
					if (m_SpeedDest <= SPEED_DASH)
					{
						move->y = 50.0f;
					}
					else if (m_SpeedDest <= SPEED_WALK)
					{
						move->y = 15.0f;
					}
					else
					{
						move->y = 5.0f;
					}

					m_SpeedDest = -10.0f;
					rot->y = atan2f(m_vecWall.x, m_vecWall.z);
					m_rotDest.z = atan2f(m_vecWall.x, m_vecWall.z);
					m_rotMove.y = atan2f(m_vecWall.x, m_vecWall.z);

					move->x += sinf(m_rotMove.y) * m_SpeedDest;
					move->z += cosf(m_rotMove.y) * m_SpeedDest;

					D3DXVECTOR3 posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					posDest.x = sinf(rot->y) * m_SpeedDest;
					posDest.z = cosf(rot->y) * m_SpeedDest;

					*pos += posDest;

					m_bWall = false;

					CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(1);
					int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
					CManager::Get()->Get()->GetScene()->GetScore()->AddScore(300 * combo);

					CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
				}

				if (m_SpeedDest <= SPEED_MAX && m_bBoost)
				{
					m_SpeedDest += SPEED_DASH;
					m_bBoost = false;
				}
			}
		}

		if (inputMouse->GetTrigger(0) == true && !m_Hook->GetBoolShot() && !m_bWall)
		{
			m_Hook->ShotHook(D3DXVECTOR3(rot->x, m_rotDest.z, rot->z), 700.0f);
		}

		if (m_nHookSoundTimer >= 0)
		{
			CSound::StopSound(CSound::SOUND_LABEL_SE_CHAIN);
			CSound::PlaySound(CSound::SOUND_LABEL_SE_CHAINJUMP);
			m_nHookSoundTimer = -1;
		}
	}
	else
	{
		D3DXVECTOR3 vecMove = *pos - m_Hook->GetPos();
		D3DXVec3Normalize(&vecMove, &vecMove);
		m_rotDest.z = atan2f(vecMove.x, vecMove.z);
		rot->y = m_rotDest.z;
		m_SpeedDest = -30.0f;
		*move = vecMove * m_Speed * 10.0f;

		if (input->GetTrigger(DIK_SPACE) == true)
		{
			m_SpeedDest = 0.0f;
			m_Hook->SetBoolCollision(false);
			m_Hook->SetBoolShot(false);
			move->y = 50.0f;
		}

		if (m_pMotion->GetKey() == 0)
		{
			if (m_nHookSoundTimer <= 0)
			{
				CSound::PlaySound(CSound::SOUND_LABEL_SE_CHAIN);
				m_nHookSoundTimer = 3;
			}

			m_nHookSoundTimer -= 1;
		}
	}
}

//=====================================
// �v���C���[�̃p�b�h���쏈��
//=====================================
void CPlayer::ControlPad(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth)
{
	CInput *input = CManager::Get()->Get()->GetInputPad();
	CInput *inputKeyboard = CManager::Get()->Get()->GetInputKeyboard();
	CInput *inputMouse = CManager::Get()->Get()->GetInputMouse();

	D3DXVECTOR3 rotCamera = CManager::Get()->Get()->GetScene()->GetCamera()->GetRot();
	D3DXVECTOR3 rotCameraDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float RotStick = atan2f((float)input->GetLStickLRPress(), (float)input->GetLStickUDPress());
	float lengthStick = hypotf((float)input->GetLStickLRPress(), (float)input->GetLStickUDPress());

	if (lengthStick > 1000.0f)
	{
		lengthStick = 1000.0f;
	}

	CManager::Get()->Get()->GetDebugProc()->Print("L�X�e�B�b�N�̋���: %f\n", lengthStick);

	if (inputKeyboard->GetAll() || inputMouse->GetAll())
	{
		return;
	}

	if (!m_Hook->GetBoolCollision())
	{
		if (lengthStick > 10.0f)
		{
			if (input->GetButtonPress(4) == true)
			{
				if (m_SpeedDest < SPEED_DASH)
				{
					m_SpeedDest += SPEED_DECELERATION;
				}
				else
				{
					m_SpeedDest = SPEED_DASH;
				}

				m_rotDest.z = rotCamera.y - RotStick;

				m_bDash = true;
			}
			else
			{
				if (m_SpeedDest < SPEED_DASH)
				{
					m_SpeedDest += SPEED_DECELERATION * 5.0f;
				}
				else
				{
					m_SpeedDest = SPEED_WALK;
				}

				m_rotDest.z = rotCamera.y - RotStick;

				m_bDash = false;
				m_bTurn = false;
			}
		}
		else
		{
			m_SpeedDest = 0.0f;

			//m_bWall = false;
			m_bDash = false;
			m_bTurn = false;
		}

		if (input->GetButtonTrigger(5) == true && m_bAir == false && m_Speed >= SPEED_EFFECT_BOOST)
		{
			m_bTurn = true;
		}

		if (input->GetButtonPress(5) == true && m_bAir == false && m_bTurn)
		{
			if (m_bDash)
			{
				m_SpeedDest = SPEED_TURN;

				m_bDash = false;
			}
		}
		else
		{
			if (m_bDash && m_bAir == false && m_bTurn)
			{
				m_SpeedDest = SPEED_TURN;

				m_bDash = false;

				if (m_Speed >= SPEED_BOOSTON)
				{
					m_SpeedDest = SPEED_BOOST;
					m_bTurn = false;
					m_bBoost = true;

					CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(1);
					int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
					CManager::Get()->Get()->GetScene()->GetScore()->AddScore(1000 * combo);
				
					CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
					CSound::PlaySound(CSound::SOUND_LABEL_SE_BOOST);
				}
			}
			else
			{
				m_rotMove = *rot;
			}
		}

		if (input->GetButtonTrigger(2) == true)
		{
			if (m_bAir)
			{
				move->y = -250.0f;

				if (pos->y > 500.0f)
				{
					m_bBoost = true;
					m_bControl = false;
				}
			}
			else
			{
				move->y = SPEED_JUMP;
				m_bTurn = false;

				if (m_bWall)
				{
					if (m_SpeedDest <= SPEED_DASH)
					{
						move->y = 50.0f;
					}
					else if (m_SpeedDest <= SPEED_WALK)
					{
						move->y = 15.0f;
					}
					else
					{
						move->y = 5.0f;
					}

					m_SpeedDest = -10.0f;
					rot->y = atan2f(m_vecWall.x, m_vecWall.z);
					m_rotDest.z = atan2f(m_vecWall.x, m_vecWall.z);
					m_rotMove.y = atan2f(m_vecWall.x, m_vecWall.z);

					move->x += sinf(m_rotMove.y) * m_SpeedDest;
					move->z += cosf(m_rotMove.y) * m_SpeedDest;

					D3DXVECTOR3 posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					posDest.x = sinf(rot->y) * m_SpeedDest;
					posDest.z = cosf(rot->y) * m_SpeedDest;

					*pos += posDest;

					m_bWall = false;

					CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(1);
					int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
					CManager::Get()->Get()->GetScene()->GetScore()->AddScore(300 * combo);

					CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
				}

				if (m_SpeedDest <= SPEED_MAX && m_bBoost)
				{
					m_SpeedDest += SPEED_DASH;
					m_bBoost = false;
				}
			}
		}

		if (input->GetButtonTrigger(7) == true && !m_Hook->GetBoolShot() && !m_bWall)
		{
			m_Hook->ShotHook(D3DXVECTOR3(rot->x, m_rotDest.z, rot->z), 700.0f);
		}

		if (m_nHookSoundTimer >= 0)
		{
			CSound::StopSound(CSound::SOUND_LABEL_SE_CHAIN);
			CSound::PlaySound(CSound::SOUND_LABEL_SE_CHAINJUMP);
			m_nHookSoundTimer = -1;
		}
	}
	else
	{
		D3DXVECTOR3 vecMove = *pos - m_Hook->GetPos();
		D3DXVec3Normalize(&vecMove, &vecMove);
		m_rotDest.z = atan2f(vecMove.x, vecMove.z);
		rot->y = m_rotDest.z;
		m_SpeedDest = -30.0f;
		*move = vecMove * m_Speed * 10.0f;

		if (input->GetButtonTrigger(2) == true)
		{
			m_SpeedDest = 0.0f;
			m_Hook->SetBoolCollision(false);
			m_Hook->SetBoolShot(false);
			move->y = 50.0f;
		}

		if (m_pMotion->GetKey() == 0)
		{
			if (m_nHookSoundTimer <= 0)
			{
				CSound::PlaySound(CSound::SOUND_LABEL_SE_CHAIN);
				m_nHookSoundTimer = 3;
			}

			m_nHookSoundTimer -= 1;
		}
	}
}

//=====================================
// �v���C���[�̈ړ���������
//=====================================
void CPlayer::ControlMove(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *rot, D3DXVECTOR3 *move, float *fHeight, float *fWidth)
{
	float speed;

	if (m_bTurn)
	{
		m_Speed += (m_SpeedDest - m_Speed) * 0.05f;
	}
	else
	{
		if (m_bDash)
		{
			m_Speed += (m_SpeedDest - m_Speed) * 0.1f;
		}
		else
		{
			m_Speed += (m_SpeedDest - m_Speed) * 0.05f;
		}
	}

	if (m_Speed < SPEED_MAX)
	{
		speed = SPEED_MAX;
	}
	else
	{
		speed = m_Speed;
	}

	if (m_bWall)
	{
		speed = 0.0f;
	}

	CManager::Get()->Get()->GetDebugProc()->Print("�ڕW���x: %f\n", m_SpeedDest);
	CManager::Get()->Get()->GetDebugProc()->Print("���ݑ��x: %f\n", m_Speed);
	CManager::Get()->Get()->GetDebugProc()->Print("�������x: %f\n", speed);

	move->x += sinf(m_rotMove.y) * speed;
	move->z += cosf(m_rotMove.y) * speed;

	if (speed <= SPEED_EFFECT || m_Hook->GetBoolCollision() || (m_bBoost && !m_bControl))
	{
		D3DXVECTOR3 posEffectOffset = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
		//posEffectOffset = useful::PosRelativeMtx(posEffectOffset, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		for (int nCntEff = 0; nCntEff < 5; nCntEff++)
		{
			float randRot = (float)(rand() % 629 - 314) * 0.01f;
			D3DXVECTOR3 posEffect = useful::PosRelativeMtx(posEffectOffset, D3DXVECTOR3(0.0f, D3DX_PI, randRot + (D3DX_PI * -0.0f)), D3DXVECTOR3(((float)(rand() % 300) + 210.0f + (speed * -3.0f)), 0.0f, 0.0f));

			if (speed >= SPEED_EFFECT_BOOST)
			{
				CEffect2D::Create(posEffect, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, randRot), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 400.0f + (speed * -3.0f), 15.0f);
				break;
			}
			else
			{
				CEffect2D::Create(posEffect, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, randRot), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 400.0f + (speed * -3.0f), 15.0f);
				//CEffect2D::Create(posEffect, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, randRot), useful::HSLtoRGB((float)(rand() % 360)), 20, 100.0f, 5.0f);
			}
		}

		if (!m_bAir || m_Hook->GetBoolCollision())
		{
			int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
			CManager::Get()->Get()->GetScene()->GetScore()->AddScore((int)(0.2f * -speed) * combo);
		}

		if (m_bBoost && !m_bControl)
		{
			int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
			CManager::Get()->Get()->GetScene()->GetScore()->AddScore((int)(0.2f * 30.0f) * combo);
		}
	}
}

//=====================================
// �v���C���[�̃��[�V������������
//=====================================
void CPlayer::ControlMotion(D3DXVECTOR3 move)
{
	if (m_bAir)
	{
		if (m_Hook->GetBoolCollision())
		{
			m_pMotion->Set(MOTION_HOOK);
		}
		else
		{
			if (m_bWall)
			{
				m_pMotion->Set(MOTION_WALL);
			}
			else
			{
				if (m_bBoost)
				{
					m_pMotion->Set(MOTION_DROP);
				}
				else
				{
					m_pMotion->Set(MOTION_AIR);
				}
			}
		}
	}
	else
	{
		if (m_bTurn)
		{
			m_pMotion->Set(MOTION_TURN);
		}
		else
		{
			if (m_bWall)
			{
				m_pMotion->Set(MOTION_WALL);
			}
			else
			{
				if (m_Speed < SPEED_EFFECT)
				{
					m_pMotion->Set(MOTION_DASH);
				}
				else if (m_Speed < SPEED_TURN)
				{
					m_pMotion->Set(MOTION_MOVE);
				}
				else
				{
					m_pMotion->Set(MOTION_NORMAL);
				}
			}
		}
	}

	if (m_pMotion->GetType() == MOTION_MOVE)
	{
		if (m_pMotion->GetKey() == 0 || m_pMotion->GetKey() == 2)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_SE_DASH);
		}
	}

	if (m_pMotion->GetType() == MOTION_DASH)
	{
		if (m_pMotion->GetKey() == 0 || m_pMotion->GetKey() == 2)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_SE_DASH);
		}
	}

	if (m_Speed <= SPEED_EFFECT_BOOST || GetMove().y <= -200.0f)
	{
		if (m_nWindCounter != 1)
		{
			CSound::StopSound(CSound::SOUND_LABEL_SE_WIND_WALK);
			CSound::PlaySound(CSound::SOUND_LABEL_SE_WIND_DASH);
			m_nWindCounter = 1;
		}
	}
	else if (m_Speed <= SPEED_EFFECT && !m_bWall)
	{
		if (m_nWindCounter != 2)
		{
			CSound::PlaySound(CSound::SOUND_LABEL_SE_WIND_WALK);
			CSound::StopSound(CSound::SOUND_LABEL_SE_WIND_DASH);
			m_nWindCounter = 2;
		}
	}
	else if(m_Speed > SPEED_EFFECT)
	{
		if (m_nWindCounter != 0)
		{
			CSound::StopSound(CSound::SOUND_LABEL_SE_WIND_WALK);
			CSound::StopSound(CSound::SOUND_LABEL_SE_WIND_DASH);
			m_nWindCounter = 0;
		}
	}
}

//=====================================
// �v���C���[�̊p�x��������
//=====================================
void CPlayer::SetRot(D3DXVECTOR3 *rot)
{
	float fRotMove, fRotDest, fRotDiff;

	//���݂̊p�x�ƖړI�̊p�x�̍������v�Z
	fRotMove = rot->y;
	fRotDest = m_rotDest.z;
	fRotDiff = fRotDest - fRotMove;

	if (fRotDiff > 3.14f)
	{
		fRotDiff -= 6.28f;
	}
	else if (fRotDiff <= -3.14f)
	{
		fRotDiff += 6.28f;
	}

	//���X�ɑ����Ă�
	if (m_bDash)
	{
		if (m_Speed > SPEED_EFFECT_BOOST)
		{
			fRotMove += fRotDiff * 0.02f;
		}
		else
		{
			fRotMove += fRotDiff * 0.008f;
		}
	}
	else
	{
		if (m_Speed > SPEED_WALK * 2.0f)
		{
			fRotMove += fRotDiff * 0.1f;
		}
		else if (m_Speed > SPEED_EFFECT_BOOST)
		{
			fRotMove += fRotDiff * 0.02f;
		}
		else
		{
			fRotMove += fRotDiff * 0.008f;
		}
	}

	if (fRotMove > 3.14f)
	{
		fRotMove -= 6.28f;
	}
	else if (fRotMove <= -3.14f)
	{
		fRotMove += 6.28f;
	}

	rot->y = fRotMove;
}

//=====================================
// �v���C���[�̓����蔻�菈��
//=====================================
bool CPlayer::Collision(D3DXVECTOR3 *pos,D3DXVECTOR3 *posOld, D3DXVECTOR3 *move)
{
	CCollider *pCollider;

	pCollider = CCollision::Get()->GetTop();

	while (pCollider != NULL)
	{
		CCollider *pColliderNext = pCollider->GetNext();

		CCollider::TAG tag;

		//��ގ擾
		tag = pCollider->GetTag();

		bool bColAir = false;
		if (m_bAir && m_Hook->GetBoolCollision())
		{
			bColAir = true;
		}

		if (tag == CCollider::TAG_BOX)
		{
			if (pCollider->CollisionSquare(pos, *posOld, move, bColAir, &m_vecWall) == true)
			{
				move->y = -1.0f;
				if (m_vecWall.y > -0.8f && m_bAir)
				{
					m_bWall = true;
					move->y = 0.0f;
					m_Hook->SetBoolCollision(false);
					m_Hook->SetBoolShot(false);
				}

				if (bColAir)
				{
					m_SpeedDest = SPEED_DASH;
				}

				m_bAir = false;
				m_Hook->SetBoolGet(true);

				if (m_bBoost)
				{
					m_bBoost = false;
					m_bControl = true;
					m_SpeedDest = SPEED_BOOST;
					CSound::PlaySound(CSound::SOUND_LABEL_SE_BOOST);
				}
			}
		}

		if (tag == CCollider::TAG_CAR)
		{
			if (m_bDash && m_bAir && !m_Hook->GetBoolCollision())
			{
				if (pCollider->CollisionSquareTrigger(*pos) == true && m_nPalTimer == 0)
				{
					move->y = 30.0f;
					m_bBoost = false;
					m_bControl = true;
					m_SpeedDest = SPEED_BOOST;

					CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(1);
					int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
					CManager::Get()->Get()->GetScene()->GetScore()->AddScore(500 * combo);
					
					CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
					m_nPalTimer = 30;
				}
			}
			else
			{
				if (pCollider->CollisionSquare(pos, *posOld, move, bColAir, &m_vecWall) == true)
				{
					move->y = -1.0f;
					if (m_vecWall.y > -0.8f && m_bAir)
					{
						m_bWall = true;
						move->y = 0.0f;
						m_Hook->SetBoolCollision(false);
						m_Hook->SetBoolShot(false);
					}

					if (bColAir)
					{
						m_SpeedDest = SPEED_DASH;
					}

					m_bAir = false;
					m_Hook->SetBoolGet(true);

					if (m_bBoost)
					{
						m_bBoost = false;
						m_bControl = true;
						m_SpeedDest = SPEED_BOOST;
						CSound::PlaySound(CSound::SOUND_LABEL_SE_BOOST);
					}
				}
			}
		}

		if (tag == CCollider::TAG_DRONE)
		{
			if (pCollider->CollisionSquareTrigger(*pos) == true && m_nPalTimer == 0)
			{
				move->y = 100.0f;
				m_bBoost = false;
				m_bControl = true;

				CManager::Get()->Get()->GetScene()->GetCombo()->AddCombo(1);
				int combo = CManager::Get()->Get()->GetScene()->GetCombo()->GetCombo();
				CManager::Get()->Get()->GetScene()->GetScore()->AddScore(500 * combo);

				CSound::PlaySound(CSound::SOUND_LABEL_SE_SCORE);
				m_nPalTimer = 30;
			}
		}

		pCollider = pColliderNext;
	}

	return false;
}

//===========================
//�v���C���[�̃��[�V�����t�@�C���ǂݍ��ݏ���
//===========================
void CPlayer::ReadFilePlayer(void)
{
	char aText[128];
	//int nTrash;
	float fTrash;
	int nParent;
	D3DXVECTOR3 pos, rot;
	CMotion::Info info;
	m_pMotionOrigin = new CMotion;

	if (m_pMotionOrigin != nullptr)
	{
		m_pMotionOrigin->Init();
	}

	memset(&info, 0, sizeof(info));
	
	FILE *pFile = fopen("data\\MOTION\\motion_dessan_delivery.txt", "r");

	//�t�@�C���ǂݍ���
	if (pFile == nullptr)
	{
		return;
	}
	
	while (1)
	{
		fscanf(pFile, "%s", &aText[0]);

		if (strcmp(aText, "NUM_MODEL") == 0)
		{//�t�@�C�����ǂݍ���
		 //"="�ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�t�@�C�����擾
			fscanf(pFile, "%d", &m_nNumModel);

			break;
		}
	}

	for (int nCntFile = 0, nTemp = 0; nCntFile < m_nNumModel; nTemp++)
	{//�t�@�C�����ǂݍ���
	 //�����ǂݍ���
		fscanf(pFile, "%s", &aText[0]);
		
		if (strcmp(aText, "MODEL_FILENAME") == 0)
		{//�t�@�C�����ǂݍ���
		 //"="�ǂݍ���
			fscanf(pFile, "%s", &aText[0]);

			//�t�@�C�����擾
			fscanf(pFile, "%s", &aText[0]);

			m_apModelOrigin[nCntFile] = CModel::Create(&aText[0]);

			//�ǂݍ��񂾃��f�����J�E���^���Z
			nCntFile++;
		}
	}

	do
	{
		fgets(&aText[0], 128, pFile);
	} while (strcmp(aText, "CHARACTERSET\n") != 0);

	fscanf(pFile, "%s = %f	%s %s", &aText[0], &fTrash, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %f	%s %s", &aText[0], &fTrash, &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %s	%s %s", &aText[0], &aText[0], &aText[0], &aText[0]);
	fscanf(pFile, "%s = %d	%s %s\n", &aText[0], &m_nNumModel, &aText[0], &aText[0]);

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		fgets(&aText[0], 128, pFile);
		fscanf(pFile, "%s = %d", &aText[0], &nParent);
		fscanf(pFile, "%s = %d	%s %s", &aText[0], &nParent, &aText[0], &aText[0]);
		fscanf(pFile, "%s = %f %f %f", &aText[0], &pos.x, &pos.y, &pos.z);
		fscanf(pFile, "%s = %f %f %f\n", &aText[0], &rot.x, &rot.y, &rot.z);
		fgets(&aText[0], 128, pFile);
		fgets(&aText[0], 128, pFile);

		m_apModelOrigin[nCntModel]->SetPosDef(pos);
		m_apModelOrigin[nCntModel]->SetRotDef(rot);

		if (nParent == -1)
		{
			m_apModelOrigin[nCntModel]->SetParent(nullptr);
		}
		else
		{
			m_apModelOrigin[nCntModel]->SetParent(m_apModelOrigin[nParent]);
		}
	}

	for (int nCnt = 0; nCnt < 7; nCnt++)
	{
		fgets(&aText[0], 128, pFile);
	}
	
	if (m_pMotionOrigin != nullptr)
	{
		m_pMotionOrigin->SetModel(&m_apModelOrigin[0], m_nNumModel);
	}

	for (int nCntMotion = 0, nTemp = 0; nCntMotion < MOTION_MAX; nTemp++)
	{
		fscanf(pFile, "%s", &aText[0]);

		if (strcmp(aText, "MOTIONSET") == 0)
		{
			int nCntKey = 0;
			do
			{
				fscanf(pFile, "%s", &aText[0]);

				if (strcmp(aText, "LOOP") == 0)
				{
					fscanf(pFile, "	=	%d", &info.nLoop);
				}

				if (strcmp(aText, "NUM_KEY") == 0)
				{
					fscanf(pFile, "	=	%d", &info.nNumKey);
				}

				if (strcmp(aText, "KEYSET") == 0)
				{
					fgets(&aText[0], 128, pFile);

					do
					{
						fscanf(pFile, "%s", &aText[0]);

						if (strcmp(aText, "KEYSET") == 0)
						{

						}

						if (strcmp(aText, "FRAME") == 0)
						{
							fscanf(pFile, "	=	%d", &info.aKeyInfo[nCntKey].nFrame);
						}

						if (strcmp(aText, "KEY") == 0)
						{
							for (int nCntModel = 0, nTemp = 0; nCntModel < m_nNumModel; nTemp++)
							{
								do
								{
									fscanf(pFile, "%s", &aText[0]);

									if (strcmp(aText, "POS") == 0)
									{
										fscanf(pFile, "	=	%f	%f	%f", &info.aKeyInfo[nCntKey].aKey[nCntModel].pos.x, &info.aKeyInfo[nCntKey].aKey[nCntModel].pos.y, &info.aKeyInfo[nCntKey].aKey[nCntModel].pos.z);
									}

									if (strcmp(aText, "ROT") == 0)
									{
										fscanf(pFile, "	=	%f	%f	%f", &info.aKeyInfo[nCntKey].aKey[nCntModel].rot.x, &info.aKeyInfo[nCntKey].aKey[nCntModel].rot.y, &info.aKeyInfo[nCntKey].aKey[nCntModel].rot.z);
									}
								} while (strcmp(aText, "END_KEY") != 0);

								nCntModel++;
							}
						}
					} while (strcmp(aText, "END_KEYSET") != 0);

					nCntKey++;
				}
			} while (strcmp(aText, "END_MOTIONSET") != 0);
			
			if (m_pMotionOrigin != nullptr)
			{
				m_pMotionOrigin->SetInfo(info);
			}

			nCntMotion++;
		}
	}

	fclose(pFile);
}

//=====================================
// �|���S���̐ݒ菈��
//=====================================
void  CPlayer::Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight)
{
	m_pos = pos;
	m_rot = rot;
}