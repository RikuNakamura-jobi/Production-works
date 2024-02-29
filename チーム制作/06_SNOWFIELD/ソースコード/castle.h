//============================================
//
// ��w�b�_�[[castle.h]
// Author�F��������
//
//============================================
#ifndef _CASTLE_H_					// ���̃}�N����`������Ă��Ȃ�������
#define _CASTLE_H_					// 2�d�C���N���[�h�h�~�̃}�N�����`����

//********************************************
// �C���N���[�h�t�@�C��
//********************************************
#include "model.h"

//--------------------------------------------
// �N���X(��N���X)
//--------------------------------------------
class CCastle : public CModel
{
public:			// �N�ł��A�N�Z�X�ł���

	CCastle();				// �R���X�g���N�^
	~CCastle();				// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);		// ����������
	void Uninit(void);		// �I������
	void Update(void);		// �X�V����
	void Draw(void);		// �`�揈��

	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);				// ���̐ݒ菈��

	// �ÓI�����o�֐�
	static CCastle* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);		// ��������

private:		// ���������A�N�Z�X�ł���

};

#endif