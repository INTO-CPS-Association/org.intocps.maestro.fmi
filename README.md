# org.intocps.fmi
Java FMI support

## JNIFMUAPI project
root is referring to org.intocps.fmi/jnifmuapi/src/main/native/

### To build the JNIFMUAPI do the following:
To generate a shared library for the platform you are working on do the following:
1. Get submodules: FIRST CHECKOUT: `git submodule update --init --recursive`
   otherwise: `git submodule update --recursive`
2. Navigate to org.intocps.maestro.fmi/jnifmuapi/src/main/native 
3. run cmake .
4. run make
5. This generates libfmuapi.dylib (for mac), which can be placed within the lib/Mac-x86_64 directory of jnifmuapi-*.jar

*OR:* Push the branch, run it at build.overture.au and download the jar: http://overture.au.dk/artifactory/into-cps/org/into-cps/fmi/jnifmuapi/ 

### To Use this library do the following:

1. Download the jar of [`jnifmuapi`](https://mvnrepository.com/artifact/org.into-cps.fmi/jnifmuapi/1.0.2) or [snapshot](http://overture.au.dk/artifactory/into-cps/org/into-cps/fmi/jnifmuapi/)
1. Download [`fmi2.jar`](https://mvnrepository.com/artifact/org.into-cps.fmi/fmi2/1.0.2) or [snapshot](http://overture.au.dk/artifactory/into-cps/org/into-cps/fmi/fmi2/)
1. Add both previous jars to your project.
1. Extract the `lib` folder that is locate inside the jar obtained in step 1. Place that folder somewhere in your java library path, so that, for example, the entry `lib/Windows-amd64/fmuapi.dll` will be found in one of the folders in the java library path.
1. Test the link by running 
`org.intocps.fmi.jnifmuapi.Factory.checkApi()`

### To set up a dev environment using eclipse. 
If your system variables are not detected in eclipse, then open it via the
terminal. I.e. on MAC: `open /Applications/eclipse.app/contents/mac/eclipse` and **not**
`open /Applications/eclipse.app`. See https://superuser.com/questions/28344/path-env-variable-on-mac-os-x-and-or-eclipse
1. Perform submodule step from above
2. Download the plugin cmake4eclipse
3. Follow this to import a cmake project: https://stackoverflow.com/a/38716337/1308616
   (https://github.com/15knots/cmake4eclipse#quick-start might describe how to
   access more up-to-date instructions)
 4. It should now be possible to build.
 
To get proper IDE features it might be necessary to add includes for both C and
C++ at: project properties -> C/C++ General -> Preprocesser Include Paths,
Macros etc -> CDT User Settings Entires -> Add -> ... The following might need
to be added:
- root/include
- path to jdk include
i.e.`/Library/Java/JavaVirtualMachines/jdk1.8.0_152.jdk/Contents/Home/include`
check contains system headers
- specific jni_md for the platform, i.e.
`/Library/Java/JavaVirtualMachines/jdk1.8.0_152.jdk/Contents/Home/include/darwin`
check contains system headers.

## FMI2 project
This contains the JAVA types and interfaces representing the FMI standard.
Furthermore, the project contains XSD files from the FMI standard that are used for verifying model description files.

## Environment

* java 7 jdk
* gcc


The location of JAVA_HOME properties must be set for the desired platform which should be compiled.



The project is released to:

```XML
<distributionManagement>
    <repository>
        <id>overture.au.dk</id>
        <name>overture.au.dk-releases</name>
        <url>http://overture.au.dk/artifactory/into-cps</url>
    </repository>
    <snapshotRepository>
        <id>overture.au.dk</id>
        <name>overture.au.dk-snapshots</name>
        <url>http://overture.au.dk/artifactory/into-cps</url>
    </snapshotRepository>
</distributionManagement>
```

# Release Procedure

**Since the binaries are not compilled locally, you have to download the ones from last snapshot before the release** 

- 1. Download last `-SNAPSHOT` of [`jnifmuapi`](http://overture.au.dk/artifactory/into-cps/org/into-cps/fmi/jnifmuapi/) and extract the `lib` folder
- 2. Set the `fmi.native.lib` property as described below to point to the `lib` folder just extracted

Make sure you have the pre-build natives e.g. from last snapshot and that these are avaliable as:

```bash
-Dfmi.native.lib=/path/to/lib
```
or in your local maven settings file

```xml
<profiles>
  ...
  <profile>
    <properties>
      <fmi.native.lib>/path/to/lib</fmi.native.lib>
    </properties>
  </profile>
</profiles>
```


```bash
mvn -Dmaven.test.skip=true -Dmaven.repo.local=repository release:prepare -DreleaseVersion=0.0.4 -DdevelopmentVersion=0.0.5-SNAPSHOT -DpushChanges=false
```

if all is ok then push and release

```bash
git push --follow-tags
mvn -Dmaven.repo.local=repository release:perform
```

Afterwards, go to http://oss.sonatype.org -> staging repositories -> select the release.
Then click `close`, and once available, `release`.
