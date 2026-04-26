cmake_minimum_required(VERSION 4.1)
project(algorithm C)

set(CMAKE_C_STANDARD 11)

include_directories(
        ${CMAKE_SOURCE_DIR}/utils
        ${CMAKE_SOURCE_DIR}/src/stack
        ${CMAKE_SOURCE_DIR}/src/tree
        ${CMAKE_SOURCE_DIR}/src/list
        ${CMAKE_SOURCE_DIR}/src/HashMap
        ${CMAKE_SOURCE_DIR}/datastruct/queue
        ${CMAKE_SOURCE_DIR}/src/graph
        ${CMAKE_SOURCE_DIR}/src/heap
        ${CMAKE_SOURCE_DIR}/src/algorithm
        ${CMAKE_SOURCE_DIR}/src/Greedy_Algorithm
)

# ============================================================
#  把公共模块编成静态库
# ============================================================
file(GLOB STACK_SOURCES "${CMAKE_SOURCE_DIR}/src/stack/*.c")
file(GLOB UTILS_SOURCES "${CMAKE_SOURCE_DIR}/utils/*.c")
file(GLOB LIST_SOURCES "${CMAKE_SOURCE_DIR}/src/list/*.c")
file(GLOB GRAPH_SOURCES "${CMAKE_SOURCE_DIR}/src/graph/*.c")
file(GLOB HASHMAP_SOURCES "${CMAKE_SOURCE_DIR}/src/HashMap/*.c")
file(GLOB HASHMAP_SOURCES "${CMAKE_SOURCE_DIR}/src/heap/*.c")
file(GLOB HASHMAP_SOURCES "${CMAKE_SOURCE_DIR}/src/algorithm/*.c")
file(GLOB HASHMAP_SOURCES "${CMAKE_SOURCE_DIR}/src/Greedy_Algorithm/*.c")
file(GLOB QUEUE_SOURCES "${CMAKE_SOURCE_DIR}/datastruct/queue/*.c")

add_library(common STATIC
        ${STACK_SOURCES}
        ${UTILS_SOURCES}
        ${LIST_SOURCES}
        ${GRAPH_SOURCES}
        ${HASHMAP_SOURCES}
        ${QUEUE_SOURCES}
)

# ============================================================
#  给含 main 的入口文件生成可执行目标
# ============================================================
file(GLOB_RECURSE ALGO_SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

set(ALL_TARGETS "")
foreach (source_file ${ALGO_SOURCES})
    string(FIND ${source_file} "${CMAKE_BINARY_DIR}" IS_BUILD_DIR)
    if (NOT IS_BUILD_DIR EQUAL -1)
        continue()
    endif ()

    string(FIND ${source_file} "/stack/" IN_STACK)
    string(FIND ${source_file} "/utils/" IN_UTILS)
    string(FIND ${source_file} "/list/" IN_LIST_DIR)   # 改这里
    string(FIND ${source_file} "/HashMap/" IN_HASHMAP)
    string(FIND ${source_file} "/queue/" IN_QUEUE)
    string(FIND ${source_file} "/heap/" IN_HEAP)
    string(FIND ${source_file} "/Greedy_Algorithm/" IN_GREEDY)
    string(FIND ${source_file} "/algorithm/" IN_ALGORITHM)

    if (NOT IN_STACK EQUAL -1)
        continue()
    endif ()
    if (NOT IN_UTILS EQUAL -1)
        continue()
    endif ()
    if (NOT IN_LIST_DIR EQUAL -1)                         # 改这里
        continue()
    endif ()
    if (NOT IN_HASHMAP EQUAL -1)
        continue()
    endif ()
    if (NOT IN_QUEUE EQUAL -1)
        continue()
    endif ()
    if (NOT IN_GREEDY EQUAL -1)
        continue()
    endif ()
    if (NOT IN_HEAP EQUAL -1)
        continue()
    endif ()
    if (NOT IN_ALGORITHM EQUAL -1)
        continue()
    endif ()

    get_filename_component(target_name ${source_file} NAME_WE)

    if (TARGET ${target_name})
        continue()
    endif ()

    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} common)

    if (NOT MSVC)
        target_link_libraries(${target_name} m)
    endif ()

    list(APPEND ALL_TARGETS ${target_name})
endforeach ()

add_custom_target(algorithm DEPENDS ${ALL_TARGETS})