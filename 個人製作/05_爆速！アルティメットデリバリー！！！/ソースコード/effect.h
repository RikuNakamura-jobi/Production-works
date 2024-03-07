//=====================================
//
// effect�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "objectbillboard.h"
#include "object2D.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CEffect
{
public:				//�O������A�N�Z�X�\
	
	//�R���X�g���N�^�E�f�X�g���N�^
	CEffect() {};				//�f�t�H���g
	~CEffect() {};

	static HRESULT Load(void);
	static void Unload(void);

	static LPDIRECT3DTEXTURE9 GetTexture(void) { return m_pTexture; }

private:			//�O������A�N�Z�X�s�\

	//�ÓI�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�e�N�X�`���ւ̃|�C���^
};

class CEffect3D : public CObjectBillboard
{
public:				//�O������A�N�Z�X�\

	//�R���X�g���N�^�E�f�X�g���N�^
	CEffect3D(int nPriority);				//�f�t�H���g
	~CEffect3D();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�
	static CEffect3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight, bool bCenter = true);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	int m_nCounterAnim;											//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;											//�A�j���[�V�����p�^�[��No.
	int m_nLife;
	int m_nLifeMax;
};

class CEffect2D : public CObject2D
{
public:				//�O������A�N�Z�X�\

					//�R���X�g���N�^�E�f�X�g���N�^
	CEffect2D(int nPriority);				//�f�t�H���g
	~CEffect2D();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�
	static CEffect2D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, int nLife, float fWidth, float fHeight, bool bCenter = true);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

					//�����o�֐�

					//�����o�ϐ�
	int m_nCounterAnim;											//�A�j���[�V�����J�E���^�[
	int m_nPatternAnim;											//�A�j���[�V�����p�^�[��No.
	int m_nLife;
	int m_nLifeMax;
};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_EFFECT_H_