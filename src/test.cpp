#include <iostream>
#include <queue>
#include <windows.h>
#include <algorithm>
#include <string.h>
#include <set>
#include <climits>
#include <fstream>
#include <map>
#include <string>
#include <time.h>
using namespace std;

vector<vector<vector<int>>> read_result(std::ifstream &inifile)
{
    vector<vector<vector<int>>> result;
    std::string line;
    vector<vector<int>> child;
    while (std::getline(inifile, line))
    {
        if (line == "")
        {
            continue;
        }
        int n1;
        int n2;
        int nq = sscanf(line.c_str(), "%d %d", &n1, &n2);
        if (nq == 2)
        {
            vector<int> cchild;
            cchild.push_back(n1);
            cchild.push_back(n2);
            child.push_back(cchild);
        }
        else
        {
            if (child.size() > 0)
            {
                result.push_back(child);
            }
            child = vector<vector<int>>(0);
        }
    }
    result.push_back(child);
    return result;
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(65001);
    std::ifstream infile("E:\\github\\quantum_compiler_optim\\compare\\total");
    vector<vector<vector<int>>> result = read_result(infile);
        double prob=0,prob1=0;
    if (result.size() > 0)
    {
        int before = 0, after = 0, after1 = 0, after2 = 0, eq1 = 0, eq2 = 0, noresult = 0,noresult1=0;
        for (int i = 0; i < result.size(); i++)
        {
            if (result[i].size() == 3)
            {     
                    noresult1+=1;
                    cout<<result[i][1][0] <<" "<<result[i][2][0] <<endl;
                if (result[i][1][1] <= result[i][2][1])
                {
                    before = before + 1;
                }else{
                    prob+=(result[i][1][1]-result[i][2][1]+0.0)/result[i][1][1];
                    prob1+=(result[i][1][0]-result[i][2][0]+0.0)/result[i][1][0];
                    after+=1;
                }
            }
            else if (result[i].size() == 4)
            {
                if (result[i][1][1] <= result[i][2][1] && result[i][1][1] < result[i][3][1])
                {
                    before = before + 1;
                }
                else if (result[i][1][1] >= result[i][2][1] && result[i][1][1] > result[i][3][1])
                {         prob+=(result[i][1][1]-result[i][2][1]+0.0)/result[i][1][1]; 
                          prob1+=(result[i][1][0]-result[i][2][0]+0.0)/result[i][1][0];
                    after = after + 1;
                    if (result[i][2][1] <= result[i][3][1] && result[i][2][0] < result[i][3][0])
                    {
                        after1 = after1 + 1;
                    }
                    else if (result[i][3][1] <= result[i][2][1] && result[i][3][0] < result[i][2][0])
                    {
                        after2 += 1;
                    }
                    else
                    {
                        eq2 += 1;
                    }
                }
                else
                {
                    eq1 += 1;
                }
            }
            else
            {
                noresult += 1;
            }
        }
        cout<<"提升效率：深度--"<<prob/result.size()<<" 基础门--"<<prob1/result.size()<<endl;
        cout << "没有结果：" << noresult <<"优化后结果可能没有："<<noresult1<< endl;
        cout << "优化前更好：" << before << " 优化后更好：" << after << " 优化前后无差别：" << eq1 << endl;
        cout << "优化12条件更好：" << after1 << " 优化123条件更好：" << after2 << " 两种一样：" << eq2 << endl;
    }



    return 0;
}