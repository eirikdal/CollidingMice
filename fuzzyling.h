#ifndef FUZZYLING_H
#define FUZZYLING_H

#include "fuzzyitem.h"
#include <list>

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
    FuzzyLing(char* desc, LINGTYPE type);

    void Add(FuzzyItem<T>*, char* szName);
    FuzzyItem<T>* Fuzzify(T d);

    void setType(LINGTYPE type) { m_eType = type; }

    LINGTYPE getType() { return m_eType; }

    ~FuzzyLing()
    {
        while (start != NULL)
        {
            free(start->ths);
            start = start->nxt;
        }
    }


    struct LingNode
    {
        FuzzyItem<T>* ths;
        LingNode* nxt;
    };

    LingNode* List() { return start; }

private:
    LINGTYPE m_eType;
    char* m_szDescription;
    LingNode* start;

public:
    char* Name() { return m_szDescription; }
};


template <class T>
FuzzyLing<T>::FuzzyLing(char* d, LINGTYPE type)
{
    start = new LingNode();
    start->ths = NULL;
    start->nxt = NULL;
    m_eType = type;
    m_szDescription = d;
}

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

template <class T>
FuzzyItem<T>* FuzzyLing<T>::Fuzzify(T d)
{
    LingNode* cur = start;

    FuzzyItem<T>* found;

    /*
     * For each of the membership functions,
     * find out which is closest of each of
     * the lingvars in the list
     */
    while (cur->nxt != NULL)
    {
        float temp = .0;

        switch (cur->ths->Type())
        {
        case GRADE:
            temp = ((FuzzyGrade<T>*) cur->ths)->Member(d);
            break;
        case REVERSE:
            temp = ((FuzzyRev<T>*) cur->ths)->Member(d);
            break;
        case TRIANGLE:
            temp = ((FuzzyTri<T>*) cur->ths)->Member(d);
            break;
        case TRAPEZOID:
            temp = ((FuzzyTrap<T>*) cur->ths)->Member(d);
            break;
        }

        cur->ths->Update(temp);

        cur = cur->nxt;
    }

    return found;
}


#endif // FUZZYLING_H
