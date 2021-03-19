project(Lab1 C)

set(CMAKE_C_STANDARD 11)

set(allocator_SOURCES arena.h header.h allocator.h allocator.c driver.h header.c arena.c main.c tree.h tree.c rbtree/rbtree.c rbtree/rbtree.h Utils/align_utils.h Utils/align_utils.c)

if(WIN32)
    set(allocator_SOURCES ${allocator_SOURCES} Drivers/windriver.c)
elseif(UNIX)
    set(allocator_SOURCES ${allocator_SOURCES} Drivers/linuxdriver.c Utils/align_utils.c)
endif()

add_executable(Lab1 ${allocator_SOURCES})