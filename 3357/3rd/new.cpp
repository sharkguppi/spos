#include <jni.h>
#include <iostream>
using namespace std;

extern "C" JNIEXPORT void JNICALL Java_sample_shoutout(JNIEnv *env, jobject obj) {
    cout << "Hello Ma'am" << endl;
}

















/*
# 0) Install JDK and C++ toolchain
sudo apt update
sudo apt install -y openjdk-21-jdk build-essential

# 1) Set JAVA_HOME for this shell
export JAVA_HOME="$(dirname "$(dirname "$(readlink -f "$(which javac)")")")"

# 2) Go to your project folder (adjust path)
cd /path/to/your/project

# 3) Compile Java and generate JNI header
javac -h . sample.java

# 4) Build the shared library (.so)
g++ -fPIC -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" -shared -o libnew.so new.cpp

# 5) Run Java (enable native access to suppress warnings)
java --enable-native-access=ALL-UNNAMED -Djava.library.path=. sample

*/
/*# 0) Change to project folder
Set-Location -Path 'D:\pendrive_backup\3357\3rd'

# 1) Quick detection of required tools
where.exe javac
where.exe java
where.exe g++
where.exe cl

# 2) Set JAVA_HOME (PowerShell) based on the first javac found
$javac = (where.exe javac | Select-Object -First 1)
$env:JAVA_HOME = Split-Path -Parent (Split-Path -Parent $javac)
Write-Host "JAVA_HOME = $env:JAVA_HOME"

# 3) Compile Java and generate JNI header
javac -h . sample.java

# 4) Build native DLL with MinGW-w64 g++ (Windows headers)
g++ -I"$env:JAVA_HOME\include" -I"$env:JAVA_HOME\include\win32" -shared -o new.dll new.cpp

# If you get unresolved symbols from standard library, try:
# g++ -I"$env:JAVA_HOME\include" -I"$env:JAVA_HOME\include\win32" -shared -o new.dll new.cpp -static-libgcc -static-libstdc++

# 5) (Alternate) Build native DLL with MSVC (use Developer Command Prompt / vcvars)
# Run this from Developer Command Prompt or call vcvarsall.bat first
# In plain cmd (example):
# cl /I "%JAVA_HOME%\include" /I "%JAVA_HOME%\include\win32" /LD new.cpp /Fe:new.dll

# 6) Confirm files present
Get-ChildItem -Name

# 7) Run the Java program (use cmd /c wrapper to avoid PowerShell arg parsing issues)
cmd /c "java --enable-native-access=ALL-UNNAMED -Djava.library.path=. sample"

# Alternative PowerShell-safe invocation
& java "--enable-native-access=ALL-UNNAMED" "-Djava.library.path=." sample

# Or without the native-access option (you'll get a warning)
cmd /c "java -Djava.library.path=. sample"

# 8) (Optional) Clean up build artifacts
Remove-Item -Path sample.class,sample.h,new.dll -ErrorAction SilentlyContinue*/



/*
# 0) Change to project folder
Set-Location -Path 'D:\pendrive_backup\3357\3rd'

# 1) Quick detection of required tools
where.exe javac
where.exe java
where.exe g++
where.exe cl

# 2) Set JAVA_HOME (PowerShell) based on the first javac found
$javac = (where.exe javac | Select-Object -First 1)
$env:JAVA_HOME = Split-Path -Parent (Split-Path -Parent $javac)
Write-Host "JAVA_HOME = $env:JAVA_HOME"

# 3) Compile Java and generate JNI header
javac -h . sample.java

# 4) Build native DLL with MinGW-w64 g++ (Windows headers)
g++ -I"$env:JAVA_HOME\include" -I"$env:JAVA_HOME\include\win32" -shared -o new.dll new.cpp

# If you get unresolved symbols from standard library, try:
# g++ -I"$env:JAVA_HOME\include" -I"$env:JAVA_HOME\include\win32" -shared -o new.dll new.cpp -static-libgcc -static-libstdc++

# 5) (Alternate) Build native DLL with MSVC (use Developer Command Prompt / vcvars)
# Run this from Developer Command Prompt or call vcvarsall.bat first
# In plain cmd (example):
# cl /I "%JAVA_HOME%\include" /I "%JAVA_HOME%\include\win32" /LD new.cpp /Fe:new.dll

# 6) Confirm files present
Get-ChildItem -Name

# 7) Run the Java program (use cmd /c wrapper to avoid PowerShell arg parsing issues)
cmd /c "java --enable-native-access=ALL-UNNAMED -Djava.library.path=. sample"

# Alternative PowerShell-safe invocation
& java "--enable-native-access=ALL-UNNAMED" "-Djava.library.path=." sample

# Or without the native-access option (you'll get a warning)
cmd /c "java -Djava.library.path=. sample"

# 8) (Optional) Clean up build artifacts
Remove-Item -Path sample.class,sample.h,new.dll -ErrorAction SilentlyContinue


*/