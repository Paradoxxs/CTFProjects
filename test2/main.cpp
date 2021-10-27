#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <Lmcons.h>

 
using namespace std;
 
#pragma comment (lib, "Ws2_32.lib")
 
#define DEFAULT_BUFLEN 512            
#define IP_ADDRESS "192.168.245.129"
#define DEFAULT_PORT "27015"
 
struct client_type
{
    SOCKET socket;
    int id;
    char received_message[DEFAULT_BUFLEN];
};
 

//Allows the system to revieve data from server 
int process_client(client_type &new_client)
{
    while (1)
    {
        memset(new_client.received_message, 0, DEFAULT_BUFLEN);
 
        if (new_client.socket != 0)
        {
            //prepares to recives from server, with a buffer of 512
            int iResult = recv(new_client.socket, new_client.received_message, DEFAULT_BUFLEN, 0);
 
            if (iResult != SOCKET_ERROR)
            //Prints out the message recived 
                cout << new_client.received_message << endl;
                
            else
            {
                cout << "recv() failed: " << WSAGetLastError() << endl;
                break;
            }
        }
    }
 
    if (WSAGetLastError() == WSAECONNRESET)
        cout << "The server has disconnected" << endl;
 
    return 0;
}
 
int main()
{
    WSAData wsa_data;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
    string sent_message = "test";
    string enc_message = "";
    client_type client = { INVALID_SOCKET, -1, "" };
    int iResult = 0;
    string message;
    char key = 'H';
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (iResult != 0) {
        cout << "WSAStartup() failed with error: " << iResult << endl;
        return 1;
    }
 
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
 
 
    // Resolve the server address and port
    iResult = getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        cout << "getaddrinfo() failed with error: " << iResult << endl;
        WSACleanup();
        system("pause");
        return 1;
    }
 
    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
 
        // Create a SOCKET for connecting to server
        client.socket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (client.socket == INVALID_SOCKET) {
            cout << "socket() failed with error: " << WSAGetLastError() << endl;
            WSACleanup();
            system("pause");
            return 1;
        }
 
        // Connect to server.
        iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(client.socket);
            client.socket = INVALID_SOCKET;
            continue;
        }
        break;
    }
 
    freeaddrinfo(result);
 
    if (client.socket == INVALID_SOCKET) {
        cout << "Unable to connect to server!" << endl;
        WSACleanup();
        system("pause");
        return 1;
    }
 
 
    //Obtain id from server for this client;
/*     recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
    message = client.received_message;
    client.id = atoi(client.received_message); */

    //thread my_thread(process_client);
    while (1)
    {
  
        //getline(cin, sent_message);
        for (int i = 0; i < sent_message.size(); i++)
            sent_message[i] = sent_message[i] ^ key;

        iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0);

        if (iResult <= 0)
        {
            cout << "send() failed: " << WSAGetLastError() << endl;
            break;
            return 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    }

    //Shutdown the connection since no more data will be sent
    //my_thread.detach();
    
 
    closesocket(client.socket);
    WSACleanup();
    system("pause");
    return 0;
}