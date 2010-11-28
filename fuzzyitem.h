#ifndef FUZZYITEM_H
#define FUZZYITEM_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

enum MTYPE { GRADE, TRAPEZOID, REVERSE, TRIANGLE, UNKNOWN };

/* Linguistic variable */
template <class T>
class FuzzyItem
{
public:
    FuzzyItem<T>(char*, MTYPE, T, ...);
    FuzzyItem<T>() {}

    ~FuzzyItem()
    {
        free(m_iParams);
    }

    MTYPE Type() { return m_eType; }
    void Update(float f) { m_fGrade = f; }

    int Max() { return m_iMax; }
    int Min() { return m_iMin; }
protected:
    float Grade() { return m_fGrade; }  
    int* m_iParams;

private:
    int m_iMax;
    int m_iMin;
    float m_fGrade;
    MTYPE m_eType;
    char m_szDescription[64];
};

template <class T>
class FuzzyTrap : public FuzzyItem<T>
{
public:
    FuzzyTrap(char* d, T x0, T x1, T x2, T x3) : FuzzyItem<T>(d, TRAPEZOID, 4, x0, x1, x2, x3) {}
    float Member (T);
    T fromMember(float, int);
};

template <class T>
class FuzzyGrade : public FuzzyItem<T>
{
public:
    FuzzyGrade(char* d, T x0, T x1) : FuzzyItem<T>(d, GRADE, 2, x0,x1) {}
    float Member(T);
    T fromMember(float, int);
};

template <class T>
class FuzzyTri : public FuzzyItem<T>
{
public:
    FuzzyTri(char* d, T x0, T x1, T x2) : FuzzyItem<T>(d, TRIANGLE, 3, x0, x1, x2) {}
    float Member(T);
    T fromMember(float, int);
};

template <class T>
class FuzzyRev : public FuzzyItem<T>
{
public:
    FuzzyRev(char* d, T x0, T x1) : FuzzyItem<T>(d, REVERSE, 2, x0, x1) {}
    float Member(T);
    T fromMember(float, int);
};


/* Instantiate new FuzzyItem with descriptor, number of arguments, etc .. */
template <class T>
FuzzyItem<T>::FuzzyItem (char* desc, MTYPE eType, T n, ...)
{
    this->m_iParams = new int[n];

    va_list ap;
    va_start(ap, n);

    int min = 0; //va_arg(ap, int);
    int max = min;

    for (int i=0; i < n; i++)
    {
        int v = va_arg(ap,int);
        m_iParams[i] = v;

        if (m_iParams[i] > max)
            max = m_iParams[i];
        if (m_iParams[i] < min || i == 0)
            min = m_iParams[i];
    }

    m_iMax = max;
    m_iMin = min;
    m_eType = eType;

    strcpy(m_szDescription, desc);

    va_end(ap);
}

template <class T>
float FuzzyGrade<T>::Member(T d)
{
    float f = .0;
    //float x0 = this->m_iParams[0], x1 = this->m_iParams[1];
    float x0 = 0, x1 = this->m_iParams[1] - this->m_iParams[0];
    d -= this->m_iParams[0];

    if (d < x0)
        f = 0.0;
    else if (d > x1)
        f = 1.0;
    else
        f = d / (x0 + x1);

    return f;
}

template <class T>
T FuzzyGrade<T>::fromMember(float f, int i)
{
    T d = this->Min();
    float x0 = this->m_iParams[0], x1 = this->m_iParams[1];

    if (d < x0)
        d = this->Min();
    else if (d > x1)
        d = this->Max();
    else
        d = f * (x0 + x1);

    return d;
}

template <class T>
T FuzzyTri<T>::fromMember(float f, int i)
{
    T d = this->Min();
    float x0 = this->m_iParams[0], x1 = this->m_iParams[1], x2 = this->m_iParams[2];

    if (d <= x0)
        d = this->Min();
    else if (d > x0 && d <= x1)
        d = f * (x0 + x1);
    else if (d > x1 && d <= x2)
        d = f * (x1 + x2);
    else
        d = this->Max();

    return d;
}


template <class T>
T FuzzyRev<T>::fromMember(float f, int i)
{
    T d = this->Min();
    T x0 = this->m_iParams[0], x1 = this->m_iParams[1];

    if (d < x0)
        d = this->Max();
    else if (d > x1)
        d = this->Min();
    else
        d = (1.0f - f) * (x0 + x1);

    return d;
}


template <class T>
T FuzzyTrap<T>::fromMember(float f, int i)
{
    T d;

    float x0 = this->m_iParams[0], x1 = this->m_iParams[1], x2 = this->m_iParams[2], x3 = this->m_iParams[3];

    if (d <= x0)
        d = this->Min();
    else if (d > x0 && d <= x1)
        d = f * (x0 + x1);
    else if (d > x1 && d <= x2)
        d = this->Max();
    else if (d > x2 && d <= x3)
        d  =  f * (x2 + x3);
    else
        d = this->Min();

    return d;
}


template <class T>
float FuzzyTrap<T>::Member(T d)
{
    float f = .0;
    //float x0 = this->m_iParams[0], x1 = this->m_iParams[1], x2 = this->m_iParams[2], x3 = this->m_iParams[3];
    float x0 = 0, x1 = this->m_iParams[1] - this->m_iParams[0], x2 = this->m_iParams[2] - this->m_iParams[0], x3 = this->m_iParams[3] - this->m_iParams[0];
    d -= this->m_iParams[0];

    if (d <= x0)
        f = 0.0;
    else if (d > x0 && d <= x1)
        f = d / (x0 + x1);
    else if (d > x1 && d <= x2)
        f = 1.0;
    else if (d > x2 && d <= x3)
        f = d / (x2 + x3);
    else
        f = 0.0;

    return f;
}

template <class T>
float FuzzyRev<T>::Member(T d)
{
    float f = .0;
    //T x0 = this->m_iParams[0], x1 = this->m_iParams[1];
    T x0 = 0, x1 = this->m_iParams[1] - this->m_iParams[0];
    d -= this->m_iParams[0];

    if (d < x0)
        f = 1.0;
    else if (d > x1)
        f = 0.0;
    else
        f = 1.0f - (d / ((float)x0 + (float)x1));

    return f;
}

template <class T>
float FuzzyTri<T>::Member(T d)
{
    float f = .0;
    //float x0 = this->m_iParams[0], x1 = this->m_iParams[1], x2 = this->m_iParams[2];
    float x0 = 0, x1 = this->m_iParams[1] - this->m_iParams[0], x2 = this->m_iParams[2] - this->m_iParams[0];
    d -= this->m_iParams[0];

    if (d <= x0)
        f = 0.0;
    else if (d > x0 && d <= x1)
        f = d / (x0 + x1);
    else if (d > x1 && d <= x2)
        f = d / (x1 + x2);
    else
        f = 0.0;

    return f;
}

#endif // FUZZYITEM_H
