#!/bin/sh
echo "Building ExeSketch"
echo "--Making Build Direcroty"
mkdir build/
echo "--Copying image files"
cp Circle.bmp build/Circle.bmp
echo "----Circle.bmp done"
cp Rect.bmp build/Rect.bmp
echo "----Rect.bmp done"
cp Polygon.bmp build/Polygon.bmp
echo "----Polygon.bmp done"
echo "--Running Makefile"
make
echo "Done."
echo "Loading ExeSketch"
cd build/
./ExeSketch
echo "Done."