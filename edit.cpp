//============================
//
// edit [edit.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//***********************
//�C���N���[�h�t�@�C��
//***********************
#include "edit.h"
#include "input.h"
#include "block.h"
#include "meshfield.h"
#include "camera.h"
#include "light.h"
#include <stdbool.h>
#include <stdio.h>
#include "fade.h"
#include "game.h"
#include "mouse.h"

//****************************
//�}�N����`
//****************************
#define OBJ_MOVE (10.0f) // �I�u�W�F�N�g�̈ړ���
#define MAX_WORD (128) // ������

//****************************
//�v���g�^�C�v�錾
//****************************
void LoadEditObj(int category); // �ҏW���[�h�Ŏg���I�u�W�F�N�g�̃��[�h����

//****************************
//�O���[�o���ϐ�
//****************************
EDIT_INFO g_Edit[MAX_BLOCK];			// �G�f�B�b�g�̏��
EditTex g_BlockTexInfo[EDITMODE_MAX];	// �G�f�B�b�g�̏��ۑ��p�ϐ�
int g_EditCount, nCntobj;               // �I�u�W�F�N�g�̃J�E���g�A�ۑ��p
int g_nNumBlock;						// �I�u�W�F�N�g�̐�

//===========================
//�G�f�B�b�g��ʂ̏���������
//===========================
void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�u���b�N�̍ő吔��������
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_Edit[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu�̏�����
		g_Edit[nCnt].Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �傫���̏�����(�ŏ�1.0f)
		g_Edit[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ړ��ʂ̏�����
		g_Edit[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ��]�ʂ̏�����
		g_Edit[nCnt].bUse = false;							// ���g�p���
		g_Edit[nCnt].ObjCnt = 0;							// �I�u�W�F�̐��̕ۑ��p�ϐ��̏�����
		g_Edit[nCnt].EditCategory = EDITMODE_BLOCK;				// ���݂̃I�u�W�F�N�g�̃J�e�S���[�̏�����
		g_Edit[nCnt].fMove = 10.0f;							// �I�u�W�F�N�g�̈ړ���
	}

	// �J�e�S���[����
	for (int nCntNum = 0; nCntNum < EDITMODE_MAX; nCntNum++)
	{
		LoadEditObj(nCntNum); // �J�e�S���[�����[�h

		// �J�e�S���[�̎�ޕ���
		for (int nCntModel = 0; nCntModel < g_BlockTexInfo[nCntNum].nNumModel; nCntModel++)
		{
			D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pBuffMatEdit->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_BlockTexInfo[nCntNum].pModel[nCntModel].g_dwNumMatEdit; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
						//�e�N�X�`���̓ǂݍ���
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&g_BlockTexInfo[nCntNum].pModel[nCntModel].g_apTextureEdit[nCntMat]);
				}
			}
		}
	}

	g_nNumBlock = 0;												// �I�u�W�F�N�g�̐��̏�����
	g_Edit[0].bUse = true;											// ��ڂ��g�p��Ԃɂ���
	g_Edit[0].Category[0].nNumModel = g_BlockTexInfo[0].nNumModel;  // 0�Ԃ̃I�u�W�F�N�g�Ɏ�ނ̐�����(������Ȃ��ƕ`�悳��Ȃ�)
	g_Edit[0].Category[0].pModel[0] = g_BlockTexInfo[0].pModel[0];  // 0�Ԃ̃J�e�S���[0�Ԗڂ�0��ޖڂ̏�����
	g_EditCount = 0;												// �I�u�W�F�N�g�̃J�E���g�̏�����
	nCntobj = 0;													// �I�u�W�F�N�g�̃J�E���g�ۑ��p�ϐ��̏�����
}

//===========================
//�G�f�B�b�g��ʂ̏I������
//===========================
void UninitEdit(void)
{
	//�J�e�S���[����
	for (int nCntNum = 0; nCntNum < EDITMODE_MAX; nCntNum++)
	{
		//�e�J�e�S���[�̎�ސ�����
		for (int nCntModel = 0; nCntModel < g_BlockTexInfo[nCntNum].nNumModel; nCntModel++)
		{
			//�e�N�X�`������
			for (int TexCnt = 0; TexCnt < MAX_TEX; TexCnt++)
			{
				//�e�N�X�`���̔j��
				if (g_BlockTexInfo[nCntNum].pModel[nCntModel].g_apTextureEdit[TexCnt] != NULL)
				{
					g_BlockTexInfo[nCntNum].pModel[nCntModel].g_apTextureEdit[TexCnt]->Release();
					g_BlockTexInfo[nCntNum].pModel[nCntModel].g_apTextureEdit[TexCnt] = NULL;
				}
			}

			//�o�b�t�@�̔j��
			if (g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pBuffMatEdit != NULL)
			{
				g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pBuffMatEdit->Release();
				g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pBuffMatEdit = NULL;
			}

			//���b�V���̔j��
			if (g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pMeshEdit != NULL)
			{
				g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pMeshEdit->Release();
				g_BlockTexInfo[nCntNum].pModel[nCntModel].g_pMeshEdit = NULL;
			}
		}
	}

}

