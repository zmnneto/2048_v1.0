# CMakeList.txt: projeto de CMake para app2048, incluir origem e definir
# lógica específica a um projeto aqui.
#

# Adicione a origem ao executável deste projeto.
add_executable (app2048 "app2048.cpp" "app2048.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET app2048 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Adicione testes e instale destinos, se necessário.
