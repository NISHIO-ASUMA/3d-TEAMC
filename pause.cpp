//==============================================================================================================
//
// �|�[�Y [pause.h]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include"pause.h"
#include"input.h"
#include"game.h"
#include"fade.h"
#include "mouse.h"

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_MENU_MAX] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//���_�o�b�t�@�ւ̃|�C���^
PAUSE_MENU g_PauseMenu;//�|�[�Y���j���[

//=====================================================================================================
//�|�[�Y�̏���������
//=====================================================================================================
void InitPause(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`��1�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause_continue.png",
		&g_apTexturePause[0]);

	// �e�N�X�`��2�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause_retry.png",
		&g_apTexturePause[1]);

	// �e�N�X�`��3�̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\pause_quit.png",
		&g_apTexturePause[2]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4* PAUSE_MENU_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(400.0f, (100.0f + nCntPause * 200), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(900.0f, (100.0f + nCntPause * 200), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(400.0f, (200.0f + nCntPause * 200), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(900.0f, (200.0f + nCntPause * 200), 0.0f);

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

		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4; // ���_�f�[�^�̃|�C���^��4�����₷
	}
	// �A�����b�N
	g_pVtxBuffPause->Unlock();

	g_PauseMenu = PAUSE_MENU_CONTINUE; // �|�[�Y���j���[�̏�����
}
//=====================================================================================================
//�|�[�Y�̏I������
//=====================================================================================================
void UninitPause(void)
{
	// �e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}
	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//=====================================================================================================
//�|�[�Y�̍X�V����
//=====================================================================================================
void UpdatePause(void)
{
	switch (g_PauseMenu)
	{
	case PAUSE_MENU_CONTINUE:
		
		if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN)==true || KeyboardTrigger(DIK_S) == true)
		{
			g_PauseMenu = PAUSE_MENU_RETRY; // ���g���C�I��
		}
		else if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
		{
			g_PauseMenu = PAUSE_MENU_QUIT; // ��߂�I��
		}
		SelectPause(0);
		break;
	case PAUSE_MENU_RETRY:
		if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true)
		{
			g_PauseMenu = PAUSE_MENU_QUIT; // ��߂�I��
		}
		else if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
		{
			g_PauseMenu = PAUSE_MENU_CONTINUE; // ������I��	
		}
		SelectPause(1);
		break;

	case PAUSE_MENU_QUIT:
		
		if (KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true)
		{
			g_PauseMenu = PAUSE_MENU_CONTINUE; // ������I��
		}
		else if (KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true)
		{
			g_PauseMenu = PAUSE_MENU_RETRY; // ���g���C�I��
		}
		SelectPause(2);
		break;
	}

	if (KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(LEFT_MOUSE) == true)
	{// Enter�L�[ or A�{�^�� or ���N���b�N
		switch (g_PauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY:
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:
			SetFade(MODE_TITLE);
			break;
		}
	}
}
//=====================================================================================================
//�|�[�Y�̕`�揈��
//=====================================================================================================
void DrawPause(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTexturePause[nCntPause]);

		// �v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);//�v���~�e�B�u�̎��
	}
}
//=====================================================================================================
//�|�[�Y�̑I������
//=====================================================================================================
void SelectPause(int nNumSelect)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (nCntPause == nNumSelect)
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 140);
			pVtx[1].col = D3DCOLOR_RGBA(0, 255, 255, 140);
			pVtx[2].col = D3DCOLOR_RGBA(0, 255, 255, 140);
			pVtx[3].col = D3DCOLOR_RGBA(0, 255, 255, 140);
		}
		pVtx += 4;
	}
	// �A�����b�N
	g_pVtxBuffPause->Unlock();
}