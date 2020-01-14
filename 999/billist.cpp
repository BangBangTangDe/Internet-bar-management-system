#include"stdafx.h"
#include<string>
#include<iostream>
#include<fstream>

using namespace std;
#include "menu.h" 
#include "global.h"
#include"model.h"
#include"cardlist.h"
#include"billist.h"
#include"baocuncard.h"
#include"tool.h"



// 卡的计费信息是否存在返回计费在链表中的节点
Billing* BillingIsExist(string strNo, int& nBillingIndex, BillingNode *pBillNodeHead)
{    
	BillingNode *pBillingNode = pBillNodeHead; 


       nBillingIndex = 0; 
while (pBillingNode != NULL)
{ 
	if (strcmp(pBillingNode->data.aCardName, strNo.c_str()) == 0 && pBillingNode->data.nStatus == NOSETTLEMENT) 
	{ 
		return &(pBillingNode->data);
	
	}   
	pBillingNode = pBillingNode->next;   
	nBillingIndex++;
}

return NULL; 
}  

//判断是否可以统计查询单卡的遍历函数
Billing* BillingIsExisttwo(string strNo, int& nBillingIndex, BillingNode *pBillNodeHead)
{
	BillingNode *pBillingNode = pBillNodeHead;


	nBillingIndex = 0;
	while (pBillingNode != NULL)
	{
		if (strcmp(pBillingNode->data.aCardName, strNo.c_str()) == 0 && pBillingNode->data.nStatus == YESSETTLEMENT)
		{
			return &(pBillingNode->data);

		}
		pBillingNode = pBillingNode->next;
		nBillingIndex++;
	}

	return NULL;
}

