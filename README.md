# Java API for FMI

This is a Java API for the fmi standard which is defined as a collection of C headers and xml descriptions.

# Development Environment

* JDK 21
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

* JDK 21
* Maven 3.6 >
* https://www.msys2.org/
    * With package:
        * `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`
        * `pacman -S mingw64/mingw-w64-x86_64-cmake`
    * Add Java and Maven to `~/.bash_profile`, using your installation paths:
        * `export JAVA_HOME="/c/path/to/jdk-21"`
        * `export PATH="$JAVA_HOME/bin:/c/path/to/apache-maven/bin:$PATH"`

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

Both snapshot and release workflows build and test on these GitHub-hosted runners:

| Target | Runner label | JVM architecture |
| --- | --- | --- |
| Linux x86_64 | `ubuntu-24.04` | `x64` |
| Windows x86_64 | `windows-2022` | `x64` |
| macOS Intel | `macos-15-intel` | `x64` |
| macOS Apple Silicon | `macos-15` | `aarch64` |

All Maven jobs use JDK 21. The Maven compiler still targets Java 8 source and bytecode;
older JVMs are not currently tested by CI.

These are native builds, not Linux-hosted cross-compilation. Both macOS runners
build universal libraries and run the tests under their native JVM architecture.
Only the Apple Silicon job uploads the macOS universal libraries, avoiding collisions
when merging artifacts. The final job packages the Linux, Windows, and universal macOS
libraries into one JAR without rebuilding them and verifies all expected resource paths.
The snapshot workflow also assembles combined multi-platform test FMUs.
OS-specific steps use `runner.os`, independently of the selected runner image label.
See the [GitHub-hosted runner reference](https://docs.github.com/en/actions/reference/runners/github-hosted-runners)
for current label availability.

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