#定义项目的名称
CONTIKI_PROJECT = udp-multicast
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI = ../../..
#添加项目配置文件
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
#添加编译规则文件
include $(CONTIKI)/Makefile.include

