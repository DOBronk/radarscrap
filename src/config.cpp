#include "common.h"

using namespace radarscraper;
using namespace std;

void Config::readfile(const string &filename)
{
    ifstream file(filename);
    string tmp;

    while (getline(file, tmp))
    {
        trim(tmp);

        if (tmp.empty() || (!tmp.empty() && tmp.at(0) == '#'))
            continue;

        parseline(tmp);
    }
}

void Config::parseline(const string &line)
{
    int eq = line.find('=');

    if (eq != string::npos)
    {
        string key = line.substr(0, eq);
        string value = line.substr(eq + 1);
        rtrim(key);
        ltrim(value);
        _settings.insert_or_assign(key, value);
    }
}