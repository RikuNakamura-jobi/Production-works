//=====================================
//
// managerヘッダー
// Author:中村　陸
//
//=====================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"
#include "scene.h"

//マクロ定義---------------------------

//列挙型定義---------------------------

//クラス定義---------------------------
class CRenderer;
class CInput;
class CSound;
class CCamera;
class CLight;
class CDebugProc;
class CTexture;
class CPlayer;
class CObject3D;
class CEnemyManager;
class CCollision;
class CManager
{
public:				//外部からアクセス可能

	//コンストラクタ・デストラクタ
	CManager();				//デフォルト
	~CManager();

	//メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CRenderer *GetRenderer(void) { return m_pRenderer; }

	CInput *GetInputKeyboard(void) { return m_pInputKeyboard; }

	CInput *GetInputPad(void) { return m_pInputPad; }

	CInput *GetInputMouse(void) { return m_pInputMouse; }

	CLight *GetLight(void) { return m_pLight; }

	CDebugProc *GetDebugProc(void) { return m_pDebugProc; }

	CTexture *GetTexture(void) { return m_pTexture; }

	CCollision *GetCollision(void) { return m_pCollision; }

	void SetMode(CScene::MODE mode);
	CScene::MODE GetMode(void) { return m_pScene->GetMode(); }

	CScene *GetScene(void) { return m_pScene; }

	void SetFPS(int nCountFPS) { m_nCountFPS = nCountFPS; }

	//静的メンバ関数
	static CManager *Get(void);
	static HRESULT Release(void);

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	CRenderer *m_pRenderer;
	CInput *m_pInputKeyboard;
	CInput *m_pInputPad;
	CInput *m_pInputMouse;
	CSound *m_pSound;
	CLight *m_pLight;
	CDebugProc *m_pDebugProc;
	CTexture *m_pTexture;
	CScene *m_pScene;
	CEnemyManager *m_pEnemyManager;
	CCollision *m_pCollision;
	int m_nCountFPS;

	//静的メンバ変数
	static CManager *pManager;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_MANAGER_H_