//=============================================================================
//
// 
// Author :������
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "main.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------
class CCollider;
class CCollision
{
public:

	CCollision();
	~CCollision();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	CCollider *GetTop(void) { return m_pTop; }
	void SetTop(CCollider *collider) { m_pTop = collider; }

	CCollider *GetCur(void) { return m_pCur; }
	void SetCur(CCollider *collider) { m_pCur = collider; }

	int GetNumAll(void) { return m_nNumAll; }
	void SetNumAll(int nNumAll) { m_nNumAll = nNumAll; }

	//�ÓI�����o�֐�
	static CCollision *Get(void);
	static HRESULT Release(void);

private:

	//�����o�֐�

	//�����o�ϐ�
	CCollider *m_pTop;
	CCollider *m_pCur;

	int m_nNumAll;						//�I�u�W�F�N�g����

	//�ÓI�����o�ϐ�
	static CCollision *m_pCollision;

};

class CCollider
{
public:

	enum TYPE
	{
		TYPE_NONE = 0,		//�^�C�v����
		TYPE_POINT,			//�_
		TYPE_BOX,			//�{�b�N�X
		TYPE_MAX
	};

	enum TAG
	{
		TAG_NONE = 0,		//�^�C�v����
		TAG_PLAYER,			//�v���C���[
		TAG_BOX,			//�{�b�N�X
		TAG_CAR,			//�{�b�N�X
		TAG_DRONE,			//�{�b�N�X
		TAG_MAX
	};

	CCollider();
	~CCollider();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	CCollider *GetPrev(void) { return m_pPrev; }
	void SetPrev(CCollider *collider) { m_pPrev = collider; }

	CCollider *GetNext(void) { return m_pNext; }
	void SetNext(CCollider *collider) { m_pNext = collider; }

	bool GetDeath(void) { return m_bDeath; }
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }

	TAG GetTag(void) { return m_tag; }
	void SetTag(TAG tag) { m_tag = tag; }

	void SetType(TYPE type) { m_type = type; }

	bool CollisionSquare(D3DXVECTOR3 *posTarget, D3DXVECTOR3 posTargetOld, D3DXVECTOR3 *move, bool bStop = false, D3DXVECTOR3 *vecNor = nullptr);	//���̓����蔻��(�����߂�����)
	bool CollisionSquareTrigger(D3DXVECTOR3 posTarget);																			//���̓����蔻��(���O����̂�)

	//�ÓI�����o�֐�
	static CCollider *Create(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot, D3DXVECTOR3 offsetMax, D3DXVECTOR3 offsetMin, TAG tag);

private:

	//�����o�ϐ�
	D3DXVECTOR3 *m_pos;
	D3DXVECTOR3 *m_rot;
	D3DXVECTOR3 m_offsetMax;
	D3DXVECTOR3 m_offsetMin;

	int m_nID;									//�������g��ID
	TYPE m_type;
	TAG m_tag;
	bool m_bDeath;
	bool m_bMovable;
	bool m_bOnCollider;
	bool m_bTrigger;

	float m_nHue;

	CCollider *m_pPrev;
	CCollider *m_pNext;

	//�ÓI�����o�ϐ�

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_COLLISION_H_
