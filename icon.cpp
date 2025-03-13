//=================================================================================================================
//
// ����̃A�C�R������ [icon.cpp]
// Author: Asuma Nishio
//
//=================================================================================================================

//******************************************************************************************************************
// �C���N���[�h�t�@�C���錾
//******************************************************************************************************************
#include "item.h"
#include "icon.h"
#include"math.h"
#include "easing.h"
#include "craftui.h"

//******************************************************************************************************************
// �v���g�^�C�v�錾
//******************************************************************************************************************
void SetWeponDurabilityAnim(int nCnt);		// �A�C�e���A�C�R���̓_�ŏ���
void CraftScreenAnim(int nCnt);             // �N���t�g��ʂ̃A�C�R���̐ݒ�
void SetVtxBufferIcon(int nCnt,float fWidth,float fHeight, D3DXVECTOR3 pos); // �A�C�R���̒��_�o�b�t�@�̐ݒ�
void SetUpDownIcon(int nCnt,int *EaseCnt);
void ShowCraftingAnim(int nCnt);
bool CheckHoldItem(int nCnt);
bool CheckStockItem(int nCnt);

//******************************************************************************************************************
// �O���[�o���ϐ��錾
//******************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureIcon[WEPONTYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
ICON g_Icon[WEPONTYPE_MAX]; // �\���̕ϐ�
bool g_bCraftIconAnim = false;
bool g_bSetMixItemAnim = false;

