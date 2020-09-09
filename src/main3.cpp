#include <iostream>
#include <queue>
#include <algorithm>
#include <string.h>
#include <set>
#include <climits>
#include <fstream>
#include <map>
#include <string>
#include<time.h>
#include<set>
#include<windows.h>

#pragma comment(linker, "/STACK:10240000000,10240000000") 
using namespace std;
struct edge {
	int v1;
	int v2;
    double weight=1;
};
struct gate {
	int target;
	int control;
	char type[4];
};
struct MyException : public exception
{
  const char * what () const throw ()
  {
    return "C++ recursion Exception";
  }
};
inline bool operator<(const edge& lhs, const edge& rhs) {
	if (lhs.v1 != rhs.v1) {
		return lhs.v1 < rhs.v1;
	}
	return lhs.v2 < rhs.v2;
}

struct node {
	int cost_fixed;
	int cost_heur;
	int cost_heur2;
	int depth;
	int* qubits; // get qubit of location -> -1 indicates that there is "no" qubit at a certain location
	int* locations; // get location of qubits -> -1 indicates that a qubit does not have a location -> shall only occur for i > nqubits
	int nswaps;
	int done;
	vector<vector<edge> > swaps;
	vector<gate > all_gates;
};

struct node_cmp {
	bool operator()(node& x, node& y) const {
		if ((x.cost_fixed + x.cost_heur + x.cost_heur2) != (y.cost_fixed + y.cost_heur + y.cost_heur2)) {
			return (x.cost_fixed + x.cost_heur + x.cost_heur2) > (y.cost_fixed + y.cost_heur + y.cost_heur2);
		}

		if(x.done == 1) {
			return false;
		}
		if(y.done == 1) {
			return true;
		}

		return x.cost_heur + x.cost_heur2 > y.cost_heur + y.cost_heur2;
	}
};

int** dist;
vector<edge> swap_list;
std::set<int> swap_qubit_set;
vector<edge> next_swap_list;
std::set<int> next_swap_qubit_set;
int positions;
double weaken1=0.4;
double weaken2=0.3;
double weaken3=0.2;
double weaken4=0.1;
unsigned long ngates = 0;
unsigned int nqubits = 0;
std::set<edge> graph;
vector<vector<gate> > layers;
priority_queue<node, std::vector<node>, node_cmp> nodes;
vector<vector<int> > qlist= vector<vector<int> >();
	vector<vector<int> > lolist= vector<vector<int> >();
	vector<int > depthes;
	vector<int > gates_sum;
	vector<int > swap_sum;
void printLocations(int *loc){
		for(int i=0; i<nqubits; i++) {
		cout << "  q" << i << " is  mapped to Q" << loc[i] << endl;
	}
}
// void print_gates(vector<gate> all_gates){
// 			for(vector<gate>::iterator it = all_gates.begin(); it != all_gates.end(); it++) {
// 				gate g=*it;
// 		cout  << g.type << " " << g.control<< " " << g.target << endl;
			
// 	}
// }
//build a graph representing the coupling map of IBM QX3
void build_graph_QX3() {
	graph.clear();
	positions = 16;
	edge e;
	e.v1 = 0;
	e.v2 = 1;
	graph.insert(e);
	e.v1 = 1;
	e.v2 = 2;
	graph.insert(e);
	e.v1 = 2;
	e.v2 = 3;
	graph.insert(e);
	e.v1 = 3;
	e.v2 = 14;
	graph.insert(e);
	e.v1 = 4;
	e.v2 = 3;
	graph.insert(e);
	e.v1 = 4;
	e.v2 = 5;
	graph.insert(e);
	e.v1 = 6;
	e.v2 = 7;
	graph.insert(e);
	e.v1 = 6;
	e.v2 = 11;
	graph.insert(e);
	e.v1 = 7;
	e.v2 = 10;
	graph.insert(e);
	e.v1 = 8;
	e.v2 = 7;
	graph.insert(e);
	e.v1 = 9;
	e.v2 = 8;
	graph.insert(e);
	e.v1 = 9;
	e.v2 = 10;
	graph.insert(e);
	e.v1 = 11;
	e.v2 = 10;
	graph.insert(e);
	e.v1 = 12;
	e.v2 = 5;
	graph.insert(e);
	e.v1 = 12;
	e.v2 = 11;
	graph.insert(e);
	e.v1 = 12;
	e.v2 = 13;
	graph.insert(e);
	e.v1 = 13;
	e.v2 = 4;
	graph.insert(e);
	e.v1 = 13;
	e.v2 = 14;
	graph.insert(e);
	e.v1 = 15;
	e.v2 = 0;
	graph.insert(e);
	e.v1 = 15;
	e.v2 = 14;
	graph.insert(e);
}


