#include<iostream>

#include"model.h"
; using namespace std;
CardNode* CardListInit(const string cardFilename);
int addnewcard(string strNo, string strPwd, float balance, CardNode **ppCardNodeHead);
Card* CardIsExist(string strNo, int &nCardIndex, CardNode *pCardNodeHead);
bool updateCard(const Card* pCard, const string pPath, int nCardIndex);
bool updatelogout(const Card* pCard, const string pPath, int nCardIndex);
void writeallvec(CardNode*pCardNodeHead, int nCardIndex);