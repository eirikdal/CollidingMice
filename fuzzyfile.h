#ifndef FUZZYFILE_H
#define FUZZYFILE_H

using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "fuzzyitem.h"
#include "fuzzyrule.h"
#include "fuzzyling.h"

struct FuzzyData
{
    list< FuzzyLing<int>* > lingVars;
    FuzzyRule* rules;
};

class FuzzyFile
{
public:
    FuzzyFile();
    ~FuzzyFile();

    bool Load();
private:
    ifstream m_pRead;
    ofstream m_pWrite;

    list< FuzzyLing<int>* > m_pFuzzyLings;
    FuzzyData* m_pFuzzyData;

    LingVar* FindLV(char*, list<LingVar>);
    FuzzySet* FindMF(char*, list<FuzzySet>);

    void c_if(char*, list<LingVar>, FuzzyRule*);
    void c_fuzzy(char*, list<LingVar>);

public:
    FuzzyData* getData() { return m_pFuzzyData; }
    list< FuzzyLing<int>* > getLings() { return m_pFuzzyData->lingVars; }
    FuzzyRule* getRules() { return m_pFuzzyData->rules; }
};

#endif // FUZZYFILE_H