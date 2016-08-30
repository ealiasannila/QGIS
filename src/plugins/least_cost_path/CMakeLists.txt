
########################################################
# Files

SET (leastcostpath_SRCS
     leastcostpath.cpp
     leastcostpathgui.cpp
    testclass.cpp
    ../../../../lcpc/lib/common/shapes.cc
    ../../../../lcpc/lib/sweep/advancing_front.cc
    ../../../../lcpc/lib/sweep/cdt.cc
    ../../../../lcpc/lib/sweep/sweep.cc
    ../../../../lcpc/lib/sweep/sweep_context.cc
    ../../../../lcpc/src/Coords.cpp
    ../../../../lcpc/src/Funnel.cpp
    ../../../../lcpc/src/lcpfinder.cpp

)

SET (leastcostpath_UIS leastcostpathguibase.ui)

SET (leastcostpath_MOC_HDRS
     leastcostpath.h
     leastcostpathgui.h
    testclass.h
    ../../../../lcpc/lib/poly2tri.h
    ../../../../lcpc/src/minHeap.h
    ../../../../lcpc/src/Coords.h
    ../../../../lcpc/src/Funnel.h
    ../../../../lcpc/src/defs.h
    ../../../../lcpc/src/geomfunc.h
    ../../../../lcpc/src/lcpfinder.h
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

