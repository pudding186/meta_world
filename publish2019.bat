if "%TIME:~0,1%"==" " (
set date=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%_0%TIME:~1,1%%TIME:~3,2%%TIME:~6,2%
) else (
set date=%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%_%TIME:~0,2%%TIME:~3,2%%TIME:~6,2%
)

set builder="C:/Program Files (x86)/Microsoft Visual Studio/2019/Enterprise/MSBuild/Current/Bin/MSBuild.exe"

set branch=..

@echo 打标签 %date%
IF NOT EXIST "%branch%\%date%" (
md "%branch%\%date%"
git clone https://gitee.com/pudding331/meta_world.git %branch%\%date%
cd /d %branch%\%date%
) else (
cd /d %branch%\%date%
git pull
)

git tag -a %date% -m"add tag %date%"
git push origin %date%

@echo 打标签 %date%成功