#include <string>
#include"model.h"
using namespace std;
void outputMenu();
bool inputCardNo(string& strNo);
bool inputCardPwd(string& strPwd);
bool inputCardBalance(float& balance);
bool inputNoPwd(string& strNo, string& strPwd);
bool inputNoPwdBalance(string& strNo, string& strPwd, float& balance);
void displayCard(CardNode *pCardNodeHead);
void displayBill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead);

