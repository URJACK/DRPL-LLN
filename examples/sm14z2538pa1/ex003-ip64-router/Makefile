#项目的名称
CONTIKI_PROJECT = ip64-router
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI=../../..
#DEFINES+=ROUTER=\"ip64-router.h\"
#使用路由
ROUTER = 1
#配置文件
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
#引用公共编译规则文件
include $(CONTIKI)/Makefile.include

