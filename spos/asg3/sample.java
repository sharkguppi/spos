public class sample {
    public native void func();

    static {
        System.loadLibrary("new");
    }

    public static void main(String[] args) {
        new sample().shoutout();
    }
}
