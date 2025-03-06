//==============================================================================================================
//
// �{�X�̗̑̓Q�[�W [ bosslife.h ]
// Author:Asuma Nishio
//
//==============================================================================================================

#ifndef _BOSSLIFE_H_
#define _BOSSLIFE_H_

//***************************
// �C���N���[�h�t�@�C���錾
//***************************
#include "main.h"

//***************************
// ��ނ̗񋓌^
//***************************
typedef enum
{
	BOSSTEX_LIFE = 0,
	BOSSTEX_FRAME,
	BOSSTEX_DELAY,
	BOSSTEX_MAX
}BOSSTEX;

//***************************
// �e�N�X�`���ݒ�
//***************************
static const char* BOSSLIFE_TEX[BOSSTEX_MAX] =
{
	"data\\TEXTURE\\boss_life.png",
	"data\\TEXTURE\\boss_lifeframe.png",
	"data\\TEXTURE\\boss_lifedamage.png",
};

//***************************
// �\���̐錾
//***************************
typedef struct
{
	D3DXMATRIX mtxWorld; // ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos; //�@���W
	int nType; // ���
	bool bUse; // �g�p����
	float fLength,DelayLength; // �̗̓o�[�̒���
	int nEasingCnt;   // �C�[�W���O�̃J�E���g
}BossLife;

//***************************
// �v���g�^�C�v�錾
//***************************
void InitBossLife(); // �̗̓o�[�̏���������
void UninitBossLife(); // �̗̓o�[�̏I������
void UpdateBossLife(Boss* pBoss); // �̗̓o�[�̍X�V����
void DrawBossLife(); // �̗̓o�[�̕`�揈��
int SetBossLife(D3DXVECTOR3 pos, int nType); // �̗̓o�[�̐ݒ菈��
void SetPositionLifeBar(int nIdxBar,int nIdxFrame,int nIdxDelay,D3DXVECTOR3 pos); // �{�X��HP�Q�[�W�̍X�V����
void DeleateLifeBar(int nIdxBar,int nIdxFrame,int nIdxDelay); // �{�X�̗̑̓Q�[�W�̏���
#endif
