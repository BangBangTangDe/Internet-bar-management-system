#pragma once
#include<time.h>
struct Card
{
	char aName [18];
	char aPwd[8];
	int nStatus;  //卡状态(UNUSE - 未上机；USING - 正在上机；INVALID - 已注销
	time_t tstart; //开卡时间
	float totaluse;//累计金额
	time_t tLast; //最后使用时间
	int nusecount; //使用次数
	float balance;//余额
};
struct CardNode
{
	Card data;
	struct CardNode*next;

};


//查询单卡的结构体
struct querryonecard
{
	char aCardName[18];
	float dollar;
};

//计费信息结构体
struct Billing
{
	char aCardName[18];//上机卡号
	time_t tStart;
	time_t tEnd;
	float fAmount;//消费金额
	int nStatus;//上机状态
	int nDel;//删除标识


};

//计费信息节点
struct BillingNode
{
	Billing data;
	struct BillingNode*next;


};

//上机结构体
struct LogonInfo
{
	char aCardName[18];
	time_t tLogon;
	float fbalance;
};
//下机结构体
struct SettleInfo
{
	char aCardName[18];
	time_t tStart;
	time_t tEnd;
	float fAmount; //消费金额
	float fbalance;  //余额
};
//充值结构体
struct addmoney
{
	char aCardName[18];
	float afterbalance;

};
//退费结构体
struct backmoney
{
	char aCardName[18];
	float afterbackmoney;

};
//注销卡的结构体

struct logoutcard
{
	char aCardName[18];
	float money;


};


//查询单卡的消费记录
struct onebillremember
{
	char aCardName[18];
	float money;
	float fAmount;
	time_t tStart;
	time_t tEnd;
};
