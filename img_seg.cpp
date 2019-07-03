#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <algorithm>
#include <iterator>
using namespace std;

#define MAX_NEI 6

void parseInput();

// u -> v   flow/capacity
struct Edge
{
	int v;
	int flow = 0;
	int cap;
};

// height and excess and neigbhor_head and neighbor list
struct Vertex {
	bool seg = false;
	int h = 1;
	int e = 0;
	unsigned int nei_i = 0;
	vector<Edge> neighb;
};

int m, n, V;
vector<Vertex> Graph;
vector<int> gap;
int pushcnt = 0;
int relabelcnt = 0;

void printheights()
{
	for (std::size_t i = 0; i < Graph.size(); i++)
	{
		// cout << " "<< i << ":" << Graph[i].h;
		cout << " "<< Graph[i].h;
	}
	cout << "\n";
}

void printgapheights()
{
	for(int i : gap)
	{
		cout<<i<<" ";
	}
	cout<<"\n";
}

void Preflow()
{
	//alture da source = |V|
	Graph[V].h = V;

	for (int i = 0; i < V; i++)
	{
		for(Edge e : Graph[i].neighb)
		{
			if(e.v == V) {Graph[i].e = e.cap;}
		}
	}
}

void relabel(int u)
{
	int min = INT32_MAX;

	gap[Graph[u].h]--;

	// Encontrar o vizinho de u com a menor altura e colocar a altura
	// de u igual a altura desse vizinho mais um
	for (auto e : Graph[u].neighb)
	{
		int v = e.v;
		if (e.cap - e.flow > 0  && Graph[v].h < min && Graph[v].h >= Graph[u].h)
		{
			min = Graph[v].h;
			// Graph[u].h = min + 1;
		}
	}
	Graph[u].h = min + 1;
	
	gap[min + 1]++;
}

int push(int u, int v, int residual)
{
	// nao podemos mandar mais do que o edge suporta ou do que u tem
	int amount = min(Graph[u].e, residual);

	for (unsigned int i = 0; i < Graph[u].neighb.size(); ++i)
	{
		if (Graph[u].neighb[i].v == v) {
			Graph[u].neighb[i].flow += amount;
		}
	}
	for (unsigned int i = 0; i < Graph[v].neighb.size(); ++i)
	{
		if (Graph[v].neighb[i].v == u) {
			Graph[v].neighb[i].flow -= amount;
		}
	}

	// Graph[u].neighb[v].flow += amount;
	// Graph[v].neighb[u].flow -= amount;

	Graph[u].e -= amount;
	Graph[v].e += amount;

	// cout<<amount<<"\n";
	return 0;
}

void Discharge(int u)
{

	while (Graph[u].e > 0)
	{
		// obter vizinho corrente
		unsigned int curr_nb = Graph[u].nei_i;


		// se percorremos todos vizinhos
		if (curr_nb >= Graph[u].neighb.size())
		{
			relabel(u);
			relabelcnt++;
			// colocar iterador para head da lista de vizinhos
			Graph[u].nei_i = 0;
		} 
		else
		{
			int v = Graph[u].neighb[curr_nb].v;
			int cap = Graph[u].neighb[curr_nb].cap;
			int flow = Graph[u].neighb[curr_nb].flow;

			if (cap - flow > 0 && Graph[u].h == (Graph[v].h + 1))
			{
				push(u, v, cap - flow);
				pushcnt++;
				// switch neibh here
				// if(curr_nb>=Graph[u].neighb.size()){
				// cout<<u<<":"<<curr_nb<<" "<< Graph[u].neighb.size()-1<<"\n";
				// }
				// unsigned int n = Graph[u].nei_i;
				// Edge t = Graph[u].neighb[0];
				// Graph[u].neighb[0] = Graph[u].neighb.at(n);
				// Graph[u].neighb[curr_nb] = t;

				// cout<< "we pushed " << u << " to " << v << '\n';
			} 
			else { Graph[u].nei_i++; }
		}
	}
}

void gapHeuristic(deque<int> &L, int k)
{
	for (deque<int>::iterator i = L.begin(); i != L.end(); ++i)
	{
		if(Graph[*i].h > k)	{ L.erase(i); }
	}

	for(int i = 0; i < V; ++i)
	{
		if(Graph[i].h > k) { Graph[i].seg = true; Graph[i].h=V+1;}
	}
}