//===========================
//�G�f�B�b�g��ʂ̍X�V����
//===========================
void UpdateEdit(void)
{
	static int nLoad = 0; // �����[�h�񐔐����p�ϐ��̏�����

	Camera* pCamera = GetCamera(); // �J�����̃|�C���^���擾

	//�g�p��Ԃ�������
	if (g_Edit[g_EditCount].bUse)
	{
		//�ړ��ʂ̌���
		g_Edit[g_EditCount].move.x += (0.0f - g_Edit[g_EditCount].move.x) * 0.5f;
		g_Edit[g_EditCount].move.z += (0.0f - g_Edit[g_EditCount].move.z) * 0.5f;

		//�ʒu�̍X�V
		g_Edit[g_EditCount].pos += g_Edit[g_EditCount].move;

		//��ړ�
		if (GetKeyboardPress(DIK_W))
		{
			g_Edit[g_EditCount].move.x += sinf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z += cosf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}
		//���ړ�
		if (GetKeyboardPress(DIK_A))
		{
			g_Edit[g_EditCount].move.x -= cosf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z -= sinf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}
		//���ړ�
		if (GetKeyboardPress(DIK_S))
		{
			g_Edit[g_EditCount].move.x -= sinf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z -= cosf(pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}
		//�E�ړ�
		if (GetKeyboardPress(DIK_D))
		{
			g_Edit[g_EditCount].move.x += cosf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
			g_Edit[g_EditCount].move.z += sinf(-pCamera->rot.y) * g_Edit[g_EditCount].fMove;
		}

		int category = g_Edit[g_EditCount].EditCategory; // �^�C�v�ۑ��p(Old)

		//�z�u
		if (KeyboardTrigger(DIK_RETURN))
		{
			g_Edit[g_EditCount + 1].pos = g_Edit[g_EditCount].pos;							// ���̃I�u�W�F�N�g�Ɍ��݂̃I�u�W�F�N�g�̈ʒu����
			g_Edit[g_EditCount + 1].bUse = true;											// ���̃I�u�W�F�N�g���g�p��Ԃɂ���
			g_Edit[g_EditCount + 1].Category[g_Edit[g_EditCount +1].EditCategory].pModel[g_Edit[g_EditCount + 1].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];	// ���̃I�u�W�F�N�g�ɃJ�e�S���[0�A���0�Ԃ̏�����
			g_Edit[g_EditCount + 1].Category[g_Edit[g_EditCount + 1].EditCategory].nNumModel = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].nNumModel;	// ���̃I�u�W�F�N�g�ɃJ�e�S���[0�A�̎�ސ����� 
			g_nNumBlock++;																	// �I�u�W�F�N�g�� + 1
			g_EditCount++;																	// �I�u�W�F�N�g�̃J�E���g + 1
		}

		//�u���b�N����
		if (KeyboardTrigger(DIK_BACKSPACE) && g_EditCount > 0)
		{
			g_Edit[g_EditCount].bUse = false; // �I�𒆂̃I�u�W�F�N�g������
			g_EditCount--;                    // �I�u�W�F�N�g�̃J�E���g�����炷
			g_nNumBlock--;                    // �I�u�W�F�N�g�������炷
		}

		if (KeyboardTrigger(DIK_F)|| KeyboardTrigger(DIK_G))
		{
			g_Edit[g_EditCount].Category[category].nNumModel = g_BlockTexInfo[category].nNumModel;	// ���̃I�u�W�F�N�g�ɃJ�e�S���[0�A�̎�ސ����� 
		}

		//��ޕύX
		if (KeyboardTrigger(DIK_F) && g_Edit[g_EditCount].nType < g_Edit[g_EditCount].Category[category].nNumModel - 1)
		{
			g_Edit[g_EditCount].nType++; // ��� + 1
			g_Edit[g_EditCount].Category[category].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[category].pModel[g_Edit[g_EditCount].nType]; // ���݂̃I�u�W�F�N�g�̏�����
		}
		else if (KeyboardTrigger(DIK_G) && g_Edit[g_EditCount].nType > 0)
		{
			g_Edit[g_EditCount].nType--; // ��� - 1
			g_Edit[g_EditCount].Category[category].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[category].pModel[g_Edit[g_EditCount].nType]; // ���݂̃I�u�W�F�N�g�̏�����
		}

		if (KeyboardTrigger(DIK_V))
		{
			g_Edit[g_EditCount].Scal *= 1.1f; // �g��
		}
		if (KeyboardTrigger(DIK_B))
		{
			g_Edit[g_EditCount].Scal *= 0.9f; // �k��
		}

		// �J�e�S���[�ύX
		if (KeyboardTrigger(DIK_Y) && g_Edit[g_EditCount].EditCategory < EDITMODE_MAX - 1)
		{
			g_Edit[g_EditCount].EditCategory++;																											// �J�e�S���[ + 1
			g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].nNumModel;		// ���݂̃J�e�S���[�̏�����

			// ���̃J�e�S���[�̑S��ސ������݂̃��f���̃J�e�S���[�S��ސ���菭�Ȃ�������
			if (g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel < g_Edit[g_EditCount].Category[category].nNumModel)
			{
				int nNumModel = g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel - 1; // ���
				g_Edit[g_EditCount].nType = nNumModel; //��ԍŌ�̎�ނɂ���
			}

			g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];		// ���݂̃J�e�S���[�̏�����
		}
		else if (KeyboardTrigger(DIK_U) && g_Edit[g_EditCount].EditCategory > 0)
		{
			g_Edit[g_EditCount].EditCategory--;	// �J�e�S���[ + 1

			g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].nNumModel;		// ���݂̃J�e�S���[�̏�����

			// ���̃J�e�S���[�̑S��ސ������݂̃��f���̃J�e�S���[�S��ސ���菭�Ȃ�������
			if (g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel < g_Edit[g_EditCount].Category[category].nNumModel)
			{
				int nNumModel = g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].nNumModel - 1; // ���
				g_Edit[g_EditCount].nType = nNumModel; //��ԍŌ�̎�ނɂ���
			}

			g_Edit[g_EditCount].Category[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType] = g_BlockTexInfo[g_Edit[g_EditCount].EditCategory].pModel[g_Edit[g_EditCount].nType];		// ���݂̃J�e�S���[�̏�����
		}

		if (GetKeyboardPress(DIK_H))
		{
			g_Edit[g_EditCount].fMove += 0.1f; // �ړ��ʕύX
		}
		else if (GetKeyboardPress(DIK_J) && g_Edit[g_EditCount].fMove > 0.0f)
		{
			g_Edit[g_EditCount].fMove -= 0.1f; // �ړ��ʕύX
		}

		if (GetKeyboardPress(DIK_UP))
		{
			g_Edit[g_EditCount].pos.y += g_Edit[g_EditCount].fMove;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			g_Edit[g_EditCount].pos.y -= g_Edit[g_EditCount].fMove;
		}
	}

	if (KeyboardTrigger(DIK_F7))
	{
		SaveEdit(); // �����o��
	}

	else if (KeyboardTrigger(DIK_F8) && nLoad == 0)
	{
		nLoad = 1; // �񐔐���
		ReLoadEdit(); // �����[�h
	}

	if (KeyboardTrigger(DIK_F2) && nLoad == 1)
	{
		nLoad = 0; // �񐔐�������
	}
}

