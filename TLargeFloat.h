#ifndef _TLARGE_FLOAT_H__INCLUDED_
#define _TLARGE_FLOAT_H__INCLUDED_


#include  <vector>
#include  <sstream>
#include  <string>
#include  <Exception>
#include  <limits>


class TLargeFloat;//���߾��ȸ�������TLargeFloat
class TLargeFloatException;// ���߾��ȸ������쳣��

// �Ľ�����
//   1.ǿ���Ż�ArrayMUL���������(��ǰʵ���˶��ַ���FFT�㷨)�� 
//      a.��ʵ������ż��Ϊ�������и���Ҷ�任���㷨ʵ�֣��ӿ�˷��ٶ�
//      b.ʵ�ֻ�ϻ��ĸ���Ҷ�任���ӿ�˷��ٶ�
//      c.������x87��10byte������ʵ��FFT�Լ�С���Ӷ�����FFT�ܹ���������λ������
//      d.��SSE2���Ż����ٸ���Ҷ�任���ӿ�˷��ٶ�  
//      e.���߽�����Ҷ�任�滻Ϊ���۱任��ʵ��(ʹ������)
//   2.�ڲ�ʹ��8λ(��9λ)ʮ������ʵ�֣���Լ�ڴ棻����2���Ƶĵ���(�����Ļ�����������ͻ��鷳һЩ��)
//   3.����µĻ������㺯�����磺ָ������power����������log�����Ǻ���sin,cos,tan��

// ע�⣺�����������TLargeFloat���л������, ���ܻ�������(��Чλ�����ܵ�������Ӱ��)��
//   ���� �� Ϊ�ɱ�ʾ�����ĸ����� �������㲻�������
//   ���ںܴ������������С��λ�ĸ��������������ַ�������ʽת��Ϊ���߾��ȸ�����(�����������)

//void ArrayMUL(TInt32bit* result,long rminsize,const TInt32bit* x,long xsize,const TInt32bit* y,long ysize);// �����, ��Ҫ�Ż�����ҪĿ��

//���߾��ȸ������쳣��  //TLargeFloat�����г��ִ����ʱ����׳������͵��쳣
class TLargeFloatException :public  std::runtime_error
{
public:
    TLargeFloatException(const char* Error_Msg) :runtime_error(Error_Msg) {}
    virtual ~TLargeFloatException() throw () {}
};


// TCatchIntErrorֻ�Ƕ���������TInt���е�һ���װ
// ���߾��ȸ��������ָ������ʱʹ��
// ���TCatchIntError��Ϊ�˵��������㳬��ֵ���ʱ���׳��쳣
//template<Ҫ��װ���������ͣ�����ʱ�׳����쳣���ͣ�TInt��Сֵ��TInt���ֵ>
template <typename TInt, typename TException, TInt MinValue, TInt MaxValue>
class  TCatchIntError
{
private:
    typedef TCatchIntError<TInt, TException, MinValue, MaxValue>  SelfType;
    TInt  m_Int;

    inline SelfType& inc(const TInt& uValue)
    {
        if ((uValue < 0) || (uValue > MaxValue) || (MaxValue - uValue < m_Int))
            throw TException("ERROR:TCatchIntError::inc(); ");
        m_Int += uValue;
        return (*this);
    }
    inline SelfType& dec(const TInt& uValue)
    {
        if ((uValue < 0) || (uValue > MaxValue) || (MinValue + uValue > m_Int))
            throw TException("ERROR:TCatchIntError::dec()");
        m_Int -= uValue;
        return (*this);
    }
    inline SelfType& mul(const TInt& iValue)
    {
        if (iValue == 0)
            m_Int = 0;
        else
        {
            TInt tmp = m_Int * iValue;
            if ((iValue < MinValue) || (iValue > MaxValue) || (tmp < MinValue) || (tmp > MaxValue) || ((tmp / iValue) != m_Int))
                throw TException("ERROR:TCatchIntError::mul(); ");
            m_Int = tmp;
        }
        return (*this);
    }
public:
    inline TCatchIntError() :m_Int(0) { }
    inline TCatchIntError(const TInt& Value) : m_Int(0) { (*this) += Value; }
    inline TCatchIntError(const SelfType& Value) : m_Int(0) { (*this) += (Value.m_Int); }
    inline const TInt& AsInt() const { return  m_Int; }
    inline SelfType& operator +=(const TInt& Value) //throw(TLargeFloatException)
    {
        if (Value < 0) return dec(-Value);
        else return  inc(Value);
    }
    inline SelfType& operator -=(const TInt& Value) //throw(TLargeFloatException)
    {
        if (Value < 0)  return inc(-Value);
        else return  dec(Value);
    }
    inline SelfType& operator *=(const TInt& Value) //throw(TLargeFloatException)
    {
        return  mul(Value);
    }

