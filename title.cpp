//============================
//
// �^�C�g�� [title.cpp]
// Author: TEAM_C
//
//============================

//****************************
//�C���N���[�h�t�@�C��
//****************************
#include "title.h"
#include "input.h"

//****************************
//�}�N����`
//****************************
#define NUM_TITLE (2) // �^�C�g���̃e�N�X�`���̐�

//****************************
//�O���[�o���錾
//****************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLETYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL; // ���_�o�b�t�@�ւ̃|�C���^
TITLE g_Title[NUM_TITLE];

//============================
//�^�C�g���̏���������
//============================
void InitTitle(void)
{
	VERTEX_2D* pVtx;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TITLE_TYPE[nCnt],
			&g_pTextureTitle[nCnt]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		g_Title[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Title[nCnt].bUse = false;
		g_Title[nCnt].fHeight = 0.0f;
		g_Title[nCnt].fWidth = 0.0f;
		g_Title[nCnt].nType = TITLETYPE_TITLE;
		g_Title[nCnt].TitleMenu = TITLESELECT_TITLE;

		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;
	}

	SetTitle(D3DXVECTOR3(640.0f, 200.0f, 0.0f), TITLETYPE_TITLE, 200.0f, 50.0f);
	SetTitle(D3DXVECTOR3(640.0f, 500.0f, 0.0f), TITLETYPE_TUTO, 200.0f, 50.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}
//============================
//�^�C�g���̏I������
//============================
void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}
}
//============================
//�^�C�g���̍X�V����
//============================
void UpdateTitle(void)
{
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		switch (g_Title[nCnt].TitleMenu)
		{
		case TITLESELECT_TITLE:
			if (KeyboardTrigger(DIK_DOWN))
			{
				g_Title[nCnt].TitleMenu = TITLESELECT_TUTO;
			}
			SelectTitle(0);

			break;
		case TITLESELECT_TUTO:
			if (KeyboardTrigger(DIK_UP))
			{
				g_Title[nCnt].TitleMenu = TITLESELECT_TITLE;
			}
			SelectTitle(1);
			break;
		default:
			break;
		}
	}
}
//============================
//�^�C�g���̕`�揈��
//============================
void DrawTitle(void)
{
	LPDIRECT3DDEVICE9 	pDevice = GetDevice();//�f�o�C�X�ւ̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[g_Title[nCnt].nType]);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);//�v���~�e�B�u�̎��
	}
}
//============================
//�^�C�g���̕`�揈��
//============================
void SetTitle(D3DXVECTOR3 pos, int nType,float fWidth, float fHeight)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		//���g�p��������
		if (!g_Title[nCnt].bUse)
		{
			g_Title[nCnt].pos = pos;
			g_Title[nCnt].nType = nType;
			g_Title[nCnt].bUse = true;

			switch (nType)
			{
			case TITLETYPE_TITLE:
				//���_���W�̍X�V
				pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
				break;
			case TITLETYPE_TUTO:
				//���_���W�̍X�V
				pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);
				break;
			default:
				break;
			}
			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}

//============================
//�^�C�g���̑I������
//============================
void SelectTitle(int select)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		if (nCnt == select)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}
