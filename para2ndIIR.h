#pragma once
#include<math.h>

/**< IIR 二阶参量滤波器直接II型实现 */
class para2ndIIR
{
public:
	para2ndIIR();
	void reset();
	void setPara(double fCenter, double QFactor,double gainEqDB,int fs);
	double filter(double data);
protected:
private:
	double *m_pNum;
	double *m_pDen;
	double *m_pW; // 延时用的存储器
	double a1;  // oneof iir filter coefficient  bCoeff=1/(1+tan(deltaw/2))
	double a2;  // in radius
	double gainCoeff;
	double bCoeff;
	const int num_order=2; // order for Numerator
	const int den_order=2; // order for Denominator
	const double pi = 3.1416;
	int m_N; // 延时用存储器的个数
};



para2ndIIR::para2ndIIR() // for initial purpose
{
	//ctor  
	m_pNum = NULL;
	m_pDen = NULL;
	m_pW = NULL;
	m_N = 0;
};

/** \brief 将滤波器的内部状态清零，滤波器的系数保留
* \return
*/
void para2ndIIR::reset()
{
	for (int i = 0; i < m_N; i++)
	{
		m_pW[i] = 0.0;
	}
}
/** \brief
*
* \param num 分子多项式的系数，升序排列,num[0] 为常数项
* \param m 分子多项式的阶数
* \param den 分母多项式的系数，升序排列,den[0] 为常数项
* \param m 分母多项式的阶数
* \return
*/
void para2ndIIR::setPara(double fCenter, double QFactor, double gainEqDB,int fs)
{
	bCoeff= 1 / (1 + tan(pi*fCenter/QFactor/fs));
	a1 =-2*bCoeff*cos(2*pi*fCenter/fs );
	a2 =2*bCoeff-1;
	gainCoeff = pow (10, gainEqDB / 20);
	delete[] m_pNum;
	delete[] m_pDen;
	delete[] m_pW;
	m_N = static_cast<int>(fmax(num_order, den_order) + 1);
	m_pNum = new double[num_order + 1];
	m_pDen = new double[den_order + 1];
	m_pW = new double[m_N];
	
	m_pNum[0] = (1+a2+gainCoeff-gainCoeff*a2)/2;
	m_pNum[1] = a1;
	m_pNum[2] = (1 + a2 - gainCoeff + gainCoeff*a2)/2;
	m_pDen[0] = 1;
	m_pDen[1] = a1;
	m_pDen[2] = a2;

	for (int i = 0; i < m_N; i++)
	{
		m_pW[i] = 0.0;
	}
	
}


/** \brief 滤波函数，采用直接II型结构
*
* \param data 输入数据
* \return 滤波后的结果
*/
double para2ndIIR::filter(double data)
{
	m_pW[0] = data;  // 在 class IIR_II中已定义为double指针，可以调用，用于存储input x
	for (int i = 1; i <= den_order; i++) // 先更新 w[n] 的状态  
	{
		m_pW[0] = m_pW[0] - m_pDen[i] * m_pW[i];
	}
	data = 0.0;  // reused for ouput
	for (int i = 0; i <= num_order; i++)
	{
		data = data + m_pNum[i] * m_pW[i];
	}
	for (int i = m_N - 1; i >= 1; i--)
	{
		m_pW[i] = m_pW[i - 1];
	}
	return data;
}

