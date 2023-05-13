#定义项目的名称
CONTIKI_PROJECT =dht11
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI=../../..
#CONTIKI_SOURCEFILES +=$(CONTIKI)/platform/cc2538dk/dev/DHT11.c
#添加dht11文件
CONTIKI_TARGET_SOURCEFILES += DHT11-arch.c
#添加oled.c文件
CONTIKI_TARGET_SOURCEFILES += oled.c
#添加公共变异规则文件
include $(CONTIKI)/Makefile.include
