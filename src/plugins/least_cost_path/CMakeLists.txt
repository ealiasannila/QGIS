
########################################################
# Files

SET (leastcostpath_SRCS
     leastcostpath.cpp
     leastcostpathgui.cpp
    testclass.cpp
    lib/common/shapes.cc
    lib/sweep/advancing_front.cc
    lib/sweep/cdt.cc
    lib/sweep/sweep.cc
    lib/sweep/sweep_context.cc
    src/Coords.cpp
    src/Funnel.cpp
    src/lcpfinder.cpp

)

SET (leastcostpath_UIS leastcostpathguibase.ui)

SET (leastcostpath_MOC_HDRS
     leastcostpath.h
     leastcostpathgui.h
    testclass.h
    lib/poly2tri.h
    src/minHeap.h
    src/Coords.h
    src/Funnel.h
    src/defs.h
    src/geomfunc.h
    src/lcpfinder.h
)

SET (leastcostpath_RCCS  leastcostpath.qrc)

########################################################
# Build

QT4_WRAP_UI (leastcostpath_UIS_H  ${leastcostpath_UIS})

QT4_WRAP_CPP (leastcostpath_MOC_SRCS  ${leastcostpath_MOC_HDRS})

QT4_ADD_RESOURCES(leastcostpath_RCC_SRCS ${leastcostpath_RCCS})

ADD_LIBRARY (leastcostpathplugin MODULE ${leastcostpath_SRCS} ${leastcostpath_MOC_SRCS} ${leastcostpath_RCC_SRCS} ${leastcostpath_UIS_H})

INCLUDE_DIRECTORIES(
     ${CMAKE_CURRENT_BINARY_DIR}
     ../../core ../../core/geometry ../../core/raster
     ../../gui
     ..
)

TARGET_LINK_LIBRARIES(leastcostpathplugin
  qgis_core
  qgis_gui
)


########################################################
# Install

INSTALL(TARGETS leastcostpathplugin
  RUNTIME DESTINATION ${QGIS_PLUGIN_DIR}
  LIBRARY DESTINATION ${QGIS_PLUGIN_DIR})

