//==============================================================================================================
//
// �N���t�gUI [craftui.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "HPGauge.h"
#include "player.h"
#include "craftui.h"
#include "icon.h"
#include "craftrecipe.h"
#include "math.h"
#include "game.h"
#include "Score.h"

//**************************************************************************************************************
// �}�N����`
//**************************************************************************************************************
#define MAX_FALLPOS (720.0f) // �����ł���ő�̏ꏊ

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void UpdateCraftIconAnim(int nCnt); // �N���t�g�A�C�R���̃A�j���[�V�����̍X�V
void ResetCraftAnim(int nCnt);      // �N���t�g�A�C�R���̃A�j���[�V�����̏�����

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureCraftUI[CRAFTUITYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureItemIcon[WEPONTYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCraftUI = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItemIcon = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
Craftui g_CraftUI[CRAFTUITYPE_MAX];
Craftui g_MixUI[WEPONTYPE_MAX];

//==============================================================================================================
// UI�̏���������
//==============================================================================================================
void InitCraftUI(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//InitIcon();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			CRAFTUITYPE_INFO[nCnt],
			&g_pTextureCraftUI[nCnt]);
	}

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * CRAFTUITYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCraftUI,
		NULL);

	// ���_���b�N
	g_pVtxBuffCraftUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		g_CraftUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_CraftUI[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_CraftUI[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_CraftUI[nCnt].fHeight = 0.0f;
		g_CraftUI[nCnt].fWidth = 0.0f;
		g_CraftUI[nCnt].nUseTime = 0;
		g_CraftUI[nCnt].bUse = false;
		g_CraftUI[nCnt].nType = CRAFTUITYPE_MAX;

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

		pVtx += 4;
	}
	//���_���b�N����
	g_pVtxBuffCraftUI->Unlock();


	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			ICON_TEXTURE[nCnt],
			&g_pTextureItemIcon[nCnt]);
	}

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * WEPONTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItemIcon,
		NULL);

	// ���_���b�N
	g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		g_MixUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MixUI[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MixUI[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MixUI[nCnt].fHeight = 0.0f;
		g_MixUI[nCnt].fWidth = 0.0f;
		g_MixUI[nCnt].nUseTime = 0;
		g_MixUI[nCnt].bUse = false;
		g_MixUI[nCnt].nIconType = WEPONTYPE_BAT;

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

		pVtx += 4;
	}

	//���_���b�N����
	g_pVtxBuffItemIcon->Unlock();

	// �N���t�g��ʂ̔w�i
	SetCraftUI(D3DXVECTOR3(640.0f, 360.0f, 0.0f), CRAFTUITYPE_BACKGROUND, 640.0f, 360.0f, 0);

	// �N���t�g��ʂ̃N���t�g�{�^��
	SetCraftUI(D3DXVECTOR3(225.0f, 625.0f, 0.0f), CRAFTUITYPE_CRAFT, 100.0f, 50.0f, -1);

	// �N���t�g��ʂ̕���{�^��
	SetCraftUI(D3DXVECTOR3(1025.0f, 625.0f, 0.0f), CRAFTUITYPE_CLOSE, 100.0f, 50.0f, -1);
}
//==============================================================================================================
// UI�̏I������
//==============================================================================================================
void UninitCraftUI(void)
{
	//UninitIcon();

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_pTextureCraftUI[nCnt] != NULL)
		{
			g_pTextureCraftUI[nCnt]->Release();
			g_pTextureCraftUI[nCnt] = NULL;
		}
	}

	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_pTextureItemIcon[nCnt] != NULL)
		{
			g_pTextureItemIcon[nCnt]->Release();
			g_pTextureItemIcon[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCraftUI != NULL)
	{
		g_pVtxBuffCraftUI->Release();
		g_pVtxBuffCraftUI = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffItemIcon != NULL)
	{
		g_pVtxBuffItemIcon->Release();
		g_pVtxBuffItemIcon = NULL;
	}

}
//==============================================================================================================
// UI�̍X�V����
//==============================================================================================================
void UpdateCraftUI(void)
{
	Player* pPlayer = GetPlayer();

	//for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	//{
	//	if (g_CraftUI[nCnt].bUse == false)
	//	{
	//		continue;
	//	}
	//}

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_MixUI[nCnt].bUse == false)
		{
			continue;
		}

		// �N���t�g����߂���
		if (pPlayer->bCraft == false)
		{
			// ���Z�b�g
			g_MixUI[nCnt].bUse = false;
		}

		if (GetIsCraftIconAnim() == true)
		{
			UpdateCraftIconAnim(nCnt);
		}
		else
		{
			ResetCraftAnim(nCnt);
		}
	}

	UpdateCraftRecipe();

}
//==============================================================================================================
// UI�̕`�揈��
//==============================================================================================================
void DrawCraftUI(void)
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̏K��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCraftUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_CraftUI[nCnt].bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCraftUI[g_CraftUI[nCnt].nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
	}

	// �N���t�g�̃��V�s�̕`�揈��
	DrawCraftRecipe();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItemIcon, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_MixUI[nCnt].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureItemIcon[g_MixUI[nCnt].nIconType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
	}
}
//==============================================================================================================
// UI�̐ݒ菈��
//==============================================================================================================
void SetCraftUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffCraftUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < CRAFTUITYPE_MAX; nCnt++)
	{
		if (g_CraftUI[nCnt].bUse == false)
		{
			g_CraftUI[nCnt].pos = pos;
			g_CraftUI[nCnt].nType = nType;
			g_CraftUI[nCnt].fWidth = fWidth;
			g_CraftUI[nCnt].fHeight = fHeight;
			g_CraftUI[nCnt].nUseTime = nUseTime;
			g_CraftUI[nCnt].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// ���_���b�N����
	g_pVtxBuffCraftUI->Unlock();
}
//==============================================================================================================
// ����UI�̐ݒ菈��
//==============================================================================================================
void SetMixUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_MixUI[nCnt].bUse == false)
		{
			g_MixUI[nCnt].pos = pos;
			g_MixUI[nCnt].nIconType = nType;
			g_MixUI[nCnt].fWidth = fWidth;
			g_MixUI[nCnt].fHeight = fHeight;
			g_MixUI[nCnt].nUseTime = nUseTime;
			g_MixUI[nCnt].bUse = true;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		pVtx += 4;
	}

	// ���_���b�N����
	g_pVtxBuffItemIcon->Unlock();

}
//==============================================================================================================
// ���������A�C�e����UI�̐ݒ菈��
//==============================================================================================================
void SetMixItemUI(int nCnt)
{
	Item* pItem = GetItem();
}
//==============================================================================================================
// �擾����
//==============================================================================================================
Craftui* GetMixUI(void)
{
	return &g_MixUI[0];
}
//==============================================================================================================
// �N���t�g�A�C�R���̃A�j���[�V�����̍X�V
//==============================================================================================================
void UpdateCraftIconAnim(int nCnt)
{
	// ���[�h���擾
	MODE mode = GetMode();

	// ���_���W�̃|�C���^
	VERTEX_2D* pVtx;

	// �v���C���\���擾
	Player* pPlayer = GetPlayer();

	// �W�����v�̃J�E���^�[
	static int JumpCounter = 0;

	// �ʒu���X�V
	g_MixUI[nCnt].pos.y += g_MixUI[nCnt].move.y;

	// �d�͂����Z
	g_MixUI[nCnt].move.y += MAX_GLABITY;

	// �W�����v�̃J�E���^�[��0�̎��̂ݒʂ�
	if (JumpCounter == 0)
	{
		// �A�C�e���̃A�C�R�����W�����v������
		g_MixUI[nCnt].move.y = -15.0f;

		// 2��ڒʂ������Ȃ��̂�1�ɂ���
		JumpCounter = 1;
	}

	// ���_���b�N
	g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̃|�C���^��i�߂�
	pVtx += 4 * nCnt;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x - g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y - g_MixUI[nCnt].fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x + g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y - g_MixUI[nCnt].fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x - g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y + g_MixUI[nCnt].fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_MixUI[nCnt].pos.x + g_MixUI[nCnt].fWidth, g_MixUI[nCnt].pos.y + g_MixUI[nCnt].fHeight, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_���b�N����
	g_pVtxBuffItemIcon->Unlock();

	if (g_MixUI[nCnt].pos.y >= MAX_FALLPOS)
	{
		// �v���C���[�̃A�C�e���̕ύX
		CraftMixItem(pPlayer->ItemIdx, pPlayer->StockItemIdx);

		// �A�C�R���̃A�j���[�V�������~�߂�
		EnableCraftIconAnim(false);

		// �N���t�g�A�C�R���̃A�j���[�V�������~�߂�
		EnableMixIconAnim(false);

		// �W�����v������������
		JumpCounter = 0;

		// �v���C���[�̃N���t�g��Ԃ�����
		pPlayer->bCraft = false;

		// �N���t�g��Ԃ�����
		EnableCraft(false);

		// ���̃��[�h���Q�[����������
		if(mode == MODE_GAME) AddScore(12345);
	}
}
//==============================================================================================================
// �N���t�g�A�C�R���̃A�j���[�V�����̏�����
//==============================================================================================================
void ResetCraftAnim(int nCnt)
{
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffItemIcon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//���_���b�N����
	g_pVtxBuffItemIcon->Unlock();
}
