//===================================================
//
//�K�w�\���ɂ�郂�[�V�������� [motion.cpp]
//Author Ibuki Okusada
//
//===================================================
#include "motion.h"
#include "fileload.h"

//===================================================
//���[�V�����̏���������
//===================================================
void InitMotion(void)
{

}

//===================================================
//���[�V�����̏I������
//===================================================
void UninitMotion(Body *pBody)
{
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		//���b�V���̔p��
		if (pBody->FileData.aParts[nCntParts].FileData.pMesh != NULL)
		{
			pBody->FileData.aParts[nCntParts].FileData.pMesh->Release();
			pBody->FileData.aParts[nCntParts].FileData.pMesh = NULL;
		}

		//�}�e���A���̔p��
		if (pBody->FileData.aParts[nCntParts].FileData.pBuffMat != NULL)
		{
			pBody->FileData.aParts[nCntParts].FileData.pBuffMat->Release();
			pBody->FileData.aParts[nCntParts].FileData.pBuffMat = NULL;
		}

		//�e�N�X�`���̔p��
		for (int nCntTexture = 0; nCntTexture < MAX_MODELTEX; nCntTexture++)
		{
			if (pBody->FileData.aParts[nCntParts].FileData.apTexture[nCntTexture] != NULL)
			{
				pBody->FileData.aParts[nCntParts].FileData.apTexture[nCntTexture]->Release();
				pBody->FileData.aParts[nCntParts].FileData.apTexture[nCntTexture] = NULL;
			}
		}
	}
}

//===================================================
//���[�V�����̍X�V����
//===================================================
void UpdateMotion(Body *pBody)
{
	if (pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey > 0)
	{//�L�[�������݂��Ă���ꍇ
		int nNextkey = (pBody->nNowKey + 1) % pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey;	//���̃L�[

		for (int nCntParts = 0; nCntParts < pBody->FileData.nNumParts; nCntParts++)
		{
			//�������Z�o
			float fPosDiffX = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosX	//X���W
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosX;
			float fPosDiffY = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosY	//Y���W
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosY;
			float fPosDiffZ = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fPosZ	//Z���W
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosZ;
			float fRotDiffX = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotX	//X����
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotX;
			float fRotDiffY = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotY	//Y����
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotY;
			float fRotDiffZ = pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[nNextkey].aKey[nCntParts].fRotZ	//Z����
				- pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotZ;

			//���݂̃t���[���̍��W�����߂�
			pBody->aSet[nCntParts].fPosX = pBody->FileData.aParts[nCntParts].SetPos.x + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosX	//X���W
				+ fPosDiffX * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fPosY = pBody->FileData.aParts[nCntParts].SetPos.y + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosY	//Y���W
				+ fPosDiffY * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fPosZ = pBody->FileData.aParts[nCntParts].SetPos.z + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fPosZ	//Z���W
				+ fPosDiffZ * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);

			//���݂̃t���[���̌��������߂�
			pBody->aSet[nCntParts].fRotX = pBody->FileData.aParts[nCntParts].SetRot.x + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotX	//X����
				+ fRotDiffX * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fRotY = pBody->FileData.aParts[nCntParts].SetRot.y + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotY	//Y����
				+ fRotDiffY * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
			pBody->aSet[nCntParts].fRotZ = pBody->FileData.aParts[nCntParts].SetRot.z + pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].aKey[nCntParts].fRotZ	//Z����
				+ fRotDiffZ * ((float)pBody->nNowFrame / (float)pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame);
		}

		pBody->nNowFrame++;

		if (pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame != 0)
		{//�t���[����0�ł͂Ȃ��ꍇ
			if (pBody->nNowFrame % pBody->FileData.aMotionInfo[pBody->nNowMotion].aKeyInfo[pBody->nNowKey].nFrame == 0)
			{//���݂̃t���[���������B�����ꍇ
				pBody->nNowKey = (pBody->nNowKey + pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey - 1) % pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey;	//���̃L�[�Ɉړ�

				if (pBody->nNowKey == pBody->FileData.aMotionInfo[pBody->nNowMotion].nNumKey - 1
					&& pBody->FileData.aMotionInfo[pBody->nNowMotion].bLoop == false)
				{//���[�v�I�t�̏ꍇ
					pBody->nNowMotion = MOTIONTYPE_NEUTRAL;
					pBody->nNowKey = 0;
					pBody->nNowFrame = 0;
				}
				else
				{
					pBody->nNowFrame = 0;
				}
			}
		}
		else
		{//0�̏ꍇ
			pBody->nNowKey = 0;
			pBody->nNowFrame = 0;
		}
	}
}

