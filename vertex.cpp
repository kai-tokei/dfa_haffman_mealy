#pragma once

#include <iostream>
#include <vector>

using namespace std;

// 頂点
struct Vertex
{
    int id;
    string lambda;
    string sigma;
    vector<set<int>> to;

    Vertex(int i, int t)
    {
        id = i;
        to.resize(t, set<int>());
    }

    // dot言語化
    string to_str()
    {
        string out;
        for (int i = 0; i < to.size(); i++)
        {
            for (const int &v : to[i])
            {
                out += "    ";
                out += "q_" + to_string(id) + " -> " + "q_" + to_string(v);
                out += " [label=" + to_string(i) + "];";
                out += "\n";
            }
        }
        return out;
    }
};
