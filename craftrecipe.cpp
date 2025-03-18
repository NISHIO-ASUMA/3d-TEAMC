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
bool IsStoneBat(Item* pItem, Player* pPlayer); // �X�g�[���o�b�g�̑f�ނ������Ă��邩����
bool IsSurfboardFish(Item* pItem, Player* pPlayer);     // ���̑f�ނ������Ă��邩����
bool IsHexMandorin(Item* pItem, Player* pPlayer);      // �����̑f�ނ������Ă��邩����
bool IsGolfHunmer(Item* pItem, Player* pPlayer);   // �n���}�[�̑f�ނ������Ă��邩����
bool IsMixKatana(Item* pItem, Player* pPlayer);   // ���n���̑f�ނ������Ă��邩����
bool IsHexSpear(Item* pItem, Player* pPlayer);    // ���n���̑f�ނ������Ă��邩����
bool IsHeadTorso(Item* pItem, Player* pPlayer);    // ��t���}�l�L���̑f�ނ������Ă��邩����

void UpdateRecipeUIParam(void);				   // ���V�sUI�̃p�����[�^�ݒ�

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
		g_aRecipe[nCnt].pos = D3DXVECTOR3(600.0f,300.0,0.0f); // �ʒu
		g_aRecipe[nCnt].nType = nCnt;						  // ���
		g_aRecipe[nCnt].fWidth = 200.0f;					  // ����
		g_aRecipe[nCnt].fHeight = 150.0f;					  // ����
		g_aRecipe[nCnt].bUse = false;						  // �g�p���
		g_aRecipe[nCnt].state = RECIPEUISTATE_NONE;           // UI�̏��

		D3DXVECTOR3 pos = g_aRecipe[nCnt].pos;
		float fWidth = g_aRecipe[nCnt].fWidth;
		float fHeight = g_aRecipe[nCnt].fHeight;

			//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

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
	// ���V�s�̃p�����[�^�ݒ�
	UpdateRecipeUIParam();
	
	// �\������Ă���UI�̎��0
	static int SetUIType1 = 0;

	// �\������Ă���UI�̎��1
	static int SetUIType2 = 0;

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// UI���Z�b�g����Ă�����
		if (g_aRecipe[nCnt].state == RECIPEUISTATE_SET)
		{
			// �C���f�b�N�X���擾
			SetUIType1 = nCnt;

			// ������
			break;
		}
	}

	// ���V�s�̎�ޕ�
	for (int nCnt = 0; nCnt < RECIPETYPE_MAX; nCnt++)
	{
		// UI���Z�b�g����Ă��邩��1�߂ƕʂ̎�ނ�������
		if (g_aRecipe[nCnt].state == RECIPEUISTATE_SET && nCnt != SetUIType1)
		{
			// 2�߂�UI�̃C���f�b�N�X���擾
			SetUIType2 = nCnt;
			break;
		}
	}

	// 1�ڂ�2�ڂ��Z�b�g����Ă��邩�A�ʁX�̎��
	if (g_aRecipe[SetUIType1].state == RECIPEUISTATE_SET && g_aRecipe[SetUIType2].state == RECIPEUISTATE_SET && SetUIType1 != SetUIType2)
	{
		// ���_���W���X�V
		SetCraftRecipe(g_aRecipe[SetUIType1].pos, SetUIType1, SetUIType2, g_aRecipe[SetUIType1].fWidth, g_aRecipe[SetUIType1].fHeight);
	}
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
			pDevice->SetTexture(0, g_pTextureRecipe[nCnt]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}
