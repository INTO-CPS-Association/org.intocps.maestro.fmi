# Java API for FMI

This is a Java API for the fmi standard which is defined as a collection of C headers and xml descriptions.

# Development Environment

* java 11
* Maven 3
* CMake 3.10 or newer (including CMake 4)
* A working C and C++ compiler

## Linux

* `apt install build-essential git cmake`

## Mac

* `brew install cmake`
* Install Xcode Command Line Tools: `xcode-select --install`

## Windows

The API can be build using the following tools:

* Java 1.8
* Maven 3.6 >
* https://www.msys2.org/
    * With package:
        * `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
        * `pacman -S mingw64/mingw-w64-x86_64-cmake`
    * Remember to add Java and Maven to `~/.bash_profile`
        * `PATH="$PATH:/c/Program Files/OpenJDK/jdk-8.0.262.10-hotspot/bin"`
        * `PATH="$PATH:/c/msys64/home/apache-maven-3.8.4/bin"`
        * `export PATH`

Build using `c:\msys64\mingw64.exe` terminal

## Checkout

```bash
git clone git@github.com:INTO-CPS-Association/org.intocps.maestro.fmi.git 
cd org.intocps.maestro.fmi
# The FMI headers and schemas are submodules
git submodules update --init
```

## Build

To build the project simply run maven `package` with the profile `make` to generate the native library for your
platform. If you dont have `JAVA_HOME` and the includes required there set correctly you can set the environment
variable `CI=1` (`export CI=1`) to use the embedded headers.

On Linux and Windows, the compiler targets its default host architecture; no architecture
argument is needed. On macOS, the default is a universal dylib containing both `x86_64`
and `arm64`, built using the Apple toolchain on either Intel or Apple Silicon.
The same universal JNI library is packaged under the Java resource paths
`lib/Mac-x86_64`, `lib/Mac-aarch64`, and `lib/Mac-arm64` to match `os.arch`.
Verify both slices with `lipo -archs jnifmuapi/target/classes/lib/Mac-aarch64/libfmuapi.dylib`.

FMI 2 macOS FMUs use `binaries/darwin64` on both Intel and Apple Silicon and must
contain a compatible library (a universal dylib supports both). FMI 3 uses
`binaries/x86_64-darwin` or `binaries/aarch64-darwin`, according to the JVM architecture,
as defined by the [FMI 3 platform tuples](https://fmi-standard.org/docs/3.0.2/#platform-tuple-definition).
The test FMUs built by the `make` profile are also universal on macOS, with their
libraries copied into the appropriate FMI 2 or FMI 3 directories.
An Intel-only FMU still requires an Intel JVM, for example under Rosetta on Apple Silicon.

```bash
mvn clean package -Pmake
...
[INFO] ------------------------------------------------------------------------
[INFO] BUILD SUCCESS
[INFO] ------------------------------------------------------------------------

```

### Build instructions

This will compile the library both Java and native for the current platform

```bash
mvn clean package -Pmake
# to enable debugging symbols
mvn clean package -Pmake -Pnative_debug
```

# CI integration and cross build

The project is build on github actions with a cross compilation step where the native code is compiled on ubuntu, mac
and windows and a final step packing these together into one java build make a single bundle that supports all
platforms.

# Usage

Add this dependency to your project (replace the `version` with the latest release)

```xml

<dependency>
    <groupId>org.into-cps.fmi</groupId>
    <artifactId>jnifmuapi</artifactId>
    <version>1.3.1</version>
</dependency>
```

The classes:

* `org.intocps.fmi.jnifmuapi` - fmi2 main factory
* `org.intocps.fmi.jnifmuapi.fmi3` - fmi3 main interface without the zip

# Release Procedure

Simply trigger the `Release new version` action.