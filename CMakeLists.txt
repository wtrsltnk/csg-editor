cmake_minimum_required(VERSION 3.3)

project(MapViewer)

include(win-cpp-deps.cmake/win-cpp-deps.cmake)

install_dep("https://bitbucket.org/wtrsltnk/bullet3.git")
install_dep("https://bitbucket.org/wincppdeps/freeglut.git")
install_dep("https://bitbucket.org/wincppdeps/glm.git")
install_dep("https://bitbucket.org/wincppdeps/glextloader.git")

find_package(OpenGL REQUIRED)
find_package(BULLET3 REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLM REQUIRED)
find_package(GLEXTL REQUIRED)

set(CONTEXT_FILES
    ../Context/Common/BoundingVolume.cpp
    ../Context/Common/Common.cpp
    ../Context/Common/FileLoader.cpp
    ../Context/Common/TextureLoader.cpp
    ../Context/Common/Texture.cpp
    ../Context/Common/Tokenizer.cpp
    ../Context/Common/uiFont.cpp
    ../Context/Common/stb_truetype.cpp
    ../Context/Common/Camera.cpp
    ../Context/Context/GlContext.cpp
    ../Context/Context/UserInput.cpp
    ../Context/Geometry/Scene.cpp
    ../Context/Geometry/MapLoader.cpp
    ../Context/Geometry/Brush.cpp
    ../Context/Geometry/Entity.cpp
    )

set(FILES
    MapViewer.h
    Status.h
    MapViewer.cpp
    Status.cpp
    AllInOneTool.cpp
    AllInOneTool.h
    MorphTool.cpp
    MorphTool.h
    Tool.cpp
    Tool.h
    )

add_executable(MapViewer ${FILES} ${CONTEXT_FILES})

target_include_directories(MapViewer
    PRIVATE ../Context/Common
    PRIVATE ../Context/Context
    PRIVATE ../Context/Geometry
    PRIVATE ${BULLET3_INCLUDE_DIR}
    PRIVATE ${GLUT_INCLUDE_DIR}
    PRIVATE ${GLM_INCLUDE_DIR}
    PRIVATE ${GLEXTL_INCLUDE_DIR}
    )

target_compile_features(MapViewer
    PRIVATE cxx_auto_type
    PRIVATE cxx_nullptr
    PRIVATE cxx_range_for
    )

target_link_libraries(MapViewer
    ${OPENGL_LIBRARIES}
    ${BULLET3_LIBRARIES}
    ${GLUT_LIBRARIES}
    )

#target_compile_definitions(MapViewer
#    )
