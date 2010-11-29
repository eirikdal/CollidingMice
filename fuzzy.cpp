#include "fuzzyfile.h"
#include "fuzzy.h"

Fuzzy::Fuzzy()
{
    m_pFile = new FuzzyFile();

    /* Load data from file */
    if (m_pFile->Load())
    {
        FuzzyData* fd = m_pFile->getData();

        /* Update class with the I/O data */
        for (list< LingVar >::iterator lv = fd->lingVars.begin(); lv != fd->lingVars.end(); lv++)
        {
            LingVar v = *lv;

            /* The type of variable (input/output) is hardcoded here. */
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
    /*
    for (list< LingVar >::iterator lv = m_pLings.begin(); lv != m_pLings.end(); lv++)
    {
        LingVar v = *lv;

        for (list<FuzzySet>::iterator it = v.items.begin(); it != v.items.end(); it++)
        {
            FuzzySet s = *it;

            free (s.set);
            free (s);
        }

        free(v);
    }*/
}

/* Helper-function for finding a linguistic variable by name */
LingVar* Fuzzy::FindLV(char* name, list<LingVar> lingVars)
{
    for (list<LingVar>::iterator lv = lingVars.begin(); lv != lingVars.end(); lv++)
    {
        LingVar v = *lv;

        LingVar* ling = new LingVar();
        ling->items = v.items;
        ling->name = v.name;

        if (strcmp(name, v.name) == 0)
            return ling;
    }

    return NULL;
}

/* Helper-function for finding a membership function by name */
FuzzySet* Fuzzy::FindMF(char* name, list<FuzzySet> items)
{
    for (list<FuzzySet>::iterator it = items.begin(); it != items.end(); it++)
    {
        FuzzySet fs = *it;

        FuzzySet* set = new FuzzySet();
        set->set = fs.set;
        set->name = fs.name;

        if (strcmp(name, fs.name) == 0)
            return set;
    }

    return NULL;
}

/* Helper-function for finding a membership function by name */
FuzzySet* Fuzzy::FindMF(char* name, list<LingVar> lingVars)
{
    for (list<LingVar>::iterator lv = lingVars.begin(); lv != lingVars.end(); lv++)
    {
        LingVar v = *lv;

        for (list<FuzzySet>::iterator it = v.items.begin(); it != v.items.end(); it++)
        {
            FuzzySet fs = *it;

            FuzzySet* set = new FuzzySet();
            set->set = fs.set;
            set->name = fs.name;

            if (strcmp(name, fs.name) == 0)
                return set;
        }
    }

    return NULL;
}

/*
* Implement the 4 steps of Fuzzy Logic Inference,
* for each of the linguistic variables:
*
* Health, Rating and Range
*
* Return appropriate action
*/
int Fuzzy::action(int health, int rating, int range)
{
    list<FuzzySet*> vars;

    if (health != 100)
        health = health + 0;

    /* Step 1: Fuzzification */
    for (list< FuzzyLing<int>* >::iterator lv = m_pLings.begin(); lv != m_pLings.end(); lv++)
    {
        FuzzyLing<int>* v = *lv;

        FuzzySet* f = new FuzzySet();

        /* Fuzzify the linguistic variables health, rating and range */
        if (strcmp(v->Name(), "health") == 0)
            memcpy(f, v->Fuzzify(health), sizeof(FuzzySet));
        else if (strcmp(v->Name(), "rating") == 0)
            memcpy(f, v->Fuzzify(rating), sizeof(FuzzySet));
        else if (strcmp(v->Name(), "range") == 0)
            memcpy(f, v->Fuzzify(range), sizeof(FuzzySet));

        /* Add the fuzzified variables to a temporary list */
        *lv = v;
        f->type = v->Name();
        vars.push_back(f);
    }

    /* Step 2: Apply Rule evaluation to the list of fuzzified variables */
    list<FuzzySet*> res = m_pRules->Eval(vars);

    /* since we know the output space, we can simulate it */
    float m_fVal[100];

    memset(m_fVal, 0, sizeof(float)*100);

    /* Step 3: Apply Rule aggregation to the output */
    for (list<FuzzySet*>::iterator lv = res.begin(); lv != res.end(); lv++)
    {
        FuzzySet* v = *lv;

        for (int i=v->set->Min();i<=v->set->Max();i++)
        {
            float t = 0;

            switch (v->set->Type())
            {
            case TRIANGLE:
                {
                FuzzyTri<int>* vt = reinterpret_cast<FuzzyTri<int>*> (v->set);
                t = vt->Member(i);
                break;
            }
            case GRADE:
                {
                FuzzyGrade<int>* vg = reinterpret_cast<FuzzyGrade<int>*> (v->set);
                t = vg->Member(i);
                break;
            }
            case REVERSE:
                {
                FuzzyRev<int>* vr = reinterpret_cast<FuzzyRev<int>*> (v->set);
                t = vr->Member(i);
                break;
            }
            case TRAPEZOID:
                {
                FuzzyTrap<int>* vtr = reinterpret_cast<FuzzyTrap<int>*> (v->set);
                t = vtr->Member(i);
                break;
            }
            }

            /* Clipping */
            if (t <= v->f)
                m_fVal[i] = max(t, m_fVal[i]);
            else
                m_fVal[i] = max(v->f, m_fVal[i]);
        }
    }


    float cog = .0;
    /* Step 4: Defuzzification, using either Sugeno or Mamdani to get crisp value. */
    if (m_pRules->Format() == MAMDANI)
    {
        float cog1 = .0, cog2 = .0;

        /* Mamdani */
        for (int i = 0; i < 100; i+=10)
        {
            cog1 += m_fVal[i] * i;
            cog2 += m_fVal[i];
        }

        if (cog2 != 0)
            cog = cog1 / cog2;
    }
    else if (m_pRules->Format() == SUGENO)
        cog = (-10 * m_fVal[7] + m_fVal[15] + m_fVal[22] * 10) / (m_fVal[7] + m_fVal[15] + m_fVal[22]);

    /* Step 4: Defuzzification, find out which action the crisp value most belongs to. */
    for (list< FuzzyLing<int>* >::iterator lv = m_pLings.begin(); lv != m_pLings.end(); lv++)
    {
        FuzzyLing<int>* v = *lv;

        FuzzySet* f = new FuzzySet();

        /* Fuzzify the linguistic variables health, rating and range */
        if (v->getType() == OUTPUT)
        {

        }

        /* Add the fuzzified variables to a temporary list */
        *lv = v;
        f->type = v->Name();
        vars.push_back(f);
    }

    // if cog is a number, return it, otherwise return 1 (donothing)
    return (cog != cog) ? 1 : cog;
}
