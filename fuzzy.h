#ifndef FUZZY_H
#define FUZZY_H

#include "fuzzyfile.h"
#include "fuzzyling.h"

class Fuzzy
{
public:
    Fuzzy ();
    ~Fuzzy();

    /*
    struct FuzzySet
    {
        FuzzyLing<int>::FuzzyLing* ths;
        FuzzySet* nxt;
    };
*/
    int action(int, int, int);

    static LingVar* FindLV(char*, list<LingVar>);
    static FuzzySet* FindMF(char*, list<FuzzySet>);
    static FuzzySet* FindMF(char*, list<LingVar>);

private:
    FuzzySet* root;
    FuzzyFile* m_pFile;
    FuzzyRule* m_pRules;

    list< FuzzyLing<int>* > m_pLings;
};

#endif // FUZZY_H

