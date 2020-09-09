#include <iostream>
#include <stdio.h>
#include <io.h>
#include <string>
#include <fstream>
using namespace std;

int main()
{
	std::ofstream out("res.bat",std::ios::app| std::ios::out|std::ios::in);
    if (!out.is_open())
    {
        cout<<"111111111111111111: "<<out.is_open()<<endl;
        cout<<"file open fails!"<<endl;
        exit(-1);
    }else{
        cout<<"000000000000000: "<<out.is_open()<<endl;
        out<<"00000000000000000000"<<endl;
		out.write("34454545454545454545454554544",sizeof("34454545454545454545454554544"));

    }
	out.close();
	return 0;
}