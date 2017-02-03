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