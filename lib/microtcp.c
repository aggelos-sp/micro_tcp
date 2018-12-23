/*
 * microtcp, a lightweight implementation of TCP for teaching,
 * and academic purposes.
 *
 * Copyright (C) 2015-2017  Manolis Surligas <surligas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "microtcp.h"
#include "../utils/crc32.h"

microtcp_sock_t microtcp_socket (int domain, int type, int protocol)
{
  microtcp_sock_t newSocket;
  newSocket.sd = socket(domain, type, protocol);

  if(newSocket.sd == -1){
    perror("Fail to initialize socket");
    exit(EXIT_FAILURE);
  }
  newSocket.state = UNKNOWN;
  newSocket.init_win_size = MICROTCP_WIN_SIZE;
  newSocket.curr_win_size = MICROTCP_WIN_SIZE;
  newSocket.ssthresh = MICROTCP_INIT_SSTHRESH;
  newSocket.cwnd = MICROTCP_INIT_CWND;

  return newSocket;
}

int microtcp_bind (microtcp_sock_t *socket, const struct sockaddr *address,
                  socklen_t address_len)
{
  int bind_number = bind(socket->sd, address, address_len);
  if(bind_number < 0){
    perror("Error Could Not Bind Socket");
    exit(EXIT_FAILURE);
  }
  return bind_number;
}

int
microtcp_connect (microtcp_sock_t *socket, const struct sockaddr *address,
                  socklen_t address_len)
{
  microtcp_header_t send_head, check_head;
  microtcp_header_t * recieve_head;
  uint8_t buff[MICROTCP_RECVBUF_LEN];
  uint32_t checksum_tmp;
  int i = 0, recieve;
  recieve_head = malloc(sizeof(microtcp_header_t));

  for(i = 0; i<MICROTCP_RECVBUF_LEN; i++){
    buff[i] = 0;
  }
  /*Initialize first pack*/
  send_head = initialize_packets_notpointers(send_head, rand(),0,htons(SYN),0,0,0,0,0,0,0,0);
  /*
  send_head.ack_number = 0;
  send_head.seq_number = rand();
  send_head.future_use0 = 0;
  send_head.future_use1 = 0;
  send_head.future_use2 = 0;
  send_head.checksum = 0;
  send_head.window = 0;
  send_head.data_len = 0;
  send_head.control = htons(SYN);
  */
  memcpy(buff, &send_head, sizeof(microtcp_header_t));
  send_head.checksum = crc32(buff, MICROTCP_RECVBUF_LEN);
  /*Send first packet*/
  if(sendto(socket->sd,(void *)&send_head,sizeof(microtcp_header_t),0,address,address_len) < 0){
    perror("Failed to send first pack");
    exit(EXIT_FAILURE);
  }
  /*Getting second packet*/
  recieve = recvfrom(socket->sd, recieve_head, sizeof(microtcp_header_t), 0 ,address,&address_len);
  if(recieve == -1){
    perror("Failed to recieve second syn ack @connect");
    exit(EXIT_FAILURE);
  }
  check_head = initialize_packets_notpointers(check_head, recieve_head->seq_number, recieve_head->ack_number,
                                              recieve_head->control,recieve_head->window,0,0,0,0,htonl(recieve_head->checksum),
                                              0,0);
  checksum_tmp = htonl(recieve_head->checksum);
  check_head.data_len = 0;
  check_head.future_use0 = 0;
  check_head.future_use1 = 0;
  check_head.future_use2 = 0;
  check_head.checksum = 0;
  check_head.ack_number = recieve_head->ack_number;
  check_head.seq_number = recieve_head->seq_number;
  check_head.window = recieve_head->window;
  check_head.control = recieve_head->control;

}

int
microtcp_accept (microtcp_sock_t *socket, struct sockaddr *address,
                 socklen_t address_len)
{
  /* Your code here */
}

int
microtcp_shutdown (microtcp_sock_t *socket, int how)
{
  /* Your code here */
}

ssize_t
microtcp_send (microtcp_sock_t *socket, const void *buffer, size_t length,
               int flags)
{
  /* Your code here */
}

ssize_t
microtcp_recv (microtcp_sock_t *socket, void *buffer, size_t length, int flags)
{
  /* Your code here */
}


/*My methods*/
microtcp_header_t* initialize_packets_forpointers(microtcp_header_t* packet, uint32_t seq_number, 
                  uint32_t ack_number,uint16_t control,uint16_t window, 
                  uint32_t data_len, uint32_t future_use0, uint32_t future_use1,
                  uint32_t future_use2, uint32_t checksum, 
                  uint32_t left_sack, uint32_t right_sack)
{
  packet->seq_number = seq_number;
  packet->ack_number = ack_number;
  packet->control = control;
  packet->window = window;
  packet->data_len = data_len;
  packet->future_use0 = future_use0;
  packet->future_use1 = future_use1;
  packet->future_use2 = future_use0;
  packet->checksum = checksum;
  return packet;
}

microtcp_header_t initialize_packets_notpointers(microtcp_header_t packet, uint32_t seq_number, 
                  uint32_t ack_number,uint16_t control,uint16_t window, 
                  uint32_t data_len, uint32_t future_use0, uint32_t future_use1,
                  uint32_t future_use2, uint32_t checksum, 
                  uint32_t left_sack, uint32_t right_sack)
{
  packet->seq_number = seq_number;
  packet->ack_number = ack_number;
  packet->control = control;
  packet->window = window;
  packet->data_len = data_len;
  packet->future_use0 = future_use0;
  packet->future_use1 = future_use1;
  packet->future_use2 = future_use0;
  packet->checksum = checksum;
  return packet;