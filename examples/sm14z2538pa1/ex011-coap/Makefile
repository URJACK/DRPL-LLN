#定义项目的名称
CONTIKI_PROJECT =coap-post
#编译所有相关的项目
all: $(CONTIKI_PROJECT)
#定义根目录
CONTIKI=../../..

WITH_SE95_SENSOR=0
WITH_TMP102_SENSOR=0
WITH_BUTTON_SENSOR=1


CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

# Contiki IPv6 configuration
#IPV6配置
WITH_UIP6=1
UIP_CONF_IPV6=1
# IPv6 make config disappeared completely
CFLAGS += -DUIP_CONF_IPV6=1
# variable for Makefile.include
CFLAGS += -DUIP_CONF_IPV6_RPL=1

SMALL=1

# REST Engine shall use Erbium CoAP implementation
APPS += er-coap
APPS += rest-engine

ifeq ($(WITH_SE95_SENSOR),1)
CFLAGS += -DWITH_SE95_SENSOR=1
endif
ifeq ($(WITH_TMP102_SENSOR),1)
CFLAGS += -DWITH_TMP102_SENSOR=1
endif
ifneq ($(filter 1,$(WITH_SE95_SENSOR) $(WITH_TMP102_SENSOR)),)
CONTIKI_TARGET_SOURCEFILES += se95-sensor.c
endif
ifeq ($(WITH_BUTTON_SENSOR),1)
CFLAGS += -DWITH_BUTTON_SENSOR=1
endif

PROJECTDIRS += rplinfo
PROJECT_SOURCEFILES += rplinfo.c
#添加公共编译规则文件
include $(CONTIKI)/Makefile.include
