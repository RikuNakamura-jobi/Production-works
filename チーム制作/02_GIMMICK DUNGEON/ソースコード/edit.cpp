//========================================
//
//3D���f���̃G�f�B�b�g�@�\[edit.cpp]
//Author:Kai Takada
//
//========================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "edit.h"
#include "object.h"
#include "player.h"
#include "enemy.h"
#include "wall.h"
#include "stdio.h"

//==========================================
//  �}�N����`
//==========================================
#define EDITER_MOVE (2.0f)		//�G�f�B�^�[�̃T�C�Y
#define EDIT_MAX (3000)			//�G�f�B�^�[�̍ő吔
#define TEX_MAX (10)			//�e�N�X�`���̐�
#define DEBUG_NUM (11)			//�f�o�b�O�\���̕�����̐�
#define SAVE_MAX (6)			//�Z�[�u������̎�ނ̐�
#define MAP_FILE "data\\SAVE\\MAP\\editdata003.txt"		//�Z�[�u�f�[�^�e�L�X�g�t�@�C��
#define MAP_RANKING "data\\SAVE\\MAP\\RankingMap000.txt"		//�Z�[�u�f�[�^�e�L�X�g�t�@�C��
#define MAP_TITLE "data\\SAVE\\MAP\\TitleMap000.txt"		//�Z�[�u�f�[�^�e�L�X�g�t�@�C��
#define MAP_TUTORIAL "data\\SAVE\\MAP\\TutorialMap000.txt"		//�Z�[�u�f�[�^�e�L�X�g�t�@�C��

//==========================================
//  �񋓌^��`
//==========================================
//�G�f�B�b�g�̏�Ԃ̗񋓌^
typedef enum
{
	EDITMODE_SET = 0,		//�ݒu�c�[��
	EDITMODE_DELETE,		//�폜�c�[��
	EDITMODE_MAX
}EDITMODE;

//�G�f�B�b�g��������ނ̗񋓌^
typedef enum
{
	EDITTYPE_OBJECT = 0,		//�I�u�W�F�N�g
	EDITTYPE_PLAYER,		//�v���C���[
	EDITTYPE_ENEMY,		//�G
	EDITTYPE_WALL,		//��
	EDITTYPE_MAX
}EDITTYPE;

//==========================================
//  �\���̐錾
//==========================================
//�Z�[�u��p�\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nType;
	float fHeight;
	float fWidth;
}EDITSAVE;

//�G�f�B�b�g��̉��Ԗڂ��g�p����̂����߂�\����
typedef struct
{
	int nObjectType;
}EDITNUM;

//�G�f�B�b�g�J�[�\���̐F�\����
typedef struct
{
	D3DXMATERIAL SetMat;		//�ݒu���[�h�F
	D3DXMATERIAL DeleteMat;		//�폜���[�h�F
}EDITCOLOR;

//X�t�@�C���p�̍\����
typedef struct
{
	DWORD dwNumMat = 0;		//�}�e���A���̐�
	LPD3DXMESH pMesh = {};		//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = {};		//�}�e���A���ւ̃|�C���^
	LPDIRECT3DTEXTURE9 pTexture[TEX_MAX] = {};	//�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;		//���f���̍ŏ��l�E�ő�l
}Xcont;

//�G�f�B�^�[�\����
typedef struct
{
	D3DXVECTOR3 pos;		//���W
	D3DXVECTOR3 rot;		//����
	float fHeight,fWidth;	//�|���S���p�����E��
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	EDITNUM EditModeType;	//���Ԗڂ�Set���邩
	int EditType;	//�ǂ̃^�C�v��I�𒆂�
	Xcont xData;		//X�t�@�C���̏��
	EDITCOLOR col;		//�G�f�B�^�[���̂̐F
	int nState;		//�G�f�B�^�[�̏��
}Edit;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void MoveEdit(void);	//�G�f�B�^�[�̈ړ�����
void RotEdit(void);		//�G�f�B�^�[�̕�������
void SetEdit(void);		//�G�f�B�^�[�ɂ��ݒu����
void DeleteEdit(void);	//�G�f�B�^�[�ɂ��폜����
void DebugEdit(void);	//�f�o�b�O�\������
void SaveEdit(void);	//�G�f�B�b�g���e�̃Z�[�u
void SetXDataEdit(void);		//Set�\��̃��f�������擾
void SelectIdx(int nSelect);	//�I�𒆂̃C���f�b�N�X���g�p����Ă�����̂ɂ��炷

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
Edit g_edit;						//�G�f�B�b�g���
int g_nSelectIdx;	//���ݑI�𒆂̃C���f�b�N�X(�ԍ�)

