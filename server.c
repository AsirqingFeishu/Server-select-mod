#include "include/Sql.h"
#include "include/tool.h"
#include <fcntl.h>
#include <unistd.h>
#define MAXLINE 5        // 读取最大字节长度
#define MAX_BUF_SIZE 512 // 接收缓存大小
#define SERV_PORT 8000

#define Reg_Ok "r1"
#define Reg_Fail "r0"
#define Log_Ok "l1"
#define Log_Fail "l0"
#define Cha_Ok "c1"
#define Cha_Fail "c0"
/*
  注册命令："r,用户名,密码"
  登陆命令："l,用户名,密码”
  聊天命令："c,聊天内容"

*/

int main(int argc, char *argv[]) {

  int i, maxi, maxfd;
  int listenfd, confd, sockfd;

  int nready, client[FD_SETSIZE];
  ssize_t n;
  //    用户名      用户密码      Ack/Fail指令
  char szName[30], szPwd[30], repBuf[3];

  //  可读fd，所有fd
  fd_set read_fds, all_fds;

  char buf[MAX_BUF_SIZE];    // 接收缓存
  char str[INET_ADDRSTRLEN]; // 点分十进制 ip地址
  memset(buf, 0, sizeof(buf));
  memset(str, 0, sizeof(str));
  memset(repBuf, 0, sizeof(repBuf));
  socklen_t cliaddr_len;
  struct sockaddr_in cliaddr, servaddr;

  // 创建套接字
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  errif(listenfd == 0, "create listen failed!\n");

  int val = 0;
  int ret =
      setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void *)&val, sizeof(int));
  errif(ret != 0, "setsockopt failed!\n");

  // 绑定
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  ret = bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  errif(ret == -1, "bind address failed");
  // 开启监听
  ret = listen(listenfd, 20);
  errif(ret == -1, "listen failed");
  printf("listening .....\n");

  // 最大接收文件描述符
  maxfd = listenfd;

  for (int i = 0; i < FD_SETSIZE; ++i) {
    client[i] = -1;
  }
  FD_ZERO(&all_fds);
  // listenfd 添加到fds
  FD_SET(listenfd, &all_fds);

  for (;;) {
    read_fds = all_fds;
    printf("start select.....\n");
    // 轮询查找可读fd
    nready = select(maxfd + 1, &read_fds, NULL, NULL, NULL);
    printf("select....\n");
    if (nready < 0) {
      puts("select error");
      break;
    }

    // 存在连接请求
    if (FD_ISSET(listenfd, &read_fds)) {
      cliaddr_len = sizeof(cliaddr);
      confd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
      if (confd < 0) {
        printf("accept error\n");
        break;
      }
      printf("received from %s at PORT %d\n",
             inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
             ntohs(cliaddr.sin_port));

      // 添加新的confd到数组
      for (i = 0; i < FD_SETSIZE; ++i) {
        if (client[i] < 0) {
          client[i] = confd;
          maxi = i; // 最大confd 下标
          break;
        }
      }
      // 连接上限判断
      if (i == FD_SETSIZE) {
        fputs("too many clients\n", stderr);
        exit(1);
      }
      // 添加新的confd到 fd_set
      FD_SET(confd, &all_fds);

      if (maxfd < confd) {
        maxfd = confd;
      }

      // 处理完listenfd
      if (--nready == 0)
        continue;
    }

    // 接收数据 fd
    for (i = 0; i <= maxi; ++i) {
      if ((sockfd = client[i]) < 0)
        continue;
      if (FD_ISSET(sockfd, &read_fds)) {
        // 读数据
        // n=read(sockfd,buf,MAXLINE);
        n = recv(sockfd, buf, sizeof(buf), 0);
        // 对方已经关闭连接
        if (n == 0) {
          close(sockfd);
          FD_CLR(sockfd, &all_fds);
          client[i] = -1;
        }
        // 接收数据
        else {
          char code = buf[0];
          switch (code) {
          case CL_CMD_REG: // 处理注册
            if (!Is_LegalProtocol(buf, CL_CMD_REG)) {
              puts("CL_CMD_REG:  invalid protocal!");
              break;
            }
            // 读取用户信息
            GetUserinfo(buf, szName, szPwd, sizeof(szName), sizeof(szPwd));

            // 名字合法判断
            if (IsExist(szName) == 0) {
              sprintf(repBuf, Reg_Fail);
            } else {
              /********************************************************目前问题*/
              // 合法用户添加入数据库
              if (Insert(szName, szPwd) == 1) {
                sprintf(repBuf, Reg_Fail);
                printf("register was failed\n");
              } else {
                sprintf(repBuf, Reg_Ok);
                printf("login was successful!\n");
              }
              ShowTables();
            }
            send(sockfd, repBuf, 3, 0);
            break;

            //******************************* 处理登陆
            //*****************************************
          case CL_CMD_LOGIN:
            if (!Is_LegalProtocol(buf, CL_CMD_LOGIN)) {
              puts("CL_CMD_LOGIN:  Invalid protocal!\n");
              break;
            }
            // 读取用户信息
            GetUserinfo(buf, szName, szPwd, sizeof(szName), sizeof(szPwd));
            printf("name:%s  pwd:%s \n", szName, szPwd);
            if (IsExist(szName) == 0) {
              sprintf(repBuf, Log_Ok);
              printf("Allow login.\n");
            } else {
              sprintf(repBuf, Log_Fail);
              printf("Login failed\n");
            }
            send(sockfd, repBuf, 3, 0);
            break;
            // ****************************************** 处理聊天
            // *********************************
          case CL_CMD_CHAT:
            if (!Is_LegalProtocol(buf, CL_CMD_CHAT)) {
              puts("CL_CMD_CHAT:  Invalid protocal!\n");
              break;
            } else {
              // sprintf(repBuf, Cha_Ok);
              // send(sockfd, repBuf, sizeof(repBuf), 0);
              // 发送信息给每一个客户端, 除了自己的窗口
              printf("chat: %s\n", buf);
              for (i = 0; i <= maxi; ++i) {
                if (client[i] != -1 && client[i] != sockfd) {
                  int size = send(client[i], buf, strlen(buf) + 1, 0);
                  // printf("send byte: %d\n ", size);
                }
                printf("send chat information successfully.\n");
              }

              if (--nready == 0)
                break;
            }
            break;
          }
        }
      }
    }
  }
  close(listenfd);
  return 0;
}