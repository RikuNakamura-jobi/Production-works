//========================================
//
//2D�V���[�e�B���O�Q�[��(�A�C�e���擾����)[getitem.h]
//Author:���v�ԗD��
//
//========================================
#ifndef _OBJECT_H_							//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT_H_							//2�d�C���N���[�h�h�~�̃}�N�����`����v

#include"main.h"

//====================
// �}�N����`
//====================
#define MAX_OBJECT	(120)	//���f���̍ő吔
#define NUM_TEX_OBJECT	(10)	// �e�N�X�`���̐�

//===================
// �񋓌^�錾
//===================
//�I�u�W�F�N�g�̎��
typedef enum
{
	OBJECTTYPE_FIRST = 0,					//��
	OBJECTTYPE_CATAPULT,					//���ˑ�
	OBJECTTYPE_REFLECTOR,					//���˔�
	OBJECTTYPE_REFLECTOR_BASE,				//���˔̑��
	OBJECTTYPE_LEVER_BAR,					//������̃��o�[�̖_
	OBJECTTYPE_LEVER_BASE,					//������̃��o�[�̎x�_
	OBJECTTYPE_TORCH,						//����
	OBJECTTYPE_BUTTON,						//�{�^��
	OBJECTTYPE_SANDPOLE,					//���̒�
	OBJECTTYPE_SANDPOLE_BASE,				//���̒��̑��
	OBJECTTYPE_STONEPOLE,					//�Β�
	OBJECTTYPE_DOOR,						//��
	OBJECTTYPE_BOOK,						//�{
	OBJECTTYPE_ROCK_FRAG,					//��̔j��
	OBJECTTYPE_RUBBLE,						//���ꂫ
	OBJECTTYPE_STATUE_0,					//����0
	OBJECTTYPE_DEBRIS,						//�c�[
	OBJECTTYPE_SKULL,						//���W��
	OBJECTTYPE_SKULLMT,						//���W���̎R
	OBJECTTYPE_STATUE_1,					//���H����1
	OBJECTTYPE_STATUE_3,					//���l�T���X����3
	OBJECTTYPE_STATUE_4,					//����������4
	OBJECTTYPE_STATUE_5,					//����������5
	OBJECTTYPE_STATUE_6,					//����������6
	OBJECTTYPE_STATUE_7,					//����������7
	OBJECTTYPE_STATUE_8,					//����������8
	OBJECTTYPE_SWORD,						//�n�ʂɎh��������
	OBJECTTYPE_TREASUREBOX,					//��
	OBJECTTYPE_STATUE_100,					//����L����
	OBJECTTYPE_STATUE_101,					//�W���b�N�I�[����
	OBJECTTYPE_BUTTON_POLE,					//�Β������{�^��
	OBJECTTYPE_MAX
}OBJECTTYPE;

//�F�̎��
typedef enum
{
	COLTYPE_NORMAL = 0,						//�ʏ펞
	COLTYPE_DAMAGE,							//�_���[�W��
	COLTYPE_DEATH,							//���S��
	COLTYPE_EDIT,							//�G�f�B�b�g��
	COLTYPE_DELETETARGET,					//�폜�I����
	COLTYPE_MAX,
}COLTYPE;

//�I�u�W�F�N�g�̏��
typedef enum
{
	OBJECTSTATE_NORMAL = 0,					//�ʏ�
	OBJECTSTATE_DAMEGE,						//�_���[�W���
	OBJECTSTATE_DEATH,						//���S���
	OBJECTSTATE_MAX
}BLOCKSTATE;

//====================
// �\���̒�`
//===================
//X�t�@�C���\���̂̒�`
typedef struct
{
	LPD3DXMESH pMesh;									//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;								//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;										//�}�e���A���̐�	
	D3DXVECTOR3 vtxMin, vtxMax;							//�ő�l�A�ŏ��l	
	LPDIRECT3DTEXTURE9 apTexture[10];					//�e�N�X�`���ւ̃|�C���^
}Xfile;

//�I�u�W�F�N�g�\����
typedef struct
{
	D3DXVECTOR3 pos;									//�ʒu
	D3DXVECTOR3 posOld;									//�O��̈ʒu
	D3DXVECTOR3 move;									//�ړ���
	D3DXVECTOR3 rot;									//����
	D3DXVECTOR3 rotBase;								//�����̊(���o�[�p)
	D3DXVECTOR3 rotate;									//��]��
	D3DXVECTOR3 vtxMin;									//�ŏ�
	D3DXVECTOR3 vtxMax;									//�ő�
	D3DXVECTOR3 point[4];								//����̊�_
	D3DXVECTOR3 WindMove;								//���̈ړ���
	D3DXVECTOR3 posBlock;								//�����ʒu
	D3DXVECTOR3 posCollision;							//�����蔻��̃I�t�Z�b�g
	D3DXMATRIX mtxWorld;								//���[���h�}�g���b�N�X
	D3DXMATRIX mtxWorldCollision;						//�����蔻��̃��[���h�}�g���b�N�X
	int nType;											//���
	bool bUse;											//�g�p���Ă��邩�ǂ���
	int nCounter;										//�₪�o��悤�̃J�E���^�[
	int nCounterObject;									//�J�E���^�[	
	BLOCKSTATE state;									//���
	int nCounterState;									//��ԊǗ��J�E���^�[
	D3DXMATERIAL RedMat;								//�}�e���A���f�[�^�ւ̃|�C���^
	int nLife;											//�̗�
	int nIdxEdit;										//�G�f�B�b�g�̔ԍ�
	int nIdxBillboard;									//�r���{�[�h�̔ԍ�
	int nIdxshadow;										//�e�̔ԍ�
	bool bBillboard;									//�r���{�[�h��\�����Ă��邩�ǂ���
	int RockCounter;									//�J�E���^�[
	int nCounterEff;									//�G�t�F�N�g���o���J�E���^�[
	int nCntSound;										//�����o���J�E���^�[
	float fScale;										//�X�P�[��
}Object;

//====================
// �v���g�^�C�v�錾
//====================
//��{����
void InitObject(void);
void UninitObject(void);
void UpdateObject(void);
void DrawObject(void);
void UpdateObjectRanking(float moveX);

//�����蔻��
bool CollisionObject2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool bJump);
bool CollisionRockInvisible(D3DXVECTOR3 *pos, float *vtxMax);
void HitObject(int nDamage, int nCntObject);
bool CollisionReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float fLength, int type);
bool PushReflector(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, float *rot, bool jump);

//���擾����
Object *ObjectGet(void);
LPD3DXMESH ObjectMeshGet(OBJECTTYPE type);
LPD3DXBUFFER ObjectBuffmatGet(OBJECTTYPE type);
DWORD ObjectdwNummatGet(OBJECTTYPE type);
int GetUseObject(void);
float LengthPointToLine(D3DXVECTOR3 point, D3DXVECTOR3 line1, D3DXVECTOR3 line2);

//�ݒ菈��
void SetObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
void DeleteObject(int nIdx);
void SetPoint(int nNum);
void SetBreakObj(D3DXVECTOR3 pos, int nNum);

//���̑�
void WindMove(int nCntRock);

//�f�o�b�O�\��
void DebugObject(void);

#endif