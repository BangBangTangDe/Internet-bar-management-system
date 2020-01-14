#include"model.h"
#include"cardlist.h"
bool saveCard(const Card* pCard, const char* pPath);
CardNode* CardListInit(const string cardFilename);

BillingNode* BillListInit(const string billingFilename);
bool saveBilling(const Billing* pBilling, const char* pPath);
