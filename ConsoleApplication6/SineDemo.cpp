// ConsoleApplication5.cpp : 定义控制台应用程序的入口点。
//
#include "SineDemo.h"

int _tmain(int argc, _TCHAR* argv[])

{
	int j;
	int i;
	int iTestCase;

	float fpData[CHANNEL];
	float fDriveSignal;
	long long int PreSampleNums;
	long long int SampleNums;

	float fFreq;
	float fPhase;
	float fColaSin;
	float fColaCos;	

	float fpChannelResp[CHANNEL];

	int ipAcquCh[CHANNEL];
	int iAcquChLen;

	float fpNoiseLevel[CHANNEL];
	float fMaxNoise;
	int iNoiseResult;

	float fFreqRate;
	float iSweepDitect;

	int iScheduleAdd;
	float fpScheduleRate[100];
	float fpFreqGoal1[100];
	float fpFreqGoal2[100];
	long long int llpTimeGoal[100];
    long long int llpDwellTime[100];
	int iGetEnd;
	int iGetEnd1;
	int iGetEnd2;
	int iGetEnd3;

	float fTempPhase;
	int iZeroPass;
	int iRefresh;
	int iCtrCycles;
	int iPassTimes;
	float fLowFreq;
	float fHighFreq;
	float fLowTime;
	float fHighTime;
	int iMidCycles;
	float fCtrRadio;



	float fSumX[CHANNEL];
	float fSumX2[CHANNEL];
	float fResp[CHANNEL];
	float F_CycleLen;


	while (SInputCmd.iExit != 0)
	{
		switch (iTestCase)
		{
		case 0:
			while (iTestCase == 0)
			{
				iTestCase = SBufCmd.iTestCase;
			}
			break;
		case 1:
			fMaxNoise = SInitPara.fNoiseThreshold;
			break;
		case 2:
			while (iTestCase == 2)
			{
				iTestCase = SBufCmd.iTestCase;
			}
			break;
		case 3:
			fDriveSignal = NOTZERO;
			iNoiseResult = 0;

			for (j = 0; j < NOISECHECKTIME; j += 8)
			{
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};
				for (i = 0; i < iAcquChLen; i++)
				{
					fSumX[ipAcquCh[i]] += fpData[ipAcquCh[i]];
					fSumX2[ipAcquCh[i]] += fpData[ipAcquCh[i]] * fpData[ipAcquCh[i]];
				}
			}

			for (i = 0; i < iAcquChLen; i++)
			{
				fpChannelResp[ipAcquCh[i]] = sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / NOISECHECKTIME) / NOISECHECKTIME * 2);
				fSumX2[ipAcquCh[i]] = 0;
				fSumX[ipAcquCh[i]] = 0;
				fpNoiseLevel[ipAcquCh[i]] = fpChannelResp[ipAcquCh[i]];
				iNoiseResult |= ((fpChannelResp[ipAcquCh[i]]>fMaxNoise) << (ipAcquCh[i]));
			}

			if (iNoiseResult > 0)
			{														//噪声结果大于0，则有通道噪声检查不通过
				SInputCmd.iTestCase = 7;							//噪声检查不成功
			}
			else
			{
				SInputCmd.iTestCase = 4;							//噪声检查成功
			}
			printf("流程=%d，噪声结果=&d,噪声量级=%f", iTestCase,iNoiseResult,iNoiseResult);
			break;

		case 4:
			fDriveSignal = NOTZERO;

			while (iTestCase == 4)
			{
				SampleNums++;
				for (i = 0; i < CHANNEL; i++)
				{
					fpData[i] = fDriveSignal;
				};

				fFreqRate = iSweepDitect * fpScheduleRate[iScheduleAdd];
				fFreq += (fFreqRate)* (fFreq);															//频率累乘的结果
				llpDwellTime[iScheduleAdd] += (fFreqRate == 0);											//驻留时间累加
				iGetEnd1 = (fFreq < fpFreqGoal1[iScheduleAdd]);											//当前频率是否小于下限
				iGetEnd2 = (fFreq > fpFreqGoal2[iScheduleAdd]);											//当前频率是否大于上限
				iGetEnd3 = (llpDwellTime[iScheduleAdd] >= llpTimeGoal[iScheduleAdd]);					//驻留时间到达时间目标
				fFreq = iGetEnd1 ? fpFreqGoal1[iScheduleAdd] : fFreq;									//达到目标1，频率=目标；未达到，频率=累乘结果
				fFreq = iGetEnd2 ? fpFreqGoal2[iScheduleAdd] : fFreq;									//达到目标2，频率=目标；未达到，频率=累乘结果
				iGetEnd = (iGetEnd1 | iGetEnd2 | iGetEnd3);
				iScheduleAdd += iGetEnd;																//达到目标，计划表地址+1；未达到，+0


				
				fPhase += fFreq / SAMPINGRATE;
				iZeroPass = (fPhase > 1);
				fTempPhase = fPhase - 1;
				fPhase = iZeroPass ? fTempPhase : fPhase;



				fColaSin = sinf(fPhase);
				fColaCos = cosf(fPhase);



				if (iZeroPass == 1){
					iPassTimes++;
					if (fFreq <= fLowFreq)
						iCtrCycles = ceilf(fLowTime * fFreq);
					else if (fFreq <= fHighFreq)
						iCtrCycles = iMidCycles;
					else iCtrCycles = ceilf(fHighTime * fFreq);
					if (iPassTimes >= (iCtrCycles * fCtrRadio))
					{
						iRefresh = 1;
						iPassTimes = 0;
					}
				}
				else iRefresh = 0;



				for (i = 0; i < iAcquChLen; i++){
					fSumX[ipAcquCh[i]] += fpData[ipAcquCh[i]];
					fSumX2[ipAcquCh[i]] += fpData[ipAcquCh[i]] * fpData[ipAcquCh[i]];
				}

				if (iRefresh == 1){
					F_CycleLen = SampleNums - PreSampleNums;
					PreSampleNums = SampleNums;

					for (i = 0; i < iAcquChLen; i++){
						fpChannelResp[ipAcquCh[i]]= sqrtf((fSumX2[ipAcquCh[i]] - fSumX[ipAcquCh[i]] * fSumX[ipAcquCh[i]] / F_CycleLen) / F_CycleLen * 2);
						fSumX2[ipAcquCh[i]] = 0;
						fSumX[ipAcquCh[i]] = 0;
					}
				}








			}



			







			break;
		case 5:;
			break;
		case 6:
			break;
		case 7:
			break;
		}
	}
	return 0;
}




