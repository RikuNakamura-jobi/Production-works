//==================================================================================================
//
//gauge.h
//Author:���R����
//
//==================================================================================================

#ifndef _GAUGE_H_
#define _GAUGE_H_

//===============================
// �}�N����`
//===============================
#define ENEMY_LIFE_CR	(0.006f)	//�G�̗̓Q�[�W�̕ω��{��
#define PLAYER_LIFE_CR	(0.006f)	//�v���C���[�̗̓Q�[�W�̕ω��{��
#define SKIPGAUGE_CR	(0.05f)	//�X�L�b�v�Q�[�W�̕ω��{��

//===============================
// �񋓌^��`
//===============================
typedef enum
{//�Q�[�W�̌���
	GAUGETYPE_HORIZON = 0,				//����
	GAUGETYPE_VERTICAL,					//����
	GAUGETYPE_MAX
}GAUGETYPE;

//===============================
// �\���̒�`
//===============================
typedef struct
{//�Q�[�W�̍\����
	D3DXVECTOR3 pos;	//�������̂Ƃ���̈ʒu
	D3DXCOLOR col;	//�F
	D3DXCOLOR colFrame;	//�g�F
	D3DXCOLOR colBack;	//�w�i�F
	GAUGETYPE type;	//���
	float fParamMax;	//�p�����[�^�[�̍ő�l
	float fParam;	//�p�����[�^�[�̌��݂̒l
	float fParamOld;	//�p�����[�^�[�̑O��̒l
	float fParamDest;	//�ړI�̃p�����[�^�[�l
	float fRateChange;	//�ω�����{��
	float height;	//����
	float width;	//��
	bool bUse;	//�g�p���Ă��邩�ǂ���
}Gauge;

//===============================
// �v���g�^�C�v�錾
//===============================
// ��{����================
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

// ���擾================
Gauge *GetGauge(void);

// �ݒ菈��================
int SetGauge(D3DXVECTOR3 pos,float fParamMax,float height,float width,GAUGETYPE type,float fRateChange);
void SetInfoGauge(int nIdx,D3DXCOLOR col,float fParam);

#endif