//===================================================
//���[�V�����̐ݒ菈��
//===================================================
void SetMotion(Body *pBody, int nMotionType)
{
	if (pBody->nNowMotion != nMotionType)
	{//�w��̃��[�V�����ƌ��݂̃��[�V�������Ⴄ�ꍇ
		pBody->nNowMotion = nMotionType;	//���[�V�����^�C�v��ύX
		pBody->nNowFrame = 0;	//�t���[�������Z�b�g
		pBody->nNowKey = 0;		//���݂̃L�[�����Z�b�g
	}
}

//==========================================================
//�p�[�c��X�t�@�C�����ǂݍ���
//==========================================================
void LoadPartsFileData(Parts *pParts, const char *pFileName)
{
	int nNumVtx;		//���_��
	DWORD dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;		//���_�o�b�t�@�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pParts->FileData.pBuffMat,
		NULL,
		&pParts->FileData.dwNumMat,
		&pParts->FileData.pMesh);

	//���_�����擾
	nNumVtx = pParts->FileData.pMesh->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(pParts->FileData.pMesh->GetFVF());

	//���_�o�b�t�@�����b�N
	pParts->FileData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	//���_���W�̑��

		if (vtx.x < pParts->FileData.vtxMin.x)
		{//�ŏ��l�����l���������ꍇ
			pParts->FileData.vtxMin.x = vtx.x;
		}
		else if (vtx.x > pParts->FileData.vtxMax.x)
		{//�ő�l�����l���傫���ꍇ
			pParts->FileData.vtxMax.x = vtx.x;
		}

		if (vtx.z < pParts->FileData.vtxMin.z)
		{//�ŏ��l�����l���������ꍇ
			pParts->FileData.vtxMin.z = vtx.z;
		}
		else if (vtx.z > pParts->FileData.vtxMax.z)
		{//�ő�l�����l���傫���ꍇ
			pParts->FileData.vtxMax.z = vtx.z;
		}

		if (vtx.y < pParts->FileData.vtxMin.y)
		{//�ŏ��l�����l���������ꍇ
			pParts->FileData.vtxMin.y = vtx.y;
		}
		else if (vtx.y > pParts->FileData.vtxMax.y)
		{//�ő�l�����l���傫���ꍇ
			pParts->FileData.vtxMax.y = vtx.y;
		}

		pVtxBuff += dwSizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
	}

	//���_�o�b�t�@���A�����b�N
	pParts->FileData.pMesh->UnlockVertexBuffer();

	D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)pParts->FileData.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pParts->FileData.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂��Ă���
		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice, pMat[nCntMat].pTextureFilename, &pParts->FileData.apTexture[nCntMat]);
		}
	}

	//�|�C���^���w��
	pParts->model.pModelFiledata = &pParts->FileData;
}

//==========================================================
//�p�[�c�̏��e�L�X�g�ǂݍ���
//==========================================================
void OpenBodyFile(const char *pFileData, BodyFileData *pBody)
{
	FILE *pFile;

	pFile = fopen(pFileData, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		char aStr[256];

		//�J�n�����܂œǂݍ���
		while (1)
		{
			//�e�L�X�g�ǂݍ���
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], START_SCRIPTTXT) == 0)
			{//�X�N���v�g�J�n�̕������m�F�ł����ꍇ
			 //�e�f�[�^�̓ǂݍ��݊J�n
				LoadBodyFile(pFile, pBody);
				break;
			}
			else if (nResult == EOF)
			{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
				break;
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		return;
	}
}

//==========================================================
//�p�[�c�e�L�X�g���ǂݍ���
//==========================================================
void LoadBodyFile(FILE *pFile, BodyFileData *pBodyFile)
{
	char aStr[256];
	int nMotion = 0;	//�ǂݍ��݃��[�V�����ԍ�

	//���ǂݍ��݊J�n
	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], MODELNUM_TXT) == 0)
		{//�g�p���郂�f�����̏ꍇ
			fscanf(pFile, "%s", &aStr[0]); //=
			fscanf(pFile, "%d", &pBodyFile->nNumParts); //�p�[�c���擾

			//�t�@�C�����擾
			LoadBodyXFileName(pFile, pBodyFile);
		}
		else if (strcmp(&aStr[0], CHARACTERSET_TXT) == 0)
		{//�L�����N�^�[���̏ꍇ
			LoadPartsSetData(pFile, pBodyFile);
		}
		else if (strcmp(&aStr[0], MOTIONSET_TXT) == 0)
		{//���[�V�������̏ꍇ
			LoadMotionData(pFile, pBodyFile, nMotion);
			nMotion++;	//���[�V�����ԍ���i�߂�
		}
		else if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//�X�N���v�g�I���̕������m�F�ł����ꍇ
			break;
		}
		else if (nResult == EOF)
		{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
			break;
		}
	}
}

