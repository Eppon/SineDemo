

#include "math.h"

const float SAMPINGRATE = 200000;
const float SAMPINGTIME = 1/SAMPINGRATE;
const float NOTZERO = 0.00001f;
const int CHANNEL = 8;
const int NOISECHECKTIME =(int)(5 * SAMPINGRATE);
const int TABLELEN = 2048;
const long long int INF = 10000000000000;
const float PI2 = 3.1415926f * 2;
const float PI = 3.1415926f;
const int SAMPLETIMES = 100;

typedef struct SetPara
{
	int   iTestMode;             //试验模式
	int   iRunMode;              //操作模式
	int   iTestSweeps;           //试验扫频次数
	float fTestTime;             //试验总时间
	int   iTestCycles;           //试验周期数
	float fMaxFreq;              //频率上限
	float fMinFreq;              //频率下限
	float fSweepRate;            //扫描率
	float fSweepTime;            //单次扫频时间
	float fDwellFreq;            //驻留频率
	int   iFDDMode;              //频域保存
	int   iFDDTimes;             //频域保存频次
	int   iTPDMode;              //时域保存
	int   iTPDTimes;             //频域保存频次

	float fInitialLevel;         //试验初始量级
	int   iStartDirect;          //起始方向
	int   iWeightMode;           //控制加权策略
	int   iDriveFix;             //驱动修正
	float fTheoryWeight;         //修正系数

	int   iCompressionMode;      //压缩策略
	float fLowRadioFreq;         //压缩策略_频率间隔1
	float fHighRadioFreq;        //压缩策略_频率间隔2
	float fLowRadioTime;         //压缩策略_低频段更新时间
	float fHighRadioTime;        //压缩策略_中频段更新周期
	int   fMidRadioCycles;       //压缩策略_高频段更新时间

	float fppScheTable[101][9];      //计划表
	int   iScheTableLen;             //计划表长度

	float fppTableFreq[8][101];       //8个谱的频率折点
	float fppTableAcc[8][101];        //8个谱的加速度折点
	float fppTableVel[8][101];        //8个谱的速度折点
	float fppTableDesp[8][101];       //8个谱的位移折点
	float fppTableLeftSlp[8][101];    //8个谱的左斜率折点
	float fppTableRightSlp[8][101];   //8个谱的右斜率折点
	float fppTableLowAlm[8][101];     //8个谱的报警下限折点
	float fppTableHighAlm[8][101];    //8个谱的报警上限折点
	float fppTableLowAbt[8][101];     //8个谱的中止下限折点
	float fppTableHighAbt[8][101];    //8个谱的中止上限折点
	int   ipProfileType[8];           //8个谱的类型（加速orEu）
	int   iTableDisPlay;              //谱显示（加速度谱/速度谱/位移谱）

	int ipChannelType[CHANNEL];       //通道类型
	int ipChannelCheck[CHANNEL];      //自检
	int ipRecogMode[CHANNEL];         //识别模式
	float fpWeighting[CHANNEL];       //权重
	int ipSenserType[CHANNEL];        //传感器类型
	int ipCouplingType[CHANNEL];      //耦合方式
	float fpSensitivity[CHANNEL];     //灵敏度
	float fpSenserUnit[CHANNEL];      //单位
	float fpSenserRange[CHANNEL];     //量程
	int ipChannelAbort[CHANNEL];      //限值中断
	float fpChannellimit[CHANNEL];    //通道限值
	int ipReferType[CHANNEL];         //限制谱

	float fMaxShakerInput;      //功放最大输入
	float fMaxShakerD;          //振动台最大位移
	float fMaxShakerV;          //振动台最大速度
	float fMaxShakerSineA;      //振动台最大正弦加速度
	float fMaxShakerRandA;      //振动台最大随机加速度
	float fMaxShakerShockA;     //振动台最大冲击加速度
	float fMaxShakerSineF;      //振动台最大正弦推力
	float fMaxShakerRandF;      //振动台最大随机推力
	float fMaxShakerShockF;     //振动台最大冲击推力

	float fLevelupRate;          //量级抬升速率
	float fLeveldownRate;        //量级下降速率

	float fNoiseThreshold;      //噪声检查阈值
	float fLoopCheckFreq;       //闭环检查频率
	float fLoopCheckLevel;      //闭环通过量级
	float fLoopCheckLimit;      //闭环最大驱动

	float fCSLLevel;            //控制信号丢失
	float fMaxDrive;            //最大驱动电压
	int   iAbortLoops;          //中断条件
}SetPara;

