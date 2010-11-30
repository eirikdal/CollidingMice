#include "fuzzyfile.h"
#include "fuzzyling.h"
#include "fuzzyitem.h"
#include "fuzzyrule.h"
#include "fuzzy.h"
#include <string.h>

#define SDATA "F:\\Dokumenter\\NTNU\\IT3105\\CollidingMice\\definition.txt"

using namespace std;

FuzzyFile::FuzzyFile()
{
    m_pRead.open(SDATA);
}

FuzzyFile::~FuzzyFile()
{
    m_pRead.close();
}

void FuzzyFile::c_fuzzy(char* tok, list<LingVar> lingVars)
{
    /* create lingvar with the associated membership function */
    tok = strtok(NULL, " ");
    char* key = tok;
    tok = strtok(NULL, " ");

    for (list<LingVar>::iterator lv = lingVars.begin(); lv != lingVars.end(); lv++)
    {
        LingVar v = *lv;

        for (list<FuzzySet>::iterator it = v.items.begin(); it != v.items.end(); it++)
        {
            FuzzySet s = *it;

            if (strcmp(key, s.name) == 0)
            {
                /* Parse membership functions, updating
                   the linguistic variables as we find them */
                while (tok != NULL)
                {
                    char* name = strtok(tok, "=");
                    char* func = strtok(NULL, "=");

                    if (strcmp(tok, "triangle") == 0)
                    {
                        char *c0, *c1, *c2;
                        c0 = strtok(func, "|");
                        c1 = strtok(NULL, "|");
                        c2 = strtok(NULL, "|");

                        char *x1, *x2, *x3;
                        char *y1, *y2, *y3;

                        x1 = strtok(c0, ",");
                        y1 = strtok(NULL, ",");

                        x2 = strtok(c1, ",");
                        y2 = strtok(NULL, ",");

                        x3 = strtok(c2, ",");
                        y3 = strtok(NULL, ",");

                        int x00, x01, x02;

                        x00 = atoi(x1);
                        x01 = atoi(x2);
                        x02 = atoi(x3);

                        memcpy(s.set, new FuzzyTri<int>(name, x00, x01, x02), sizeof(FuzzyTri<int>));
                        break;
                    }
                    else if (strcmp(tok, "grade") == 0)
                    {
                        char *c0, *c1, *c2;
                        c0 = strtok(func, "|");
                        c1 = strtok(NULL, "|");
                        c2 = strtok(NULL, "|");

                        char *x1, *x2;
                        char *y1, *y2;

                        x1 = strtok(c0, ",");
                        y1 = strtok(NULL, ",");

                        x2 = strtok(c1, ",");
                        y2 = strtok(NULL, ",");

                        int x00, x01;

                        x00 = atoi(x1);
                        x01 = atoi(x2);

                        memcpy(s.set, new FuzzyGrade<int>(name,x00,x01), sizeof(FuzzyGrade<int>));
                        break;
                    }
                    else if (strcmp(tok, "trapezoid") == 0)
                    {
                        char *c0, *c1, *c2, *c3;
                        c0 = strtok(func, "|");
                        c1 = strtok(NULL, "|");
                        c2 = strtok(NULL, "|");
                        c3 = strtok(NULL, "|");

                        char *x1, *x2, *x3, *x4;
                        char *y1, *y2, *y3, *y4;

                        x1 = strtok(c0, ",");
                        y1 = strtok(NULL, ",");

                        x2 = strtok(c1, ",");
                        y2 = strtok(NULL, ",");

                        x3 = strtok(c2, ",");
                        y3 = strtok(NULL, ",");

                        x4 = strtok(c3, ",");
                        y4 = strtok(NULL, ",");

                        int x00, x01, x02, x03;

                        x00 = atoi(x1);
                        x01 = atoi(x2);
                        x02 = atoi(x3);
                        x03 = atoi(x4);

                        memcpy(s.set, new FuzzyTrap<int>(name,x00, x01, x02, x03), sizeof(FuzzyTrap<int>));
                        break;
                    }
                    else if (strcmp(tok, "reverse") == 0)
                    {
                        char *c0, *c1, *c2;
                        c0 = strtok(func, "|");
                        c1 = strtok(NULL, "|");
                        c2 = strtok(NULL, "|");

                        char *x1, *x2;
                        char *y1, *y2;

                        x1 = strtok(c0, ",");
                        y1 = strtok(NULL, ",");

                        x2 = strtok(c1, ",");
                        y2 = strtok(NULL, ",");

                        int x00, x01;

                        x00 = atoi(x1);
                        x01 = atoi(x2);

                        memcpy(s.set, new FuzzyRev<int>(name,x00,x01), sizeof(FuzzyRev<int>));
                        break;
                    }
                }
            }
        }

        *lv = v;
    }
}

