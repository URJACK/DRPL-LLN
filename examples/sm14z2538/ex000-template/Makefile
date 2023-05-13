#help
help:
	@echo "Build Help Info"
	@echo "make TARGET=sm14z2538"

#项目的名称
CONTIKI_PROJECT = template
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义目录变量
CONTIKI = ../../..


#添加定义的头文件
DEFINES+=PROJECT_CONF_H=\"project-conf.h\"


#引用编译规则
include $(CONTIKI)/Makefile.include
