#ifndef SQL_H
#define SQL_H
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define BUF_SiZE 512
#define TIME_SIZE 55

/*    注册用户名    */
int Insert(char szName[], char szPwd[]);
/*    判断用户是否存在      */
int IsExist(char szName[]);
/*    显示数据表      */
void ShowTables(void);
#endif