SetPara SSetPara=
{
	0,//int   iTestMode;             //试验模式
	2,//int   iRunMode;              //操作模式
	3,//int   iTestSweeps;           //试验扫频次数
	300,//float fTestTime;             //试验总时间
	300,//int   iTestCycles;           //试验周期数
	2000,//float fMaxFreq;              //频率上限
	5,//float fMinFreq;              //频率下限
	4,//float fSweepRate;            //扫描率
	100,//float fSweepTime;            //单次扫频时间
	200,//float fDwellFreq;            //驻留频率
	0,//int   iFDDMode;              //频域保存
	1,//int   iFDDTimes;             //频域保存频次
	0,//int   iTPDMode;              //时域保存
	1,//int   iTPDTimes;             //时域保存频次

	-20,//float fInitialLevel;         //试验初始量级
	0,//int   iStartDirect;          //起始方向
	0,//int   iWeightMode;           //控制加权策略
	0,//int   iDriveFix;             //驱动修正
	0,//float fTheoryWeight;         //修正系数

	3,//int   iCompressionMode;      //压缩策略
	50,//float fLowRadioFreq;         //压缩策略_频率间隔1
	250,//float fHighRadioFreq;        //压缩策略_频率间隔2
	0.16,//float fLowRadioTime;         //压缩策略_低频段更新时间
	0.032,//float fHighRadioTime;        //压缩策略_中频段更新周期
	8,//int   fMidRadioCycles;       //压缩策略_高频段更新时间

	{
		{ 0, 5, 2000, 4, 0, 2, INF, 2, 2 },
		{ 1, 5, 5, 3000, -3 },
		{ 2, 5 },
		{ 0, 5, 2000, 4, 0, 2, INF, 2, 2 }, 
		{ 1, 5, 5, 3000, -3 }, 
		{ 3 }
	},//float fppScheTable[101][9];      //计划表
	5, //int   iScheTableLen;             //计划表长度

	{ { 5, 200, 2000 } },//float fppTableFreq[8][101];       //8个谱的频率折点
	{ { 1, 2, 2 } },//float fppTableAcc[8][101];        //8个谱的加速度折点
	{ { 5, 200, 2000 } },//float fppTableVel[8][101];        //8个谱的速度折点
	{ { 5, 200, 2000 } },//float fppTableDesp[8][101];       //8个谱的位移折点
	{ { 5, 200, 2000 } },//float fppTableLeftSlp[8][101];    //8个谱的左斜率折点
	{ { 5, 200, 2000 } },//float fppTableRightSlp[8][101];   //8个谱的右斜率折点
	{ { -3, -3, -3 } },//float fppTableLowAlm[8][101];     //8个谱的报警下限折点
	{ { 3, 3, 3 } },//float fppTableHighAlm[8][101];    //8个谱的报警上限折点
	{ { -6, -6, -6 } },//float fppTableLowAbt[8][101];     //8个谱的中止下限折点
	{ { 6, 6, 6 } },//float fppTableHighAbt[8][101];    //8个谱的中止上限折点
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int   ipProfileType[8];           //8个谱的类型（加速orEu）
	0,//int   iTableDisPlay;              //谱显示（加速度谱/速度谱/位移谱）

	{ 2, 2, 2, 2, 2, 2, 2, 2 }, //int ipChannelType[CHANNEL];       //通道类型
	{ 1, 1, 1, 1, 1, 1, 1, 1 }, //int ipChannelCheck[CHANNEL];      //自检
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipRecogMode[CHANNEL];         //识别模式
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//float fpWeighting[CHANNEL];       //权重
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//int ipSenserType[CHANNEL];        //传感器类型
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//int ipCouplingType[CHANNEL];      //耦合方式
	{ 100, 100, 100, 100, 100, 100, 100, 100 },//float fpSensitivity[CHANNEL];     //灵敏度
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//float fpSenserUnit[CHANNEL];      //单位
	{ 1, 1, 1, 1, 1, 1, 1, 1 },//float fpSenserRange[CHANNEL];     //量程
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipChannelAbort[CHANNEL];      //限值中断
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//float fpChannellimit[CHANNEL];    //通道限值
	{ 0, 0, 0, 0, 0, 0, 0, 0 },//int ipReferType[CHANNEL];         //限制谱

	10,//float fMaxShakerInput;      //功放最大输入
	50,//float fMaxShakerD;          //振动台最大位移
	50,//float fMaxShakerV;          //振动台最大速度
	50,//float fMaxShakerSineA;      //振动台最大正弦加速度
	50,//float fMaxShakerRandA;      //振动台最大随机加速度
	50,//float fMaxShakerShockA;     //振动台最大冲击加速度
	50,//float fMaxShakerSineF;      //振动台最大正弦推力
	50,//float fMaxShakerRandF;      //振动台最大随机推力
	50,//float fMaxShakerShockF;     //振动台最大冲击推力

	20,//float fLevelupRate;          //量级抬升速率
	-20,//float fLeveldownRate;        //量级下降速率

	50,//float fNoiseThreshold;      //噪声检查阈值
	200,//float fLoopCheckFreq;       //闭环检查频率
	6,//float fLoopCheckLevel;      //闭环通过量级
	300,//float fLoopCheckLimit;      //闭环最大驱动

	-12,//float fCSLLevel;            //控制信号丢失
	10,//float fMaxDrive;            //最大驱动电压
	1,//int   iAbortLoops;          //中断条件
};

