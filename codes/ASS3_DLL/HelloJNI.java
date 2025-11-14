public class HelloJNI {
    // Declare the native method (implemented in C)
    public native void sayHello();

    // Static block to load the shared library
    static {
        System.loadLibrary("hello");  // Loads libhello.so
    }

    public static void main(String[] args) {
        HelloJNI hello = new HelloJNI();
        hello.sayHello();  // Calls the native method
    }
}//gcc -shared -fPIC -o libhello.so -I/usr/lib/jvm/java-11-openjdk-amd64/include -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux HelloJNI.c