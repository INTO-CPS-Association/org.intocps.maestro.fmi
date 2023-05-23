package org.intocps.fmi.jnifmuapi.fmi3;

/* tag::Status[] */
//    typedef enum {
//        fmi3OK,
//                fmi3Warning,
//                fmi3Discard,
//                fmi3Error,
//                fmi3Fatal,
//    } fmi3Status;
/* end::Status[] */
public enum Fmi3Status {


    OK(0),

    Warning(1),

    Discard(2),

    Error(3),

    Fatal(4);

    public final int value;

    Fmi3Status(int v) {
        value = v;
    }

    public static Fmi3Status valueOf(byte v) {
        for (Fmi3Status item : values()) {
            if (item.value == v) {
                return item;
            }
        }
        return null;
    }
}