//判断可以上机
int logon(string strNo, string strPwd, LogonInfo* pInfo, CardNode* pCardNodeHead, BillingNode **ppBillingNodeHead)
{
	Billing billing;  // 计费信息   
	int nCardIndex;
	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // 未找到卡 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // 密码不匹配 

	if (pCard->nStatus == USING)
	{
		return USING;           // 卡正在上机 
	}

	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // 卡已经注销  
	}

	if (pCard->balance <= 0) {

		return ENOUGHMONEY;     // 卡的余额为0  
	}

	// 如果可以上机，更新卡信息 
	pCard->nStatus = USING;
	// 状态为正在使用  
	pCard->nusecount++;
	// 使用次数加1 
	pCard->tLast = time(NULL);
	// 更新最后使用时间为当前时间 
	
	// 更新文件中的卡信息  
	updateCard(pCard, CARDPATH, nCardIndex);

	// 添加消费记录  
	strcpy(billing.aCardName, strNo.c_str());
	// 上机卡号 
	billing.tStart = time(NULL);
	// 上机时间 
	billing.tEnd = 0;                            // 下机时间 
	billing.nStatus = NOSETTLEMENT;              // 消费状态 
	billing.fAmount = 0;                         // 消费金额 

												 // 先将计费信息保存到文件中
	saveBilling(&billing, "billing.dat");

	// 在计费链表中增加一条计费信息 
	BillingNode* pBillingNode = new BillingNode;  pBillingNode->data = billing;

	BillingNode* pBillingNodeTail = *ppBillingNodeHead;

	pBillingNode->next = NULL;
	if (*ppBillingNodeHead == NULL)
	{
		*ppBillingNodeHead = pBillingNode;
		//pBillingNodeTail = pBillingNode;
	}
	else
	{
		while (pBillingNodeTail->next != NULL)
			pBillingNodeTail = pBillingNodeTail->next;//while循环找到末尾为空的节点


		pBillingNodeTail->next = pBillingNode;
		//pBillingNodeTail = pBillingNode;
	}   
	
	//pBillingNodeTail->next = NULL;

	// 组装上机信息 
	strcpy(pInfo->aCardName, strNo.c_str());
	pInfo->fbalance = pCard->balance;
	pInfo->tLogon = billing.tStart;
	return SUCCESS;
}
double getAmount(time_t tStart)
{
	double dbAmount = 0;       // 消费金额
	int nCount = 0;            // 上机的时间单元数，每个单元15分钟  
	int nSec = 0;              // 消费时间(单位：秒)  
	int nMinutes = 0;          // 消费时间(单位：分钟)  
	time_t tEnd = time(NULL);  // 结算时间为当前时间 


							   // 计算消费时长 
	nSec = (int)(tEnd - tStart);
	nMinutes = nSec / 60;

	// 计算消费的时间单元数 
	if (nMinutes % UNIT == 0)
	{

		nCount = nMinutes / UNIT;//最小的计时单元
	}
	else 
	{
		nCount = nMinutes / UNIT + 1;//多出来的时间也算15分钟计算
	}

	if (nCount == 0)
		nCount = 1;

	// 计算消费金额  
	dbAmount = nCount * CHARGE;

	return dbAmount;
}//计算消费金额的函数
bool updateBilling(const Billing* pBilling, const string pPath, int nBillingIndex)
{
	fstream ofile;
	ofile.open(pPath, ios_base::in | ios_base::out);
	ofile.seekp(sizeof(Billing)*nBillingIndex, ios_base::beg);
	ofile.write((char*)pBilling, sizeof(Billing));
	ofile.close();

	return true;
}
int settle(string strNo, string strPwd, SettleInfo* pInfo, CardNode* const pCardNodeHead, BillingNode* const pBillingNodeHead)
{
	int nCardIndex;
	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	// 未找到卡     
	if (pCard == NULL)
		return NOFINDCARD;

	// 密码不匹配    
	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;

	// 判断该卡是否正在上机，只有正在上机卡才能进行下机操作  
	if (pCard->nStatus != USING)   return UNUSE;

	// 根据卡号，查询计费信息 
	int nBillingIndex;
	Billing* pBilling = BillingIsExist(strNo, nBillingIndex, pBillingNodeHead);


	// 如果查询计费信息为空，表示下机失败
	if (pBilling == NULL) { cout << "计费信息为空" << endl;   return UNUSE; }

	// 计算消费金额
	double dbAmount = getAmount(pBilling->tStart);


	// 如果余额小于消费金额，则不能进行下机  
	float fBalance = pCard->balance - (float)dbAmount;
	if (fBalance < 0) { return ENOUGHMONEY; }

	// 更新卡信息 
	pCard->balance = fBalance;        // 余额 
	pCard->nStatus = UNUSE;            // 状态 
	pCard->tLast = time(NULL);         // 上次使用时间 
	pCard->totaluse = pCard->totaluse + dbAmount;
									   // 更新文件中的卡信息 
	updateCard(pCard, CARDPATH, nCardIndex);

	// 更新计费信息  
	pBilling->fAmount = (float)dbAmount;    // 消费信息 
	pBilling->nStatus = YESSETTLEMENT;       // 状态，已结算
	pBilling->tEnd = time(NULL);            // 下机时间 

											// 更新文件中的计费信息  
	updateBilling(pBilling, BILLINGPATH, nBillingIndex);



	// 组装下机信息  
	strcpy(pInfo->aCardName, strNo.c_str());    // 卡号 
	pInfo->fAmount = (float)dbAmount;           // 消费金额  
	pInfo->fbalance = fBalance;                 // 余额
	pInfo->tStart = pBilling->tStart;           // 上机时间
	pInfo->tEnd = time(NULL);                   // 下机时间 

	return SUCCESS;

}
// 根据上机时间，计算消费金额
//判断能否充值
int judgeaddmoney(string strNo , string strPwd, addmoney* p, CardNode* pCardNodeHead)
{
	
	 int a;
	int nCardIndex;

	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

if (pCard == NULL)
return NOFINDCARD;      // 未找到卡 

if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
return NOMATCH;         // 密码不匹配 
if (pCard->nStatus == INVALID)
{
	return INVALID;         // 卡已经注销  
}

cout << "卡中的余额为：" << pCard->balance;
  cout << "请输入要充值的金额";
   cin >> a;
   pCard->balance + a;
 p->afterbalance = pCard->balance + a;

										
// 组装充值信息 
strcpy(p->aCardName, strNo.c_str());
 pCard->balance= p->afterbalance;


// 更新文件中的卡信息  
updateCard(pCard, CARDPATH, nCardIndex);


return SUCCESS;
}
//判断能否退费
int judgebackmoney(string strNo, string strPwd, backmoney* pBack, CardNode* pCardNodeHead)
{

	 unsigned  int a;
	int nCardIndex;
	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // 未找到卡 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // 密码不匹配 

	if (pCard->nStatus == USING)
	{
		return USING;           // 卡正在上机 
	}

	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // 卡已经注销  
	}

	if (pCard->balance <= 0) {

		return ENOUGHMONEY;     // 卡的余额为0  
	}
	while (1)
	{
		cout << "卡中的余额为：" << pCard->balance;
		cout << "请输入要退费的金额";
		cin >> a;
			if (a <= pCard->balance)
			{
				pBack->afterbackmoney = pCard->balance - a;
				break;
			}
			else
			{
				cout << "卡中余额不满足退费要求" << endl;
			}
	}
	//组装退费的信息
	strcpy(pBack->aCardName, strNo.c_str());
	pCard->balance = pBack->afterbackmoney;


	// 更新文件中的卡信息  
	updateCard(pCard, CARDPATH, nCardIndex);



	return SUCCESS;
}

