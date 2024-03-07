//=====================================
//
// deliverypoint�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _DELIVERY_H_
#define _DELIVERY_H_

#include "main.h"
#include "objectX.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CDeliverypoint : public CObjectX
{
public:				//�O������A�N�Z�X�\

	//�R���X�g���N�^�E�f�X�g���N�^
	CDeliverypoint(int nPriority);				//�f�t�H���g
	~CDeliverypoint();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	//�ÓI�����o�֐�
	static CDeliverypoint *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�
	bool Collision(void);

	//�����o�ϐ�
	MODELX m_modelTemp;

	int m_nCntDelivery;
	float m_fHue;

	//�ÓI�����o�ϐ�
	static MODELX m_model;

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_DELIVERY_H_