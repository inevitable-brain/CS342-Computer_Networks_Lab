#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>

using namespace std;

int MAX_BACKOFF;
const int COLLISION_BACKOFF = 6;
const int num_nodes = 4;
const int num_rounds = 20;

int collisions = 0;
int transmissions = 0;
vector<int> backoffvec(num_nodes + 1, 0);

class WirelessNode
{
public:
    WirelessNode(int id)
    {
        node_id = id;
        backoff = rand() % (MAX_BACKOFF + 1);
        transmitting = 0;
        transmitting_time = 1;
        start_time = (rand() % 10);
    }
    void transmit_data(vector<WirelessNode *> &v)
    {
        v.push_back(this);
    }

    void update()
    {
        if (backoff > 0)
        {
            backoff--;
        }
        if (transmitting > 0)
        {
            transmitting--;
        }
    }

    bool is_ready_to_transmit()
    {
        return backoff == 0;
    }

    int node_id;
    int backoff;
    int start_time;
    int transmitting;
    int transmitting_time;
};

int is_channel_busy(vector<WirelessNode> &v)
{
    for (auto node : v)
    {

        if (node.transmitting > 0)
        {
            return node.node_id;
        }
    }
    return 0;
}

void handle_collision(vector<WirelessNode *> &nodes)
{

    for (auto node : nodes)
    {
        node->backoff = (rand() % COLLISION_BACKOFF) + 1;
        node->transmitting = 0;
        cout << "Collision detected for Node " << node->node_id << ". Backing off. with backoff= " << node->backoff << endl;
        // collisions++;
        backoffvec[node->node_id + 1]++;
    }
}

int main()
{

    srand(time(nullptr));
    cout << "Enter value of backoff interval" << endl;
    cin >> MAX_BACKOFF;
    vector<WirelessNode> nodes;
    for (int i = 0; i < num_nodes; i++)
    {
        nodes.push_back(WirelessNode(i));
        cout << "Node: " << nodes[i].node_id << ", transmitting time: " << nodes[i].transmitting_time << endl;
    }

    for (int round = 0; round < num_rounds; round++)
    {
        cout << "Round " << round + 1 << ":" << endl;

        vector<WirelessNode *> can_transmit;

        int busy_node = is_channel_busy(nodes);
        for (WirelessNode &node : nodes)
        {
            if (node.is_ready_to_transmit())
            {

                if (busy_node > 0)
                {
                    cout << "Channel is busy";
                    if (node.node_id != busy_node)
                    {
                        cout << "Node " << node.node_id << " detects channel being used by " << busy_node << endl;
                        node.backoff = rand() % (MAX_BACKOFF + 1);
                    }
                    else
                    {
                        cout << "Node " << busy_node << " is already transmitting" << endl;
                        cout << node.backoff << " " << node.transmitting << " is the backoff and time left" << endl;
                    }
                }
                else
                {
                    node.transmit_data(can_transmit);
                }
            }
            else
            {
                cout << "Node " << node.node_id << " is waiting (backoff = " << node.backoff << ")." << endl;
                backoffvec[node.node_id + 1]++;
            }
        }
        if (can_transmit.size() == 1)
        {
            transmissions++;
            cout << "Node " << can_transmit[0]->node_id << " starts transmitting" << endl;
            can_transmit[0]->transmitting = can_transmit[0]->transmitting_time;
            can_transmit[0]->backoff = 0;
        }
        else
        {
            if (can_transmit.size() > 1)
                collisions++;
            handle_collision(can_transmit);
        }

        for (WirelessNode &node : nodes)
        {
            node.update();
        }

        this_thread::sleep_for(chrono::seconds(0));
        cout << "####################################" << endl;
    }

    cout << "Stats: \n";
    cout << "Transmission time (amount of time slots effectively used) " << transmissions << endl;
    cout << "Collision time (amount of time slots where collision occured) " << collisions << endl;

    for (int i = 0; i < num_nodes; i++)
    {
        cout << "Node " << i << "backoff time : " << backoffvec[i + 1] << endl;
    }
    return 0;
}