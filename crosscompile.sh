#!/bin/sh
# Crosscompiles Search And Destroy Chess to the Windows operating system

DESKTOP_PRO=SearchAndDestroyChess.pro

if [ ! -e $DESKTOP_PRO ]
then
  echo "ERROR: Cannot find "$DESKTOP_PRO
  exit
fi
i686-w64-mingw32.static-qmake-qt5 $DESKTOP_PRO
make

DESKTOP_EXE=./release/SearchAndDestroyChess.exe

if [ ! -e $DESKTOP_EXE ]
then
  echo "ERROR: Cannot find "$DESKTOP_EXE
else
  echo "OK: Can find "$DESKTOP_EXE
fi

mv $DESKTOP_EXE SearchAndDestroyChess.exe
