/*
 * DFS.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: Santhosh Ramaiah
 */

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdlib>

#define MAX 300 // maximum number of nodes or edges
using namespace std;

enum colors {BLACK, WHITE, GRAY}; // description for node state
int color[MAX], d[MAX], p[MAX], f[MAX],ftemp[MAX], t, vertex,checkDirected;
int edge,k=0,snode,cont=0, roat=0, contrev=0;
vector<int> bkeeping[MAX]={}; // book keeping for edges

int NIL = numeric_limits<int>::min();// 0 value to NIL
bool bflag=false,sflag=true; // bflag- flag for back edge represents cyclic or not. sflag- flag for edge repeat.
bool sccflag=false; // Strongly connected component flag
vector<int> contgra[MAX]={};  // Connected graph book keeping.
vector<int> contgrarev[MAX]={};	//reverse Connected graph book keeping.
void DFS(vector<int>[]); // DFS
void DFS_VISIT(vector<int>[],int, int);// DFS visit


int main(void)
{

		string line,filename;
		char filename1[40];
		int intArray[MAX][MAX]={}; // temporary variable which stores node and its adjacent edges
		int temp; // checks if the graph is directed or not.

		std::vector<int> adjList[MAX]; // list of node and its adjacent edges
		std::vector<int> revadjList[MAX]; // reverse adjacency list for Graph transpose.

	 	cout<<"Enter the name of the graph file: "<<endl;
	 	cin>>filename;
	 	strcpy(filename1, filename.c_str()); //string to character store to pass

	    ifstream myfile; // read the file.
	    myfile.open(filename1);// open the file

	    ifstream resourcefile; // read the file
	    resourcefile.open("RESOURCE_FILE.txt"); // open the resource file
	    if(resourcefile.is_open()){// further expansion for reading resource file nedded.
	    	resourcefile.close();
	    }else{
	    	cout<<"resource file not available"<<endl; // close resource file // used later on.
	    }

	    if (myfile.is_open()) // check if the file is open.
	    {
	      while ( getline (myfile,line) ) // get every line and store it to line
	      {
	    	  std::stringstream ss(line);  // converts string to int
	    	  int templength1=line.length();
	    	  for(int idx = 0; idx < templength1; idx++)
	          {
	        	  ss>>intArray[k][idx]; // store the int to array
	          }
	          k++;
	      }
	      myfile.close();



    vertex=intArray[1][0]; // store the vertex
    checkDirected=intArray[0][0]; // store the directed/undirected graph

    cout<<"Enter the first node to start"<<"(0 to "<<(vertex-1)<<"):"<<endl;
    cin>>snode; // selected node or source node

    for(int e=0; e<vertex; e++) { // read all the vertices
    	for(int ed=0; ed<intArray[e+2][2];ed++){ // read all the edges
			temp=intArray[e+2][ed+3];
			adjList[e].push_back(temp); //for directed graph
			revadjList[temp].push_back(e); //for SCC graph transpose

			if(checkDirected==(-1)){
				adjList[temp].push_back(e);//for undirected graph.
			}
    	}
    }


    cout<<endl;
    cout<<"Classification of edges"<<endl;

    DFS(adjList); // do DFS visit

    cout<<endl;
    cout<<"Vertex (Discovery Time/Finish Time)"<<endl;
    int vert=vertex-1, swap;
    for(int v=0; v<vertex; v++) { // print all the vertices and their discovery/finish time
    		printf("v%d (%d/%d)\n", v, d[v], f[v]);
            ftemp[v]=(f[v]); // store the finish times for sorting
            //bkeeping[0].push_back(v);
        }

    // bubble sort the finish times
   for (int c = 0 ; c < vertex; c++)
   {
	   for (int d = 0 ; d < vertex - c - 1; d++)
	   {
		   if (ftemp[d] < ftemp[d+1]) /* For decreasing order use < */
		   {
			   swap       = ftemp[d];
			   ftemp[d]   = ftemp[d+1];
			   ftemp[d+1] = swap;
		   }
	   }
   }

   // finish times sorted.
   cout<<endl;
	if (checkDirected==0){ // graph is directed
		if (bflag){ // graph is cyclic
			cout<<"Graph is Directed Cyclic."<<endl;
			cout<<"No Topological Sorting"<<endl;
		}
		else{
				cout<<"Graph is Directed Acyclic."<<endl;
				cout<<"The Topological Sorting is:"<<endl;

				for (int i = 0; i <= vert; i++)	{
					for (int j = 0; j <= vert; j++){
						if(ftemp[i]==f[j]){
							cout <<j<< " ";
						}
					}
				}
				cout<<endl;
			}
			cout<<endl;

				sccflag=true;
				DFS(revadjList);

				cout<<"Number of Strongly Connected Components are: "<<contrev<<endl;
				cout<<"Strongly Connected Components are:"<<endl;
				for (int i = 0; i < contrev; i++)	{
					int tempsize1=contgrarev[i].size();
					for (int j = 0; j < tempsize1; j++){
						cout<<contgrarev[i][j]<<" ";
					}cout<<endl;
				}
		}

	else if (checkDirected==(-1)){ // graph is undirected
		if(bflag){ // graph is cyclic
			cout<<"Graph is Undirected Cyclic."<<endl;
		}
		else{
			cout<<"Graph is Undirected Acyclic."<<endl;
		}
		cout<<"Number of Connected Components are: "<<cont<<endl;
		cout<<"Connected Components are:"<<endl;

		for (int i = 0; i < cont; i++)	{
			int tempsize2=contgra[i].size();
			for (int j = 0; j < tempsize2; j++){
				cout<<contgra[i][j]<<" ";
				}cout<<endl;
		}
	}
	else {cout<<"File Read Error.";}

	    }

	    else cout << "Unable to open file";

   return 0;
}



