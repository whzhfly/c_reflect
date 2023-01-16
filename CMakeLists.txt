cmake_minimum_required(VERSION 3.0.0)
project(rain_day VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-rtti")
set(CMAKE_CXX_CFLAGS "${CMAKE_CXX_CFLAGS} -fno-rtti")

set(CMAKE_PREFIX_PATH $(CMAKE_PREFIX_PATH) "/mnt/f/GitFile/llvm-project/build") # 设置前置使用


# recurse add
file(GLOB_RECURSE HEADERS "./src/*.hpp")
file(GLOB_RECURSE SOURCES "./src/*.cpp")
message(STATUS "HEADERS ${HEADERS}")
message(STATUS "SOURCES ${SOURCES}")

aux_source_directory(src sourceList)
aux_source_directory(src/parse parseList)
# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

add_executable(
	rain_day
	${HEADERS}
	${SOURCES}
	# cmdargs.cpp
	# cmdargs.hpp # 设置路径 加入include
	# main.cpp
)

# add_executable(
# 	rain_day
# 	${sourceList}
# 	${parseList}
# 	# cmdargs.cpp
# 	# cmdargs.hpp # 设置路径 加入include
# 	# main.cpp
# )

## LLVM LINK

find_package(LLVM REQUIRED CONFIG)

# include
include_directories(${LLVM_INCLUDE_DIRS})


# msg
# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# message(STATUS "LLVM_INCLUDE_LIBS== : ${LLVM_INCLUDE_LIBS}")
# message(STATUS "LLVM_INCLUDE_DIRS == : ${LLVM_INCLUDE_DIRS}")



separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
# message(STATUS "LLVM_DEFINITIONS_LIST == : ${LLVM_DEFINITIONS_LIST}")


# link lib
llvm_map_components_to_libnames(llvm_libs support core irreader)
# message(STATUS "llvm_libs == : ${llvm_libs}")
target_link_libraries(rain_day ${llvm_libs})


## Clang--Link
# find libclang

find_path(LibClang_INCLUDE_DIR clang-c/Index.h)
find_library(LIBCLANG_LIBRARY NAMES clang)

# message(STATUS "LibClang_INCLUDE_DIR == : ${LibClang_INCLUDE_DIR}")
# message(STATUS "LIBCLANG_LIBRARY == : ${LIBCLANG_LIBRARY}")


link_directories(${LIBCLANG_LIBRARY})
target_link_libraries(rain_day ${LIBCLANG_LIBRARY})

## Check Clang Exist ？？
find_package(Clang REQUIRED CONFIG)
message(STATUS "CLANG_INCLUDE_DIRS == : ${CLANG_INCLUDE_DIRS}")
message(STATUS "CLANG_LIBS == : ${CLANG_LIBS}") # NONO

include_directories(${CLANG_INCLUDE_DIRS}) # add clang source
add_definitions(${CLANG_DEFINITIONS})

# message(STATUS "clangFrontend == : ${clangFrontend}")

set(CC_LIB $(CC_LIB) ${CMAKE_CURRENT_SOURCE_DIR}/lib) # 设置前置使用
message(STATUS "CC_LIB == : ${CC_LIB}")
link_directories($(CC_LIB))


message(STATUS "clangAST == : ${clangAST}") # # NONO