//==================================================
//�G�f�B�^�[�̏���������
//==================================================
void InitEdit(void)
{
	g_edit.pos = { 0.0f,0.0f,0.0f };
	g_edit.rot = { 0.0f,0.0f,0.0f };
	g_edit.fHeight = 0.0f;
	g_edit.fWidth = 0.0f;
	g_edit.EditModeType.nObjectType = 1;		//���f����0���R�����g�A�E�g��Ԃ̂���1����
	g_edit.nState = EDITMODE_SET;
	g_edit.EditType = EDITTYPE_OBJECT;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ݒu���F
	ZeroMemory(&g_edit.col.SetMat, sizeof(D3DXMATERIAL));
	g_edit.col.SetMat.MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 0.8f);
	g_edit.col.SetMat.MatD3D.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	g_edit.col.SetMat.MatD3D.Emissive = D3DXCOLOR(0.2f, 0.2f, 0.8f, 1.0f);

	//�폜���F
	ZeroMemory(&g_edit.col.DeleteMat, sizeof(D3DXMATERIAL));
	g_edit.col.DeleteMat.MatD3D.Diffuse = D3DXCOLOR(0.8f, 0.2f, 0.2f, 0.5f);
	g_edit.col.DeleteMat.MatD3D.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	g_edit.col.DeleteMat.MatD3D.Emissive = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);

	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	SetXDataEdit();

	//���_�����擾
	nNumVtx = g_edit.xData.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_edit.xData.pMesh->GetFVF());

	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_edit.xData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_edit.xData.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,				//�e�N�X�`���̃t�@�C����
				&g_edit.xData.pTexture[nCntMat]);
		}
	}

	//���_�o�b�t�@�����b�N
	g_edit.xData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;		//���_���W�̑��

		//���ׂĂ̒��_���r���ēG�̍ŏ��l�E�ő�l�𔲂��o��
		if (g_edit.xData.vtxMaxObject.x < vtx.x)
		{
			g_edit.xData.vtxMaxObject.x = vtx.x;
		}
		if (g_edit.xData.vtxMaxObject.y < vtx.y)
		{
			g_edit.xData.vtxMaxObject.y = vtx.y;
		}
		if (g_edit.xData.vtxMaxObject.z < vtx.z)
		{
			g_edit.xData.vtxMaxObject.z = vtx.z;
		}

		if (g_edit.xData.vtxMinObject.x > vtx.x)
		{
			g_edit.xData.vtxMinObject.x = vtx.x;
		}
		if (g_edit.xData.vtxMinObject.y > vtx.y)
		{
			g_edit.xData.vtxMinObject.y = vtx.y;
		}
		if (g_edit.xData.vtxMinObject.z > vtx.z)
		{
			g_edit.xData.vtxMinObject.z = vtx.z;
		}
		
		pVtxBuff += dwSizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	g_edit.xData.pMesh->UnlockVertexBuffer();
}

//==================================================
//�G�f�B�^�[�̏I������
//==================================================
void UninitEdit(void)
{
	for (int nCntTex = 0;nCntTex < TEX_MAX;nCntTex++)
	{
		if (g_edit.xData.pTexture[nCntTex] != NULL)
		{
			g_edit.xData.pTexture[nCntTex]->Release();
			g_edit.xData.pTexture[nCntTex] = NULL;					//�e�N�X�`���j��
		}
	}

	if (g_edit.xData.pBuffMat != NULL)
	{
		g_edit.xData.pBuffMat->Release();
		g_edit.xData.pBuffMat = NULL;					//�o�b�t�@�̔j��
	}

	if (g_edit.xData.pMesh != NULL)
	{
		g_edit.xData.pMesh->Release();
		g_edit.xData.pMesh = NULL;					//���b�V���̔j��
	}
}