void FreqBulid(float *fpFreq, int *ipScheduleAdd, long long int *llpDwellTime, float fRate, float fFreqGoal1, float fFreqGoal2, long long int llTimesGoal)
{
	int iGetEnd1 = 0;
	int iGetEnd2 = 0;
	int iGetEnd3 = 0;
	*fpFreq += (fRate)* (*fpFreq);								//频率累乘的结果
	*llpDwellTime += (fRate == 0);								//驻留时间累加
	iGetEnd1 = (*fpFreq < fFreqGoal1);							//当前频率是否小于下限
	iGetEnd2 = (*fpFreq > fFreqGoal2);							//当前频率是否大于上限
	iGetEnd3 = (*llpDwellTime >= llTimesGoal);					//驻留时间到达时间目标
	*fpFreq = iGetEnd1 ? fFreqGoal1 : *fpFreq;					//达到目标，频率=目标；未达到，频率=累乘结果
	*fpFreq = iGetEnd2 ? fFreqGoal2 : *fpFreq;					//达到目标，频率=目标；未达到，频率=累乘结果
	*ipScheduleAdd += (iGetEnd1 | iGetEnd2 | iGetEnd3);			//达到目标，计划表地址+1；未达到，+0
}

void PhaseSum(float *fPhase, int *iZeroPass, float fFreq)
{
	float fTempPhase;
	*fPhase += fFreq / SAMPINGRATE;
	*iZeroPass = (*fPhase > 1);
	fTempPhase = *fPhase - 1;
	*fPhase = *iZeroPass ? fTempPhase : *fPhase;
}

