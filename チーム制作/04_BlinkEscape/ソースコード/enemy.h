//==========================================
//
//  エネミークラス(enemy.h)
//  Author : Riku Nakamura
//
//==========================================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "object2D.h"

//==========================================
//  マクロ定義
//==========================================
#define ENEMY_GLAZE (65.0f)

//==========================================
//  クラス定義
//==========================================
class CEnemy : public CObject2D
{
public:

	// エネミータイプ列挙
	typedef enum
	{
		TYPE_NONE = 0,		// なし
		TYPE_STICK,			// 回転棒
		TYPE_STICK_SLIDE,	// 横移動棒
		TYPE_MAX			// この列挙型の総数
	}TYPE;

	//メンバ関数
	CEnemy();
	~CEnemy();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }

	//静的メンバ関数
	static CEnemy* Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rSize = VEC3_ONE, const D3DXVECTOR3& rRot = VEC3_ZERO, const D3DXCOLOR& rCol = XCOL_WHITE, const TYPE type = TYPE_NONE);

private:

	//メンバ関数
	void Collision(D3DXVECTOR3 rPos, D3DXVECTOR3 rSize, D3DXVECTOR3 rRot);

	//メンバ変数
	TYPE m_type;
	float m_moveSpeed;

};

#endif
