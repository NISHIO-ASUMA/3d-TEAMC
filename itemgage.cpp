//==============================================================================================================
//
// �A�C�e���̃Q�[�W [itemgage.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================
#include "itemgage.h"
#include "item.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_LENGTH (150.0f) // ����

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureItemGage[ITEMGAGE_MAX] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemGage = NULL;// ���_�o�b�t�@�ւ̃|�C���^
ItemGage g_ItemGage[ITEMGAGE_MAX]; // �A�C�e���Q�[�W�̏��

//==============================================================================================================
// �A�C�e���̃Q�[�W�̏���������
//==============================================================================================================
void InitItemGage(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			ITEMGAGE_INFO[nCnt],
			&g_pTextureItemGage[nCnt]);
	}

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * ITEMGAGE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemGage,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffItemGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
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

		// ���_�o�b�t�@�̃|�C���^��i�߂�
		pVtx += 4;
	}
	//�A�����b�N
	g_pVtxBuffItemGage->Unlock();

	SetItemGage(D3DXVECTOR3(5.0f,630.0f,0.0f),ITEMGAGE_FRAME,8.0f);
	SetItemGage(D3DXVECTOR3(5.0f,630.0f, 0.0f), ITEMGAGE_GAGE, 8.0f);
}
//==============================================================================================================
// �A�C�e���̃Q�[�W�̏I������
//==============================================================================================================
void UninitItemGage(void)
{
	// �A�C�e���̎�ޕ���
	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// �e�N�X�`���̉��
		if (g_pTextureItemGage[nCnt] != NULL)
		{
			g_pTextureItemGage[nCnt]->Release();
			g_pTextureItemGage[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̉��
	if (g_pVtxBuffItemGage != NULL)
	{
		g_pVtxBuffItemGage->Release();
		g_pVtxBuffItemGage = NULL;
	}

}
//==============================================================================================================
// �A�C�e���̃Q�[�W�̍X�V����
//==============================================================================================================
void UpdateItemGage(void)
{
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffItemGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// �A�C�e���Q�[�W���g�p��Ԃ�������
		if (g_ItemGage[nCnt].bUse == false)
		{
			continue;
		}

		// �Q�[�W�̊��������߂�
		float RateGage = (float)pItem[pPlayer->ItemIdx].durability / (float)pItem[pPlayer->ItemIdx].Maxdurability;

		// �������璷�������߂�
		g_ItemGage[nCnt].fLength = RateGage * MAX_LENGTH;

		// �ʒu����,����
		D3DXVECTOR3 pos = g_ItemGage[nCnt].pos;
		float fHeight = g_ItemGage[nCnt].fHeight;

		switch (g_ItemGage[nCnt].nType)
		{
		case ITEMGAGE_FRAME:

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + MAX_LENGTH, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + MAX_LENGTH, pos.y + fHeight, 0.0f);

			break;
		case ITEMGAGE_GAGE:
			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + g_ItemGage[nCnt].fLength, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + g_ItemGage[nCnt].fLength, pos.y + fHeight, 0.0f);
			break;
		default:
			break;
		}

		pVtx += 4;
	}

	//�A�����b�N
	g_pVtxBuffItemGage->Unlock();
}
//==============================================================================================================
// �A�C�e���̃Q�[�W�̕`�揈��
//==============================================================================================================
void DrawItemGage(void)
{
	Item* pItem = GetItem();

	Player* pPlayer = GetPlayer();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItemGage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// �A�C�e�����g�p��Ԃ�������
		if (g_ItemGage[nCnt].bUse == true && pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
		{
			// ��ނ���
			int nType = g_ItemGage[nCnt].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItemGage[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//==============================================================================================================
// �A�C�e���̃Q�[�W�̐ݒ菈��
//==============================================================================================================
void SetItemGage(D3DXVECTOR3 pos, int nType,float fHeight)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffItemGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < ITEMGAGE_MAX; nCnt++)
	{
		// �A�C�e�����g�p��Ԃ�������
		if (g_ItemGage[nCnt].bUse == false)
		{
			g_ItemGage[nCnt].pos = pos;
			g_ItemGage[nCnt].nType = nType;
			g_ItemGage[nCnt].fHeight = fHeight;
			g_ItemGage[nCnt].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x, pos.y + fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//�A�����b�N
	g_pVtxBuffItemGage->Unlock();
}
