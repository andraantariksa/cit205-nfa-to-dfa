cmake_minimum_required(VERSION 3.16)
project(nfa_to_dfa)

set(CMAKE_CXX_STANDARD 17)

add_executable(nfa_to_dfa
        src/main.cpp)

target_include_directories(nfa_to_dfa
        PRIVATE

        external/graphviz/lib/gvc
        external/graphviz/lib/common
        external/graphviz/lib/pathplan
        external/graphviz/lib/cgraph
        external/graphviz/lib/cdt)

target_link_directories(nfa_to_dfa
        PRIVATE

        )

target_link_libraries(nfa_to_dfa
        gvc
        pathplan
        cgraph
        cdt)
