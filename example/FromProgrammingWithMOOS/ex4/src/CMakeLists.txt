if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(EXECNAME ex4)

find_package(MOOS 10)

SET(SRCS main.cpp Simulator.cpp)

include_directories( ${MOOS_INCLUDE_DIRS} ${MOOS_DEPEND_INCLUDE_DIRS} )

add_executable(${EXECNAME} ${SRCS})

target_link_libraries( ${EXECNAME} ${MOOS_LIBRARIES} ${MOOS_DEPEND_LIBRARIES})
