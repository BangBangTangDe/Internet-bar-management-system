#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED 
#include<string>
using namespace std;
const string CARDPATH = "card.txt";	
const string BILLINGPATH = "billing.txt";
const string MONEYPATH = "money.txt";
const int TIMELENGTH = 20; // ʱ���ַ����鳤��
const int SUCCESS = 0;      
const int FINDCARD = 1;
const int NOFINDCARD = 2;
const int NOMATCH = 3;   //�������벻ƥ��
const int ENOUGHMONEY = 4;  //������ 
const int NOFARE = 5;
const int USING = 6;
const int UNUSE = 7;
const int INVALID = 8;  //ע����
const int NOSETTLEMENT = 0; //�ϻ��Ѿ�����
const int YESSETTLEMENT = 1; //�ϻ�û�н���
const int UNIT = 15;    //��С�շѵ�Ԫ(����)
const float CHARGE = 0.5f; //  ÿ���Ʒѵ�Ԫ�շ�(Ԫ) 
#endif

