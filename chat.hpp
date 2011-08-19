// Chat (.Hpp)
// -----------

#define MAX_CHAT_LINES											8

#define MAX_USERS														16

#define CHAT_MODE_SERVER_QUESTION						0
#define CHAT_MODE_NAME_QUESTION							1
#define CHAT_MODE_IP_QUESTION								2
#define CHAT_MODE_CHATTING									3

struct chat_line_t
{
 char text[512];
 Rgba rgba;
};

struct chat_t
{
 int line, char_pos, mode, num_users, user_id, input_time;
 char name[512], text[512];
 int old_key[256];
};

struct user_t
{
 char name[512];
 int id;
};

extern struct chat_t chat;
extern struct chat_line_t chat_line[MAX_CHAT_LINES];
extern struct user_t user[MAX_USERS];

void setup_chat_lines(void);
void setup_users(void);
void add_user(char *name, int pos);
int find_free_user_spot(void);
void remove_user(int id);
void do_chat_question_logic(void);
void add_chat_line(char *text, Rgba rgba);
void do_chat_logic(void);
void draw_chat_text(void);
