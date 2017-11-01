# org.intocps.fmi
Java FMI support


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

```bash
mvn -Dmaven.test.skip=true -Dmaven.repo.local=repository release:prepare -DreleaseVersion=0.0.4 -DdevelopmentVersion=0.0.5-SNAPSHOT -DpushChanges=false
```

if all is ok then push and release

```bash
git push --follow-tags
mvn -Dmaven.repo.local=repository release:perform
```

**Since the binaries are not compilled locally, you have to update artifactory manually after the release to include these**

- 1. Download the released `jnifmuapi-X.X.X.jar` from artifactory
- 2. Download last `-SNAPSHOT` extract the `lib` folder and add it to the released JAR
- 3. Upload the file from step 1 to artifactory to override the released jar
