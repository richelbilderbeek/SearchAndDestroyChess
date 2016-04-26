TEMPLATE = app
CONFIG += qt console
CONFIG -= app_bundle

SOURCES += main.cpp \
    UnitChessBoard.cpp \
    UnitChessMove.cpp \
    UnitChessPiece.cpp \
    UnitCoordinatGetter.cpp \
    piece_color.cpp \
    piece_type.cpp \
    game_dialog.cpp \
    command.cpp \
    game.cpp \
    game_state.cpp \
    textures.cpp

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

HEADERS += \
    UnitChessBoard.h \
    UnitChessMove.h \
    UnitChessPiece.h \
    UnitCoordinatGetter.h \
    piece_color.h \
    piece_type.h \
    game_dialog.h \
    command.h \
    game.h \
    game_state.h \
    textures.h
