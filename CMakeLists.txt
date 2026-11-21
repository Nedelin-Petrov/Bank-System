
cmake_minimum_required(VERSION 3.20)

project(BankSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BankSystem main.cpp
        Customer.cpp
        Customer.h
        Transaction.cpp
        Transaction.h
        Account.cpp
        Account.h
        BankSystem.cpp
        BankSystem.h
        Utils.cpp
        Utils.h
)

target_compile_options(BankSystem PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>)