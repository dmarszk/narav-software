#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#


PROJECT_NAME := narav

# Include src dir
EXTRA_COMPONENT_DIRS := $(CURDIR)/src

include $(IDF_PATH)/make/project.mk

