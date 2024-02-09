# 编译产物
BINALL=wpa_supplicant

# 编译可执行文件
ALL = $(BINALL)
# 编译静态库
ALL += libwpa_client.a
# 编译动态库
ALL += libwpa_client.so

CONFIG_FILE=.config
# 引用build.rules文件
# 这里有第一个目标文件ALL，编译从这里开始
include ../src/build.rules

# 头文件引用路径
CFLAGS += -I$(abspath ../src)
CFLAGS += -I$(abspath ../src/utils)
CFLAGS += -I$(abspath ../src/common)

# 可配置.config文件
mkconfig:
	@if [ -f .config ]; then \
		echo '.config exists - did not replace it'; \
		exit 1; \
	fi
	cp defconfig .config
	echo CONFIG_DRIVER_HOSTAP=y >> .config
	echo CONFIG_DRIVER_WEXT=y >> .config

ifndef CONFIG_MAIN
CONFIG_MAIN=main
endif

#编译的源文件
OBJS += $(CONFIG_MAIN).o
OBJS += ../src/utils/utils.o
OBJS += ../src/common/common.o

# 每次编一个产物，就对_OBJS_VAR进行重新赋值，在调用objs.mk

# LDO定义在src/build.rules中，其实就是gcc或arm-gcc
# LIB和EXTRALIBS都是外部的库
# OBJS是一堆需要的.o文件
# EXTRA_progs, wpa_priv
_OBJS_VAR := OBJS
# 调用函数， 更新_OBJS_VAR
# 编译wpa_suupplicant
include ../src/objs.mk
wpa_supplicant: $(OBJS) $(EXTRA_progs)
	$(Q)$(LDO) $(LDFLAGS) -o wpa_supplicant $(OBJS) $(LIBS) $(EXTRALIBS)
	@$(E) "  LD " $@

# 编译eapol_test
_OBJS_VAR := OBJS_t
include ../src/objs.mk
eapol_test: $(OBJS_t)
	$(Q)$(LDO) $(LDFLAGS) -o eapol_test $(OBJS_t) $(LIBS)
	@$(E) "  LD " $@

# 编译preauth_test
_OBJS_VAR := OBJS_t2
include ../src/objs.mk
preauth_test: $(OBJS_t2)
	$(Q)$(LDO) $(LDFLAGS) -o preauth_test $(OBJS_t2) $(LIBS)
	@$(E) "  LD " $@

# 编译wpa_passphrase
_OBJS_VAR := OBJS_p
include ../src/objs.mk
wpa_passphrase: $(OBJS_p)
	$(Q)$(LDO) $(LDFLAGS) -o wpa_passphrase $(OBJS_p) $(LIBS_p) $(LIBS)
	@$(E) "  LD " $@

LIBCTRL += ../src/common/common.o
LIBCTRL += ../src/utils/utils.o

LIBCTRLSO += ../src/common/common.c
LIBCTRLSO += ../src/utils/utils.c

_OBJS_VAR := LIBCTRL
include ../src/objs.mk
libwpa_client.a: $(LIBCTRL)
	$(Q)rm -f $@
	$(Q)$(AR) crs $@ $?
	@$(E) "  AR " $@

libwpa_client.so: $(LIBCTRLSO)
	@$(E) "  CC  $@ ($^)"
	$(Q)$(CC) $(LDFLAGS) -o $@ $(CFLAGS) -shared -fPIC $^

OBJS_wpatest := libwpa_test.o
_OBJS_VAR := OBJS_wpatest
include ../src/objs.mk
libwpa_test1: $(OBJS_wpatest) libwpa_client.a
	$(Q)$(LDO) $(LDFLAGS) -o libwpa_test1 $(OBJS_wpatest) libwpa_client.a $(LIBS_c)
	@$(E) "  LD " $@

libwpa_test2: $(OBJS_wpatest) libwpa_client.so
	$(Q)$(LDO) $(LDFLAGS) -o libwpa_test2 $(OBJS_wpatest) -L. -lwpa_client $(LIBS_c)
	@$(E) "  LD " $@

_OBJS_VAR := OBJS_nfc
include ../src/objs.mk
nfc_pw_token: $(OBJS_nfc)
	$(Q)$(LDO) $(LDFLAGS) -o nfc_pw_token $(OBJS_nfc) $(LIBS)
	@$(E) "  LD " $@

win_if_list: win_if_list.c
	$(Q)$(LDO) $(LDFLAGS) -o $@ win_if_list.c $(CFLAGS) $(LIBS_w)
	@$(E) "  LD " $@
# so库生成规则
eap_psk.so: $(SRC_EAP_PSK)
	$(Q)$(CC) $(LDFLAGS) -o $@ $(CFLAGS) -shared -rdynamic -fPIC $^ \
		-Deap_peer_psk_register=eap_peer_method_dynamic_init
	@$(E) "  CC/LD " $@

%.so: ../src/eap_peer/%.c
	$(Q)$(CC) $(LDFLAGS) -o $@ $(CFLAGS) -shared -rdynamic -fPIC $< \
		-D$(*F:eap_%=eap_peer_%)_register=eap_peer_method_dynamic_init
	@$(E) "  CC/LD " $@


clean: common-clean
	rm -f *.o *.d
	$(MAKE) -C ../src clean
