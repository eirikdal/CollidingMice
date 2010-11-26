#include "fuzzyrule.h"

FuzzyRule::FuzzyRule()
{
    //root = NULL;
}

bool FuzzyRule::Eval(FuzzyLing<int>* ling)
{
    bool b = false;

    /*
    RuleNode* rules = root;


    FuzzyLing<int>::LingNode* l = ling->List();

    while (l->nxt != NULL)
    {
        if (b) return b;

        b = true;

        while (rules->nxt != NULL)
        {
            if (ling->getType() == INPUT)
            {
                /* If this lingvar is an input variable, we need to check
                   it against the rules */

          /*  }
            else
            {
                if (l->ths == NULL)
                {
                    /* If this lingvar is an output variable,
                       we need to return them */
                    // out->ths = l;
              /*  }
            }
        }
    }
    */
    return b;
}

/* Add another rule, with input, output and possibly connector */
/*void FuzzyRule::Add(char* pre, char* ant, PREFIX sign, RULECONN conn)
{
    RuleNode* k = new RuleNode();
    k->pre = pre;
    k->ant = ant;
    k->conn = conn;
    k->sign = sign;

    if (root == NULL)
        root = k;
    else
    {
        RuleNode* start = root;

        while (start->nxt != NULL)
            start = start->nxt;

        start->nxt = k;
    }
}*/

void FuzzyRule::Add(list<RuleNode*>* rules, char* cons)
{
    Rule* rule = new Rule();

    rule->cons = cons;
    rule->rules = rules;

    m_pRules.push_back(rule);
}
