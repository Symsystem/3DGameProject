3DGameProject
=============


mkdir build
mkdir bin
mkdir lib


for var in main AppDemarrage InputListener entity/Entite entity/NewCamera entity/Object3D entity/Player
    do
           #Statement(s) to be executed for every word.
              g++ -I/usr/include/OIS -I"/usr/include/OGRE" -I"/usr/include/OGRE/Overlay" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"build/$var.d" -MT"build/$var.d" -o "build/$var.o" "src/$var.cpp"
              done

# Adapt this line to your install.
# To find the right folder : find /usr -iname libOgreMain*
SHAREDLIB=/usr/lib/x86_64-linux-gnu/
g++ -L"$SHAREDLIB" -o "bin/3DGameProject.out" ./build/entity/Entite.o ./build/entity/NewCamera.o ./build/entity/Object3D.o ./build/entity/Player.o ./build/AppDemarrage.o ./build/InputListener.o ./build/main.o -lOIS -lboost_system -lOgreMain -lOgreTerrain -lOgreOverlay

