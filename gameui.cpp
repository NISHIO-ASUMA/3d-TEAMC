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

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************

//**************************************************************************************************************
//�v���g�^�C�v�錾
//**************************************************************************************************************
void UIFlash(int nType);	// �_�ŏ���
float fcolorA;

//**************************************************************************************************************
//�O���[�o���ϐ�
//**************************************************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureGameUI[UITYPE_MAX] = {}; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUI = NULL;      // ���_�o�b�t�@�ւ̃|�C���^
Gameui g_GameUI[UITYPE_MAX];
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * UITYPE_MAX,
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

		if (g_GameUI[nCnt].nType == UITYPE_SYUTYUSEN)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 0.5f);
		}
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

	// ���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (!g_GameUI[nCnt].bUse)
		{
			continue;
		}

		switch (g_GameUI[nCnt].nType)
		{
		case UITYPE_TITLE:

			if (g_GameUI[nCnt].pos.y <= 140.0f)
			{
				g_GameUI[nCnt].pos.y += 5.0f; // ���Ɉړ�
			}

			break;
		case UITYPE_FIVER:
		{
			static bool bUp = false;

			// �㏸���Ă���Ƃ�
			if (bUp)
			{
				g_GameUI[nCnt].pos.y -= 2.0f; // ���ɉ�����

				if (g_GameUI[nCnt].pos.y < 630.0f)
				{
					bUp = false; // ���ɉ�����
				}
			}
			else if (!bUp)
			{
				g_GameUI[nCnt].pos.y += 2.0f;

				if (g_GameUI[nCnt].pos.y > 650.0f)
				{
					bUp = true; // ��ɏグ��
				}
			}

			if (!pPlayer->FeverMode)
			{
				g_GameUI[nCnt].bUse = false;
			}

			break;
		}
		case UITYPE_SYUTYUSEN:
			g_nCounterAnim++;

			if (g_nCounterAnim > 2)
			{
				g_nCounterAnim = 0;

				g_nPatternAnim++;//�p�^�[���i���o�[���X�V

			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f);

			//���_���W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_nPatternAnim * 0.5f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f + g_nPatternAnim * 0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + g_nPatternAnim * 0.5f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f + g_nPatternAnim * 0.5f, 1.0f);

			if (g_nPatternAnim > 2)
			{
				g_nPatternAnim = 0;
			}
		
			if (!pPlayer->FeverMode)
			{
				g_GameUI[nCnt].bUse = false;
			}

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
		default:
			break;
		}

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y - g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x - g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_GameUI[nCnt].pos.x + g_GameUI[nCnt].fWidth, g_GameUI[nCnt].pos.y + g_GameUI[nCnt].fHeight, 0.0f);

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
		if (g_GameUI[nCnt].bUse)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureGameUI[g_GameUI[nCnt].nType]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2); // �v���~�e�B�u�̎��
		}
	}
}
//==============================================================================================================
// UI�̐ݒ菈��
//==============================================================================================================
void SetGameUI(D3DXVECTOR3 pos, int nType, float fWidth, float fHeight, int nUseTime)
{
	// ���_���̃|�C���^
	VERTEX_2D* pVtx;

	// ���_���b�N
	g_pVtxBuffGameUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < UITYPE_MAX; nCnt++)
	{
		if (!g_GameUI[nCnt].bUse)
		{
			g_GameUI[nCnt].pos = pos;
			g_GameUI[nCnt].nType = nType;
			g_GameUI[nCnt].fWidth = fWidth;
			g_GameUI[nCnt].fHeight = fHeight;
			g_GameUI[nCnt].nUseTime = nUseTime;
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
