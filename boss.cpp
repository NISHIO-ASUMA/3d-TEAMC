//============================
//
// �{�X [boss.cpp]
// Author:YOSHIDA YUUTO
//
//============================

//****************************
// �C���N���[�h�t�@�C��
//****************************
#include "boss.h"
#include "input.h"
#include "mouse.h"
#include "camera.h"

//****************************
// �}�N����`
//****************************
#define MAX_WORD (256) // �ő�̕�����

//****************************
// �v���g�^�C�v�錾
//****************************
void LoadBoss(void);

//****************************
// �O���[�o���ϐ��錾
//****************************
Boss g_Boss; // �{�X�̏��

//=============================
// �{�X�̏���������
//=============================
void InitBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	g_Boss.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W
	g_Boss.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �ړ���
	g_Boss.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �p�x
	g_Boss.bUse = false;						// ���g�p���
	g_Boss.nLife = 20;							// �̗�
	g_Boss.state = BOSSSTATE_NORMAL;			// ���

	LoadBoss(); // �{�X�̃��[�h

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Boss.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				//���̃t�@�C�������g�p���ăe�N�X�`����ǂݍ���
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_Boss.Motion.aModel[nCntModel].pTexture[nCntMat]);
			}
		}
	}

	//���_���W�̎擾
	int nNumVtx;//���_��
	DWORD sizeFVF;//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		//���_���̎擾
		nNumVtx = g_Boss.Motion.aModel[nCntModel].pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y�擾
		sizeFVF = D3DXGetFVFVertexSize(g_Boss.Motion.aModel[nCntModel].pMesh->GetFVF());

		//���_�o�b�t�@�̃��b�N
		g_Boss.Motion.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ă{�X�̍ŏ��l,�ő�l���擾
			if (vtx.x < g_Boss.Motion.aModel[nCntModel].vtxMin.x)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.x = vtx.x;
			}
			else if (vtx.y < g_Boss.Motion.aModel[nCntModel].vtxMin.y)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.y = vtx.y;
			}
			else if (vtx.z < g_Boss.Motion.aModel[nCntModel].vtxMin.z)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMin.z = vtx.z;
			}
			else if (vtx.x > g_Boss.Motion.aModel[nCntModel].vtxMax.x)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.x = vtx.x;
			}
			else if (vtx.y > g_Boss.Motion.aModel[nCntModel].vtxMax.y)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.y = vtx.y;
			}
			else if (vtx.z > g_Boss.Motion.aModel[nCntModel].vtxMax.z)
			{
				g_Boss.Motion.aModel[nCntModel].vtxMax.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;

			//�T�C�Y�ɑ��
			g_Boss.Motion.aModel[nCntModel].Size.x = g_Boss.Motion.aModel[nCntModel].vtxMax.x - g_Boss.Motion.aModel[nCntModel].vtxMin.x;
			g_Boss.Motion.aModel[nCntModel].Size.y = g_Boss.Motion.aModel[nCntModel].vtxMax.y - g_Boss.Motion.aModel[nCntModel].vtxMin.y;
			g_Boss.Motion.aModel[nCntModel].Size.z = g_Boss.Motion.aModel[nCntModel].vtxMax.z - g_Boss.Motion.aModel[nCntModel].vtxMin.z;
		}

		//���_�o�b�t�@�̃A�����b�N
		g_Boss.Motion.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}
}
//=============================
// �{�X�̏I������
//=============================
void UninitBoss(void)
{
	for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
	{
		//�e�N�X�`���̔j��
		for (int nCntTex = 0; nCntTex < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntTex++)
		{
			if (g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex]->Release();
				g_Boss.Motion.aModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_Boss.Motion.aModel[nCntModel].pMesh != NULL)
		{
			g_Boss.Motion.aModel[nCntModel].pMesh->Release();
			g_Boss.Motion.aModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_Boss.Motion.aModel[nCntModel].pBuffMat != NULL)
		{
			g_Boss.Motion.aModel[nCntModel].pBuffMat->Release();
			g_Boss.Motion.aModel[nCntModel].pBuffMat = NULL;
		}
	}
}
//=============================
// �{�X�̍X�V����
//=============================
void UpdateBoss(void)
{
	UpdateMotion(&g_Boss.Motion);
}
//=============================
// �{�X�̕`�揈��
//=============================
void DrawBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�v�Z�p�̃}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxSize;

	D3DMATERIAL9 matDef;//���݂̃}�e���A���ۑ��p

	D3DXMATERIAL* pMat;//�}�e���A���f�[�^�ւ̃|�C���^

	if (g_Boss.bUse == true)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Boss.mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Boss.rot.y, g_Boss.rot.x, g_Boss.rot.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Boss.pos.x, g_Boss.pos.y, g_Boss.pos.z);
		D3DXMatrixMultiply(&g_Boss.mtxWorld, &g_Boss.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Boss.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�S���f���p�[�c�̕`��
		for (int nCntModel = 0; nCntModel < g_Boss.Motion.nNumModel; nCntModel++)
		{
			D3DXMATRIX mtxRotModel, mtxTransform;//�v�Z�p
			D3DXMATRIX mtxParent;//�e�̃}�g���b�N�X

			//�p�[�c�̃}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Boss.Motion.aModel[nCntModel].mtxWorld);

			//�p�[�c�̌����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Boss.Motion.aModel[nCntModel].rot.y, g_Boss.Motion.aModel[nCntModel].rot.x, g_Boss.Motion.aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld, &g_Boss.Motion.aModel[nCntModel].mtxWorld, &mtxRotModel);

			//�p�[�c�̈ʒu(�I�t�Z�b�g)�𔽉f
			D3DXMatrixTranslation(&mtxTransform, g_Boss.Motion.aModel[nCntModel].pos.x, g_Boss.Motion.aModel[nCntModel].pos.y, g_Boss.Motion.aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld, &g_Boss.Motion.aModel[nCntModel].mtxWorld, &mtxTransform);

			//�p�[�c��[�e�̃}�g���b�N�X]��ݒ�
			if (g_Boss.Motion.aModel[nCntModel].nIdxModelParent != -1)
			{
				//�e���f��������ꍇ
				mtxParent = g_Boss.Motion.aModel[g_Boss.Motion.aModel[nCntModel].nIdxModelParent].mtxWorld;
			}
			else
			{//�e���f�����Ȃ��ꍇ
				mtxParent = g_Boss.mtxWorld;
			}

			//�Z�o����[�p�[�c�̃��[���h�}�g���b�N�X]��[�e�̃}�g���b�N�X]���������킹��
			D3DXMatrixMultiply(&g_Boss.Motion.aModel[nCntModel].mtxWorld,
				&g_Boss.Motion.aModel[nCntModel].mtxWorld,
				&mtxParent);//���������e

			//�p�[�c�̃��[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD,
				&g_Boss.Motion.aModel[nCntModel].mtxWorld);

			for (int nCntMat = 0; nCntMat < (int)g_Boss.Motion.aModel[nCntModel].dwNumMat; nCntMat++)
			{
				//�}�e���A���̃f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_Boss.Motion.aModel[nCntModel].pBuffMat->GetBufferPointer();

				D3DXMATERIAL color;

				if (g_Boss.state != BOSSSTATE_DAMAGE)
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}
				else if (g_Boss.state == BOSSSTATE_DAMAGE)
				{
					color = pMat[nCntMat];

					color.MatD3D.Diffuse.r = 1.0f;
					color.MatD3D.Diffuse.g = 0.3f;
					color.MatD3D.Diffuse.b = 0.3f;
					color.MatD3D.Diffuse.a = 1.0f;

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&color.MatD3D);
				}

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_Boss.Motion.aModel[nCntModel].pTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_Boss.Motion.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&matDef);
		}
	}
}
//=============================
// �{�X�̐ݒ菈��
//=============================
void SetBoss(D3DXVECTOR3 pos, float speed, int nLife)
{
	if (!g_Boss.bUse)
	{
		g_Boss.pos = pos; // �ʒu����
		g_Boss.Speed = speed; // ���̑���
		g_Boss.nLife = nLife; // �̗͂�}��
		g_Boss.bUse = true;   // �g�p��Ԃɂ���
	}
}

void LoadBoss(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	FILE* pFile; // �t�@�C���̃|�C���^

	pFile = fopen("data\\MOTION\\boss.txt", "r");

	int nCntMotion = 0;
	char Skip[3];
	int nCntKey = 0;
	int nCntPosKey = 0;
	int nCntRotkey = 0;
	int nCnt = 0;
	int nNumParts = 0;
	int nIdx = 0;

	if (pFile != NULL)
	{//�@NULL ����Ȃ�
		char aString[MAX_WORD];

		while (1)
		{
			// �ǂݔ�΂�
			fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SCRIPT") == 0)
			{// SCRIPT��ǂݎ������
				while (1)
				{
					// �ǂݔ�΂�
					fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "NUM_MODEL") == 0)
					{// NUM_MODEL��ǂݎ������
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// �l����
							fscanf(pFile, "%d", &g_Boss.Motion.nNumModel);
						}
					}

					else if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
					{
						// MODEL_FILENAME��ǂݎ������
						fscanf(pFile, "%s", &aString[0]);

						if (strcmp(&aString[0], "=") == 0)
						{// ���
							// �������ǂݍ���
							fscanf(pFile, "%s", &aString[0]);

							const char* MODEL_FILE = {};	// ���f���t�@�C�����i�[�p�̕ϐ�

							// �ǂݍ��񂾕������ۑ�����
							MODEL_FILE = aString;

							//X�t�@�C���̓ǂݍ���
							D3DXLoadMeshFromX(MODEL_FILE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_Boss.Motion.aModel[nCnt].pBuffMat,
								NULL,
								&g_Boss.Motion.aModel[nCnt].dwNumMat,
								&g_Boss.Motion.aModel[nCnt].pMesh);

							nCnt++; // nCnt���C���N�������g
						}
					}
					else if (strcmp(&aString[0], "CHARACTERSET") == 0)
					{
						while (1)
						{
							// �������ǂݔ�΂�
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(&aString[0], "NUM_PARTS") == 0)
							{// NUM_PARTS��ǂݎ������

								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// �l����
									fscanf(pFile, "%d", &nNumParts);
								}
							}

							else if (strcmp(&aString[0], "PARTSSET") == 0)
							{
								while (1)
								{
									// �������ǂݔ�΂�
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(&aString[0], "INDEX") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���
											fscanf(pFile, "%d", &nIdx);
										}
									}

									if (strcmp(&aString[0], "PARENT") == 0)
									{// PARENT��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���
											// �y�A�l���g
											fscanf(pFile, "%d", &g_Boss.Motion.aModel[nIdx].nIdxModelParent);
										}
									}


									if (strcmp(&aString[0], "POS") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// ���W����
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.x);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.y);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].offpos.z);
										}
									}

									if (strcmp(&aString[0], "ROT") == 0)
									{// INDEX��ǂݎ������

										fscanf(pFile, "%s", &aString[0]);

										if (strcmp(&aString[0], "=") == 0)
										{// �p�x����
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.x);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.y);
											fscanf(pFile, "%f", &g_Boss.Motion.aModel[nIdx].rot.z);
										}
									}

									if (strcmp(&aString[0], "END_PARTSSET") == 0)
									{// END_PARTSSET��ǂݎ������
										// while�𔲂���
										break;
									}

								}// while����
							}
							else if (strcmp(&aString[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
						}
					}
					else if (strcmp(&aString[0], "MOTIONSET") == 0)
					{// MOTIONSET��ǂݎ������


						while (1)
						{
							// ������ǂݔ�΂�
							fscanf(pFile, "%s", &aString[0]);

							if (strcmp(aString, "LOOP") == 0)
							{// LOOP ��ǂݎ������
								// ������ǂݔ�΂�
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = ��ǂݎ������
									// �l����
									fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].bLoop);
								}
							}

							else if (strcmp(aString, "NUM_KEY") == 0)
							{// NUM_KEY��ǂݎ������
								// ������ǂݔ�΂�
								fscanf(pFile, "%s", &aString[0]);

								if (strcmp(&aString[0], "=") == 0)
								{// = ��ǂݎ������
									// �l����
									fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].nNumkey);
								}

								while (nCntKey < g_Boss.Motion.aMotionInfo[nCntMotion].nNumkey)
								{
									// ������ǂݔ�΂�
									fscanf(pFile, "%s", &aString[0]);

									if (strcmp(aString, "KEYSET") == 0)
									{// KEYSET��ǂݎ������


										while (1)
										{
											// ������ǂݔ�΂�
											fscanf(pFile, "%s", &aString[0]);


											if (strcmp(&aString[0], "FRAME") == 0)
											{// FRAME��ǂݎ������
												// ������ǂݔ�΂�
												fscanf(pFile, "%s", &aString[0]);

												if (strcmp(&aString[0], "=") == 0)
												{// �l����
													fscanf(pFile, "%d", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
													break;
												}
											}

										}

										while (1)
										{
											// ������ǂݔ�΂�
											fscanf(pFile, "%s", &aString[0]);
											if (strcmp(&aString[0], "KEY") == 0)
											{// KEY��ǂ݂Ƃ�����
												while (1)
												{
													// ������ǂݔ�΂�
													fscanf(pFile, "%s", &aString[0]);

													if (strcmp(&aString[0], "POS") == 0)
													{// POS��ǂݎ������
														// ������ǂݔ�΂�
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// �l����
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosX);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosY);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPosKey].fPosZ);
															nCntPosKey++;		// �C���N�������g
														}
													}

													else if (strcmp(&aString[0], "ROT") == 0)
													{// ROT��ǂݎ������
														// ������ǂݔ�΂�
														fscanf(pFile, "%s", &aString[0]);

														if (strcmp(&aString[0], "=") == 0)
														{// �l����
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotX);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotY);
															fscanf(pFile, "%f", &g_Boss.Motion.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRotkey].fRotZ);
															nCntRotkey++;		// �C���N�������g
														}
													}

													else if (strcmp(&aString[0], "END_KEY") == 0)
													{// END_KEY��ǂݎ������
														break;
													}
												}
											}
											else if (strcmp(&aString[0], "END_KEYSET") == 0)
											{// END_KEYSET��ǂݎ������
												nCntRotkey = 0;
												nCntPosKey = 0;
												nCntKey++;			// �C���N�������g
												break;
											}


										}

									}

								}
							}

							if (strcmp(&aString[0], "END_MOTIONSET") == 0)
							{// END_MOTIONSET��ǂݎ������
								nCntMotion++;		// ���[�V�����̍X�V
								nCntKey = 0;		// 0����n�߂�
								break;
							}
						}
					}

					else if (strcmp(&aString[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else
					{
						continue;
					}
				}// while����

				break;
			}
		}// while����
	}
	else
	{
		//���b�Z�[�W�{�b�N�X
		MessageBox(NULL, "�t�@�C�����J���܂���B", "�G���[(Boss.cpp)", MB_OK);
		return;
	}
	// �t�@�C�������
	fclose(pFile);
}
