//=====================================================================================================================
//
// �`���[�g���A������̃v���O���� [tutorialsupport.cpp]
// Author; �����I��
//
//=====================================================================================================================

//**************************************************************************************************************
// �C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "player.h"
#include "TutorialSupport.h"
#include "sound.h"
#include "camera.h"
#include "input.h"
#include "spgauge.h"

//**************************************************************************************************************
// ��ނ̗񋓌^�錾
//**************************************************************************************************************
typedef enum
{
	TYPE_0 = 0, // �ړ�
	TYPE_1,     // �W�����v
	TYPE_2,     // ���
	TYPE_3,		// �U��
	TYPE_4,		// SP�U��
	TYPE_5,		// �E��
	TYPE_6,     // �X�g�b�N
	TYPE_7,     // ����
	TYPE_8,     // 
	TYPE_9,		// ���R����
	TYPE_MAX
}TYPE;

//**************************************************************************************************************
// �e�N�X�`���񋓌^�錾
//**************************************************************************************************************
static const char* TUTOTEX[TYPE_MAX] =
{
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_1.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_4.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_3.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_2.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_3.5.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_5.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_6.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_key_7.png",
	"data\\TEXTURE\\UI_craft.png",
	"data\\TEXTURE\\tutorial_Temporary\\tuto_step_8.png",
};

//**************************************************************************************************************
// �T�|�[�g�\���̐錾
//**************************************************************************************************************
typedef struct
{
	int nType; // ���
}TutoSupport;

//**************************************************************************************************************
// �O���[�o���ϐ��錾
//************************************************************************************************************** 
LPDIRECT3DTEXTURE9 g_pTexture_Tutorial[TYPE_MAX] = {}; // �e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManager = NULL; // ���_�o�b�t�@�̃|�C���^
TutoSupport g_Tutosupport; // �\���̕ϐ�
int nSteps = 0;    // �X�e�b�v���̃J�E���g
float fALv2 = 1.0f;
float bAmove = 0.0f;
float fPlusX = 0.0f;

//==============================================================================================================
// �`���[�g���A���T�|�[�g�̏���������
//==============================================================================================================
void InitManager(void)
{
	// �f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �O���[�o���ϐ��̏�����
	nSteps = 0;
	fALv2 = 1.0f;
	bAmove = 0.0f;
	g_Tutosupport.nType = 0;

	// �e�N�X�`���̐ݒ�
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		D3DXCreateTextureFromFile(pDevice,
			TUTOTEX[nCnt],
			&g_pTexture_Tutorial[nCnt]);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffManager, 
		NULL);

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffManager->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(950.0f + fPlusX, 70.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1250.0f + fPlusX, 70.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(950.0f + fPlusX, 240.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1250.0f + fPlusX, 240.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManager->Unlock();
}
//==============================================================================================================
// �`���[�g���A���T�|�[�g�̏I������
//==============================================================================================================
void UninitManager(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (g_pTexture_Tutorial[nCnt] != NULL)
		{
			g_pTexture_Tutorial[nCnt]->Release();
			g_pTexture_Tutorial[nCnt] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffManager != NULL)
	{
		g_pVtxBuffManager->Release();
		g_pVtxBuffManager = NULL;
	}
}
//==============================================================================================================
// �`���[�g���A���T�|�[�g�̍X�V����
//==============================================================================================================
void UpdateManager(void)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �A�C�e���̎擾
	Item* pItem = GetItem();

	// �J�����̎擾
	Camera* pCamera = GetCamera();

	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���Ē��_���ւ̃|�C���^���擾
	g_pVtxBuffManager->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(950.0f + fPlusX, 70.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1250.0f + fPlusX, 70.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(950.0f + fPlusX, 240.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1250.0f + fPlusX, 240.0f, 0.0f);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fALv2);

	// �A�����b�N����
	g_pVtxBuffManager->Unlock();

	// �ړ��ʂ����Z
	fALv2 += bAmove;

	if (fALv2 > 1.0f)
	{
		fALv2 = 1.0f;
		bAmove = 0.0f;
	}
	else if (fALv2 < 0.0f)
	{
		fALv2 = 0.0f;
		nSteps++;
		bAmove *= -1.0f;
	}

	fPlusX += bAmove * -100;

	if (nSteps == 0 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_MOVE && fALv2 == 1.0f)
	{// �ŏ�
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 1 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_JUMP && fALv2 == 1.0f)
	{// 2�Ԗ�
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 2 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_AVOID && fALv2 == 1.0f)
	{// 3�Ԗ�
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 3 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION && fALv2 == 1.0f)
	{// 4�Ԗ�
		if (pPlayer->AttackSp == false)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			AddSpgauge(300.0f);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 4 && pPlayer->Motion.motiontypeBlend == MOTIONTYPE_ACTION && fALv2 == 1.0f)
	{// 5�Ԗ�
		if (pPlayer->AttackSp == true)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 5 && pPlayer->Motion.nNumModel == 16 && fALv2 == 1.0f)
	{// 6�Ԗ�
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 6 && pItem[pPlayer->StockItemIdx].state == ITEMSTATE_STOCK && fALv2 == 1.0f)
	{// 7�Ԗ�
		PlaySound(SOUND_LABEL_ENTER_SE);
		bAmove = -0.01f;
	}
	else if (nSteps == 7 && fALv2 == 1.0f)
	{// 8�Ԗ�
		if (pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_STONE && pItem[pPlayer->ItemIdx].nType == ITEMTYPE_BAT)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
		else if (pItem[pPlayer->StockItemIdx].nType == ITEMTYPE_BAT && pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONE)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
	}
	else if (nSteps == 8 && fALv2 == 1.0f)
	{// 9�Ԗ�
		if (pItem[pPlayer->ItemIdx].nType == ITEMTYPE_STONEBAT)
		{
			PlaySound(SOUND_LABEL_ENTER_SE);
			bAmove = -0.01f;
		}
	}
}
//==============================================================================================================
// �`���[�g���A���T�|�[�g�̕`�揈��
//==============================================================================================================
void DrawManager(void)
{
	// �f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffManager, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ��ނ���
	g_Tutosupport.nType = nSteps;

	// �e�N�X�`����ݒ�
	pDevice->SetTexture(0, g_pTexture_Tutorial[g_Tutosupport.nType]);

	// �|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}