cmake_minimum_required(VERSION 3.10)
project(MyImageApp)

# Especificar a versão do C++
set(CMAKE_CXX_STANDARD 17)

# Encontrar pacotes OpenCV e Qt
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Incluir diretórios com headers
include_directories(${OpenCV_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})

# Adicionar os arquivos fonte do projeto
add_executable(FPIFase1 main.cpp teste.cpp)

# Linkar as bibliotecas OpenCV e Qt
target_link_libraries(FPIFase1 ${OpenCV_LIBS} Qt5::Widgets)

# Para incluir PThreads
find_package(Threads REQUIRED)
target_link_libraries(FPIFase1 ${OpenCV_LIBS} Threads::Threads)
