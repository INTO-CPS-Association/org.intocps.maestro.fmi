package org.intocps.fmi.jnifmuapi.fmi3;
/* tag::DependencyKind[] */
// typedef enum {
//     fmi3Independent,
//     fmi3Constant,
//     fmi3Fixed,
//     fmi3Tunable,
//     fmi3Discrete,
//     fmi3Dependent
// } fmi3DependencyKind;
/* end::DependencyKind[] */

import java.util.Arrays;

public enum Fmi3DependencyKind {
    //    fmi3Independent,
    //    fmi3Constant,
    //    fmi3Fixed,
    //    fmi3Tunable,
    //    fmi3Discrete,
    //    fmi3Dependent


    Independent(0),
    Constant(1),
    Fixed(2),
    Tunable(3),
    Discrete(4),
    Dependent(5);

    private final int value;

    Fmi3DependencyKind(final int newValue) {
        value = newValue;
    }

    public int getValue() {
        return value;
    }

    public static Fmi3DependencyKind valueOf(int v) {
        return Arrays.stream(values()).filter(elem -> elem.value == v).findFirst().orElse(null);
    }
}
