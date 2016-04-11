LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

# 遍历目录及子目录函数
define walk
    $(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# 遍历Classes目录
ALLFILES = $(call walk, $(LOCAL_PATH)/../../Classes)
                   
FILE_LIST := hellolua/main.cpp
# 从文件中提取所有的.cpp文件
FILE_LIST += $(filter %.cpp %.c %.cc, $(ALLFILES))

FILE_INCLUDES := $(shell find $(LOCAL_PATH)/../../Classes -type d)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES := $(FILE_INCLUDES)
# 到这个注释自动查找cpp的修改就全部结束了


# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
