CONTIKI = ../../contiki_ewsn
CONTIKI_PROJECT = unicast-test

ifndef TARGET
    TARGET=sky
endif

DEFINES+=PROJECT_CONF_H=\"project-conf.h\"

all: $(CONTIKI_PROJECT).ihex
upload: $(CONTIKI_PROJECT).upload

CONTIKI_WITH_RIME = 1
include $(CONTIKI)/Makefile.include
