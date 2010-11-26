#include "fuzzyfile.h"
#include "fuzzy.h"

Fuzzy::Fuzzy()
{
    m_pFile = new FuzzyFile();

    if (m_pFile->Load())
    {
        FuzzyData* fd = m_pFile->getData();
        //m_pLings = fd->lingVars;

        for (list< LingVar >::iterator lv = fd->lingVars.begin(); lv != fd->lingVars.end(); lv++)
        {
            LingVar v = *lv;

            if (strcmp("action", v.name) != 0)
                m_pLings.push_back(new FuzzyLing<int>(v, INPUT));
            else
                m_pLings.push_back(new FuzzyLing<int>(v, OUTPUT));
        }

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
    list<FuzzySet*> vars;

    for (list< FuzzyLing<int>* >::iterator lv = m_pLings.begin(); lv != m_pLings.end(); lv++)
    {
        FuzzyLing<int>* v = *lv;

        FuzzySet* f = new FuzzySet();

        if (strcmp(v->Name(), "health") == 0)
            memcpy(f, v->Fuzzify(health), sizeof(FuzzySet));
        else if (strcmp(v->Name(), "rating") == 0)
            memcpy(f, v->Fuzzify(rating), sizeof(FuzzySet));
        else if (strcmp(v->Name(), "range") == 0)
            memcpy(f, v->Fuzzify(range), sizeof(FuzzySet));

        f->type = v->Name();
        vars.push_back(f);
    }

    m_pRules->Eval(vars);

    return NONE;
}