void UpdataHappen(int *ipRefresh, float *fpFreq, int *ipZeroPass, float fT1, float fT2, int iN, float fF1, float fF2)
{
	int iCtrlCycles;
	static int iPassTimes = 0;
	if (*ipZeroPass == 1){
		(iPassTimes)++;
		if (*fpFreq <= fF1)
			iCtrlCycles = ceilf((fT1)* (*fpFreq));
		else if (*fpFreq <= fF2)
			iCtrlCycles = iN;
		else iCtrlCycles = ceilf((fT2)* (*fpFreq));
		if (iPassTimes >= iCtrlCycles){
			*ipRefresh = 1;
			iPassTimes = 0;
		}
	}
	else *ipRefresh = 0;
}




void Reference(float *ChannelRefer, float F_Freq, float F_ReferGain, float F_Rate, float(*ReferAmp)[2048], float(*ReferFreq)[2048], int ReferType[], int FreqMark[], int *ActiveCh, int ActiveLen)
{
	int i;
	for (i = 0; i < ActiveLen; i++){

		while ((F_Freq > ReferFreq[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]]]) && (F_Rate > 0)){
			(FreqMark[ActiveCh[i]])++;
		}
		while ((F_Freq < ReferFreq[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]] - 1]) && (F_Rate < 0)){
			(FreqMark[ActiveCh[i]])--;
		}
		ChannelRefer[ActiveCh[i]] = ReferAmp[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]]] - (ReferAmp[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]]] - ReferAmp[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]] - 1])\
			*(ReferFreq[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]]] - F_Freq) / (ReferFreq[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]]] - ReferFreq[ReferType[ActiveCh[i]]][FreqMark[ActiveCh[i]] - 1]);
		ChannelRefer[ActiveCh[i]] *= F_ReferGain;
	}
}