    inline SelfType& operator +=(const SelfType& Value) { return (*this) += (Value.m_Int); }//throw(TLargeFloatException)
    inline SelfType& operator -=(const SelfType& Value) { return (*this) -= (Value.m_Int); }//throw(TLargeFloatException)
    inline SelfType& operator *=(const SelfType& Value) { return (*this) *= (Value.m_Int); }//throw(TLargeFloatException)
};

// ָ������ʹ�õ���������  ��д������֧�ֵĽϴ���������� 
//typedef __int64 TMaxInt; // type long long TMaxInt;   
// const TMaxInt   TMaxInt_MAX_VALUE =   TMaxInt(9223372036854775807);
//const TMaxInt   TMaxInt_MIN_VALUE = - TMaxInt_MAX_VALUE;
typedef long   TMaxInt;
const TMaxInt  TMaxInt_MAX_VALUE = 2147483647;
const TMaxInt  TMaxInt_MIN_VALUE = -TMaxInt_MAX_VALUE;

//С������ʹ�õ���������  32bitλ���������� 
typedef long  TInt32bit;
const TInt32bit  TInt32bit_MAX_VALUE = 2147483647;
const TInt32bit  TInt32bit_MIN_VALUE = -TInt32bit_MAX_VALUE;


//���߾��ȸ�������
class  TLargeFloat
{
public:
    //һЩ���������Ͷ���
    typedef std::vector<TInt32bit> TArray;//С��λʹ�õ���������
    enum {
        em10Power = 4,   //����Ϊ10000���ƣ�
        emBase = 10000,  //�����һ��Ԫ�ض�Ӧ4��ʮ����λ
        emLongDoubleDigits = std::numeric_limits<long double>::digits10,//long double��10������Ч����
        emLongDoubleMaxExponent = std::numeric_limits<long double>::max_exponent10,//long double�����10����ָ��
        emLongDoubleMinExponent = std::numeric_limits<long double>::min_exponent10
    };//long double����С10����ָ��
    typedef   TLargeFloatException   TException;
    typedef   TCatchIntError<TMaxInt, TException, TMaxInt_MIN_VALUE, TMaxInt_MAX_VALUE>  TExpInt;//ָ������
    typedef  TLargeFloat  SelfType;
public:
    class TDigits//TDigits��������TLargeFloat�ľ���;//�����������Ϊ�˱���TLargeFloat�Ĺ��캯���Ŀ�������
    {
    private:
        unsigned long   m_DigitsArraySize;
    public:
        explicit TDigits(const long  uiDigitsLength)
        {
            if (uiDigitsLength <= 0) throw TException("ERROR:TLargeFloat::TDigits()");
            m_DigitsArraySize = (uiDigitsLength + em10Power - 1) / em10Power;
        }
        inline const unsigned long  GetDigitsArraySize() const { return  m_DigitsArraySize; }
    };
    TLargeFloat() {}
    TLargeFloat(const SelfType& Value);
    explicit TLargeFloat(const long double DefultValue);//Ĭ�ϸ��㾫��  //ע�⣺ת�����ܴ���С�����
    explicit TLargeFloat(const long double DefultValue, const TDigits& DigitsLength);//TDigits ʮ������Чλ�� //ע�⣺ת�����ܴ���С�����
    explicit TLargeFloat(const char* strValue);//ʹ���ַ�������ľ���
    explicit TLargeFloat(const char* strValue, const TDigits& DigitsLength);
    explicit TLargeFloat(const std::string& strValue);
    explicit TLargeFloat(const std::string& strValue, const TDigits& DigitsLength);
    long double AsFloat() const;//ת��Ϊ������
    std::string  AsString() const;//ת��Ϊ�ַ���
    void SetDigitsLength(const TDigits& DigitsLength);  //��������10������Чλ�� 
    inline void SetDigitsLength(const long  uiDigitsLength) { SetDigitsLength(TDigits(uiDigitsLength)); }
    unsigned long GetDigitsLength() const;//���ص�ǰ��10������Чλ��

