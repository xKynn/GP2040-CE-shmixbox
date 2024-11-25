add_library(CaseLEDs
src/CaseLEDs.cpp
)
target_include_directories(CaseLEDs PUBLIC 
src
)
target_link_libraries(CaseLEDs 
pico_stdlib
)