void DFS(vector<int> G[]) {
    for(int u=0; u<vertex; u++) { // initially paint all nodes white
        color[u] = WHITE;
        p[u] = NIL;
    }
    t = 0; // reset time

    if(sccflag==false){ // do if not of strongly connected graph
		if(color[snode] == WHITE) { // do DFS visit for source node
			contgra[cont].push_back(snode);
			DFS_VISIT(G,snode,cont);
			cont++;
		}

		for(int u=0; u<vertex; u++) { // do DFS visit for all nodes other than source node.
			if(u==snode){continue; 	} // not needed as it is backed by color.
			else{
			if(color[u] == WHITE) {
				contgra[cont].push_back(u);
				DFS_VISIT(G,u,cont);
				cont++;
			}
			}
		}
    }
    else{ // for SCC graph transpose node sending in reverse of finish times
    	for (int i = 0; i < vertex; i++)	{
    		for (int u = 0; u < vertex; u++){
    			if(ftemp[i]==f[u]){
    				if(color[u] == WHITE) {
    					contgrarev[contrev].push_back(u);
    					DFS_VISIT(G,u,contrev);
    					contrev++;
    				}
    			}
    		}
    	}
    }
}

void DFS_VISIT(vector<int> G[], int u, int contg) {
    t = t + 1; // increment time
    d[u] = t; // put discovery time for node
    color[u] = GRAY; // paint gray to node
    int tempsize3=G[u].size();
    for(int v=0; v<tempsize3; v++) { // do for all adjacent nodes

			for (int sj=0;sj<roat;sj++){ //check for edge repetition.
    			if(((bkeeping[sj][1]==u)&&(bkeeping[sj][0]==G[u][v]))||((bkeeping[sj][1]==u)&&(bkeeping[sj][0]==G[u][v]))){
					sflag=false;
					break;
				}
					else{
						sflag=true;

					}
				}
			bkeeping[roat].push_back(u); //put edge in book keeping array.
			bkeeping[roat].push_back(G[u][v]); //put edge in book keeping array.

			roat++;

		if(color[G[u][v]] == WHITE) { // check for nodes which are undiscovered

            p[G[u][v]] = u;
            if(sccflag==false){// check for SCC
            	contgra[contg].push_back(G[u][v]);
            	cout<<"Edge "<<u<<"----"<<G[u][v]<<" Tree Edge"<<endl;
            }
            else{
            	contgrarev[contg].push_back(G[u][v]);
            }

            DFS_VISIT(G,G[u][v],contg);

        }

        else if(color[G[u][v]] == GRAY&&sflag==true&&sccflag==false){ //check for back edge
        	cout<<"Edge "<<u<<"----"<<G[u][v]<<" Back Edge"<<endl;
        	bflag=true;
        }
        else if(color[G[u][v]] == BLACK&&sflag==true&&sccflag==false){ // check for forward/cross edge
        		if(d[u]<d[v]){
                	cout<<"Edge "<<u<<"----"<<G[u][v]<<" Forward Edge"<<endl;
                }
        		else{
        			cout<<"Edge "<<u<<"----"<<G[u][v]<<" Cross Edge"<<endl;
        		}
        }
        sflag=false;
    }
    color[u] = BLACK; // paint node black
    t = t + 1; //increment time
    f[u] = t; // put finish time for node
}
