package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.List;
import java.util.Vector;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class Fmi3ApiTest {
    @Test
    public void testPlatformVersion() {
        Assert.assertEquals("3.0-beta.3", DirectoryFmi3Fmu.getJniApiFmiVersion());
    }


    static final String FMU_UNPACKED_PATH = "target/fmus/fmi3/fmi3functiontest".replace('/', File.separatorChar);
    static final String VALID_TOKEN = "{FMI3_TEST_TOKEN}";

    IFmi3Fmu fmu = null;
    IFmi3Instance instance = null;
    List<Object[]> logData = new Vector<>();

    @Before
    public void before() throws FmuInvocationException, FmuMissingLibraryException {
        fmu = new DirectoryFmi3Fmu(new File(FMU_UNPACKED_PATH), "fmi3functiontest");
        fmu.load();
        ILogMessageCallback lm = (instanceName, status, category, message) -> {

            logData.add(new Object[]{instanceName, status, category, message});
            System.out.printf("Received log message:\n" + "instanceName: '%s'\n" + "Status: '%s'\n" + "Category: '%s'\n" + "Message: '%s'%n",
                    instanceName, status.name(), category, message);
        };
        instance = fmu.instantiateCoSimulation("fmi3functiontest", VALID_TOKEN, "/tmp", true, true, true, true, new long[]{1, 2}, lm, null);

        Assert.assertNotNull("Instantiate returned null", instance);
        Assert.assertTrue(instance.isValid());
    }

    @After
    public void after() throws FmiInvalidNativeStateException {
        if (instance != null) {
            instance.freeInstance();
        }
        if (fmu != null) {
            fmu.unLoad();
        }
    }

    @Test
    public void instantiateModelExchangeNoCallback() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        String validResourceLocation = "/tmp";
        boolean validVisible = true;
        boolean validLoginOn = true;

        IFmi3Instance validInstance = fmu.instantiateModelExchange(validName, VALID_TOKEN, validResourceLocation, validVisible, validLoginOn, null);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        validInstance.freeInstance();


        Assert.assertNull("Name not send correctly",
                fmu.instantiateModelExchange("not as expected", VALID_TOKEN, validResourceLocation, validVisible, validLoginOn, null));
        Assert.assertNull("Token not send correctly",
                fmu.instantiateModelExchange(validName, "123", validResourceLocation, validVisible, validLoginOn, null));
        Assert.assertNull("ResourcePath not send correctly",
                fmu.instantiateModelExchange(validName, VALID_TOKEN, "123", validVisible, validLoginOn, null));
        Assert.assertNull("Visible not send correctly",
                fmu.instantiateModelExchange(validName, VALID_TOKEN, validResourceLocation, false, validLoginOn, null));
        Assert.assertNull("LoggingOn not send correctly",
                fmu.instantiateModelExchange(validName, VALID_TOKEN, validResourceLocation, validVisible, false, null));
        Assert.assertNull("All not send correctly", fmu.instantiateModelExchange(null, null, null, false, false, null));
    }

    @Test
    public void instantiateModelExchangeCallbackCheck() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        String validResourceLocation = "/tmp";
        boolean validVisible = true;
        boolean validLoginOn = true;

        class CallbackChecker implements ILogMessageCallback {
            List<Object[]> logs = new Vector<>();

            @Override
            public void logMessage(String instanceName, Fmi3Status status, String category, String message) {
                logs.add(new Object[]{instanceName, status, category, message});
            }
        }


        CallbackChecker callbacks = new CallbackChecker();
        IFmi3Instance validInstance = fmu.instantiateModelExchange(validName, VALID_TOKEN, validResourceLocation, validVisible, validLoginOn,
                callbacks);
        Assert.assertNotNull("Instantiation failed", validInstance);
        Assert.assertTrue(validInstance.isValid());
        Assert.assertFalse("log cb not working", callbacks.logs.isEmpty());
        validInstance.freeInstance();


    }


    @Test
    public void instantiateScheduledNoCallback() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        String validResourceLocation = "/tmp";
        boolean validVisible = true;
        boolean validLoginOn = true;

        IFmi3Instance validInstance = fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validResourceLocation, validVisible, validLoginOn,
                null, null, null, null);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        validInstance.freeInstance();


        Assert.assertNull("Name not send correctly",
                fmu.instantiateScheduledExecution("not as expected", VALID_TOKEN, validResourceLocation, validVisible, validLoginOn, null, null, null,
                        null));
        Assert.assertNull("Token not send correctly",
                fmu.instantiateScheduledExecution(validName, "123", validResourceLocation, validVisible, validLoginOn, null, null, null, null));
        Assert.assertNull("ResourcePath not send correctly",
                fmu.instantiateScheduledExecution(validName, VALID_TOKEN, "123", validVisible, validLoginOn, null, null, null, null));
        Assert.assertNull("Visible not send correctly",
                fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validResourceLocation, false, validLoginOn, null, null, null, null));
        Assert.assertNull("LoggingOn not send correctly",
                fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validResourceLocation, validVisible, false, null, null, null, null));
        Assert.assertNull("All not send correctly", fmu.instantiateScheduledExecution(null, null, null, false, false, null, null, null, null));
    }

    @Test
    public void instantiateScheduledCallbackCheck() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        String validResourceLocation = "/tmp";
        boolean validVisible = true;
        boolean validLoginOn = true;

        class CallbackChecker implements ILogMessageCallback, IClockUpdateCallback, ILockPreemptionCallback, IUnlockPreemptionCallback {

            Boolean updated = null;
            Boolean locked = null;
            Boolean unlocked = null;
            List<Object[]> logs = new Vector<>();

            @Override
            public void updated() {
                updated = true;
            }

            @Override
            public void locked() {
                locked = true;
            }

            @Override
            public void logMessage(String instanceName, Fmi3Status status, String category, String message) {
                logs.add(new Object[]{instanceName, status, category, message});
            }

            @Override
            public void unlocked() {
                unlocked = true;
            }
        }


        CallbackChecker callbacks = new CallbackChecker();
        IFmi3Instance validInstance = fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validResourceLocation, validVisible, validLoginOn,
                callbacks, callbacks, callbacks, callbacks);
        Assert.assertNotNull("Instantiation failed", validInstance);
        Assert.assertTrue(validInstance.isValid());
        Assert.assertEquals("Locked cb not working", true, callbacks.locked);
        Assert.assertEquals("unLocked cb not working", true, callbacks.unlocked);
        Assert.assertEquals("Updated cb not working", true, callbacks.updated);
        Assert.assertFalse("log cb not working", callbacks.logs.isEmpty());
        validInstance.freeInstance();


    }

    @Test
    public void instantiateCoSimulationNoCallback() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        String validResourceLocation = "/tmp";
        boolean validVisible = true;
        boolean validLoginOn = true;
        boolean validEventModeUsed = true;
        boolean validEarlyReturnAllowed = true;
        long[] validRequiredIntermediateVariables = new long[]{1, 2};

        IFmi3Instance validInstance = fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, validVisible, validLoginOn,
                validEventModeUsed, validEarlyReturnAllowed, validRequiredIntermediateVariables, null, null);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        validInstance.freeInstance();

        Assert.assertNull("Name not send correctly",
                fmu.instantiateCoSimulation("not valid", VALID_TOKEN, validResourceLocation, validVisible, validLoginOn, validEventModeUsed,
                        validEarlyReturnAllowed, validRequiredIntermediateVariables, null, null));
        Assert.assertNull("Token not send correctly",
                fmu.instantiateCoSimulation(validName, "123", validResourceLocation, validVisible, validLoginOn, validEventModeUsed,
                        validEarlyReturnAllowed, validRequiredIntermediateVariables, null, null));
        Assert.assertNull("ResourcePath not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, "123", validVisible, validLoginOn, validEventModeUsed, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("Visible not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, false, validLoginOn, validEventModeUsed,
                        validEarlyReturnAllowed, validRequiredIntermediateVariables, null, null));
        Assert.assertNull("LoggingOn not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, validVisible, false, validEventModeUsed,
                        validEarlyReturnAllowed, validRequiredIntermediateVariables, null, null));
        Assert.assertNull("EventModeUsed not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, validVisible, false, false, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("EarlyReturnAllowed not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, validVisible, false, validEventModeUsed, false,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("RequiredIntermediateVariables not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, validVisible, false, validEventModeUsed,
                        validEarlyReturnAllowed, new long[]{4, 5, 6}, null, null));


        Assert.assertNull("All not send correctly",
                fmu.instantiateCoSimulation(null, null, null, validVisible, validLoginOn, validEventModeUsed, validEarlyReturnAllowed, null, null,
                        null));


    }

    @Test
    public void instantiateCoSimulationCallback() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        String validResourceLocation = "/tmp";
        boolean validVisible = true;
        boolean validLoginOn = true;
        boolean validEventModeUsed = true;
        boolean validEarlyReturnAllowed = true;
        long[] validRequiredIntermediateVariables = new long[]{1, 2};

        class CallbackChecker implements ILogMessageCallback, IIntermediateUpdateCallback {
            List<Object[]> logs = new Vector<>();

            @Override
            public void logMessage(String instanceName, Fmi3Status status, String category, String message) {
                logs.add(new Object[]{instanceName, status, category, message});
            }

            @Override
            public IntermediateUpdateResponse intermediateUpdate(long instanceEnvironment, double intermediateUpdateTime, boolean clocksTicked,
                    boolean intermediateVariableSetRequested, boolean intermediateVariableGetAllowed, boolean intermediateStepFinished,
                    boolean canReturnEarly) {
                return new IntermediateUpdateResponse(true, 555.555);
            }
        }
        CallbackChecker callbacks = new CallbackChecker();

        IFmi3Instance validInstance = fmu.instantiateCoSimulation(validName, VALID_TOKEN, validResourceLocation, validVisible, validLoginOn,
                validEventModeUsed, validEarlyReturnAllowed, validRequiredIntermediateVariables, callbacks, callbacks);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        Assert.assertFalse("log cb not working", callbacks.logs.isEmpty());
        validInstance.freeInstance();
    }

    static long[] VREFS = new long[]{0, 1, 2};
    static long[] VREFS_TOO_MANY = new long[]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    byte[] int8 = {Byte.MAX_VALUE, 0, Byte.MIN_VALUE};
    byte[] uint8 = {new BigInteger("FF", 16).byteValue(), 0, 0x0};//8 bit
    short[] int16 = {-32768, 0, 32767};
    short[] uint16 = {new BigInteger("FFFF", 16).shortValue(), 0, 0x0};//8 bit
    int[] int32 = {Integer.MAX_VALUE, 0, Integer.MIN_VALUE};//32
    int[] uint32 = {0xFFFFFFFF, 0, 0x0};
    long[] int64 = {Long.MAX_VALUE, 0, Long.MIN_VALUE};//32
    long[] uint64 = {new BigInteger("FFFFFFFFFFFFFFFF", 16).longValue(), 0, 0x0};
    boolean[] bool = {true, false, false};
    char[] chars = {0xFF, 0, 0};
    String[] strings = {IntStream.range(0, 1000).mapToObj(i -> "#").collect(Collectors.joining()), null, ""};
    //    @Test
    //    public void testSetGetFloat32() throws FmiInvalidNativeStateException {
    //        Assert.assertEquals(Fmi3Status.OK, instance.setFloat32(VREFS, new float[]{Float.MAX_VALUE, 0, 0, Float.MAX_VALUE}));
    //    }

    @Test
    public void testSetGetUInt8() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt8(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt8(VREFS, uint8));
        FmuResult<byte[]> result = instance.getUInt8(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint8, result.result);
    }

    @Test
    public void testSetGetInt8() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt8(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt8(VREFS, int8));
        FmuResult<byte[]> result = instance.getInt8(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int8, result.result);
    }

    @Test
    public void testSetGetUInt16() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt16(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt16(VREFS, uint16));
        FmuResult<short[]> result = instance.getUInt16(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint16, result.result);
    }

    @Test
    public void testSetGetInt16() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt16(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt16(VREFS, int16));
        FmuResult<short[]> result = instance.getInt16(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int16, result.result);
    }

    @Test
    public void testSetGetUInt32() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt32(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt32(VREFS, uint32));
        FmuResult<int[]> result = instance.getUInt32(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint32, result.result);
    }

    @Test
    public void testSetGetInt32() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt32(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt32(VREFS, int32));
        FmuResult<int[]> result = instance.getInt32(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int32, result.result);
    }


    @Test
    public void testSetGetUInt64() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt64(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt64(VREFS, uint64));
        FmuResult<long[]> result = instance.getUInt64(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint64, result.result);
    }

    @Test
    public void testSetGetInt64() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt64(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt64(VREFS, int64));
        FmuResult<long[]> result = instance.getInt64(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int64, result.result);
    }

    @Test
    public void testSetGetBoolean() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getBoolean(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setBoolean(VREFS, bool));
        FmuResult<boolean[]> result = instance.getBoolean(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(bool, result.result);
    }

    //    @Test
    //    public void testSetGetChar() throws FmiInvalidNativeStateException {
    //        Assert.assertEquals(Fmi3Status.Error, instance.getC(VREFS_TOO_MANY).status);
    //        Assert.assertEquals(Fmi3Status.OK, instance.setBoolean(VREFS, bool));
    //        FmuResult<boolean[]> result = instance.getBoolean(VREFS);
    //        Assert.assertEquals(Fmi3Status.OK, result.status);
    //        Assert.assertArrayEquals(bool, result.result);
    //    }


    //    @Test
    //        public void testSetGetClocks() throws FmiInvalidNativeStateException {
    //            Assert.assertEquals(Fmi3Status.Error, instance.getClass(VREFS_TOO_MANY).status);
    //            Assert.assertEquals(Fmi3Status.OK, instance.setBoolean(VREFS, bool));
    //            FmuResult<boolean[]> result = instance.getBoolean(VREFS);
    //            Assert.assertEquals(Fmi3Status.OK, result.status);
    //            Assert.assertArrayEquals(bool, result.result);
    //        }

    @Test
    public void testSetGetString() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getString(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setString(VREFS, strings));
        FmuResult<String[]> result = instance.getString(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        //the jniapi will convert null to "\0" so the FMUs never receive an invalid char pointer
        Assert.assertArrayEquals(Arrays.stream(strings).map(s -> s == null ? "" : s).toArray(String[]::new), result.result);
    }


    @Test
    public void testSetGetBinary() throws FmiInvalidNativeStateException, IOException {

        byte[][] binaryData = new byte[3][];

        String someText = "Hi, I would like to to share this message with the FMU instances...";
        binaryData[0] = someText.getBytes(StandardCharsets.UTF_8);
        binaryData[1] = null;

        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        try (ZipOutputStream zos = new ZipOutputStream(bos)) {
            zos.putNextEntry(new ZipEntry("MyMessage.txt"));
            zos.write(someText.getBytes(StandardCharsets.UTF_8));
        }
        bos.close();
        binaryData[2] = bos.toByteArray();

        Assert.assertEquals(Fmi3Status.Error, instance.getBinary(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setBinary(VREFS, binaryData));
        FmuResult<byte[][]> result = instance.getBinary(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(Arrays.stream(binaryData).map(a -> a == null ? new byte[0] : a).toArray(), result.result);
    }
}
