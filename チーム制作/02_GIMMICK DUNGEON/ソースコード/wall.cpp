//========================================
//
//3D���f���̕ǃ|���S������(wall.cpp)
// Author: Kai Takada
//
//========================================
#include "stdio.h"
#include "input.h"
#include "main.h"
#include "wall.h"
#include "edit.h"
#include "enemy.h"

//=============================
//�}�N����`
//=============================
#define WALL_SIZEX (400.0f)	//�T���v���T�C�Y
#define WALL_SIZEY (100.0f)	//�T���v���T�C�Y
#define WALL_TEX_X (300.0f)	//�e�N�X�`���̒���x
#define WALL_TEX_Y (300.0f)	//�e�N�X�`���̒���y

//=============================
//�v���g�^�C�v�錾
//=============================
void SetVtxpos(int nNum);

//=============================
//�O���[�o���ϐ��錾
//=============================
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//�e�N�X�`���ւ̃|�C���^ 
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Wall g_aWall[MAX_WALL];

//==============================
//�ǂ̏���������
//==============================
void InitWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\wall000.jpg",				//�e�N�X�`���̃t�@�C����
		&g_pTextureWall);

	for (int nCntInit = 0;nCntInit < MAX_WALL;nCntInit++)
	{//����������
		g_aWall[nCntInit].pos = D3DXVECTOR3(0.0f, 0.0f, WALL_SIZEX);
		g_aWall[nCntInit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntInit].fHeight = WALL_SIZEY;
		g_aWall[nCntInit].fWidth = WALL_SIZEX;
		g_aWall[nCntInit].fDivX = WALL_SIZEX / WALL_TEX_X;
		g_aWall[nCntInit].fDivY = WALL_SIZEY / WALL_TEX_Y;
		g_aWall[nCntInit].aVtxpos[0] = {};
		g_aWall[nCntInit].aVtxpos[1] = {};
		g_aWall[nCntInit].aVtxpos[2] = {};
		g_aWall[nCntInit].aVtxpos[3] = {};
		g_aWall[nCntInit].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;		//���_���̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWall->Lock(0,0,(void**)&pVtx,0);

	for (int nCntWall = 0;nCntWall < MAX_WALL;nCntWall++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-WALL_SIZEX, WALL_SIZEY * 2, 0.0f);		//(x,y,z)
		pVtx[1].pos = D3DXVECTOR3(WALL_SIZEX, WALL_SIZEY * 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-WALL_SIZEX, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(WALL_SIZEX, 0.0f, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	//(x,y,z)
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
		pVtx[1].tex = D3DXVECTOR2(g_aWall[nCntWall].fDivX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aWall[nCntWall].fDivY);
		pVtx[3].tex = D3DXVECTOR2(g_aWall[nCntWall].fDivX, g_aWall[nCntWall].fDivY);

		if (nCntWall >= 4)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWall->Unlock();
}

//==============================
//�ǂ̏I������
//==============================
void UninitWall(void)
{
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;					//�e�N�X�`���j��
	}
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;					//�o�b�t�@�̔j��
	}
}

//==============================
//�ǂ̍X�V����
//==============================
void UpdateWall(void)
{

}