//===========================
//�G�f�B�b�g��ʂ̕`�揈��
//===========================
void DrawEdit(void)
{
	MODE mode = GetMode();
	EDIT_INFO* pEdit = GetEdit();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();;

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (!g_Edit[nCntBlock].bUse)
		{//���g�p��������
			//�������΂�
			continue;
		}

		int nType = g_Edit[nCntBlock].nType;
		int EditCategory = g_Edit[nCntBlock].EditCategory;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Edit[nCntBlock].mtxWorld);

		//�����𔽉f
		D3DXMatrixScaling(&mtxScal, g_Edit[nCntBlock].Scal.y, g_Edit[nCntBlock].Scal.x, g_Edit[nCntBlock].Scal.z);
		D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxScal);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Edit[nCntBlock].rot.y, g_Edit[nCntBlock].rot.x, g_Edit[nCntBlock].rot.z);
		D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Edit[nCntBlock].pos.x, g_Edit[nCntBlock].pos.y, g_Edit[nCntBlock].pos.z);
		D3DXMatrixMultiply(&g_Edit[nCntBlock].mtxWorld, &g_Edit[nCntBlock].mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Edit[nCntBlock].mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCntMat = 0; nCntMat < (int)g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_dwNumMatEdit; nCntMat++)
		{
			//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_pBuffMatEdit->GetBufferPointer();

			//�I�𒆂̃u���b�N��������
			if (nCntBlock == g_EditCount)
			{
				D3DXMATERIAL color;

				color = pMat[nCntMat];

				color.MatD3D.Diffuse.r = 1.0f;
				color.MatD3D.Diffuse.g = 1.0f;
				color.MatD3D.Diffuse.b = 1.0f;
				color.MatD3D.Diffuse.a = 0.5f;

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&color.MatD3D);
			}
			else
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_apTextureEdit[nCntMat]);

			//�u���b�N(�p�[�c)�̕`��
			g_Edit[nCntBlock].Category[EditCategory].pModel[nType].g_pMeshEdit->DrawSubset(nCntMat);
		}
	}

	pDevice->SetMaterial(&matDef);
}

