#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED 
#include<string>
using namespace std;
const string CARDPATH = "card.txt";	
const string BILLINGPATH = "billing.txt";
const string MONEYPATH = "money.txt";
const int TIMELENGTH = 20; // 时间字符数组长度
const int SUCCESS = 0;      
const int FINDCARD = 1;
const int NOFINDCARD = 2;
const int NOMATCH = 3;   //卡号密码不匹配
const int ENOUGHMONEY = 4;  //卡余额不足 
const int NOFARE = 5;
const int USING = 6;
const int UNUSE = 7;
const int INVALID = 8;  //注销了
const int NOSETTLEMENT = 0; //上机已经结算
const int YESSETTLEMENT = 1; //上机没有结算
const int UNIT = 15;    //最小收费单元(分钟)
const float CHARGE = 0.5f; //  每个计费单元收费(元) 
#endif

