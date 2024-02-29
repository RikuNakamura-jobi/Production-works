//==================================================================================================
//
//gauge.cpp
//Author:���R����
//
//==================================================================================================

//===============================
// �C���N���[�h
//===============================
#include "main.h"
#include "gauge.h"
#include "player.h"

//===============================
// �}�N����`
//===============================
#define NUM_TEX		(2)	//�e�N�X�`���̐�
#define MAX_GAUGE	(16)	//�Q�[�W�̍ő吔(�Q�[�W�Ƙg�����邩���{����)

#define PARAM_FACT	(0.1f)	//�p�����[�^�[�̊����̌W��
#define MULTIPLE_POLYGON	(4)	//�|���S�����̔{���@�Q�[�W�A�g�A�w�i�A�����̐F�Ⴂ�̕�
#define DIFF_RANGE	(0.001f)	//�����Q�[�W�������o���܂ł̍����͈�
#define RATE_FRAME	(2.0f)	//�g�̃Q�[�W�̊���

//===============================
// �O���[�o���ϐ��錾
//===============================
LPDIRECT3DTEXTURE9 g_pTextureGauge[NUM_TEX] = {};								//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;									//���_�o�b�t�@�ւ̃|�C���^
const char *c_apFilenameGauge[NUM_TEX] =
{
	"data\\TEXTURE\\life000.png",
	"data\\TEXTURE\\gauge_frame.png"
};																				//�e�N�X�`���t�@�C����
Gauge g_aGauge[MAX_GAUGE];	//�Q�[�W�̍ő吔

//==================================================================================================
//����������
//==================================================================================================
void InitGauge(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���擾
	Gauge *pGauge = GetGauge();

	for (int nCntTex = 0;nCntTex < NUM_TEX;nCntTex++)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameGauge[nCntTex],
			&g_pTextureGauge[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGE * MULTIPLE_POLYGON,	
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//�ϐ�������
	ZeroMemory(pGauge, sizeof(Gauge) * MAX_GAUGE);

	for (int nCntGauge = 0;nCntGauge < MAX_GAUGE * MULTIPLE_POLYGON;nCntGauge++,pGauge++)
	{
		//���_��񏉊���=====================
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[1].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[2].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
		pVtx[3].col = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//�|�C���^��i�߂�
		pVtx += 4;
		//���_��񏉊���=====================
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGauge->Unlock();
}

//==================================================================================================
//�I������
//==================================================================================================
void UninitGauge(void)
{
	for (int nCntGauge = 0;nCntGauge < NUM_TEX;nCntGauge++)
	{
		if (g_pTextureGauge[nCntGauge] != NULL)
		{//�e�N�X�`���ւ̃|�C���^�̔j��
			g_pTextureGauge[nCntGauge]->Release();
			g_pTextureGauge[nCntGauge] = NULL;
		}
	}

	if (g_pVtxBuffGauge != NULL)
	{//���_�o�b�t�@�ւ̃|�C���^�̔j��
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//==================================================================================================
//�X�V����
//==================================================================================================
void UpdateGauge(void)
{
	//�ϐ��錾
	float fLength;
	float fGaugeDiff;

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	//���擾
	Gauge *pGauge = GetGauge();

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse)
		{//�g�p����Ă�����
			//�p�����[�^�[��ړI�̒l�Ɋ񂹂�
			fGaugeDiff = pGauge->fParamDest - pGauge->fParam;

			if (fGaugeDiff > 0.0f)
			{//�������v���X�̏ꍇ
				pGauge->fParam += pGauge->fParamMax * pGauge->fRateChange;

				if (pGauge->fParam > pGauge->fParamDest)
				{//�ڕW�̒l�ɕ␳
					pGauge->fParam = pGauge->fParamDest;
				}
			}
			else if(fGaugeDiff < 0.0f)
			{//�������}�C�i�X�̏ꍇ
				pGauge->fParam -= pGauge->fParamMax * pGauge->fRateChange;

				if (pGauge->fParam < pGauge->fParamDest)
				{//�ڕW�̒l�ɕ␳
					pGauge->fParam = pGauge->fParamDest;
				}
			}

			//pGauge->fParam += fGaugeDiff * PARAM_FACT;

			//�p�����[�^�[�̊������Z�o
			fLength = pGauge->fParam / pGauge->fParamMax;

			//���_�J���[�̐ݒ�
			pVtx[0].col = pGauge->col;
			pVtx[1].col = pGauge->col;
			pVtx[2].col = pGauge->col;
			pVtx[3].col = pGauge->col;

			//��ނ��Ƃ̒��_���̐ݒ�
			switch (pGauge->type)
			{
			case GAUGETYPE_VERTICAL:
				//�����^�C�v=======================
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y, 0.0f);

				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f * fLength);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f * fLength);

				//�Q�[�W�����܂Ń|�C���^��i�߂�
				pVtx += 4 * MAX_GAUGE * 3;

				//�Q�[�W�����̒��_���ݒ�
				//���_���W�̐ݒ�
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x - pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * 0.5f, pGauge->pos.y - pGauge->height * fLength, 0.0f);

				//�����Q�[�W�̎���
				if (fGaugeDiff * fGaugeDiff < DIFF_RANGE * DIFF_RANGE)
				{//���������ȉ��̂Ƃ��̂ݍX�V
					pVtx[0].pos += (pVtx[2].pos - pVtx[0].pos) * PARAM_FACT;
					pVtx[1].pos += (pVtx[3].pos - pVtx[1].pos) * PARAM_FACT;
				}
				//�����^�C�v=======================
				break;

			case GAUGETYPE_HORIZON:
				//�����^�C�v=======================
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);

				//�e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(1.0f - fLength, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f - fLength, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				//�Q�[�W�����܂Ń|�C���^��i�߂�
				pVtx += 4 * MAX_GAUGE * 3;

				//�Q�[�W�����̒��_���ݒ�
				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y - pGauge->height * 0.5f, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pGauge->pos.x + pGauge->width * fLength, pGauge->pos.y + pGauge->height * 0.5f, 0.0f);

				//�����Q�[�W�̎���
				if (fGaugeDiff * fGaugeDiff < DIFF_RANGE * DIFF_RANGE)
				{//���������ȉ��̂Ƃ��̂ݍX�V
					pVtx[1].pos += (pVtx[0].pos - pVtx[1].pos) * PARAM_FACT;
					pVtx[3].pos += (pVtx[2].pos - pVtx[3].pos) * PARAM_FACT;
				}
				
				//�����^�C�v=======================
				break;
			default:
				break;
			}

			//�|�C���^��߂�
			pVtx -= 4 * MAX_GAUGE * 3;
		}

		//�|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGauge->Unlock();
}