typedef struct InputPara
{
	float fSamplingRate;          //采样率
	float fSamplingTime;         //采样间隔

	int   iTestMode;             //试验模式
	int   iRunMode;              //操作模式
	int   iTestSweeps;           //试验扫频次数
	float fTestTime;             //试验总时间
	int   iTestCycles;           //试验周期数
	float fMaxFreq;              //频率上限
	float fMinFreq;              //频率下限
	float fSweepRate;            //扫描率
	float fSweepTime;            //单次扫频时间
	float fDwellFreq;            //驻留频率
	int   iFDDMode;              //频域保存
	int   iFDDTimes;             //频域保存频次
	int   iTPDMode;              //时域保存
	int   iTPDTimes;             //频域保存频次

	float fInitialLevel;         //试验初始量级
	int   iStartDirect;          //起始方向
	int   iWeightMode;           //控制加权策略
	int   iDriveFix;             //驱动修正
	float fTheoryWeight;         //修正系数

	int   iCompressionMode;      //压缩策略
	float fLowRadioFreq;         //压缩策略_频率间隔1
	float fHighRadioFreq;        //压缩策略_频率间隔2
	float fLowRadioTime;         //压缩策略_低频段更新时间
	float fHighRadioTime;        //压缩策略_中频段更新周期
	int   fMidRadioCycles;       //压缩策略_高频段更新时间

	float fppScheTable[101][8];      //计划表
	int   iScheTableLen;             //计划表长度

	float fppTableFreq[8][101];       //8个谱的频率折点
	float fppTableAcc[8][101];        //8个谱的加速度折点
	float fppTableVel[8][101];        //8个谱的速度折点
	float fppTableDesp[8][101];       //8个谱的位移折点
	float fppTableLeftSlp[8][101];    //8个谱的左斜率折点
	float fppTableRightSlp[8][101];   //8个谱的右斜率折点
	float fppTableLowAlm[8][101];     //8个谱的报警下限折点
	float fppTableHighAlm[8][101];    //8个谱的报警上限折点
	float fppTableLowAbt[8][101];     //8个谱的中止下限折点
	float fppTableHighAbt[8][101];    //8个谱的中止上限折点
	int   ipProfileType[8];           //8个谱的类型（加速orEu）
	int   iTableDisPlay;              //谱显示（加速度谱/速度谱/位移谱）

	int ipChannelType[CHANNEL];       //通道类型
	int ipChannelCheck[CHANNEL];      //自检
	int ipRecogMode[CHANNEL];         //识别模式
	float fpWeighting[CHANNEL];       //权重
	int ipSenserType[CHANNEL];        //传感器类型
	int ipCouplingType[CHANNEL];      //耦合方式
	float fpSensitivity[CHANNEL];     //灵敏度
	float fpSenserUnit[CHANNEL];      //单位
	float fpSenserRange[CHANNEL];     //量程
	int ipChannelAbort[CHANNEL];      //限值中断
	float fpChannellimit[CHANNEL];    //通道限值
	int ipReferType[CHANNEL];         //限制谱

	float fMaxShakerInput;      //功放最大输入
	float fMaxShakerD;          //振动台最大位移
	float fMaxShakerV;          //振动台最大速度
	float fMaxShakerSineA;      //振动台最大正弦加速度
	float fMaxShakerRandA;      //振动台最大随机加速度
	float fMaxShakerShockA;     //振动台最大冲击加速度
	float fMaxShakerSineF;      //振动台最大正弦推力
	float fMaxShakerRandF;      //振动台最大随机推力
	float fMaxShakerShockF;     //振动台最大冲击推力

	float fLevelupRate;          //量级抬升速率
	float fLeveldownRate;        //量级下降速率
	float fLevelupOffset;        //量级抬升偏置
	float fLeveldownOffset;      //量级下降偏置

	float fNoiseThreshold;      //噪声检查阈值
	float fLoopCheckFreq;       //闭环检查频率
	float fLoopCheckLevel;      //闭环通过量级
	float fLoopCheckLimit;      //闭环最大驱动

	float fCSLLevel;            //控制信号丢失
	float fMaxDrive;            //最大驱动电压
	int   iAbortLoops;          //中断条件

}InputPara;

