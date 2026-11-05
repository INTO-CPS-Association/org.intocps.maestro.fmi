package org.intocps.fmi.jnifmuapi;

import org.intocps.fmi.Fmi2Status;
import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.IFmiComponent;
import org.intocps.fmi.IFmuCallback;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.File;
import java.util.concurrent.atomic.AtomicInteger;

import static org.junit.Assert.*;

public class FmuLifecycleTest {
    private Fmu fmu;

    @Before
    public void load() throws Exception {
        fmu = new Fmu(new File(TestUtils.DUMMY_FMU_PATH));
        fmu.load();
    }

    @After
    public void unload() throws Exception {
        fmu.unLoad();
    }

    @Test
    public void repeatedCyclesWithoutCallback() throws Exception {
        repeatCycles(null);
    }

    @Test
    public void repeatedCyclesWithCallback() throws Exception {
        AtomicInteger calls = new AtomicInteger();
        repeatCycles(callback(calls));
        assertTrue(calls.get() > 0);
    }

    private void repeatCycles(IFmuCallback callback) throws Exception {
        for (int i = 0; i < 100; i++) {
            IFmiComponent component = fmu.instantiate(TestUtils.guid, "repeated", false, true, callback);
            assertNotNull(component);
            assertEquals(Fmi2Status.OK, component.setupExperiment(false, 0, 0, true, 1));
            assertEquals(Fmi2Status.OK, component.enterInitializationMode());
            assertEquals(Fmi2Status.OK, component.exitInitializationMode());
            assertEquals(Fmi2Status.OK, component.terminate());
            component.freeInstance();
            assertFalse(component.isValid());
        }
    }

    @Test
    public void failedInstantiationWithAndWithoutCallbackCanBeRetried() throws Exception {
        for (int i = 0; i < 20; i++) {
            assertNull(fmu.instantiate("incorrect-guid", "retry", false, true, null));
            assertNull(fmu.instantiate("incorrect-guid", "retry", false, true, callback(new AtomicInteger())));
            IFmiComponent component = fmu.instantiate(TestUtils.guid, "retry", false, true, null);
            assertNotNull(component);
            component.freeInstance();
        }
    }

    @Test
    public void callbackExceptionsPropagateAndCleanupAllowsRetry() throws Exception {
        IFmuCallback throwing = new IFmuCallback() {
            @Override
            public void log(String instanceName, Fmi2Status status, String category, String message) {
                throw new IllegalStateException("callback failure");
            }

            @Override
            public void stepFinished(Fmi2Status status) {
            }
        };
        IllegalStateException failure = assertThrows(IllegalStateException.class,
                () -> fmu.instantiate(TestUtils.guid, "retry", false, true, throwing));
        assertEquals("callback failure", failure.getMessage());
        IFmiComponent component = fmu.instantiate(TestUtils.guid, "retry", false, true, null);
        assertNotNull(component);
        component.freeInstance();
    }

    @Test
    public void sameNameInstancesKeepSeparateCallbacks() throws Exception {
        AtomicInteger firstCalls = new AtomicInteger();
        AtomicInteger secondCalls = new AtomicInteger();
        IFmiComponent first = fmu.instantiate(TestUtils.guid, "same-name", false, true, callback(firstCalls));
        IFmiComponent second = fmu.instantiate(TestUtils.guid, "same-name", false, true, callback(secondCalls));
        firstCalls.set(0);
        secondCalls.set(0);
        first.setDebugLogging(true, new String[0]);
        assertTrue(firstCalls.get() > 0);
        assertEquals(0, secondCalls.get());
        first.freeInstance();
        second.setDebugLogging(true, new String[0]);
        assertTrue(secondCalls.get() > 0);
        second.freeInstance();
    }

    @Test
    public void nativeCallbackTestAcceptsNullLogCategoryAndMessage() throws Exception {
        AtomicInteger calls = new AtomicInteger();
        IFmiComponent component = fmu.instantiate(TestUtils.guid, fmu.name, false, true, callback(calls));
        int before = calls.get();
        fmu.testNativeCallback();
        assertEquals(before + 1, calls.get());
        component.freeInstance();
    }

