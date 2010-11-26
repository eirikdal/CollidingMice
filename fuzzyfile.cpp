#include "fuzzyfile.h"
#include "fuzzyling.h"
#include "fuzzyitem.h"
#include "fuzzyrule.h"
#include <string.h>

#define SDATA "F:\\Dokumenter\\NTNU\\IT3105\\CollidingMice\\definition.txt"

using namespace std;

FuzzyFile::FuzzyFile()
{
    m_pRead.open(SDATA);
    //m_pWrite.open(szFile);
}

FuzzyFile::~FuzzyFile()
{
    m_pRead.close();
    //m_pWrite.close();
}

FuzzyFile::LingVar* FuzzyFile::FindLV(char* name, list<LingVar> lingVars)
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

FuzzyFile::FuzzySet* FuzzyFile::FindMF(char* name, list<FuzzySet> items)
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
                /* we found the right fuzzyset, now update it */
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

                        s.set = new FuzzyTri<int>(name, x00, x01, x02);
                        *it = s;
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

                        s.set = new FuzzyGrade<int>(name,x00,x01);
                        *it = s;
                        break;
                    }
                    else if (strcmp(tok, "trapezoid") == 0)
                    {
                        char *c0, *c1, *c2;
                        c0 = strtok(func, "|");
                        c1 = strtok(NULL, "|");
                        c2 = strtok(NULL, "|");

                        char *x1, *x2, *x3, *x4;
                        char *y1, *y2, *y3, *y4;

                        x1 = strtok(c0, ",");
                        y1 = strtok(NULL, ",");

                        x2 = strtok(c1, ",");
                        y2 = strtok(NULL, ",");

                        x3 = strtok(c2, ",");
                        y3 = strtok(NULL, ",");

                        x4 = strtok(c2, ",");
                        y4 = strtok(NULL, ",");

                        int x00, x01, x02, x03;

                        x00 = atoi(x1);
                        x01 = atoi(x2);
                        x02 = atoi(x3);
                        x03 = atoi(x4);

                        s.set =  new FuzzyTrap<int>(name,x00, x01, x02, x03);
                        *it = s;
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

                        s.set = new FuzzyRev<int>(name,x00,x01);
                        *it = s;
                        break;
                    }
                }
            }

            *it = s;
        }

        *lv = v;
    }
}

void FuzzyFile::c_if(char* tok, list<LingVar> lingVars, FuzzyRule* rules)
{
    list<RuleNode*>* ruleList = new list<RuleNode*>();

    char *pre, *ant;
    pre = new char[48];
    ant = new char[48];

    PREFIX sign = POS;
    RULECONN conn = NONE;

    tok = strtok(NULL, " "); // if
    strcpy(pre, tok);
    tok = strtok(NULL, " "); // var
    tok = strtok(NULL, " "); // is

    if (strcmp(tok, "not") == 0) // (not)
    {
        sign = NEG;
        tok = strtok(NULL, " ");
    }

    strcpy(ant, tok);
    LingVar* v = this->FindLV(pre, lingVars);

    /* add to the rule base */
    // FuzzySet* fs = this->FindMF(ant, v->items);
    FuzzyLing<int>* lv = new FuzzyLing<int>(v->name, INPUT);
    //FuzzyItem<int>* f = fs->set;
    m_pFuzzyLings.push_back(lv);

    RuleNode* ruleNode = new RuleNode();
    ruleNode->ant = ant;
    ruleNode->conn = conn;
    ruleNode->pre = pre;
    ruleNode->sign = sign;

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
            //ant = new char[48];

            strcpy(pre, tok);
            tok = strtok(NULL, " ");
            //tok = strtok(NULL, " ");
            //strcpy(ant, tok);

            LingVar* v = this->FindLV(pre, lingVars);
            //FuzzyLing<int>* lv = new FuzzyLing<int>(v->name, OUTPUT);
            //m_pFuzzyLings.push_back(lv);
            //FuzzySet* fs = this->FindMF(ant, v->items);

            rules->Add(ruleList, pre);

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

            //pre = new char[48];
            ant = new char[48];

            strcpy(ant, tok);
            tok = strtok(NULL, " ");
            //strcpy(ant, tok);


            LingVar* v = this->FindLV(pre, lingVars);
            FuzzyLing<int>* lv = new FuzzyLing<int>(v->name, INPUT);
            m_pFuzzyLings.push_back(lv);
            //FuzzyItem<int>* fs = (this->FindMF(ant, v->items))->set;

            RuleNode* ruleNode = new RuleNode();
            ruleNode->ant = ant;
            ruleNode->conn = conn;
            ruleNode->pre = pre;
            ruleNode->sign = sign;

            ruleList->push_back(ruleNode);
        }
    }
}

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
                    rules = new FuzzyRule();
                }
                else if (strcmp(tok, "if") == 0)
                {
                    this->c_if(tok, lingVars, rules);
                }
                tok = strtok(NULL, " ");
            }
        }
        m_pFuzzyData = new FuzzyData();
        m_pFuzzyData->lingVars = m_pFuzzyLings;
        m_pFuzzyData->rules = rules;
    }
    else
        return false;

    return true;
}
