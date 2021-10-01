package org.intocps.fmi3;
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

public enum Fmi3DependencyKind {
    fmi3Independent,
    fmi3Constant,
    fmi3Fixed,
    fmi3Tunable,
    fmi3Discrete,
    fmi3Dependent
}
