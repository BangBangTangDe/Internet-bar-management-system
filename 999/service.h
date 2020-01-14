#include"model.h"
using namespace std;

void addCard(CardNode** ppCardNodeHead);
void queryCard(CardNode* const pCardNodeHead);
void clearData(CardNode* pCardNodeHead);

void shangJi(CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead);
int logon(string strNo, string strPwd, LogonInfo* pInfo, CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead);
bool updateCard(const Card* pCard, const string pPath, int nCardIndex);
void xiaJi(CardNode* pCardNodeHead, BillingNode* pBillingNodeHead);
int settle(string strNo, string strPwd, SettleInfo* pInfo, CardNode* const pCardNodeHead, BillingNode* const pBillingNodeHead);
double getAmount(time_t tStart);
void chongzhi(CardNode* pCardNodeHead);
void backbalance(CardNode* pCardNodeHead);
void logout(CardNode* pCardNodeHead);
void statistics(BillingNode* pBillingNodeHead, CardNode* pCardNodeHead);
void logcaarddelete(CardNode **pCardNodeHead);
void recovery(CardNode **t);

