//==============================================================================================================
//
// �N���t�g�̃��V�s [craftrecipe.cpp]
// Author: YOSHIDA YUTO
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "craftrecipe.h"
#include "mark.h"
#include "item.h"

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRecipe[RECIPETYPE_MAX] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRecipe = NULL; // ���_�o�b�t�@�ւ̃|�C���^
CraftRecipe g_aRecipe[RECIPETYPE_MAX]; // ���V�s�̏��

//==============================================================================================================
// �N���t�g���V�s�̏���������
//==============================================================================================================
void InitCraftRecipe(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ��݃R����
		D3DXCreateTextureFromFile(pDevice,
			RECIPE_TEXTURE[nCnt],
			&g_pTextureRecipe[nCnt]);
	}

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * RECIPETYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRecipe,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		g_aRecipe[nCnt].pos = NULLVECTOR3; // �ʒu
		g_aRecipe[nCnt].nType = nCnt;      // ���
		g_aRecipe[nCnt].fWidth = 0.0f;     // ����
		g_aRecipe[nCnt].fHeight = 0.0f;    // ����
		g_aRecipe[nCnt].bUse = false;      // �g�p���

		//���_���W�̐ݒ�
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

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffRecipe->Unlock();

	SetCraftRecipe(D3DXVECTOR3(400.0f, 400.0f, 0.0f), RECIPETYPE_STONEBAT, 100.0f, 50.0f);
}
//==============================================================================================================
// �N���t�g���V�s�̏I������
//==============================================================================================================
void UninitCraftRecipe(void)
{
	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̔j��
		if (g_pTextureRecipe[nCnt] != NULL)
		{
			g_pTextureRecipe[nCnt]->Release();
			g_pTextureRecipe[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffRecipe != NULL)
	{
		g_pVtxBuffRecipe->Release();
		g_pVtxBuffRecipe = NULL;
	}

}
//==============================================================================================================
// �N���t�g���V�s�̍X�V����
//==============================================================================================================
void UpdateCraftRecipe(void)
{
	// �A�C�e���̎擾
	Item* pItem = GetItem();

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		D3DXVECTOR3 pos = g_aRecipe[nCnt].pos; // �ʒu
		float fWidth = g_aRecipe[nCnt].fWidth; // ����
		float fHeight = g_aRecipe[nCnt].fHeight; // ����

		if ((pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONE || pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_STONE) &&
			(pItem[pPlayer->ItemIdx].nType == ITEMTYPE_BAT || pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_BAT))
		{
			EnableRecipeUI(RECIPETYPE_STONEBAT, true);
		}
		else
		{
			EnableRecipeUI(RECIPETYPE_STONEBAT, false);
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
		
		pVtx += 4;
	}
	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffRecipe->Unlock();

}
//==============================================================================================================
// �N���t�g���V�s�̕`�揈��
//==============================================================================================================
void DrawCraftRecipe(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRecipe, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// �g�p��Ԃ�������
		if (g_aRecipe[nCnt].bUse == true)
		{
			int nType = g_aRecipe[nCnt].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRecipe[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//==============================================================================================================
// �N���t�g���V�s�̐ݒ菈��
//==============================================================================================================
void SetCraftRecipe(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// �g�p��Ԃ�������
		if (g_aRecipe[nCnt].bUse == false)
		{
			g_aRecipe[nCnt].pos = pos;		   // �ʒu
			g_aRecipe[nCnt].nType = nType;     // ���
			g_aRecipe[nCnt].fWidth = fWidth;   // ����
			g_aRecipe[nCnt].fHeight = fHeight; // ����

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffRecipe->Unlock();
}
//==============================================================================================================
// �N���t�g���V�s�̗L����������
//==============================================================================================================
void EnableRecipeUI(int nType, bool bUse)
{
	// �g�p��Ԃ�������
	if (g_aRecipe[nType].bUse == true && bUse == false)
	{
		g_aRecipe[nType].bUse = bUse;
	}
	// �g�p���ĂȂ�������
	else if (g_aRecipe[nType].bUse == false && bUse == true)
	{
		g_aRecipe[nType].bUse = bUse;
	}
}
