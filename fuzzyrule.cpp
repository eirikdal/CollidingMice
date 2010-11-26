#include "fuzzyrule.h"
#include <list>

FuzzyRule::FuzzyRule()
{
    //root = NULL;
}

bool FuzzyRule::Eval(list<FuzzySet*> ling)
{
    bool b = false;
    bool bAnd = false;
    bool bOr = false;

    for (list<Rule*>::iterator pr = m_pRules.begin(); pr != m_pRules.end(); pr++)
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
                    else
                        b = true;
                }
                else if (strcmp(s->name, n->ant) != 0 && n->sign == NEG)
                {
                    if (!b && bAnd)
                        b = false;
                    else
                        b = true;
                }
                else if (bOr && b)
                    b = true;
                else
                    b = false;
            }
        }
        if (b)
        {
            b = false;
        }
    }

    return b;
}

void FuzzyRule::Add(list<RuleNode*>* rules, char* cons)
{
    Rule* rule = new Rule();

    rule->cons = cons;
    rule->rules = rules;

    m_pRules.push_back(rule);
}
