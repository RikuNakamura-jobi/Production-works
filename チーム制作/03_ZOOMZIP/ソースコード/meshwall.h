//==========================================================
//
//���b�V���E�H�[������ [meshwall.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include <stdio.h>
#include "polygon.h"

//==========================================================
//�}�N����`
//==========================================================
#define MAX_MESHWALL	(256)		//�ǂ̍ő吔

//==========================================================
//�ǂ̍\����
//==========================================================
typedef struct
{
	polygon Polygon;		//�|���S����b���
	int nHeightMesh;		//��������
	int nWidthMesh;			//������
	int nVertex;			//���_��
	int nIndex;				//�`��ԍ���
	int nStartIndex;		//�J�n�C���f�b�N�X�ԍ�
	bool bUse;				//�g�p����Ă��邩�ǂ���
	int nTextype;			//�e�N�X�`���^�C�v
}MeshWall;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType);
void SetIndexMeshWall(int nCntWall);
void SetpVtxMeshWall(int nCntWall);
MeshWall *GetMeshWall(void);
void LoadMeshWallData(FILE *pFile);
void CreateWallBuff(void);
void DrawResultMeshWall(void);

#endif