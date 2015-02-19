CC = gcc
CFLAGS = -Wall -g
LDFLAGS += -lm
BIN_DIR := ./bin
CP := cp

MODULE_TOP_DIR:=.
MODULE_BIN_DIR:=$(MODULE_TOP_DIR)/src
MODULE_SRC_DIR:=$(MODULE_TOP_DIR)/src
MODULE_INC_DIR:=-I$(MODULE_TOP_DIR)/inc
MODULE_SRCS:=$(wildcard $(MODULE_SRC_DIR)/*.c)
MODULE_OBJS:=$(addprefix $(MODULE_BIN_DIR)/, $(notdir $(patsubst %.c, %.o, $(MODULE_SRCS))))
MODULE_DEPS:=$(MODULE_OBJS:.o=.d)
TARGET=epollsrv

exec : $(MODULE_DEPS) $(MODULE_OBJS)
	$(CC) $(CFLAGS) -o $(TARGET)  $(MODULE_OBJS) $(LDFLAGS)

$(MODULE_OBJS) : %.o : %.d
	$(MAKE) -f $<
	
$(MODULE_DEPS) : $(MODULE_BIN_DIR)/%.d : src/%.c
	@$(CC) -MM $(CFLAGS) $(MODULE_INC_DIR) $< -o $@
	@echo '	$(CC) $(CFLAGS) $(MODULE_INC_DIR) -c $< -o $(patsubst %.d, %.o, $@)' >> $@ 

install:
	mkdir -p ${BIN_DIR}
	$(CP) $(TARGET) ${BIN_DIR}
clean:
	rm -rf $(MODULE_OBJS)
	rm -rf $(MODULE_DEPS)
	rm -rf $(TARGET)
	
