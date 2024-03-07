//=============================================================================
//
// サウンド処理 [camera.h]
// Author :中村陸
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

class CCamera
{
public:

	CCamera();
	~CCamera();

	//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Set(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	D3DXVECTOR3 GetPosV(void) { return m_posV; }
	D3DXVECTOR3 GetPosR(void) { return m_posR; }

	D3DXVECTOR3 GetPosVDest(void) { return m_posVDest; }
	D3DXVECTOR3 GetPosRDest(void) { return m_posRDest; }

	D3DXMATRIX GetMtxView(void) { return m_mtxView; }

	void Setzrot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }

	void SetLength(float length) { m_cameraLength = length; }
	float GetLength(void) { return m_cameraLength; }

	//静的メンバ関数

private:

	//メンバ関数
	void Rot(void);

	//メンバ変数
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;			//ビューマトリックス

	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	D3DXVECTOR3 m_posVDest;		//目的の視点
	D3DXVECTOR3 m_posRDest;		//目的の注視点
	D3DXVECTOR3 m_posVOld;		//目的の視点
	D3DXVECTOR3 m_posROld;		//目的の注視点
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXVECTOR3 m_pos;			//角度
	D3DXVECTOR3 m_rot;			//角度
	D3DXVECTOR3 m_rotOld;		//角度
	float m_cameraLength;		//角度
	float m_fViewBack;

	//静的メンバ変数

};

#endif // !_CAMERA_H_