    void Swap(SelfType& Value);//����ֵ
    void Zero(); //����Ϊ0

    inline  void StrToLargeFloat(const std::string& strValue) { sToLargeFloat(strValue); }
    inline void StrToLargeFloat(const char* strValue) { sToLargeFloat(std::string(strValue)); }
    SelfType& operator =  (const SelfType& Value);
    SelfType& operator =  (long double  fValue); //ע�⣺ת�����ܴ���С�����  
    inline const SelfType  operator -  () const { SelfType temp(*this); temp.Chs(); return temp; }//��
    inline const SelfType& operator +  () const { return (*this); }//����

    SelfType& operator += (const SelfType& Value);
    SelfType& operator -= (const SelfType& Value);
    inline SelfType& operator += (long double  fValue) { return (*this) += TLargeFloat(fValue); }
    inline SelfType& operator -= (long double  fValue) { return (*this) -= TLargeFloat(fValue); }
    friend inline const TLargeFloat operator + (const TLargeFloat& x, const TLargeFloat& y) { TLargeFloat temp(x);  return temp += y; }
    friend inline const TLargeFloat operator - (const TLargeFloat& x, const TLargeFloat& y) { TLargeFloat temp(x);  return temp -= y; }
    friend inline const TLargeFloat operator + (const TLargeFloat& x, long double y) { TLargeFloat temp(x);  return temp += y; }
    friend inline const TLargeFloat operator - (const TLargeFloat& x, long double y) { TLargeFloat temp(x);  return temp -= y; }
    friend inline const TLargeFloat operator + (long double x, const TLargeFloat& y) { return y + x; }
    friend inline const TLargeFloat operator - (long double x, const TLargeFloat& y) { return -(y - x); }

    SelfType& operator *= (long double   fValue);
    SelfType& operator *= (const SelfType& Value);
    friend inline const TLargeFloat operator * (const TLargeFloat& x, const TLargeFloat& y) { TLargeFloat temp(x); if (&x != &y)  return temp *= y; else { temp.Sqr(); return  temp; } }
    friend inline const TLargeFloat operator * (const TLargeFloat& x, long double y) { TLargeFloat temp(x); return temp *= y; }
    friend inline const TLargeFloat operator * (long double x, const TLargeFloat& y) { return y * x; }
    SelfType& operator /= (long double   fValue);
    SelfType& operator /= (const SelfType& Value);
    friend inline const TLargeFloat operator / (const TLargeFloat& x, const TLargeFloat& y) { TLargeFloat temp(x); return temp /= y; }
    friend inline const TLargeFloat operator / (const TLargeFloat& x, long double y) { TLargeFloat temp(x); return temp /= y; }
    friend inline const TLargeFloat operator / (long double x, const TLargeFloat& y) { TLargeFloat temp(y); temp.Rev(); return temp *= x; }

