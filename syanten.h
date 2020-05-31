
#pragma once
#ifndef __SYANTEN
#define __SYANTEN

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;

//�V�����e������Ԃ��N���X(HashTable��)
class Syanten{

	void mentu_cut(int);//�����c�����֐�
	void taatu_cut(int);//�^�[�c�����֐�

	int tehai[39];
	int mentu;		//�����c��
	int toitu;		//�g�C�c��
	int kouho;		//�^�[�c��
	int fuurosuu;		//�t�[����
	int temp;		//�V�����e�����i�v�Z�p�j
	int syanten_normal;	//�V�����e�����i���ʗp�j

public:
	Syanten();//�R���X�g���N�^
	int NormalSyanten();//�ʏ��V�����e��
	int KokusiSyanten();//���m�V�����e��
	int TiitoituSyanten();//�`�[�g�C�V�����e��

	void set_tehai(int t[]);//��v�Z�b�g�iint[38]�ȏ�A�ԓ����ς݁j
	void set_fuurosuu(int a){fuurosuu=a;}//�t�[����
	void clear();//���g�N���A
	//�ʏ�V�����e����Ɨp
	int checkNormalSyanten();
	map<int, vector<int>> map_; 
};

#endif
