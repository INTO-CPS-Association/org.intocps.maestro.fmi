package org.intocps.fmi.jnifmuapi.fmi3;

import java.util.Arrays;

public enum Fmi3EventQualifier {

    EventFalse(0),
    EventTrue(1),
    EventUnknown(2);

    private final int value;

    Fmi3EventQualifier(final int newValue) {
        value = newValue;
    }

    public int getValue() {
        return value;
    }

    public static Fmi3EventQualifier valueOf(int v) {
        return Arrays.stream(values()).filter(elem -> elem.value == v).findFirst().orElse(null);
    }

}
