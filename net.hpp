// Net (.Hpp)
// ----------

#define MESSAGE_NEW_USER							1
#define MESSAGE_ADD_USER							2
#define MESSAGE_REMOVE_USER						3
#define MESSAGE_USER_ID								4
#define MESSAGE_CHAT									5
#define MESSAGE_MOUSE									6

struct net_t
{
 bool server, connected;
 char IP[32];
};

extern struct net_t net;

void net_find_ip(void);
bool net_setup(bool server, bool first);
void net_shutdown(void);
void net_process(void);
//void PrintMessage(RPCParameters *rpcParameters);
void net_send_message(char *str, int message);