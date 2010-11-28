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
    char* type;
    float f;
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
    FuzzySet* Fuzzify(T d);

    void setType(LINGTYPE type) { m_eType = type; }

    LINGTYPE getType() { return m_eType; }

    ~FuzzyLing()
    {
    }

    FuzzySet* Find(char*);

private:
    LINGTYPE m_eType;
    char* m_szName;
    list< FuzzySet > m_pLing;

public:
    char* Name() { return m_szName; }
};

template<class T>
FuzzySet* FuzzyLing<T>::Find(char* c)
{
    for (list<FuzzySet>::iterator lv = m_pLing.begin(); lv != m_pLing.end(); lv++)
    {
        FuzzySet cur = *lv;

        if (strcmp(cur.name, c) == 0)
            return &cur;
    }

    return NULL;
}

template <class T>
FuzzyLing<T>::FuzzyLing(LingVar v, LINGTYPE type)
{
    m_eType = type;
    m_pLing = v.items;
    m_szName = v.name;
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
FuzzySet* FuzzyLing<T>::Fuzzify(T d)
{
    FuzzySet* found = new FuzzySet();

    /*
     * For each of the membership functions,
     * find out which is closest of each of
     * the lingvars in the list
     */
    float max = .0;
    float temp = .0;

    for (list<FuzzySet>::iterator lv = m_pLing.begin(); lv != m_pLing.end(); lv++)
    {
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

        if (temp >= max)
        {
            max = temp;
            memcpy(found, &(*lv), sizeof(FuzzySet));
            found->f = max;
            //found = &cur;
        }

        /* update the grade of membership */
        cur.set->Update(temp);
        cur.f = temp;
        memcpy(&(*lv), &cur, sizeof(FuzzySet));
    }

    return found;
}


#endif // FUZZYLING_H
