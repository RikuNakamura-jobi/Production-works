//==================================================================================================
//
//Book.cpp
//Author:���R����
//
//==================================================================================================

#include "main.h"
#include "book.h"
#include "model.h"
#include "fade.h"
#include "camera.h"
#include "input.h"

//************************************
// �}�N����`
//************************************
#define MOVE_SPEED	(1.0f)	// �ړ����x
#define ROLL_SPEED	(0.015f)	// ��]���x
#define DIFF_HEIGHT	(10.0f) // �J�����̍����Ƃ̍���
#define DIST_BOOK	(49.0f)	// �J��������̖{�̋���
#define BOOK_ROLL_LIMIT	(D3DX_PI * 0.9f)	// �{�̉�]����

//************************************
// �O���[�o���ϐ��錾
//************************************
Book g_aBook[NUM_BOOK];	//�\���̂̏��
D3DXVECTOR3 g_posBook;	//�S�̂̈ʒu
D3DXVECTOR3 g_rotBook;	//�S�̂̌���
D3DXMATRIX g_mtxBook;	//�S�̂̃}�g���b�N�X
BOOKSTATE g_bookState;	//�{�̏��

//==================================================================================================
// ����������
//==================================================================================================
void InitBook(void)
{
	// �ϐ�������
	ZeroMemory(&g_aBook[0],sizeof(Book) * NUM_BOOK);

	// �e��ϐ�������
	g_posBook = { 0.0f,100.0f,0.0f };
	g_rotBook = { 0.0f,D3DX_PI * 0.5f,0.0f };
	g_bookState = BOOKSTATE_NONE;
}

//==================================================================================================
// �I������
//==================================================================================================
void UninitBook(void)
{
}

//==================================================================================================
// �X�V����
//==================================================================================================
void UpdateBook(void)
{
	if (g_bookState != BOOKSTATE_END)
	{
		// ���擾
		Camera *pCamera = GetCamera();

		//�ʒu�̐ݒ�
		g_posBook =
		{
			pCamera->posVDest.x,
			g_posBook.y,
			pCamera->posVDest.z - DIST_BOOK
		};

		switch (g_bookState)
		{
		case BOOKSTATE_NONE:
			if (GetFade() == FADE_NONE)
			{//�����n�߂�
				g_bookState = BOOKSTATE_IN;
			}
			break;
		case BOOKSTATE_IN:
			//�{���~
			g_posBook.y -= MOVE_SPEED;

			if (pCamera->posVDest.y - DIFF_HEIGHT > g_posBook.y)
			{//�X���C�h�C���I��
			 //�����␳
				g_posBook.y = GetCamera()->posVDest.y - DIFF_HEIGHT;

				//�{���J��
				g_bookState = BOOKSTATE_OPEN;
			}
			break;
		case BOOKSTATE_OPEN:
			//�{�J��
			g_aBook[0].rot.y += ROLL_SPEED;

			if (g_aBook[0].rot.y > BOOK_ROLL_LIMIT)
			{//�{�J���̏I��
			 //�p�x�̕␳
				g_aBook[0].rot.y = BOOK_ROLL_LIMIT;

				//�ҋ@��Ԃֈڍs
				g_bookState = BOOKSTATE_WAIT;
			}
			break;
		case BOOKSTATE_WAIT:
			if (GetKeyboardTrigger(DIK_RETURN) || GetJoyPadButtonTrigger(KEY_START, 0) || GetJoyPadButtonTrigger(KEY_A, 0) || GetJoyPadButtonTrigger(KEY_B, 0))
			{//�ҋ@�I��
				g_bookState = BOOKSTATE_OUT;
			}
			break;
		case BOOKSTATE_OUT:
			//�{����
			g_aBook[0].rot.y -= ROLL_SPEED * 4.0f;

			if (g_aBook[0].rot.y < 0.0f)
			{//�p�x�̕␳
				g_aBook[0].rot.y = 0.0f;
			}

			//�{�㏸
			g_posBook.y += MOVE_SPEED;

			if (130.0f < g_posBook.y)
			{//�X���C�h�A�E�g�I��
				//�����␳
				g_posBook.y = 130.0f;

				//�{���I����Ԃ�
				g_bookState = BOOKSTATE_END;
			}
			break;
		default:
			break;
		}
	}
}

//==================================================================================================
// �`�揈��
//==================================================================================================
void DrawBook(void)
{
	if (g_bookState != BOOKSTATE_END)
	{
		//�f�o�C�X�|�C���^�ݒ�
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//�ϐ��錾
		D3DXMATRIX mtxRot, mtxTrans, mtxParent;
		D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
		D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

										//���擾
		Book *pBook = GetBook();
		Model *pModel = GetModelBook();

		for (int nCntBook = 0; nCntBook < NUM_BOOK; nCntBook++, pBook++)
		{
			//�S�̂̃}�g���b�N�X==========================================
			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&g_mtxBook);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_rotBook.y, g_rotBook.x, g_rotBook.z);
			D3DXMatrixMultiply(&g_mtxBook, &g_mtxBook, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				g_posBook.x, g_posBook.y, g_posBook.z);
			D3DXMatrixMultiply(&g_mtxBook, &g_mtxBook, &mtxTrans);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_mtxBook);
			//�S�̂̃}�g���b�N�X==========================================

			//�{�p�[�c==========================================
			//���[���h�}�g���b�N�X������
			D3DXMatrixIdentity(&pBook->mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				pBook->rot.y, pBook->rot.x, pBook->rot.z);
			D3DXMatrixMultiply(&pBook->mtxWorld, &pBook->mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans,
				pBook->pos.x, pBook->pos.y, pBook->pos.z);
			D3DXMatrixMultiply(&pBook->mtxWorld, &pBook->mtxWorld, &mtxTrans);

			//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���������킹��
			D3DXMatrixMultiply(&pBook->mtxWorld,
				&pBook->mtxWorld,
				&g_mtxBook);

			//���[���h�}�g���b�N�X�ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &pBook->mtxWorld);
			//�{�p�[�c==========================================

			//���݂̃}�e���A���擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)pModel[nCntBook].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)pModel[nCntBook].dwNumMat; nCntMat++)
			{
				//�}�e���A���ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���ݒ�
				pDevice->SetTexture(0, pModel[nCntBook].apTexture[nCntMat]);

				//���f���i�p�[�c�j�`��
				pModel[nCntBook].pMesh->DrawSubset(nCntMat);
			}

			//�}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================================
// �|�C���^�擾����
//==================================================================================================
Book *GetBook(void)
{
	return &g_aBook[0];
}

//==================================================================================================
// ��Ԏ擾
//==================================================================================================
BOOKSTATE *GetBookState(void)
{
	return &g_bookState;
}