//=================================================================================================================
// �A�C�R���̏���������
//=================================================================================================================
void InitIcon()
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	g_bCraftIconAnim = false;
	g_bSetMixItemAnim = false;

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			ICON_TEXTURE[nCnt],
			&g_pTextureIcon[nCnt]);
	}

	// ���_�o�b�t�@�̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * WEPONTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIcon,
		NULL);

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		g_Icon[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // ���W
		g_Icon[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // �ړ���
		g_Icon[nCnt].fHeight = 0.0f;						  // ����
		g_Icon[nCnt].fWidth = 0.0f;							  // ����
		g_Icon[nCnt].bUse = false;							  // ���g�p����
		g_Icon[nCnt].nType = WEPONTYPE_STONEBAT;					  // ���
		g_Icon[nCnt].nIconType = ICONTYPE_HOLDITEM;			  // ���
		g_Icon[nCnt].EaseCnt = 0;							  // �C�[�W���O
		g_Icon[nCnt].bUp = true;							  // �㏸�ł��邩
		g_Icon[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	  // ����
		g_Icon[nCnt].fRadius = 0.0f;						  // ���a
		g_Icon[nCnt].fDistance = 0.0f;						  // ����

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

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���_����i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
// �A�C�R���̏I������
//=================================================================================================================
void UninitIcon()
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_pTextureIcon[nCnt] != NULL)
		{
			g_pTextureIcon[nCnt]->Release();
			g_pTextureIcon[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffIcon != NULL)
	{
		g_pVtxBuffIcon->Release();
		g_pVtxBuffIcon = NULL;
	}
}
//=================================================================================================================
// �A�C�R���̍X�V����
//=================================================================================================================
void UpdateIcon()
{

	Player* pPlayer = GetPlayer();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)// �S�ẴA�C�R���𐔂�
	{
		if (g_Icon[nCnt].bUse == false)
		{
			continue;
		}

		float fWidth = g_Icon[nCnt].fWidth;
		float fHeight = g_Icon[nCnt].fHeight;

		// �A�C�e���A�C�R���̓_�ŏ���
		SetWeponDurabilityAnim(nCnt);

		if (pPlayer->bCraft == false && CheckHoldItem(nCnt) == true)
		{
			g_Icon[nCnt].fWidth = 60.0f;
			g_Icon[nCnt].fHeight = 60.0f;

			// ���_���W�̍X�V
			SetVtxBufferIcon(nCnt, fWidth, fHeight, D3DXVECTOR3(70.0f, 640.0f, 0.0f));
		}

		if (pPlayer->bCraft == false && CheckStockItem(nCnt) == true)
		{
			g_Icon[nCnt].fWidth = 40.0f;
			g_Icon[nCnt].fHeight = 40.0f;

			// ���_���W�̍X�V
			SetVtxBufferIcon(nCnt, fWidth, fHeight, D3DXVECTOR3(200.0f, 670.0f, 0.0f));
		}

		//if (g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM)
		//{
		//	
		//}
		
		if (g_bCraftIconAnim == false)
		{
			CraftScreenAnim(nCnt);
		}
		else
		{
			ShowCraftingAnim(nCnt);
		}
	}

	// �A�����b�N
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
// �A�C�R���̕`�揈��
//=================================================================================================================
void DrawIcon()
{
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffIcon, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//if (pPlayer->bCraft == true && strcmp(pItem[pPlayer->ItemIdx].Itemtag, "CRAFT") == 0)
	//{
	//	return;
	//}

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// �����Ă���A�C�e���̃A�C�R��
		if (g_Icon[nCnt].bUse == true && pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD && g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM)
		{// �A�C�R�����g�p��� ���� �A�C�e���̏�Ԃ��z�[���h�Ȃ�

			// ���
			int nType = pItem[pPlayer->ItemIdx].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureIcon[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
		// �X�g�b�N���Ă���A�C�e���̃A�C�R��
		if (g_Icon[nCnt].bUse == true && pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && g_Icon[nCnt].nIconType != ICONTYPE_HOLDITEM)
		{// �A�C�R�����g�p��� ���� �A�C�e���̏�Ԃ��z�[���h�Ȃ�

			// ���
			int nType = pItem[pPlayer->StockItemIdx].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureIcon[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
	}
}
//=================================================================================================================
// �A�C�R���̐ݒ菈��
//=================================================================================================================
void SetIcon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType, int IconType) // �ݒ菈��
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (!g_Icon[nCnt].bUse)
		{
			g_Icon[nCnt].pos = pos;		   // ���W
			g_Icon[nCnt].fWidth = fWidth;  // ����
			g_Icon[nCnt].fHeight = fHeight;// ����
			g_Icon[nCnt].nType = nType;	   // ���
			g_Icon[nCnt].nIconType = IconType;	   // ���
			g_Icon[nCnt].bUse = true;	   // �g�p���

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}

		// ���_���W��i�߂�
		pVtx += 4;
	}

	// �A�����b�N
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
// �N���t�g�̃A�j���[�V�����J�n����
//=================================================================================================================
void EnableCraftIconAnim(bool bCraftAnim)
{
	// �L���A�����ɂ���
	g_bCraftIconAnim = bCraftAnim;

	if (bCraftAnim == false)
	{
		return;
	}

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_Icon[nCnt].bUse == false)
		{
			continue;
		}

		// �A�C�e���������Ă��邩����
		if (CheckHoldItem(nCnt) == true)
		{
			// �A�C�e���̈ʒu��ݒ�
			g_Icon[nCnt].pos = D3DXVECTOR3(300.0f,550.0f,0.0f);

			g_Icon[nCnt].fWidth = 120.0f;
			g_Icon[nCnt].fHeight = 120.0f;

			// ���������߂�
			D3DXVECTOR3 DiffPos = g_Icon[nCnt].pos - UIPOTISION;

			// ���������߂�
			g_Icon[nCnt].fDistance = sqrtf((DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y));
		}
		// �A�C�e�����X�g�b�N���Ă��邩����
		if (CheckStockItem(nCnt) == true)
		{
			g_Icon[nCnt].fWidth = 80.0f;
			g_Icon[nCnt].fHeight = 80.0f;

			// �A�C�e���̈ʒu��ݒ�
			g_Icon[nCnt].pos = D3DXVECTOR3(900.0f, 250.0f, 0.0f);

			// ���������߂�
			D3DXVECTOR3 DiffPos = g_Icon[nCnt].pos - UIPOTISION;

			// ���������߂�
			g_Icon[nCnt].fDistance = sqrtf((DiffPos.x * DiffPos.x) + (DiffPos.y * DiffPos.y));
		}
	}
}
//=================================================================================================================
// �A�C�R���̃A�j���[�V������Ԏ擾
//=================================================================================================================
bool GetIconAnim(void)
{
	return g_bCraftIconAnim;
}
//=================================================================================================================
// �N���t�g�A�C�R���̃A�j���[�V�����I���I�t
//=================================================================================================================
void EnableMixIconAnim(bool bEnable)
{
	g_bSetMixItemAnim = bEnable;
}
//=================================================================================================================
// �N���t�g�A�C�R���̃A�j���[�V�������ł��邩
//=================================================================================================================
bool GetIsCraftIconAnim(void)
{
	return g_bSetMixItemAnim;
}
//=================================================================================================================
// �A�C�e���A�C�R���̓_�ŏ���
//=================================================================================================================
void SetWeponDurabilityAnim(int nCnt)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;
	Item* pItem = GetItem();
	Player* pPlayer = GetPlayer();

	// �t���[���p�ϐ�
	static int fFrame;
	static bool bUP;

	// �A�C�e���������Ă��邩����
	const bool isStateHold = pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD;

	// �A�C�R���̎�ނ������Ă���A�C�e��������
	const bool TypeHold = g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM;

	// �_�łł��邩����
	const bool is_frash = (float)pItem[pPlayer->ItemIdx].durability / (float)pItem[pPlayer->ItemIdx].Maxdurability < 0.3f;

	// �_�ł̏����ɓ���邩����
	const bool CanFrashProcess = isStateHold && TypeHold;

	// �������������
	if (CanFrashProcess == false)
	{// �֐��𔲂���
		return;
	}

	// �N���t�g��Ԃ�������
	if (pPlayer->bCraft == true)
	{
		// ���_�o�b�t�@�̃��b�N
		g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f,1.0f,1.0f);

		// �A�����b�N
		g_pVtxBuffIcon->Unlock();

		return;
	}
	// �g�p����Ă邩�A�C�e������
	if (is_frash == true)// �X�ɑϋv�͂������Ă���Ԃ��_��
	{
		if (bUP)
		{
			fFrame++;
		}
		else
		{
			fFrame--;
		}

		// ���_�o�b�t�@�̃��b�N
		g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f - (fFrame / 15.0f), 1.0f - (fFrame / 15.0f), 1.0f);

		// �A�����b�N
		g_pVtxBuffIcon->Unlock();

		if (fFrame >= 10)
		{
			bUP = false;
		}
		else if (fFrame <= 0)
		{
			bUP = true;
		}
	}
	else // �����łȂ�������F��߂�
	{
		// ���_�o�b�t�@�̃��b�N
		g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * nCnt;

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �A�����b�N
		g_pVtxBuffIcon->Unlock();

		fFrame = 0;
		bUP = true;
	}
}
//=================================================================================================================
// �N���t�g��ʂ̃A�C�R���̐ݒ�
//=================================================================================================================
void CraftScreenAnim(int nCnt)
{
	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	float fWidth = g_Icon[nCnt].fWidth;
	float fHeight = g_Icon[nCnt].fHeight;

	// �N���t�g��ʂ��J���Ă�Ƃ�
	if (CheckHoldItem(nCnt) == true &&pPlayer->bCraft == true)
	{
		SetUpDownIcon(nCnt, &g_Icon[nCnt].EaseCnt);

		// ���_���W�̍X�V
		SetVtxBufferIcon(nCnt, fWidth * 2.0f, fHeight * 2.0f, D3DXVECTOR3(400.0f, g_Icon[nCnt].pos.y, 0.0f));
	}
	// �N���t�g��ʂ���Ă���Ƃ�
	else if(CheckHoldItem(nCnt) == true && pPlayer->bCraft == false)
	{
		// ���_���W�̍X�V
		SetVtxBufferIcon(nCnt, fWidth, fHeight, D3DXVECTOR3(70.0f, 640.0f, 0.0f));
	}

	// �N���t�g��ʂ��J���Ă�Ƃ�
	if (CheckStockItem(nCnt) == true && pPlayer->bCraft == true)
	{
		SetUpDownIcon(nCnt,&g_Icon[nCnt].EaseCnt);

		// ���_���W�̍X�V
		SetVtxBufferIcon(nCnt, fWidth * 3.0f, fHeight * 3.0f, D3DXVECTOR3(840.0f, g_Icon[nCnt].pos.y, 0.0f));
	}
	// �N���t�g��ʂ���Ă���Ƃ�
	else if(CheckStockItem(nCnt) && pPlayer->bCraft == false)
	{
		// ���_���W�̍X�V
		SetVtxBufferIcon(nCnt, fWidth, fHeight, D3DXVECTOR3(200.0f, 670.0f, 0.0f));
	}
}
//=================================================================================================================
// �A�C�R���̒��_�o�b�t�@�̐ݒ�
//=================================================================================================================
void SetVtxBufferIcon(int nCnt, float fWidth, float fHeight,D3DXVECTOR3 pos)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffIcon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	// �A�����b�N
	g_pVtxBuffIcon->Unlock();
}
//=================================================================================================================
//=================================================================================================================
void SetUpDownIcon(int nCnt, int* EaseCnt)
{
	Player* pPlayer = GetPlayer();

	const bool bSetPos = g_Icon[nCnt].pos.y >= 390.0f && g_Icon[nCnt].pos.y <= 510.0f;

	if (bSetPos == false)
	{
		g_Icon[nCnt].pos.y = 450.0f;
	}
	if (g_Icon[nCnt].pos.y <= 410.0f)
	{
		(*EaseCnt) = 0;
		g_Icon[nCnt].bUp = false;
	}
	else if (g_Icon[nCnt].pos.y >= 490.0f)
	{
		(*EaseCnt) = 0;
		g_Icon[nCnt].bUp = true;
	}
	if (g_Icon[nCnt].bUp == true)
	{
		(*EaseCnt)++;
		float time = SetEase((*EaseCnt), 360);

		g_Icon[nCnt].pos.y += SetSmoothAprroach(400.0f, g_Icon[nCnt].pos.y, EaseOutSine(time));
	}
	else
	{
		(*EaseCnt)++;
		float time = SetEase((*EaseCnt), 360);

		g_Icon[nCnt].pos.y += SetSmoothAprroach(500.0f, g_Icon[nCnt].pos.y, EaseOutSine(time));
	}
}
//=================================================================================================================
//=================================================================================================================
void ShowCraftingAnim(int nCnt)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();
	Item* pItem = GetItem();

	float fWidth = g_Icon[nCnt].fWidth;
	float fHeight = g_Icon[nCnt].fHeight;

	if (g_Icon[nCnt].rot.z < -D3DX_PI)
	{
		g_Icon[nCnt].rot.z += D3DX_PI * 2.0f;
	}
	else if (g_Icon[nCnt].rot.z > D3DX_PI)
	{
		g_Icon[nCnt].rot.z += -D3DX_PI * 2.0f;
	}


	// �N���t�g��ʂ��J���Ă�Ƃ�
	if (CheckHoldItem(nCnt) == true && pPlayer->bCraft == true)
	{
		g_Icon[nCnt].rot.z += 0.05f;
		g_Icon[nCnt].fWidth += SetSmoothAprroach(0.0f, g_Icon[nCnt].fWidth, 0.02f);
		g_Icon[nCnt].fHeight += SetSmoothAprroach(0.0f, g_Icon[nCnt].fHeight, 0.02f);

		g_Icon[nCnt].fDistance += SetSmoothAprroach(0.0f, g_Icon[nCnt].fDistance, 0.02f);
		g_Icon[nCnt].pos.x = UIPOTISION.x + sinf(g_Icon[nCnt].rot.z) * g_Icon[nCnt].fDistance;
		g_Icon[nCnt].pos.y = UIPOTISION.y + cosf(g_Icon[nCnt].rot.z) * g_Icon[nCnt].fDistance;

		// ���_���W�̍X�V
		SetVtxBufferIcon(nCnt, fWidth * 2.0f, fHeight * 2.0f, g_Icon[nCnt].pos);
	}
	// �N���t�g��ʂ��J���Ă�Ƃ�
	if (CheckStockItem(nCnt) == true && pPlayer->bCraft == true)
	{
		g_Icon[nCnt].rot.z += 0.05f;

		g_Icon[nCnt].fWidth += SetSmoothAprroach(0.0f, g_Icon[nCnt].fWidth, 0.02f);
		g_Icon[nCnt].fHeight += SetSmoothAprroach(0.0f, g_Icon[nCnt].fHeight, 0.02f);

		g_Icon[nCnt].fDistance += SetSmoothAprroach(0.0f, g_Icon[nCnt].fDistance, 0.02f);
		g_Icon[nCnt].pos.x = UIPOTISION.x - sinf(g_Icon[nCnt].rot.z) * g_Icon[nCnt].fDistance;
		g_Icon[nCnt].pos.y = UIPOTISION.y - cosf(g_Icon[nCnt].rot.z) * g_Icon[nCnt].fDistance;

		// ������0�ɋ߂��Ȃ�����
		if (g_Icon[nCnt].fWidth <= 3.0f)
		{// �N���t�g�A�C�R���̃A�j���[�V����OK
			g_bSetMixItemAnim = true;
		}

		// ���_���W�̍X�V
		SetVtxBufferIcon(nCnt, fWidth * 3.0f, fHeight * 3.0f, g_Icon[nCnt].pos);
	}
}
//=================================================================================================================
// �z�[���h��Ԃ̃A�C�e���ƈ�v���Ă��邩����
//=================================================================================================================
bool CheckHoldItem(int nCnt)
{
	Player* pPlayer = GetPlayer();	// �v���C���[�̎擾
	Item* pItem = GetItem();		// �A�C�e���̎擾

	// �����Ă���A�C�e���̏�Ԃ��z�[���h������
	const bool checkhold = pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD;

	// �A�C�R���̎�ނ��z�[���h�A�C�e���̃A�C�R������
	const bool checkholditemicon = g_Icon[nCnt].nIconType == ICONTYPE_HOLDITEM;

	// �����Ԃ�
	return checkhold && checkholditemicon;
}
//=================================================================================================================
// �X�g�b�N��Ԃ̃A�C�e���ƈ�v���Ă��邩����
//=================================================================================================================
bool CheckStockItem(int nCnt)
{
	Player* pPlayer = GetPlayer();	// �v���C���[�̎擾
	Item* pItem = GetItem();		// �A�C�e���̎擾

	// �����Ă���A�C�e���̏�Ԃ��z�[���h������
	const bool checkStock = pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK;

	// �A�C�R���̎�ނ��X�g�b�N�A�C�e���̃A�C�R��������
	const bool checkStockitemicon = g_Icon[nCnt].nIconType == ICONTYPE_STOCKITEM;

	// �����Ԃ�
	return checkStock && checkStockitemicon;
}
