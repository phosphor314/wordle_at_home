add_executable(wordle_at_home "main.cpp" "wordle.h" "wordle.cpp" "layout.h" "layout.cpp")

target_link_libraries(wordle_at_home sfml-system sfml-graphics sfml-window)
