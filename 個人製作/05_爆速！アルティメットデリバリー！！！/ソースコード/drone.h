//=====================================
//
// drone�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _DRONE_H_
#define _DRONE_H_

#include "main.h"
#include "objectX.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CDrone : public CObjectX
{
public:				//�O������A�N�Z�X�\

	enum TYPE
	{
		TYPE_NONE = 0,			//�Ȃɂ����ĂȂ����
		TYPE_MAX
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CDrone(int nPriority);				//�f�t�H���g
	~CDrone();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	//�ÓI�����o�֐�
	static CDrone *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, CDrone::TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	MODELX m_modelTemp;

	float m_fHue;
	float m_fHueOrg;

	//�ÓI�����o�ϐ�
	static MODELX m_model[TYPE_MAX];
	static const char *m_apFilename[TYPE_MAX];			//�e�N�X�`���̖��O

	TYPE m_type;

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_DRONE_H_