//==========================================================
//�p�[�c���f���t�@�C�����ǂݍ���
//==========================================================
void LoadBodyXFileName(FILE *pFile, BodyFileData *pBodyFile)
{
	int nParts = 0;		//���݂̃p�[�c�ԍ�
	char aStr[128] = {};	//�z�u�������i�[�p

	while (1)
	{
		if (nParts >= pBodyFile->nNumParts)
		{//�p�[�c�����܂œ��B�����ꍇ
			break;
		}
		else
		{//���B���Ă��Ȃ��ꍇ
		 //�e�L�X�g�ǂݍ���
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], MODELFILENAME_TXT) == 0)
			{//���f�����ǂݍ���
				fscanf(pFile, "%s", &aStr[0]); //=
				fscanf(pFile, "%s", &aStr[0]); //�t�@�C����

				//X�t�@�C������ǂݍ���
				LoadPartsFileData(&pBodyFile->aParts[nParts], &aStr[0]);
				nParts++;
			}
			if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
			{//�X�N���v�g�I���̕������m�F�ł����ꍇ
				break;
			}
			else if (nResult == EOF)
			{//�t�@�C���̍Ō�܂œǂݍ���ł��܂����ꍇ
				break;
			}
		}
	}
}

//==========================================================
//�p�[�c�̏����ݒu���ǂݍ���
//==========================================================
void LoadPartsSetData(FILE *pFile, BodyFileData *pBodyFile)
{
	char aStr[128] = {};	//�z�u�������i�[�p
	int nCntParts = 0;
	while (1)
	{
		//�e�L�X�g�ǂݍ���
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], PARTSSET_TXT) == 0)
		{//�p�[�c���m�F�����������ꍇ
			while (1)
			{
				//�ǂݍ���
				fscanf(pFile, "%s", &aStr[0]);

				if (nCntParts >= pBodyFile->nNumParts)
				{//�p�[�c�����ɓ��B�����ꍇ
					break;
				}
				if (strcmp(&aStr[0], LOAD_POS) == 0)
				{//���W
					fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetPos.x);	//x���W�ǂݍ���
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetPos.y);	//y���W�ǂݍ���
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetPos.z);	//z���W�ǂݍ���
					pBodyFile->aParts[nCntParts].model.pos = pBodyFile->aParts[nCntParts].SetPos;
				}
				else if (strcmp(&aStr[0], LOAD_ROT) == 0)
				{//����
					fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetRot.x);	//x���W�ǂݍ���
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetRot.y);	//y���W�ǂݍ���
					fscanf(pFile, "%f", &pBodyFile->aParts[nCntParts].SetRot.z);	//z���W�ǂݍ���
					pBodyFile->aParts[nCntParts].model.rot = pBodyFile->aParts[nCntParts].SetRot;
				}
				else if (strcmp(&aStr[0], LOAD_INDEX) == 0)
				{//�p�[�c�ԍ�
					fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
					fscanf(pFile, "%d", &pBodyFile->aParts[nCntParts].nIndex);	//�ǂݍ���
				}
				else if (strcmp(&aStr[0], LOAD_PARENT) == 0)
				{//�e�ԍ�
					fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
					fscanf(pFile, "%d", &pBodyFile->aParts[nCntParts].nIdexParent);	//�ǂݍ���
				}
				else if (strcmp(&aStr[0], ENDPARTSSET_TXT) == 0)
				{//�p�[�c���I�������m�F�ł����ꍇ
					nCntParts++;
				}
			}


		}
		if (strcmp(&aStr[0], ENDCHARACTERSET_TXT) == 0)
		{//�L�����N�^�[���I�������m�F�ł����ꍇ
			return;	//�Ȃɂ������Ԃ�
		}
	}
}

