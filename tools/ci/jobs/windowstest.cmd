mkdir test
cd test
mkdir %1
xcopy /s C:\gitlabrunner\%1 %1
cd %1
copy ..\..\src\manaplustests.exe .\
dir
manaplustests.exe
