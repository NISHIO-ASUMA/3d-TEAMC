//==============================================================================================================
//
// �p�[�e�B�N��[particle.cpp]
// Author:YOSHIDA YUUTO
//
//==============================================================================================================

//**************************************************************************************************************
//�C���N���[�h�t�@�C��
//**************************************************************************************************************
#include "input.h"
#include "player.h"
#include "camera.h"
#include "effectEdit.h"
#include "meshfield.h"
#include <stdio.h>
#include "particle2.h"
#include "effect2.h"
#include "particle2.h"

//**************************************************************************************************************
//�}�N����`
//**************************************************************************************************************
#define BULLET_WIDTH (25.0f) //����
#define BULLET_HEIGHT (25.0f) //����
#define MAX_EFFECTEDIT (256) //�e�̍ő吔
#define MAX_MOVE (5.0f) //�ړ���

//**************************************************************************************************************
//�O���[�o���ϐ��錾
//**************************************************************************************************************
SETPARTICLE g_EditEffect[MAX_EFFECTEDIT];
int g_nCntEffect;
int g_bEnableLife;

//==============================================================================================================
//�e�̏���������
//==============================================================================================================
void InitEffectEdit(void)
{
	InitParticleX();

	InitEffectX();

	InitCamera();

	InitMeshField();

	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_EFFECTEDIT; nCnt++)
	{
		g_EditEffect[nCnt].pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
		g_EditEffect[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EditEffect[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_EditEffect[nCnt].nLife = 10;
		g_EditEffect[nCnt].nType = 0;
		g_EditEffect[nCnt].fSize = 10.0f;
		g_EditEffect[nCnt].nNumParticle = 10;
		g_EditEffect[nCnt].ColorType = COLORCHANGE_R;
		g_EditEffect[nCnt].AngleMax = 628;
		g_EditEffect[nCnt].AngleMin = -314;
		g_EditEffect[nCnt].fSpeed = 1.0f;
		g_EditEffect[nCnt].decfAlv = 0.02f;
		g_EditEffect[nCnt].dir = D3DXVECTOR3(1.0f,1.0f, 1.0f);
		g_EditEffect[nCnt].bUse = false;
	}
	g_bEnableLife = false;
	g_EditEffect[0].bUse = true;

	// ��ڂ��Z�b�g
	SetParticleX(D3DXVECTOR3(g_EditEffect[g_nCntEffect].pos.x, g_EditEffect[g_nCntEffect].pos.y, g_EditEffect[g_nCntEffect].pos.z),
		D3DXVECTOR3(g_EditEffect[g_nCntEffect].move.x, g_EditEffect[g_nCntEffect].move.y, g_EditEffect[g_nCntEffect].move.z),
		g_EditEffect[g_nCntEffect].nLife,
		g_EditEffect[g_nCntEffect].col,
		0,
		g_EditEffect[g_nCntEffect].fSize,
		g_EditEffect[g_nCntEffect].nNumParticle,
		g_EditEffect[g_nCntEffect].AngleMax,
		g_EditEffect[g_nCntEffect].AngleMin,
		g_EditEffect[g_nCntEffect].fSpeed,
		g_EditEffect[g_nCntEffect].dir,
		g_EditEffect[g_nCntEffect].decfAlv,
		g_EditEffect[g_nCntEffect].EffectTex,
		false);
}
//==============================================================================================================
//�e�̏I������
//==============================================================================================================
void UninitEffectEdit(void)
{
	UninitEffectX();

	UninitMeshField();

}
//==============================================================================================================
//�e�̍X�V����
//==============================================================================================================
void UpdateEffectEdit(void)
{
	UpdateEffectX();

	UpdateCamera();

	// ���q�̐��ύX
	if (GetKeyboardPress(DIK_R) && g_EditEffect[g_nCntEffect].nNumParticle < 30)
	{
		g_EditEffect[g_nCntEffect].nNumParticle++;
	}
	else if (GetKeyboardPress(DIK_T) && g_EditEffect[g_nCntEffect].nNumParticle > 1)
	{
		g_EditEffect[g_nCntEffect].nNumParticle--;
	}

	// ���q�̐��ύX
	if (GetKeyboardPress(DIK_F) && g_EditEffect[g_nCntEffect].fSize < 30.0f)
	{
		g_EditEffect[g_nCntEffect].fSize++;
	}
	else if (GetKeyboardPress(DIK_G) && g_EditEffect[g_nCntEffect].fSize > 2.0f)
	{
		g_EditEffect[g_nCntEffect].fSize--;
	}

	// rgba�ύX
	if (KeyboardTrigger(DIK_V) && g_EditEffect[g_nCntEffect].ColorType < COLORCHANGE_MAX - 1)
	{
		g_EditEffect[g_nCntEffect].ColorType++;
	}
	else if (KeyboardTrigger(DIK_B) && g_EditEffect[g_nCntEffect].ColorType > 0)
	{
		g_EditEffect[g_nCntEffect].ColorType--;
	}

	// �F�ύX���Z
	if (GetKeyboardPress(DIK_UP))
	{
		switch (g_EditEffect[g_nCntEffect].ColorType)
		{
		case COLORCHANGE_R:
			g_EditEffect[g_nCntEffect].col.r += 0.01f;
			break;
		case COLORCHANGE_G:
			g_EditEffect[g_nCntEffect].col.g += 0.01f;
			break;
		case COLORCHANGE_B:
			g_EditEffect[g_nCntEffect].col.b += 0.01f;
			break;
		case COLORCHANGE_A:
			g_EditEffect[g_nCntEffect].col.a += 0.01f;
			break;
		default:
			break;
		}
	}

	// �F�ύX���Z
	else if (GetKeyboardPress(DIK_DOWN))
	{
		switch (g_EditEffect[g_nCntEffect].ColorType)
		{
		case COLORCHANGE_R:
			g_EditEffect[g_nCntEffect].col.r -= 0.01f;
			break;
		case COLORCHANGE_G:
			g_EditEffect[g_nCntEffect].col.g -= 0.01f;
			break;
		case COLORCHANGE_B:
			g_EditEffect[g_nCntEffect].col.b -= 0.01f;
			break;
		case COLORCHANGE_A:
			g_EditEffect[g_nCntEffect].col.a -= 0.01f;
			break;
		default:
			break;
		}
	}

	// �F�̐���
	if (g_EditEffect[g_nCntEffect].col.r > 1.0f)
	{
		g_EditEffect[g_nCntEffect].col.r = 1.0f;
	}
	if (g_EditEffect[g_nCntEffect].col.g > 1.0f)
	{
		g_EditEffect[g_nCntEffect].col.g = 1.0f;
	}
	if (g_EditEffect[g_nCntEffect].col.b > 1.0f)
	{
		g_EditEffect[g_nCntEffect].col.b = 1.0f;
	}
	if (g_EditEffect[g_nCntEffect].col.a > 1.0f)
	{
		g_EditEffect[g_nCntEffect].col.a = 1.0f;
	}
	// �F�̐���
	if (g_EditEffect[g_nCntEffect].col.r < 0.1f)
	{
		g_EditEffect[g_nCntEffect].col.r = 0.1f;
	}
	if (g_EditEffect[g_nCntEffect].col.g < 0.1f)
	{
		g_EditEffect[g_nCntEffect].col.g = 0.1f;
	}
	if (g_EditEffect[g_nCntEffect].col.b < 0.1f)
	{
		g_EditEffect[g_nCntEffect].col.b = 0.1f;
	}
	if (g_EditEffect[g_nCntEffect].col.a < 0.1f)
	{
		g_EditEffect[g_nCntEffect].col.a = 0.1f;
	}

	// �A���O���ύX
	if (GetKeyboardPress(DIK_Y) && g_EditEffect[g_nCntEffect].AngleMax > 1)
	{
		g_EditEffect[g_nCntEffect].AngleMax --;
	}
	else if(GetKeyboardPress(DIK_U) && g_EditEffect[g_nCntEffect].AngleMax < 628)
	{
		g_EditEffect[g_nCntEffect].AngleMax++;
	}

	// �A���O���ύX
	if (GetKeyboardPress(DIK_I) && g_EditEffect[g_nCntEffect].AngleMin > -314)
	{
		g_EditEffect[g_nCntEffect].AngleMin--;
	}
	else if (GetKeyboardPress(DIK_O) && g_EditEffect[g_nCntEffect].AngleMin < 0)
	{
		g_EditEffect[g_nCntEffect].AngleMin++;
	}

	// �ړ��ʂ̕ύX
	if (GetKeyboardPress(DIK_H) && g_EditEffect[g_nCntEffect].fSpeed > -100.0f)
	{
		g_EditEffect[g_nCntEffect].fSpeed -= 0.1f;
	}
	else if (GetKeyboardPress(DIK_J) && g_EditEffect[g_nCntEffect].fSpeed < 100.0f)
	{
		g_EditEffect[g_nCntEffect].fSpeed += 0.1f;
	}

	// �����̕ύX
	if (GetKeyboardPress(DIK_K) && g_EditEffect[g_nCntEffect].nLife < 400)
	{
		g_EditEffect[g_nCntEffect].nLife ++;
	}
	else if (GetKeyboardPress(DIK_L) && g_EditEffect[g_nCntEffect].nLife > 10)
	{
		g_EditEffect[g_nCntEffect].nLife--;
	}

	// �d�͂̕ύX
	// ���Z
	if (GetKeyboardPress(DIK_1))
	{
		g_EditEffect[g_nCntEffect].dir.x += 0.01f;
	}
	else if (GetKeyboardPress(DIK_2))
	{
		g_EditEffect[g_nCntEffect].dir.y += 0.01f;
	}
	else if (GetKeyboardPress(DIK_3))
	{
		g_EditEffect[g_nCntEffect].dir.z += 0.01f;
	}
	// ���Z
	else if (GetKeyboardPress(DIK_4))
	{
		g_EditEffect[g_nCntEffect].dir.x -= 0.01f;
	}
	else if (GetKeyboardPress(DIK_5))
	{
		g_EditEffect[g_nCntEffect].dir.y -= 0.01f;
	}
	else if (GetKeyboardPress(DIK_6))
	{
		g_EditEffect[g_nCntEffect].dir.z -= 0.01f;
	}

	if (GetKeyboardPress(DIK_N) && g_EditEffect[g_nCntEffect].decfAlv < 0.1f)
	{
		g_EditEffect[g_nCntEffect].decfAlv += 0.0001f;
	}
	else if (GetKeyboardPress(DIK_M) && g_EditEffect[g_nCntEffect].decfAlv > 0.0001f)
	{
		g_EditEffect[g_nCntEffect].decfAlv -= 0.0001f;
	}

	if (KeyboardTrigger(DIK_F2) && g_EditEffect[g_nCntEffect].EffectTex < EFFECT_MAX - 1)
	{
		g_EditEffect[g_nCntEffect].EffectTex++;
	}
	else if (KeyboardTrigger(DIK_F3) && g_EditEffect[g_nCntEffect].EffectTex > 0)
	{
		g_EditEffect[g_nCntEffect].EffectTex--;
	}

	if (KeyboardTrigger(DIK_F7))
	{
		SaveEffect();
	}

	// �G�t�F�N�g�̐ݒ�
	SetParticleEdit(g_EditEffect[g_nCntEffect]);

	// �G�t�F�N�g�̍X�V
	UpdateParticleX();

}
//==============================================================================================================
//�e�̕`�揈��
//==============================================================================================================
void DrawEffectEdit(void)
{
	DrawMeshField();

	DrawEffectX();
}
//==============================================================================================================
//�e�̕`�揈��
//==============================================================================================================
void SetEffectEdit(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nLife, float fSize)
{
	for (int nCnt = 0; nCnt < MAX_EFFECTEDIT; nCnt++)
	{
		if (g_EditEffect[nCnt].bUse == false)
		{
			g_EditEffect[nCnt].pos = pos;
			g_EditEffect[nCnt].fSize = fSize;
			g_EditEffect[nCnt].move = move;
			g_EditEffect[nCnt].col = col;
			g_EditEffect[nCnt].nLife = nLife;
			g_EditEffect[nCnt].bUse = true;

			break;
		}
	}
}
//==============================================================================================================
// �p�[�e�B�N���̃Z�[�u����
//==============================================================================================================
void SaveEffect(void)
{
	FILE* pFile; // �t�@�C���̃|�C���^�錾

	pFile = fopen("data\\EFFECT\\particleinfo.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_nCntEffect, sizeof(int), 0, pFile);

		fprintf(pFile, "+============================================+\n");
		fprintf(pFile, "+   �p�[�e�B�N���G�f�B�^�[ �쐬�� �g�c�S�l   +\n");
		fprintf(pFile, "+============================================+\n");

		if (g_EditEffect[0].bUse)
		{
			fprintf(pFile, "PARTICLESET\n");

			fprintf(pFile, "    POS = %3.2f %3.2f %3.2f					# �ʒu\n",g_EditEffect[0].pos.x, g_EditEffect[0].pos.y, g_EditEffect[0].pos.z);

			fprintf(pFile, "    MOVE = %3.2f %3.2f %3.2f					# �ړ���\n", g_EditEffect[0].move.x, g_EditEffect[0].move.y, g_EditEffect[0].move.z);

			fprintf(pFile, "    LIFE = %d								# ����\n", g_EditEffect[0].nLife);
								    
			fprintf(pFile, "    COLOR = %3.2f %3.2f %3.2f %3.2f				# �F\n", g_EditEffect[0].col.r, g_EditEffect[0].col.g,g_EditEffect[0].col.b,g_EditEffect[0].col.a);
								    
			fprintf(pFile, "    TYPE = %d								# �p�[�e�B�N���̎��\n", g_EditEffect[0].nType);
								    
			fprintf(pFile, "    SIZE = %3.2f						# �p�[�e�B�N���̑傫��\n", g_EditEffect[0].fSize);
								    
			fprintf(pFile, "    NUM_PARTICLE = %d						# �p�[�e�B�N���̗��q�̐�\n", g_EditEffect[0].nNumParticle);
								    
			fprintf(pFile, "    ANGLE_MAX = %d							# �p�[�e�B�N���̊p�x��MAX\n", g_EditEffect[0].AngleMax);
								    
			fprintf(pFile, "    ANGLE_MIN = %d							# �p�[�e�B�N���̊p�x��MIN\n", g_EditEffect[0].AngleMin);
								    
			fprintf(pFile, "    SPEED = %3.2f							# �p�[�e�B�N���̑���\n", g_EditEffect[0].fSpeed);
								    
			fprintf(pFile, "    DIR = %3.2f %3.2f %3.2f					# �p�[�e�B�N���̏d��XYZ\n", g_EditEffect[0].dir.x, g_EditEffect[0].dir.y, g_EditEffect[0].dir.z);
								    
			fprintf(pFile, "    DECREMENT = %f							# �p�[�e�B�N���̃A���t�@�l�̌����̑���\n", g_EditEffect[0].decfAlv);
								    
			fprintf(pFile, "    EFFECT_TEXTPE = %d						# �p�[�e�B�N���̃e�N�X�`���̎��\n", g_EditEffect[0].EffectTex);
								    
			fprintf(pFile, "    BLIFE = %d								# �p�[�e�B�N���̎��������邩���Ȃ���\n", g_EditEffect[0].bLifedecrease);

			fprintf(pFile, " END_PARTICLESET\n\n");
		}
		

		fprintf(pFile, " END_SCRIPT\n");
	}
	else
	{
		return;
	}
	fclose(pFile);

}
//==============================================================================================================
//�G�t�F�N�g�̃��[�h
//==============================================================================================================
void LoadEffect(int nType,D3DXVECTOR3 pos)
{
	FILE* pFile; // �t�@�C���̃|�C���^�錾

	char skip[3];
	//D3DXVECTOR3 pos(0.0f,0.0f,0.0f);
	D3DXVECTOR3 move(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);

	int nNumParticle = 0;
	int AngleMax = 0;
	int AngleMin = 0;
	int EffectType = 0;
	int EffectTex = 0;
	int nLife = 0;

	D3DXCOLOR col(0.0f, 0.0f, 0.0f, 0.0f);

	float fSize = 0.0f;
	float fSpeed = 0.0f;
	float Dec = 0.0f;

	bool EnableLife = false;
	int bLife = 1;

	switch (nType)
	{
	case 0:
		pFile = fopen("data\\EFFECT\\effect000.txt", "r");
		break;
	default:
		pFile = NULL;
		break;
	}
	if (pFile != NULL)
	{
		char aString[1000];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "PARTICLESET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "MOVE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &move.x);
						fscanf(pFile, "%f", &move.y);
						fscanf(pFile, "%f", &move.z);
					}
					else if (strcmp(aString, "LIFE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nLife);
					}
					else if (strcmp(aString, "COLOR") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &col.r);
						fscanf(pFile, "%f", &col.g);
						fscanf(pFile, "%f", &col.b);
						fscanf(pFile, "%f", &col.a);
					}
					else if (strcmp(aString, "TYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &EffectType);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &fSize);
					}
					else if (strcmp(aString, "NUM_PARTICLE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nNumParticle);
					}
					else if (strcmp(aString, "ANGLE_MAX") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &AngleMax);
					}
					else if (strcmp(aString, "ANGLE_MIN") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &AngleMax);
					}
					else if (strcmp(aString, "SPEED") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &fSpeed);
					}
					else if (strcmp(aString, "DIR") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &dir.x);
						fscanf(pFile, "%f", &dir.y);
						fscanf(pFile, "%f", &dir.z);
					}

					else if (strcmp(aString, "DECREMENT") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &Dec);
					}
					else if (strcmp(aString, "EFFECT_TEXTPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &EffectTex);
					}
					else if (strcmp(aString, "BLIFE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &bLife);

						if (bLife == 0)
						{
							EnableLife = true;
						}
						else if (bLife == 1)
						{
							EnableLife = false;
						}
					}
					else if (strcmp(aString, "END_PARTICLESET") == 0)
					{
						SetParticleX(pos, move, nLife, col, EffectType, fSize, nNumParticle, AngleMax, AngleMin, fSpeed, dir, Dec, EffectTex, EnableLife);
						break;
					}
				}
			}

			if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	else
	{
		return;
	}
	fclose(pFile);
}

//==============================================================================================================
//�G�t�F�N�g�ҏW�̎擾
//==============================================================================================================
SETPARTICLE* GetEditEffect(void)
{
	return &g_EditEffect[0];
}
