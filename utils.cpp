#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>

#include "vertex.cpp"

using namespace std;

// グラフデータを読み込む -> vector<Vertex>
/*
N (頂点数)
sigma (入力期待記号列)
(1番目のsigma[0])
(1番目のsigma[1])
(1番目のsigma[1])
(n番目のsigma[0])
(n番目のsigma[1])
(n番目のsigma[n])
(1番目の頂点の出力記号)
(n番目の頂点の出力記号)
*/
vector<Vertex> read_graph_data(string path)
{
    fstream file(path);
    string line;

    int N;               // 頂点数
    string sigma;        // 入力期待文字列
    vector<Vertex> vtxs; // 頂点テーブル

    // 頂点数を取得
    getline(file, line);
    N = stoi(line);

    // sigmaを取得
    getline(file, line);
    sigma = line;

    // 頂点テーブルを初期化
    vtxs.resize(N, Vertex(0, sigma.size()));
    for (int i = 0; i < N; i++)
    {
        vtxs[i].id = i;
        vtxs[i].sigma = sigma;
    }

    // pathを登録
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < sigma.size(); j++)
        {
            //(n文字目の遷移作の頂点番号)を登録
            getline(file, line);
            stringstream ss{line};
            string s;
            while (getline(ss, s, ' '))
            {
                vtxs[i].to[j].insert(stoi(s));
            }
        }
    }

    // 出力を登録
    for (int i = 0; i < N; i++)
    {
        getline(file, line);
        vtxs[i].lambda = line;
    }

    return vtxs;
}

// ファイル読み込み
string read_file(string path)
{
    ifstream file(path);
    string line, out = "";

    // ファイルが開けるか
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file " << path << endl;
        return "";
    }

    while (getline(file, line))
    {
        out += line + '\n';
    }
    return out;
}

// ファイル書き込み
void write_file(string path, string content)
{
    ofstream file(path);

    // ファイルが開けるか
    if (!file.is_open())
    {
        cerr << "Error: Cannot open file " << path << endl;
        return;
    }

    file << content << endl;
}

// Dotのノードを生成する
string gen_node_dot(int id, string name, string shape = "circle")
{
    return "q_" + to_string(id) + "[label=<q<sub>" + name + "</sub>>,shape=" + shape + "];";
}
