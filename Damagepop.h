//=====================================================================================================================
//
//�_���[�W�\���֘A�̃w�b�_�[
//Author;�����I��
//
//=====================================================================================================================

// �d���m�F
#ifndef _DAMEGE_H_
#define _DAMEGE_H_

// �C���N���[�h�t�@�C��
#include "Main.h"

// �v���g�^�C�v�錾
void InitDamege(void);
void UninitDamege(void);
void UpdateDamege(void);
void DrawDamege(void);
void SetDamege(D3DXVECTOR3 pos, int nDamege, int nLife, bool Heal);

#endif