
#定义项目的名称
CONTIKI_PROJECT =http-example
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI=../../..
#添加模块
MODULES += core/net/http-socket
#使用路由功能
ROUTER = 1
#添加头文件
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
#添加公共编译规则文件
include $(CONTIKI)/Makefile.include
#define ROUTER 1