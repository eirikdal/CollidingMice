#ifndef FUZZY_H
#define FUZZY_H

#include "fuzzyfile.h"
#include "fuzzyling.h"

class Fuzzy
{
public:
    Fuzzy ();
    ~Fuzzy();

    enum ACTION { FLEE, RUN, ATTACK, NONE };
    enum HEALTH { VERYLOW=1, LOW=3, MED=5, HIGH=7, VERYHIGH=9};
    enum RATING { VERYWEAK=1, WEAK, AVERAGE, STRONG, VERYSTRONG };
    enum RANGE { VERYSHORT, SHORT, MEDIUM, LONG, VERYLONG };

    /*
    struct FuzzySet
    {
        FuzzyLing<int>::FuzzyLing* ths;
        FuzzySet* nxt;
    };
*/
    ACTION action(int, int, int);

private:
    FuzzySet* root;
    FuzzyFile* m_pFile;
    FuzzyRule* m_pRules;

    list< FuzzyLing<int>* > m_pLings;
};

#endif // FUZZY_H

