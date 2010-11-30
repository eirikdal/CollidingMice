#include "fuzzyrule.h"
#include <list>

/* Adjust for hedges if necessary */
float FuzzyRule::Hedgify(float f, HEDGE h)
{
    switch (h)
    {
    case SLIGHTLY:
        f = ::pow(f, 1.7);
        break;
    case LITTLE:
        f = ::pow(f, 1.3);
        break;
    case VERY:
        f = ::pow(f, 2.0);
        break;
    case EXTREMELY:
        f = ::pow(f, 3.0);
        break;
    }

    return f;
}

/* Step 2: Rule evaluation */
list<FuzzySet*> FuzzyRule::Eval(list<FuzzySet*> ling)
{
    /* temp variables for control flow */
    bool b = false;
    bool bAnd = false;
    bool bOr = false;

    /* temp variables for clipping/scaling */
    float fVal = .0;

    list<FuzzySet*>* c_list = new list<FuzzySet*>();
    list<Rule*> r_list = m_pRules;

    /* For each rule */
    for (list<Rule*>::iterator pr = r_list.begin(); pr != r_list.end(); pr++)
    {
        Rule* r = *pr;

        /* We need to evaluate each part of the rule seperately */
        for (list<RuleNode*>::iterator pn = r->rules->begin(); pn != r->rules->end(); pn++)
        {
            RuleNode* n = *pn;

            /* Check to see if the statement is true */
            for (list<FuzzySet*>::iterator ps = ling.begin(); ps != ling.end(); ps++)
            {
                FuzzySet* s = *ps;

                if (strcmp(n->pre, s->type) != 0)
                    continue;

                if (n->conn == AND)
                {
                    bAnd = true;
                    bOr = false;
                }
                else if (n->conn == OR)
                {
                    bOr = true;
                    bAnd = false;
                }
                else
                {
                    bOr = false;
                    bAnd = false;
                }

                /* if true */
                if (strcmp(s->name, n->ant) == 0 && n->sign == POS)
                {
                    /* Check to see if the AND-operator or OR-operator are specified. If so,
                        we need to handle them properly. b is the value of the previous statement. */
                    if (!b && bAnd)
                        b = false;
                    else if(b && bAnd)
                    {
                        fVal = min(fVal, this->Hedgify(s->f, n->hedge));
                        b = true;
                    }
                    else if (b && bOr)
                    {
                        fVal = max(fVal, this->Hedgify(s->f, n->hedge));
                        b = true;
                    }
                    else
                    {
                        fVal = this->Hedgify(s->f, n->hedge);
                        b = true;
                    }
                }
                else if (strcmp(s->name, n->ant) != 0 && n->sign == NEG)
                {
                    /* Same procedure as above, only this time for negated statements. */
                    if (!b && bAnd)
                        b = false;
                    else if(b && bAnd)
                    {
                        fVal = min(fVal, this->Hedgify(s->f, n->hedge));
                        b = true;
                    }
                    else if (b && bOr)
                    {
                        fVal = max(fVal, this->Hedgify(s->f, n->hedge));
                        b = true;
                    }
                    else
                    {
                        fVal = this->Hedgify(s->f, n->hedge);
                        b = true;
                    }
                }
                /* if the OR-operator is specified and the last statement was true, but this wasn't
                    we need to preserve the information for the future */
                else if (bOr && b)
                    b = true;
                else
                    b = false;
            }
        }
        if (b)
        {
            b = false;
            r->cons->f = fVal;
            c_list->push_back(r->cons);
        }

        fVal = .0;
    }

    return *c_list;
}

/* Add a new rule to the list */
void FuzzyRule::Add(list<RuleNode*>* rules, FuzzySet* cons)
{
    Rule* rule = new Rule();

    rule->cons = cons;
    rule->rules = rules;

    m_pRules.push_back(rule);
}
