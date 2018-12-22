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
  int i = 0;
  recieve_head = malloc(sizeof(microtcp_header_t));

  for(i = 0; i<MICROTCP_RECVBUF_LEN; i++){
    buff[i] = 0;
  }
  /*Initialize first pack*/
  send_head.ack_number = 0;
  send_head.seq_number = rand();
  send_head.future_use0 = 0;
  send_head.future_use1 = 0;
  send_head.future_use2 = 0;
  send_head.checksum = 0;
  send_head.window = 0;
  send_head.data_len = 0;
  send_head.control = htons(SYN);
  
  memcpy(buff, &send_head, sizeof(microtcp_header_t));
  send_head.checksum = crc32(buff, MICROTCP_RECVBUF_LEN);
  /*Send first packet*/
  if(sendto(socket->sd,(void *)&send_head,sizeof(microtcp_header_t),0,address,address_len) < 0){
    perror("Faild to send first pack");
    exit(EXIT_FAILURE);
  }
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
