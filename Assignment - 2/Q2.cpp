#include <iostream>
#include <string>
#include <unistd.h>
#include <list>
#include <sys/socket.h>
#include <unordered_map>
#include <cstring>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

class PageNode
{
public:
    string url;
    string content;
};

const int SIZE_OF_CACHE = 5;

struct Hf
{
    size_t operator()(const PageNode &page) const
    {
        size_t ret = hash<string>()(page.url);
        return ret;
    }
};

struct Presence
{
    bool operator()(const PageNode &lf, const PageNode &rg) const
    {
        string a = lf.url;
        string b = rg.url;
        if (a == b)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

unordered_map<PageNode, list<PageNode>::iterator, Hf, Presence> mp;
list<PageNode> llcache;

string f(string res)
{
    size_t constartind = res.find("\r\n\r\n");
    constartind += 4;
    if (constartind != string::npos)
    {
        return res.substr(constartind);
    }
    else
    {
        cerr << "Error parsing response" << endl;
        return "";
    }
}

string getContentOfURL(string &url)
{
    int hstartIndex = url.find("://");
    hstartIndex += 3;
    int hendIndex = url.length();
    for (int i = hstartIndex + 1; i < url.size(); i++)
    {
        if (url[i] == '/')
        {
            hendIndex = i;
            break;
        }
    }

    int length = hendIndex - hstartIndex;
    string host = url.substr(hstartIndex, length);
    string path;
    if (hendIndex == url.length())
    {
        path = "/";
    }
    else
    {
        path = url.substr(hendIndex);
    }

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        cerr << "Socket could not be created" << endl;
        return "";
    }

    struct hostent *si = gethostbyname(host.c_str());
    if (si == NULL)
    {
        cerr << "Resolution of hostname failed" << endl;
        close(sd);
        return "";
    }

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(80);
    memcpy(&sa.sin_addr.s_addr, si->h_addr, si->h_length);

    int k = connect(sd, (struct sockaddr *)&sa, sizeof(sa));
    if (k == -1)
    {
        cerr << "Could not connect to the server" << endl;
        close(sd);
        return "";
    }

    string req = "GET " + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";

    int l = send(sd, req.c_str(), req.length(), 0);
    if (l == -1)
    {
        cerr << "Request could not be sent" << endl;
        close(sd);
        return "";
    }

    string res;
    char buffer[4096];
    int bytesRead = recv(sd, buffer, sizeof(buffer), 0);
    while ((bytesRead) > 0)
    {
        res.append(buffer, bytesRead);
        bytesRead = recv(sd, buffer, sizeof(buffer), 0);
    }

    close(sd);

    string ret = f(res);
    return ret;
}
string g(string &url)
{
    string text = getContentOfURL(url);
    if (text == "")
    {
        return text;
    }
    PageNode insertPageNode;
    insertPageNode.url = url;
    insertPageNode.content = text;
    llcache.push_front(insertPageNode);
    mp[insertPageNode] = llcache.begin();
    return text;
}
string modifyCache(string &url)
{
    PageNode *node = new PageNode();
    node->url = url;
    node->content = "";
    auto x = mp.find(*node);
    if (x != mp.end())
    {
        llcache.splice(llcache.begin(), llcache, x->second);
        string ans = x->second->content;

        // PageNode &evictPageNode = llcache.back();
        // mp.erase(evictPageNode);
        // llcache.pop_back();

        // PageNode &InsertPageNode = evictPageNode;
        // llcache.push_front(InsertPageNode);
        // mp[InsertPageNode] = llcache.begin();

        return ans;
    }
    else
    {
        // string text = getContentOfURL(url);
        // PageNode insertPageNode;
        // insertPageNode.url = url;
        // insertPageNode.content = text;
        // llcache.push_front(insertPageNode);
        // mp[insertPageNode] = llcache.begin();
        string text = g(url);
        if (mp.size() > SIZE_OF_CACHE)
        {
            PageNode &evictPageNode = llcache.back();
            mp.erase(evictPageNode);
            llcache.pop_back();
        }
        return text;
    }
}

void print()
{
    cout << "The current cache size:" << mp.size() << endl;
    int cnt = 1;
    for (auto &x : llcache)
    {
        cout << cnt << ": " << x.url << endl;
        cnt++;
    }
}

int main()
{
    string a;
    while (true)
    {
        cout << "Enter URL (or 'exit' to quit): ";
        cin >> a;
        if (a == "exit")
        {
            break;
        }

        string pageContent = modifyCache(a);
        if (!pageContent.empty())
        {
            cout << "Page content:\n"
                 << endl;
            cout << pageContent << endl;
            for (int i = 0; i < 40; i++)
            {
                cout << "-";
            }
            cout << endl;
            cout << "Current status of the cache" << endl;
            print();
            for (int i = 0; i < 40; i++)
            {
                cout << "-";
            }
            cout << endl;
            for (int i = 0; i < 40; i++)
            {
                cout << "-";
            }
            cout << endl;
        }
    }

    return 0;
}