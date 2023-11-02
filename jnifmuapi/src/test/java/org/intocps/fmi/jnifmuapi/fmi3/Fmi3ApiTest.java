package org.intocps.fmi.jnifmuapi.fmi3;

import org.intocps.fmi.FmiInvalidNativeStateException;
import org.intocps.fmi.FmuInvocationException;
import org.intocps.fmi.FmuMissingLibraryException;
import org.junit.*;
import org.junit.rules.TestName;
import org.mockito.Mockito;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.function.Consumer;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class Fmi3ApiTest {

    public static final String FMI_VERSION = "3.0";

    @Test
    public void testPlatformVersion() {
        Assert.assertEquals(FMI_VERSION, DirectoryFmi3Fmu.getJniApiFmiVersion());
    }

    public class ConsoleColors {
        // Reset
        public static final String RESET = "\033[0m";  // Text Reset

        // Regular Colors
        public static final String BLACK = "\033[0;30m";   // BLACK
        public static final String RED = "\033[0;31m";     // RED
        public static final String GREEN = "\033[0;32m";   // GREEN
        public static final String YELLOW = "\033[0;33m";  // YELLOW
        public static final String BLUE = "\033[0;34m";    // BLUE
        public static final String PURPLE = "\033[0;35m";  // PURPLE
        public static final String CYAN = "\033[0;36m";    // CYAN
        public static final String WHITE = "\033[0;37m";   // WHITE

        // Bold
        public static final String BLACK_BOLD = "\033[1;30m";  // BLACK
        public static final String RED_BOLD = "\033[1;31m";    // RED
        public static final String GREEN_BOLD = "\033[1;32m";  // GREEN
        public static final String YELLOW_BOLD = "\033[1;33m"; // YELLOW
        public static final String BLUE_BOLD = "\033[1;34m";   // BLUE
        public static final String PURPLE_BOLD = "\033[1;35m"; // PURPLE
        public static final String CYAN_BOLD = "\033[1;36m";   // CYAN
        public static final String WHITE_BOLD = "\033[1;37m";  // WHITE

        // Underline
        public static final String BLACK_UNDERLINED = "\033[4;30m";  // BLACK
        public static final String RED_UNDERLINED = "\033[4;31m";    // RED
        public static final String GREEN_UNDERLINED = "\033[4;32m";  // GREEN
        public static final String YELLOW_UNDERLINED = "\033[4;33m"; // YELLOW
        public static final String BLUE_UNDERLINED = "\033[4;34m";   // BLUE
        public static final String PURPLE_UNDERLINED = "\033[4;35m"; // PURPLE
        public static final String CYAN_UNDERLINED = "\033[4;36m";   // CYAN
        public static final String WHITE_UNDERLINED = "\033[4;37m";  // WHITE

        // Background
        public static final String BLACK_BACKGROUND = "\033[40m";  // BLACK
        public static final String RED_BACKGROUND = "\033[41m";    // RED
        public static final String GREEN_BACKGROUND = "\033[42m";  // GREEN
        public static final String YELLOW_BACKGROUND = "\033[43m"; // YELLOW
        public static final String BLUE_BACKGROUND = "\033[44m";   // BLUE
        public static final String PURPLE_BACKGROUND = "\033[45m"; // PURPLE
        public static final String CYAN_BACKGROUND = "\033[46m";   // CYAN
        public static final String WHITE_BACKGROUND = "\033[47m";  // WHITE

        // High Intensity
        public static final String BLACK_BRIGHT = "\033[0;90m";  // BLACK
        public static final String RED_BRIGHT = "\033[0;91m";    // RED
        public static final String GREEN_BRIGHT = "\033[0;92m";  // GREEN
        public static final String YELLOW_BRIGHT = "\033[0;93m"; // YELLOW
        public static final String BLUE_BRIGHT = "\033[0;94m";   // BLUE
        public static final String PURPLE_BRIGHT = "\033[0;95m"; // PURPLE
        public static final String CYAN_BRIGHT = "\033[0;96m";   // CYAN
        public static final String WHITE_BRIGHT = "\033[0;97m";  // WHITE

        // Bold High Intensity
        public static final String BLACK_BOLD_BRIGHT = "\033[1;90m"; // BLACK
        public static final String RED_BOLD_BRIGHT = "\033[1;91m";   // RED
        public static final String GREEN_BOLD_BRIGHT = "\033[1;92m"; // GREEN
        public static final String YELLOW_BOLD_BRIGHT = "\033[1;93m";// YELLOW
        public static final String BLUE_BOLD_BRIGHT = "\033[1;94m";  // BLUE
        public static final String PURPLE_BOLD_BRIGHT = "\033[1;95m";// PURPLE
        public static final String CYAN_BOLD_BRIGHT = "\033[1;96m";  // CYAN
        public static final String WHITE_BOLD_BRIGHT = "\033[1;97m"; // WHITE

        // High Intensity backgrounds
        public static final String BLACK_BACKGROUND_BRIGHT = "\033[0;100m";// BLACK
        public static final String RED_BACKGROUND_BRIGHT = "\033[0;101m";// RED
        public static final String GREEN_BACKGROUND_BRIGHT = "\033[0;102m";// GREEN
        public static final String YELLOW_BACKGROUND_BRIGHT = "\033[0;103m";// YELLOW
        public static final String BLUE_BACKGROUND_BRIGHT = "\033[0;104m";// BLUE
        public static final String PURPLE_BACKGROUND_BRIGHT = "\033[0;105m"; // PURPLE
        public static final String CYAN_BACKGROUND_BRIGHT = "\033[0;106m";  // CYAN
        public static final String WHITE_BACKGROUND_BRIGHT = "\033[0;107m";   // WHITE
    }

    static final String FMU_UNPACKED_PATH = "target/fmus/fmi3/fmi3functiontest".replace('/', File.separatorChar);
    static final String VALID_TOKEN = "{FMI3_TEST_TOKEN}";

    IFmi3Fmu fmu = null;
    IFmi3Instance instance = null;
    List<Object[]> logData = new Vector<>();

    @Before
    public void before() throws FmuInvocationException, FmuMissingLibraryException {
        System.out.println(ConsoleColors.GREEN + "Running ==> " + testName.getMethodName() + ConsoleColors.RESET);
        fmu = new DirectoryFmi3Fmu(new File(FMU_UNPACKED_PATH), "fmi3functiontest");
        fmu.load();
        Assert.assertEquals(FMI_VERSION, fmu.getVersion());
        ILogMessageCallback lm = (instanceName, status, category, message) -> {

            logData.add(new Object[]{instanceName, status, category, message});
            System.out.printf("Received log message:\n" + "instanceName: '%s'\n" + "Status: '%s'\n" + "Category: '%s'\n" + "Message: '%s'%n",
                    instanceName, status.name(), category, message);
        };

        DirectoryFmi3Fmu aSpy = (DirectoryFmi3Fmu) Mockito.spy(fmu);
        Mockito.when(aSpy.getResourceLocation()).thenReturn("/resources");
        fmu = aSpy;

        instance = fmu.instantiateCoSimulation("fmi3functiontest", VALID_TOKEN, true, true, true, true, new long[]{1, 2}, lm, null);

        Assert.assertNotNull("Instantiate returned null", instance);
        Assert.assertTrue(instance.isValid());
        Object[] msg = logData.get(logData.size() - 1);
        Assert.assertEquals("fmi3functiontest", msg[0]);
        Assert.assertEquals(Fmi3Status.OK, msg[1]);
        Assert.assertEquals("som category", msg[2]);
        Assert.assertEquals("some message", msg[3]);
    }

    @After
    public void after() throws FmiInvalidNativeStateException {
        if (instance != null) {
            instance.freeInstance();
        }
        if (fmu != null) {
            fmu.unLoad();
        }
        System.out.println("Stopping ==> " + testName.getMethodName());
    }

    @Rule
    public TestName testName = new TestName();

    @Test
    public void instantiateModelExchangeNoCallback() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        boolean validVisible = true;
        boolean validLoginOn = true;

        IFmi3Instance validInstance = fmu.instantiateModelExchange(validName, VALID_TOKEN, validVisible, validLoginOn, null);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        validInstance.freeInstance();

        Assert.assertNull("Name not send correctly", fmu.instantiateModelExchange("not as expected", VALID_TOKEN, validVisible, validLoginOn, null));
        Assert.assertNull("Token not send correctly", fmu.instantiateModelExchange(validName, "123", validVisible, validLoginOn, null));
        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("123");
        Assert.assertNull("ResourcePath not send correctly", fmu.instantiateModelExchange(validName, VALID_TOKEN, validVisible, validLoginOn, null));
        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("/tmp");
        Assert.assertNull("Visible not send correctly", fmu.instantiateModelExchange(validName, VALID_TOKEN, false, validLoginOn, null));
        Assert.assertNull("LoggingOn not send correctly", fmu.instantiateModelExchange(validName, VALID_TOKEN, validVisible, false, null));
        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn(null);
        Assert.assertNull("All not send correctly", fmu.instantiateModelExchange(null, null, false, false, null));
    }

    @Test
    public void instantiateModelExchangeCallbackCheck() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
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
        IFmi3Instance validInstance = fmu.instantiateModelExchange(validName, VALID_TOKEN, validVisible, validLoginOn, callbacks);
        Assert.assertNotNull("Instantiation failed", validInstance);
        Assert.assertTrue(validInstance.isValid());
        Assert.assertFalse("log cb not working", callbacks.logs.isEmpty());
        validInstance.freeInstance();


    }


    @Test
    public void instantiateScheduledNoCallback() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
        boolean validVisible = true;
        boolean validLoginOn = true;

        IFmi3Instance validInstance = fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validVisible, validLoginOn, null, null, null, null);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        validInstance.freeInstance();


        Assert.assertNull("Name not send correctly",
                fmu.instantiateScheduledExecution("not as expected", VALID_TOKEN, validVisible, validLoginOn, null, null, null, null));
        Assert.assertNull("Token not send correctly",
                fmu.instantiateScheduledExecution(validName, "123", validVisible, validLoginOn, null, null, null, null));


        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("123");
        Assert.assertNull("ResourcePath not send correctly",
                fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validVisible, validLoginOn, null, null, null, null));
        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("/tmp");

        Assert.assertNull("Visible not send correctly",
                fmu.instantiateScheduledExecution(validName, VALID_TOKEN, false, validLoginOn, null, null, null, null));
        Assert.assertNull("LoggingOn not send correctly",
                fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validVisible, false, null, null, null, null));

        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("123");
        Assert.assertNull("All not send correctly", fmu.instantiateScheduledExecution(null, null, false, false, null, null, null, null));
    }

    @Test
    public void instantiateScheduledCallbackCheck() throws FmiInvalidNativeStateException {

        String validName = "fmi3functiontest";
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
        IFmi3Instance validInstance = fmu.instantiateScheduledExecution(validName, VALID_TOKEN, validVisible, validLoginOn, callbacks, callbacks,
                callbacks, callbacks);
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

        IFmi3Instance validInstance = fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, validLoginOn, validEventModeUsed,
                validEarlyReturnAllowed, validRequiredIntermediateVariables, null, null);
        Assert.assertNotNull("Parameters not send correctly", validInstance);
        Assert.assertTrue(validInstance.isValid());
        validInstance.freeInstance();

        Assert.assertNull("Name not send correctly",
                fmu.instantiateCoSimulation("not valid", VALID_TOKEN, validVisible, validLoginOn, validEventModeUsed, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("Token not send correctly",
                fmu.instantiateCoSimulation(validName, "123", validVisible, validLoginOn, validEventModeUsed, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));

        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("123");
        Assert.assertNull("ResourcePath not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, validLoginOn, validEventModeUsed, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn("/tmp");
        Assert.assertNull("Visible not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, false, validLoginOn, validEventModeUsed, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("LoggingOn not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, false, validEventModeUsed, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("EventModeUsed not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, false, false, validEarlyReturnAllowed,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("EarlyReturnAllowed not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, false, validEventModeUsed, false,
                        validRequiredIntermediateVariables, null, null));
        Assert.assertNull("RequiredIntermediateVariables not send correctly",
                fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, false, validEventModeUsed, validEarlyReturnAllowed,
                        new long[]{4, 5, 6}, null, null));


        Mockito.when(((DirectoryFmi3Fmu) fmu).getResourceLocation()).thenReturn(null);
        Assert.assertNull("All not send correctly",
                fmu.instantiateCoSimulation(null, null, validVisible, validLoginOn, validEventModeUsed, validEarlyReturnAllowed, null, null, null));


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

        IFmi3Instance validInstance = fmu.instantiateCoSimulation(validName, VALID_TOKEN, validVisible, validLoginOn, validEventModeUsed,
                validEarlyReturnAllowed, validRequiredIntermediateVariables, callbacks, callbacks);
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
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt8(VREFS,VREFS.length, uint8,uint8.length));
        FmuResult<byte[]> result = instance.getUInt8(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint8, result.result);
    }

    @Test
    public void testSetGetInt8() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt8(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt8(VREFS, VREFS.length,int8,int8.length));
        FmuResult<byte[]> result = instance.getInt8(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int8, result.result);
    }

    @Test
    public void testSetGetUInt16() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt16(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt16(VREFS,VREFS.length, uint16,uint16.length));
        FmuResult<short[]> result = instance.getUInt16(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint16, result.result);
    }

    @Test
    public void testSetGetInt16() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt16(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt16(VREFS,VREFS.length, int16,int16.length));
        FmuResult<short[]> result = instance.getInt16(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int16, result.result);
    }

    @Test
    public void testSetGetUInt32() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt32(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt32(VREFS,VREFS.length, uint32,uint32.length));
        FmuResult<int[]> result = instance.getUInt32(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint32, result.result);
    }

    @Test
    public void testSetGetInt32() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt32(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt32(VREFS,VREFS.length, int32,int32.length));
        FmuResult<int[]> result = instance.getInt32(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int32, result.result);
    }


    @Test
    public void testSetGetUInt64() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getUInt64(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setUInt64(VREFS,VREFS.length, uint64,uint64.length));
        FmuResult<long[]> result = instance.getUInt64(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(uint64, result.result);
    }

    @Test
    public void testSetGetInt64() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getInt64(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setInt64(VREFS,VREFS.length, int64,int64.length));
        FmuResult<long[]> result = instance.getInt64(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(int64, result.result);
    }

    @Test
    public void testSetGetBoolean() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getBoolean(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setBoolean(VREFS,VREFS.length, bool,bool.length));
        FmuResult<boolean[]> result = instance.getBoolean(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(bool, result.result);
    }

    @Test
    public void testSetGetClock() throws FmiInvalidNativeStateException {

        Assert.assertEquals(Fmi3Status.OK, instance.setClock(VREFS, new boolean[]{true, false, true}));

        FmuResult<boolean[]> result = instance.getClock(VREFS);
        Assert.assertEquals(Fmi3Status.OK, result.status);
        Assert.assertArrayEquals(result.result, new boolean[]{true, false, true});
    }

    @Test
    public void testSetGetString() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getString(VREFS_TOO_MANY).status);
        Assert.assertEquals(Fmi3Status.OK, instance.setString(VREFS,VREFS.length, strings,strings.length));
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

    @Test
    public void testSetDebugLogging() throws FmiInvalidNativeStateException {

        Assert.assertEquals(Fmi3Status.OK, instance.setDebugLogging(true, "A", "B"));
    }

    @Test
    public void testEnterEventMode() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.enterEventMode());
    }

    @Test
    public void testTerminate() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.terminate());
    }

    @Test
    public void testReset() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.reset());
    }

    @Test
    public void testGetNumberOfVariableDependencies() throws FmiInvalidNativeStateException {
        FmuResult<Long> res = instance.getNumberOfVariableDependencies(1);
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(5, res.result, 0.0);
    }


    @Test
    public void testGetVariableDependencies() throws FmiInvalidNativeStateException {

        Assert.assertEquals(Fmi3Status.Error, instance.getVariableDependencies(9, 0).status);

        FmuResult<IFmi3Instance.VariableDependency> res = instance.getVariableDependencies(1, 5);
        Assert.assertEquals(Fmi3Status.OK, res.status);

        Assert.assertEquals(5, res.result.dependencyKinds.length);
        for (int i = 0; i < 5; i++) {
            Assert.assertEquals(Fmi3DependencyKind.valueOf(i), res.result.dependencyKinds[i]);
            Assert.assertEquals(i, res.result.elementIndicesOfDependent[i]);
            Assert.assertEquals(i, res.result.independents[i]);
            Assert.assertEquals(i, res.result.elementIndicesOfIndependents[i]);
        }

    }


    @Test
    public void testState() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.setInt8(VREFS,VREFS.length, new byte[]{0, 0, 0},3));
        FmuResult<byte[]> baseInt = instance.getInt8(VREFS);
        Assert.assertEquals(Fmi3Status.OK, baseInt.status);

        FmuResult<Fmi3State> resBase = instance.getState();
        Assert.assertEquals(Fmi3Status.OK, resBase.status);
        Fmi3State state1 = resBase.result;


        Consumer<Map.Entry<Fmi3State, byte[]>> checker = pair -> {
            try {
                Assert.assertEquals(Fmi3Status.OK, instance.setState(pair.getKey()));
                FmuResult<byte[]> tmp = instance.getInt8(VREFS);
                Assert.assertEquals(Fmi3Status.OK, tmp.status);
                Assert.assertArrayEquals(pair.getValue(), tmp.result);
            } catch (FmiInvalidNativeStateException e) {
                Assert.fail(e.getMessage());
            }
        };
        //check our initial state can be set and is consistent
        checker.accept(new AbstractMap.SimpleEntry<>(state1, baseInt.result));

        //make state dirty and then override it
        Assert.assertEquals(Fmi3Status.OK, instance.setInt8(VREFS,VREFS.length, int8,int8.length));
        checker.accept(new AbstractMap.SimpleEntry<>(state1, baseInt.result));

        //make a new state
        Assert.assertEquals(Fmi3Status.OK, instance.setInt8(VREFS,VREFS.length, int8,int8.length));
        FmuResult<byte[]> tmp = instance.getInt8(VREFS);
        Assert.assertEquals(Fmi3Status.OK, tmp.status);
        Assert.assertArrayEquals(int8, tmp.result);
        FmuResult<Fmi3State> res = instance.getState();
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Fmi3State state2 = res.result;

        checker.accept(new AbstractMap.SimpleEntry<>(state2, int8));
        checker.accept(new AbstractMap.SimpleEntry<>(state1, baseInt.result));
        checker.accept(new AbstractMap.SimpleEntry<>(state2, int8));

        Assert.assertEquals(Fmi3Status.OK, res.result.free());
        Assert.assertEquals(Fmi3Status.OK, state1.free());

    }

    @Test
    @Ignore
    public void testSerializeState() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.setInt8(VREFS,VREFS.length, int8,int8.length));
        FmuResult<Fmi3State> res = instance.getState();
        Assert.assertEquals(Fmi3Status.OK, res.status);

        FmuResult<Long> sizeRes = instance.getSerializedStateSize(res.result);
        Assert.assertEquals(Fmi3Status.OK, sizeRes.status);

        FmuResult<byte[]> serializedStateRes = instance.serializedState(res.result, sizeRes.result);
        Assert.assertEquals(Fmi3Status.OK, serializedStateRes.status);

        Assert.assertEquals(Fmi3Status.OK, res.result.free());

        FmuResult<Fmi3State> deserializedState = instance.deSerializedState(serializedStateRes.result);
        Assert.assertEquals(Fmi3Status.OK, deserializedState.status);

        Assert.assertEquals(Fmi3Status.OK, deserializedState.result.free());

    }


    @Test
    public void testGetDirectionalDerivative() throws FmiInvalidNativeStateException {

        long[] unknowns = new long[2];
        long[] knows = new long[2];
        double[] seeds = new double[2];

        for (int i = 0; i < 2; i++) {
            unknowns[i] = i;
            knows[i] = i;
            seeds[i] = i;
        }

        FmuResult<double[]> res = instance.getDirectionalDerivative(unknowns, knows, seeds);
        Assert.assertEquals(Fmi3Status.OK, res.status);
        //        for (int i = 0; i < 2; i++) {
        //            Assert.assertEquals(i * 0.1, res.result[i], 0.0);
        //        }
    }

    @Test
    public void testGetAdjointDerivative() throws FmiInvalidNativeStateException {
        Assume.assumeTrue(true);
        FmuResult<double[]> res = instance.getGetAdjointDerivative(new long[0], new long[0], new double[0], 0);
        Assert.assertEquals(Fmi3Status.OK, res.status);
    }

    @Test
    public void testEnterConfigurationMode() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.enterConfigurationMode());
    }


    @Test
    public void testExitConfigurationMode() throws FmiInvalidNativeStateException {

        Assert.assertEquals(Fmi3Status.OK, instance.exitConfigurationMode());
    }

    @Test
    public void testGetIntervalDecimal() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getIntervalDecimal(new long[]{9L}).status);

        FmuResult<IFmi3Instance.GetIntervalDecimalResponse> res = instance.getIntervalDecimal(new long[]{1L, 2L});
        Assert.assertEquals(Fmi3Status.OK, res.status);

        Assert.assertEquals(2, res.result.intervals.length);
        Assert.assertEquals(2, res.result.qualifiers.length);

        for (int i = 0; i < 2; i++) {
            Assert.assertEquals(i * 0.1, res.result.intervals[i], 0.0);
            Assert.assertEquals(Fmi3IntervalQualifier.valueOf(i), res.result.qualifiers[i]);
        }
    }

    @Test
    public void testGetIntervalFraction() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getIntervalFraction(new long[]{9L}).status);

        FmuResult<IFmi3Instance.IntervalFractionResponse> res = instance.getIntervalFraction(new long[]{1L, 2L});
        Assert.assertEquals(Fmi3Status.OK, res.status);

        Assert.assertEquals(2, res.result.intervalCounters.length);
        Assert.assertEquals(2, res.result.resolutions.length);
        Assert.assertEquals(2, res.result.qualifiers.length);

        for (int i = 0; i < 2; i++) {
            Assert.assertEquals(i, res.result.intervalCounters[i]);
            Assert.assertEquals(i, res.result.resolutions[i]);
            Assert.assertEquals(Fmi3IntervalQualifier.valueOf(i), res.result.qualifiers[i]);
        }

    }

    @Test
    public void testGetShiftDecimal() throws FmiInvalidNativeStateException {

        Assert.assertEquals(Fmi3Status.Error, instance.getShiftDecimal(new long[]{9L}).status);

        FmuResult<double[]> res = instance.getShiftDecimal(new long[]{1L, 2L});
        Assert.assertEquals(Fmi3Status.OK, res.status);

        Assert.assertEquals(0 * 0.1, res.result[0], 0.0);
        Assert.assertEquals(1 * 0.1, res.result[1], 0.0);

    }

    @Test
    public void testGetShiftFraction() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.Error, instance.getShiftFraction(new long[]{9L}).status);

        FmuResult<IFmi3Instance.GetShiftFractionResponse> res = instance.getShiftFraction(new long[]{1L, 2L});
        Assert.assertEquals(Fmi3Status.OK, res.status);

        Assert.assertEquals(2, res.result.shiftCounters.length);
        Assert.assertEquals(2, res.result.resolutions.length);

        for (int i = 0; i < 2; i++) {
            Assert.assertEquals(i, res.result.shiftCounters[i]);
            Assert.assertEquals(i, res.result.resolutions[i]);
        }

    }

    @Test
    public void testSetIntervalDecimal() throws FmiInvalidNativeStateException {

        Assert.assertEquals(Fmi3Status.OK, instance.setIntervalDecimal(new long[]{1L, 2L}, new double[]{1.1, 2.2}));

    }

    @Test
    public void testSetIntervalFraction() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.setIntervalFraction(new long[]{1L, 2L}, new long[]{1L, 2L}, new long[]{3L, 4L}));

    }


    @Test
    public void testSetShiftDecimal() {
        Assert.assertEquals(Fmi3Status.OK, instance.setShiftDecimal(new long[]{1L, 2L}, new double[]{1.1, 2.2}));
    }

    @Test
    public void testSetShiftFraction() {
        Assert.assertEquals(Fmi3Status.OK, instance.setShiftFraction(new long[]{1L, 2L}, new long[]{1L, 2L}, new long[]{3L, 4L}));
    }

    @Test
    public void testEvaluateDiscreteStates() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.evaluateDiscreteStates());

    }

    @Test
    public void testUpdateDiscreteStates() throws FmiInvalidNativeStateException {

        FmuResult<IFmi3Instance.UpdateDiscreteStates> res = instance.updateDiscreteStates();
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99.99, res.result.nextEventTime, 0.0);

    }

    @Test
    public void testEnterContinuousTimeMode() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.enterContinuousTimeMode());
    }

    @Test
    public void testCompletedIntegratorStep() throws FmiInvalidNativeStateException {
        FmuResult<IFmi3Instance.CompletedIntegratorStepResponse> res = instance.completedIntegratorStep(false);
        Assert.assertEquals(Fmi3Status.OK, res.status);

    }

    @Test
    public void testSetTime() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.setTime(99.99));

    }

    @Test
    public void testSetContinuousStates() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.setContinuousStates(new double[]{1.1, 2.2}));

    }

    @Test
    public void testGetContinuousStateDerivatives() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.getContinuousStateDerivatives(3).status);
    }

    @Test
    public void testGetEventIndicators() throws FmiInvalidNativeStateException {
        FmuResult<double[]> res = instance.getGetEventIndicators(1);
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99.99, res.result[0], 0.0);

    }

    @Test
    public void testGetContinuousStates() throws FmiInvalidNativeStateException {
        FmuResult<double[]> res = instance.getGetContinuousStates(1);
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99.99, res.result[0], 0.0);
    }

    @Test
    public void testGetNominalsOfContinuousStates() throws FmiInvalidNativeStateException {
        FmuResult<double[]> res = instance.getGetNominalsOfContinuousStates(1);
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99.99, res.result[0], 0.0);

    }

    @Test
    public void testGetNumberOfEventIndicators() throws FmiInvalidNativeStateException {
        FmuResult<Long> res = instance.getNumberOfEventIndicators();
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99L, (long) res.result);

    }

    @Test
    public void testGetNumberOfContinuousStates() throws FmiInvalidNativeStateException {
        FmuResult<Long> res = instance.getNumberOfContinuousStates();
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99L, (long) res.result);

    }

    @Test
    public void testEnterStepMode() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.enterStepMode());
    }


    @Test
    public void testGetOutputDerivatives() throws FmiInvalidNativeStateException {
        FmuResult<double[]> res = instance.getOutputDerivatives(new long[]{1L, 2L}, new int[]{0, 1, 2});
        Assert.assertEquals(Fmi3Status.OK, res.status);
        Assert.assertEquals(99.99, res.result[0], 0.0);
    }

    @Test
    public void testDoStep() throws FmiInvalidNativeStateException {
        FmuResult<IFmi3Instance.DoStepResult> res = instance.doStep(0.0, 1.1, false);
        Assert.assertEquals(Fmi3Status.OK, res.status);
    }

    @Test
    public void testActivateModelPartition() throws FmiInvalidNativeStateException {
        Assert.assertEquals(Fmi3Status.OK, instance.activateModelPartition(1L, 99.99));

    }


}