InputPara SInputPara;

typedef struct InputCmd
{
	int iExit;
	int iRunMode;
	int iCmd;
	int iTestCase;
	int iScheduleAdd;
	int iSweepDirect;
	float fTestlevel;
	float fSweepRate;
	float fCtrRadio;
	int   iCmdCount;
}InputCmd;

InputCmd SBufCmd = {
	0,//int iExit;
	0,//int iRunMode;
	0,//int iCmd;
	1,//int iTestCase;
	0,//int iScheduleAdd;
	1,//int iSweepDirect;
	0.03f,//float fTestlevel;
	0.0f,//float fSweepRate;
	1.0f,//float fCtrRadio;
	0//int   iCmdCount;
};
InputCmd SInputCmd = {
	0,//int iExit;
	0,//int iCmd;
	0,//int iTestCase;
	0,//int iScheduleAdd;
	1,//int iSweepDirect;
	0.03f,//float fTestlevel;
	0.0f,//float fSweepRate;
	1.0f,//float fCtrRadio;
	0//int   iCmdCount;
};
typedef struct UpdataPara
{
	int iCmd;
	int iRunMode;
	int iSweepDirect;
	float fTestlevel;
	float fSweepRate;
	float fCtrRadio;
	int iTestCase;
	int iStopReason;
	int iScheduleAdd;
	float fChannelNoise;
	int iNoiseResult;
	int iLoopResult;
	int iAlarmorNot;
	int iAbortorNot; 
	int iCSLorNot;
	int iCSLResult;
	int iChannelAbort;
	int iChannelAbortResult;
	int iEffectCh;
	int iLimitChannelEnable;
	int iDriveCliped;
	int iDataCliped;
}UpdataPara;

typedef struct UpdataData
{
	float fFreq;
	float fCtrRefer;
	float fCtrResp;
	float fCtrDrive;
	float fpFourPhase[8];
	float fpFourResp[8];
	float fpRMSResp[8];
	float fpPeakResp[8];

}UpdataData;


float dBtoLevel(float dB);  //dB转实际值

float LevetodB(float Level); //实际值转dB

void LevelRate(float *fRate, float *fOffset, float RealRate, float LowT, float HighT);  //等效量级速度计算

void InterpPoints(float *fpX, float *fpY, float *fpx, float *fpy, int iXLenth, int ixLenth); //多点插值函数

float OcttoRate(float Oct, float fSamplingtime);//扫描率转频率实际速度

float RatetoOct(float Rate, float fSamplingRate);//频率实际速度转扫描率

float Interp(float x1, float y1, float x2, float y2, float x);//单点插值函数

void SettoInput(InputPara *SInputPara, SetPara *SSetPara);//设置参数—下发参数转换函数

UpdataPara SUpdataPara;
UpdataData SUpdataData;
double x;