//==============================================================================================================
//
// manual [manual.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "manual.h"
#include "mark.h"
#include "input.h"
#include "game.h"
#include "gameui.h"
#include "mouse.h"

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void SetUpManualBoss(int nCnt);	// �{�X�̃}�j���A���̐ݒ�

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureManual[MANUALTYPE_MAX] = {};			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Manual g_Manual[MANUALTYPE_MAX];					// �}�j���A���̏��
bool g_bmanual = false; // �}�j���A����ʂ��o����

//=======================================================================================================
// �}�j���A���̏���������
//=======================================================================================================
void InitManual(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ��ޕ���
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			MANUAL_TYPE[nCnt],
			&g_pTextureManual[nCnt]);
	}

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MANUALTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManual,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);

	// �}�j���A����ʂ��o����
	g_bmanual = false;

	// ��ޕ���
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		g_Manual[nCnt].pos = NULLVECTOR3;		// �ʒu
		g_Manual[nCnt].nType = MANUALTYPE_ONE;	// ���
		g_Manual[nCnt].fWidth = 0.0f;			// ����
		g_Manual[nCnt].fHeight = 0.0f;			// ����
		g_Manual[nCnt].bUse = false;			// �g�p���
		g_Manual[nCnt].bFirstSet = true;		// �ŏ��ɃZ�b�g����������ʂ��Ȃ�

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//�A�����b�N
	g_pVtxBuffManual->Unlock();

	// �}�j���A���̐ݒ�
	SetManual(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 550.0f, 250.0f, MANUALTYPE_ONE);
}
//=======================================================================================================
// �}�j���A���̏I������
//=======================================================================================================
void UninitManual(void)
{
	// ��ޕ���
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureManual[nCnt] != NULL)
		{
			g_pTextureManual[nCnt]->Release();
			g_pTextureManual[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffManual != NULL)
	{
		g_pVtxBuffManual->Release();
		g_pVtxBuffManual = NULL;
	}
}
//=======================================================================================================
// �}�j���A���̍X�V����
//=======================================================================================================
bool UpdateManual(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ��ޕ���
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		if (g_Manual[nCnt].bUse == false)
		{
			continue;
		}

		switch (g_Manual[nCnt].nType)
		{
		case MANUALTYPE_ONE:
			SetUpManualBoss(nCnt);
			break;
		default:
			break;
		}
		// �ʒu
		D3DXVECTOR3 pos = g_Manual[nCnt].pos;

		// ����
		float fWidth = g_Manual[nCnt].fWidth;

		// ����
		float fHeight = g_Manual[nCnt].fHeight;

		// ���_�o�b�t�@�̃��b�N
		g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

		//�A�����b�N
		g_pVtxBuffManual->Unlock();
	}

	return g_bmanual;
}
//=======================================================================================================
// �}�j���A���̕`�揈��
//=======================================================================================================
void DrawManual(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ��ޕ���
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		if (g_Manual[nCnt].bUse == true)
		{
			// ���
			int nType = g_Manual[nCnt].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureManual[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//=======================================================================================================
// �}�j���A���̐ݒ菈��
//=======================================================================================================
void SetManual(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);

	// ��ޕ���
	for (int nCnt = 0; nCnt < MANUALTYPE_MAX; nCnt++)
	{
		// ���g�p��������
		if (g_Manual[nCnt].bUse == false)
		{
			g_Manual[nCnt].pos = pos;			// �ʒu
			g_Manual[nCnt].fWidth = fWidth;		// ����
			g_Manual[nCnt].fHeight = fHeight;	// ����
			g_Manual[nCnt].nType = nType;		// ���

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}
	//�A�����b�N
	g_pVtxBuffManual->Unlock();
}
//==============================================================================================================
// �}�j���A���̏�Ԑݒ�
//==============================================================================================================
void EnableManual(bool bManual,int nType)
{
	if (g_Manual[nType].bFirstSet == false) return;

	// ��Ԃ�ݒ�
	g_bmanual = bManual;
	g_Manual[nType].bUse = true;

	switch (nType)
	{
	case MANUALTYPE_ONE:
		SetGameUI(D3DXVECTOR3(1000.0f,670.0f,0.0f),UITYPE_BOSSMANUALEXIT,120.0f,60.0f,false,0);
		break;
	default:
		break;
	}
}
//==============================================================================================================
// �{�X�̃}�j���A���̐ݒ�
//==============================================================================================================
void SetUpManualBoss(int nCnt)
{
	//�}�j���A����ʂ������ł���܂ł̎���
	static int ExitTime = 0;

	ExitTime++;

	// �X�y�[�X���������� && ��x���Z�b�g���ĂȂ����
	if (OnMouseTriggerDown(LEFT_MOUSE) == true && ExitTime >= 180)
	{
		g_bmanual = false;
		g_Manual[nCnt].bUse = false;
		g_Manual[nCnt].bFirstSet = false;
	}
}
//==============================================================================================================
// �}�j���A���̏�Ԃ��擾
//==============================================================================================================
bool GetManualState(void)
{
	return g_bmanual;
}
