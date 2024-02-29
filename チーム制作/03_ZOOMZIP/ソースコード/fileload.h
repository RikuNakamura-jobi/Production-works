//==========================================================
//
//�O���t�@�C���ǂݍ��ݏ��� [fileload.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _FILELOAD_H_
#define _FILELOAD_H_

#include "main.h"			//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�}�N����`
//==========================================================
//�m�F����
#define START_SCRIPTTXT	"SCRIPT"		//�X�N���v�g�J�n�m�F����
#define END_SCRIPTTXT	"END_SCRIPT"	//�X�N���v�g�I���m�F����
#define TEXTURENUM_TXT	"NUM_TEXTURE"	//�e�N�X�`���g�p�����m�F����
#define TEXTUREFILENAME_TXT	"TEXTURE_FILENAME"	//�e�N�X�`���t�@�C�����m�F����
#define MODELNUM_TXT	"NUM_MODEL"				//���f���g�p�m�F����
#define MODELFILENAME_TXT	"MODEL_FILENAME"	//���f���t�@�C�����m�F����
#define FIELDSET_TXT	"FIELDSET"		//���b�V���t�B�[���h�ݒu�m�F����
#define WALLSET_TXT		"WALLSET"		//���b�V���E�H�[���ݒu�m�F����
#define MODELSET_TXT	"MODELSET"		//���f���z�u�m�F����
#define MOTIONSET_TXT	"MOTIONSET"		//���[�V�����m�F����
#define NUMKEY_TXT		"NUM_KEY"		//���ǂݍ��݊m�F����
#define NUMLOOP_TXT		"LOOP"			//�p�[�c���ǂݍ��݊m�F����
#define NUMFRAME_TXT	"FRAME"			//�t���[���ǂݍ��݊m�F����
#define SETMOTION_TXT	"MOTIONSET"		//���[�V�������m�F����
#define KEYSET_TXT		"KEYSET"		//�L�[�Z�b�g���m�F����
#define KEY_TXT			"KEY"			//�L�[�m�F����
#define LOAD_POS		"POS"			//���W
#define LOAD_ROT		"ROT"			//����
#define LOAD_TEXTYPE	"TEXTYPE"		//�e�N�X�`���ԍ�
#define LOAD_MODELTYPE	"TYPE"			//���f���ԍ�
#define LOAD_BLOCK		"BLOCK"			//����
#define LOAD_SIZE		"SIZE"			//�T�C�Y
#define LOAD_SHADOW		"SHADOW"		//�T�C�Y
#define ENDFIELDSET_TXT	"END_FIELDSET"	//���ǂݍ��ݏI��
#define ENDWALLSET_TXT	"END_WALLSET"	//�Ǔǂݍ��ݏI��
#define ENDMODELSET_TXT	"END_MODELSET"	//���f���ǂݍ��ݏI��
#define ENDMOTIONSET_TXT	"END_MOTIONSET"	//���[�V�������ǂݍ��ݏI��
#define ENDKEYSET_TXT	"END_KEYSET"	//�L�[�Z�b�g���ǂݍ��ݏI��
#define ENDKEY_TXT		"END_KEY"		//�L�[�ǂݍ��ݏI��
#define CHARACTERSET_TXT	"CHARACTERSET"	//�L�����N�^�[���ǂݍ��݊m�F����
#define ENDCHARACTERSET_TXT	"END_CHARACTERSET"	//�L�����N�^�[���ǂݍ��ݏI������
#define PARTSSET_TXT	"PARTSSET"		//�p�[�c�z�u���ǂݍ��݊J�n
#define ENDPARTSSET_TXT	"END_PARTSSET"	//�p�[�c���ǂݍ��ݏI��
#define LOAD_INDEX		"INDEX"			//�p�[�c�ԍ�
#define LOAD_PARENT		"PARENT"		//�e�ԍ�
#define MOTIONSET_TXT	"MOTIONSET"		//���[�V�������m�F����
#define ENDMOTIONSET_TXT	"END_MOTIONSET"	//���[�V�������ǂݍ��ݏI������
#define LOAD_BLOCKTYPE	"BLOCKTYPE"		//���[�V�������m�F����
#define MODELFILE_TXT	"MODELFILESET"	//�t�@�C�����m�F
#define END_MODELFILE_TXT	"END_MODELFILESET"	//�t�@�C�����m�F
#define ITEMTYPE_TXT	"ITEMTYPE"		//�A�C�e����ފm�F����
#define ITEMSET_TXT		"ITEMSET"		//�A�C�e���z�u���m�F����
#define END_ITEMSETTXT	"END_ITEMSET"	//�A�C�e���z�u���ǂݍ��ݏI��
#define PARTSSET_TXT	"PARTSSET"		//�p�[�c�z�u���m�F����
#define END_PARTSSETTXT	"END_PARTSSET"	//�p�[�c�z�u���ǂݍ��ݏI��
#define BODYTYPE_TXT	"BODYTYPE"		//���g�m�F����
#define AREASET_TXT		"AREASET"		//Tips�G���A���m�F����
#define END_AREASETTXT	"END_AREASET"	//Tips�G���A���ǂݍ��ݏI��
#define NUM_AREATXT		"NUM_AREA"		//Tips�G���A��񐔊m�F����
#define CHACKPOS_TXT	"CHACKPOS"		//Tips�`�F�b�N�|�C���g�m�F����
#define PARTSFILETXT		"PARTSFILESET"		//�ǂݍ��݊m�F
#define END_PARTSFILETXT	"END_PARTSFILESET"	//�I���m�F
#define PARTSTYPE_TXT		"PARTSTYPE"			//�p�[�c���
#define PARTSFILENAME_TXT	"PARTS_FILENAME"	//�p�[�c�t�@�C�����m�F

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitFileLoad(void);
void UninitFileLoad(void);
void UpdateFileLoad(void);
void SetFileLoad(void);

//�e�ǂݍ��݃t�@�C����
void GametxtFileLoad(void);
int GetTextureCnt(void);
int GetModelCnt(void);
const char *pTextureFileName(int nCntTxture);
const char *pModelFileName(int nCntModel);

#endif