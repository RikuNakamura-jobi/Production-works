//=====================================
//
// scene�w�b�_�[
// Author:�����@��
//
//=====================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "player.h"

//�}�N����`---------------------------

//�񋓌^��`---------------------------

//�N���X��`---------------------------
class CBg;
class CBgMulti;
class CScore;
class CCombo;
class CObject3D;
class CCamera;
class CTime;
class CField;
class CSky;
class CMap;
class CDeliverypoint;
class CScene
{
public:				//�O������A�N�Z�X�\

	//���
	enum MODE
	{
		MODE_NONE = 0,			//�^�C�v����
		MODE_TITLE,				//�^�C�g�����
		MODE_TUTORIAL,			//�`���[�g���A�����
		MODE_GAME,				//�Q�[�����
		MODE_RESULT,			//���U���g���
		MODE_MAX
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CScene();				//�f�t�H���g
	virtual ~CScene();

	//�����o�֐�
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetMode(MODE mode) { m_mode = mode; }
	MODE GetMode(void) { return m_mode; }

	virtual CCamera *GetCamera(void) { return NULL; }

	virtual void SetPlayer(CPlayer *player) { return; }
	virtual CPlayer *GetPlayer(void) { return NULL; }

	virtual void SetPlayerSave(CPlayer *playerSave) { return; }
	virtual CPlayer *GetPlayerSave(void) { return NULL; }

	virtual CField *GetField(void) { return NULL; }

	virtual CScore *GetScore(void) { return NULL; }

	virtual CCombo *GetCombo(void) { return NULL; }

	virtual CMap *GetMap(void) { return NULL; }

	virtual CDeliverypoint *GetDeliverypoint(void) { return NULL; }

	virtual int GetStartCount(void) { return 0; }

	virtual void SetFinish(void) { return; }

	void addCntFade(void) { m_nCntFade++; }
	int GetCntFade(void) { return m_nCntFade; }

	bool GetbFade(void){ return m_bFade; }
	void SetbFade(bool bFade) { m_bFade = bFade; }

	//�ÓI�����o�֐�
	static CScene *Create(MODE mode);

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	MODE m_mode;
	int m_nCntFade;
	bool m_bFade;

	//�ÓI�����o�ϐ�

};

class CTitle : public CScene
{
public:				//�O������A�N�Z�X�\

	//�R���X�g���N�^�E�f�X�g���N�^
	CTitle();				//�f�t�H���g
	~CTitle();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	CBgMulti *m_pBgMulti;

	//�ÓI�����o�ϐ�

};

class CTutorial : public CScene
{
public:				//�O������A�N�Z�X�\

					//�R���X�g���N�^�E�f�X�g���N�^
	CTutorial();				//�f�t�H���g
	~CTutorial();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CCamera *GetCamera(void) { return m_pCamera; }

	void SetPlayer(CPlayer *player);
	CPlayer *GetPlayer(void) { return m_pPlayer; }

	//void SetPlayerSave(CPlayer *playerSave);
	CPlayer *GetPlayerSave(void) { return m_pPlayerSave; }

	CField *GetField(void) { return m_pField; }

	CSky *GetSky(void) { return m_pSky; }

	//�ÓI�����o�֐�

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

					//�����o�֐�

					//�����o�ϐ�
	CBg *m_pBg;
	CPlayer *m_pPlayer;
	CPlayer *m_pPlayerSave;
	CField *m_pField;
	CCamera *m_pCamera;
	CSky *m_pSky;
	bool m_bFinish;
	
	//�ÓI�����o�ϐ�

};

class CGame : public CScene
{
public:				//�O������A�N�Z�X�\

					//�R���X�g���N�^�E�f�X�g���N�^
	CGame();				//�f�t�H���g
	~CGame();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CCamera *GetCamera(void) { return m_pCamera; }

	void SetPlayer(CPlayer *player);
	CPlayer *GetPlayer(void) { return m_pPlayer; }

	//void SetPlayerSave(CPlayer *playerSave);
	CPlayer *GetPlayerSave(void) { return m_pPlayerSave; }

	CField *GetField(void) { return m_pField; }

	CScore *GetScore(void) { return m_pScore; }

	CCombo *GetCombo(void) { return m_pCombo; }

	CSky *GetSky(void) { return m_pSky; }

	CMap *GetMap(void) { return m_pMap; }

	CDeliverypoint *GetDeliverypoint(void) { return m_pDeliverypoint; }

	int GetStartCount(void) { return m_nStartCount; }

	void SetFinish(void) { m_bFinish = true; }

	//�ÓI�����o�֐�

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	CScore *m_pScore;
	CCombo *m_pCombo;
	CPlayer *m_pPlayer;
	CPlayer *m_pPlayerSave;
	CField *m_pField;
	CCamera *m_pCamera;
	CTime *m_pTime;
	CSky *m_pSky;
	CMap *m_pMap;
	CDeliverypoint *m_pDeliverypoint;
	CBg *m_pBgStart;

	bool m_bFinish;

	int m_nStartCount;
	int m_nTutorialCount;
	int m_nPadCrossCount;

	//�ÓI�����o�ϐ�

};

class CResult : public CScene
{
public:				//�O������A�N�Z�X�\

					//�R���X�g���N�^�E�f�X�g���N�^
	CResult();				//�f�t�H���g
	~CResult();

	//�����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ÓI�����o�֐�

protected:			//�q�Ȃ�A�N�Z�X�\(�g��Ȃ�)

private:			//�O������A�N�Z�X�s�\

	//�����o�֐�

	//�����o�ϐ�
	CBg *m_pBg;
	CScore *m_pScore[6];

	//�ÓI�����o�ϐ�

};

//�\���̒�`---------------------------

//�v���g�^�C�v�錾---------------------

#endif // !_SCENE_H_