void RelabelToFront() 
{
	// // initialize preflow
	Preflow();

	// set up discharge List without S and t
	deque<int> L;
	L.resize(V);
	for (int i = 0; i<V; i++) { L[i]=i; }

	auto currInd = L.begin();
	int gapheight = 0;

	while (currInd != L.end())
	{
		int u = *currInd;
		int oldHeight = Graph[u].h;

		Discharge(u);

		// vertex foi relabeled se altura alterar
		if (Graph[u].h > oldHeight)
		{
			// Gap heuristic
			if (gap[oldHeight] == 0)
			{
				gapheight = oldHeight;
				gapHeuristic(L, oldHeight);
			} else {
				// colocar u na frente de L
				L.erase(currInd);
				L.push_front(u);
			}
			currInd = L.begin();
		} else { currInd++; }

		// printheights();
		// cout<<Graph[V+1].e<<"\n";
		// printgapheights();
	}

	for (int i = 0; i < V; ++i)
	{
		if (Graph[i].h > gapheight)
		{
			Graph[i].seg = true;
		}
	}
}


int main() 
{
	string line;

	char *e;
	getline(cin, line);
	m = strtol(line.c_str(), &e, 10);
	n = strtol(e, NULL, 10);
	V = m*n;
	getline(cin, line); // empty line
	
	Graph.resize(V+2);
	gap.resize(V+2);
	gap[1] = V;
	Graph[V+1].h = 0;

	parseInput();

	// for(auto i : Graph)
	// {
	// 	for(auto j : i.neighb)
	// 	{
	// 		cout<< j.v<<" ";
	// 	}
	// 	cout<<"\n";
	// }

	RelabelToFront();

	cout<<"Push: "<<pushcnt<<" Rel: "<<relabelcnt<<"\n";
	cout << Graph[V + 1].e << "\n\n";

	// Print do grafo segmentado final
	int u = 0; string label;
	for (int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if (Graph[u].seg){label = "C";} else {label = "P";}
				cout << label << " ";
			u++;
		}
		cout<<"\n";
	}


	return 0;
}

void parseInput()
{
	string line;
	int index = 0;

	//edges from source    primeiro plano
	for (int i = 0; i < m; i++)
	{
		getline(cin, line);
		const char *l = line.c_str();
		char *end;
		for (int j = 0; j < n; j++)
		{
			int t = strtol(l, &end, 10);
			l = end;


			Edge e; e.cap = t; e.v = V;
			Graph[index].neighb.push_back(e);
			// Graph[index].neighb[V].cap = t;
			index++;
		}
	}
	// cout <<index<<"\n";
	index = 0;
	getline(cin, line); // empty line

	// edges to sink   cenario
	for (int i = 0; i < m; i++)
	{
		getline(cin, line);
		const char *l = line.c_str();
		char *end;
		for (int j = 0; j < n; j++)
		{
			int t = strtol(l, &end, 10);
			l = end;

			Edge e; e.cap = t; e.v = V+1;
			Graph[index].neighb.push_back(e);
			// Graph[index].neighb[V+1].cap = t;
			index++;
		}
	}
	getline(cin, line); // empty line

	if (m > 1)
	{
		int ui = 0;
		int vi = 1;

		// edges horizontais
		for (int i = 0; i < m; i++)
		{
			getline(cin, line);
			const char *l = line.c_str();
			char *end;
			for (int j = 0; j < n-1; j++)
			{
				int t = strtol(l, &end, 10);
				l = end;

				Edge e;	e.cap = t; e.v = vi;
				Graph[ui].neighb.push_back(e);
				e.v = ui;
				Graph[vi].neighb.push_back(e);
				// Graph[ui].neighb[vi].cap = t;
				// Graph[vi].neighb[ui].cap = t;

				ui++; vi++;
			}
			ui++;vi++;
		}
		getline(cin, line); // empty line

		ui = 0;
		vi = n;

		// edges verticais
		for (int i = 0; i < m-1; i++)
		{
			getline(cin, line);
			const char *l = line.c_str();
			char *end;
			for (int j = 0; j < n; j++)
			{
				int t = strtol(l, &end, 10);
				l = end;


				Edge e;	e.cap = t; e.v = vi;
				Graph[ui].neighb.push_back(e);
				e.v = ui;
				Graph[vi].neighb.push_back(e);
				// Graph[ui].neighb[vi].cap = t;
				// Graph[vi].neighb[ui].cap = t;

				ui++; vi++;
			}
		}
	}
}