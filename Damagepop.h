//===================================
//
//�_���[�W�\���֘A�̃w�b�_�[
//Author;�����I��
//
//===================================

// �d���m�F�ƃC���N���[�h
#ifndef _DAMEGE_H_
#define _DAMEGE_H_
#include "Main.h"

// �����̐F�X
void InitDamege(void);
void UninitDamege(void);
void UpdateDamege(void);
void DrawDamege(void);
void SetDamege(D3DXVECTOR3 pos, int nDamege, int nLife, bool Heal);

#endif