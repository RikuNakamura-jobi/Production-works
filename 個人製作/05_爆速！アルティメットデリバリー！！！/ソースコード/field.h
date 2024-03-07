//=====================================
//
// bg�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "object3D.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CField : public CObject3D
{
public:				//�O������A�N�Z�X�\

	enum TYPE
	{
		TYPE_NONE = 0,				//�Ȃɂ����ĂȂ����
		TYPE_ROAD_CROSS,			//�t�F�[�h�C�����
		TYPE_ROAD_T,				//�t�F�[�h�C�����
		TYPE_ROAD_L,				//�t�F�[�h�C�����
		TYPE_ROAD_U,				//�t�F�[�h�C�����
		TYPE_ROAD_O,				//�t�F�[�h�C�����
		TYPE_ROAD_S,				//�t�F�[�h�C�����
		TYPE_RAND,					//�t�F�[�h�C�����
		TYPE_MAX
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CField(int nPriority);				//�f�t�H���g
	~CField();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CField::TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	int m_nHue;
	TYPE m_type;

	//�ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		//�e�N�X�`���ւ̃|�C���^
	static const char *m_apFilename[TYPE_MAX];			//�e�N�X�`���̖��O

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_FIELD_H_