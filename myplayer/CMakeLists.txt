#版本
cmake_minimum_required(VERSION 3.4.1)

#导入头文件目录，不然就要逐个文件的导入
include_directories(src/main/cpp/include)

#添加动态库
add_library(avcodec-57 SHARED IMPORTED)

#设置库路径
set_target_properties(avcodec-57
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavcodec-57.so)

#添加动态库
add_library(avfilter-6 SHARED IMPORTED)

#设置库路径
set_target_properties(avfilter-6
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavfilter-6.so)

#添加动态库
add_library(avformat-57 SHARED IMPORTED)

#设置库路径
set_target_properties(avformat-57
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavformat-57.so)

#添加动态库
add_library(avutil-55 SHARED IMPORTED)

#设置库路径
set_target_properties(avutil-55
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libavutil-55.so)

#添加动态库
add_library(swresample-2 SHARED IMPORTED)

#设置库路径
set_target_properties(swresample-2
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libswresample-2.so)

#添加动态库
add_library(swscale-4 SHARED IMPORTED)

#设置库路径
set_target_properties(swscale-4
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libswscale-4.so)


#把c++文件生成一个名字为 native-lib 的动态库
add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        #每次添加了 C 文件，这里别忘了加上
        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp
        src/main/cpp/WlAudio.cpp
        src/main/cpp/WlCallJava.cpp
        src/main/cpp/WlFFmpeg.cpp)

#复用已经存在的动态库
#find_library( # Sets the name of the path variable.
#        com.test.myplayer.log-lib #别名

# Specifies the name of the NDK library that
# you want CMake to locate.
#        com.test.myplayer.log)

#连接动态库
target_link_libraries( # Specifies the target library.
        native-lib

        avcodec-57
        avfilter-6
        avformat-57
        avutil-55
        swresample-2
        swscale-4
        # Links the target library to the com.test.myplayer.log library
        # included in the NDK.
        #省略下面这句 find_library 代码块就可以省略了
        # ${com.test.myplayer.log-lib}
        log)