#pragma once
#include<math.h>

/**< IIR ���ײ����˲���ֱ��II��ʵ�� */
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
	double *m_pW; // ��ʱ�õĴ洢��
	double a1;  // oneof iir filter coefficient  bCoeff=1/(1+tan(deltaw/2))
	double a2;  // in radius
	double gainCoeff;
	double bCoeff;
	const int num_order=2; // order for Numerator
	const int den_order=2; // order for Denominator
	const double pi = 3.1416;
	int m_N; // ��ʱ�ô洢���ĸ���
};



para2ndIIR::para2ndIIR() // for initial purpose
{
	//ctor  
	m_pNum = NULL;
	m_pDen = NULL;
	m_pW = NULL;
	m_N = 0;
};

/** \brief ���˲������ڲ�״̬���㣬�˲�����ϵ������
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
* \param num ���Ӷ���ʽ��ϵ������������,num[0] Ϊ������
* \param m ���Ӷ���ʽ�Ľ���
* \param den ��ĸ����ʽ��ϵ������������,den[0] Ϊ������
* \param m ��ĸ����ʽ�Ľ���
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


/** \brief �˲�����������ֱ��II�ͽṹ
*
* \param data ��������
* \return �˲���Ľ��
*/
double para2ndIIR::filter(double data)
{
	m_pW[0] = data;  // �� class IIR_II���Ѷ���Ϊdoubleָ�룬���Ե��ã����ڴ洢input x
	for (int i = 1; i <= den_order; i++) // �ȸ��� w[n] ��״̬  
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

