public class sample {
    public native void shoutout();

    static {
        System.loadLibrary("new");
    }

    public static void main(String[] args) {
        new sample().shoutout();
    }
}
