#ifndef FUZZYLING_H
#define FUZZYLING_H

#include "fuzzyitem.h"
#include <list>

using namespace std;

enum LINGTYPE
{
    INPUT, OUTPUT, NOTSET,
};

struct FuzzySet
{
    char* name;
    FuzzyItem<int>* set;
};

struct LingVar
{
    char* name;
    list<FuzzySet> items;
};

template <class T>
class FuzzyLing
{
public:
    FuzzyLing(LingVar, LINGTYPE);

    // void Add(FuzzyItem<T>*, char* szName);
    FuzzyItem<T>* Fuzzify(T d);

    void setType(LINGTYPE type) { m_eType = type; }

    LINGTYPE getType() { return m_eType; }

    ~FuzzyLing()
    {
    }

private:
    LINGTYPE m_eType;
    char* m_szName;
    LingVar m_pLing;

public:
    char* Name() { return m_szName; }
};


template <class T>
FuzzyLing<T>::FuzzyLing(LingVar v, LINGTYPE type)
{
    m_eType = type;
    m_pLing = v;
}

/*
template <class T>
void FuzzyLing<T>::Add(FuzzyItem<T>* lv)
{
    if (this->start == NULL)
    {
        start = new LingNode();
        //memcpy( &start->ths, lv, sizeof( FuzzyItem<int> ) );
        start->ths = *lv;
    }
    else
    {
        LingNode* it = start;

        while (it->nxt != NULL)
            it = it->nxt;

        // memcpy( &it->ths, lv, sizeof( FuzzyItem<int> ) );
        it->ths = *lv;
    }
}
*/

template <class T>
FuzzyItem<T>* FuzzyLing<T>::Fuzzify(T d)
{
    FuzzyItem<T>* found;

    /*
     * For each of the membership functions,
     * find out which is closest of each of
     * the lingvars in the list
     */
    for (list<FuzzySet>::iterator lv = m_pLing.items.begin(); lv != m_pLing.items.end(); lv++)
    {
        float temp = .0;
        FuzzySet cur = *lv;

        switch (cur.set->Type())
        {
        case GRADE:
            temp = ((FuzzyGrade<T>*) cur.set)->Member(d);
            break;
        case REVERSE:
            temp = ((FuzzyRev<T>*) cur.set)->Member(d);
            break;
        case TRIANGLE:
            temp = ((FuzzyTri<T>*) cur.set)->Member(d);
            break;
        case TRAPEZOID:
            temp = ((FuzzyTrap<T>*) cur.set)->Member(d);
            break;
        case UNKNOWN:
            throw;
            break;
        }

        cur.set->Update(temp);
    }

    return found;
}


#endif // FUZZYLING_H
