#定义项目的名称
CONTIKI_PROJECT = websocket-example
#添加websocket应用
APPS+=websocket
#开启路由
ROUTER = 1
#添加项目配置文件
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI = ../../..
#添加编译规则文件
include $(CONTIKI)/Makefile.include