//===========================
//�G�f�B�b�g��ʂ̃Z�[�u����
//===========================
void SaveEdit(void)
{
	FILE* pFile;

	pFile = fopen("data\\save.txt", "w");

	if (pFile != NULL)
	{
		fwrite(&g_EditCount, sizeof(int), 0, pFile);

		fprintf(pFile,

			"+==========================================+\n"
			"+			   �}�b�v�G�f�B�^�[             +\n"
			"+==========================================+\n\n");

		for (int nCnt = 0; nCnt < g_EditCount; nCnt++)
		{
			if (g_Edit[nCnt].bUse && g_Edit[nCnt].EditCategory == EDITMODE_BLOCK)
			{
				fprintf(pFile, "BLOCKSET\n");

				fprintf(pFile, "EDITCATEGORY = %d\n", g_Edit[nCnt].EditCategory);

				fprintf(pFile, "   BLOCKTYPE = %d\n", g_Edit[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f\n", g_Edit[nCnt].pos.x, g_Edit[nCnt].pos.y, g_Edit[nCnt].pos.z);

				fprintf(pFile, "   SIZE = %.1f %.1f %.1f\n", g_Edit[nCnt].Scal.x, g_Edit[nCnt].Scal.y, g_Edit[nCnt].Scal.z);

				fprintf(pFile, "END_BLOCKSET\n\n");
			}
			else if (g_Edit[nCnt].bUse && g_Edit[nCnt].EditCategory == EDITMODE_ITEM)
			{
				fprintf(pFile, "ITEMSET\n");

				fprintf(pFile, "EDITCATEGORY = %d\n", g_Edit[nCnt].EditCategory);

				fprintf(pFile, "   ITEMTYPE = %d\n", g_Edit[nCnt].nType);

				fprintf(pFile, "   POS = %.1f %.1f %.1f\n", g_Edit[nCnt].pos.x, g_Edit[nCnt].pos.y, g_Edit[nCnt].pos.z);

				fprintf(pFile, "   SIZE = %.1f %.1f %.1f\n", g_Edit[nCnt].Scal.x, g_Edit[nCnt].Scal.y, g_Edit[nCnt].Scal.z);

				fprintf(pFile, "END_ITEMSET\n\n");
			}
		}

		fprintf(pFile, "BLOCK_COUNT = %d \n", g_EditCount);
		fprintf(pFile, "END_SCRIPT");
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Edit.cpp)", MB_OK);
		return;
	}
	fclose(pFile);
}

//===========================
//�G�f�B�b�g��ʂ̃��[�h����
//===========================
void LoadEdit(void)
{
	FILE* pFile;
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	float fWidth;
	float fHeight;
	char skip[5];
	fWidth = 0.0f;
	fHeight = 0.0f;

	nType = 0;

	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);

	//switch (g_LoadCnt)
	//{
	//case 0:
	//	//�t�@�C�����J��
	//	pFile = fopen("data\\save.txt,","r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	pFile = fopen("data\\save.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0 || strcmp(aString, "ITEMSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "ITEMTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &Scal.x);
						fscanf(pFile, "%f", &Scal.y);
						fscanf(pFile, "%f", &Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{
						SetBlock(pos, nType, Scal);
						break;
					}
					else if (strcmp(aString, "END_ITEMSET") == 0)
					{
						SetItem(pos, nType, Scal);
						break;
					}
				}
			}

			if (strcmp(aString, "BLOCK_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nCntobj);
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
		/*g_Edit++;*/

	}
	else
	{//�J���Ȃ������Ƃ�	
				//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Edit.cpp)", MB_OK);
		return;
	}
	fclose(pFile);
}
//===========================
//�G�f�B�b�g�̃����[�h
//===========================
void ReLoadEdit(void)
{
	FILE* pFile;
	int nType;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;

	int nIdx = 0;
	float fWidth;
	float fHeight;
	char skip[5];
	fWidth = 0.0f;
	fHeight = 0.0f;

	nType = 0;

	move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Scal(0.0f, 0.0f, 0.0f);

	//switch (g_LoadCnt)
	//{
	//case 0:
	//	//�t�@�C�����J��
	//	pFile = fopen("data\\save.txt,","r");
	//	break;
	//default:
	//	pFile = NULL;
	//	break;
	//}

	pFile = fopen("data\\save.txt", "r");

	if (pFile != NULL)
	{
		while (1)
		{
			char aString[MAX_WORD];

			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "BLOCKSET") == 0 || strcmp(aString, "ITEMSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(aString, "BLOCKTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].nType);
					}
					else if (strcmp(aString, "ITEMTYPE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].nType);
					}
					else if (strcmp(aString, "EDITCATEGORY") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%d", &g_Edit[nIdx].EditCategory);
					}
					else if (strcmp(aString, "POS") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].pos.z);
					}
					else if (strcmp(aString, "SIZE") == 0)
					{
						fscanf(pFile, "%s", &skip[0]);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.x);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.y);
						fscanf(pFile, "%f", &g_Edit[nIdx].Scal.z);
					}
					else if (strcmp(aString, "END_BLOCKSET") == 0)
					{
						nIdx++;
						break;
					}
					else if (strcmp(aString, "END_ITEMSET") == 0)
					{
						nIdx++;
						break;
					}
				}
			}

			if (strcmp(aString, "BLOCK_COUNT") == 0)
			{
				fscanf(pFile, "%s", &skip[0]);
				fscanf(pFile, "%d", &nCntobj);
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
		/*g_Edit++;*/
	}
	else
	{//�J���Ȃ������Ƃ�	
				//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Edit.cpp)", MB_OK);
		return;
	}
	fclose(pFile);

	for (int nCnt = 0; nCnt < nCntobj; nCnt++)
	{
		g_Edit[g_EditCount + 1].bUse = true; // �u����Ă����u���b�N���g�p��Ԃɂ���
		g_Edit[g_EditCount + 1].Category[g_Edit[nCnt].EditCategory].pModel[g_Edit[nCnt].nType] = g_BlockTexInfo[g_Edit[nCnt].EditCategory].pModel[g_Edit[nCnt].nType]; // ������
		g_Edit[g_EditCount + 1].Category[g_Edit[nCnt].EditCategory].nNumModel = g_BlockTexInfo[g_Edit[nCnt].EditCategory].nNumModel; // ������
		g_EditCount++;
		g_nNumBlock++;
	}

}

