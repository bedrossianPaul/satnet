# Target name
TARGET = build/app

# Paths
SRC_DIR = .
BUILD_DIR = build
LWIP_DIR = lwip
LWIP_CONTRIB_PORT_DIR = $(LWIP_DIR)/contrib/ports/unix/port

# Include dirs
INCLUDE_DIRS = -I$(SRC_DIR)/include -I$(SRC_DIR)/src/olsr/inc -I$(LWIP_CONTRIB_PORT_DIR)/include -I$(LWIP_DIR)/src/include 

# Compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -g -DLWIP_DEBUG $(INCLUDE_DIRS)
LDFLAGS = -lutil -pthread -lm

# Source files
APP_SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/src/*/*.c)
LWIPFILES = \
	$(LWIP_DIR)/src/core/init.c \
	$(LWIP_DIR)/src/core/def.c \
	$(LWIP_DIR)/src/core/dns.c \
	$(LWIP_DIR)/src/core/inet_chksum.c \
	$(LWIP_DIR)/src/core/ip.c \
	$(LWIP_DIR)/src/core/mem.c \
	$(LWIP_DIR)/src/core/memp.c \
	$(LWIP_DIR)/src/core/netif.c \
	$(LWIP_DIR)/src/core/pbuf.c \
	$(LWIP_DIR)/src/core/raw.c \
	$(LWIP_DIR)/src/core/stats.c \
	$(LWIP_DIR)/src/core/sys.c \
	$(LWIP_DIR)/src/core/altcp.c \
	$(LWIP_DIR)/src/core/altcp_alloc.c \
	$(LWIP_DIR)/src/core/altcp_tcp.c \
	$(LWIP_DIR)/src/core/tcp.c \
	$(LWIP_DIR)/src/core/tcp_in.c \
	$(LWIP_DIR)/src/core/tcp_out.c \
	$(LWIP_DIR)/src/core/timeouts.c \
	$(LWIP_DIR)/src/core/udp.c \
    $(LWIP_DIR)/src/core/ipv4/acd.c \
	$(LWIP_DIR)/src/core/ipv4/autoip.c \
	$(LWIP_DIR)/src/core/ipv4/dhcp.c \
	$(LWIP_DIR)/src/core/ipv4/etharp.c \
	$(LWIP_DIR)/src/core/ipv4/icmp.c \
	$(LWIP_DIR)/src/core/ipv4/igmp.c \
	$(LWIP_DIR)/src/core/ipv4/ip4_frag.c \
	$(LWIP_DIR)/src/core/ipv4/ip4.c \
	$(LWIP_DIR)/src/core/ipv4/ip4_addr.c \
    $(LWIP_DIR)/src/netif/ethernet.c \
	$(LWIP_DIR)/src/netif/bridgeif.c \
	$(LWIP_DIR)/src/netif/bridgeif_fdb.c 

ARCHFILES = \
	$(LWIP_CONTRIB_PORT_DIR)/perf.c \
	$(LWIP_CONTRIB_PORT_DIR)/sys_arch.c \
	$(LWIP_CONTRIB_PORT_DIR)/netif/tapif.c \
	$(LWIP_CONTRIB_PORT_DIR)/netif/list.c \
	$(LWIP_CONTRIB_PORT_DIR)/netif/sio.c \
	$(LWIP_CONTRIB_PORT_DIR)/netif/fifo.c

# Object files
APP_OBJ     = $(patsubst ./%.c,$(BUILD_DIR)/%.o,$(APP_SRC))
LWIPOBJS    = $(patsubst %.c,$(BUILD_DIR)/%.o,$(LWIPFILES))
ARCHOBJS    = $(patsubst %.c,$(BUILD_DIR)/%.o,$(ARCHFILES))
OBJS        = $(APP_OBJ) $(LWIPOBJS) $(ARCHOBJS)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Generic rule for building .o from .c into build/
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


TEST_SRC = $(wildcard tests/*.c)
TEST_FILES = $(SRC_DIR)/src/table/table.c
TEST_BIN = $(patsubst tests/%.c,build/tests/%,$(TEST_SRC))

# Tests
tests: $(TEST_BIN)

build/tests/%: tests/%.c $(TEST_FILES)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

# Clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
