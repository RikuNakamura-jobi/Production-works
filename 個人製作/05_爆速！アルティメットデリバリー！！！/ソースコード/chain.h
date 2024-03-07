//=====================================
//
// chain�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _CHAIN_H_
#define _CHAIN_H_

#include "main.h"
#include "objectX.h"
#include <map>

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CModel;
class COrbit;
class CHook : public CObjectX
{
public:				//�O������A�N�Z�X�\

	//�R���X�g���N�^�E�f�X�g���N�^
	CHook(int nPriority);				//�f�t�H���g
	~CHook();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	bool GetBoolShot(void) { return m_bShot; }
	void SetBoolShot(bool Shot) { m_bShot = Shot; }

	bool GetBoolGet(void) { return m_bGet; }
	void SetBoolGet(bool get) { m_bGet = get; }

	bool GetBoolCollision(void) { return m_bCollision; }
	void SetBoolCollision(bool Collision) { m_bCollision = Collision; }

	void ShotHook(D3DXVECTOR3 rot, float speed);

	//�ÓI�����o�֐�
	static CHook *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�
	bool Collision(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);
	bool CollisionReticle(D3DXVECTOR3 *pos, D3DXVECTOR3 *move);

	//�����o�ϐ�
	MODELX m_modelTemp;
	COrbit *m_orbit[4];
	D3DXVECTOR3 m_posOld;

	bool m_bShot;
	bool m_bGet;
	bool m_bCollision;

	//�ÓI�����o�ϐ�
	static MODELX m_model;

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_CHAIN_H_