//==============================================================================================================
// �N���t�g���V�s�̐ݒ菈��
//==============================================================================================================
void SetCraftRecipe(D3DXVECTOR3 pos, int nType0,int nType1, float fWidth, float fHeight)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�̃|�C���^��i�߂�
	pVtx += 4 * nType0;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(-(fWidth * 1.5f) + pos.x + fWidth, pos.y + fHeight, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffRecipe->Unlock();

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�̃|�C���^��i�߂�
	pVtx += 4 * nType1;

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3((fWidth * 1.5f) + pos.x + fWidth, pos.y + fHeight, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffRecipe->Unlock();
}
//==============================================================================================================
// �N���t�g���V�s�̗L����������
//==============================================================================================================
void EnableRecipeUI(int nType, bool bUse)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �g�p��Ԃ�������
	if (g_aRecipe[nType].bUse == true && bUse == false)
	{
		g_aRecipe[nType].state = RECIPEUISTATE_NONE;
		g_aRecipe[nType].bUse = bUse;
	}
	// �g�p���ĂȂ�������
	else if (g_aRecipe[nType].bUse == false && bUse == true)
	{
		g_aRecipe[nType].state = RECIPEUISTATE_SET;
		g_aRecipe[nType].bUse = bUse;
	}


	if (g_aRecipe[nType].bUse == true)
	{
		// ���_�o�b�t�@�̃��b�N
		g_pVtxBuffRecipe->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�o�b�t�@�̃|�C���^��i�߂�
		pVtx += 4 * nType;

		// �ʒu
		D3DXVECTOR3 pos = g_aRecipe[nType].pos;

		// ����
		float fWidth = g_aRecipe[nType].fWidth;

		// ����
		float fHeight = g_aRecipe[nType].fHeight;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

		// ���_�o�b�t�@�̃A�����b�N
		g_pVtxBuffRecipe->Unlock();
	}
}
//==============================================================================================================
// �X�g�[���o�b�g�̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsStoneBat(Item* pItem, Player* pPlayer)
{
	// �΂������Ă��邩����
	const bool HaveStone = pPlayer->HoldItemType == ITEMTYPE_STONE;

	// �΂��X�g�b�N���Ă��邩����
	const bool StockStone = pPlayer->StockItemType == ITEMTYPE_STONE;

	// �o�b�g�������Ă��邩����
	const bool HaveBat = pPlayer->HoldItemType == ITEMTYPE_BAT;

	// �o�b�g���X�g�b�N���Ă��邩����
	const bool StockBat = pPlayer->StockItemType == ITEMTYPE_BAT;

	// �΃o�b�g�̃��V�s��\���ł��邩����
	const bool CanSetUI = HaveStone || StockStone || HaveBat || StockBat;

	// UI��\���ł���
	if (CanSetUI == true)
	{
		// �\���ł���
		return true;
	}

	// �\���ł��Ȃ�
	return false;
}
//==============================================================================================================
// ���̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsSurfboardFish(Item* pItem, Player* pPlayer)
{
	// �΂������Ă��邩����
	const bool HaveFish = pPlayer->HoldItemType == ITEMTYPE_FISH;

	// �΂��X�g�b�N���Ă��邩����
	const bool StockFish = pPlayer->StockItemType == ITEMTYPE_FISH;

	// �o�b�g�������Ă��邩����
	const bool HaveSurfBoard = pPlayer->HoldItemType == ITEMTYPE_SURFBOARD;

	// �o�b�g���X�g�b�N���Ă��邩����
	const bool StockSurfBoard = pPlayer->StockItemType == ITEMTYPE_SURFBOARD;

	// �T�[�t�{�[�h���̃��V�s��\���ł��邩����
	const bool CanSetUI = HaveFish || StockFish || HaveSurfBoard || StockSurfBoard;

	// UI��\���ł���
	if (CanSetUI == true)
	{
		// �\���ł���
		return true;
	}

	// �\���ł��Ȃ�
	return false;

}
//==============================================================================================================
// �����̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsHexMandorin(Item* pItem, Player* pPlayer)
{
	// �����������Ă��邩����
	const bool HaveHex = pPlayer->HoldItemType == ITEMTYPE_HEX;

	// �������X�g�b�N���Ă��邩����
	const bool StockHex = pPlayer->StockItemType == ITEMTYPE_HEX;

	// �y��������Ă��邩����
	const bool HaveMandorin = pPlayer->HoldItemType == ITEMTYPE_MANDORIN;

	// �y����X�g�b�N���Ă��邩����
	const bool StockMandorin = pPlayer->StockItemType == ITEMTYPE_MANDORIN;

	// �􂢂̊y��̃��V�s��\���ł��邩����
	const bool CanSetUI = HaveHex || StockHex || HaveMandorin || StockMandorin;

	// UI��\���ł���
	if (CanSetUI == true)
	{
		// �\���ł���
		return true;
	}

	// �\���ł��Ȃ�
	return false;
}
//==============================================================================================================
// �n���}�[�̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsGolfHunmer(Item* pItem, Player* pPlayer)
{
	// �S���t�������Ă��邩����
	const bool HaveGolf = pPlayer->HoldItemType == ITEMTYPE_GOLF;

	// �S���t���X�g�b�N���Ă��邩����
	const bool StockGolf = pPlayer->StockItemType == ITEMTYPE_GOLF;

	// �n���}�[�������Ă��邩����
	const bool HaveHunmer = pPlayer->HoldItemType == ITEMTYPE_HUNMER;

	// �n���}�[���X�g�b�N���Ă��邩����
	const bool StockHunmer = pPlayer->StockItemType == ITEMTYPE_HUNMER;

	// �S���t�n���}�[�̃��V�s��\���ł��邩����
	const bool CanSetUI = HaveGolf || StockGolf || HaveHunmer || StockHunmer;

	// UI��\���ł���
	if (CanSetUI == true)
	{
		// �\���ł���
		return true;
	}

	// �\���ł��Ȃ�
	return false;
}
//==============================================================================================================
// ���n���̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsMixKatana(Item* pItem, Player* pPlayer)
{
	bool bFlag = false;
	
	// ���������Ă��邩����
	const bool HaveKatana = pPlayer->HoldItemType == ITEMTYPE_KATANA;

	// �����X�g�b�N���Ă��邩����
	const bool StockKatana = pPlayer->StockItemType == ITEMTYPE_KATANA;

	// �g�[�`�������Ă��邩����
	const bool HaveTorch = pPlayer->HoldItemType == ITEMTYPE_TORCH;

	// �g�[�`���X�g�b�N���Ă��邩����
	const bool StockTorch = pPlayer->StockItemType == ITEMTYPE_TORCH;

	// ���̌��̃��V�s��\���ł��邩����
	const bool FireSwordSetUI = HaveKatana || StockKatana || HaveTorch || StockTorch;

	// UI��\���ł���
	if (FireSwordSetUI == true)
	{
		// �\���ł���
		bFlag = true;
	}

	// �X�������Ă��邩����
	const bool HaveIceBlock = pPlayer->HoldItemType == ITEMTYPE_ICEBLOCK;

	// �X���X�g�b�N���Ă��邩����
	const bool StockIceBlock = pPlayer->StockItemType == ITEMTYPE_ICEBLOCK;

	// �X�̌��̃��V�s��\���ł��邩����
	const bool IceSwordSetUI = HaveKatana || StockKatana || HaveIceBlock || StockIceBlock;

	// UI��\���ł���
	if (IceSwordSetUI == true)
	{
		bFlag = true;
	}

	// �u�����������Ă��邩����
	const bool HaveLight = pPlayer->HoldItemType == ITEMTYPE_LIGHT;

	// �u�������X�g�b�N���Ă��邩����
	const bool StockLight = pPlayer->StockItemType == ITEMTYPE_LIGHT;

	// ���̌��̃��V�s��\���ł��邩����
	const bool LightSwordSetUI = HaveKatana || StockKatana || HaveLight || StockLight;

	if (LightSwordSetUI == true)
	{
		bFlag = true;
	}

	// �\���ł��Ȃ�
	return bFlag;
}
//==============================================================================================================
// ���n���̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsHexSpear(Item* pItem, Player* pPlayer)
{
	// ���������Ă��邩����
	const bool HaveSpear = pPlayer->HoldItemType == ITEMTYPE_SPEAR;

	// �����X�g�b�N���Ă��邩����
	const bool StockSpear = pPlayer->StockItemType == ITEMTYPE_SPEAR;

	// ���������Ă��邩����
	const bool HaveBone = pPlayer->HoldItemType == ITEMTYPE_BONE;

	// �����X�g�b�N���Ă��邩����
	const bool StockBone = pPlayer->StockItemType == ITEMTYPE_BONE;

	// ���̑��̃��V�s��\���ł��邩����
	const bool CanSetUI = HaveSpear || StockSpear || HaveBone || StockBone;

	// UI��\���ł���
	if (CanSetUI == true)
	{
		// �\���ł���
		return true;
	}

	// �\���ł��Ȃ�
	return false;
}
//==============================================================================================================
// ��t���}�l�L���̑f�ނ������Ă��邩����
//==============================================================================================================
bool IsHeadTorso(Item* pItem, Player* pPlayer)
{
	// ���W���������Ă��邩����
	const bool HaveTorso = pPlayer->HoldItemType == ITEMTYPE_TORSO;

	// ���W�����X�g�b�N���Ă��邩����
	const bool StockTorso = pPlayer->StockItemType == ITEMTYPE_TORSO;

	// �񖳂��}�l�L���������Ă��邩����
	const bool HaveHead = pPlayer->HoldItemType == ITEMTYPE_HEADSTATUE;

	// �񖳂��}�l�L�����X�g�b�N���Ă��邩����
	const bool StockHead = pPlayer->StockItemType == ITEMTYPE_HEADSTATUE;

	// ���̑��̃��V�s��\���ł��邩����
	const bool CanSetUI = HaveTorso || StockTorso || HaveHead || StockHead;

	// UI��\���ł���
	if (CanSetUI == true)
	{
		// �\���ł���
		return true;
	}

	// �\���ł��Ȃ�
	return false;

}
//==============================================================================================================
// ���V�sUI�̃p�����[�^�ݒ�
//==============================================================================================================
void UpdateRecipeUIParam(void)
{
	// �A�C�e���̎擾
	Item* pItem = GetItem();

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �f�ނ������Ă�����
	if (IsStoneBat(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_STONEBAT, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_STONEBAT, false);
	}

	// ���̑f�ނ������Ă�����
	if (IsSurfboardFish(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_FISH, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_FISH, false);
	}

	// �􂢂̊y��̑f�ނ������Ă�����
	if (IsHexMandorin(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_HEX, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_HEX, false);
	}

	// �S���t�n���}�[�̑f�ނ������Ă�����
	if (IsGolfHunmer(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_HUMMER, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_HUMMER, false);
	}

	// ���n�̑f�ނ������Ă�����
	if (IsMixKatana(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_KATANA, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_KATANA, false);
	}

	// ���̑f�ނ������Ă�����
	if (IsHexSpear(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_SPEAR, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_SPEAR, false);
	}

	// ��t���}�l�L���̑f�ނ������Ă�����
	if (IsHeadTorso(pItem, pPlayer) == true)
	{
		// �L���ɂ���
		EnableRecipeUI(RECIPETYPE_TORSO, true);
	}
	else
	{
		// �����ɂ���
		EnableRecipeUI(RECIPETYPE_TORSO, false);
	}

}
