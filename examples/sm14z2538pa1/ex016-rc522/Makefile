#定义项目的名称
CONTIKI_PROJECT = rc522
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI = ../../..

#添加程序源码文件到目标中
CONTIKI_TARGET_SOURCEFILES += mfrc522-arch.c
#添加公共编译规则文件
include $(CONTIKI)/Makefile.include
