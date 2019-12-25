include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
 
PKG_NAME:=bbp-debug
PKG_RELEASE:=1
 
include $(INCLUDE_DIR)/package.mk

EXTRA_CFLAGS:= \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=m,%,$(filter %=m,$(EXTRA_KCONFIG)))) \
	$(patsubst CONFIG_%, -DCONFIG_%=1, $(patsubst %=y,%,$(filter %=y,$(EXTRA_KCONFIG))))

MAKE_OPTS:=$(KERNEL_MAKE_FLAGS) \
	SUBDIRS="$(PKG_BUILD_DIR)" \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	CONFIG_BBP_DEBUG=m

define KernelPackage/bbp-debug
	SUBMENU:=Other modules
	TITLE:=bbp-debug
	FILES:=$(PKG_BUILD_DIR)/bbp-debug.ko
#	AUTOLOAD:=$(call AutoLoad, 99, bbp-debug)
	KCONFIG:=
endef

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" $(MAKE_OPTS) modules
endef

$(eval $(call KernelPackage,bbp-debug))