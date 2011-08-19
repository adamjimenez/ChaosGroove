// Net (.Hpp)
// ----------
#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include "KPTK.h"
#include "time.h"

using namespace std;

#include "bridge.hpp"
#include "game.hpp"
#include "log.hpp"
#include "timer.hpp"
#include "scene.hpp"
#include "config.hpp"

#include "net.hpp"
#include "chat.hpp"
/*
#ifdef WIN32
    //#include <winsock2.h>
    #include <windows.h>
#else
    #include <netinet/in.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <sys/socket.h>
#endif

//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

#define MAX_CLIENTS 32
#define SERVER_PORT 60000

struct net_t net;
RakPeerInterface *peer;
Packet *packet;

void net_find_ip(void)
{
 PHOSTENT host;
 char lpszHostName[50]; // will accept the host name
             
#ifdef WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1, 1), &wsaData);
#endif  

 if (gethostname(lpszHostName, 50) != 0)
 {
  printf("gethostname() generated error %d", WSAGetLastError());
 }
 else
 {
  // get the host entry structure for this machine
  if ((host = gethostbyname(lpszHostName)) == NULL)
  {
   printf("gethostname() generated error %d", WSAGetLastError());
  }
  else
  {
   sprintf(net.IP, "%s\n", inet_ntoa(*(struct in_addr*)host->h_addr));
  }
 }

 WSACleanup();
}

void PrintMessage(RPCParameters *rpcParameters)
{
 char str[512];
 int message, m, id;

 RestoreWindow();

 sprintf(str, "%s\n", rpcParameters->input);
 log("%s", str);
 log("%d", str[0]);

 message = str[0];

 for (m = 0 ; m < strlen(str) - 1 ; m++)
 {
  str[m] = str[m + 1];
 }
 str[m + 1] = 0;
 //log("length new: %d", strlen(str));
 //log("message: %d, %s", message, str);
 
 if (message == MESSAGE_NEW_USER)
 {
	if (net.server)
	{
	 id = find_free_user_spot();
	 //log("id: %d", id);
   add_user(str, id);

   //log("sending all existing user info to new user..");
   for (m = 0 ; m < MAX_USERS ; m++)
   {
    if (user[m].id != -1)
		{
	   sprintf(str, "%c%c%s", MESSAGE_ADD_USER, m, user[m].name);

	   peer->RPC("PrintMessage", str, (strlen(str)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
	   rpcParameters->sender, false, 0, UNASSIGNED_NETWORK_ID,0);
		}
	 }

	 // Give new user his/her ID value.
	 sprintf(str, "%c%c", MESSAGE_USER_ID, id);
	 peer->RPC("PrintMessage", str, (strlen(str)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
	 rpcParameters->sender, false, 0, UNASSIGNED_NETWORK_ID,0);

	 // Tell everyone else to add the new user.
	 sprintf(str, "%c%c%s", MESSAGE_ADD_USER, id, user[id].name);
	 peer->RPC("PrintMessage", str, (strlen(str)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
	 rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
 }

 if (message == MESSAGE_ADD_USER)
 {
  id = str[0];
	//log("ID: %d", id);
	add_user(str, id);
 }

 if (message == MESSAGE_REMOVE_USER)
 {
  remove_user(str[0]);
 }

 if (message == MESSAGE_USER_ID)
 {
  chat.user_id = str[0];
 }

 if (message == MESSAGE_CHAT)
 {
  add_chat_line(str, Rgba(1.0, 1.0, 1.0));
 }

 // If we are the server, send message back to other clients (apart from the one it came from)
 if (net.server)
 {
	peer->RPC("PrintMessage",(const char*)rpcParameters->input, rpcParameters->numberOfBitsOfData, 
	HIGH_PRIORITY, RELIABLE_ORDERED, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID, 0);
 }
}

bool net_setup(bool server, bool first)
{
 char str[512];
 net.connected = false;
 
 if (first)
 {
  peer = RakNetworkFactory::GetRakPeerInterface();
  REGISTER_STATIC_RPC(peer, PrintMessage);

  net.server = server;

  if (!server)
  {
 	 peer->Startup(1, 30, &SocketDescriptor(), 1);
  }
  else
  {
	 peer->Startup(MAX_CLIENTS, 30, &SocketDescriptor(SERVER_PORT,0), 1);
  }
 
	return true;
 }

 if (net.server)
 {
	add_chat_line("Starting the server", Rgba(1.0, 1.0, 1.0));
	net_find_ip();

	sprintf(str, "IP Address : %s", net.IP);
	add_chat_line(str, Rgba(1.0, 1.0, 1.0));
	// We need to let the server accept incoming connections from the clients
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	sprintf(str, "%c%s", 1, chat.name);
	add_user(str, 1);
	chat.user_id = 1;
 }
 else
 {
  add_chat_line("Starting the client.\n", Rgba(1.0, 1.0, 1.0));
	peer->Connect(net.IP, SERVER_PORT, 0,0);
 }

 net.connected = true;

 return true;
}

void net_send_message(char *str, int message)
{
 char text[512];

 sprintf(text, "%c%s", message, str);

 peer->RPC("PrintMessage", text, (strlen(text)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
 UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void net_process(void)
{
 char str[512];
 int u;

 if (!net.connected) return;

 packet = peer->Receive();

 while(packet)
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				add_chat_line("Another client has disconnected.\n", Rgba(1.0, 1.0, 1.0));
				break;
			case ID_REMOTE_CONNECTION_LOST:
				add_chat_line("Another client has lost the connection.\n", Rgba(1.0, 1.0, 1.0));
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				add_chat_line("Another client has connected.\n", Rgba(1.0, 1.0, 1.0));
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				add_chat_line("Our connection request has been accepted.\n", Rgba(1.0, 1.0, 1.0));

				sprintf(str, "%c%s", 1, chat.name);
				// Two tricky things here.  First, you have to remember to send the NULL terminator so you need strlen(str)+1
				// Second, if you didn't read the docs you might not realize RPC takes the number of bits rather than the number of bytes.
				// You have to multiply the number of bytes by 8
				// Don't forget that RPC takes bits as the data length, not bytes!
				//peer->RPC("PrintMessage",str, (strlen(str)+1)*8, HIGH_PRIORITY, RELIABLE_ORDERED, 0, 
				//UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

				net_send_message(str, MESSAGE_NEW_USER);
				break;
				
			case ID_NEW_INCOMING_CONNECTION:
				//add_chat_line("A connection is incoming.\n");

				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				add_chat_line("The server is full.\n", Rgba(1.0, 1.0, 1.0));
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (net.server){
					sprintf(str, "A client has disconnected", Rgba(1.0, 1.0, 1.0));
					add_chat_line(str, Rgba(1.0, 1.0, 1.0));
				} else {
					add_chat_line("We have been disconnected.\n", Rgba(1.0, 1.0, 1.0));
				}
				break;
			case ID_CONNECTION_LOST:
				if (net.server){
					add_chat_line("A client lost the connection.\n", Rgba(1.0, 1.0, 1.0));
				} else {
					add_chat_line("Connection lost.\n", Rgba(1.0, 1.0, 1.0));
				}
				break;
			default:
				//add_chat_line("Message with identifier has arrived.\n");
				break;
			}

			peer->DeallocatePacket(packet);

			// Stay in the loop as long as there are more packets.
			packet = peer->Receive();
		}
}

void net_shutdown(void)
{
 char str[512];

 if (!net.connected) return;
 
 sprintf(str, "%c", chat.user_id);
 net_send_message(str, MESSAGE_REMOVE_USER);

 net.connected = false;
 peer->Shutdown(300);
 RakNetworkFactory::DestroyRakPeerInterface(peer);
}
*/