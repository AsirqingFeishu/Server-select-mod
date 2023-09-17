#include "include/Sql.h"

/*
     注册用户名函数
        成功：返回0
        失败：返回1
 */
int Insert(char szName[], char szPwd[]) {
  MYSQL mysql;
  int r, t, id = 12;
  char query[50] = {0};
  // 初始化MYSQL连接句柄
  mysql_init(&mysql);

  // 连接mysql 数据库
  if (!mysql_real_connect(&mysql, "localhost", "root", "mysql", "chatdb", 0,
                          NULL, 0)) {
    printf("Failed to connect to Mysql!\n");
    return EXIT_FAILURE;
  } else {
    printf("Connect to Mysql successfully!\n");
  }
  // 查语句并插入
  sprintf(query, "INSERT INTO qqnum(name,passwd) VALUES('%s','%s')", szName,
          szPwd);
  r = mysql_query(&mysql, query);

  if (!r) {
    printf("Insert data success!\n ");
    mysql_close(&mysql);
    return EXIT_SUCCESS;
  } else {
    printf("Insert data Failure\n");
    mysql_close(&mysql);
    return EXIT_FAILURE;
  }
}
/*    判断用户是否存在      */
int IsExist(char szName[]) {
  MYSQL mysql;
  MYSQL_RES *res;
  MYSQL_ROW row;
  int r, i, id = 12;
  char buf[BUF_SiZE]; // cur_time[TIME_SIZE];
  // 初始化MYSQL连接句柄
  mysql_init(&mysql);

  // 连接数据库
  if (!mysql_real_connect(&mysql, "localhost", "root", "mysql", "chatdb", 0,
                          NULL, 0)) {
    printf("Failed to connect to Mysql!\n");
    return -1;
  }
  printf("Connected to Mysql successfully!\n");
  // 执行查询
  char query[50];
  sprintf(query, "select name from qqnum where name='%s'", szName);
  if (mysql_query(&mysql, query)) {
    printf("Failed to query to Mysql!\n");
    return -1;
  }
  // 获取查询结果集
  MYSQL_RES *result = mysql_store_result(&mysql);
  if (result == NULL) {
    printf("store failed, ");
    return -1;
  }
  int line = mysql_num_rows(result);
  if (line == 0) {
    printf("No rows found\n");
    return -1;
  }
  row = mysql_fetch_row(result);
  if (row == NULL) {
    return -1;
  }
  printf("%s\n", row[0]);
  mysql_close(&mysql);
  return 0;
}
/*    显示数据表      */
void ShowTables(void) {
  MYSQL mysql;
  MYSQL_RES *result;
  MYSQL_ROW row;
  const char *query;
  int flag, t;

  // 初始化mysql连接句柄
  mysql_init(&mysql);
  // 连接数据库
  if (!mysql_real_connect(&mysql, "localhost", "root", "mysql", "chatdb", 0,
                          NULL, 0)) {
    printf("Error connecting to Mysql!\n ");
    return;
  }
  // 查询
  query = "select * from qqnum";
  flag = mysql_query(&mysql, query);
  if (flag) {
    printf("Query failed!\n");
    return;
  } else {
    printf("\n[%s] made ...\n", query);
  }
  // 读取结果
  result = mysql_store_result(&mysql);
  do {
    row = mysql_fetch_row(result);
    if (row == 0)
      break;
    // 显示一行数据
    for (t = 0; t < mysql_num_fields(result); ++t) {
      printf("%s\t", row[t]);
    }
    printf("\n");
  } while (1);
  mysql_close(&mysql);
  return;
}
