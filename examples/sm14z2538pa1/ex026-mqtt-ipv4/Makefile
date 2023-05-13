#定义项目的名称
CONTIKI_PROJECT =mqtt-demo
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI=../../..
#添加项目配置文件
DEFINES+=PROJECT_CONF_H=\"project-conf.h\"
#使用IPV6模块
CONTIKI_WITH_IPV6 = 1
#添加mqtt应用
APPS += mqtt

#添加公共编译规则文件
include $(CONTIKI)/Makefile.include
