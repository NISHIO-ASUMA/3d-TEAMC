//===============================
//
// ����̃A�C�R������ [icon.cpp]
// Author: Asuma Nishio
//
//===============================

//********************************
// �C���N���[�h�t�@�C���錾
//********************************
#include "item.h"
#include "icon.h"

//********************************
// �O���[�o���ϐ��錾
//********************************
LPDIRECT3DTEXTURE9 g_pTextureIcon[WEPONTYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIcon = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
ICON g_Icon[WEPONTYPE_MAX]; // �\���̕ϐ�

//===============================
// �A�C�R���̏���������
//===============================
void InitIcon()
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

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
		g_Icon[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W
		g_Icon[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړ���
		g_Icon[nCnt].fHeight = 0.0f;					  // ����
		g_Icon[nCnt].fWidth = 0.0f;						  // ����
		g_Icon[nCnt].bUse = false;						  // ���g�p����
		g_Icon[nCnt].nType = WEPONTYPE_BAT;				  // ���

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
//===============================
// �A�C�R���̏I������
//===============================
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
//===============================
// �A�C�R���̍X�V����
//===============================
void UpdateIcon()
{
	// ����
}
//===============================
// �A�C�R���̕`�揈��
//===============================
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

	for (int nCnt = 0; nCnt < WEPONTYPE_MAX; nCnt++)
	{
		if (g_Icon[nCnt].bUse && pItem[pPlayer->ItemIdx].state == ITEMSTATE_HOLD)
		{// �A�C�R�����g�p��� ���� �A�C�e���̏�Ԃ��z�[���h�Ȃ�

			// ���
			int nType = pItem[pPlayer->ItemIdx].nType;

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureIcon[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
			
		}
	}
}
//===============================
// �A�C�R���̐ݒ菈��
//===============================
void SetIcon(D3DXVECTOR3 pos, float fWidth, float fHeight, int nType)
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
