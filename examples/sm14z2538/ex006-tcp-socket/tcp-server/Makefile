#项目的名称
CONTIKI_PROJECT = tcp-server
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI = ../../../..
#使用路由
ROUTER = 1
#添加项目配置文件
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
#引用编译规则
include $(CONTIKI)/Makefile.include

