#include <bits/stdc++.h>
 
using namespace std;
int unique_id = 1;
 
int n;
int total_req = 0;
class HttpRequest
{
public:
    int id;
    int website_id;
    int processing_time;
 
    HttpRequest(int website_id, int processing_time)
    {
        this->id = unique_id++;
        this->website_id = website_id;
        this->processing_time = processing_time;
    }
};
class Website
{
public:
    int id;
    int owner_id;
    int bandwidth;
    int processing_power;
    vector<HttpRequest> requests;
    Website(){}
    Website(int id, int owner_id, int bandwidth, int processing_power)
    {
        this->id = id;
        this->owner_id = owner_id;
        this->bandwidth = bandwidth;
        this->processing_power = processing_power;
    }
};
 
unordered_map<int, Website> websites;
class LoadBalancer
{
public:

 
    void add_website(int website_id, int owner_id, int bandwidth, int processing_power)
    {
        websites[website_id] = Website(website_id, owner_id, bandwidth, processing_power);
    }
 
    void enqueue_request(HttpRequest request)
    {
        int time = request.processing_time;
        request.processing_time = 1;
        total_req += time-1;
        for (int i = 0; i < time; i++)
        {
            websites[request.website_id].requests.push_back(request);   
        }
    }
 
    HttpRequest dequeue_request(int website_id)
    {
        HttpRequest request = websites[website_id].requests.front();
        websites[website_id].requests.erase(websites[website_id].requests.begin());
        return request;
    }
 
    void wfq_scheduling()
    {
        int again = 1;
        unordered_map<int, float> share;
        unordered_map<int, int> times;
        while (total_req)
        {
            int total_weight = 0;
            if (again)
            {
                for (int i = 1; i <= n; i++)
                {
                    if (websites[i].requests.size() == 0)
                    {
                        continue;
                    }
                    total_weight += websites[i].bandwidth * websites[i].processing_power;
                }
 
                float min_share = 2.5000;
 
                for (int i = 1; i <= n; i++)
                {
                    if (websites[i].requests.size() == 0)
                    {
                        continue;
                    }
                    share[websites[i].id] = (float)(websites[i].bandwidth * websites[i].processing_power) / (float)(total_weight);
                    min_share = min(min_share, share[websites[i].id]);
                }
 
                for (int i = 1; i <= n; i++)
                {
                    if (websites[i].requests.size() == 0)
                    {
                        continue;
                    }
                    times[websites[i].id] = ceil(share[websites[i].id] / min_share);
                }
                again = 0;
            }
            for (int i = 1; i <= n; i++)
            {
                if (websites[i].requests.size() == 0)
                {
                    continue;
                }
                int left = websites[i].requests.size();
                for (int j = 0; j < min(left, times[websites[i].id]); j++)
                {
                    HttpRequest request = this->dequeue_request(websites[i].id);
                    cout << "Packet of request " << request.id << " from Website " << websites[i].id << endl;
                    total_req--;
                }
                if (websites[i].requests.size() == 0)
                {
                    again = 1;
                }
            }
        }
    }
};
 
int main()
{

 
    cout << "Please enter the number of websites to be used for simulation: ";
    cin >> n;
    LoadBalancer lb;
    for (int i = 0; i < n; i++)
    {
        int band, pow, req;
        cout << "Please enter the bandwidth of the website with number " << i + 1 << ": "<<endl;
        cin >> band;
        cout << "Please enter the processing power of the webiste with number " << i + 1 << ": "<<endl;
        cin >> pow;
        lb.add_website(i + 1, i + 1, band, pow);
        cout << "Please enter the number of requests for website with number " << i + 1 << ": "<<endl;
        cin >> req;
        total_req += req;
        for (int j = 0; j < req; j++)
        {
            lb.enqueue_request(HttpRequest(i+1, (rand()%5)+1));
        }
    }
    cout<<"---------------------------------------------------------------------------\n";
    cout<<"-------------------------Problem Scenario----------------------------------\n";
    cout<<"---------------------------------------------------------------------------\n";
    for (int i = 1; i <= n; i++)
    {
        cout<<"Website "<<i<<" :-\n";
        map<int,int> m;
        for (int j = 0; j < websites[i].requests.size() ; j++)
        {
            m[websites[i].requests[j].id] += websites[i].requests[j].processing_time;
        }
        for (auto &id : m)
        {
            cout<<"Request "<<id.first<<" with processing time = "<<id.second<<endl;
        }
    }
    cout<<"---------------------------------------------------------------------------\n";
    cout<<"-------------------------Simulation----------------------------------------\n";
    cout<<"---------------------------------------------------------------------------\n";
 
    lb.wfq_scheduling();
 
    return 0;
}
