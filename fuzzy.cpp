#include "fuzzyfile.h"
#include "fuzzy.h"

Fuzzy::Fuzzy()
{
    m_pFile = new FuzzyFile();

    if (m_pFile->Load())
    {
        FuzzyData* fd = m_pFile->getData();
        m_pLings = fd->lingVars;
        m_pRules = fd->rules;
    }
}

Fuzzy::~Fuzzy()
{

}

/*
* Implement the 4 steps of Mamdani inference, for each
* Health, Rating and Range fuzzy sets
*
* Return appropriate action
*/
Fuzzy::ACTION Fuzzy::action(int health, int rating, int range)
{
    for (list< FuzzyLing<int>* >::iterator lv = m_pLings.begin(); lv != m_pLings.end(); lv++)
    {
        FuzzyLing<int>* v = *lv;
        FuzzyItem<int>* f;

        if (strcmp(v->Name(), "health") == 0)
            f = v->Fuzzify(health);
        else if (strcmp(v->Name(), "rating") == 0)
            f = v->Fuzzify(rating);
        else if (strcmp(v->Name(), "range") == 0)
            f = v->Fuzzify(range);

        m_pRules->Eval(v);

    }
    return NONE;
}
