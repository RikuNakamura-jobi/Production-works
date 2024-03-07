//=====================================
//
// block�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectX.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CBlock : public CObjectX
{
public:				//�O������A�N�Z�X�\

	enum TYPE
	{
		TYPE_NONE = 0,			//�Ȃɂ����ĂȂ����
		TYPE_BOX,				//�t�F�[�h�C�����
		TYPE_BIGBUILDING,		//�t�F�[�h�C�����
		TYPE_SMALLBUILDING,		//�t�F�[�h�C�����
		TYPE_APART,				//�t�F�[�h�C�����
		TYPE_MAX
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CBlock(int nPriority);				//�f�t�H���g
	~CBlock();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetModel(MODELX model) { m_modelTemp = model; }
	MODELX *GetModel(void) { return &m_modelTemp; }

	//�ÓI�����o�֐�
	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight, CBlock::TYPE type);
	static HRESULT Load(void);
	static void Unload(void);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	MODELX m_modelTemp;

	//�ÓI�����o�ϐ�
	static MODELX m_model[TYPE_MAX];
	static const char *m_apFilename[TYPE_MAX];			//�e�N�X�`���̖��O

	TYPE m_type;

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_BLOCK_H_