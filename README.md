# Archanoid Game
This is my first year Algorithms course project that i wrote with C using allegro 4.2 library.

To open this project(for windows on VS), you need to download allegro-msvc10-4.2.3.zip from link below
https://www.allegro.cc/files/?v=4.2  (direct link http://cdn.allegro.cc/file/library/allegro-4.2.3/allegro-msvc10-4.2.3.zip)

After download Allegro library, extract the file and you will see 3 folder namely bin,include,lib
You need the copy these folders to 2 different places.

1-)Copy bin and include folders to VC folder that is located C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC,
and copy lib folder to Microsoft Visual Studio 14.0 folder(go back 1 folder from VC).

2-)There are 3 dll files in bin folder, if your system is x64, copy these dll files to C:\Windows\SysWOW64 or
if your system is x86, copy dll files to C:\Windows\system32.

Last step before run the program.

Open Visual Studio and follow the steps below
Project->Properties->Configuration Properties->Linker->Input 
edit to Additional Dependencies and add alleg.lib.
After that you can run the project. 

To run the project in other platforms you can check https://wiki.allegro.cc/index.php?title=Allegro_4