//==========================================================
//���[�V�������ǂݍ���
//==========================================================
void LoadMotionData(FILE *pFile, BodyFileData *pBodyFile, int nMotion)
{
	char aStr[128] = {};	//�z�u�������i�[�p
	int nCntParts = 0;
	int nCntKey = 0;
	bool bKey = false;
	while (1)
	{
		//�e�L�X�g�ǂݍ���
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (nCntKey >= pBodyFile->aMotionInfo[nMotion].nNumKey && bKey == true)
		{//�L�[���ɗ����ꍇ
			break;
		}
		if (strcmp(&aStr[0], NUMLOOP_TXT) == 0)
		{//���[�v�m�F
			int nLoop = 0;
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &nLoop);	//(=)�ǂݍ���

			if (nLoop == 0)
			{//���[�v���Ȃ��ꍇ
				pBodyFile->aMotionInfo[nMotion].bLoop = false;	//���[�v���Ȃ��悤�ɂ���
			}
			else
			{
				pBodyFile->aMotionInfo[nMotion].bLoop = true;	//���[�v����悤�ɂ���
			}
		}
		else if (strcmp(&aStr[0], NUMKEY_TXT) == 0)
		{//�L�[��
			fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
			fscanf(pFile, "%d", &pBodyFile->aMotionInfo[nMotion].nNumKey);	//x���W�ǂݍ���
			bKey = true;	//�L�[�����擾������Ԃɂ���
		}
		else if (strcmp(&aStr[0], KEYSET_TXT) == 0)
		{//�p�[�c���m�F�����������ꍇ
			bool bFrame = false;
			nCntParts = 0;
			while (1)
			{
				//�ǂݍ���
				fscanf(pFile, "%s", &aStr[0]);

				if (nCntParts >= pBodyFile->nNumParts)
				{//�p�[�c�����ɓ��B�����ꍇ
					break;
				}
				else if (strcmp(&aStr[0], NUMFRAME_TXT) == 0)
				{//�t���[����
					fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
					fscanf(pFile, "%d", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].nFrame);	//�ǂݍ���
					bFrame = true;
				}
				else if (strcmp(&aStr[0], KEY_TXT) == 0 && bFrame == true)
				{//�e�ԍ�
					while (1)
					{
						//�ǂݍ���
						fscanf(pFile, "%s", &aStr[0]);

						if (strcmp(&aStr[0], ENDKEY_TXT) == 0)
						{//�t���[����
							break;
						}
						else if (strcmp(&aStr[0], LOAD_POS) == 0)
						{//�t���[����
							fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX);	//x���W�ǂݍ���
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY);	//y���W�ǂݍ���
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);	//z���W�ǂݍ���
						}
						else if (strcmp(&aStr[0], LOAD_ROT) == 0)
						{//�t���[����
							fscanf(pFile, "%s", &aStr[0]);	//(=)�ǂݍ���
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX);	//x���W�ǂݍ���
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY);	//y���W�ǂݍ���
							fscanf(pFile, "%f", &pBodyFile->aMotionInfo[nMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);	//z���W�ǂݍ���
						}
					}
					nCntParts++;
				}
				else if (strcmp(&aStr[0], ENDKEYSET_TXT) == 0)
				{//�p�[�c���I�������m�F�ł����ꍇ
					break;
				}
			}
			nCntKey++;	//�L�[���ړ�
		}
		if (strcmp(&aStr[0], ENDMOTIONSET_TXT) == 0)
		{//�L�����N�^�[���I�������m�F�ł����ꍇ
			return;	//�Ȃɂ������Ԃ�
		}
	}
}

//==========================================================
//���g����������
//==========================================================
void ResetBodyData(Body * pBody)
{
	pBody->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	pBody->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	pBody->nNowFrame = 0;	//�t���[��
	pBody->nNowKey = 0;		//�L�[��
}

//==========================================================
//���g�t�@�C����񏉊�������
//==========================================================
void ResetBodyFileData(Body * pBody)
{
	pBody->FileData.nNumParts = 0;

	//�p�[�c���
	for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
	{
		pBody->FileData.aParts[nCntParts].FileData.nType = 0;
		pBody->FileData.aParts[nCntParts].FileData.dwNumMat = 0;
		pBody->FileData.aParts[nCntParts].FileData.vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);
		pBody->FileData.aParts[nCntParts].FileData.vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);
		pBody->FileData.aParts[nCntParts].FileData.pBuffMat = NULL;
		pBody->FileData.aParts[nCntParts].FileData.pMesh = NULL;
		pBody->FileData.aParts[nCntParts].model.bUse = false;
		pBody->FileData.aParts[nCntParts].model.fMoveY = 0.0f;
		pBody->FileData.aParts[nCntParts].model.nShadow = -1;
		pBody->FileData.aParts[nCntParts].model.nType = 0;
		pBody->FileData.aParts[nCntParts].model.pModelFiledata = NULL;
		pBody->FileData.aParts[nCntParts].model.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBody->FileData.aParts[nCntParts].model.posOld = pBody->FileData.aParts[nCntParts].model.pos;
	}

	//���[�V�������
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{
		pBody->FileData.aMotionInfo[nCntMotion].bLoop = false;
		pBody->FileData.aMotionInfo[nCntMotion].nNumKey = 0;
		
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY; nCntKeyInfo++)
		{
			pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame = 0;

			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosX = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosY = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosZ = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotX = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotY = 0.0f;
				pBody->FileData.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotZ = 0.0f;
			}
		}
	}
}
