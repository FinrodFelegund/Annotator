cmake_minimum_required(VERSION 3.20)
project(Annotator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set( CMAKE_BUILD_TYPE Debug)

INCLUDE(FindPackageHandleStandardArgs)


FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIBRARY_DIRS})
ADD_DEFINITIONS(${OpenCV_DEFINITIONS})


find_package_handle_standard_args(openslide DEFAULT_MSG openslide_LIBRARY openslide_INCLUDE_DIR)
mark_as_advanced(OPENSLIDE_LIBRARY OPENSLIDE_INCLUDE_DIR)
find_path(openslide_INCLUDE_DIR NAMES openslide-features.h openslide.h PATH_SUFFIXES openslide)
find_library(openslide_LIBRARY NAMES openslide)

if(openslide_FOUND)
    set(OPENSLIDE_LIBRARIES    ${openslide_LIBRARY})
    set(OPENSLIDE_INCLUDE_DIRS ${openslide_INCLUDE_DIR})
    include_directories( ${OPENSLIDE_INCLUDE_DIRS} )
    message("Openslide found")
else()
    message("Openslide not found")
endif()

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)
if(Qt6_FOUND)
    message("found Qt6: " ${Qt6_ROOT})
    include_directories(${Qt6_INCLUDE_DIR})
endif()




add_executable(Annotator Resources.qrc main.cpp Reader/WholeSlideImageReader.cpp Reader/WholeSlideImageReader.h Threads/Worker.cpp Threads/Worker.h Threads/Manager.cpp Threads/Manager.h "Data Structures/TileJob.cpp" "Data Structures/TileJob.h" AnnotatorUI/AnnotatorMainWindow.cpp AnnotatorUI/AnnotatorMainWindow.h AnnotatorView/AnnotatorViewer.cpp AnnotatorView/AnnotatorViewer.h "Data Structures/GraphicsItem.cpp" "Data Structures/GraphicsItem.h" AnnotatorApplication/AnnotatorController.cpp AnnotatorApplication/AnnotatorController.h "Data Structures/Tile.cpp" "Data Structures/Tile.h" "Data Structures/Tiler.cpp" "Data Structures/Tiler.h" AnnotatorUI/Toolbar.cpp AnnotatorUI/Toolbar.h AnnotatorUI/Dockwidget.cpp AnnotatorUI/Dockwidget.h AnnotatorView/MiniMap.cpp AnnotatorView/MiniMap.h "Data Structures/ItemCache.cpp" "Data Structures/ItemCache.h" Reader/ImageReader.cpp Reader/ImageReader.h Reader/OpenCVImageReader.cpp Reader/OpenCVImageReader.h Reader/ImageFactory.cpp Reader/ImageFactory.h)
target_link_libraries(Annotator
        Qt::Core
        Qt::Gui
        Qt::Widgets
        ${OPENSLIDE_LIBRARIES}
        ${OpenCV_LIBRARIES})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

