#
#CMakeLists generates the makefile for joinless apriori algorithm
# 


cmake_minimum_required(VERSION 2.6)

project(apriori)

add_executable(apriori main.c apriori_joinless apriori_joinless.h)

install(TARGETS apriori DESTINATION bin)


