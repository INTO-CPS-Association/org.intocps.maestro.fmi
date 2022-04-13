package org.intocps.fmi.jnifmuapi.fmi3;

import java.util.Arrays;

/* tag::IntervalQualifier[] */
//typedef enum {
//        fmi3IntervalNotYetKnown,
//        fmi3IntervalUnchanged,
//        fmi3IntervalChanged
//} fmi3IntervalQualifier;
/* end::IntervalQualifier[] */
public enum Fmi3IntervalQualifier {
    //    fmi3IntervalNotYetKnown,
    //    fmi3IntervalUnchanged,
    //    fmi3IntervalChanged

    IntervalNotYetKnown(0),
    IntervalUnchanged(1),
    IntervalChanged(2);

    private final int value;

    Fmi3IntervalQualifier(final int newValue) {
        value = newValue;
    }

    public int getValue() {
        return value;
    }

    public static Fmi3IntervalQualifier valueOf(int v) {
        return Arrays.stream(values()).filter(elem -> elem.value == v).findFirst().orElse(null);
    }
}
