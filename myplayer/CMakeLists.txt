#版本
cmake_minimum_required(VERSION 3.4.1)

#导入头文件目录，不然就要逐个文件的导入
include_directories(src/main/cpp/include)

#添加动态库
add_library( avcodec-57 SHARED IMPORTED)


#把c++文件生成一个名字为 native-lib 的动态库
add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp)

#复用已经存在的动态库
#find_library( # Sets the name of the path variable.
#        log-lib #别名

        # Specifies the name of the NDK library that
        # you want CMake to locate.
#        log)

#连接动态库
target_link_libraries( # Specifies the target library.
        native-lib

        # Links the target library to the log library
        # included in the NDK.
        #省略下面这句 find_library 代码块就可以省略了
        # ${log-lib}
        log)