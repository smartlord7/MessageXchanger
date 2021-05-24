#ifndef MESSAGEXCHANGER_SESSIONS_AVL_H
#define MESSAGEXCHANGER_SESSIONS_AVL_H

#define true 1
#define false 0

extern user_session_t * find_session(int ip, int port);
extern user_session_t * insert_session(user_session_t * session);

#endif //MESSAGEXCHANGER_SESSIONS_AVL_H
