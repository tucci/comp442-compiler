I used Visual Studio 2015 for this project.

If you use Visual Studio 2015 v140 then it should compile easily when you open the solution file.

If you use Visual Studio 2012 v120, then
	- right click the project in the Solution Explorer on the right
	- Go to properties
	- Go to Configuration Properties
	- go to in the general section
	- Change platform toolset to v120
	- Then build in the toolset

Or follow instructions here
https://msdn.microsoft.com/en-CA/library/ff770576.aspx


To run the tests
on 32bit machine: Go to Test>Test Settings > Processor Architecture > x86
on 64bit machine: Go to Test>Test Settings > Processor Architecture > x64

Or follow instructions here
https://msdn.microsoft.com/en-us/library/ee782531.aspx

Since the moon code is being run through visual studio, there are two options.

If you are using visual studio 2013, copy the moon.exe from the moon_for_vs2013 into the comp442_compilers folder
If you are using visual studio 2015, copy the moon.exe from the moon_for_vs2015 into the comp442_compilers folder

If none of those work. Take the codegen output of the sourcefile.m file and run with your own moon.exe

The input file for the code is sourcefile.txt
and the output code gen file is sourcefile.m