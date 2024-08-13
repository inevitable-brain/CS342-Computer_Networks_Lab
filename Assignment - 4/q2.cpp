#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <limits.h>
#include <queue>

using namespace std;

struct packet
{
    double arrival;
    double departure;
    double serviceTime;
    packet()
    {
        arrival = 0;
        departure = 0;
        serviceTime = -1;
    }
    packet(int a, int b, int c)
    {
        arrival = a;
        departure = b;
        serviceTime = c;
    }
};

int PacketDropped;

struct server
{
    int size;
    deque<packet *> q;
    exponential_distribution<double> distribution;
    default_random_engine generator;
    double prev, total, summation, utilisation, avgQueueSize, lambda;
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    server(int m, double n)
    {
        q.clear();
        size = m;
        lambda = n;
        distribution.param(std::exponential_distribution<double>::param_type(lambda));
        prev = 0;
        total = 0;
        summation = 0;
        utilisation = 0;
    }
    void randomise(int j)
    {
        generator.seed(int(chrono::system_clock::now().time_since_epoch().count()) + j);
    }
    int currsize()
    {
        return q.size();
    }
    double enquePacket(packet *p)
    {
        if (q.size() >= size)
        {
            p->departure = -1;
            PacketDropped++;
            return -1;
        }
        total += (p->arrival) - prev;
        if (q.size() != 0)
            utilisation += p->arrival - prev;
        summation += double(q.size()) * (p->arrival - prev);
        prev = p->arrival;
        if (!q.empty())
        {
            p->departure = q.back()->departure + distribution(generator);
            cout << "Service Time = " << p->departure - q.back()->departure << endl;
            p->serviceTime = p->departure - q.back()->departure;
        }
        else
        {
            p->departure = p->arrival + distribution(generator);
            cout << "Service Time = " << p->departure - p->arrival << endl;
            p->serviceTime = p->departure - p->arrival;
        }
        q.push_back(p);
        return p->departure;
    }
    void dequePacket()
    {
        total += q.front()->departure - prev;
        utilisation += q.front()->departure - prev;
        summation += double(q.size()) * (q.front()->departure - prev);
        prev = q.front()->departure;
        q.pop_front();
    }
    void CalAvgQueue(double maxTot)
    {
        while (!q.empty())
        {
            dequePacket();
        }
        avgQueueSize = summation / maxTot;
        utilisation = (utilisation / maxTot) * 100;
    }
};
void simulate(vector<server> &Servers, vector<packet> &Packets, double &avgWaiting)
{
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    int i = 0;
    while (i < Packets.size())
    {
        if (pq.empty() || Packets[i].arrival < pq.top().first)
        {
            int index = -1, length = INT_MAX;
            for (int j = 0; j < Servers.size(); j++)
            {
                if (Servers[j].currsize() < length)
                {
                    index = j;
                    length = Servers[j].currsize();
                }
            }
            cout << "Packet " << i+1 << " Enqued at Server " << index+1 << endl;
            pq.push({Servers[index].enquePacket(&Packets[i]), index});
            if (pq.top().first == -1)
            {
                cout << "Insufficient buffer size, thus packet dropped" << endl << endl;
                PacketDropped++;
            }
            i++;
        }
        else
        {
            int index = pq.top().second;
            if (pq.top().first > 0)
            {
                cout << "server finished job " << index+1 << endl << endl;
                Servers[index].dequePacket();
            }
            pq.pop();
        }
    }
    cout << endl;
    while (!pq.empty())
    {
        int index = pq.top().second;
        if (pq.top().first > 0)
        {
            cout << "server finished job " << index+1 << endl << endl;
            Servers[index].dequePacket();
        }
        pq.pop();
    }
    double maxTot = 0;
    for (int i = 0; i < Servers.size(); i++)
    {
        maxTot = max(maxTot, Servers[i].total);
    }
    for (int i = 0; i < Servers.size(); i++)
    {
        Servers[i].CalAvgQueue(maxTot);
    }
    double avg = 0;
    int total = 0;
    for (auto x : Packets)
    {
        if (x.departure != -1)
        {
            avg += x.departure - x.arrival - x.serviceTime;
            total++;
        }
    }
    avgWaiting = avg / double(total);
}
 
int main()
{
    PacketDropped = 0;
    int seed = chrono::system_clock::now().time_since_epoch().count();
    
    default_random_engine generator(seed);
    
    int n;
    int s;
    int b;
    double l;
    double w;
    cout << "##############################################" << endl;
    cout << "Enter the number of packets:- ";
    cin >> n;
    cout << "Enter lamda(arrival rate):- ";
    cin >> w;
    exponential_distribution<double> distribution(w);
    cout << "Enter Number of servers:- ";
    cin >> s;
    cout << "Enter Buffer size,(For no buffer enter the value 1):- ";
    cin >> b;
    cout << "Enter mu(service rate):- ";
    cin >> l;
    cout << "################################################" << endl;
    vector<server> Servers(s, {b, l});
    for (int i = 0; i < s; i++)
    {
        Servers[i].randomise(i);
    }
    vector<packet> v(n);
    double prev = 0;
    cout << endl;
    cout << "######################### Arrival Times #########################" << endl;
    for (int i = 0; i < n; ++i)
    {
        v[i].arrival = distribution(generator) + prev;
        prev = v[i].arrival;
        cout << "Arrival time of Packet " << i+1 << ":- " << v[i].arrival << endl;
    }
    cout << "#################################################################" << endl;
    cout << endl;
    double avgWaiting;
    
    cout << "######################### Simulation #########################" << endl;
    
    simulate(Servers, v, avgWaiting);
    
    cout << "##############################################################" << endl << endl;
    
    cout << "######################### Departure Times #########################" << endl;
    for (int i = 0; i < n; ++i)
    {
        if(v[i].departure != -1) cout << "Departure time of Packet " << i+1 << ":- " << v[i].departure << endl;
        else cout << "Packet " << i+1 << " has been dropped." << endl;
    }
    cout << "###################################################################" << endl;
    cout << endl;
    cout << "################################################" << endl;
    for (int i = 0; i < Servers.size(); i++)
    {
        cout << "Average Queue Length of Server " << i+1 << ":- " << Servers[i].avgQueueSize << endl << "Utitlisation of Server " << i+1 << ":- " << Servers[i].utilisation << endl << endl;
    }
    cout << "################################################" << endl;
    cout << endl;
    if(avgWaiting >= 0) cout << "Average Waiting Time:- " << avgWaiting << endl;
    else cout << "Average Waiting Time:- 0" << endl;
    
    cout << "Total number of packets dropped:- " << PacketDropped / 2 << endl << endl;
    
    cout << "################################################" << endl;
    return 0;
}
