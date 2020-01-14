#include<iostream>

using namespace std;
#include"model.h"

Billing* BillingIsExist(string strNo, int& nBillingIndex, BillingNode *pBillingNodeHead);
Billing* BillingIsExisttwo(string strNo, int& nBillingIndex, BillingNode *pBillNodeHead);
int logon(string strNo, string strPwd, LogonInfo* pInfo, CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead);
bool updateCard(const Card* pCard, const string pPath, int nCardIndex);
int settle(string strNo, string strPwd, SettleInfo* pInfo, CardNode* const pCardNodeHead, BillingNode* const pBillingNodeHead);
double getAmount(time_t tStart);
int judgeaddmoney(string strNo, string strPwd, addmoney* p, CardNode * pCardNodeHead);
int judgebackmoney(string strNo, string strPwd, backmoney* p, CardNode* pCardNodeHead);
int judgelogout(string strNo, string strPwd, logoutcard* pout, CardNode* pCardNodeHead);
//int querrycard(string strNo, string strPwd, querryonecard* q, CardNode* pCardNodeHead);
//查询统计的声明
void querryonebill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead);
void totalMoney(BillingNode* pBillNodeHead);
void recordtimebill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead);
void daymoney(BillingNode* pBillNodeHead);
void yearmoney(BillingNode* pBillNodeHead);
void logcard(CardNode *pCardNodeHead);
