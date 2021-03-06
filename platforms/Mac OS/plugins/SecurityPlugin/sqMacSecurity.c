//JMM 2/13/01 create docs folder if non-existant
//JMM 4/4/01  look for documents/My Squeak folder versus just documents as secure location
//JMM 5/3/01  path was wrong for unsecure folder which uncovered a bug in lookupPath
//JMM 8/15/01 only allow call to ioInitSecurity Once, also return  proper return code
//JMM 9/5/01  make it as a plugin
// 3.7.0bx Nov 24th, 2003 JMM gCurrentVMEncoding

#include "sq.h"
#include "sqMacFileLogic.h"	
#include "SecurityPlugin.h"
#include "sqMacUIConstants.h"
#include "FilePlugin.h"

#include <Files.h> 

#if !defined(PATH_MAX)
# include <sys/syslimits.h>
#endif
extern struct VirtualMachine * interpreterProxy;

#define fromSqueak(string,length) string
void fixPath(char *path);
int dir_CreateSecurity(char *pathString, int pathStringLength);
int _ioSetFileAccess(int enable);

static char secureUserDirectory[PATH_MAX];
static char untrustedUserDirectory[PATH_MAX];
static Boolean gInitialized = false;

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* file security */
static int allowFileAccess = 1;  /* full access to files */

static int isAccessiblePathName(char *pathName) {
  unsigned int i;
  /* Check if the path/file name is subdirectory of the image path */
  for(i=0; i<strlen(untrustedUserDirectory)-1; i++)
    if(untrustedUserDirectory[i] != pathName[i]) return 0;
  /* special check for the trusted directory */
  if(pathName[i] == 0) return 1; /* allow access to trusted directory */
  /* check last character in image path (e.g., backslash) */
  if(untrustedUserDirectory[i] != pathName[i]) return 0;

return 1;
}

static int isAccessibleFileName(char *fileName) {
  unsigned int i;
  /* Check if the path/file name is subdirectory of the image path */
  for(i=0; i<strlen(untrustedUserDirectory); i++)
    if(untrustedUserDirectory[i] != fileName[i]) return 0;
  return 1;
}

/* directory access */
int ioCanCreatePathOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathStringLength)
  if(allowFileAccess) return 1;
  return isAccessiblePathName(fromSqueak(pathString, pathStringLength));
}

int ioCanListPathOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathStringLength)
  if(allowFileAccess) return 1;
  return isAccessiblePathName(fromSqueak(pathString, pathStringLength));
}

int ioCanDeletePathOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathStringLength)
  if(allowFileAccess) return 1;
  return isAccessiblePathName(fromSqueak(pathString, pathStringLength));
}

/* file access */
int ioCanOpenFileOfSizeWritable(char* pathString, int pathStringLength, int writeFlag) {
#pragma unused(pathStringLength,writeFlag)
  if(allowFileAccess) return 1;
  return isAccessibleFileName(fromSqueak(pathString, pathStringLength));
}

int ioCanOpenAsyncFileOfSizeWritable(char* pathString, int pathStringLength, int writeFlag) {
  return ioCanOpenFileOfSizeWritable(pathString,pathStringLength,writeFlag);
}
int ioCanDeleteFileOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathStringLength)
  if(allowFileAccess) return 1;
  return isAccessibleFileName(fromSqueak(pathString, pathStringLength));
}

int ioCanRenameFileOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathStringLength)
  if(allowFileAccess) return 1;
  return isAccessibleFileName(fromSqueak(pathString, pathStringLength));
}


int ioCanGetFileTypeOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathString,pathStringLength)
  return 1; /* of no importance here */
}

int ioCanSetFileTypeOfSize(char* pathString, int pathStringLength) {
#pragma unused(pathString,pathStringLength)
  return 1; /* of no importance here */
}

/* disabling/querying */
int ioDisableFileAccess(void) {
  allowFileAccess = 0;
 return 0;
}

int ioHasFileAccess(void) {
  return allowFileAccess;
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* image security */

static int allowImageWrite = 1;  /* allow writing the image */

int ioCanRenameImage(void) {
  return allowImageWrite; /* only when we're allowed to save the image */
}

int ioCanWriteImage() {
  return allowImageWrite;
}

int ioDisableImageWrite() {
  allowImageWrite = 0;
	return 0;
}


/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* socket security - for now it's all or nothing */
static int allowSocketAccess = 1; /* allow access to sockets */

int ioCanCreateSocketOfType(int netType, int socketType) {
#pragma unused(netType,socketType)
  return allowSocketAccess;
}

int ioCanConnectToPort(int netAddr, int port) {
#pragma unused(netAddr,port)
  return allowSocketAccess;
}

int ioCanListenOnPort(int  s, int port) {
#pragma unused(s,port)
  return allowSocketAccess;
}

int ioDisableSocketAccess() {
  allowSocketAccess = 0;
	return 0;
}

int ioHasSocketAccess() {
  return allowSocketAccess;
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* SecurityPlugin primitive support */


char *ioGetSecureUserDirectory(void) {
  return  secureUserDirectory;
}

char *ioGetUntrustedUserDirectory(void) {
  return untrustedUserDirectory;
}

/* note: following is called from VM directly, not from plugin */
int ioInitSecurity(void) {
  extern char gSqueakUntrustedDirectoryName[],gSqueakTrustedDirectoryName[];
  
  if (gInitialized) return 1;
  gInitialized  = true;

  secureUserDirectory[0] = 0x00;
  strcpy(secureUserDirectory, gSqueakTrustedDirectoryName);
  untrustedUserDirectory[0] = 0x00;
  strcpy(untrustedUserDirectory, gSqueakUntrustedDirectoryName);
  return 1;
}

/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/***************************************************************************/
/* private entries for restoring rights */
int _ioSetImageWrite(int enable);

int _ioSetImageWrite(int enable) {
  if(enable == allowImageWrite) return 1;
  allowImageWrite = enable;
  return 1;
}

int _ioSetFileAccess(int enable);

int _ioSetFileAccess(int enable) {
  if(enable == allowFileAccess) return 1;
  allowFileAccess = enable;
  return 1;
}

int _ioSetSocketAccess(int enable);

int _ioSetSocketAccess(int enable) {
  if(enable == allowSocketAccess) return 1;
  allowSocketAccess = enable;
  return 1;
}


void fixPath(char *path) {
    long i;
    for(i=strlen(path);i>0;i--) 
        if(path[i-1]==DELIMITERInt) {
            path[i-1]=0x00;
            return;
        }
}

int dir_CreateSecurity(char *pathString, int pathStringLength) {
	/* Create a new directory with the given path. By default, this
	   directory is created in the current directory. Use
	   a full path name such as "MyDisk:Working:New Folder" to
	   create folders elsewhere. */

    //JMM tests create file in Vm directory, other place, other volume
	return dir_Create(pathString, pathStringLength);
}
