//==============================================================================================================
//
// ���U���g [result.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "result.h"
#include "fade.h"
#include "input.h"
#include "time.h"
#include "camera.h"
#include "resultscore.h"
#include "sound.h"
#include "player.h"
#include "Timer.h"
#include "gameui.h"
#include "mouse.h"

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;// ���_�o�b�t�@�ւ̃|�C���^

//=======================================================================================================
//���U���g�̏���������
//=======================================================================================================
void InitResult(void)
{
	// UI�̏�����
	InitGameUI();
	
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �e�N�X�`���̃p�X��ۑ�
	const char* Texturename = {};

	if (GetPlayer()->bDisp || GetTimer() <= 0)
	{// �v���C���[������ or �^�C�}�[��0�b�ȉ�
		Texturename = "data/TEXTURE/result.png";
	}
	else if (GetPlayer()->bDisp == false)
	{// �v���C���[�����S
		Texturename = "data/TEXTURE/result1.jpg";
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		Texturename,
		&g_pTextureResult);

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	// ���_�o�b�t�@�̃��b�N
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�A�����b�N
	g_pVtxBuffResult->Unlock();

	// ���U���g�X�R�A�̏���������
	InitResultScore();

	// ���y���Đ�
	PlaySound(SOUND_LABEL_RESULT_BGM);
}
//=======================================================================================================
//���U���g�̏I������
//=======================================================================================================
void UninitResult(void)
{
	// ���y���~
	StopSound();

	// UI�̏I������
	UninitGameUI();

	//�e�N�X�`���̔j��
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}
	// �o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	// ���U���g�X�R�A�̏I������
	UninitResultScore();
}
//=======================================================================================================
//���U���g�̍X�V����
//=======================================================================================================
void UpdateResult(void)
{
	// ���݂̃��[�h���擾
	MODE Mode = GetMode();

	if (KeyboardTrigger(DIK_RETURN) == true||JoypadTrigger(JOYKEY_A)==true || OnMouseTriggerDown(LEFT_MOUSE))
	{// Enter�L�[ or �p�b�h��A�{�^�� or ���N���b�N 
		// �����L���O�ɑJ��
		SetFade(MODE_RANKING);
	}

	// ���U���g�X�R�A�̍X�V
	UpdateResultScore();
}
//=======================================================================================================
//���U���g�̕`�揈��
//=======================================================================================================
void DrawResult(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResult);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// ���U���g�X�R�A�̕`�揈��
	DrawResultScore();
}