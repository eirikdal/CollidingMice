#include "fuzzyrule.h"
#include <list>

FuzzyRule::FuzzyRule()
{
    //root = NULL;
}

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

    /* TODO: we need to do AND(min), OR(max) "analysis here too" */

    for (list<Rule*>::iterator pr = r_list.begin(); pr != r_list.end(); pr++)
    {
        Rule* r = *pr;

        for (list<RuleNode*>::iterator pn = r->rules->begin(); pn != r->rules->end(); pn++)
        {
            RuleNode* n = *pn;

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

                if (strcmp(s->name, n->ant) == 0 && n->sign == POS)
                {
                    if (!b && bAnd)
                        b = false;
                    else if(b && bAnd)
                    {
                        fVal = min(fVal, this->Hedgify(s->f, n->hedge));
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
                    if (!b && bAnd)
                        b = false;
                    else if(b && bAnd)
                    {
                        fVal = min(fVal, this->Hedgify(s->f, n->hedge));
                        b = true;
                    }
                    else
                    {
                        fVal = this->Hedgify(s->f, n->hedge);
                        b = true;
                    }
                }
                else if (bOr && b)
                {
                    fVal = max(fVal, this->Hedgify(s->f, n->hedge));
                    b = true;
                }
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

void FuzzyRule::Add(list<RuleNode*>* rules, FuzzySet* cons)
{
    Rule* rule = new Rule();

    rule->cons = cons;
    rule->rules = rules;

    m_pRules.push_back(rule);
}
