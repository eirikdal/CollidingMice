#ifndef FUZZYRULE_H
#define FUZZYRULE_H

#include "fuzzyling.h"
#include <list>

using namespace std;

enum RULECONN { AND, OR, NONE };
enum PREFIX { NEG, POS };

struct RuleNode
{
    // precedent
    char* pre;
    // antecedant
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

enum FORMAT
{
    SUGENO,
    MAMDANI
};

class FuzzyRule
{
public:
    FuzzyRule(FORMAT format) { m_eFormat = format; }

    void Add(list<RuleNode*>*, FuzzySet*);
    list<FuzzySet*> Eval(list<FuzzySet*>);

    FORMAT Format() { return m_eFormat; }
private:
    FORMAT m_eFormat;
    list<Rule*> m_pRules;
    float Hedgify(float, HEDGE);
};

#endif // FUZZYRULE_H