    friend inline bool operator ==(const TLargeFloat& x, const TLargeFloat& y) { return (x.Compare(y) == 0); }
    friend inline bool operator < (const TLargeFloat& x, const TLargeFloat& y) { return (x.Compare(y) < 0); }
    friend inline bool operator ==(const TLargeFloat& x, long double y) { return (x == TLargeFloat(y)); }
    friend inline bool operator < (const TLargeFloat& x, long double y) { return (x < TLargeFloat(y)); }
    friend inline bool operator ==(long double x, const TLargeFloat& y) { return (y == x); }
    friend inline bool operator < (long double x, const TLargeFloat& y) { return (y > x); }
    friend inline bool operator !=(const TLargeFloat& x, const TLargeFloat& y) { return !(x == y); }
    friend inline bool operator > (const TLargeFloat& x, const TLargeFloat& y) { return (y < x); }
    friend inline bool operator >=(const TLargeFloat& x, const TLargeFloat& y) { return !(x < y); }
    friend inline bool operator <=(const TLargeFloat& x, const TLargeFloat& y) { return !(x > y); }
    friend inline bool operator !=(const TLargeFloat& x, long double y) { return !(x == y); }
    friend inline bool operator > (const TLargeFloat& x, long double y) { return (y < x); }
    friend inline bool operator >=(const TLargeFloat& x, long double y) { return !(x < y); }
    friend inline bool operator <=(const TLargeFloat& x, long double y) { return !(x > y); }
    friend inline bool operator !=(long double x, const TLargeFloat& y) { return !(x == y); }
    friend inline bool operator > (long double x, const TLargeFloat& y) { return (y < x); }
    friend inline bool operator >=(long double x, const TLargeFloat& y) { return !(x < y); }
    friend inline bool operator <=(long double x, const TLargeFloat& y) { return !(x > y); }

    friend inline const TLargeFloat abs(const TLargeFloat& x) { TLargeFloat result(x); result.Abs(); return result; }//����ֵ,|x|
    friend inline const TLargeFloat sqrt(const TLargeFloat& x) { TLargeFloat result(x); result.Sqrt(); return result; } //����,x^0.5 
    friend inline const TLargeFloat revsqrt(const TLargeFloat& x) { TLargeFloat result(x); result.RevSqrt(); return result; }//��1/x^0.5;
    friend inline const TLargeFloat sqr(const TLargeFloat& x) { TLargeFloat result(x); result.Sqr(); return result; };//ƽ��,x^2
    friend inline void swap(TLargeFloat& a, TLargeFloat& b) { a.Swap(b); }//����ֵ
    friend inline std::ostream& operator << (std::ostream& cout, const TLargeFloat& Value) { return cout << Value.AsString(); }

    ///
private:
    TInt32bit   m_Sign;     //����λ  ��:1,  ��:-1, ��: 0
    TExpInt     m_Exponent; //����10Ϊ�׵�ָ��
    TArray      m_Digits;   //С������ ����˳����TArray[0]Ϊ��һ��С��λ(em10Power��10����λ),�������ƣ�ȡֵ��Χ[0--(emBase-1)]
private:
    void Canonicity();//��� ת��ֵ���Ϸ���ʽ
    void fToLargeFloat(long double fValue);//�ڲ�ʹ�� ������ת��Ϊ TLargeFloat,������Ĭ�Ͼ���
    void iToLargeFloat(TMaxInt iValue);//�ڲ�ʹ�� ����ת��Ϊ TLargeFloat,������Ĭ�Ͼ���
    void sToLargeFloat(const std::string& strValue);//�ڲ�ʹ�� �ַ���ת��Ϊ TLargeFloat
    long Compare(const SelfType& Value) const;//�Ƚ���������(*this)>Value ����1��С�ڷ���-1����ȷ���0
    void Abs_Add(const SelfType& Value);//����ֵ��  x:=|x|+|y|;
    void Abs_Sub_Abs(const SelfType& Value);//����ֵ���ľ���ֵx:=| |x|-|y| |;

public:
    void Chs();//��
    void Abs();//����ֵ
    void MulInt(TMaxInt iValue);//����һ��������
    void DivInt(TMaxInt iValue);//����һ��������
    void Rev();//����
    void RevSqrt();//��1/x^0.5;
    inline void Sqrt() { SelfType x(*this);  x.RevSqrt(); (*this) *= x; } //��x^0.5;
    inline void Sqr() { (*this) *= (*this); };//ƽ��,x^2


};


//��Ԫ����
void  LargeFloat_UnitTest();


#endif // _TLARGE_FLOAT_H__INCLUDED_
// TLargeFloat.h