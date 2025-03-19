//==============================================================================================================
//
// �Q�[���ɕK�v��UI [gameui.cpp]
// Author:TEAM_C
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "gameui.h"
#include "HPGauge.h"
#include "player.h"
#include "math.h"
#include "easing.h"
#include "event.h"
#include "game.h"
#include "boss.h"
#include "manual.h"
#include "title.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define DEST_WIDTHEXPANSION (120.0f) // �ڕW�̊g�嗦(����)
#define DEST_WIDTHREDUCTION (100.0f) // �ڕW�̏k����(����)

#define DEST_HEIGHTEXPANSION (45.0f) // �ڕW�̊g�嗦(�c��)
#define DEST_HEIGHTREDUCTION (25.0f) // �ڕW�̏k����(�c��)

#define MAX_UI (120)				 // �ő�UI��

#define UITYPE_SPWIDTH (55.0f)		 // �X�y�V����UI�̉���
#define UITYPE_SPHEIGHT (50.0f)		 // �X�y�V����UI�̍���

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void UIFlash(int nType);				// �_�ŏ���
void UpdateDestroyUI(int nCnt);			// ���킪��ꂽ����UI
void SetEventUIAnimation(int nCnt);		// �C�x���g��UI�̐ݒ�
void SetTutoUIAnimation(int nCnt);		// �`���[�g���A��UI�̃A�j���[�V����
void SetDamageUIAnimation(int nCnt);	// �_���[�WUI�̐ݒ�
void SetTerritoryTimeUI(int nCnt);		// �e���g���[���o��܂ł̎���
void SetBossManual(int nCnt);			// �{�X�̃}�j���A���̐ݒ�
void UpdateUIFlash(int nCnt, float* pAlv,float Maxtime);	// UI�̓_�ŏ���(�g���܂킹����)
void SetTerritoryEnemyUI(int nCnt);		// �e���g���[�ɓG���o������UI�̐ݒ�
void UIScalAnimation(int nCnt,float widthEx,float widthdec,float heightEx,float heigthDec, float MaxtimeWidth,float MaxtimeHeight); // �g��k���̃A�j���[�V����
void UpdateCraftTimeUI(int nCnt);		// �N���t�g�̎��Ԓ���UI
void UpdateCraftTimeMenuUI(int nCnt);	// �N���t�g���j���[��UI�̐ݒ�

float fcolorA;

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameUI[UITYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;	  // ���_�o�b�t�@�ւ̃|�C���^
Gameui g_GameUI[MAX_UI];
int g_nPatternAnim, g_nCounterAnim;

//==============================================================================================================
// UI�̏���������
//==============================================================================================================
void InitGameUI(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			UITYPE_INFO[nCnt],
			&g_pTextureGameUI[nCnt]);
	}

	// ���_�o�b�t�@�̐����E���_���̐ݒ�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UI,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameUI,
		NULL);

	// ���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	fcolorA = 0.0f;
	g_nPatternAnim = 0;
	g_nCounterAnim = 0;

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		g_GameUI[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_GameUI[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_GameUI[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_GameUI[nCnt].fHeight = 0.0f;
		g_GameUI[nCnt].fWidth = 0.0f;
		g_GameUI[nCnt].nUseTime = 0;
		g_GameUI[nCnt].bUse = false;
		g_GameUI[nCnt].nType = UITYPE_TITLE;
		g_GameUI[nCnt].col = COLOR_WHITE;
		g_GameUI[nCnt].nEaseCnt = 0;
		g_GameUI[nCnt].bLife = false;
		g_GameUI[nCnt].nCounterAnim = 0;
		g_GameUI[nCnt].nPatternAnim = 0;
		g_GameUI[nCnt].fAlv = 0.0f;

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

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UI�̏I������
//==============================================================================================================
void UninitGameUI(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_pTextureGameUI[nCnt] != NULL)
		{
			g_pTextureGameUI[nCnt]->Release();
			g_pTextureGameUI[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGameUI != NULL)
	{
		g_pVtxBuffGameUI->Release();
		g_pVtxBuffGameUI = NULL;
	}
}
//==============================================================================================================
// UI�̍X�V����
//==============================================================================================================
void UpdateGameUI(void)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	// �g�傷��
	static bool bExpansion = true;

	// 
	float a = (float)(1 % 10) * 0.1f;
	
	// ���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse == true)
		{
			switch (g_GameUI[nCnt].nType)
			{
			case UITYPE_TITLE:

				if (g_GameUI[nCnt].pos.y <= 140.0f)
				{
					g_GameUI[nCnt].pos.y += 5.0f; // ���Ɉړ�
				}

				break;
			case UITYPE_FIVER:

				if (g_GameUI[nCnt].fWidth >= 150.0f)
				{
					bExpansion = false;
				}
				else if (g_GameUI[nCnt].fWidth <= 100.0f)
				{
					bExpansion = true;
				}
				if (bExpansion == true)
				{
					g_GameUI[nCnt].fWidth += SetSmoothAprroach(160.0f, g_GameUI[nCnt].fWidth, 0.1f);
					g_GameUI[nCnt].fHeight += SetSmoothAprroach(70.0f, g_GameUI[nCnt].fHeight, 0.1f);
				}
				else if (bExpansion == false)
				{
					g_GameUI[nCnt].fWidth += SetSmoothAprroach(90.0f, g_GameUI[nCnt].fWidth, 0.1f);
					g_GameUI[nCnt].fHeight += SetSmoothAprroach(30.0f, g_GameUI[nCnt].fHeight, 0.1f);
				}

				if (pPlayer->FeverMode == false)
				{
					g_GameUI[nCnt].bUse = false;
				}

				//���_�J���[�̐ݒ�
				pVtx[0].col = COLOR_WHITE;
				pVtx[1].col = COLOR_WHITE;
				pVtx[2].col = COLOR_WHITE;
				pVtx[3].col = COLOR_WHITE;

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			case UITYPE_SYUTYUSEN:

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
				pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);

				SetTextureAnimation(2, 1, 2, &g_GameUI[nCnt].nCounterAnim, &g_GameUI[nCnt].nPatternAnim, pVtx, g_pVtxBuffGameUI, nCnt);

				if (!pPlayer->FeverMode)
				{
					g_GameUI[nCnt].bUse = false;
				}

				break;
			case UITYPE_TUTORIAL:
				SetTutoUIAnimation(nCnt);
				break;
			case UITYPE_BLACK:
			{
				if (fcolorA >= 0.9f)
				{
					fcolorA = 0.9f; // ���l���Œ�
				}
				else
				{
					fcolorA += 0.01f; // �C���N�������g
				}

				if (pPlayer->AttackSp && pPlayer->Motion.nKey == 4)
				{
					g_GameUI[nCnt].bUse = false; // ����
					fcolorA = 0.0f; // ������
				}

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fcolorA);
			}
			break;

			case UITYPE_KATANA:

				if (g_GameUI[nCnt].pos.x >= 650.0f)
				{
					g_GameUI[nCnt].pos.x -= 10.0f; // ���Ɉړ�
				}

				break;
			case UITYPE_RED:

				if (pPlayer->nLife <= 150)
				{
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
				}
				else
				{
					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				break;
			case UITYPE_DESTORY:

				UpdateDestroyUI(nCnt);

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(g_GameUI[nCnt].col);
				pVtx[1].col = D3DXCOLOR(g_GameUI[nCnt].col);
				pVtx[2].col = D3DXCOLOR(g_GameUI[nCnt].col);
				pVtx[3].col = D3DXCOLOR(g_GameUI[nCnt].col);

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			case UITYPE_EVENT:
				// �C�x���gUI�̐ݒ�
				SetEventUIAnimation(nCnt);

				//���_�J���[�̐ݒ�
				pVtx[0].col = COLOR_WHITE;
				pVtx[1].col = COLOR_WHITE;
				pVtx[2].col = COLOR_WHITE;
				pVtx[3].col = COLOR_WHITE;

				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

				break;
			case UITYPE_SP:
				// �e�N�X�`���A�j���[�V�����̐ݒ菈��
				SetTextureAnimation(2, 1, 10, &g_GameUI[nCnt].nCounterAnim, &g_GameUI[nCnt].nPatternAnim, pVtx, g_pVtxBuffGameUI, nCnt);

				// sp�����܂��Ă��Ȃ�������傫����0�ɂ��Č����Ȃ�����
				g_GameUI[nCnt].fWidth = pPlayer->SpMode ? UITYPE_SPWIDTH : 0.0f;

				// sp�����܂��Ă��Ȃ�������傫����0�ɂ��Č����Ȃ�����
				g_GameUI[nCnt].fHeight = pPlayer->SpMode ? UITYPE_SPHEIGHT : 0.0f;
				break;

			case UITYPE_DAMAGE:
				// �_���[�WUI�̐ݒ�
				SetDamageUIAnimation(nCnt);
				break;
			case UITYPE_SETENEMYTIME:
				// �e���g���[�o��܂ł̏���UI
				SetTerritoryTimeUI(nCnt);
				break;
			case UITYPE_BOSSMANUALEXIT:
				SetBossManual(nCnt);
				break;
			case UITYPE_POPENEMY:
				SetTerritoryEnemyUI(nCnt);
				break;
			case UITYPE_CRAFTTIME:
				UpdateCraftTimeUI(nCnt);
				break;
			case UITYPE_CRAFTTIMEMENU:
				UpdateCraftTimeMenuUI(nCnt);
				break;
			}

			// ����������
			if (g_GameUI[nCnt].bLife == true)
			{
				// ���������炷
				g_GameUI[nCnt].nUseTime--;

				// �g�p���Ԃ��I�����
				if (g_GameUI[nCnt].nUseTime <= 0)
				{
					// ����
					g_GameUI[nCnt].bUse = false;
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		}
		pVtx += 4;
	}

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UI�̕`�揈��
//==============================================================================================================
void DrawGameUI(void)
{
	// ���݂̃��[�h���擾
	MODE mode = GetMode();

	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�ւ̃|�C���^

    // �f�o�C�X�̏K��
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameUI, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse == false)
		{
			continue;
		}

		// ���
		int nType = g_GameUI[nCnt].nType;

		// �_���[�WUI����Ȃ�������
		if (nType != UITYPE_DAMAGE)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureGameUI[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
		// �_���[�WUI��������
		else
		{
			pDevice->SetTexture(0, g_pTextureGameUI[nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
	}
}
//==============================================================================================================
// UI�̐ݒ菈��
//==============================================================================================================
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, bool bLife,int nUseTime)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].bUse == false)
		{
			g_GameUI[nCnt].pos = pos;
			g_GameUI[nCnt].nType = nType;
			g_GameUI[nCnt].fWidth = fWidth;
			g_GameUI[nCnt].fHeight = fHeight;
			g_GameUI[nCnt].nUseTime = nUseTime;
			g_GameUI[nCnt].bLife = bLife;
			g_GameUI[nCnt].bUse = true;

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
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// �I������UI�_�ŏ���
//==============================================================================================================
void FlashGameUI(int nSelect)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (g_GameUI[nCnt].nType == UITYPE_TITLE2)
		{
			if (nSelect == TITLESELECT_GAME)
			{
				g_GameUI[nCnt].pos.y = 350.0f;
			}
			else if (nSelect == TITLESELECT_TUTO)
			{
				g_GameUI[nCnt].pos.y = 500.0f;
			}
			else if (nSelect == TITLESELECT_RANKING)
			{
				g_GameUI[nCnt].pos.y = 650.0f;
			}
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);

		pVtx += 4;
	}

	// ���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UI�̓_�ŏ���
//==============================================================================================================
void UIFlash(int nType)
{
	VERTEX_2D* pVtx;
	static float fA = 1.0f;
	static bool bAlv = false;

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (nType == nCnt)
		{
			// false�̎��ɓ�����
			if (fA > 0.01f && !bAlv)
			{
				fA -= 0.01f; // ���Z

				if (fA <= 0.01f)
				{
					bAlv = true;
				}
			}
			//true�̎��ɂ��񂾂񌩂���悤�ɂȂ�
			else if (fA <= 1.0f && bAlv)
			{
				fA += 0.01f; // ���Z

				if (fA >= 1.0f)
				{
					bAlv = false;
				}
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
		}
		else
		{

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		pVtx += 4;
	}

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UI�̓_�ŏ���
//==============================================================================================================
void UpdateDestroyUI(int nCnt)
{
	g_GameUI[nCnt].nUseTime--;
	static int WidthEase = 0;
	static int HeightEase = 0;

	HeightEase++;

	// �C�[�W���O��ݒ�
	float Widthtime = SetEase(WidthEase,120);

	static bool bWIdthExpansion = false;
	static bool bHeightExpansion = false;

	//// �g��̃J�E���g
	//static int nExpansionCnt = 0;

	//// �k���̃J�E���g
	//static int nReductionCnt = 0;

	// ������300.0f�𒴂�����
	if (g_GameUI[nCnt].fWidth >= DEST_WIDTHEXPANSION - 5.0f)
	{
		WidthEase = 0;
		// �k���J�n
		bWIdthExpansion = false;
	}
	// ������160�����������
	else if(g_GameUI[nCnt].fWidth <= DEST_WIDTHREDUCTION + 5.0f)
	{
		WidthEase = 0;
		// �g��J�n
		bWIdthExpansion = true;
	}

	// �g��ł���
	if (bWIdthExpansion == true)
	{
		// �g�傷��
		g_GameUI[nCnt].fWidth += SetSmoothAprroach(DEST_WIDTHEXPANSION, g_GameUI[nCnt].fWidth, 0.1f);
	}
	// �g��ł��Ȃ�
	else if (bWIdthExpansion == false)
	{
		g_GameUI[nCnt].fWidth += SetSmoothAprroach(DEST_WIDTHREDUCTION, g_GameUI[nCnt].fWidth, 0.1f);
	}

	// ������300.0f�𒴂�����
	if (g_GameUI[nCnt].fHeight >= DEST_HEIGHTEXPANSION - 5.0f)
	{
		// �k���J�n
		bHeightExpansion = false;
	}
	// ������160�����������
	else if (g_GameUI[nCnt].fHeight <= DEST_HEIGHTREDUCTION + 5.0f)
	{
		// �g��J�n
		bHeightExpansion = true;
	}

	// �g��ł���
	if (bHeightExpansion == true)
	{
		// �g�傷��
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(DEST_HEIGHTEXPANSION, g_GameUI[nCnt].fHeight, 0.1f);
	}
	// �g��ł��Ȃ�
	else if (bHeightExpansion == false)
	{
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(DEST_HEIGHTREDUCTION, g_GameUI[nCnt].fHeight, 0.1f);
	}

	// �g�p���Ԃ�0�ɂȂ��������
	if (g_GameUI[nCnt].nUseTime <= 0)
	{
		g_GameUI[nCnt].bUse = false;
	}
}
//==============================================================================================================
// �C�x���g��UI�̐ݒ�
//==============================================================================================================
void SetEventUIAnimation(int nCnt)
{
	GAMESTATE gamestate = GetGameState();
	static int fWidthEasing = 0;
	static int fHeightEasing = 0;

	if (gamestate == GAMESTATE_MOVIE)
	{
		fWidthEasing++;
		fHeightEasing++;

		float timeWidth = SetEase(fWidthEasing, 180);
		float timeHeight = SetEase(fHeightEasing, 160);

		g_GameUI[nCnt].fWidth += SetSmoothAprroach(250.0f, g_GameUI[nCnt].fWidth, EaseInSine(timeWidth));
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(80.0f, g_GameUI[nCnt].fHeight, EaseInSine(timeHeight));
	}
	// �Q�[����Ԃ����ʂ̎�
	if (gamestate == GAMESTATE_NORMAL)
	{
		fWidthEasing = 0;
		fHeightEasing = 0;

		g_GameUI[nCnt].fWidth += SetSmoothAprroach(100.0f, g_GameUI[nCnt].fWidth, 0.1f);
		g_GameUI[nCnt].fHeight += SetSmoothAprroach(30.0f, g_GameUI[nCnt].fHeight, 0.1f);

		g_GameUI[nCnt].pos.x += SetSmoothAprroach(1150.0f, g_GameUI[nCnt].pos.x, 0.1f);

		g_GameUI[nCnt].pos.y += SetSmoothAprroach(230.0f, g_GameUI[nCnt].pos.y, 0.1f);
	}

	// �{�X�̐��̎擾
	int NumBoss = GetNumBoss();

	// �{�X�����Ȃ�������
	if (NumBoss <= 0)
	{
		g_GameUI[nCnt].bUse = false;
	}
}
//==============================================================================================================
// �`���[�g���A��UI�̃A�j���[�V����
//==============================================================================================================
void SetTutoUIAnimation(int nCnt)
{
	VERTEX_2D* pVtx;

	// ����������t���O
	static bool bDec = true;

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	// �A���t�@�l
	static float fA = 1.0f;
	
	// �A���t�@�l�̌����
	float DecAlv = 1.0f / 120.0f;

	// ���炷��Ԃ�������
	if (bDec == true)
	{
		// �A���t�@�l�������w����
		fA -= DecAlv;
	}
	// ���₷��Ԃ�������
	else
	{
		// �A���t�@�l�𑝂₷
		fA += DecAlv;
	}

	// �����x��1.0f�ɂȂ�����
	if (fA >= 1.0f)
	{
		// ����������
		bDec = true;
	}
	// �����x��0.0f�ɂȂ�����
	else if (fA <= 0.0f)
	{
		// ���₷
		bDec = false;
	}
	pVtx += 4 * nCnt;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fA);

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// �_���[�WUI�̐ݒ�
//==============================================================================================================
void SetDamageUIAnimation(int nCnt)
{
	// �v���C���[�̎擾
	Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	// �ړI�̃A���t�@�l
	static float fDestAlv = 0.0f;

	// ���݂̃A���t�@�l
	static float fAlv = 0.0f;

	// ��Ԃ��_���[�W��������
	const bool isDamage = pPlayer->state == PLAYERSTATE_DAMAGE;

	// �_���[�W��Ԃ�������
	fDestAlv = isDamage ? 0.5f : 0.0f;

	// �ړI�̒l�ɒm�킯��
	fAlv += SetSmoothAprroach(fDestAlv, fAlv, 0.1f);

	pVtx += 4 * nCnt;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();

}
//==============================================================================================================
// �e���g���[���o��܂ł̎���
//==============================================================================================================
void SetTerritoryTimeUI(int nCnt)
{
	// �e���g���[�̐��̎擾
	int NumTerritory = GetNumTeriitory();

	// �e���g���[�̐���0�݂���������
	const bool isSetUI = NumTerritory <= 0;

	// �ŏ��̃N���t�g���[�h��������
	const bool NotCraftTime = GetFirstCraftTIme() == true;

	// �傫����ݒ�
	g_GameUI[nCnt].fWidth = NotCraftTime ?  0.0f : (isSetUI ? 40.0f : 0.0f);
	g_GameUI[nCnt].fHeight = NotCraftTime ?  0.0f : (isSetUI ? 25.0f : 0.0f);
}
//==============================================================================================================
// �{�X�̃}�j���A���̐ݒ�
//==============================================================================================================
void SetBossManual(int nCnt)
{
	VERTEX_2D* pVtx;
	 
	// �A���t�@�l
	static float fAlv = 1.0f;

	// �A���t�@�l�̌����t���O
	static bool isDecAlv = true;

	// �����ł���
	if (isDecAlv == true)
	{
		fAlv -= 1.0f / 120.0f;
	}
	else
	{
		fAlv += 1.0f / 120.0f;
	}

	if (fAlv >= 1.0f)
	{
		isDecAlv = true;
	}
	else if (fAlv <= 0.0f)
	{
		isDecAlv = false;
	}

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// UI�̓_�ŏ���(�g���܂킹����)
//==============================================================================================================
void UpdateUIFlash(int nCnt,float *pAlv,float Maxtime)
{
	VERTEX_2D* pVtx;

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, (*pAlv));

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();

}
//==============================================================================================================
// �e���g���[�ɓG���o������UI�̐ݒ�
//=============================================================================================================
void SetTerritoryEnemyUI(int nCnt)
{
	VERTEX_2D* pVtx;

	// �傫���Ȃ����菬�����Ȃ����肷��A�j���[�V����
	UIScalAnimation(nCnt, 200.0f, 150.0f, 80.0f, 40.0f, 40.0f, 40.0f);

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();
}
//==============================================================================================================
// �g��k���̃A�j���[�V����
//==============================================================================================================
void UIScalAnimation(int nCnt, float widthEx, float widthdec, float heightEx, float heigthDec,float MaxtimeWidth, float MaxtimeHeight)
{
	// �����̊g��
	static bool isWidthEasing = true;

	// �����̊g��
	static bool isHeightEasing = true;

	// �����̊g�嗦
	static float Widthscal = widthEx / MaxtimeWidth;

	// �����̊g�嗦
	static float Heightscal = heightEx / MaxtimeHeight;

	// �傫�����ړI�̑傫���ɂȂ�����
	if (g_GameUI[nCnt].fWidth >= widthEx)
	{
		// �k��
		isWidthEasing = false;
		isHeightEasing = false;
	}
	// ���������ړI�̏������ɂȂ�����
	else if (g_GameUI[nCnt].fWidth <= widthdec)
	{
		// �g��
		isWidthEasing = true;
		isHeightEasing = true;

	}

	//if (g_GameUI[nCnt].fHeight >= heightEx)
	//{
	//	
	//}
	//else if (g_GameUI[nCnt].fHeight <= heigthDec)
	//{
	//}

	// �����̊g�嗦
	Widthscal = isWidthEasing ? (widthEx / MaxtimeWidth) : (-widthdec / MaxtimeWidth);
	Heightscal = isHeightEasing ? (heightEx / MaxtimeHeight) : (-heigthDec / MaxtimeHeight);

	// �ړI�̒l�ɋ߂Â���
	g_GameUI[nCnt].fWidth += Widthscal;

	// �ړI�̒l�ɋ߂Â���
	g_GameUI[nCnt].fHeight += Heightscal;
}
//==============================================================================================================
// �N���t�g�̎��Ԓ���UI
//==============================================================================================================
void UpdateCraftTimeUI(int nCnt)
{
	VERTEX_2D* pVtx;

	Player* pPlayer = GetPlayer();

	// �N���t�g�̎��Ԃ��I�������
	if (GetFirstCraftTIme() == false)
	{
		g_GameUI[nCnt].bUse = false;
	}

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// �ړI�̒l�ɋ߂Â���
	g_GameUI[nCnt].pos.x += SetSmoothAprroach(640.0f, g_GameUI[nCnt].pos.x, 0.1f);	

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();

}
//==============================================================================================================
// �N���t�g���j���[��UI�̐ݒ�
//==============================================================================================================
void UpdateCraftTimeMenuUI(int nCnt)
{
	VERTEX_2D* pVtx;

	static float fAlv = 0.0f;

	//���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCnt;

	// �O�b
	fAlv += 1.0f / 120.0f;

	// �J�E���^�[�p�ϐ�
	static int nCounter = 0;

	// 1.0f�ɐ�������
	if (fAlv >= 1.0f)
	{
		fAlv = 1.0f;

		// 2�b��������
		if (nCounter >= 120)
		{
			// �ړI�̈ʒu�ɋ߂Â���
			g_GameUI[nCnt].pos.x += SetSmoothAprroach(1050.0f, g_GameUI[nCnt].pos.x, 0.1f);
			g_GameUI[nCnt].pos.y += SetSmoothAprroach(500.0f, g_GameUI[nCnt].pos.y, 0.1f);
		}
		else
		{
			// �C���N�������g
			nCounter++;
		}
	}

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlv);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_���b�N����
	g_pVtxBuffGameUI->Unlock();

	// �ŏ��̃N���t�g�̎��Ԃ��I�������
	if (GetFirstCraftTIme() == false)
	{
		// ���Z�b�g����
		fAlv = 0.0f;
		nCounter = 0;

		// ���g�p�ɂ���
		g_GameUI[nCnt].bUse = false;
	}
}
