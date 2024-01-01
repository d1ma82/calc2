cmake_minimum_required(VERSION 3.22)


#cmake_path(CONVERT $ENV{COMPILER} TO_CMAKE_PATH_LIST COMPILER)
#set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_C_COMPILER "${COMPILER}/clang.exe")
#set(CMAKE_RC_COMPILER "llvm-rc")
set(CMAKE_BUILD_TYPE Release)

add_definitions(-std=c++17)
add_compile_definitions(DEBUG_INFO)

project (calc)

file(GLOB SRC 
	"./src/main.cpp"
    "./src/lexic.cpp"
    "./src/syntax.cpp"
    "./src/token.cpp"
    "./src/postfix.cpp"
    "./src/exec.cpp"
)

add_executable (${PROJECT_NAME} ${SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "../bin"
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE "./src" 
)