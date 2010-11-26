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
    PREFIX sign;
    RULECONN conn;
};

struct Rule
{
    // list of all arguments on left side
    list<RuleNode*>* rules;
    // consequent of rule
    char* cons;
};

class FuzzyRule
{
public:
    FuzzyRule();

    void Add(list<RuleNode*>*, char*);
    bool Eval(list<FuzzySet*>);

private:
    list<Rule*> m_pRules;
};

#endif // FUZZYRULE_H
