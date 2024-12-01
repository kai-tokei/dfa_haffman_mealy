#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

#include "./vertex.cpp"
#include "./utils.cpp"

using namespace std;

const string dot_header_path = "./dot_header.txt";
const string dot_footer_path = "./dot_footer.txt";

int main(int argc, char *argv[])
{
    string graph_data_path;       // グラフデータのpath
    string accepted_string = "1"; // 受理文字列

    // グラフデータファイルのpathを読み込む
    if (argc > 1)
    {
        graph_data_path = argv[1];
    }
    else
    {
        cerr << "Error: No graph data !!" << endl;
        return 0;
    }
    if (argc > 2)
    {
        accepted_string = argv[3];
    }

    string dot_header = read_file(dot_header_path); // dotファイルのheader
    string dot_footer = read_file(dot_footer_path); // dotファイルのfooter

    string dot_code = dot_header; // dot言語のコード

    vector<Vertex> vtxs; // 頂点テーブル

    // グラフデータを読み取る
    vtxs = read_graph_data(graph_data_path);

    // 頂点をDotコードに変換
    dot_code += "\n";
    for (int i = 0; i < vtxs.size(); i++)
    {
        dot_code += "    " + gen_node_dot(i, to_string(i), (vtxs[i].lambda == "1" ? "doublecircle" : "circle")) + "\n";
    }
    dot_code += "\n    input -> q_0;\n";

    map<string, set<int>> groups;            // 頂点のグループ
    vector<string> group_table(vtxs.size()); // Vertexがどのグループに属しているかを表すテーブル

    // Q\F, Fで分類
    for (int i = 0; i < vtxs.size(); i++)
    {
        groups[to_string(vtxs[i].lambda == accepted_string)].insert(i);
        group_table[i] = to_string(vtxs[i].lambda == accepted_string);
    }

    // 分類先をナンバリング
    // それぞれのグループ内の要素の遷移先グループを確認
    // 異なる遷移先なら、別グループを作成し、分類
    while (true)
    {
        map<string, set<int>> next_groups;            // 分類中のグループ
        vector<string> next_group_table(vtxs.size()); // 頂点がどのグループに属すか記録するテーブル

        for (const auto &g : groups)
        {
            // それぞれの頂点に関して、遷移先をグループのどれに該当するかを調べる
            for (const int &v : g.second)
            {
                // 遷移先グループのidを結合して、文字列化する
                string dist = "";
                for (const set<int> &to : vtxs[v].to)
                {
                    for (const int &i : to)
                    {
                        dist += group_table[i];
                    }
                }
                next_groups[dist].insert(v); // 該当のグループに頂点を登録
                next_group_table[v] = dist;  // 登録した旨をtableに保存
            }
        }

        if (next_groups.size() == groups.size())
        {
            // グループ数が変わっていないなら、終了
            break;
        }
        else
        {
            groups = next_groups;
            group_table = next_group_table;
        }
    }

    // 同じクラス内の要素をmerge
    // クラス番号で頂点tableを作成

    // 頂点の接続関係をDotコードに変換
    dot_code += "\n";
    for (int i = 0; i < vtxs.size(); i++)
    {
        dot_code += vtxs[i].to_str() + "\n";
    }

    dot_code += dot_footer + "\n";

    // Dotファイルを作成
    ofstream file("graph.dot");
    file << dot_code;

    return 0;
}