    @Test
    public void exceptionDuringFreeDoesNotLeaveAValidInstance() throws Exception {
        IFmuCallback throwing = new IFmuCallback() {
            @Override
            public void log(String instanceName, Fmi2Status status, String category, String message) {
                if (message.contains("fmi2FreeInstance")) {
                    throw new IllegalStateException("free callback failure");
                }
            }

            @Override
            public void stepFinished(Fmi2Status status) {
            }
        };
        IFmiComponent component = fmu.instantiate(TestUtils.guid, "free", false, true, throwing);
        assertThrows(IllegalStateException.class, component::freeInstance);
        assertFalse(component.isValid());
        assertThrows(FmiInvalidNativeStateException.class, component::freeInstance);
    }

    @Test
    public void callbackCannotUnloadLibraryDuringInstantiation() throws Exception {
        IFmuCallback unloading = new IFmuCallback() {
            @Override
            public void log(String instanceName, Fmi2Status status, String category, String message) {
                fmu.nUnLoad(fmu.getFmuPtr());
            }

            @Override
            public void stepFinished(Fmi2Status status) {
            }
        };
        assertThrows(Exception.class,
                () -> fmu.instantiate(TestUtils.guid, "unload", false, true, unloading));
        assertEquals("2.0", fmu.getVersion());
    }

    @Test
    public void freedInstancesRejectFurtherCalls() throws Exception {
        IFmiComponent component = fmu.instantiate(TestUtils.guid, "freed", false, false, null);
        component.freeInstance();
        assertFalse(component.isValid());
        assertThrows(FmiInvalidNativeStateException.class, component::freeInstance);
        assertThrows(FmiInvalidNativeStateException.class, component::terminate);
        assertThrows(FmiInvalidNativeStateException.class, component::getState);
    }

    @Test
    public void unloadFreesAndInvalidatesLiveInstances() throws Exception {
        IFmiComponent component = fmu.instantiate(TestUtils.guid, "live", false, false, null);
        fmu.unLoad();
        assertFalse(fmu.isValid());
        assertFalse(component.isValid());
        assertThrows(FmiInvalidNativeStateException.class, component::freeInstance);
        assertThrows(FmiInvalidNativeStateException.class, fmu::getVersion);
        assertThrows(FmiInvalidNativeStateException.class,
                () -> fmu.instantiate(TestUtils.guid, "unloaded", false, false, null));
    }

    @Test
    public void nullOrEmptyNamesAndGuidsAreRejected() {
        for (String invalid : new String[]{null, ""}) {
            assertThrows(IllegalArgumentException.class,
                    () -> fmu.instantiate(TestUtils.guid, invalid, false, false, null));
            assertThrows(IllegalArgumentException.class,
                    () -> fmu.instantiate(invalid, "name", false, false, null));
        }
    }

    @Test
    public void nativeLifecycleRejectsNullAndUnknownHandles() {
        for (long invalid : new long[]{0, 1}) {
            assertThrows(Exception.class,
                    () -> fmu.ninstantiate(invalid, "name", TestUtils.guid, "", false, false, null));
            assertThrows(Exception.class, () -> fmu.nUnLoad(invalid));
            assertThrows(Exception.class, () -> new NativeFmuComponent().nFreeInstance(invalid, invalid));
            assertThrows(Exception.class, () -> new NativeFmuComponent().nTerminate(invalid, invalid));
        }
        assertThrows(Exception.class,
                () -> fmu.ninstantiate(fmu.getFmuPtr(), null, TestUtils.guid, "", false, false, null));
        assertThrows(Exception.class,
                () -> fmu.ninstantiate(fmu.getFmuPtr(), "name", null, "", false, false, null));
        assertThrows(Exception.class,
                () -> fmu.ninstantiate(fmu.getFmuPtr(), "name", TestUtils.guid, null, false, false, null));
    }

    private IFmuCallback callback(AtomicInteger calls) {
        return new IFmuCallback() {
            @Override
            public void log(String instanceName, Fmi2Status status, String category, String message) {
                calls.incrementAndGet();
            }

            @Override
            public void stepFinished(Fmi2Status status) {
            }
        };
    }
}
