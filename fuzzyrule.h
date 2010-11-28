#ifndef FUZZYRULE_H
#define FUZZYRULE_H

#include "fuzzyling.h"
#include <list>

using namespace std;

enum RULECONN { AND, OR, NONE };
enum PREFIX { NEG, POS };

struct RuleNode
{
    char* pre;
    char* ant;

    HEDGE hedge;
    PREFIX sign;
    RULECONN conn;
};

struct Rule
{
    // list of all arguments on left side
    list<RuleNode*>* rules;
    // consequent of rule
    FuzzySet* cons;
};

class FuzzyRule
{
public:
    FuzzyRule();

    void Add(list<RuleNode*>*, FuzzySet*);
    list<FuzzySet*> Eval(list<FuzzySet*>);

private:
    list<Rule*> m_pRules;
    float Hedgify(float, HEDGE);
};

#endif // FUZZYRULE_H