/* Helper function for rule parsing. */
HEDGE FuzzyFile::c_hedge(char* tok)
{
    HEDGE hedge;

    if (strcmp(tok, "slightly") == 0)
        hedge = SLIGHTLY;
    else if (strcmp(tok, "little") == 0)
        hedge = LITTLE;
    else if (strcmp(tok, "very") == 0)
        hedge = VERY;
    else if (strcmp(tok, "extremely") == 0)
        hedge = EXTREMELY;
    else
        hedge = NORMAL;

    return hedge;
}

/* Rule parsing */
void FuzzyFile::c_if(char* tok, list<LingVar> lingVars, FuzzyRule* rules)
{
    list<RuleNode*>* ruleList = new list<RuleNode*>();

    char *pre, *ant;
    pre = new char[48];
    ant = new char[48];

    PREFIX sign = POS;
    RULECONN conn = NONE;
    HEDGE hedge = NORMAL;

    tok = strtok(NULL, " "); // if
    strcpy(pre, tok);
    tok = strtok(NULL, " "); // var
    tok = strtok(NULL, " "); // is

    if (strcmp(tok, "not") == 0) // (not)
    {
        sign = NEG;
        tok = strtok(NULL, " ");
    }

    hedge = c_hedge(tok);

    if (hedge != NORMAL)
        strtok(NULL, " ");

    strcpy(ant, tok);

    RuleNode* ruleNode = new RuleNode();
    ruleNode->ant = ant;
    ruleNode->conn = conn;
    ruleNode->pre = pre;
    ruleNode->sign = sign;
    ruleNode->hedge = hedge;

    ruleList->push_back(ruleNode);

    tok = strtok(NULL, " ");

    /* we need to consider any AND or OR statements as well */
    while (tok != NULL)
    {
        RULECONN conn = NONE;
        if (strcmp(tok, "AND") == 0)
            conn = AND;
        else if (strcmp(tok, "OR") == 0)
            conn = OR;
        else if (strcmp(tok, "then") == 0)
        {
            tok = strtok(NULL, " ");
            tok = strtok(NULL, " ");
            tok = strtok(NULL, " ");

            pre = new char[48];

            strcpy(pre, tok);
            tok = strtok(NULL, " ");

            rules->Add(ruleList, Fuzzy::FindMF(pre, lingVars));

            break;
        }

        pre = new char[48];
        ant = new char[48];

        tok = strtok(NULL, " ");
        strcpy(pre, tok);
        tok = strtok(NULL, " ");

        if (strcmp(tok, "is") == 0)
        {
            tok = strtok(NULL, " ");

            if (strcmp(tok, "not") == 0)
            {
                sign = NEG;
                tok = strtok(NULL, " ");
            }

            hedge = c_hedge(tok);

            if (hedge != NORMAL)
                strtok(NULL, " ");

            ant = new char[48];

            strcpy(ant, tok);
            tok = strtok(NULL, " ");

            RuleNode* ruleNode = new RuleNode();
            ruleNode->ant = ant;
            ruleNode->conn = conn;
            ruleNode->pre = pre;
            ruleNode->sign = sign;
            ruleNode->hedge = hedge;

            ruleList->push_back(ruleNode);
        }
    }
}

/* Load the dataset from file */
bool FuzzyFile::Load()
{
    bool b = false;

    FuzzyData fuzzyData;

    if (m_pRead.is_open())
    {
        char line[2048];

        list<LingVar> lingVars;
        FuzzyRule* rules;

        while(!m_pRead.eof())
        {
            m_pRead.getline(line, 2048);

            if (line == NULL)
                return b;

            char* tok;

            tok = strtok(line, " ");

            while (tok != NULL)
            {
                if (strcmp(tok, "lingvar") == 0)
                {
                    /* create new lingvar */
                    tok = strtok(NULL, " ");

                    /* consume name token */
                    char* name = tok;
                    tok = strtok(NULL, " ");

                    list<FuzzySet> items;

                    /* consume fuzzyset tokens */
                    while (tok != NULL)
                    {
                        FuzzySet set;
                        set.name = new char[64];
                        set.set = new FuzzyItem<int>();
                        strcpy(set.name, tok);

                        items.push_back(set);
                        tok = strtok(NULL, " ");
                    }

                    LingVar l;
                    l.items = items;
                    l.name = new char[64];
                    strcpy(l.name, name);
                    lingVars.push_back(l);
                }
                else if (strcmp(tok, "fuzzyset") == 0)
                {
                    this->c_fuzzy(tok, lingVars);
                }
                else if (strcmp(tok, "ruleformat") == 0)
                {
                    tok = strtok(NULL, " ");

                    if (strcmp(tok, "mamdani") == 0)
                        rules = new FuzzyRule(MAMDANI);
                    else if (strcmp(tok, "sugeno") == 0)
                        rules = new FuzzyRule(SUGENO);
                }
                else if (strcmp(tok, "if") == 0)
                {
                    this->c_if(tok, lingVars, rules);
                }
                tok = strtok(NULL, " ");
            }
        }
        m_pFuzzyData = new FuzzyData();
        m_pFuzzyData->lingVars = lingVars;
        m_pFuzzyData->rules = rules;
    }
    else
        return false;

    return true;
}
