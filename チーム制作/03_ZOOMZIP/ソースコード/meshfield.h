//==========================================================
//
//���b�V���t�B�[���h���� [meshfield.h]
//Author ����ɐ�
//
//==========================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "polygon.h"
#include "main.h"
#include <stdio.h>

//==========================================================
//�\���̂̒�`
//==========================================================
//���b�V���t�B�[���h
typedef struct
{
	polygon Polygon;		//�|���S����b���
	int nHeightMesh;		//��������
	int nWidthMesh;			//������
	int nVertex;			//���_��
	int nIndex;				//�`��ԍ���
	bool bUse;				//�g�p����Ă��邩�ǂ���
	int nTextype;			//�e�N�X�`���ԍ�
}MeshField;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeight, float fWidth, int nHeight, int nWidth, int nType);
void SetIndexMeshField(int nCntField);
void SetpVtxMeshField(int nCntField);
void LoadMeshFieldData(FILE *pFile);
void CreateFieldBuff(void);
bool CollisionMeshField(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

#endif