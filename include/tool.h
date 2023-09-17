#ifndef TOOL_H
#define TOOL_H
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

// 客户端-服务器之间的命令
#define CL_CMD_REG 'r'
#define CL_CMD_CHAT 'c'
#define CL_CMD_LOGIN 'l'

/*
  读取用户信息函数
  instruct: 注册/登陆指令
  szName: 用于保存用户名的指针
  szNameSize：数组大小
  szName: 用于保存密码的指针
  szNameSize：数组大小
*/
int GetUserinfo(char *instruct, char *szName, char *szPwd, size_t szNameSize,
                size_t szPwdSize);
/*
  读取用户名
  str: 注册/登陆指令
  szName: 用于保存用户名的指针
  szNameSize：数组大小
*/
void GetName(char *str, char *szName, size_t szNameSize);
/*
  读取用户密码
  str: 注册/登陆指令
  szName: 用于保存密码的指针
  szNameSize：数组大小
*/
void GetPwd(char *str, char *szPwd, size_t szPwdSize);
// 统计字符串str 中 字符ch 的数量
int countChar(const char *str, const char ch);
/*
  协议合法性判断
  proto: 输入的协议
  code：指令
*/
bool Is_LegalProtocol(char *proto, char code);
/*
  错误判断函数
*/
void errif(bool condition, const char *errmsg);
#endif