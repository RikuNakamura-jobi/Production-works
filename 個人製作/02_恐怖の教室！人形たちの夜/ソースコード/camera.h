//===========================
//
//ダイレクトX.cameraヘッダー
//Author:中村　陸
//
//===========================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 posVDest;		//目的の視点
	D3DXVECTOR3 posRDest;		//目的の注視点
	D3DXVECTOR3 posVOld;		//目的の視点
	D3DXVECTOR3 posROld;		//目的の注視点
	D3DXVECTOR3 vecU;			//上方向ベクトル
	D3DXVECTOR3 rot;			//角度
	D3DXVECTOR3 rotOld;			//角度
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX mtxView;			//ビューマトリックス
}Camera;

//プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void UpdateCameraTitle(void);
void SetCamera(void);
Camera GetCamera(void);
void RotCamera(void);
void RotCameraTitle(void);

#endif // !_CAMERA_H_