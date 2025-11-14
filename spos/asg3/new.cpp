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




javac MyJavaClass.java
javac -h . MyJavaClass.java
echo $JAVA_HOME
g++ -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux -fPIC -shared -o libmyjava.so MyJavaClass.cpp
