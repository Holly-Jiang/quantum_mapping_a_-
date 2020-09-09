#include <iostream>
#include <queue>
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
        int nq = sscanf(line.c_str(), "%d : %d", &n1, &n2);
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
vector<vector<vector<int>>> allocateRemaining2(vector<vector<vector<int>>> args){
vector<vector<vector<int>>> result;
queue<int> que;
   if (result.size() > 0)
    {
         for (int i = 0; i < result.size(); i++)
        {
           for (int j = 0; j < result[i].size(); j++)
           {    
               if (result[i][j][1]-99999!=0)
               {
                   que.push(result[i][j][0]);
               }
           }
           
        }
        while (!que.empty())
        {
           int queId=que.front();
           que.pop();
           
        }
        
    }
}
int main(int argc, char** argv) {
    std::ifstream infile("E:\\github\\SubgraphComparing\\build\\matching\\res.dat");
    vector<vector<vector<int>>> result = read_result(infile);
     if (result.size() > 0)
    {
         for (int i = 0; i < result.size(); i++)
        {
           for (int j = 0; j < result[i].size(); j++)
           {
               cout<<result[i][j][0]<<" "<<result[i][j][1]<<endl;
           }
           
        }
    }
    

    return 0;
}
