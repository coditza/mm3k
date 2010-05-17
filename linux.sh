#!/bin/sh
THISPATH="`pwd`/"
if [ "$1" = ""  ]
then
    MAKETARGET=mm3k
else
    MAKETARGET=$1
fi


cd build/linux/

make WXLFLAGS="`wx-config --libs`" WXCXXFLAGS="`wx-config --cxxflags`" PROJECTDIR=$THISPATH $MAKETARGET
cd $THISPATH
