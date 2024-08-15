// // #include <iostream>
// // #include <cstring>
// // #include <cstdlib>
// // #include <sys/socket.h>
// // #include <netinet/in.h>
// // #include <arpa/inet.h>
// // #include <unistd.h>
// // #include <pthread.h>
// // #include <string>

// // void ReceiveMessages(int clientSocket) {
// //     char buffer[1024];
// //     int bytesRead;

// //     while (true) {
// //         bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
// //         if (bytesRead <= 0) {
// //             std::cout << "Server disconnected." << std::endl;
// //             break;
// //         }
// //         buffer[bytesRead] = '\0';
// //         std::cout << "Server: " << buffer << std::endl;
// //     }
// // }

// // void* ReceiveMessagesThread(void* arg) {
// //     int clientSocket = *reinterpret_cast<int*>(arg);
// //     ReceiveMessages(clientSocket);
// //     return nullptr;
// // }

// // int main(int argc, char* argv[]) {
// //     if (argc != 4) {
// //         std::cerr << "Usage: " << argv[0] << " <Server_IP_Address> <Server_Port_Number> <User_Name> " << std::endl;
// //         return 1;
// //     }

// //     const char* serverIP = argv[1];
// //     int serverPort = std::atoi(argv[2]);
// //     const char* user_name = argv[3];

// //     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
// //     if (clientSocket == -1) {
// //         std::cerr << "Error creating socket." << std::endl;
// //         return 1;
// //     }

// //     struct sockaddr_in serverAddr;
// //     serverAddr.sin_family = AF_INET;
// //     serverAddr.sin_port = htons(serverPort);
// //     serverAddr.sin_addr.s_addr = inet_addr(serverIP);

// //     if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
// //         std::cerr << "Error connecting to server." << std::endl;
// //         close(clientSocket);
// //         return 1;
// //     }

// //     std::cout << "Connected to server." << std::endl;

// //     send(clientSocket, user_name, strlen(user_name), 0);

// //     pthread_t receiveThread;
// //     pthread_create(&receiveThread, nullptr, &ReceiveMessagesThread, &clientSocket);

// //     char buffer[1024];
// //     while (true) {
// //         std::string message = "";
// //         std::cout << "You: ";
// //         std::getline(std::cin, message);

// //         if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
// //             std::cerr << "Error sending data." << std::endl;
// //             break;
// //         }

// //         if (message == "/exit") {
// //             std::cout << "Exiting..." << std::endl;
// //             break;
// //         }
// //     }

// //     close(clientSocket);

// //     return 0;
// // }




// #include <iostream>
// #include <cstring>
// #include <cstdlib>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <pthread.h>
// #include <string>
// #include<bits/stdc++.h>
// using namespace std;
// void ReceiveMessages(int clientSocket) {
//     char buffer[1024];
//     int bytesRead;

//     while (true) {
//         bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
//         if (bytesRead <= 0) {
//             std::cout << "Server disconnected." << std::endl;
//             break;
//         }
//         buffer[bytesRead] = '\0';
//         std::cout << "Server: " << buffer << std::endl;
//     }
// }

// void* ReceiveMessagesThread(void* arg) {
//     int clientSocket = *reinterpret_cast<int*>(arg);
//     ReceiveMessages(clientSocket);
//     return nullptr;
// }

// int main(int argc, char* argv[]) {
//     if (argc != 4) {
//         std::cerr << "Usage: " << argv[0] << " <Server_IP_Address> <Server_Port_Number> <User_Name> " << std::endl;
//         return 1;
//     }

//     const char* serverIP = argv[1];
//     int serverPort = std::atoi(argv[2]);
//     const char* user_name = argv[3];

//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
//     if (clientSocket == -1) {
//         std::cerr << "Error creating socket." << std::endl;
//         return 1;
//     }

//     struct sockaddr_in serverAddr;
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(serverPort);
//     serverAddr.sin_addr.s_addr = inet_addr(serverIP);

//     if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
//         std::cerr << "Error connecting to server." << std::endl;
//         close(clientSocket);
//         return 1;
//     }

//     std::cout << "Connected to server." << std::endl;

//     send(clientSocket, user_name, strlen(user_name), 0);

//     pthread_t receiveThread;
//     pthread_create(&receiveThread, nullptr, &ReceiveMessagesThread, &clientSocket);

//     char buffer[1024];
//     while (true) {
//         cout<<"if you want to send to server the start the msg with 0 else for broadcasting start with 1\n"
//         string message = "";
//         cout << "You: ";
//         getline(std::cin, message);

//         if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
//             std::cerr << "Error sending data." << std::endl;
//             break;
//         }

//         if (message == "/exit") {
//             std::cout << "Exiting..." << std::endl;
//             break;
//         }
//     }

//     close(clientSocket);

//     return 0;
// }


#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <cstdint>
#include <stdint.h>
using namespace std;

void ReceiveMessages(int clientSocket) {
    char buffer[1024];
    int bytesRead;

    while (true) {
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            cout << "Server disconnected." << endl;
            break;
        }
        buffer[bytesRead] = '\0';
        cout << buffer << endl;
    }
}

void* ReceiveMessagesThread(void* arg) {
    // int clientSocket = reinterpret_cast<int>(arg);
    int clientSocket = *(int *)arg;
    ReceiveMessages(clientSocket);
    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <Server_IP_Address> <Server_Port_Number> <User_Name> " << endl;
        return 1;
    }

    const char* serverIP = argv[1];
    int serverPort = atoi(argv[2]);
    const char* user_name = argv[3];

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Error creating socket." << endl;
        return 1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = inet_addr(serverIP);

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        cerr << "Error connecting to server." << endl;
        close(clientSocket);
        return 1;
    }

    cout << "Connected to server." << endl;
    cout<<"if you want your msg to be broadcasted start your msg with '_:'\n else if you want to send to a particular user then start your msg by 'username:'\n";

    send(clientSocket, user_name, strlen(user_name), 0);

    pthread_t receiveThread;
    pthread_create(&receiveThread, nullptr, &ReceiveMessagesThread, &clientSocket);

    char buffer[1024];
    while (true) {
        string message = "";

        getline(cin, message);

        if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
            cerr << "Error sending data." << endl;
            break;
        }

        if (message == "/exit") {
            cout << "Exiting..." << endl;
            break;
        }
    }

    close(clientSocket);

    return 0;
}