//判断能否注销
int judgelogout(string strNo, string strPwd,logoutcard* pout, CardNode* pCardNodeHead)
{
	unsigned int a;
	int nCardIndex;

	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // 未找到卡 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // 密码不匹配 

	if (pCard->nStatus == USING)
	{
		return USING;           // 卡正在上机 
	}
	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // 卡已经注销  
	}
	if (pCard->balance <0) 
	{

		return ENOUGHMONEY;     // 卡的余额为0  
	}
	
	while (1)
	{ 
		
		cout << "卡中的余额为：" << pCard->balance;
		cout << "请确认注销操作 1 是 " << endl;
	cout << "请输入：";
	cin >> a;
	   if (a=1)
	{
		
		cout << "卡中余额已经清空." << endl;
		break;

	}
	   else
	   {
		   cout << "请重新选择操作" << endl;
	   }


	}

	pout->money = 0;
	pCard->nStatus = INVALID;
	// 组装注销信息 
	strcpy(pout->aCardName, strNo.c_str());
	pCard->balance = pout->money;
	// 更新文件中的卡信息  
	updateCard(pCard, CARDPATH, nCardIndex);



	return SUCCESS;

	
}

/*
//判断单个卡是否在链表之中
int querrycard(string strNo, string strPwd, querryonecard* q, CardNode* pCardNodeHead)
{
	int a;
	int nCardIndex;

	Card* pCard = CardIsExist(strNo, nCardIndex, pCardNodeHead);

	if (pCard == NULL)
		return NOFINDCARD;      // 未找到卡 

	if (strcmp(pCard->aPwd, strPwd.c_str()) != 0)
		return NOMATCH;         // 密码不匹配 
	if (pCard->nStatus == INVALID)
	{
		return INVALID;         // 卡已经注销  
	}

	return SUCCESS;
}

*/