void build_graph_QX20() {
	graph.clear();
	positions = 20;
	edge e1;
	edge e2;
	for (int i = 0; i < 4; i++)
	{
		e1.v1 = i;
		e1.v2 = i+1;
		e2.v1 = i+1;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
	for (int i = 5; i < 9; i++)
	{
		e1.v1 = i;
		e1.v2 = i+1;
		e2.v1 = i+1;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
	for (int i = 10; i < 14; i++)
	{
		e1.v1 = i;
		e1.v2 = i+1;
		e2.v1 = i+1;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}

	for (int i = 15; i < 19; i++)
	{
		e1.v1 = i;
		e1.v2 = i+1;
		e2.v1 = i+1;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
		for (int i = 0; i < 15; i++)
	{
		e1.v1 = i;
		e1.v2 = i+5;
		e2.v1 = i+5;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
	for (int i = 1; i <= 7; i+=2)
	{
		e1.v1 = i;
		e1.v2 = i+6;
		e2.v1 = i+6;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
	for (int i = 11; i <= 13; i+=2)
	{
		e1.v1 = i;
		e1.v2 = i+6;
		e2.v1 = i+6;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
	for (int i = 2; i <= 8; i+=2)
	{
		e1.v1 = i;
		e1.v2 = i+4;
		e2.v1 = i+4;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
	for (int i = 12; i <= 14; i+=2)
	{
		e1.v1 = i;
		e1.v2 = i+4;
		e2.v1 = i+4;
		e2.v2 = i;
		graph.insert(e1);
		graph.insert(e2);
	}
}

bool contains(vector<int> v, int e) {
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++) {
		if (*it == e) {
			return true;
		}
	}
	return false;
}
bool contains_edge(vector<edge> v, edge e) {
	for (vector<edge>::iterator it = v.begin(); it != v.end(); it++) {
		edge e1=*it;
		if ((e1.v1== e.v1&&e1.v2== e.v2)||(e1.v1== e.v2&&e1.v2== e.v1)) {
			return true;
		}
	}
	return false;
}
//Breadth first search algorithm to determine the shortest paths between two physical qubits
int bfs(int start, int goal, std::set<edge>& graph) {
	queue<vector<int> > queue;
	vector<int> v;
	v.push_back(start);
	queue.push(v);
	vector<vector<int> > solutions;

	int length;
	std::set<int> successors;
	while (!queue.empty()) {
		v = queue.front();
		queue.pop();
		int current = v[v.size() - 1];
		if (current == goal) {
			length = v.size();
			solutions.push_back(v);
			break;
		} else {
			successors.clear();
			for (set<edge>::iterator it = graph.begin(); it != graph.end();
					it++) {
				edge e = *it;
				if (e.v1 == current && !contains(v, e.v2)) {
					successors.insert(e.v2);
				}
				if (e.v2 == current && !contains(v, e.v1)) {
					successors.insert(e.v1);
				}
			}
			for (set<int>::iterator it = successors.begin();
					it != successors.end(); it++) {
				vector<int> v2 = v;
				v2.push_back(*it);
				queue.push(v2);
			}
		}
	}
	while (!queue.empty() && queue.front().size() == length) {
		if (queue.front()[queue.front().size() - 1] == goal) {
			solutions.push_back(queue.front());
		}
		queue.pop();
	}

	for (int i = 0; i < solutions.size(); i++) {
		vector<int> v = solutions[i];
		for (int j = 0; j < v.size() - 1; j++) {
			edge e;
			e.v1 = v[j];
			e.v2 = v[j + 1];
			if (graph.find(e) != graph.end()) {
				return (length-2)*7;
				return length - 2;
			}
		}
	}

	return (length - 2)*7 + 4;
}

//Breadth first search algorithm to determine the shortest paths between two physical qubits
vector<int>  bfsPath(int start, int goal, std::set<edge>& graph) {
	queue<vector<int> > queue;
	vector<int> v;
	v.push_back(start);
	queue.push(v);
	vector<vector<int> > solutions;

	int length;
	std::set<int> successors;
	while (!queue.empty()) {
		v = queue.front();
		queue.pop();
		int current = v[v.size() - 1];
		if (current == goal) {
			length = v.size();
			solutions.push_back(v);
			break;
		} else {
			successors.clear();
			for (set<edge>::iterator it = graph.begin(); it != graph.end();
					it++) {
				edge e = *it;
				if (e.v1 == current && !contains(v, e.v2)) {
					successors.insert(e.v2);
				}
				if (e.v2 == current && !contains(v, e.v1)) {
					successors.insert(e.v1);
				}
			}
			for (set<int>::iterator it = successors.begin();
					it != successors.end(); it++) {
				vector<int> v2 = v;
				v2.push_back(*it);
				queue.push(v2);
			}
		}
	}
	while (!queue.empty() && queue.front().size() == length) {
		if (queue.front()[queue.front().size() - 1] == goal) {
			solutions.push_back(queue.front());
		}
		queue.pop();
	}
	return solutions[0];
}

void build_dist_table(std::set<edge>& graph) {
	dist = new int*[positions];

	for (int i = 0; i < positions; i++) {
		dist[i] = new int[positions];
	}

	for (int i = 0; i < positions; i++) {
		for (int j = 0; j < positions; j++) {
			if (i != j) {
				dist[i][j] = bfs(i,j,graph);
			} else {
				dist[i][i] = 0;
			}
		}
	}
}

//A very simplified QASM parser
void read_qasm(std::ifstream& infile) {
	std::string line;

	std::getline(infile, line);
	if(line != "OPENQASM 2.0;") {
		cerr << "ERROR: first line of the file has to be: OPENQASM 2.0;" << endl;
		exit(1);
	}

	std::getline(infile, line);
	if(line != "include \"qelib1.inc\";") {
		cerr << "ERROR: second line of the file has to be: include \"qelib1.inc\"" << endl;
		exit(1);
	}

	std::getline(infile, line);
	int n = -1;
	if (sscanf(line.c_str(), "qreg q[%d];", &n) != 1) {
		cerr << "ERROR: failed to parse qasm file: " << line << endl;
		exit(1);
	}
	if (n > positions) {
		cerr << "ERROR: too many qubits for target architecture: " << n << endl;
		exit(2);
	}

	std::getline(infile, line);

	int* last_layer = new int[n];
	for (int i = 0; i < n; i++) {
		last_layer[i] = -1;
	}

	while (std::getline(infile, line)) {

		if (line == "") {
			continue;
		}
		gate g;
		int layer;

		int nq = sscanf(line.c_str(), "%3s q[%d],q[%d];", g.type, &g.control,
				&g.target);

		if (nq == 3) {
			layer = max(last_layer[g.target], last_layer[g.control]) + 1;
			last_layer[g.target] = last_layer[g.control] = layer;
		} else if (nq == 2) {
			g.target = g.control;
			g.control = -1;
			layer = last_layer[g.target] + 1;
			last_layer[g.target] = layer;
		} else {
			double angle;
			if(sscanf(line.c_str(), "rz(%lf) q[%d];", &angle, &g.target) == 2) {
				g.control = -1;
				strcpy(g.type, "rz");
				layer = last_layer[g.target] + 1;
				last_layer[g.target] = layer;
			} else {
				cerr << "ERROR: could not read gate: " << line << endl;
				exit(1);
			}
		}
		ngates++;

		if (layers.size() <= layer) {
			layers.push_back(vector<gate>());
		}
		layers[layer].push_back(g);
	}


	std::set<int> list;
	vector<vector<int> >results;
	vector<vector<gate> >::iterator it;
	cout<<"---------*************-----------"<<endl;
	std::map<string,int> layer_map;
	for (int i = 0; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			//cout<<layers[i][j].control<<"-"<<layers[i][j].target<<":"<<layer_map.count(to_string(layers[i][j].control)+"-"+to_string(layers[i][j].target))<<endl;
				if(layers[i][j].control!=-1&&!layer_map.count(to_string(layers[i][j].control)+"-"+to_string(layers[i][j].target))){
			list.insert(layers[i][j].control);
			list.insert(layers[i][j].target);
			vector<int> temp;
			temp.push_back(layers[i][j].control);
			temp.push_back(layers[i][j].target);
			results.push_back(temp);
			layer_map.insert(pair<string, int>(to_string(layers[i][j].control)+"-"+to_string(layers[i][j].target),1));
		}
	
		}
	}
	ofstream of("E:\\github\\VF2\\data\\graphDB\\Ex2.my");
	set<int>::iterator it1;
	of<<"t # 0"<<endl;
	cout<<"t # 0"<<endl;
	for ( it1=list.begin(); it1 != list.end(); it1++)
	{
		of<<"v "<<*it1<<" 2"<<endl;
		cout<<"v "<<*it1<<" 2"<<endl;
	}
	for (int i = 0; i < results.size(); i++)
	{
			of<<"e "<<results[i][0]<<" "<<results[i][1]<<" 2"<<endl;
			cout<<"e "<<results[i][0]<<" "<<results[i][1]<<" 2"<<endl;	
	}
	
	of<<"t # -1"<<endl;
	cout<<"t # -1"<<endl;
	cout<<"----------*******************----------"<<endl;
	cout<<endl;
	nqubits = n;

	of.close();
	delete[] last_layer;
}
//A very simplified QASM parser
void read_ini(std::ifstream& inifile) {
	std::string line;
	vector<int> lochild;
	vector<int> qchild;
	while (std::getline(inifile, line)) {
		if (line == "") {
			continue;
		}
		int target;
		int source;
		int nq = sscanf(line.c_str(), "%d %d", &target, &source);
	if(nq==2){
		lochild[source]=target;
		qchild[target]=source;
	}else{
		if(lochild.size()>0&&qchild.size()>0){
			qlist.push_back(qchild);
			lolist.push_back(lochild);
		}
	lochild=vector<int> (nqubits,-1);
	qchild=vector<int> (positions,-1);
	}
	}
		qlist.push_back(qchild);
		lolist.push_back(lochild);
	}

int getNextLayer(int layer) {
	int next_layer = layer+1;
	while(next_layer < layers.size()) {
		for(vector<gate>::iterator it = layers[next_layer].begin(); it != layers[next_layer].end(); it++) {
			if(it->control != -1) {
				return next_layer;
			}
		}
		next_layer++;
	}
	return -1;
}
bool comp(edge & a, edge & b) {
	return a.weight < b.weight;//若前一个元素小于后一个元素，则返回真，否则返回假，即可自定义排序方式
}
int compute_weight_with_edge(std::set<int>& qubits, edge e, vector<edge> swap_list){
      /**
       * 两个端点都在qubits 分数加2
       * 两个都不在qubit里面  分数加0
       * 一个在 加1
       * 
       * */
      int weight=0;
        if(qubits.count(e.v1) != 0&&qubits.count(e.v2) != 0){
             //两个端点都在qubits
            weight+=2;
        }else if(qubits.count(e.v1) == 0&&qubits.count(e.v2)== 0){
            //两个都不在qubit里面

        }else{
            //一个在qubit里面
            weight+=1;
        }
        e.weight+=weight;
        return e.weight;
}
void layer_mapping(gate g,int* locations,vector<gate> &all_gates){
			if (g.control == -1) {
				//single qubit gate
				if(locations[g.target] == -1) {
					//handle the case that the qubit is not yet mapped. 
					//This happens if the qubit has not yet occurred in a CNOT gate
					gate g2 = g;
					g2.target = g.target -1;
					all_gates.push_back(g2);
				} else {
					//Add the gate to the circuit
					g.target = locations[g.target];
					all_gates.push_back(g);
				}
			} else {
				//CNOT gate
				g.target =locations[g.target];
				g.control = locations[g.control];

				edge e;
				e.v1 = g.control;
				e.v2 = g.target;

				if (graph.find(e) == graph.end()) {
					//flip the direction of the CNOT by inserting H gates
					e.v1 = g.target;
					e.v2 = g.control;
					if (graph.find(e) == graph.end()) {
						cerr << "ERROR: invalid CNOT: " << e.v1 << " - " << e.v2<< endl;
						exit(3);
					}
					// gate h;
					// h.control = -1;
					// strcpy(h.type, "h");
					// h.target = g.target;
					// all_gates.push_back(h);

					// h_gates.push_back(h);
					// h.target = g.control;
					// all_gates.push_back(h);

					// h_gates.push_back(h);
					// int tmp = g.target;
					// g.target = g.control;
					// g.control = tmp;
				}
				all_gates.push_back(g);
			}
}
node a_star_fixlayer(int layer, int* map, int* loc, int** dist,
                    double begin_time,vector<gate > all_gates,int *total_swaps) {
	//cout<<"a_star_fixlayer"<<endl;
	double time = double(clock() - begin_time) / 1000;
	if (time-500.0>0)
	{
		cout<<" a_star_fixlayer overtime!"<<time<<endl;
		  throw MyException();
	}
	int next_layer = getNextLayer(layer);
	node n;
	n.cost_fixed = 0;
	n.cost_heur = n.cost_heur2 = 0;
	n.all_gates=all_gates;

	n.qubits = new int[positions];
	n.locations = new int[nqubits];
	n.swaps = vector<vector<edge> >();
	n.done = 1;

	vector<gate> v = vector<gate>(layers[layer]);
    
	memcpy(n.qubits, map, sizeof(int) * positions);
	memcpy(n.locations, loc, sizeof(int) * nqubits);
while(v.size()>0){
	//vector<gate> h_gates = vector<gate>();
    swap_list.clear();
    swap_qubit_set.clear();
    next_swap_list.clear();
    next_swap_qubit_set.clear();
   for(vector<gate>::iterator it = v.begin(); it != v.end();it++) {  
       gate g = *it;
       edge e;
        e.v1=n.locations[g.control];
        e.v2=n.locations[g.target];
        //e在途中不存在，需要进行交换的边就加入待交换list
        	if (g.control==-1||graph.find(e) != graph.end()) {
				
                    cout<<"--------------------"<<endl;
                    printLocations( n.locations);
				 layer_mapping(*it,n.locations,n.all_gates);
                    cout<<"--------------------"<<endl;
                    printLocations( n.locations);
                 v.erase(it);
                it--;
			} else {
				int tmp = e.v1;
				e.v1 = e.v2;
				e.v2 = tmp;
			if (graph.find(e) == graph.end()) {
				swap_list.push_back(e);
                swap_qubit_set.insert(e.v1);
                swap_qubit_set.insert(e.v2);
                next_swap_qubit_set.insert(e.v1);
                next_swap_qubit_set.insert(e.v2);
				}	
					}
   }
   if(swap_list.size()<=0){
       return n;
   }
   vector<edge> swap_path;
     for(vector<edge>::iterator it = swap_list.begin(); it != swap_list.end(); it++) { 
            edge e = *it;
           vector<int> path= bfsPath(e.v1,e.v2,graph);
             for(vector<int>::iterator it1 = path.begin(); it1 != path.end(); it1++) { 
                  edge temp;
                  temp.v1=*it1;
                  if(it1+1 == path.end()){
                    break;
                  }
                 temp.v2=*(it1+1);
                 //计算每条边的权重
                 temp.weight=compute_weight_with_edge(swap_qubit_set,temp,swap_list);
				  for(vector<edge>::iterator sp = swap_path.begin(); sp != swap_path.end(); sp++) { 
					  edge sp_edge=*sp;
					  if(sp_edge.v1==temp.v1&&sp_edge.v2==temp.v2){
						 sp_edge.weight+=temp.weight;
						 break; 
					  }
				  }
                swap_path.push_back(temp);
             }
     }
//look-ahed
if(next_layer!=-1){
	vector<gate> v1 = vector<gate>(layers[next_layer]);
   for(vector<gate>::iterator it = v1.begin(); it != v1.end();it++) {  
       gate g = *it;
       edge e;
        e.v1=n.locations[g.control];
        e.v2=n.locations[g.target];
        //e在途中不存在，需要进行交换的边就加入待交换list
        	if (g.control==-1||graph.find(e) != graph.end()) {
				
			//如果swap——path里面的边满足后层的cx关系就降低权重
			if(contains_edge(swap_list,e)){
				  for(vector<edge>::iterator sp = swap_path.begin(); sp != swap_path.end(); sp++) { 
					  edge sp_edge=*sp;
					  if(sp_edge.v1==e.v1&&sp_edge.v2==e.v2){
						 sp_edge.weight=(e.weight)*(1-weaken1);
						 break; 
					  }
				  }
			}
                 v1.erase(it);
                it--;
			} else {
				int tmp = e.v1;
				e.v1 = e.v2;
				e.v2 = tmp;
			if (graph.find(e) == graph.end()) {
				next_swap_list.push_back(e);
                next_swap_qubit_set.insert(e.v1);
               next_swap_qubit_set.insert(e.v2);
				}	
					}
   }
     for(vector<edge>::iterator it = next_swap_list.begin(); it != next_swap_list.end(); it++) { 
            edge e = *it;
			if(!contains_edge(swap_list,e)){
				continue;
			}
           vector<int> path= bfsPath(e.v1,e.v2,graph);
             for(vector<int>::iterator it1 = path.begin(); it1 != path.end(); it1++) { 
                  edge temp;
                  temp.v1=*it1;
                  if(it1+1 == path.end()){
                    break;
                  }
                 temp.v2=*(it1+1);

                 //计算每条边的权重
                 temp.weight=compute_weight_with_edge(next_swap_qubit_set,temp,next_swap_list);
				 	  for(vector<edge>::iterator sp = swap_path.begin(); sp != swap_path.end(); sp++) { 
					  edge sp_edge=*sp;
					  if(sp_edge.v1==temp.v1&&sp_edge.v2==temp.v2){
						 sp_edge.weight+=(temp.weight)*weaken1;
						 break; 
					  }
				  }
             }
     }

int next_layer2 = getNextLayer(next_layer);
if(next_layer2!=-1){
	next_swap_list.clear();
	vector<gate> v1 = vector<gate>(layers[next_layer2]);
   for(vector<gate>::iterator it = v1.begin(); it != v1.end();it++) {  
       gate g = *it;
       edge e;
        e.v1=n.locations[g.control];
        e.v2=n.locations[g.target];
        //e在途中不存在，需要进行交换的边就加入待交换list
        	if (g.control==-1||graph.find(e) != graph.end()) {
				
			//如果swap——path里面的边满足后层的cx关系就降低权重
			if(contains_edge(swap_list,e)){
				  for(vector<edge>::iterator sp = swap_path.begin(); sp != swap_path.end(); sp++) { 
					  edge sp_edge=*sp;
					  if(sp_edge.v1==e.v1&&sp_edge.v2==e.v2){
						 sp_edge.weight=(e.weight)*(1-weaken2);
						 break; 
					  }
				  }
			}
                 v1.erase(it);
                it--;
			} else {
				int tmp = e.v1;
				e.v1 = e.v2;
				e.v2 = tmp;
			if (graph.find(e) == graph.end()) {
				next_swap_list.push_back(e);
                next_swap_qubit_set.insert(e.v1);
               next_swap_qubit_set.insert(e.v2);
				}	
					}
   }
     for(vector<edge>::iterator it = next_swap_list.begin(); it != next_swap_list.end(); it++) { 
            edge e = *it;
			if(!contains_edge(swap_list,e)){
				continue;
			}
           vector<int> path= bfsPath(e.v1,e.v2,graph);
             for(vector<int>::iterator it1 = path.begin(); it1 != path.end(); it1++) { 
                  edge temp;
                  temp.v1=*it1;
                  if(it1+1 == path.end()){
                    break;
                  }
                 temp.v2=*(it1+1);

                 //计算每条边的权重
                 temp.weight=compute_weight_with_edge(next_swap_qubit_set,temp,next_swap_list);
				 	  for(vector<edge>::iterator sp = swap_path.begin(); sp != swap_path.end(); sp++) { 
					  edge sp_edge=*sp;
					  if(sp_edge.v1==temp.v1&&sp_edge.v2==temp.v2){
						 sp_edge.weight+=(temp.weight)*weaken2;
						 break; 
					  }
				  }
             }
     }
}

}

            sort(swap_path.rbegin(), swap_path.rend(),comp);
            //Add the required SWAPs to the circuits
					edge e = swap_path[0];
					gate cnot;
					gate h1;
					gate h2;
                    
					if (graph.find(e) != graph.end()) {
						cnot.control = e.v1;
						cnot.target = e.v2;
					} else {
						cnot.control = e.v2;
						cnot.target = e.v1;
						int tmp = e.v1;
						e.v1 = e.v2;
						e.v2 = tmp;
						if (graph.find(e) == graph.end()) {
							cerr << "ERROR: invalid SWAP gate" << endl;
							exit(2);
						}
					}
					strcpy(cnot.type, "cx");
					strcpy(h1.type, "h");
					strcpy(h2.type, "h");
					h1.control = h2.control = -1;
					h1.target = e.v1;
					h2.target = e.v2;

					gate gg;
					gg.control = cnot.control;
					gg.target = cnot.target;
					strcpy(gg.type, "SWP");
					n.all_gates.push_back(cnot);
					n.all_gates.push_back(h1);
					n.all_gates.push_back(h2);
					n.all_gates.push_back(cnot);
					n.all_gates.push_back(h1);
					n.all_gates.push_back(h2);
					n.all_gates.push_back(cnot);
					//Insert a dummy SWAP gate to allow for tracking the positions of the logical qubits
					n.all_gates.push_back(gg);
					(*total_swaps)++;
                    cout<<"total_swaps:"<<*total_swaps<<" "<<e.v1<<" "<<e.v2<<endl;
                    int tmp_qubit1=n.qubits[e.v1];
                    int tmp_qubit2=n.qubits[e.v2];
                       n.qubits[e.v1]=tmp_qubit2; 
                       n.qubits[e.v2]=tmp_qubit1;
                    cout<<"--------------------"<<endl;
                    printLocations(n.locations);
                    if(tmp_qubit1!=-1){
                    n.locations[tmp_qubit1]=e.v2;
                    }
                    if (tmp_qubit2!=-1)
                    {
                       n.locations[tmp_qubit2]=e.v1;
                    }
                    printLocations( n.locations);
                    cout<<"--------------------"<<endl;
					if(v.size()<=0){
		// 	if (h_gates.size() != 0) {
		// 	for (vector<gate>::iterator it = h_gates.begin();
		// 			it != h_gates.end(); it++) {
		// 		all_gates.push_back(*it);
		// 	}
		// }
					}
    }
	return n;
}

int main(int argc, char** argv) {
	if(argc != 3) {
		cout << "Usage: ./imb_mapping <input_file><output_file>" << endl;
		exit(0);
	}
	    SetConsoleOutputCP(65001);
	build_graph_QX20();
	// 构造两个点之间的最短距离
	build_dist_table(graph);

	std::ifstream infile(argv[1]);
	read_qasm(infile);
	unsigned int width = 0;
	for (vector<vector<gate> >::iterator it = layers.begin(); it != layers.end(); it++) {
		if ((*it).size() > width) {
			width = (*it).size();
		}
	}

	std::string outfile1;
	outfile1.append("E:\\github\\quantum_compiler_optim\\compare\\total1");
	//outfile1.append(argv[2]);
	ofstream of1(outfile1,ios::app);
	std::string outfile2;
	outfile2.append("E:\\github\\quantum_compiler_optim\\compare\\total");
	//outfile1.append(argv[2]);
	ofstream of2(outfile2,ios::app);
	char* bName = argv[1];
	of1 << "Circuit name: " << bName << " (requires " << nqubits << " qubits)" << endl;

	of1 << endl << "Before mapping: " << endl;
	of1 << "  elementary gates: " << ngates << endl;
	of1 << "  depth: " << layers.size() << endl;

	cout << "Circuit name: " << bName << " (requires " << nqubits << " qubits)" << endl;

	cout << endl << "Before mapping: " << endl;
	cout << "  elementary gates: " << ngates << endl;
	cout << "  depth: " << layers.size() << endl;

	int *locations = new int[nqubits];//逻辑qubit数量
	int *qubits = new int[positions];//物理qubit数量

	for (int i = 0; i < positions; i++) {
		qubits[i] = -1;
	}

	for(int i = 0; i < nqubits; i++) {
		locations[i] = qubits[i] = i;
	}

	//Initially, no physical qubit is occupied
	for (int i = 0; i < positions; i++) {
			qubits[i] = -1;
	}

	//Initially, no logical qubit is mapped to a physical one
	for(int i = 0; i < nqubits; i++) {
		locations[i] = -1;
	}
		std::ifstream inifile("E:\\github\\VF2\\data\\graphDB\\ini_map.my");
		read_ini(inifile);
		vector<vector<int > >::iterator it;
	for (int k = 0; k < qlist.size(); k++)
	{	
		//Start mapping algorithm
		clock_t begin_time = clock();
			for(int j=0;j < qlist[k].size();j++){
				qubits[j] = qlist[k][j];
				locations[j] = lolist[k][j];
			}
			// locations[0]=1;
			// locations[1]=5;
			// locations[2]=10;
			// locations[3]=6;
			// locations[4]=2;
			// qubits[1]=0;
			// qubits[2]=4;
			// qubits[5]=1;
			// qubits[6]=3;
			// qubits[10]=2;

	vector<gate> all_gates;			
	int swap_count=0;
	int total_swaps = 0;
		try{
   	//Fix the mapping of each layer
	for (int i = 0; i < layers.size(); i++) {
			double time = double(clock() - begin_time) / 1000;
		if (time-500.0>0)
	{
		cout<<"a_star_fixlayer after overtime"<<time<<endl;
		  throw MyException();
	}
	// cout<<"-----------------1"<<endl;
	// print_gates(all_gates);
		node result = a_star_fixlayer(i, qubits, locations, dist,begin_time,all_gates,&total_swaps);
		
	// cout<<"-----------------2"<<endl;
	// print_gates(all_gates);
        delete[] locations;
		delete[] qubits;
		locations = result.locations;
		qubits = result.qubits;	
		all_gates=result.all_gates;
		 time = double(clock() - begin_time) / 1000;
		if (time-500.0>0)
	{
		cout<<"a_star_fixlayer after overtime"<<time<<endl;
		  throw MyException();
	}

		//The first layer does not require a permutation of the qubits
		//Add all gates of the layer to the circuit
		// vector<gate> layer_vec = layers[i];
		// for (vector<gate>::iterator it = layer_vec.begin();
		// 		it != layer_vec.end(); it++) {
		// }
		
	// cout<<"-----------------3"<<endl;
	// print_gates(all_gates);
	}

	
}catch( MyException e )
{
	cout<<"continue MyException: "<<k<<e.what()<<endl;
 	continue;
}catch( exception e )
{ cout<<"-------------00000000--------"<<endl;
    printLocations(locations);
	cout<<"continue exception: "<<e.what()<<endl;
 	continue;
}
//Fix the position of the single qubit gates
	for(vector<gate>::reverse_iterator it = all_gates.rbegin(); it != all_gates.rend(); it++) {
		if(strcmp(it->type, "SWP") == 0) {
			int tmp_qubit1 = qubits[it->control];
			int tmp_qubit2 = qubits[it->target];
			qubits[it->control] = tmp_qubit2;
			qubits[it->target] = tmp_qubit1;

			if(tmp_qubit1 != -1) {
				locations[tmp_qubit1] = it->target;
			}
			if(tmp_qubit2 != -1) {
				locations[tmp_qubit2] = it->control;
			}
		}
		if(it->target < 0) {
			int target = -(it->target +1);
			it->target = locations[target];
			if(locations[target] == -1) {
				//This qubit occurs only in single qubit gates -> it can be mapped to an arbirary physical qubit
				int loc = 0;
				while(qubits[loc] != -1) {
					loc++;
				}
				locations[target] = loc;
			}
		}
	}


	int *last_layer = new int[positions];
	for(int i=0; i<positions; i++) {
		last_layer[i] = -1;
	}

	vector<vector<gate> > mapped_circuit;

	//build resulting circuit
	for(vector<gate>::iterator it = all_gates.begin(); it != all_gates.end(); it++) {
		if(strcmp(it->type, "SWP") == 0) {
			continue;
		}
		if(it->control == -1) {
			//single qubit gate
			gate g = *it;
			int layer = last_layer[g.target] + 1;

			if (mapped_circuit.size() <= layer) {
				mapped_circuit.push_back(vector<gate>());
			}
			mapped_circuit[layer].push_back(g);
			last_layer[g.target] = layer;
		} else {
			gate g = *it;
			int layer = max(last_layer[g.control], last_layer[g.target]) + 1;
			if (mapped_circuit.size() <= layer) {
				mapped_circuit.push_back(vector<gate>());
			}
			mapped_circuit[layer].push_back(g);

			last_layer[g.target] = layer;
			last_layer[g.control] = layer;
		}
	}

	double time = double(clock() - begin_time) / 1000;
	of1 << endl << "After mapping (no post mapping optimizations are conducted): " <<k<< endl;
	of1 << "  elementary gates: " << all_gates.size()-total_swaps << endl;
	of1 << "  depth: " << mapped_circuit.size() << endl;

	cout << endl << "After mapping (no post mapping optimizations are conducted): " <<k<< endl;
	cout << "  elementary gates: " << all_gates.size()-total_swaps << endl;
	cout << "  depth: " << mapped_circuit.size() << endl;
	depthes.push_back(mapped_circuit.size());
	gates_sum.push_back(all_gates.size()-total_swaps);
	cout << endl << "The mapping required " << time << " seconds" << endl;

	cout << endl << "Initial mapping of the logical qubits (q) to the physical qubits (Q) of the IBM QX3 architecture: " << endl;
	of1 << endl << "The mapping required " << time << " seconds" << endl;

	of1 << endl << "Initial mapping of the logical qubits (q) to the physical qubits (Q) of the IBM QX3 architecture: " << endl;

	for(int i=0; i<nqubits; i++) {
		cout << "  q" << i << " is initially mapped to Q" << locations[i] << endl;
		of1 << "  q" << i << " is initially mapped to Q" << locations[i] << endl;
	}

	//Dump resulting circuit
	std::string outfile;
	outfile.append("E:\\github\\quantum_compiler_optim\\result\\");
	outfile.append(argv[2]);
	outfile.append("_main1_");
	outfile.append(to_string(k));
	ofstream of(outfile);
	of << "OPENQASM 2.0;" << endl;
	of << "include \"qelib1.inc\";" << endl;
	of << "qreg q[20];" << endl;
	of << "creg c[20];" << endl;

	for (vector<vector<gate> >::iterator it = mapped_circuit.begin();
			it != mapped_circuit.end(); it++) {
		vector<gate> v = *it;
		for (vector<gate>::iterator it2 = v.begin(); it2 != v.end(); it2++) {
			of << it2->type << " ";
			if (it2->control != -1) {
				of << "q[" << it2->control << "],";
			}
			of << "q[" << it2->target << "];" << endl;
		}
	}
	swap_sum.push_back(total_swaps);
	cout<<"*_*_*_*_*_swap_count*_*_*_*_*_*:"<<swap_count<<endl;
	of.close();
	delete[] last_layer;
	}
	if (depthes.size()>0)
	{
	int mindepth=depthes[0],minId=0,mingates=gates_sum[0],minswap=swap_sum[0];
	for (int i = 1; i < depthes.size(); i++)
	{
		if(mindepth>=depthes[i]&&mingates>gates_sum[i]){
			mindepth=depthes[i];
			minId=i;
			mingates=gates_sum[i];
		}
		if(minswap>swap_sum[i]){
			minswap=swap_sum[i];
		}
	}
	of2<<mingates<<" "<<mindepth<<endl;
	of1<<minId<<"element gates:" << mingates<<" depthes:"<<mindepth<<endl;
	cout<<minId<<" 最少门数量：" << mingates<<" 最小深度"<<mindepth<<" 最小交換："<<minswap<<endl;
	of1.close();
	of2.close();

	}
	delete[] locations;
	delete[] qubits;
	return 0;
}

