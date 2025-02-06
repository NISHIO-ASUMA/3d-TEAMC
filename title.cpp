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
#include "fade.h"
#include "gameui.h"
#include "sound.h"
#include "mouse.h"

//****************************
//�}�N����`
//****************************
#define NUM_TITLE (2) // �^�C�g���̃e�N�X�`���̐�

//****************************
//�v���g�^�C�v�錾
//****************************
void SelectTitle(int select);					   // �^�C�g����ʂ̑I��
void TitleFlash(int state, int nSelect, int nIdx); // �^�C�g���̓_��
void TitleMenuFlash(int nSelect);				   // �^�C�g�����j���[�̓_��

//****************************
//�O���[�o���ϐ��錾
//****************************
LPDIRECT3DTEXTURE9 g_pTextureTitle[TITLETYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
TITLE g_Title[NUM_TITLE];								// �\���̕ϐ�
int g_nTitleCount;										// ��ʑJ�ڃJ�E���g

//============================
//�^�C�g���̏���������
//============================
void InitTitle(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			TITLE_TYPE[nCnt],
			&g_pTextureTitle[nCnt]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �\���̕ϐ��̏�����
	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		g_Title[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
		g_Title[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �p�x
		g_Title[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړ���
		g_Title[nCnt].bUse = false;						    // ���g�p���
		g_Title[nCnt].fHeight = 0.0f;					    // ����
		g_Title[nCnt].fWidth = 0.0f;					    // ����
		g_Title[nCnt].nType = TITLETYPE_TITLE;			    // ���
		g_Title[nCnt].TitleMenu = TITLESELECT_GAME;		    // �^�C�g���̃��j���[
		g_Title[nCnt].state = TITLESTATE_NORMAL;		    // ���

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
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		// ���_����i�߂�
		pVtx += 4;
	}

	// �O���[�o���ϐ��̏�����
	g_nTitleCount = 0;

	// �^�C�g�����Z�b�g
	SetTitle(D3DXVECTOR3(640.0f, 450.0f, 0.0f), TITLETYPE_TITLE, 200.0f, 50.0f);
	SetTitle(D3DXVECTOR3(640.0f, 600.0f, 0.0f), TITLETYPE_TUTO, 200.0f, 50.0f);

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();

}
//============================
//�^�C�g���̏I������
//============================
void UninitTitle(void)
{
	// ���y���X�g�b�v
	StopSound();

	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TITLETYPE_MAX; nCnt++)
	{
		if (g_pTextureTitle[nCnt] != NULL)
		{
			g_pTextureTitle[nCnt]->Release();
			g_pTextureTitle[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
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
		case TITLESELECT_GAME:

			if (KeyboardTrigger(DIK_DOWN) || KeyboardTrigger(DIK_S))
			{
				// ���y�Đ�
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_TUTO; // ���j���[�`���[�g���A��

			}
			if (g_Title[nCnt].state != TITLESTATE_FLASH)
			{
				TitleMenuFlash(TITLESELECT_GAME); // �`���[�g���A���_��
			}

			if (KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_START) || OnMouseTriggerDown(LEFT_MOUSE))
			{//Enter�L�[����������
				//�Q�[����ʂ�
				SetFade(MODE_GAME);

				// ���y�Đ�
				PlaySound(SOUND_LABEL_ENTER_SE);

				g_Title[nCnt].state = TITLESTATE_FLASH;
			}
			// �_��
			FlashGameUI(TITLESELECT_GAME);

			break;
		case TITLESELECT_TUTO:
			if (KeyboardTrigger(DIK_UP) || KeyboardTrigger(DIK_W))
			{
				// ���y�Đ�
				PlaySound(SOUND_LABEL_SELECT_SE);

				g_Title[nCnt].TitleMenu = TITLESELECT_GAME; // ���j���[�Q�[��
			}

			if (g_Title[nCnt].state != TITLESTATE_FLASH)
			{
				TitleMenuFlash(TITLESELECT_TUTO); // ���j���[�`���[�g���A��
			}

			if (KeyboardTrigger(DIK_RETURN) || JoypadTrigger(JOYKEY_START) || OnMouseTriggerDown(LEFT_MOUSE))
			{//Enter�L�[����������
				//�`���[�g���A����ʂ�
				SetFade(MODE_TUTORIAL);

				// ���y�Đ�
				PlaySound(SOUND_LABEL_ENTER_SE);

				g_Title[nCnt].state = TITLESTATE_FLASH; // �Q�[���_��
			}
			// �_��
			FlashGameUI(TITLESELECT_TUTO);
			break;
		default:
			break;
		}

		// �^�C�g���̓_�ŏ���
		TitleFlash(g_Title[nCnt].state,g_Title[nCnt].TitleMenu,nCnt);
	}

	// �J�E���^�[�����Z
	g_nTitleCount++;

	if (g_nTitleCount >= 600)
	{// 10�b�o��
		// �����L���O��ʂ�
		SetFade(MODE_RANKING);
	}
}
//============================
//�^�C�g���̕`�揈��
//============================
void DrawTitle(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[g_Title[nCnt].nType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
//============================
//�^�C�g���̕`�揈��
//============================
void SetTitle(D3DXVECTOR3 pos, int nType,float fWidth, float fHeight)
{	
	//���_���̃|�C���^
	VERTEX_2D* pVtx; 

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		// ���g�p��������
		if (!g_Title[nCnt].bUse)
		{
			g_Title[nCnt].pos = pos;	 // ���W
			g_Title[nCnt].nType = nType; // ���
			g_Title[nCnt].bUse = true;	 // �g�p����

			// ���_���W�̍X�V
			pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

			break;
		}
		// ���_����i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}
//============================
//�^�C�g���̑I������
//============================
void SelectTitle(int select)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		if (nCnt == select)
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);
		}
		else
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		}

		// ���_����i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}
//============================
//�^�C�g���̓_�ŏ���
//============================
void TitleFlash(int state,int nSelect,int nIdx)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	//�_�ŃJ�E���g�p�̃��[�J���ϐ�
	static int nCounterFlash = 0; 

	// ���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		if (state != TITLESTATE_FLASH)
		{
			continue; // �_�ł���Ȃ��������΂�
		}

		nCounterFlash++; // �J�E���g�����Z

		if (nSelect==TITLESELECT_GAME)
		{
			if (nCounterFlash == 5)
			{// �J�E���g��5�̎�
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}
			else if (nCounterFlash == 10)
			{// �J�E���g��10�̎�
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// ����������
				nCounterFlash = 0;
			}
		}
		else if (nSelect == TITLESELECT_TUTO)
		{
			if (nCounterFlash == 5)
			{// �J�E���g��5�̎�

				pVtx += 4 * nSelect;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			}
			else if (nCounterFlash == 10)
			{// �J�E���g��10�̎�
				pVtx += 4 * nSelect;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// ������
				nCounterFlash = 0;
			}
		}
	}
	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}
//============================
//�^�C�g���̃��j���[�̓_�ŏ���
//============================
void TitleMenuFlash(int nSelect)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���[�J���ϐ�
	static float fA = 1.0f;	  // �����x
	static bool bAlv = false; // ����p

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_TITLE; nCnt++)
	{
		if (nSelect == nCnt)
		{
			// false�̎��ɓ�����
			if (fA > 0.3f && !bAlv)
			{
				fA -= 0.005f; // ���Z

				if (fA <= 0.3f)
				{
					bAlv = true;
				}
			}
			// true�̎��ɂ��񂾂񌩂���悤�ɂȂ�
			else if (fA <= 1.0f && bAlv)
			{
				fA += 0.005f; // ���Z

				if (fA >= 1.0f)
				{
					bAlv = false;
				}
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
		}
		else
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		// ���_����i�߂�
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();
}