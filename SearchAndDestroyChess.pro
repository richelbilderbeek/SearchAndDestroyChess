TEMPLATE = app
CONFIG += qt console
CONFIG -= app_bundle

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

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Weffc++
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

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
