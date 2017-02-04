SOURCES += main.cpp \
    chess_board.cpp \
    chess_move.cpp \
    chess_piece.cpp \
    coordinat_getter.cpp \
    piece_color.cpp \
    piece_type.cpp \
    game_dialog.cpp \
    command.cpp \
    game.cpp \
    game_state.cpp \
    textures.cpp

HEADERS += \
    chess_board.h \
    chess_move.h \
    chess_piece.h \
    coordinat_getter.h \
    piece_color.h \
    piece_type.h \
    game_dialog.h \
    command.h \
    game.h \
    game_state.h \
    textures.h

# C++14
CONFIG += c++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -std=c++14

# Compile clean with high warning levels
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