//==================================================================================================
//�`�揈��
//==================================================================================================
void DrawGauge(void)
{
	//�f�o�C�X�|�C���^�ݒ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���擾
	Gauge *pGauge = GetGauge();

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse)
		{//�g�p����Ă�����
			//�w�i�̕`��=========================
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, NULL);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4 * 2, 2);

			//�����Q�[�W�̕`��=========================
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, NULL);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4 * 3, 2);

			//�Q�[�W�̕`��=========================
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureGauge[0]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4, 2);

			//�g�̕`��=========================
			//�e�N�X�`���ݒ�
			pDevice->SetTexture(0, g_pTextureGauge[1]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGauge * 4 + MAX_GAUGE * 4, 2);
		}
	}
}

//==================================================================================================
// �ݒ菈��
//==================================================================================================
int SetGauge(D3DXVECTOR3 pos,float fParamMax, float height, float width, GAUGETYPE type, float fRateChange)
{
	//�ϐ��錾
	int nCntGauge = -1;

	//���擾
	Gauge *pGauge = GetGauge();

	//���_���̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_GAUGE; nCount++, nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == false)
		{//�g�p���Ă��Ȃ���ԂȂ�

			//�����̎󂯎��
			pGauge->fParamMax = fParamMax;
			pGauge->fParam = fParamMax;
			pGauge->fParamDest = fParamMax;
			pGauge->fRateChange = fRateChange;
			pGauge->height = height;
			pGauge->width = width;
			pGauge->pos = pos;
			pGauge->type = type;

			//�Q�[�W�̐ݒ�========================================
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//�Q�[�W�̐ݒ�========================================

			//�g�܂Ń|�C���^��i�߂�
			pVtx += 4 * MAX_GAUGE;

			//�g�̐ݒ�========================================
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + height * RATE_FRAME, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + height * RATE_FRAME, pos.y + height * 0.5f, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//�g�̐ݒ�========================================

			//�w�i�܂Ń|�C���^��i�߂�
			pVtx += 4 * MAX_GAUGE;

			//�w�i�̐ݒ�========================================
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height * 0.5f, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[1].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[2].col = D3DCOLOR_RGBA(200, 200, 200, 200);
			pVtx[3].col = D3DCOLOR_RGBA(200, 200, 200, 200);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//�w�i�̐ݒ�========================================

			//�����Q�[�W�܂Ń|�C���^��i�߂�
			pVtx += 4 * MAX_GAUGE;

			//�����Q�[�W�̐ݒ�========================================
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - height * 0.5f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, 0.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 215, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 215, 0, 255);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			//�����Q�[�W�̐ݒ�========================================

			nCntGauge = nCount;

			//�g�p��Ԃɂ���
			pGauge->bUse = true;

			//for���𔲂���
			break;
		}

		//�|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffGauge->Unlock();

	return nCntGauge;
}

//==================================================================================================
// ���ݒ菈��
//==================================================================================================
void SetInfoGauge(int nIdx, D3DXCOLOR col, float fParam)
{
	if (nIdx == -1)
	{//�������j��h�~
		return;
	}

	//���擾
	Gauge *pGauge = GetGauge();

	//�F�̐ݒ�
	pGauge[nIdx].col = col;

	//�p�����[�^�[�̐ݒ�
	pGauge[nIdx].fParamDest = fParam;
}

//==================================================================================================
// ���擾����
//==================================================================================================
Gauge *GetGauge(void)
{
	return &g_aGauge[0];
}