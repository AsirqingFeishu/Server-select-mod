# Server-select-mod
Multi person chat server under Linux system

## 使用了select模型收发socket的数据

### 交互数据类型
r | l | c 
----| ---| ---
注册信息|登陆信息|聊天信息

### 测试环境：ubuntu 

## 生成可执行程序命令
### 	g++ -I ./include/ server.c Sql.c tool.c -o server.exe -L /usr/local/mysql/lib*.a -lz -lmysqlclient

## 如果需要使用gdb调试，需要先生成.o 文件
### 命令
### 	g++ -I ./include/ -c -g server.c Sql.c tool.c  -L /usr/local/mysql/lib*.a -lz -lmysqlclient
### g++ -I ./include/ *.o -o server.exe -L /usr/local/mysql/lib*.a -lz -lmysqlclient