//==================================================
//�G�f�B�^�[�̍X�V����
//==================================================
void UpdateEdit(void)
{
	MoveEdit();
	RotEdit();

	if (GetKeyboardTrigger(DIK_F2) == true)
	{//[ F2 ]�L�[�ŃG�f�B�b�g�̃^�C�v�ύX
		g_edit.EditType++;
		g_edit.EditType %= EDITTYPE_MAX;

		SetXDataEdit();
	}

	if (GetKeyboardTrigger(DIK_F3) == true)
	{//[ F3 ]�L�[�Ńu���b�N�̃^�C�v�ύX+
		if (g_edit.EditType == EDITTYPE_OBJECT)
		{
			g_edit.EditModeType.nObjectType++;

			if (g_edit.EditModeType.nObjectType == OBJECTTYPE_LEVER_BAR || 
				g_edit.EditModeType.nObjectType == OBJECTTYPE_REFLECTOR_BASE)
			{
				g_edit.EditModeType.nObjectType++;
			}

			g_edit.EditModeType.nObjectType %= OBJECTTYPE_MAX;
			SetXDataEdit();
		}
	}
	
	if (GetKeyboardTrigger(DIK_F4) == true)
	{//[ F4 ]�L�[�Ńu���b�N�̃^�C�v�ύX-
		if (g_edit.EditType == EDITTYPE_OBJECT)
		{
			g_edit.EditModeType.nObjectType--;
			if (g_edit.EditModeType.nObjectType == OBJECTTYPE_LEVER_BAR ||
				g_edit.EditModeType.nObjectType == OBJECTTYPE_REFLECTOR_BASE)
			{
				g_edit.EditModeType.nObjectType--;
			}

			if (g_edit.EditModeType.nObjectType == -1)
			{
				g_edit.EditModeType.nObjectType = OBJECTTYPE_MAX - 1;
			} 
			SetXDataEdit();
		}
	}

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//[ F5 ]�L�[�ŃG�f�B�^�[�̏�Ԃ�ʏ�ɖ߂�
		g_edit.nState++;
		g_edit.nState %= EDITMODE_MAX;
	}

	if (GetKeyboardTrigger(DIK_F6) == true)
	{//[ F6 ]�L�[�ōX�n�ɂ���
		Object *pObject = ObjectGet();
		Wall *pWall = GetWall();

		switch (g_edit.EditType)
		{
		case EDITTYPE_OBJECT:
			for (int nCntDelete = 0; nCntDelete < MAX_OBJECT; nCntDelete++, pObject++)
			{
				if (pObject->bUse == true)
				{
					pObject->bUse = false;
					pObject->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					pObject->nType = 0;
				}
			}
			break;
		case EDITTYPE_WALL:
			for (int nCntDelete = 0; nCntDelete < MAX_WALL; nCntDelete++, pWall++)
			{
				if (pWall->bUse == true)
				{
					pWall->bUse = false;
					pWall->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				}
			}
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_F9) == true)
	{//[ F9 ]�L�[��txt�t�@�C���ɃZ�[�u
		SaveEdit();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{//[ Enter ]�L�[�Őݒu
		switch (g_edit.nState)
		{
		case EDITMODE_SET:
			SetEdit();
			break;
		case EDITMODE_DELETE:
			DeleteEdit();
			break;
		}
	}

	if (GetKeyboardTrigger(DIK_K) == true)
	{//[ K ]�L�[�őI��Ώۂ����炷
		g_nSelectIdx++;

		SelectIdx(g_nSelectIdx);
	}
	if (GetKeyboardTrigger(DIK_L) == true)
	{//[ L ]�L�[�őI��Ώۂ�����炷
		g_nSelectIdx--;

		if (g_nSelectIdx < 0)
		{
			g_nSelectIdx = 0;
		}

		Object *pObject = ObjectGet();

		if (pObject[g_nSelectIdx].bUse == false)
		{//�I��Ώۂ��g�p����Ă��Ȃ�������
			while (1)
			{//�������[�v
				g_nSelectIdx--;

				if (pObject[g_nSelectIdx].bUse == true)
				{//�g���Ă���̂���������
					break;
				}
				if (g_nSelectIdx < 0)
				{//�S�Č�����
					g_nSelectIdx = 0;
					break;
				}
			}
		}
	}

	if (GetKeyboardPress(DIK_RIGHT) == true)
	{//[ �� ]�L�[�ŕ���L�΂�
		g_edit.fWidth += EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_LEFT) == true)
	{//[ �� ]�L�[�ŕ����k�߂�
		g_edit.fWidth -= EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_UP) == true)
	{//[ �� ]�L�[�ō�����L�΂�
		g_edit.fHeight += EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_DOWN) == true)
	{//[ �� ]�L�[�ō������k�߂�
		g_edit.fHeight -= EDITER_MOVE;
	}
}

//==================================================
//�G�f�B�^�[�̕`�揈��
//==================================================
void DrawEdit(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;						//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;								//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;									//�}�e���A���f�[�^�ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_edit.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		g_edit.rot.y, g_edit.rot.x, g_edit.rot.z);

	D3DXMatrixMultiply(&g_edit.mtxWorld, &g_edit.mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		g_edit.pos.x, g_edit.pos.y, g_edit.pos.z);

	D3DXMatrixMultiply(&g_edit.mtxWorld, &g_edit.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_edit.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_edit.xData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_edit.xData.dwNumMat; nCntMat++)
	{
		switch (g_edit.nState)
		{
		case EDITMODE_SET:
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&g_edit.col.SetMat.MatD3D);
			break;

		case EDITMODE_DELETE:
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&g_edit.col.DeleteMat.MatD3D);
			break;
		}

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_edit.xData.pTexture[nCntMat]);

		//�J�[�\���̕`��
		g_edit.xData.pMesh->DrawSubset(nCntMat);
	}

	//�ۑ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);

	DebugEdit();
}

