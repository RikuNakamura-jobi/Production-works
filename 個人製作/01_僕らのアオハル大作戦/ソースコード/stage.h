//===========================
//
//�_�C���N�gX.stage�w�b�_�[
//Author:�����@��
//
//===========================
#ifndef _STAGE_H_
#define _STAGE_H_

//�X�e�[�W�̏��
typedef enum
{
	STAGESTATE_NONE = 0,			//�������ĂȂ����
	STAGESTATE_1,					//STAGE1���
	STAGESTATE_2,					//STAGE2���
	STAGESTATE_3,					//STAGE3���
	STAGESTATE_4,					//STAGE4���
	STAGESTATE_5,					//STAGE5���
	STAGESTATE_6,					//STAGE6���
	STAGESTATE_7,					//STAGE7���
	STAGESTATE_8,					//STAGE8���
	STAGESTATE_9,					//STAGE9���
	STAGESTATE_10,					//STAGE10���
	STAGESTATE_MAX
}STAGESTATE;

//�v���g�^�C�v�錾
void InitStage(void);
void SetStageState(STAGESTATE state, int nCounter);
int GetStageType();

#endif // !_STAGE_H_