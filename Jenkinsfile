def copyStashInPlace ='''#!/bin/bash
GITLOG=git-info.txt
BINROOT=jnifmuapi/src/main/native/build

LIB=jnifmuapi/target/classes/lib
mkdir -p $LIB

D=$LIB/Linux-amd64
mkdir -p $D
cp $BINROOT/linux64/*.so $D/
cp $GITLOG $D/

D=$LIB/Linux-i386
mkdir -p $D
cp $BINROOT/linux32/*.so $D/
cp $GITLOG $D/

D=$LIB/Mac-x86_64
mkdir -p $D
cp $BINROOT/darwin64/*.dylib $D/
cp $GITLOG $D/


D=$LIB/Windows-amd64
mkdir -p $D
cp $BINROOT/win64/*.dll $D/
cp $GITLOG $D/


D=$LIB/Windows-x86
mkdir -p $D
cp $BINROOT/win32/*.dll $D/
cp $GITLOG $D/
'''


stage('Compile native code X') {
			
	parallel  darwin: {
		node('Xcompile') {
			checkout scm
			sh 'git submodule update --init'
			sh "SHM_DEPENDENCIES_ROOT=/home/privatejenkins/shm-dependencies ./xcompile.sh -darwin"
			sh "./list-dependencies.sh"
			stash includes: 'jnifmuapi/src/main/native/build/**/*.dylib, git-info.txt', name: 'jni-fmi-native-api-darwin' 
		}
	},linux32: {
		node('Xcompile') {
			checkout scm
			sh 'git submodule update --init'
			sh "SHM_DEPENDENCIES_ROOT=/home/privatejenkins/shm-dependencies ./xcompile.sh -linux32"
			sh "./list-dependencies.sh"
			stash includes: 'jnifmuapi/src/main/native/build/**/*.so, git-info.txt', name: 'jni-fmi-native-api-linux32' 
		}
	},linux64: {
		node('Xcompile') {
			checkout scm
			sh 'git submodule update --init'
			sh "SHM_DEPENDENCIES_ROOT=/home/privatejenkins/shm-dependencies ./xcompile.sh -linux64"
			sh "./list-dependencies.sh"
			stash includes: 'jnifmuapi/src/main/native/build/**/*.so, git-info.txt', name: 'jni-fmi-native-api-linux64'  
		}
	},win32: {
		node('Xcompile') {
			checkout scm
			sh 'git submodule update --init'
			sh "SHM_DEPENDENCIES_ROOT=/home/privatejenkins/shm-dependencies ./xcompile.sh -win32"
			sh "./list-dependencies.sh"
			stash includes: 'jnifmuapi/src/main/native/build/**/*.dll, git-info.txt', name: 'jni-fmi-native-api-win32' 
		}
	},win64: {
		node('Xcompile') {
			checkout scm
			sh 'git submodule update --init'
			sh "SHM_DEPENDENCIES_ROOT=/home/privatejenkins/shm-dependencies ./xcompile.sh -win64"
			sh "./list-dependencies.sh"
			stash includes: 'jnifmuapi/src/main/native/build/**/*.dll, git-info.txt', name: 'jni-fmi-native-api-win64'
		}
	}
}

node {

	// Only keep one build
	properties([[$class: 'BuildDiscarderProperty', strategy: [$class: 'LogRotator', numToKeepStr: '5']]])

	stage('Compile Java library') {
		checkout scm

		//clear all
		withMaven(mavenLocalRepo: '.repository', mavenSettingsFilePath: "${env.MVN_SETTINGS_PATH}") {

			// Run the maven build
			sh "mvn clean"
				
		}
		sh "rm -rf jnifmuapi/src/main/native/build"

		//copy back all native libraries
		unstash 'jni-fmi-native-api-darwin' 
		unstash 'jni-fmi-native-api-linux32'
		unstash 'jni-fmi-native-api-linux64'
		unstash 'jni-fmi-native-api-win32'
		unstash 'jni-fmi-native-api-win64'
							
		sh copyStashInPlace

		//compile the java library
		withMaven(mavenLocalRepo: '.repository', mavenSettingsFilePath: "${env.MVN_SETTINGS_PATH}") {

			// Run the maven build
			sh "mvn install"
			step([$class: 'ArtifactArchiver', artifacts: '**/target/*.jar', fingerprint: true])
			step([$class: 'JUnitResultArchiver', testResults: '**/target/surefire-reports/TEST-*.xml'])
			step([$class: 'JacocoPublisher'])
			step([$class: 'TasksPublisher', canComputeNew: false, defaultEncoding: '', excludePattern: '', healthy: '', high: 'FIXME', ignoreCase: true, low: '', normal: 'TODO', pattern: '', unHealthy: ''])
		}

	}

	stage('Deploy') {
		if (env.BRANCH_NAME == 'development') {
			
			def server = Artifactory.server "-844406945@1404457436085"
			def buildInfo = Artifactory.newBuildInfo()
			buildInfo.env.capture = true
				
			def rtMaven = Artifactory.newMavenBuild()
			rtMaven.tool = "Maven 3.1.1" // Tool name from Jenkins configuration
			rtMaven.opts = "-Xmx1024m -XX:MaxPermSize=256M"
			rtMaven.deployer releaseRepo:'into-cps', snapshotRepo:'into-cps', server: server
				
			rtMaven.run pom: 'pom.xml', goals: 'install', buildInfo: buildInfo

			//get rid of old snapshots only keep then for a short amount of time
			buildInfo.retention maxBuilds: 5, maxDays: 7, deleteBuildArtifacts: true
		
			// Publish build info.
			server.publishBuildInfo buildInfo
		}
	}
		stage('Build COE') {
			build '../INTO-CPS-COE-pipe/development'
		}
}