//==================================================
//�G�f�B�^�[�̈ړ�����
//==================================================
void MoveEdit(void)
{
	Camera *pCamera = GetCamera();
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fSpeed = EDITER_MOVE;

	if (GetKeyboardPress(DIK_LSHIFT))
	{//�������邩�ǂ���
		fSpeed = EDITER_MOVE * 2;
	}

	//�L�[�������ꂽ�Ƃ�
	if (GetKeyboardPress(DIK_A) == true)
	{//A�L�[�������ꂽ
		if (GetKeyboardPress(DIK_W) == true)
		{//����ړ�
			move.x += cosf(pCamera->rot.y + -D3DX_PI * 0.75f) * fSpeed;
			move.z += sinf(pCamera->rot.y + -D3DX_PI * 0.75f) * fSpeed;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//�����ړ�
			move.x += cosf(pCamera->rot.y + -D3DX_PI * 0.25f) * fSpeed;
			move.z += sinf(pCamera->rot.y + -D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//���ړ�
			move.x += cosf(pCamera->rot.y + -D3DX_PI * 0.5f) * fSpeed;
			move.z += sinf(pCamera->rot.y + -D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[�������ꂽ

		if (GetKeyboardPress(DIK_W) == true)
		{//
			move.x += cosf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;
			move.z += sinf(pCamera->rot.y + D3DX_PI * 0.75f) * fSpeed;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//
			move.x += cosf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;
			move.z += sinf(pCamera->rot.y + D3DX_PI * 0.25f) * fSpeed;
		}
		else
		{//
			move.x += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * fSpeed;
			move.z += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * fSpeed;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//W�L�[�������ꂽ
		move.x += cosf(pCamera->rot.y + D3DX_PI * 1.0f) * fSpeed;
		move.z += sinf(pCamera->rot.y + D3DX_PI * 1.0f) * fSpeed;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//S�L�[�������ꂽ
		move.x += cosf(pCamera->rot.y + D3DX_PI * 0.0f) * fSpeed;
		move.z += sinf(pCamera->rot.y + D3DX_PI * 0.0f) * fSpeed;
	}

	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{//S�L�[�������ꂽ
		move.x *= 2.0f;
		move.z *= 2.0f;
	}

	if (GetKeyboardPress(DIK_T) == true)
	{//[ T ]�L�[�������ꂽ
		move.y += EDITER_MOVE;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{//[ B ]�L�[�������ꂽ
		move.y -= EDITER_MOVE;
	}

	g_edit.pos.x += move.x;
	g_edit.pos.y += move.y;
	g_edit.pos.z += move.z;
}

//==================================================
//�G�f�B�^�[�̌����̏���
//==================================================
void RotEdit(void)
{
	if (GetKeyboardPress(DIK_Z) == true)
	{//[ Z ]�L�[�������ꂽ
		g_edit.rot.y -= 0.01f;
	}
	if (GetKeyboardPress(DIK_C) == true)
	{//[ C ]�L�[�������ꂽ
		g_edit.rot.y += 0.01f;
	}

	if (GetKeyboardTrigger(DIK_V) == true)
	{//[ V ]�L�[�������ꂽ
		g_edit.rot.y += D3DX_PI * 0.5;
	}
	if (GetKeyboardTrigger(DIK_X) == true)
	{//[ X ]�L�[�������ꂽ
		g_edit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	if (g_edit.rot.y > D3DX_PI)
	{
		g_edit.rot.y += -D3DX_PI * 2;
	}
	else if (g_edit.rot.y < -D3DX_PI)
	{
		g_edit.rot.y += D3DX_PI * 2;
	}
}

//==================================================
//�G�f�B�^�[�̈ړ�����
//==================================================
void SelectIdx(int nSelect)
{
	Object *pObject = ObjectGet();
	Wall *pWall = GetWall();

	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		if (pObject[g_nSelectIdx].bUse == false)
		{//�I��Ώۂ��g�p����Ă��Ȃ�������
			while (1)
			{
				g_nSelectIdx++;

				if (pObject[g_nSelectIdx].bUse == true)
				{//�g���Ă���̂���������
					break;
				}
				if (g_nSelectIdx > MAX_OBJECT)
				{//�S�Č�����
					g_nSelectIdx = 0;
					break;
				}
			}
		}
		break;

	case EDITTYPE_WALL:
		if (pWall[g_nSelectIdx].bUse == false)
		{//�I��Ώۂ��g�p����Ă��Ȃ�������
			while (1)
			{
				g_nSelectIdx++;

				if (pWall[g_nSelectIdx].bUse == true)
				{//�g���Ă���̂���������
					break;
				}
				if (g_nSelectIdx > MAX_WALL)
				{//�S�Č�����
					g_nSelectIdx = 0;
					break;
				}
			}
		}
		break;
	}
}

//==================================================
//�G�f�B�^�[�ɂ��ݒu����
//==================================================
void SetEdit(void)
{
	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		SetObject(g_edit.pos, g_edit.rot, g_edit.EditModeType.nObjectType);		//�ݒu

		switch (g_edit.EditModeType.nObjectType)
		{
		case OBJECTTYPE_LEVER_BASE:
			SetObject(D3DXVECTOR3(g_edit.pos.x, g_edit.pos.y + 40.0f, g_edit.pos.z),
						g_edit.rot,
						OBJECTTYPE_LEVER_BAR);
			break;
		case OBJECTTYPE_REFLECTOR:
			SetObject(g_edit.pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), OBJECTTYPE_REFLECTOR_BASE);
			break;
		case OBJECTTYPE_SANDPOLE:
			SetObject(g_edit.pos, g_edit.rot, OBJECTTYPE_SANDPOLE_BASE);
			break;
		}
		break;
	case EDITTYPE_PLAYER:
		SetPosPlayer(g_edit.pos, g_edit.rot);		//���W�ύX
		break;
	case EDITTYPE_ENEMY:
		SetPosEnemy(g_edit.pos, g_edit.rot);		//���W�ύX
		break;
	case EDITTYPE_WALL:
		SetWall(g_edit.pos, g_edit.rot,g_edit.fWidth,g_edit.fHeight);		//�ݒu
		break;
	}
}

//==================================================
//�G�f�B�^�[�ɂ��폜����
//==================================================
void DeleteEdit(void)
{
	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		DeleteObject(g_nSelectIdx);
		break;
	case EDITTYPE_WALL:
		DeleteWall(g_nSelectIdx);
		break;
	}
}

//==================================================
//X�t�@�C�����̎擾����
//==================================================
void SetXDataEdit(void)
{
	if (g_edit.EditType == EDITTYPE_OBJECT)
	{
		//	�w�蒆�̃I�u�W�F�N�g�̃��b�V������ɓ����
		g_edit.xData.pMesh = ObjectMeshGet((OBJECTTYPE)g_edit.EditModeType.nObjectType);
		g_edit.xData.pBuffMat = ObjectBuffmatGet((OBJECTTYPE)g_edit.EditModeType.nObjectType);
		g_edit.xData.dwNumMat = ObjectdwNummatGet((OBJECTTYPE)g_edit.EditModeType.nObjectType);
	}
}

//==================================================
//�C���f�b�N�X�ݒ�̏���
//==================================================
void SetEditIndex(int nIdx)
{
	Object *pObject = ObjectGet();
	Wall *pWall = GetWall();

	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		pObject[nIdx].nIdxEdit = nIdx;
		break;
	case EDITTYPE_WALL:
		pWall[nIdx].nIdx = nIdx;
		break;
	}

	g_nSelectIdx = nIdx;
}

//==================================================
//���ݑI�𒆂̃C���f�b�N�X�̎擾����
//==================================================
int GetSelectIdxEdit(void)
{
	if (g_edit.nState == EDITMODE_DELETE)
	{
		return g_nSelectIdx;
	}

	return -1;
}

//==================================================
//�O���t�@�C������ʒu����ǂݎ�鏈��
//==================================================
void LoadEdit(void)
{
	D3DXVECTOR3 Objectpos;			//���W
	D3DXVECTOR3 Objectrot;			//����
	float fWidth, fHeight = 0.0f;	//���A����
	int nType = 0;					//���
	int nCntMax = 0;			//�Z�b�g��
	char cTemp[128] = {};		//�S�~��
	char aSetTarget[128] = {};	//�Z�b�g����Ώۂ̎��

	FILE *pFile;

	switch (GetMode())
	{
	case MODE_TITLE:
	case MODE_MENU:
		pFile = fopen(MAP_TITLE, "r");
		break;

	case MODE_TUTORIAL:
		pFile = fopen(MAP_TUTORIAL, "r");
		break;

	case MODE_RANKING:
		pFile = fopen(MAP_RANKING, "r");
		break;
	
	default:
		pFile = fopen(MAP_FILE, "r");
		break;
	}

	if (pFile != NULL)
	{
		while (fscanf(pFile, "%s", &cTemp[0]) != EOF)
		{
			//�I�u�W�F�N�g�ݒ�===========================================
			if (strcmp(cTemp, "OBJECTSET") == 0)
			{
				while (1)
				{//�I���܂ŃI�u�W�F�N�g�ݒ�
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "TYPE") == 0)
					{//�^�C�v�ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �^�C�v�ǂݍ���
						fscanf(pFile, "%d", &nType);
					}

					if (strcmp(cTemp, "POS") == 0)
					{//�ʒu�ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �ʒu�ǂݍ���
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// �����ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �����ǂݍ���
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "END_OBJECTSET") == 0)
					{// for���𔲂���
					 //�I�u�W�F�N�g�ݒu
						SetObject(Objectpos, Objectrot, nType);

						break;
					}
				}//END_OBJ
			}
			// �I�u�W�F�N�g�ݒ�===========================================================

			// �v���C���[�ݒ�===========================================
			if (strcmp(cTemp, "PLAYERSET") == 0)
			{
				while (1)
				{//�I���܂ŃI�u�W�F�N�g�ݒ�
				 //������ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//�ʒu�ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �ʒu�ǂݍ���
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// �����ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �����ǂݍ���
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "END_PLAYERSET") == 0)
					{// for���𔲂���
					 //�v���C���[�ݒ�
						SetPosPlayer(Objectpos, Objectrot);

						break;
					}
				}//END_PLAYER
			}
			// �v���C���[�ݒ�===========================================

			// �G�ݒ�===========================================
			if (strcmp(cTemp, "ENEMYSET") == 0)
			{
				while (1)
				{//�I���܂ŃI�u�W�F�N�g�ݒ�
				 //������ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//�ʒu�ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �ʒu�ǂݍ���
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// �����ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �����ǂݍ���
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "END_ENEMYSET") == 0)
					{// for���𔲂���
					 //�G�ݒ�
						SetPosEnemy(Objectpos, Objectrot);

						break;
					}
				}//END_ENEMY
			}
			// �G�ݒ�===========================================

			// �ǐݒ�===========================================
			if (strcmp(cTemp, "WALLSET") == 0)
			{
				while (1)
				{//�I���܂ŃI�u�W�F�N�g�ݒ�
				 //������ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "POS") == 0)
					{//�ʒu�ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �ʒu�ǂݍ���
						fscanf(pFile, "%f", &Objectpos.x);
						fscanf(pFile, "%f", &Objectpos.y);
						fscanf(pFile, "%f", &Objectpos.z);
					}

					if (strcmp(cTemp, "ROT") == 0)
					{// �����ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �����ǂݍ���
						fscanf(pFile, "%f", &Objectrot.x);
						fscanf(pFile, "%f", &Objectrot.y);
						fscanf(pFile, "%f", &Objectrot.z);
					}

					if (strcmp(cTemp, "SIZE") == 0)
					{// �T�C�Y�ǂݍ���
					 // "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �T�C�Y�ǂݍ���
						fscanf(pFile, "%f", &fHeight);
						fscanf(pFile, "%f", &fWidth);
					}

					if (strcmp(cTemp, "END_WALLSET") == 0)
					{// for���𔲂���
					 // �ǐݒ�
						SetWall(Objectpos, Objectrot, fWidth, fHeight);

						break;
					}
				}//END_WALL
			}
			// �ǐݒ�===========================================
		}//while��
		fclose(pFile);
	}
}

