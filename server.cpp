#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

void simple_server(int new_sockfd) {
  char buf[100];
  int buf_len;
  while((buf_len = read(new_sockfd, buf, 100)) > 0) {
    write(1, buf, buf_len);//request
    printf(":request arrived\n");
    printf("doing hevy work\n");
    sleep(3);
    //if((buf_len = read(new_sockfd, buf, 100)))
    printf("response replied\n");
  }
  //close(new_sockfd);  /* ソケットを閉鎖 */
  return;
}

int main(int argc,char**argv){
  int sockfd;
  int new_sockfd;
  unsigned int writer_len;
  struct sockaddr_in reader_addr;
  struct sockaddr_in writer_addr;
  /* ソケットの生成 */
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("reader: socket");
    exit(1);
  }
  int yes = 1;
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&yes, sizeof(yes))<0){
    perror("setsockopt");
    exit(1);
  }

  /* 通信ポート・アドレスの設定 */
  bzero((char *) &reader_addr, sizeof(reader_addr));
  reader_addr.sin_family = PF_INET;
  reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  reader_addr.sin_port = htons(8000);

  /* ソケットにアドレスを結びつける */
  if (bind(sockfd, (struct sockaddr *)&reader_addr, sizeof(reader_addr)) < 0) {
    perror("reader: bind");
    exit(1);
  }

  /* コネクト要求をいくつまで待つかを設定 */
  if (listen(sockfd, 5) < 0) {
    perror("reader: listen");
    close(sockfd);
    exit(1);
  }

  /* コネクト要求を待つ */
  if ((new_sockfd = accept(sockfd,(struct sockaddr *)&writer_addr, &writer_len)) < 0) {
    perror("reader: accept");
    exit(1);
  }
  simple_server(new_sockfd);

  close(sockfd);  /* ソケットを閉鎖 */
}
/*
import socket
client = socket.socket(socket.AF_INET,socket.SOCK_STREAM);client.connect(('127.0.0.1',8000))
client.sendall("hello")
client.close()
 */