void FourRecognize(float fpFourResp[], float fpFourPhase[], float fpData[], float fPhase, float fFreq, long long int llSampleNums, int iRefresh, float fColaSin, float fColaCos, int ipActiveCh[], int iActiveLen)
{
	int i;
	float fCycleLen = 0;
	float fCompLen = 1;
	float fpDataRe[CHANNEL] = { 0 };
	float fpDataIm[CHANNEL] = { 0 };
	float fpCompAreaRe[CHANNEL] = { 0 };
	float fpCompAreaIm[CHANNEL] = { 0 };
	float fpCompDataRe[CHANNEL] = { 0 };
	float fpCompDataIm[CHANNEL] = { 0 };
	float fpRespRe[CHANNEL] = { 0 };
	float fpRespIm[CHANNEL] = { 0 };

	static long long int llPreSampleNums = 0;
	static float fpSumRe[CHANNEL] = { 0 };
	static float fpSumIm[CHANNEL] = { 0 };
	static float fpPreDataRe[CHANNEL] = { 0 };
	static float fpPreDataIm[CHANNEL] = { 0 };
	static float fpLastDataRe[CHANNEL] = { 0 };
	static float fpLastDataIm[CHANNEL] = { 0 };
	static float fpPreCompAreaRe[CHANNEL] = { 0 };
	static float fpPreCompAreaIm[CHANNEL] = { 0 };
	static float fPreCompLen = 0;

	for (i = 0; i < iActiveLen; i++){
		fpDataRe[ipActiveCh[i]] = fpData[ipActiveCh[i]] * fColaCos;
		fpDataIm[ipActiveCh[i]] = fpData[ipActiveCh[i]] * fColaSin;
		fpSumRe[ipActiveCh[i]] += fpDataRe[ipActiveCh[i]];
		fpSumIm[ipActiveCh[i]] += fpDataIm[ipActiveCh[i]];
		fpLastDataRe[ipActiveCh[i]] = fpDataRe[ipActiveCh[i]];
		fpLastDataIm[ipActiveCh[i]] = fpDataIm[ipActiveCh[i]];
	}

	if (iRefresh == 1){
		fCompLen = SAMPINGRATE * (fPhase) / (fFreq);
		fCycleLen = (float)llSampleNums - (float)llPreSampleNums + fPreCompLen - fCompLen;
		llPreSampleNums = llSampleNums;
		fPreCompLen = fCompLen;
		for (i = 0; i < iActiveLen; i++){
			fpCompDataRe[ipActiveCh[i]] = fpDataRe[ipActiveCh[i]] - (fpDataRe[ipActiveCh[i]] - fpLastDataRe[ipActiveCh[i]]) * fCompLen;
			fpCompDataIm[ipActiveCh[i]] = fpDataIm[ipActiveCh[i]] - (fpDataIm[ipActiveCh[i]] - fpLastDataIm[ipActiveCh[i]]) * fCompLen;
			fpCompAreaRe[ipActiveCh[i]] = (fpCompDataRe[ipActiveCh[i]] + fpDataRe[ipActiveCh[i]]);
			fpCompAreaIm[ipActiveCh[i]] = (fpCompDataIm[ipActiveCh[i]] + fpDataIm[ipActiveCh[i]]);
			fpRespRe[ipActiveCh[i]] = (fpSumRe[ipActiveCh[i]] * 2 + fpPreCompAreaRe[ipActiveCh[i]] - fpCompAreaRe[ipActiveCh[i]] - fpDataRe[ipActiveCh[i]] + fpPreDataRe[ipActiveCh[i]]) / fCycleLen;
			fpRespIm[ipActiveCh[i]] = (fpSumIm[ipActiveCh[i]] * 2 + fpPreCompAreaIm[ipActiveCh[i]] - fpCompAreaIm[ipActiveCh[i]] - fpDataIm[ipActiveCh[i]] + fpPreDataIm[ipActiveCh[i]]) / fCycleLen;

			fpFourResp[ipActiveCh[i]] = sqrtf(fpRespRe[ipActiveCh[i]] * fpRespRe[ipActiveCh[i]] + fpRespIm[ipActiveCh[i]] * fpRespIm[ipActiveCh[i]]);
			fpFourPhase[ipActiveCh[i]] = atanf(fpRespRe[ipActiveCh[i]] / fpRespIm[ipActiveCh[i]]);

			fpPreCompAreaRe[ipActiveCh[i]] = fpCompAreaRe[ipActiveCh[i]];
			fpPreCompAreaIm[ipActiveCh[i]] = fpCompAreaIm[ipActiveCh[i]];
			fpPreDataRe[ipActiveCh[i]] = fpDataRe[ipActiveCh[i]];
			fpPreDataIm[ipActiveCh[i]] = fpDataIm[ipActiveCh[i]];
			fpSumRe[ipActiveCh[i]] = 0;
			fpSumIm[ipActiveCh[i]] = 0;
		}
	}
	//printf("\n%f,%d,%f,%f,%f,%f", F_DataRe[0], *D_Refresh, F_FourResp[0], F_RespRe[0], F_RespIm[0], F_CycleLen);
}
void RMSRecognize(float *F_RMSResp, float *F_Data, float *F_SumX, float *F_SumX2, int D_Refresh, float F_SampleNums, float F_PreSampleNums, int *ActiveCh, int ActiveLen)
{
	int i;
	float F_CycleLen;

	for (i = 0; i < ActiveLen; i++){
		F_SumX[ActiveCh[i]] += F_Data[ActiveCh[i]];
		F_SumX2[ActiveCh[i]] += F_Data[ActiveCh[i]] * F_Data[ActiveCh[i]];
	}
	if (D_Refresh == 1){
		F_CycleLen = F_SampleNums - F_PreSampleNums;
		for (i = 0; i < ActiveLen; i++){
			F_RMSResp[ActiveCh[i]] = sqrtf((F_SumX2[ActiveCh[i]] - F_SumX[ActiveCh[i]] * F_SumX[ActiveCh[i]] / F_CycleLen) / F_CycleLen * 2);
			F_SumX2[ActiveCh[i]] = 0;
			F_SumX[ActiveCh[i]] = 0;
		}
	}
}
void PeakRecongnize(float *F_PeakResp, float *F_Data, float *F_MaxX, float *F_MinX, int D_Refresh, int *ActiveCh, int ActiveLen)
{
	int i;
	for (i = 0; i < ActiveLen; i++){
		F_MaxX[ActiveCh[i]] = F_MaxX[ActiveCh[i]] >= F_Data[ActiveCh[i]] ? F_MaxX[ActiveCh[i]] : F_Data[ActiveCh[i]];
		F_MinX[ActiveCh[i]] = F_MinX[ActiveCh[i]] <= F_Data[ActiveCh[i]] ? F_MinX[ActiveCh[i]] : F_Data[ActiveCh[i]];
	}
	if (D_Refresh == 1){
		for (i = 0; i < ActiveLen; i++){
			F_PeakResp[ActiveCh[i]] = (F_MaxX[ActiveCh[i]] - F_MinX[ActiveCh[i]]) / 2;
			F_MaxX[ActiveCh[i]] = 0;
			F_MinX[ActiveCh[i]] = 10000;
		}
	}

}
void SampleCount(float *F_SampleNums, float *F_PreSampleNums)
{
	*F_PreSampleNums = *F_SampleNums;
}
void RespChoose(float *F_ChannelResp, float *F_RecogResp, int D_RecongMode[], int *ActiveCh, int ActiveLen)
{
	int i;
	for (i = 0; i < ActiveLen; i++){
		F_ChannelResp[ActiveCh[i]] = F_RecogResp[CHANNEL*D_RecongMode[ActiveCh[i]] + ActiveCh[i]];
	}
}
void RespWeight(float *F_ChannelResp, float F_ChannelWeight[], int Mode, float F_WeightSum, int *ActiveCh, int ActiveLen)
{
	int i;
	float WeightResp[3] = { 0, 0, 10000 };																			//数组元素{平均值，最大值，最小值}
	for (i = 0; i < ActiveLen; i++){																				//所有控制通道
		WeightResp[0] += F_ChannelResp[ActiveCh[i]] * F_ChannelWeight[ActiveCh[i]];									//乘以权重再累加
		WeightResp[1] = WeightResp[1] >= F_ChannelResp[ActiveCh[i]] ? WeightResp[1] : F_ChannelResp[ActiveCh[i]];	//与已有最大值比较，取较大者为最大值
		WeightResp[2] = WeightResp[2] >= F_ChannelResp[ActiveCh[i]] ? WeightResp[2] : F_ChannelResp[ActiveCh[i]];	//与已有最小值比较，取较小者为最小值
	}
	WeightResp[0] = WeightResp[0] / F_WeightSum;																	//累加和除以权重和，得加权平均值
	F_ChannelResp[CHANNEL] = WeightResp[Mode];																		//根据加权模式，选择加权结果{平均值or最大值or最小值}
	WeightResp[0] = 0;
	WeightResp[1] = 0;
	WeightResp[2] = 10000;
}
void LevelChange(float *F_Gain, float *F_Goal, float *F_Rate, float *offset, int *Add, int AddMove, float *F_time)
{
	float F_TempGain;
	int LevelChanging;
	F_TempGain = ((*F_Rate) * (*F_time) + *offset) * (*F_Gain);			//计算当前控制周期的驱动抬升倍数*原增益（速度曲线拟合公式：k*控制周期数/当前频率+偏置）
	LevelChanging = (F_TempGain < *F_Goal) && (F_TempGain > *F_Gain);	//参考谱增益是否达到预定目标,前后两次与目标值的比较异号，则达到目标
	*F_Gain = LevelChanging ? F_TempGain : (*F_Goal);					//参考谱增益达到预定目标，增益=目标；参考谱增益未达到预定目标，增益=计算结果
	*F_Rate = LevelChanging ? *F_Rate : 0;								//到达量级后，速度变为0，保持当前增益
	*offset = LevelChanging ? *offset : 1;								//到达量级后，偏置变为1，保持当前增益
	*Add += (LevelChanging && AddMove);									//到达量级后，计划表地址根据设置改变
}
void ControlLost(int *flag, int *Result, float SystemGain[], float Num, float Resp[], float Drive, float Safebox1, float Safebox2, int Enable, int ActiveCh[], int ActiveLen)
{
	int i = 0;
	int j = 0;
	float TempSystemGain[CHANNEL];
	float DevGain;
	*Result = 0;																		//结果清零
	for (i = 0; i < ActiveLen; i++){													//所有控制通道+限制通道
		TempSystemGain[ActiveCh[i]] = Resp[ActiveCh[i]] / Drive;						//当前系统增益计算（响应/驱动）
		DevGain = abs(TempSystemGain[ActiveCh[i]] - SystemGain[ActiveCh[i]]) / Num;		//当前系统增益的导数计算
		SystemGain[ActiveCh[i]] = TempSystemGain[ActiveCh[i]];							//保存当前系统增益
		j = (Resp[ActiveCh[i]] / Drive< Safebox1);										//系统增益（响应/驱动）是否超标
		j &= (DevGain<Safebox2);														//系统增益的一阶导（响应/驱动）是否超标
		*Result |= (j << ActiveCh[i]);													//超标是,通道对应位,置1
	}
	*flag = (*Result > 0) && Enable;													//结果大于0有通道控制信号丢失，功能激活时，开环标志置1，未激活时，开环标志置0
}
void AlarmAbort(int *flag, int *Result, float *Resp, float Refer, float Highbox, float Lowbox, float Enable, int ActiveCh[], int ActiveLen)
{
	int i = 0;
	int j = 0;
	int k = 0;
	*Result = 0;																		//结果清零
	for (i = 0; i < ActiveLen; i++){													//所有控制通道+限制通道
		j = (Resp[ActiveCh[i]] / Refer> Highbox);										//量级误差是否上限
		k = (Resp[ActiveCh[i]] / Refer< Lowbox) && Enable;								//量级误差是否下限，下限可被禁用
		j |= k;																			//上下限判断结果相与
		*Result |= (j << ActiveCh[i]);													//量级误差超限,通道对应位,置1
	}
	*flag = *Result > 0;																//结果大于0有通道控制信号超限
}
void DriveUpdata(float F_FilterDrive[], float F_Refer[], float F_Resp[], float F_Drive, float F_PreCalDrive[], int D_ActiveCh[], int D_ActiveLen)
{
	int i;
	float F_CalDrive[CHANNEL + 1];
	for (i = 0; i < D_ActiveLen; i++){
		F_CalDrive[D_ActiveCh[i]] = F_Drive / F_Resp[D_ActiveCh[i]] * F_Refer[D_ActiveCh[i]];
		F_CalDrive[D_ActiveCh[i]] = (F_CalDrive[D_ActiveCh[i]]>NOTZERO ? F_CalDrive[D_ActiveCh[i]] : NOTZERO);
		F_FilterDrive[D_ActiveCh[i]] = sqrtf(F_CalDrive[D_ActiveCh[i]] * F_PreCalDrive[D_ActiveCh[i]]);
		F_FilterDrive[D_ActiveCh[i]] = (F_FilterDrive[D_ActiveCh[i]]>NOTZERO ? F_CalDrive[D_ActiveCh[i]] : NOTZERO);
		F_PreCalDrive[D_ActiveCh[i]] = F_CalDrive[D_ActiveCh[i]];
	}
}
void DriveChoose(float *F_Drive, int *LimitEnable, int *EffectCh, float F_FilterDrive[], float F_SettingDrive, float LevelGain, int D_DriveMode, int *D_ActiveCh, int D_ActiveLen)
{
	int i;
	float F_CalDrive[CHANNEL + 1];
	for (i = 0; i < D_ActiveLen; i++){
		*EffectCh = (F_FilterDrive[D_ActiveCh[i]] < F_FilterDrive[*EffectCh] ? D_ActiveCh[i] : *EffectCh);
	}
	F_SettingDrive = *F_Drive * LevelGain;
	*F_Drive = F_FilterDrive[*EffectCh];
	*LimitEnable = (*EffectCh == CHANNEL) ? 0 : 1;
	*F_Drive = (D_DriveMode == 1) ? F_SettingDrive : *F_Drive;

}
