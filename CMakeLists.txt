################################################################################
# Para mas información de este archivo visite la pagina de cmake:
# https://cmake.org/documentation/
################################################################################

#Establece la versión mínima requerida de cmake para un proyecto.
cmake_minimum_required(VERSION 2.8)

#Establece un nombre para el proyecto
project (latino-redis)

#Agrega un subdirectorio a la compilación.
include_directories(hiredis)
include_directories(hiredis/src)

#Agrega una biblioteca al proyecto utilizando los
#archivos de origen especificados.
add_library(latino-redis SHARED src/latino-redis.c)

#Vincula bibliotecas a todos los objetivos agregados posteriormente.
link_libraries(hiredis latino)

#Vincula un objetivo a bibliotecas determinadas.
target_link_libraries(latino-redis hiredis latino)

#Especifica las reglas que se ejecutarán en el momento de la instalación.
install(FILES liblatino-redis.so DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
