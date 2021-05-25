/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 18/05/2021
*/

#ifndef IPCS_MSG_QUEUE_H
#define IPCS_MSG_QUEUE_H

// region constants

#define true 1
#define false 0

// endregion constants

// region public functions prototypes

/**
 * @def create_msq_queue
 * @brief Function that creates a message queue.
 *
 * @return a system identifier of the created message queue.
 *
 * @throws MsgQueueCreationException if it is not possible to create the message queue.
 */
extern int create_msg_queue();

/**
 * @def rec_msg
 * @brief Function that reads a message of the message queue specified by the identifier.
 *
 * @param msgq_id
 * Identifier of the message queue that contains the message.
 *
 * @param msg
 * Pointer to the buffer that will store the received message.
 *
 * @param msg_size
 * Size of the message to be received.
 *
 * @param type
 * Type of the message to be received.
 *
 * @return the number of bytes read from message queue.
 *
 * @throws MsgQueueReceiveException if it is not possible to correctly received the message.
 */
extern int rcv_msg(int msgq_id, void * msg, size_t msg_size, long type);

/**
 * @def snd_msg
 * @brief Function that sends a message through the message queue specified by the identifier.
 *
 * @param msgq_id
 * Identifier of the message queue.
 *
 * @param msg
 * A pointer to a buffer that stores the message to be sent.
 *
 * @param msg_size
 * Size of the message.
 *
 * @throws MsdQueueSendException if it is not possible to send the message.
 */
extern void snd_msg(int msgq_id, void * msg, size_t msg_size);

/**
 * @def destroy_msg_queue
 * @brief Function that destroys a message queue.
 *
 * @param msgq_id
 * Identifier of the message queue to be destroyed.
 *
 * @throws MsgQueueDestructionException if it is not possible to destroy the message queue.
 */
extern void destroy_msg_queue(int msgq_id);

// endregion public functions prototypes

#endif //IPCS_MSG_QUEUE_H
