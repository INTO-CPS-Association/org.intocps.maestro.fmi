/*
 * This file is part of the INTO-CPS toolchain.
 *
 * Copyright (c) 2017-CurrentYear, INTO-CPS Association,
 * c/o Professor Peter Gorm Larsen, Department of Engineering
 * Finlandsgade 22, 8200 Aarhus N.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS INTO-CPS ASSOCIATION PUBLIC LICENSE VERSION 1.0.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL
 * VERSION 3, ACCORDING TO RECIPIENTS CHOICE.
 *
 * The INTO-CPS toolchain  and the INTO-CPS Association Public License
 * are obtained from the INTO-CPS Association, either from the above address,
 * from the URLs: http://www.into-cps.org, and in the INTO-CPS toolchain distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH IN THE
 * BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF
 * THE INTO-CPS ASSOCIATION.
 *
 * See the full INTO-CPS Association Public License conditions for more details.
 */

/*
 * Author:
 *		Kenneth Lausdahl
 */

package org.intocps.fmi;

public interface IFmiComponent {

    public abstract IFmu getFmu();

    public abstract Fmi2Status setDebugLogging(boolean loggingOn, String[] categories) throws FmuInvocationException;

    public abstract Fmi2Status setupExperiment(boolean toleranceDefined, double tolerance, double startTime, boolean stopTimeDefined,
            double stopTime) throws FmuInvocationException;

    public abstract Fmi2Status enterInitializationMode() throws FmuInvocationException;

    public abstract Fmi2Status exitInitializationMode() throws FmuInvocationException;

    public abstract Fmi2Status reset() throws FmuInvocationException;

    Fmi2Status setRealInputDerivatives(long[] scalarValueIndices, int[] order, double[] derivatives) throws FmuInvocationException;

    FmuResult<double[]> getRealOutputDerivatives(long[] scalarValueIndices, int[] order) throws FmuInvocationException;

    public FmuResult<double[]> getDirectionalDerivative(long[] vUnknownRef, long[] vKnownRef, double[] dvKnown) throws FmuInvocationException;

    /**
     * @param currentCommunicationPoint        Argument currentCommunicationPoint is the current communication point of the master (𝑡𝑐𝑖)
     * @param communicationStepSize            argument communicationStepSize is the communication step size (ℎ𝑐𝑖). The latter must be {@code >
     *                                         0.0.} The slave must integrate until time instant 𝑡𝑐𝑖+1 = 𝑡𝑐𝑖 + ℎ𝑐
     * @param noSetFMUStatePriorToCurrentPoint Argument noSetFMUStatePriorToCurrentPoint is fmi2True if fmi2SetFMUState will no longer be called for
     *                                         time instants prior to currentCommunicationPoint in this simulation run
     * @return <ul>
     * <li>fmi2OK - if the communication step was computed successfully until its end.</li>
     * <li>fmi2Discard – if the slave computed successfully only a subinterval of the communication step. The
     * master can call the appropriate fmi2GetXXXStatus functions to get further information. If possible, the
     * master should retry the simulation with a shorter communication step size. [Redoing a step is only
     * possible if the FMU state has been recorded at the beginning of the current (failed) step with
     * fmi2GetFMUState. Redoing a step is performed by calling fmi2SetFMUState and afterwards calling fmi2DoStep
     * with the new communicationStepSize. Note, it is not possible to change currentCommunicationPoint in such
     * a call.]</li>
     * <li>fmi2Error – the communication step could not be carried out at all. The master can try to repeat the
     * step with other input values and/or a different communication step size in the same way as described in
     * the fmi2Discard case above.</li>
     * <li>fmi2Fatal – if an error occurred which corrupted the FMU irreparably. [The master should stop the
     * simulation run immediatlely.] See section 2.1.3 for details.</li>
     * <li>fmi2Pending – is returned if the slave executes the function asynchronously. That means the slave
     * starts the computation but returns immediately. The master has to call fmi2GetStatus(...,fmi2DoStep,...)
     * to find out, if the slave is done. An alternative is to wait until the callback function fmi2StepFinished
     * is called by the slave. fmi2CancelStep can be called to cancel the current computation. It is not allowed
     * to call any other function during a pending fmi2DoStep</li>
     * </ul>
     * @throws FmuInvocationException ..
     */
    public abstract Fmi2Status doStep(double currentCommunicationPoint, double communicationStepSize,
            boolean noSetFMUStatePriorToCurrentPoint) throws FmuInvocationException;

    // public abstract Fmi2Status cancelStep();

    /**
     * Obtains the values of the selected scalar values
     *
     * @param scalarValueIndices svs
     * @return result
     */
    public abstract FmuResult<double[]> getReal(long[] scalarValueIndices) throws FmuInvocationException;

    public abstract FmuResult<int[]> getInteger(long[] scalarValueIndices) throws FmuInvocationException;

    /**
     * Obtains the values of the selected scalar values
     *
     * @param scalarValueIndices svs
     * @return result
     */
    public abstract FmuResult<boolean[]> getBooleans(long[] scalarValueIndices) throws FmuInvocationException;

    public abstract FmuResult<String[]> getStrings(long[] scalarValueIndices) throws FmuInvocationException;

    public abstract Fmi2Status setBooleans(long[] scalarValueIndices,
            boolean[] values) throws InvalidParameterException, FmiInvalidNativeStateException;

    public abstract Fmi2Status setReals(long[] scalarValueIndices, double[] values) throws InvalidParameterException, FmiInvalidNativeStateException;

    public abstract Fmi2Status setIntegers(long[] scalarValueIndices, int[] values) throws InvalidParameterException, FmiInvalidNativeStateException;

    public abstract Fmi2Status setStrings(long[] scalarValueIndices,
            String[] values) throws InvalidParameterException, FmiInvalidNativeStateException;

    public abstract FmuResult<Boolean> getBooleanStatus(Fmi2StatusKind kind) throws FmuInvocationException;

    public abstract FmuResult<Fmi2Status> getStatus(Fmi2StatusKind kind) throws FmuInvocationException;

    public abstract FmuResult<Integer> getIntegerStatus(Fmi2StatusKind kind) throws FmuInvocationException;

    public abstract FmuResult<Double> getRealStatus(Fmi2StatusKind kind) throws FmuInvocationException;

    public abstract FmuResult<String> getStringStatus(Fmi2StatusKind kind) throws FmuInvocationException;

    public abstract Fmi2Status terminate() throws FmuInvocationException;

    public abstract void freeInstance() throws FmuInvocationException;

    public abstract FmuResult<IFmiComponentState> getState() throws FmuInvocationException;

    public abstract Fmi2Status setState(IFmiComponentState state) throws FmuInvocationException;

    public abstract Fmi2Status freeState(IFmiComponentState state) throws FmuInvocationException;

    public abstract boolean isValid();


    //INTO-CPS extension

    /**
     * Extension method suggested in Broman et al
     *
     * @return result
     * @throws FmiInvalidNativeStateException in case of error
     */
    public abstract FmuResult<Double> getMaxStepSize() throws FmiInvalidNativeStateException;

}
