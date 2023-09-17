#include "include/tool.h"
// 获取用户名，

// 统计字符串中的字符数
int countChar(const char *str, const char ch) {
  int count = 0, i = 0;
  const char *p = str;
  while (*(p + i) != '\0') {
    if (p[i++] == ch) {
      ++count;
    }
  }
  return count;
}
/*
  读取用户信息函数
  数据格式：【指令，用户名，密码】
*/
int GetUserinfo(char *instruct, char *szName, char *szPwd, size_t szNameSize,
                size_t szPwdSize) {
  const char *split = ",";
  char *p = strtok(instruct, split); // 分割出用户名
  int i = 0;
  while (i < 3) {
    printf("%s\n", p);

    if (i == 1) {
      if (strlen(p) < szNameSize) {
        strcpy(szName, p);
      } else {
        printf("Error: Name is too long to fit in the buffer.\n");
        return 1;
      }
    } else if (i == 2) {
      if (strlen(p) < szPwdSize) {
        strcpy(szPwd, p);
      } else {
        printf("Error: Pwd is too long to fit in the buffer.\n");
        return 1;
      }
    }
    p = strtok(NULL, split);
    ++i;
  }
  return 0;
}
/*
  协议合法性判断
  proto: 输入的协议
  code：指令
*/
bool Is_LegalProtocol(char *proto, char code) {
  int n = 0;
  switch (code) {
  case CL_CMD_CHAT: { // chat只需要 有2个，
    n = countChar(proto, ',');
    if (n < 2)
      return false;
    break;
  }
  case CL_CMD_LOGIN: {
    n = countChar(proto, ',');
    if (n != 3)
      return false;
    break;
  }
  case CL_CMD_REG: {
    n = countChar(proto, ',');
    if (n != 3)
      return false;
    break;
  }
  default:
    return false;
    break;
  }
  return true;
}
// 错误判断
void errif(bool condition, const char *errmsg) {
  if (condition) {
    perror(errmsg);
    exit(EXIT_FAILURE);
  }
}