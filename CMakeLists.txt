cmake_minimum_required(VERSION 3.16)

project(glslcodegen)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fpermissive")
set(CMAKE_CXX_EXTENSIONS OFF)

add_definitions(-DUINT_MAX=4294967295)

# Add your source files here
set(SOURCES
    modules/reshade/source/effect_codegen_glsl.cpp
    # modules/reshade/source/effect_codegen_hlsl.cpp
    # modules/reshade/source/effect_codegen_spirv.cpp
    modules/reshade/source/effect_expression.cpp
    modules/reshade/source/effect_lexer.cpp
    modules/reshade/source/effect_parser_exp.cpp
    modules/reshade/source/effect_parser_stmt.cpp
    modules/reshade/source/effect_preprocessor.cpp
    modules/reshade/source/effect_symbol_table.cpp
    main.cpp
)

add_executable(glslcodegen ${SOURCES})

# Add any additional include directories here
target_include_directories(glslcodegen
    SYSTEM BEFORE PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/modules/reshade/source)