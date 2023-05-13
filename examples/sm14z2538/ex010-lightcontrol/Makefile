#定义项目的名称
CONTIKI_PROJECT =mqtt-lightcontrol

#编译所有相关的项目
all: $(CONTIKI_PROJECT)

#定义根目录
CONTIKI=../../..

#定义头文件的路径
DEFINES+=PROJECT_CONF_H=\"project-conf.h\"

#使用ipv6
CONTIKI_WITH_IPV6 = 1
#添加mqtt应用
APPS += mqtt

#添加公共的构建文件
include $(CONTIKI)/Makefile.include
