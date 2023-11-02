#include <iostream>
#include <vector>
#include <set>

using namespace std;

#define INF 1e9

class Router
{
    public:

        int router_id;
        vector< pair< Router*,int > > Neighbours;
        vector<int> RT_dist;
        vector<int> Routing_Table;

        Router(int _id,int n)
        {
            this->router_id = _id;
            RT_dist.resize(n+1, INF);
            Routing_Table.resize(n+1,-1);
        }

        void Add_Neighbour(Router*neighbour,int weight)
        {
            pair<Router *, int> p = make_pair(neighbour,weight);
            Neighbours.push_back(p);
        }

        void Update_Routing_Table()
        {
            RT_dist[this->router_id]=0;

            Routing_Table[this->router_id]=router_id;
            multiset< pair< int,Router* > > m_st;

            pair<int, Router *> x = make_pair(0,this);
            m_st.insert(x);

            while (m_st.size()!=0)
            {
                pair<int,Router*> node = *(m_st.begin());
                m_st.erase(m_st.begin());
                int d = node.first;
                Router *router = node.second;

                for(auto child:router->Neighbours)
                {
                    Router *cf = child.first;
                    int wt = child.second;

                    if (RT_dist[cf->router_id] > d + wt)
                    {
                        Routing_Table[cf->router_id] = router->router_id;
                        RT_dist[cf->router_id] = d + wt;
                        x = make_pair(d + wt, cf);
                        m_st.insert(x);
                    }
                }
            }
        }

        void Print_Dijkstra()
        {
            int n = Routing_Table.size()-1;
            for(int i = 1; i <=n; i++)
            {
                if (i==router_id)
                {
                    continue;
                }

                int link;
                if (Routing_Table[i]==this->router_id)
                {
                    link=i;
                    cout << i << ": - " << link << " -> " << this->router_id << endl;
                }
                else
                {
                    link=i;
                    cout << i << ": - ";
                    while (Routing_Table[link]!=this->router_id)
                    {
                        cout << link << " -> ";
                        link=Routing_Table[link];
                    }
                    cout  << link << " -> " << this->router_id << endl;
                }
            }
        }

        void Print_Routing_Table()
        {
            int n=Routing_Table.size()-1;

            for (int i = 1; i <=n; i++)
            {
                if (i==router_id)
                {
                    continue;
                }

                int link;
                if (Routing_Table[i]==this->router_id)
                {
                    link=i;
                }
                else
                {
                    link=i;
                    while (Routing_Table[link]!=this->router_id)
                    {
                        link=Routing_Table[link];
                    }
                }

                cout << i << ": - " << router_id << " -> " << link << endl;
            }
        }
};

int main(){

    cout << "Enter the number of nodes and total edges in the system " << endl;

    int n,m;
    cin >> n >> m;

    vector< Router * > Routers(n+1);
    for (int i = 1; i <=n; i++)
    {
        Routers[i] = new Router(i,n);
    }

    cout << "Enter the edges of the system " << endl;

    for (int i = 0; i < m; i++)
    {
        int node1,node2,wt;
        cin >> node1 >> node2 >> wt;
        
        Routers[node1]->Add_Neighbour(Routers[node2],wt);
        Routers[node2]->Add_Neighbour(Routers[node1],wt);
    }

    for (int i = 1; i <=n; i++)
    {
        Routers[i]->Update_Routing_Table();
    }

    while (1)
    {
        cout<<"Do you want to Exit?(press y) "<<endl;

        string s;
        cin>>s;
        if (s=="y")
        {
            break;
        }

        cout << "Enter d if you want to see dijkstra path of a node.\n";
        cin >> s;

        if(s == "d"){
            cout<<"Enter node whose Dijkstra Path you want "<<endl;
            int node;
            cin>>node;
            
            Routers[node]->Print_Dijkstra();
        }
        else{
            cout<<"Enter node whose Routing Table you want "<<endl;
            int node;
            cin>>node;
            
            Routers[node]->Print_Routing_Table();
        }
    }

    return 0;
}