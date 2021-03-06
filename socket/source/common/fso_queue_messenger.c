#include "fso_queue_messenger.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void send_message(char msg_txt[], long channel){
  // int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
  send_buf.mtype = channel; // SEND_CHANNEL = 1 , RECEIVE_CHANNEL = 2
  strcpy(send_buf.mtext, msg_txt); // Copying message
  buf_length = strlen(send_buf.mtext) + 1 ; // Setting size of message buffer

  if(msgsnd(msq_id, &send_buf, buf_length, IPC_NOWAIT) < 0){
    fprintf(stderr, "Error sending message.\n");
  }else{
    fprintf(stderr, "Message: \"%s\" sent over channel %ld.\n", send_buf.mtext, channel);
  }
}

char *receive_message(long channel){
  // int msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
  if(msgrcv(msq_id, &receive_buf, MSG_SIZE, channel, 0) < 0){
    fprintf(stderr, "Error getting message.\n");
  }else{
    fprintf(stderr, "Got message: \"%s\"\n", receive_buf.mtext);
    return receive_buf.mtext;
  }
  return NULL;
}

void create_message_queue(int permission){
    // msgget(key, flags) returns a new queue if there is no queue with key
    // IPC_CREAT create a queue if it no exists
    // When called with both the IPC_CREAT and IPC_EXCL flags, the function fails if the facility already exists.
    // that is, if multiple processes are trying to create a queue with the same facility, only one wins.
    srand(time(NULL));
    key = rand() % 9000 + 1000; // Random number with 4 digits

    if((msq_id = msgget(ftok("/tmp", key), IPC_CREAT | IPC_EXCL | permission)) < 0){
      fprintf(stderr, "Sorry, error creating message queue.\n");
      exit(1);
    }else{
      fprintf(stderr, "Success: The message queue has been created | ID = %d\n", (int) key);
    }
}

void delete_message_queue(void){
  // The msgctl() function alters the permissions and other characteristics of a message queue.
  // int msgctl(int msqid, int cmd, struct msqid_ds *buf )
  if(msgctl(msq_id, IPC_RMID, NULL) < 0){ // remove message_queue
    fprintf(stderr, "Error deleting message queue.\n");
    exit(1);
  }else{
    fprintf(stderr, "Success deleting message queue.\n");
  } 
}
