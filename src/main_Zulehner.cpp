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

struct edge {
	int v1;
	int v2;
};

struct gate {
	int target;
	int control;
	char type[4];
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
int positions;
unsigned long ngates = 0;
unsigned int nqubits = 0;

std::set<edge> graph;
vector<vector<gate> > layers;
priority_queue<node, std::vector<node>, node_cmp> nodes;


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
// void printLocations(int *locations){
// 		for(int i=0; i<nqubits; i++) {
// 		cout << "  q" << i << " is initially mapped to Q" << locations[i] << endl;
// 	}
// }
//A very simplified QASM parser

void print_to_file(ofstream of,std::set<int> list,vector<vector<int> >results){
	cout<<"t "<<list.size()<<" "<<results.size()<<endl;
	set<int>::iterator it1;
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
}
void read_qasm(std::ifstream& infile,string filename) {
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
	int max_node=-1;//最大的节点id
	cout<<"---------*************-----------"<<endl;
	std::map<string,int> layer_map;
	for (int i = 0; i < layers.size(); i++)
	{
		for (int j = 0; j < layers[i].size(); j++)
		{
			//cout<<layers[i][j].control<<"-"<<layers[i][j].target<<":"<<layer_map.count(to_string(layers[i][j].control)+"-"+to_string(layers[i][j].target))<<endl;
				if(layers[i][j].control!=-1&&!layer_map.count(to_string(layers[i][j].control)+"-"+to_string(layers[i][j].target))
				&&!layer_map.count(to_string(layers[i][j].target)+"-"+to_string(layers[i][j].control))){
			list.insert(layers[i][j].control);
			list.insert(layers[i][j].target);
			if (max_node<layers[i][j].control)
			{
				max_node=layers[i][j].control;
			}
			if (max_node<layers[i][j].target)
			{
				max_node=layers[i][j].target;
			}
			vector<int> temp;
			temp.push_back(layers[i][j].control);
			temp.push_back(layers[i][j].target);
			results.push_back(temp);
			layer_map.insert(pair<string, int>(to_string(layers[i][j].control)+"-"+to_string(layers[i][j].target),1));
			
		}
		
			
		}
	}
	ofstream of("/root/graph/new/VF2_mapping_opt/data/graphDB/Ex2.my");
	set<int>::iterator it1;
	of<<"t "<<max_node+1<<" "<<results.size()<<endl;
	cout<<"t "<<max_node+1<<" "<<results.size()<<endl;
	for ( it1=list.begin(); it1 != list.end(); it1++)
	{	int degree=0;
		for (int i = 0; i < results.size(); i++)
	{
		if (results[i][0]==*it1||results[i][1]==*it1)
		{
			degree++;
		}
		
	}
		of<<"v "<<*it1<<" "<<0<<" "<<degree<<endl;
		// out<<"v "<<*it1<<" "<<0<<" "<<degree<<endl;
		cout<<"v "<<*it1<<" "<<0<<" "<<degree<<endl;
	}
	for (int i = 0; i < results.size(); i++)
	{
			of<<"e "<<results[i][0]<<" "<<results[i][1]<<endl;
			// out<<"e "<<results[i][0]<<" "<<results[i][1]<<endl;
			cout<<"e "<<results[i][0]<<" "<<results[i][1]<<endl;	
	}

	of<<"t "<<"# -1"<<endl;
	// out<<"t "<<"# -1"<<endl;
	cout<<"----------*******************----------"<<endl;
	cout<<endl;
	// nqubits = n;
	nqubits = positions;
	of.close();
	// out.close();
	delete[] last_layer;
}

void expand_node(const vector<int>& qubits, int qubit, edge *swaps, int nswaps,
		int* used, node base_node, const vector<gate>& gates, int** dist, int next_layer) {
		//qubits:本层需要考虑到的2量子
	if (qubit == qubits.size()) {
		//base case: insert node into queue
		if (nswaps == 0) {
			return;
		}
		node new_node;

		new_node.qubits = new int[positions];
		new_node.locations = new int[nqubits];

		memcpy(new_node.qubits, base_node.qubits, sizeof(int) * positions);
		memcpy(new_node.locations, base_node.locations, sizeof(int) * nqubits);

		new_node.swaps = vector<vector<edge> >();
		new_node.nswaps = base_node.nswaps + nswaps;
		for (vector<vector<edge> >::iterator it2 = base_node.swaps.begin();
				it2 != base_node.swaps.end(); it2++) {
			vector<edge> new_v(*it2);
			new_node.swaps.push_back(new_v);
		}

		new_node.depth = base_node.depth + 5;
		new_node.cost_fixed = base_node.cost_fixed + 7 * nswaps;
		new_node.cost_heur = 0;

		vector<edge> new_swaps;
		for (int i = 0; i < nswaps; i++) {
			new_swaps.push_back(swaps[i]);
			int tmp_qubit1 = new_node.qubits[swaps[i].v1];
			int tmp_qubit2 = new_node.qubits[swaps[i].v2];

			new_node.qubits[swaps[i].v1] = tmp_qubit2;
			new_node.qubits[swaps[i].v2] = tmp_qubit1;

			if (tmp_qubit1 != -1) {
				new_node.locations[tmp_qubit1] = swaps[i].v2;
			}
			if (tmp_qubit2 != -1) {
				new_node.locations[tmp_qubit2] = swaps[i].v1;
			}
		}
		new_node.swaps.push_back(new_swaps);
		new_node.done = 1;

		for (vector<gate>::const_iterator it = gates.begin(); it != gates.end();
				it++) {
			gate g = *it;
			if (g.control != -1) {
				new_node.cost_heur = new_node.cost_heur + dist[new_node.locations[g.control]][new_node.locations[g.target]];
				if(dist[new_node.locations[g.control]][new_node.locations[g.target]] > 4) {
					new_node.done = 0;
				}
			}
		}

		//Calculate heuristics for the cost of the following layer
		new_node.cost_heur2 = 0;
		if(next_layer != -1) {
			for (vector<gate>::const_iterator it = layers[next_layer].begin(); it != layers[next_layer].end();
							it++) {
				gate g = *it;
				if (g.control != -1) {
					if(new_node.locations[g.control] == -1 && new_node.locations[g.target]) {
					//ignore this case
					} else if(new_node.locations[g.control] == -1) {
						int min = 1000;
						for(int i=0; i< positions; i++) {
							if(new_node.qubits[i] == -1 && dist[i][new_node.locations[g.target]] < min) {
								min = dist[i][new_node.locations[g.target]];
							}
						}
						new_node.cost_heur2 = new_node.cost_heur2 + min;
					} else if(new_node.locations[g.target] == -1) {
						int min = 1000;
						for(int i=0; i< positions; i++) {
							if(new_node.qubits[i] == -1 && dist[new_node.locations[g.control]][i] < min) {
								min = dist[new_node.locations[g.control]][i];
							}
						}
						new_node.cost_heur2 = new_node.cost_heur2 + min;
					} else {
						new_node.cost_heur2 = new_node.cost_heur2 + dist[new_node.locations[g.control]][new_node.locations[g.target]];
					}
				}
			}
		}

		nodes.push(new_node);
	} else {
		expand_node(qubits, qubit + 1, swaps, nswaps, used, base_node, gates,
				dist, next_layer);

		for (set<edge>::iterator it = graph.begin(); it != graph.end(); it++) {
			edge e = *it;
			if (e.v1 == base_node.locations[qubits[qubit]]
					|| e.v2 == base_node.locations[qubits[qubit]]) {
				if (!used[e.v1] && !used[e.v2]) {
					used[e.v1] = 1;
					used[e.v2] = 1;
					swaps[nswaps].v1 = e.v1;
					swaps[nswaps].v2 = e.v2;
					expand_node(qubits, qubit + 1, swaps, nswaps + 1, used,
							base_node, gates, dist, next_layer);
					used[e.v1] = 0;
					used[e.v2] = 0;
				}
			}
		}
	}
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
node a_star_fixlayer(int layer, int* map, int* loc, int** dist) {

	int next_layer = getNextLayer(layer);

	node n;
	n.cost_fixed = 0;
	n.cost_heur = n.cost_heur2 = 0;
	n.qubits = new int[positions];
	n.locations = new int[nqubits];
	n.swaps = vector<vector<edge> >();
	n.done = 1;

	vector<gate> v = vector<gate>(layers[layer]);
	vector<int> considered_qubits;

	//Find a mapping for all logical qubits in the CNOTs of the layer that are not yet mapped
	for (vector<gate>::iterator it = v.begin(); it != v.end(); it++) {
		gate g = *it;
		if (g.control != -1) {
			considered_qubits.push_back(g.control);
			considered_qubits.push_back(g.target);
			if(loc[g.control] == -1 && loc[g.target] == -1) {
				set<edge> possible_edges;
				for(set<edge>::iterator it = graph.begin(); it != graph.end(); it++) {
					if(map[it->v1] == -1 && map[it->v2] == -1) {
						possible_edges.insert(*it);
					}
				}
				if(!possible_edges.empty()) {
					edge e = *possible_edges.begin();
					loc[g.control] = e.v1;
					map[e.v1] = g.control;
					loc[g.target] = e.v2;
					map[e.v2] = g.target;
				} else {
					cout << "no edge available!";
					exit(1);
				}
			} else if(loc[g.control] == -1) {//控制位已经映射，就找距离控制位最近的点作为目标节点
				int min = 1000;
				int min_pos = -1;
				for(int i=0; i< positions; i++) {
					if(map[i] == -1 && dist[i][loc[g.target]] < min) {
						min = dist[i][loc[g.target]];
						min_pos = i;
					}
				}
				map[min_pos] = g.control;
				loc[g.control] = min_pos;
			} else if(loc[g.target] == -1) { //目标位已被映射就把控制位找最近的未被映射的节点
				int min = 1000;
				int min_pos = -1;
				for(int i=0; i< positions; i++) {
					if(map[i] == -1 && dist[loc[g.control]][i] < min) {
						min = dist[loc[g.control]][i];
						min_pos = i;
					}
				}
				map[min_pos] = g.target;
				loc[g.target] = min_pos;
			}
			n.cost_heur = max(n.cost_heur, dist[loc[g.control]][loc[g.target]]);
		} else {
		}
	}

	if(n.cost_heur > 4) {
		n.done = 0;
	}

	memcpy(n.qubits, map, sizeof(int) * positions);
	memcpy(n.locations, loc, sizeof(int) * nqubits);

	nodes.push(n);

	int *used = new int[positions];
	for (int i = 0; i < positions; i++) {
		used[i] = 0;
	}
	edge *edges = new edge[considered_qubits.size()];
	//printLocations(loc);
	//Perform an A* search to find the cheapest permuation
	while (!nodes.top().done) {
		node n = nodes.top();
		nodes.pop();
		expand_node(considered_qubits, 0, edges, 0, used, n, v, dist, next_layer);

		delete[] n.locations;
		delete[] n.qubits;
	}

	node result = nodes.top();
	nodes.pop();

	//clean up
	delete[] used;
	delete[] edges;
	while (!nodes.empty()) {
		node n = nodes.top();
		nodes.pop();
		delete[] n.locations;
		delete[] n.qubits;
	}
	return result;
}

int main(int argc, char** argv) {
	if(argc != 3) {
		cout << "Usage: ./imb_mapping <input_file> <output_file>" << endl;
		exit(0);
	}

	build_graph_QX20();
	// 构造两个点之间的最短距离
	build_dist_table(graph);

	std::ifstream infile(argv[1]);
	read_qasm(infile,argv[2]);

	unsigned int width = 0;
	for (vector<vector<gate> >::iterator it = layers.begin(); it != layers.end(); it++) {
		if ((*it).size() > width) {
			width = (*it).size();
		}
	}
	std::string outfile1;
	outfile1.append("/root/graph/new/quantum_mapping_a_-/compare/total1");
	std::ofstream of1(outfile1,std::ios::app);
	
	std::string outfile2;
	outfile2.append("/root/graph/new/quantum_mapping_a_-/compare/total");

	std::ofstream of2(outfile2,std::ios::app);

	if (!of1.is_open()||!of2.is_open())
	{
		cout<<"total is open fail : "<<of1.is_open()<<" "<<of2.is_open()<<endl;
	}
	char* bName = argv[1];
	of2<<argv[2]<<endl;
	of2<<"initial: " <<ngates<<" "<<layers.size() <<" "<<0<< endl;
	
	of1 << "main_Circuit name: " << bName << " (requires " << nqubits << " qubits)" << endl;
	of1 << endl << "Before mapping: " << endl;
	of1 << "  elementary gates: " << ngates << endl;
	of1 << "  depth: " << layers.size() << endl;

	cout << "main_Circuit name: " << bName << " (requires " << nqubits << " qubits)" << endl;

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
	int swap_count=0;
	vector<gate> all_gates;
	int total_swaps = 0;
try{
	clock_t begin_time = clock();
	//Fix the mapping of each layer
	for (int i = 0; i < layers.size(); i++) {
		
		node result = a_star_fixlayer(i, qubits, locations, dist);

		delete[] locations;
		delete[] qubits;
		locations = result.locations;
		qubits = result.qubits;
		vector<gate> h_gates = vector<gate>();

		//The first layer does not require a permutation of the qubits
		if (i != 0) {
			//Add the required SWAPs to the circuits
			for (vector<vector<edge> >::iterator it = result.swaps.begin();
					it != result.swaps.end(); it++) {
				for (vector<edge>::iterator it2 = it->begin(); it2 != it->end();
						it2++) {

					edge e = *it2;
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
					swap_count+=1;
					all_gates.push_back(cnot);
					all_gates.push_back(h1);
					all_gates.push_back(h2);
					all_gates.push_back(cnot);
					all_gates.push_back(h1);
					all_gates.push_back(h2);
					all_gates.push_back(cnot);
					//Insert a dummy SWAP gate to allow for tracking the positions of the logical qubits
					all_gates.push_back(gg);
					total_swaps++;
				}
			}
		}

		//Add all gates of the layer to the circuit
		vector<gate> layer_vec = layers[i];
		for (vector<gate>::iterator it = layer_vec.begin();
				it != layer_vec.end(); it++) {
			gate g = *it;
			if (g.control == -1) {
				//single qubit gate
				if(locations[g.target] == -1) {
					//handle the case that the qubit is not yet mapped. This happens if the qubit has not yet occurred in a CNOT gate
					gate g2 = g;
					g2.target = -g.target -1;
					all_gates.push_back(g2);
				} else {
					//Add the gate to the circuit
					g.target = locations[g.target];
					all_gates.push_back(g);
				}
			} else {
				//CNOT gate
				g.target = locations[g.target];
				g.control = locations[g.control];

				edge e;
				e.v1 = g.control;
				e.v2 = g.target;

				if (graph.find(e) == graph.end()) {
					//flip the direction of the CNOT by inserting H gates
					e.v1 = g.target;
					e.v2 = g.control;
					if (graph.find(e) == graph.end()) {
						cerr << "ERROR: invalid CNOT: " << e.v1 << " - " << e.v2
								<< endl;
						exit(3);
					}
					gate h;
					h.control = -1;
					strcpy(h.type, "h");
					h.target = g.target;
					all_gates.push_back(h);

					h_gates.push_back(h);
					h.target = g.control;
					all_gates.push_back(h);

					h_gates.push_back(h);
					int tmp = g.target;
					g.target = g.control;
					g.control = tmp;
				}
				all_gates.push_back(g);
			}
		}
		if (h_gates.size() != 0) {
			if (result.cost_heur == 0) {
				cerr << "ERROR: invalid heuristic cost!" << endl;
				exit(2);
			}

			for (vector<gate>::iterator it = h_gates.begin();
					it != h_gates.end(); it++) {
				all_gates.push_back(*it);
			}
		}

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
	double time = double(clock() - begin_time) / CLOCKS_PER_SEC;
	of2 <<"main_Zulehner: "<< all_gates.size()-total_swaps <<" "<<mapped_circuit.size()<<" "<<total_swaps << endl;
	of1 << endl << "After mapping (no post mapping optimizations are conducted): " << endl;
	of1 << "  elementary gates: " << all_gates.size()-total_swaps << endl;
	of1 << "  depth: " << mapped_circuit.size() << endl;
	cout << endl << "After mapping (no post mapping optimizations are conducted): " << endl;
	cout << "  elementary gates: " << all_gates.size()-total_swaps << endl;
	cout << "  depth: " << mapped_circuit.size() << endl;
	of1 << endl << "The mapping required " << time << " seconds" << endl;

	of1 << endl << "Initial mapping of the logical qubits (q) to the physical qubits (Q) of the IBM QX3 architecture: " << endl;

	cout << endl << "The mapping required " << time << " seconds" << endl;

	cout << endl << "Initial mapping of the logical qubits (q) to the physical qubits (Q) of the IBM QX3 architecture: " << endl;

	for(int i=0; i<nqubits; i++) {
		of1 << "  q" << i << " is initially mapped to Q" << locations[i] << endl;
		cout << "  q" << i << " is initially mapped to Q" << locations[i] << endl;
	}

	//Dump resulting circuit

	std::string outfile;
	outfile.append("/root/graph/new/quantum_mapping_a_-/result1/");
	outfile.append(argv[2]);
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
	
	
	cout<<"*_*_*_*_*_swap_count*_*_*_*_*_*:"<<swap_count<<endl;
	of1.close();
	of2.close();
	of.close();
	delete[] locations;
	delete[] qubits;
	delete[] last_layer;
}catch(exception e){
	of1<<e.what()<<endl;
	delete[] locations;
	delete[] qubits;
	of1.close();
	of2.close();
}
	return 0;
}
