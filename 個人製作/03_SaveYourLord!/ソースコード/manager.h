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

	//静的メンバ関数
	static CRenderer *GetRenderer(void) { return m_pRenderer; }

	static CInput *GetInputKeyboard(void) { return m_pInputKeyboard; }

	static CInput *GetInputPad(void) { return m_pInputPad; }

	static CInput *GetInputMouse(void) { return m_pInputMouse; }

	static CLight *GetLight(void) { return m_pLight; }

	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; }

	static CTexture *GetTexture(void) { return m_pTexture; }

	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_pScene->GetMode(); }

	static CScene *GetScene(void) { return m_pScene; }

	void SetFPS(int nCountFPS) { m_nCountFPS = nCountFPS; }

protected:			//子ならアクセス可能(使わない)

private:			//外部からアクセス不可能

	//メンバ関数

	//メンバ変数
	int m_nCountFPS;

	//静的メンバ変数
	static CRenderer *m_pRenderer;
	static CInput *m_pInputKeyboard;
	static CInput *m_pInputPad;
	static CInput *m_pInputMouse;
	static CSound *m_pSound;
	static CLight *m_pLight;
	static CDebugProc *m_pDebugProc;
	static CTexture *m_pTexture;
	static CScene *m_pScene;
	static CEnemyManager *m_pEnemyManager;

};

//構造体定義---------------------------

//プロトタイプ宣言---------------------

#endif // !_MANAGER_H_