//===========================
//�G�f�B�b�g�K��
//===========================
EDIT_INFO* GetEdit(void)
{
	return &g_Edit[0];
}
//===========================
//�I�u�W�F�N�g�̎擾
//===========================
int GetNumobj(void)
{
	return g_nNumBlock;
}
//===========================
//�I�u�W�F�N�g�̃��[�h
//===========================
void LoadEditObj(int category)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;
	char Skip[5];
	int nCntModel = 0;

	switch (category)
	{
	case EDITMODE_BLOCK:
		pFile = fopen("data\\MODEL_TXT\\BLOCK.txt", "r");
		break;
	case EDITMODE_ITEM:
		pFile = fopen("data\\MODEL_TXT\\ITEM.txt", "r");
		break;
	default:
		pFile = NULL;
		break;
	}

	if (pFile != NULL)
	{
		char aString[256];

		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "MAX_TYPE") == 0)
			{
				fscanf(pFile, "%s", &Skip[0]);
				fscanf(pFile, "%d", &g_BlockTexInfo[category].nNumModel);
			}
			else if (strcmp(aString, "MODEL_FILENAME") == 0)
			{
				fscanf(pFile, "%s", &Skip[0]);

				fscanf(pFile, "%s", &aString[0]);

				const char* MODEL_FILENAME = {};

				MODEL_FILENAME = aString;

				//X�t�@�C���̓ǂݍ���
				D3DXLoadMeshFromX(MODEL_FILENAME,
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_BlockTexInfo[category].pModel[nCntModel].g_pBuffMatEdit,
					NULL,
					&g_BlockTexInfo[category].pModel[nCntModel].g_dwNumMatEdit,
					&g_BlockTexInfo[category].pModel[nCntModel].g_pMeshEdit);

				nCntModel++;
			}
			else if (strcmp(aString, "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Edit.cpp)", MB_OK);
		return;
	}
	fclose(pFile);
}
