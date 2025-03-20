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
#include "mark.h"
#include "math.h"
#include "tutorial3d.h"
#include "sound.h"

//**************************************************************************************************************
// �v���g�^�C�v�錾
//**************************************************************************************************************
void SelectPauseMenuPos(int nSelect);	// �I�𒆂̃��j���[
void SetPauseUIAnim(int nType);			// �|�[�Y���̃A�j���[�V����
void CreateUITexPos(int nType);			// UI�̃e�N�X�`�����W�̍쐬
void CreateUIPos(int nType, D3DXVECTOR3 pos, float fWidth, float fHeight);			// UI�̍��W�̍쐬
void CreateUIColor(int nType);			// UI�̐F�ݒ�

//**************************************************************************************************************
// �O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_apTexturePause[PAUSE_TEXTURE_MAX] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//���_�o�b�t�@�ւ̃|�C���^
Pause g_Pause[PAUSE_TEXTURE_MAX];//�|�[�Y���j���[
PAUSE_MENU g_PauseMenu; // �|�[�Y���j���[
bool bSelect;

//=====================================================================================================
//�|�[�Y�̏���������
//=====================================================================================================
void InitPause(void)
{
	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �|�[�Y�̃e�N�X�`��
	for (int nCnt = 0; nCnt < PAUSE_TEXTURE_MAX; nCnt++)
	{
		// �e�N�X�`��1�̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			PAUSE_TEXTURE[nCnt],
			&g_apTexturePause[nCnt]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PAUSE_TEXTURE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	bSelect = false;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
	
	g_PauseMenu = PAUSE_MENU_CONTINUE; // �|�[�Y���j���[�̏�����

	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		g_Pause[nCntPause].pos = NULLVECTOR3;			 // �ʒu
		g_Pause[nCntPause].fWidth = 0.0f;				 // ����
		g_Pause[nCntPause].fHeight = 0.0f;				 // ����

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f,0.0f, 0.0f);

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

	// �w�i�̐ݒ�
	SetPause(D3DXVECTOR3(640.0f, 360.0f, 0.0f), PAUSE_TEXTURE_BG, 640.0f, 360.0f);

	// �|�[�Y���j���[�P�̐ݒ�
	SetPause(D3DXVECTOR3(240.0f, 160.0f, 0.0f), PAUSE_TEXTURE_CONTINUE, 300.0f, 35.0f);

	// �|�[�Y���j���[2�̐ݒ�
	SetPause(D3DXVECTOR3(240.0f, 360.0f, 0.0f), PAUSE_TEXTURE_RETRY, 300.0f, 35.0f);

	// �|�[�Y���j���[3�̐ݒ�
	SetPause(D3DXVECTOR3(240.0f, 560.0f, 0.0f), PAUSE_TEXTURE_QUIT, 300.0f, 35.0f);

	// �|�[�Y��UI�̐ݒ�
	SetPause(D3DXVECTOR3(1100.0f, 660.0f, 0.0f), PAUSE_TEXTURE_PAUSEUI, 100.0f, 35.0f);

	// �N���t�g�̃��V�s�̐ݒ�
	SetPause(D3DXVECTOR3(900.0f, 350.0f, 0.0f), PAUSE_TEXTURE_RECIPE, 200.0f, 200.0f);
}
//=====================================================================================================
//�|�[�Y�̏I������
//=====================================================================================================
void UninitPause(void)
{
	// ���y���~
	StopSound();

	// �e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
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
		
		if ((KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN)==true || KeyboardTrigger(DIK_S) == true) && bSelect == false)
		{
			// �T�E���h�Đ�
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_RETRY; // ���g���C�I��
		}
		else if ((KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true) && bSelect == false)
		{
			// �T�E���h�Đ�
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_QUIT; // ��߂�I��
		}
		SelectPause(PAUSE_MENU_CONTINUE);
		SelectPauseMenuPos(PAUSE_MENU_CONTINUE);

		break;
	case PAUSE_MENU_RETRY:
		if ((KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true && bSelect == false))
		{
			// �T�E���h�Đ�
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_QUIT; // ��߂�I��
		}
		else if ((KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true && bSelect == false))
		{
			// �T�E���h�Đ�
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_CONTINUE; // ������I��	
		}
		SelectPause(PAUSE_MENU_RETRY);
		SelectPauseMenuPos(PAUSE_MENU_RETRY);
		break;

	case PAUSE_MENU_QUIT:
		
		if ((KeyboardTrigger(DIK_DOWN) == true || JoypadTrigger(JOYKEY_DOWN) == true || KeyboardTrigger(DIK_S) == true) && bSelect == false)
		{
			// �T�E���h�Đ�
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_CONTINUE; // ������I��
		}
		else if ((KeyboardTrigger(DIK_UP) == true || JoypadTrigger(JOYKEY_UP) == true || KeyboardTrigger(DIK_W) == true) && bSelect == false)
		{
			// �T�E���h�Đ�
			PlaySound(SOUND_LABEL_SELECT_SE);

			g_PauseMenu = PAUSE_MENU_RETRY; // ���g���C�I��
		}
		SelectPause(PAUSE_MENU_QUIT);
		SelectPauseMenuPos(PAUSE_MENU_QUIT);
		break;
	}

	// ���肵����
	if ((KeyboardTrigger(DIK_RETURN) == true || JoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(LEFT_MOUSE) == true) && bSelect == false)
	{// Enter�L�[ or A�{�^�� or ���N���b�N

		// �T�E���h�Đ�
		PlaySound(SOUND_LABEL_ENTER_SE);

		switch (g_PauseMenu)
		{
		case PAUSE_MENU_CONTINUE:
			bSelect = false;
			SetEnablePause(false);
			break;
		case PAUSE_MENU_RETRY:
			bSelect = true; // �I������
			SetFade(MODE_GAME);
			break;
		case PAUSE_MENU_QUIT:
			bSelect = true; // �I������
			SetFade(MODE_TITLE);
			break;
		}
	}

	// �|�[�Y���̃e�N�X�`���A�j���[�V����
	SetPauseUIAnim(PAUSE_TEXTURE_PAUSEUI);
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

	// �|�[�Y�̃e�N�X�`������
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
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

	// �|�[�Y�̃e�N�X�`������
	for (int nCntPause = PAUSE_MENU_CONTINUE; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		if (nCntPause == nNumSelect)
		{
			// ���_�o�b�t�@�̃��b�N
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = COLOR_KHAKI;
			pVtx[1].col = COLOR_KHAKI;
			pVtx[2].col = COLOR_KHAKI;
			pVtx[3].col = COLOR_KHAKI;

			// �A�����b�N
			g_pVtxBuffPause->Unlock();
		}
		else
		{
			// ���_�o�b�t�@�̃��b�N
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �A�����b�N
			g_pVtxBuffPause->Unlock();
		}
	}
}
//==============================================================================================================
// �|�[�Y�̐ݒ菈��
//==============================================================================================================
void SetPause(D3DXVECTOR3 pos, int nMenu, float fWidth, float fHeight)
{
	// �e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		if (nMenu == nCntPause)
		{
			g_Pause[nCntPause].pos = pos;			 // �ʒu
			g_Pause[nCntPause].fWidth = fWidth;		 // ����
			g_Pause[nCntPause].fHeight = fHeight;	 // ����

			// ���W�̍쐬
			CreateUIPos(nMenu, pos, fWidth, fHeight);

			// �F�̍쐬
			CreateUIColor(nMenu);

			// �e�N�X�`�����W�̍쐬
			CreateUITexPos(nMenu);

			break;
		}
	}
}
//==============================================================================================================
// �I�𒆂̃��j���[
//==============================================================================================================
void SelectPauseMenuPos(int nSelect)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	for (int nCntPause = PAUSE_MENU_CONTINUE; nCntPause < PAUSE_MENU_MAX; nCntPause++)
	{
		// �ʒu
		D3DXVECTOR3 pos = g_Pause[nCntPause].pos;

		// ����
		float width = g_Pause[nCntPause].fWidth;

		// ����
		float height = g_Pause[nCntPause].fHeight;

		if (nCntPause == nSelect)
		{
			pos.x += SetSmoothAprroach(740.0f, pos.x,0.1f);

			// ���_�o�b�t�@�̃��b�N
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

			// �A�����b�N
			g_pVtxBuffPause->Unlock();
		}
		else
		{
			pos.x += SetSmoothAprroach(240.0f, pos.x, 0.1f);

			// ���_�o�b�t�@�̃��b�N
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntPause;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

			// �A�����b�N
			g_pVtxBuffPause->Unlock();
		}
	}
}
//==============================================================================================================
// �|�[�Y���̃A�j���[�V����
//==============================================================================================================
void SetPauseUIAnim(int nType)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`���̔j��
	for (int nCntPause = 0; nCntPause < PAUSE_TEXTURE_MAX; nCntPause++)
	{
		if (nType == nCntPause)
		{
			static int nPatternAnim = 0;
			static int nCounterAnim = 0;

			// ���_�o�b�t�@�̃��b�N
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			// �e�N�X�`���A�j���[�V����
			SetTextureAnimation(3, 1, 35, &nCounterAnim, &nPatternAnim, pVtx, g_pVtxBuffPause, nCntPause);

			// �A�����b�N
			g_pVtxBuffPause->Unlock();

			// �p�^�[����0�̂Ƃ����炷
			if (nPatternAnim == 0) g_Pause[nCntPause].pos.x = 1100.0f;

			// �p�^�[����1�̂Ƃ����炷
			if(nPatternAnim == 1) g_Pause[nCntPause].pos.x = 1106.0f;

			// �p�^�[����2�̂Ƃ����炷
			if (nPatternAnim == 2) g_Pause[nCntPause].pos.x = 1116.0f;

			// �ʒu
			D3DXVECTOR3 pos = g_Pause[nCntPause].pos;

			// ����
			float width = g_Pause[nCntPause].fWidth;

			// ����
			float height = g_Pause[nCntPause].fHeight;

			// ���_�o�b�t�@�̃��b�N
			g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nType;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pos.x - width, pos.y - height, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pos.x + width, pos.y - height, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pos.x - width, pos.y + height, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pos.x + width, pos.y + height, 0.0f);

			// �A�����b�N
			g_pVtxBuffPause->Unlock();
		}
	}
}
//==============================================================================================================
// UI�̃e�N�X�`�����W�̍쐬
//==============================================================================================================
void CreateUITexPos(int nType)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nType;

	if (nType == PAUSE_TEXTURE_PAUSEUI)
	{
		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.333f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.333f, 1.0f);
	}
	else
	{
		// �e�N�X�`���̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// �A�����b�N
	g_pVtxBuffPause->Unlock();
}
//==============================================================================================================
// UI�̍��W�̍쐬
//==============================================================================================================
void CreateUIPos(int nType,D3DXVECTOR3 pos,float fWidth,float fHeight)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nType;

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(pos.x - fWidth, pos.y - fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(pos.x + fWidth, pos.y - fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(pos.x - fWidth, pos.y + fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(pos.x + fWidth, pos.y + fHeight, 0.0f);

	// �A�����b�N
	g_pVtxBuffPause->Unlock();

}
//==============================================================================================================
// UI�̐F�ݒ�
//==============================================================================================================
void CreateUIColor(int nType)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nType;

	if (nType == PAUSE_TEXTURE_BG)
	{
		// ���_���W�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 100);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 100);
	}
	else
	{
		// ���_���W�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	// �A�����b�N
	g_pVtxBuffPause->Unlock();

}
