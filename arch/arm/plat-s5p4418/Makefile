#
# Copyright 2009 Nexell Co.
#
# Makefile for the Nexell platform board

ifeq ($(CONFIG_PLAT_S5P4418_REALARM),y)
PLATBOARD :=realarm
endif

ifeq ($(CONFIG_PLAT_S5P4418_DRONE),y)
PLATBOARD :=drone
endif

ifeq ($(CONFIG_PLAT_S5P4418_DRONE2),y)
PLATBOARD :=drone2
endif

ifeq ($(CONFIG_PLAT_S5P4418_SVT),y)
PLATBOARD :=svt
endif

ifeq ($(CONFIG_PLAT_S5P4418_ASB),y)
PLATBOARD :=asb
endif

#
# Build options
#

obj-y	:= $(PLATBOARD)/

KBUILD_AFLAGS 	+= -I$(srctree)/$(platdirs)common 	\
				   -I$(srctree)/$(platdirs)$(PLATBOARD)/include
KBUILD_CFLAGS 	+= -I$(srctree)/$(platdirs)common	\
				   -I$(srctree)/$(platdirs)$(PLATBOARD)/include
KBUILD_CPPFLAGS += -I$(srctree)/$(platdirs)common	\
				   -I$(srctree)/$(platdirs)$(PLATBOARD)/include



