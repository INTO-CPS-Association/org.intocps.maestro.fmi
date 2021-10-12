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
    fmi3OK,
    fmi3Warning,
    fmi3Discard,
    fmi3Error,
    fmi3Fatal
}
