/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

// region dependencies

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "sys/msg.h"
#include "msg_queue.h"

// endregion dependencies

// region public functions

int create_msg_queue() {
    int msgq_id;

    msgq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0777);

    assert(msgq_id != -1);

    return msgq_id;
}

int rcv_msg(int msgq_id, void * msg, size_t msg_size, long type) {
    assert(msg != NULL);
    int nread;

    nread = (int) msgrcv(msgq_id, msg, msg_size - sizeof(long), type, 0);

    assert(nread != -1);

    return nread;
}

void snd_msg(int msgq_id, void * msg, size_t msg_size) {
    assert(msg != NULL);

    assert(msgsnd(msgq_id, msg, msg_size - sizeof(long), 0) != -1);
}

void destroy_msg_queue(int msgq_id) {

    assert(msgctl(msgq_id, IPC_RMID, NULL) != -1);
}

// endregion public functions