//==================================================
//�O���t�@�C���Ɉʒu���������o������
//==================================================
void SaveEdit(void)
{
	Wall *pWall = GetWall();			//�ǂ̏��
	Object *pObject = ObjectGet();		//�I�u�W�F�N�g�̏��
	Player *pPlayer = GetPlayer();		//�v���C���[
	Enemy *pEnemy = GetEnemy();			//�G
	EDITSAVE SaveData[EDITTYPE_MAX][EDIT_MAX] = {};		//�Z�[�u�p�f�[�^

	int nCntMax[EDITTYPE_MAX] = {0};		//�z�u��
	char aTypename[EDITTYPE_MAX][128] = {"OBJECT","PLAYER" ,"ENEMY" ,"WALL" };		//�Ώۂ̎�ޖ�����p
	char aObjTypename[128] = {};		//�I�u�W�F�N�g�̃^�C�v������p
	int aObjectTypeNum[OBJECTTYPE_MAX] = { 0 };		//�I�u�W�F�N�g�̃^�C�v�ʌ�����p
	int nWallNum = 0;		//�ǂ̌�����p
	int aMaxCnter[EDITTYPE_MAX] = {MAX_OBJECT,1,1,MAX_WALL};		//for���񂷗p�̊e�ő吔

	for (int nCntSaveType = 0; nCntSaveType < SAVE_MAX; nCntSaveType++)
	{
		for (int nCntUse = 0; nCntUse < aMaxCnter[nCntSaveType]; nCntUse++)
		{
			switch (nCntSaveType)
			{
			case EDITTYPE_OBJECT:
				if (pObject[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = pObject[nCntUse].pos;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = pObject[nCntUse].rot;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].nType = pObject[nCntUse].nType;
					nCntMax[nCntSaveType]++;
				}
				break;

			case EDITTYPE_PLAYER:
				if (pPlayer[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = D3DXVECTOR3(0.0f,20.0f,300.0f);
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = D3DXVECTOR3(0.0f,0.0f,0.0f);
					nCntMax[nCntSaveType] = 1;
				}
				break;

			case EDITTYPE_ENEMY:
				if (pEnemy[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = D3DXVECTOR3(0.0f, 20.0f, -300.0f);
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					nCntMax[nCntSaveType] = 1;
				}
				break;

			case EDITTYPE_WALL:
				if (pWall[nCntUse].bUse == true)
				{
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].pos = pWall[nCntUse].pos;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].rot = pWall[nCntUse].rot;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].fHeight = pWall[nCntUse].fHeight;
					SaveData[nCntSaveType][nCntMax[nCntSaveType]].fWidth = pWall[nCntUse].fWidth;
					nCntMax[nCntSaveType]++;
				}
				break;
			}
		}
	}

	FILE *pFile;

	pFile = fopen(MAP_FILE, "w");

	if (pFile != NULL)
	{
		//�w�b�_�[�R�����g�ǂ�
		fprintf(pFile, "#==============================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# �ݒ�E�ݒu�t�@�C��[editdata.txt]\n");
		fprintf(pFile, "# Author:_KAI_TAKADA\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#==============================================\n");

		for (int nCntSaveType = 0; nCntSaveType < EDITTYPE_MAX; nCntSaveType++)
		{
			//������
			fprintf(pFile, "%d\n\n", nCntMax[nCntSaveType]);

			//�R�����g�p�l�[��
			switch (nCntSaveType)
			{
			case EDITTYPE_PLAYER:
				strcpy(aObjTypename, "�v���C���[");
				break;

			case EDITTYPE_ENEMY:
				strcpy(aObjTypename, "�G");
				break;

			case EDITTYPE_WALL:
				strcpy(aObjTypename, "��");
				break;
			}

			for (int nCntStr = 0; nCntMax[nCntSaveType] > 0; nCntStr++)
			{
				if (nCntSaveType == EDITTYPE_OBJECT)
				{
					switch (pObject[nCntStr].nType)
					{
					case OBJECTTYPE_CATAPULT:			strcpy(aObjTypename, "�I�u�W�F�N�g-�┭�ˑ�");	break;
					case OBJECTTYPE_REFLECTOR:		strcpy(aObjTypename, "�I�u�W�F�N�g-���˔�");	break;
					case OBJECTTYPE_REFLECTOR_BASE:	strcpy(aObjTypename, "�I�u�W�F�N�g-���˔̑��");	break;
					case OBJECTTYPE_LEVER_BAR:		strcpy(aObjTypename, "�I�u�W�F�N�g-���o�[");	break;
					case OBJECTTYPE_LEVER_BASE:		strcpy(aObjTypename, "�I�u�W�F�N�g-���o�[�x�_");	break;
					case OBJECTTYPE_TORCH:			strcpy(aObjTypename, "�I�u�W�F�N�g-����");	break;
					case OBJECTTYPE_BUTTON:			strcpy(aObjTypename, "�I�u�W�F�N�g-�{�^��");	break;
					case OBJECTTYPE_SANDPOLE:		strcpy(aObjTypename, "�I�u�W�F�N�g-����");	break;
					case OBJECTTYPE_SANDPOLE_BASE:	strcpy(aObjTypename, "�I�u�W�F�N�g-�����̑��");	break;
					case OBJECTTYPE_STONEPOLE:		strcpy(aObjTypename, "�I�u�W�F�N�g-�Β�");	break;
					case OBJECTTYPE_DOOR:			strcpy(aObjTypename, "�I�u�W�F�N�g-�h�A");	break;
					case OBJECTTYPE_BOOK:			strcpy(aObjTypename, "�I�u�W�F�N�g-�{");	break;
					case OBJECTTYPE_ROCK_FRAG:		strcpy(aObjTypename, "�I�u�W�F�N�g-��̔j��");	break;
					case OBJECTTYPE_RUBBLE:			strcpy(aObjTypename, "�I�u�W�F�N�g-���ꂫ");	break;
					case OBJECTTYPE_STATUE_0:		strcpy(aObjTypename, "�I�u�W�F�N�g-����0");	break;
					case OBJECTTYPE_DEBRIS:			strcpy(aObjTypename, "�I�u�W�F�N�g-�c�[");	break;
					case OBJECTTYPE_SKULL:			strcpy(aObjTypename, "�I�u�W�F�N�g-���W��");	break;
					case OBJECTTYPE_SKULLMT:		strcpy(aObjTypename, "�I�u�W�F�N�g-���W���̎R");	break;
					case OBJECTTYPE_STATUE_1:		strcpy(aObjTypename, "�I�u�W�F�N�g-����1");	break;
					case OBJECTTYPE_STATUE_3:		strcpy(aObjTypename, "�I�u�W�F�N�g-����3-���l�T���X");	break;
					default:						strcpy(aObjTypename, "�V�����ǉ���������");	break;
					}
				
					aObjectTypeNum[pObject[nCntStr].nType]++;
				}

				//�R�����g����
				fprintf(pFile, "#------------------\n");
				fprintf(pFile, "#%s", aObjTypename);

				switch (nCntSaveType)
				{
				case EDITTYPE_OBJECT:
					fprintf(pFile, "%d\n", aObjectTypeNum[pObject[nCntStr].nType]);
					break;
				case EDITTYPE_WALL:
					fprintf(pFile, "%d\n", nWallNum);
					nWallNum++;
					break;

				default:
					fprintf(pFile, "\n");
					break;
				}

				fprintf(pFile, "#------------------\n");
				fprintf(pFile, "%sSET\n", aTypename[nCntSaveType]);		//��ޏ���

				//�^�C�v����
				if (nCntSaveType == EDITTYPE_OBJECT)
				{
					fprintf(pFile, "	TYPE ");
					fprintf(pFile, "= ");
					fprintf(pFile, "%d\n", SaveData[nCntSaveType][nCntStr].nType);
				}

				//�|�W�V��������
				fprintf(pFile, "	POS ");
				fprintf(pFile, "= ");
				fprintf(pFile, "%.2f ", SaveData[nCntSaveType][nCntStr].pos.x);
				fprintf(pFile, "%.2f ", SaveData[nCntSaveType][nCntStr].pos.y);
				fprintf(pFile, "%.2f\n", SaveData[nCntSaveType][nCntStr].pos.z);

				//���[�e�[�g����
				fprintf(pFile, "	ROT ");
				fprintf(pFile, "= ");
				fprintf(pFile, "%.4f ", SaveData[nCntSaveType][nCntStr].rot.x);
				fprintf(pFile, "%.4f ", SaveData[nCntSaveType][nCntStr].rot.y);
				fprintf(pFile, "%.4f\n", SaveData[nCntSaveType][nCntStr].rot.z);

				if (nCntSaveType == EDITTYPE_WALL)
				{
					fprintf(pFile, "	SIZE ");
					fprintf(pFile, "= ");
					fprintf(pFile, "%.4f ", SaveData[nCntSaveType][nCntStr].fHeight);
					fprintf(pFile, "%.4f\n", SaveData[nCntSaveType][nCntStr].fHeight);
				}
				//�G���h����
				fprintf(pFile, "END_%sSET\n\n", aTypename[nCntSaveType]);

				nCntMax[nCntSaveType]--;
			}
		}

		fclose(pFile);
	}
}