//查询统计函数部分的具体函数以下~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//查找单个卡的消费信息和所有卡的消费信息
void querryonebill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead)
{
	string strNo;
	Billing * pbill;
	CardNode * q = pCardNodeHead;
	if (inputCardNo(strNo))
	{
		int nBillingIndex;
		nBillingIndex = 0;
		pbill = BillingIsExisttwo(strNo, nBillingIndex, pBillNodeHead);
		cout << endl << "----------单卡消费信息----------" << endl;

		if (pbill == NULL)
		{
			cout << endl << endl << "消费信息不存在！" << endl << endl;
			return;
		}                                                // /t的作用，向后跳四格

		cout << "卡号\t状态\t消费金额\t 上机时间\t\t下机时间" << endl;
		if (pbill != NULL)
		{
			char aStartTime[TIMELENGTH] = { 0 };
			char aEndTime[TIMELENGTH] = { 0 };
			timeToString(pbill->tStart, aStartTime);
			timeToString(pbill->tEnd, aEndTime);
			cout << pbill->aCardName << "\t";
			if ((q->data).nStatus == USING)
				cout << "上机\t";
			else if ((q->data).nStatus == UNUSE)
				cout << "未上机\t";
			else if ((q->data).nStatus == INVALID)
				cout << "注销\t";

			cout << pbill->fAmount << "\t\t";
			cout << aStartTime << "    " << "\t" << aEndTime << endl;

		}
	}

}
//总营业额统计
void totalMoney(BillingNode* pBillNodeHead)
{
	float tMoney = 0;
	BillingNode * t = pBillNodeHead;
	if (t == NULL)
	{
		cout << "不存在消费额" << endl;
	}

	while (t != NULL)
	{
		tMoney += (t->data).fAmount;
		t = t->next;
	}

	cout << "总营业额为：" << tMoney << endl;
}
//分时段查询卡的消费记录
void recordtimebill(BillingNode* pBillNodeHead, CardNode* pCardNodeHead)
{//time_t只是一个长整形的类型   tm是一个结构体

	int year1, month1, day1;
	int year2, month2, day2;
	cout << "请输入查询消费记录的时间段：" << endl;
	cout << "查询开始日期："<<endl; 
	cout << "年--->"; cin >> year1;   cout << "月--->"; cin >> month1;  cout << "日--->"; cin >> day1; cout << endl;
	cout << "查询截至日期："<<endl;
	cout << "年--->"; cin >> year2;  cout << "月--->"; cin >> month2; cout << "日--->"; cin >> day2; cout << endl;
	struct tm t1, t2;
	t1.tm_year = year1 - 1900;
	t1.tm_mon = month1 - 1;
	t1.tm_mday = day1;
	t1.tm_hour = 0;
	t1.tm_isdst = 0;
	t1.tm_min = 0;
	t1.tm_yday = 0;
	t1.tm_sec = 0;

	t2.tm_year = year2 - 1900;
	t2.tm_mon = month2 -1;
	t2.tm_mday = day2;
	
	t2.tm_hour = 0;
	t2.tm_isdst = 0;
	t2.tm_min = 0;
	t2.tm_yday = 0;
	t2.tm_sec = 0;
	CardNode * q = pCardNodeHead;
	BillingNode *p = pBillNodeHead;
	cout << endl << "----------卡消费信息----------" << endl;

	cout << "卡号\t状态\t消费金额\t 上机时间\t\t\t下机时间" << endl;
	if (p == NULL)
	{
		cout << "消费信息无记录" << endl;

      }
	while (p != NULL)
	{
		
		if (mktime(&t1) <= q->data.tLast    &&    q->data.tLast <= mktime(&t2))
		{
			char aStartTime[TIMELENGTH] = { 0 };
			char aEndTime[TIMELENGTH] = { 0 };
			timeToString((p->data).tStart, aStartTime);
			timeToString((p->data).tEnd, aEndTime);
			cout << p->data.aCardName << "\t";
			if ((q->data).nStatus == USING)
				cout << "上机\t";
			else if ((q->data).nStatus == UNUSE)
				cout << "未上机\t";
			else if ((q->data).nStatus == INVALID)
				cout << "注销\t";

			cout << p->data.fAmount << "\t\t";
			cout << aStartTime << "\t\t" << aEndTime << endl;

		}
		else
		{
			cout << "消费记录不存在" << endl;
		}
		p = p->next;
		
	}
}
//日总营业额
void daymoney(BillingNode* pBillNodeHead)
{
	int year1, month1, day1;
	time_t t2;
	float money = 0;
	cout << "请输入查询的日期：" << endl;
	cin >> year1; cin >> month1; cin >> day1;

	struct tm t1;
	t1.tm_year = year1 - 1900;
	t1.tm_mon = month1 - 1;
	t1.tm_mday = day1;
	t1.tm_hour = 0;
	t1.tm_isdst = 0;
	t1.tm_min = 0;
	t1.tm_yday = 0;
	t1.tm_sec = 0;

	t2 = time(NULL);




	BillingNode *p = pBillNodeHead;
	if (p == NULL)
	{
		cout << "日消费金额为0" << endl;

	}
	while (p != NULL)
	{
		if (mktime(&t1) < p->data.tEnd    &&    p->data.tEnd <= t2)
		{

			money += p->data.fAmount;

		}

		p = p->next;
	}

	cout << "日消费金额为：" << money << endl;

}
//年总营业额
void yearmoney(BillingNode* pBillNodeHead)
{
	int year, month, day;
	float money = 0;



	cout << "请输入所要查询的年份" << endl;
	cin >> year;

	BillingNode *p = pBillNodeHead;
	if (p == NULL)
	{
		cout << "年消费金额为0" << endl;

	}
	while (p != NULL)
	{

		if (stringToTime(year, 1, 1) <= p->data.tEnd    &&  p->data.tEnd < stringToTime(year + 1, 1, 1))
		{

			money += p->data.fAmount;

		}

		p = p->next;
	}

	cout << "年消费金额为：" << money << endl;







}
//查询已经注销的卡
void logcard(CardNode *pCardNodeHead)
{
	cout << "账号" << "\t" << "状态" << endl;
	int count = 0;
	CardNode * t = pCardNodeHead;
	if (t == NULL)
	{
		cout << "注销的卡不存在记录中" << endl;
	}
	while (t != NULL)
	{
		if (t->data.nStatus == INVALID)
		{
			cout << t->data.aName << "\t" << "已注销" << endl;
			count = count + 1;
		}

		t = t->next;

	}
	cout << "注销卡的数量为" << count << endl;
}