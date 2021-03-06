@echo off
echo ExeSketch console version compiler script
echo - - - - - - - - - - - - - - - - - - - - -
echo Creating build directory...
mkdir build
echo Copying images to build directory...
copy Bezier.bmp build
copy Circle.bmp build
copy Polygon.bmp build
copy Rect.bmp build
copy Object_view.bmp build
copy Editing.bmp build
echo Building...
g++ main.cpp event.cpp objectdisplay.cpp object.cpp objectmanager.cpp vector2d.cpp buttonmenu.cpp rect.cpp polygon.cpp circle.cpp bezier.cpp imageloader.cpp -o build/ExeSketch.exe -lfreeglut -lopengl32 -lglu32
echo Done!
pause