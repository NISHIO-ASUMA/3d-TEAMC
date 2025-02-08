//=====================================================================================================================
//
// �p�[�e�B�N�� [ Particle.h ]
// Author;�����I��
//
//=====================================================================================================================

//************************************
// 2�d�C���N���[�h�h�~
//************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//************************************
// �C���N���[�h�t�@�C���錾
//************************************
#include "main.h"

//***********************************
// �v���g�^�C�v�錾
//***********************************
void InitParticle(void);   // ����������
void UninitParticle(void); // �I������
void UpdateParticle(void); // �X�V����
void DrawParticle(void);   // �`�揈��
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 randrot, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int nELife, int nAmount, float fSpeed, float fRandR, bool bSimple, D3DXVECTOR3 Emove); // �ݒ菈��
#endif