if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

set(EXECNAME yGlobalInformation)

find_package(MOOS 10)

SET(SRCS main.cpp GlobalInformation.cpp)

include_directories( ${MOOS_INCLUDE_DIRS} ${MOOS_DEPEND_INCLUDE_DIRS} )
link_directories(${MOOS_LIBRARY_PATH})

find_package(MOOSGeodesy)
include_directories(${MOOSGeodesy_INCLUDE_DIRS})
link_directories(${MOOSGeodesy_LIBRARY_PATH})

SET(IVP_INCLUDE /home/haizhi/moos-ivp/include/ivp)
include_directories( ${IVP_INCLUDE} )


add_executable(${EXECNAME} ${SRCS})

target_link_libraries( ${EXECNAME}
  ${MOOS_LIBRARIES}
  ${MOOS_DEPEND_LIBRARIES}
  ${MOOSGeodesy_LIBRARIES}
  ${MOOSGeodesy_DEPEND_LIBRARIES}
  ${SYSTEM_LIBS})
