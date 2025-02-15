//=====================================
//
// sceneヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _SCENE_H_
#define _SCENE_H_

#include "main.h"
#include "player.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CBg;
class CBgMulti;
class CScore;
class CObject3D;
class CEnemy;
class CCamera;
class CTime;
class CField;
class CEnemyManager;
class CSky;
class CScene
{
public:				//外部からアクセス可能

	//種類
	typedef enum
	{
		MODE_NONE = 0,			//タイプ無し
		MODE_TITLE,				//タイトル画面
		MODE_TUTORIAL,			//チュートリアル画面
		MODE_GAME,				//ゲーム画面
		MODE_RESULT,			//リザルト画面
		MODE_MAX
	}MODE;

	//コンストラクタ・デストラクタ
	CScene();				//デフォルト
	virtual ~CScene();

	//メンバ関数
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

	virtual CEnemyManager *GetEnemyManager(void) { return NULL; }

	void addCntFade(void) { m_nCntFade++; }
	int GetCntFade(void) { return m_nCntFade; }

	bool GetbFade(void){ return m_bFade; }
	void SetbFade(bool bFade) { m_bFade = bFade; }

	//静的メンバ関数
	static CScene *Create(MODE mode);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	MODE m_mode;
	int m_nCntFade;
	bool m_bFade;

	//静的メンバ変数

};

class CTitle : public CScene
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CTitle();				//デフォルト
	~CTitle();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CBgMulti *m_pBgMulti;

	//静的メンバ変数

};

class CTutorial : public CScene
{
public:				//外部からアクセス可能

					//コンストラクタ・デストラクタ
	CTutorial();				//デフォルト
	~CTutorial();

	//メンバ関数
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

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

					//メンバ関数

					//メンバ変数
	CBg *m_pBg;
	CPlayer *m_pPlayer;
	CPlayer *m_pPlayerSave;
	CField *m_pField;
	CCamera *m_pCamera;
	CSky *m_pSky;

	bool m_bFinish;
	
	//静的メンバ変数

};

class CGame : public CScene
{
public:				//外部からアクセス可能

					//コンストラクタ・デストラクタ
	CGame();				//デフォルト
	~CGame();

	//メンバ関数
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

	CEnemyManager *GetEnemyManager(void) { return m_pEnemyManager; }

	CSky *GetSky(void) { return m_pSky; }

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CScore *m_pScore;
	CPlayer *m_pPlayer;
	CPlayer *m_pPlayerSave;
	CField *m_pField;
	CCamera *m_pCamera;
	CTime *m_pTime;
	CEnemyManager *m_pEnemyManager;
	CSky *m_pSky;

	bool m_bFinish;

	//静的メンバ変数

};

class CResult : public CScene
{
public:				//外部からアクセス可能

					//コンストラクタ・デストラクタ
	CResult();				//デフォルト
	~CResult();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//静的メンバ関数

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CBg *m_pBg;
	CScore *m_pScore;

	//静的メンバ変数

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_SCENE_H_