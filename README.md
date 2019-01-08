# org.intocps.fmi
Java FMI support

## JNIFMUAPI project
### To build the JNIFMUAPI do the following:
1. Get submodules: FIRST CHECKOUT: `git submodule update --init --recursive`
   otherwise: `git submodule update --recursive`
2. Navigate to org.intocps.maestro.fmi/jnifmuapi/src/main/native 
3. run cmake .
4. run make
### To set up a dev environment use eclipse. 
1. Perform submodule step from above
2. Perform 2 from above
3. Create a makefile project, i.e.: cmake . -G"Eclipse CDT4 - Unix Makefiles"


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