//==============================
//�ǂ̕`�揈��
//==============================
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���C���[�t���[��
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	for (int nCntDraw = 0; nCntDraw < MAX_WALL; nCntDraw++)
	{
		if (g_aWall[nCntDraw].bUse == true)
		{
			D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCntDraw].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntDraw].rot.y, g_aWall[nCntDraw].rot.x, g_aWall[nCntDraw].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCntDraw].mtxWorld, &g_aWall[nCntDraw].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntDraw].pos.x, g_aWall[nCntDraw].pos.y, g_aWall[nCntDraw].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCntDraw].mtxWorld, &g_aWall[nCntDraw].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntDraw].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(nCntDraw, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(nCntDraw, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				nCntDraw * 4,									//�`�悷��ŏ��̃C���f�b�N�X
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//==============================
//�ǂ̐ݒu����
//==============================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fWidth, float fHeight)
{
	for (int nCntSet = 0; nCntSet < MAX_WALL; nCntSet++)
	{
		if (g_aWall[nCntSet].bUse == false)
		{
			g_aWall[nCntSet].pos = pos;
			g_aWall[nCntSet].rot = rot;
			g_aWall[nCntSet].fHeight = fHeight;
			g_aWall[nCntSet].fWidth = fWidth;
			g_aWall[nCntSet].fDivX = fWidth / WALL_TEX_X;
			g_aWall[nCntSet].fDivY = fHeight / WALL_TEX_Y;
			g_aWall[nCntSet].bUse = true;

			SetEditIndex(nCntSet);
			//SetVtxpos(nCntSet);

			VERTEX_3D *pVtx;		//���_���̃|�C���^

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += nCntSet * 4;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeight * 2, 0.0f);		//(x,y,z)
			pVtx[1].pos = D3DXVECTOR3(fWidth, fHeight * 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, 0.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth, 0.0f, 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				//(x,y)
			pVtx[1].tex = D3DXVECTOR2(g_aWall[nCntSet].fDivX, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aWall[nCntSet].fDivY);
			pVtx[3].tex = D3DXVECTOR2(g_aWall[nCntSet].fDivX, g_aWall[nCntSet].fDivY);

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffWall->Unlock();
			break;
		}
	}
}

//==============================
//�ǂ̍폜����
//==============================
void DeleteWall(int nIdx)
{
	g_aWall[nIdx].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aWall[nIdx].bUse = false;
}

//==============================
//�ǂ̏��擾����
//==============================
Wall *GetWall(void)
{
	return &g_aWall[0];
}

//==============================
//�ǂ̎g�p���擾����
//==============================
int GetUseWall(void)
{
	int nNumUse = 0;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{
			nNumUse++;
		}
	}

	return nNumUse;
}

//==============================
//�l�p�̍��W�擾
//==============================
void SetVtxpos(int nNum)
{
	D3DXVECTOR3 math;

	//���_���p�x�ɍ��킹��
	for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
	{
		//math.x = cosf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth + g_aWall[nNum].pos.x;
		nCntVtx < 2 ? math.y = +g_aWall[nNum].pos.x + g_aWall[nNum].fHeight : math.y = g_aWall[nNum].pos.y;
		//math.z = sinf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth + g_aWall[nNum].pos.z;
	
		if (nCntVtx % 2 == 0)
		{
			math.x = (cosf(g_aWall[nNum].rot.y) * -g_aWall[nNum].fWidth) + g_aWall[nNum].pos.x;
			math.z = (sinf(g_aWall[nNum].rot.y) * -g_aWall[nNum].fWidth) + g_aWall[nNum].pos.z;
		}
		else
		{
			math.x = (cosf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth) + g_aWall[nNum].pos.x;
			math.z = (sinf(g_aWall[nNum].rot.y) * g_aWall[nNum].fWidth) + g_aWall[nNum].pos.z;
		}

		//(nCntVtx % 2) == 1 ? math.x * -1 : math.x * 1;
		//(nCntVtx % 2) == 1 ? math.z * -1 : math.z * 1;
		g_aWall[nNum].aVtxpos[nCntVtx] = math;
	}
}

//==============================
//�ǂ̓����蔻��
//==============================
bool CollisionWall(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength)
{
	bool bReflector;
	float fRate, fOutToPos, fOutToPosOld, fOutUnit, fOutRate, fRef, fDot, fRotRef;
	D3DXVECTOR3 vecLine, vecToPos, vecToPosOld, vecMove, vecNor, vecDot, vecMoveRef, posA, posB, posRef, posRefOld;

	bReflector = false;

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse == true)
		{
			posA.x = g_aWall[nCnt].pos.x - (cosf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);
			posA.y = 0.0f;
			posA.z = g_aWall[nCnt].pos.z + (sinf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);

			posB.x = g_aWall[nCnt].pos.x + (cosf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);
			posB.y = 0.0f;
			posB.z = g_aWall[nCnt].pos.z - (sinf(g_aWall[nCnt].rot.y) * g_aWall[nCnt].fWidth);

			//�v���C���[�̊p�x
			fRef = atan2f(posA.x - posB.x, posA.z - posB.z) + (D3DX_PI * 0.5f);

			posRef.x = pos->x + (fLength * sinf(fRef));
			posRef.z = pos->z + (fLength * cosf(fRef));

			posRefOld.x = posOld->x + (fLength * sinf(fRef));
			posRefOld.z = posOld->z + (fLength * cosf(fRef));

			//��_�ƃv���C���[�̈ʒu�̃x�N�g��
			vecToPos.x = posRef.x - posB.x;
			vecToPos.z = posRef.z - posB.z;

			//��_�ƃv���C���[�̑O�̈ʒu�̃x�N�g��
			vecToPosOld.x = posRefOld.x - posB.x;
			vecToPosOld.z = posRefOld.z - posB.z;

			//���ʂ̃x�N�g��
			vecLine.x = posA.x - posB.x;
			vecLine.y = 0.0f;
			vecLine.z = posA.z - posB.z;

			//�ړ��ʃx�N�g��
			vecMove = posRef - posRefOld;

			//���ʂƃv���C���[�̈ʒu�̊O��
			fOutToPos = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);
			fOutToPosOld = (vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z);

			//��_�v�Z
			fOutUnit = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);
			fOutRate = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
			fRate = fOutRate / fOutUnit;

			//���ʏՓ˔���
			if (fOutToPos > 0.0f && fOutToPosOld < 0.0f && fRate < 1.0f && fRate > 0.0f)
			{
				if (pos->y < g_aWall[nCnt].pos.y + 4000.0f && pos->y > g_aWall[nCnt].pos.y - 4000.0f)
				{
					//���������̂�bool�^��true�ɂ���
					bReflector = true;

					//�@���x�N�g���v�Z
					vecNor.x = -vecLine.z;
					vecNor.y = 0.0f;
					vecNor.z = vecLine.x;

					//�@���x�N�g���P�ʉ�
					D3DXVec3Normalize(&vecNor, &vecNor);

					float fMoveSize;
					D3DXVECTOR3 point, pointJud, vecRem;

					point.x = posB.x + (vecLine.x * fRate);
					point.z = posB.z + (vecLine.z * fRate);
					pointJud.x = point.x + vecNor.x;
					pointJud.y = 0.0f;
					pointJud.z = point.z + vecNor.z;
					fMoveSize = hypotf(posRef.x - point.x, posRef.z - point.z);

					if (OuterProduct(pointJud, posRefOld, posRef) > 0.0f)
					{
						vecRem.x = vecNor.z;
						vecRem.y = 0.0f;
						vecRem.z = -vecNor.x;
					}
					else
					{
						vecRem.x = -vecNor.z;
						vecRem.y = 0.0f;
						vecRem.z = vecNor.x;
					}

					//�@���x�N�g���P�ʉ�
					D3DXVec3Normalize(&vecRem, &vecRem);

					//���������傫��
					fDot = (-vecMove.x * vecNor.x) + (-vecMove.z * vecNor.z);

					//���������x�N�g���v�Z
					vecDot = (vecNor * fDot) * 2;

					//�ړ��ʃx�N�g���v�Z
					vecMoveRef = vecMove + vecDot;

					fRotRef = atan2f(vecRem.x, vecRem.z) - atan2f(vecMoveRef.x, vecMoveRef.z);

					vecMoveRef.x = (-sinf(fRotRef) * vecMoveRef.z) + (cosf(fRotRef) * vecMoveRef.x);
					vecMoveRef.z = (sinf(fRotRef) * vecMoveRef.x) + (cosf(fRotRef) * vecMoveRef.z);

					//�ړ��ʃx�N�g���P�ʉ�(�K�v��������Ȃ�)
					D3DXVec3Normalize(&vecMoveRef, &vecMoveRef);

					//�v���C���[�̈ʒu�ړ�
					pos->x = posB.x + (vecLine.x * fRate) + vecNor.x + (fLength * sinf(atan2f(vecNor.x, vecNor.z)));
					pos->z = posB.z + (vecLine.z * fRate) + vecNor.z + (fLength * cosf(atan2f(vecNor.x, vecNor.z)));

					/*move->x = vecMoveRef.x * move->x;
					move->z = vecMoveRef.z * move->z;*/
				}
			}
		}
	}

	return bReflector;
}

//========================================
//�����L���O�ł̕ǂ̍X�V
//========================================
void UpdateWallRanking(float moveX)
{
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == true)
		{
			g_aWall[nCntWall].pos.x += moveX;

			if (g_aWall[nCntWall].pos.x <= -1000.0f)
			{
				g_aWall[nCntWall].pos.x = 2000.0f;
			}
		}
	}
}
