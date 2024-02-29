//==========================================================
//
//�e�N�X�`������ [texture.h]
//Author Ibuki Okusada
//
//==========================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"	//main.h�Œ�`���Ă�����̂��K�v�Ȃ���include

//==========================================================
//�}�N����`
//==========================================================
#define MAX_TEXTUREFILE	(256)	//�ǂݍ��߂�e�N�X�`���̍ő吔

//�񋓌^�̒�`
typedef enum
{
	TEXTURETYPE_LOAD = 0,	//�n�ʓ��H
	TEXTURETYPE_MAX
}TEXTURETYPE;

//==========================================================
//�v���g�^�C�v�錾
//==========================================================
void InitTexture(void);
void UninitTexture(void);
void UpdateTexture(void);
LPDIRECT3DTEXTURE9 *SetTexture(int nIdx);

#endif