//==================================================
//�G�f�B�b�g�̃f�o�b�O�\��
//==================================================
void DebugEdit(void)
{
	LPD3DXFONT Font = GetFont();
	RECT rect = { 0,SCREEN_HEIGHT / 2,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[DEBUG_NUM][256];
	int nNumUseEdit = 0;	//�g�p�G�f�B�b�g��
	char sEditMode[12] = "�ݒu���[�h";		//�G�f�B�b�g�̃��[�h
	char sObjectType[16] = "��";		//�G�f�B�b�g�̃I�u�W�F�N�g�̎��
	char sModelType[16] = "�I�u�W�F�N�g";		//�G�f�B�b�g�̃��f���̎��

	switch (g_edit.nState)
	{
	case EDITMODE_SET:
		strcpy(sEditMode,"�ݒu���[�h");
		break;
	case EDITMODE_DELETE:
		strcpy(sEditMode, "�폜���[�h");
		break;
	}

	switch (g_edit.EditType)
	{
	case EDITTYPE_OBJECT:
		strcpy(sModelType, "�I�u�W�F�N�g");
		nNumUseEdit = GetUseObject();		//�g�p���̎擾
		break;
	case EDITTYPE_PLAYER:
		strcpy(sModelType, "�v���C���\");
		break;
	case EDITTYPE_ENEMY:
		strcpy(sModelType, "�G");
		break;
	case EDITTYPE_WALL:
		strcpy(sModelType, "��");
		nNumUseEdit = GetUseWall();		//�g�p���̎擾
		break;
	}

	switch (g_edit.EditModeType.nObjectType)
	{
	case OBJECTTYPE_CATAPULT:			strcpy(sObjectType, "�u���b�N");	break;
	case OBJECTTYPE_REFLECTOR:		strcpy(sObjectType, "���˔�");	break;
	case OBJECTTYPE_LEVER_BAR:		strcpy(sObjectType, "���o�[");	break;
	case OBJECTTYPE_LEVER_BASE:		strcpy(sObjectType, "���o�[�̎x�_");	break;
	case OBJECTTYPE_TORCH:			strcpy(sObjectType, "����");	break;
	case OBJECTTYPE_BUTTON:			strcpy(sObjectType, "�{�^��");	break;
	case OBJECTTYPE_SANDPOLE:		strcpy(sObjectType, "����");	break;
	case OBJECTTYPE_SANDPOLE_BASE:	strcpy(sObjectType, "�����̑��");	break;
	case OBJECTTYPE_STONEPOLE:		strcpy(sObjectType, "�Β�");	break;
	case OBJECTTYPE_DOOR:			strcpy(sObjectType, "�h�A");	break;
	case OBJECTTYPE_BOOK:			strcpy(sObjectType, "�{");	break;
	case OBJECTTYPE_ROCK_FRAG:		strcpy(sObjectType, "��̔j��");	break;
	case OBJECTTYPE_RUBBLE:			strcpy(sObjectType, "���ꂫ");	break;
	case OBJECTTYPE_STATUE_0:		strcpy(sObjectType, "����0");	break;
	case OBJECTTYPE_STATUE_1:		strcpy(sObjectType, "����1");	break;
	case OBJECTTYPE_DEBRIS:			strcpy(sObjectType, "�c�[");	break;
	case OBJECTTYPE_SKULL:			strcpy(sObjectType, "���W��");	break;
	case OBJECTTYPE_SKULLMT:		strcpy(sObjectType, "���W���̎R");	break;
	default:						strcpy(sObjectType, "�f�t�H���g");	break;
	}

	//������ɑ��
	sprintf(&aStr[0][0], "���݂̃��[�h<F5>�F%s", sEditMode);
	sprintf(&aStr[1][0], "�I�𒆂̃��f���̎��<F2>�F%s", sModelType);
	sprintf(&aStr[2][0], "�I�u�W�F�N�g�̎��<F3><F4>�F%s", sObjectType);
	sprintf(&aStr[3][0], "�g�p�C���f�b�N�X���F%d", nNumUseEdit);
	sprintf(&aStr[4][0], "�I���C���f�b�N�X<K/L>�F%d�Ԗ�", g_nSelectIdx);
	sprintf(&aStr[5][0], "���W<WASD><T/B>�F[%.3f�A%.3f�A%.3f]", g_edit.pos.x, g_edit.pos.y, g_edit.pos.z);
	sprintf(&aStr[6][0], "����<Z/C><V><X>�F[%.3f�A%.3f�A%.3f]", g_edit.rot.x, g_edit.rot.y, g_edit.rot.z);
	sprintf(&aStr[7][0], "�ǂ̍���<��/��>�F%.3f�A��<��/��>�F%.3f", g_edit.fHeight, g_edit.fWidth);
	sprintf(&aStr[8][0], "�ݒu<ENTER> || �Z�[�u<F9>");
	sprintf(&aStr[9][0], "�S�폜<F6>");
	sprintf(&aStr[10][0], "����<LSHIFT>");

	//�e�L�X�g�`��
	for (int nCntDraw = 0; nCntDraw < DEBUG_NUM; nCntDraw++)
	{
		Font->DrawText(NULL, &aStr[nCntDraw][0], -1, &rect, DT_LEFT, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		rect.top += 20;
	}
}
