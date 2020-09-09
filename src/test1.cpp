#include <iostream>
#include <queue>
#include <windows.h>
#include <algorithm>
#include <string.h>
#include <set>
#include <climits>
#include <fstream>
#include <map>
#include <time.h>
#include <climits>
#include <sstream>
using namespace std;
char qlist[1000][50];
char filenames[1000][50];
int index=0;
vector <int> str;
int list[1000];

vector<vector<vector<int>>> read_result(std::ifstream &inifile)
{
    vector<vector<vector<int>>> result;
    std::string line;
    vector<vector<int>> child;
    int i=0;
     char buf[50]={0}; 
     string s;
    while (std::getline(inifile, line))
    {
        if (line == "")
        {
            continue;
        }
        int n0;
        int n1;
        int n2;
        int n3;

        int nq = sscanf(line.c_str(), "%d:%d %d %d",&n0, &n1, &n2, &n3);
        if (nq == 4)
        {
            vector<int> cchild;
            cchild.push_back(n0);
            cchild.push_back(n1);
            cchild.push_back(n2);
            cchild.push_back(n3);
            child.push_back(cchild);
        }
        else
        {
        int nq1= sscanf(line.c_str(), "%s",buf);
        if (nq1==1)
        {
           for(int k=0;k<strlen(buf);k++){
               filenames[i][k]=buf[k];
           }
            i++;
            cout<<filenames[i-1]<<endl;
        }
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

void read_result_pycharm(std::ifstream& inifile) {
	std::string line1;
	std::string line2;
	bool flag=false;
     char buf[50]={0}; 
	while (std::getline(inifile, line1)&&std::getline(inifile, line2)) {
		if (line1 == ""||line2=="") {
			continue;
		}
        int n=0;
    int nq = sscanf(line1.c_str(), "%[^.]",buf);
    int nq1 = sscanf(line2.c_str(), "%d",&n);
    if (nq==1&&nq1==1)
    {
         for(int k=0;k<strlen(buf);k++){
               qlist[index][k]=buf[k];
           }
        list[index++]=n;
        cout<<qlist[index-1]<<" "<<list[index-1]<<endl;
    } 
	}
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(65001);
    std::ifstream infile("E:\\github\\quantum_compiler_optim\\compare\\total");
    vector<vector<vector<int>>> result = read_result(infile);
    std::ifstream infile1("E:\\github\\Qubit-Mapping-Subgraph-Isomorphism-master\\testRecord\\qct-2020-08-08-Q0.txt");


    int visited[1000]={0};
    read_result_pycharm(infile1);
    
        double prob=0,prob1=0;
    if (result.size() > 0)
    {      int result_1=0,result_2=0,result_4=0;
        int  greater_1_4_100=0, greater_4_1_100=0,eq_1_4_100=0;
        int greater_1_py=0,greater_py_1=0,eq_1_py=0,greater_1_py_sum=0,greater_py_1_sum=0;
        //int greater_1_no_py_sum=0,greater_py_no_1_sum=0;
        int greater_14 = 0,greater_14_sum = 0,  greater_41_sum = 0, greater_41 = 0, eq_14 = 0, no_result_14 = 0,no_result_1=0,no_result_0=0,no_result_4=0;
       int greater_1_less_4=0,greater_4_less_1=0; 
        for (int i = 0; i < result.size(); i++)
        { 
        

            if (result[i].size() == 4)
            {   result_1+=1;
                result_2+=1;
                result_4+=1;
                    if(result[i][0][1]<100){
                        if (result[i][3][3]>result[i][2][3])
                {
                    greater_1_4_100+=1;
                }else if (result[i][3][3]<result[i][2][3])
                {
                    greater_4_1_100+=1;
                }else{
                    eq_1_4_100+=1;
                }
                    }
                for(int j=0;j<index&&qlist[i]!="";j++){
                    if (strcmp(qlist[j],filenames[i])==0)
                    {
                         visited[j]=1;  
                        if (result[i][2][3]>list[j])
                        {
                            greater_py_1+=1;
                            cout<<"py less than 1"<<filenames[i]<<endl;
                            greater_py_1_sum+=result[i][2][3]-list[j];
                            break;
                        }else if (result[i][2][3]<list[j])
                        {
                            cout<<"1 less than  py"<<filenames[i]<<endl;
                            greater_1_py+=1;
                            greater_1_py_sum+=list[j]-result[i][2][3];
                            break;
                        }else{
                            eq_1_py+=1;
                            break;
                        }  
                    }
                    
                }
                if (result[i][3][3]>result[i][2][3])
                {
                    greater_14+=1;
                    greater_1_less_4+=result[i][3][3]-result[i][2][3];
                    greater_14_sum+=result[i][2][3];
                    greater_41_sum+=result[i][3][3];
                }else if (result[i][3][3]<result[i][2][3])
                {
                    greater_41+=1;
                    greater_4_less_1+=result[i][2][3]-result[i][3][3];
                    greater_14_sum+=result[i][2][3];
                    greater_41_sum+=result[i][3][3];
                   // greater_41_sum+=result[i][2][3]-result[i][3][3];
                }else
                {
                    eq_14+=1;
                    greater_14_sum+=result[i][2][3];
                    greater_41_sum+=result[i][3][3];
                }
                
            }
            else if (result[i].size() == 3)
            {   result_1+=1;
                   //1有4无
                if (result[i][0][0]==0&&result[i][1][0]==1&&result[i][2][0]==2)
                { 
                    result_2+=1;
                   // greater_14+=1;
                    no_result_4+=1;
                   // greater_1_py+=1;
                     // greater_1_no_py_sum+=result[i][2][3];
                  // cout<<"no py name:"<<filenames[i]<<endl;
                }else if (result[i][0][0]==0&&result[i][1][0]==1&&result[i][2][0]==4)
                {
                    result_4+=1;
                 //  greater_41+=1;
                  // greater_py_1+=1;
                //   greater_py_no_1_sum+=result[i][2][3];
                //    cout<<"no 1 name:"<<filenames[i]<<endl;
                   no_result_1+=1;
                }else{
                    cout<<result[i][0][0]<<" "<<result[i][1][0]<<" "<<result[i][2][0]<<endl;
                } 
            }
            else{
                //
                if(result[i].size()==2){
                      if (result[i][0][0]==0&&result[i][1][0]==1)
                {
                    result_1+=1;
                    no_result_14+=1;
                    no_result_1+=1;
                    no_result_4+=1;
                }else{
                    cout<<result[i][0][0]<<" "<<result[i][1][0]<<endl;
                    no_result_0+=1;
                    if(result[i][1][0]-4==0){
                    no_result_1+=1;
                    result_4+=1;
                    }
                }  
                }else{
                     cout<<filenames[i]<<":"<<result[i][0][0]<<endl;
                     no_result_0+=1;
                    no_result_14+=1;
                    no_result_1+=1;
                    no_result_4+=1;
                }
               
            }
          
        }
             cout<<index<<"-------------"<<endl;
             int count=0;
                for (int m = 0; m < index; m++)
                {
                    if (visited[m]==0&&qlist[m]!="")
                    {
                        cout<<qlist[m]<<endl;
                        count++;
                    }
                    
                }
                
                cout<<count<<"-------------"<<endl;
        cout<<"sum:"<<result.size()<<endl;
        cout << "1比4好: " << greater_14 << " ,4比1好: " << greater_41 << " ,1和4相等: " << eq_14 << endl;
        cout << "1总共： " << greater_14_sum << "， 4总共： " << greater_41_sum << "个SWAP "<< endl;
        cout << "1比4少 " << greater_1_less_4 << "个SWAP,4比1好 " << greater_4_less_1 << "个SWAP "<< endl;

        cout<< "1,4没有结果: " << no_result_14 << " ,1没有结果: " << no_result_1 << " ,4没有结果:" << no_result_4 << " 0没有结果 " << no_result_0 << endl;
    
        cout << "1比py好: " << greater_1_py << " ,py比1好: " << greater_py_1 << " ,1和py相等：" << eq_1_py << endl;
        cout << "1比py少 " << greater_1_py_sum << "个SWAP,py比1好 " << greater_py_1_sum << "个SWAP "<< endl;
      //  cout << "py无结果1有 " << greater_1_no_py_sum << "个SWAP,1无结果py有 " << greater_py_no_1_sum << "个SWAP "<< endl;

        cout<<"原始方法得到结果："<<result_1<<" main1得到结果："<<result_2<<"main4得到结果："<<result_4<<endl;
        cout<<"<100 1<4 : "<<greater_1_4_100<<"  ,4<1:"<<greater_4_1_100<<",1==4:"<<eq_1_4_100<<endl;

    }
    return 0;
}