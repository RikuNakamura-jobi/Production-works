//===========================
//
//�_�C���N�gX.result�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _RESULT_H_
#define _RESULT_H_

//��ʃ��[�h�̎��
typedef enum
{
	RESULT_CLEAR = 0,		//�^�C�g�����
	RESULT_OVER,			//�Q�[�����
	RESULT_MAX,
}RESULT;

//�v���g�^�C�v�錾
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
RESULT GetResult(void);
void SetResult(RESULT Result);

#endif // !_RESULT_H_