/*
 * Copyright 2018 aquenos GmbH.
 * Copyright 2018 Karlsruhe Institute of Technology.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * This software has been developed by aquenos GmbH on behalf of the
 * Karlsruhe Institute of Technology's Institute for Beam Physics and
 * Technology.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/errno.h>
#include <sys/file.h>

#include <epicsExport.h>
#include <iocsh.h>

// Data structures needed for the iocsh iocLockFile function.
static const iocshArg iocshIocLockFileArg0 = {"file path", iocshArgString };
static const iocshArg * const iocshIocLockFileArgs[] = {&iocshIocLockFileArg0 };
static const iocshFuncDef iocshIocLockFileFuncDef = {
    "iocLockFile", 1, iocshIocLockFileArgs };


/**
 * Implementation of the iocsh iocLockFile function.
 *
 * This function opens the specified, creating it if it does not exist. It then
 * tries to acquire a lock on that file. If it can acquire the lock, it returns.
 * If it cannot, it print an error messages and calls exit(...) with a status
 * code of 1.
 */
static void iocshIocLockFileFunc(const iocshArgBuf *args) {
  char *lockFileName = args[0].sval;
  if (!lockFileName || !strlen(lockFileName)) {
    fprintf(stderr, "Error: The lock file name must be set.\n");
    exit(1);
  }
  int lockFile = open(
    lockFileName,
    O_APPEND | O_CLOEXEC | O_CREAT,
    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (lockFile == -1) {
    fprintf(stderr,
            "Error: Could not open the file \"%s\": %s\n",
            lockFileName,
            strerror(errno));
    exit(1);
  }
  if (flock(lockFile, LOCK_EX | LOCK_NB) == -1) {
    if (errno == EWOULDBLOCK) {
      fprintf(stderr,
              "Error: Another process already holds a lock on the file \"%s\". Most likely, this IOC is already running.\n",
              lockFileName);
    } else {
      fprintf(stderr,
              "Error: Could not acquire a lock on the file \"%s\": %s\n",
              lockFileName,
              strerror(errno));
    }
    exit(1);
  }
}

/**
 * Registrar function for this support. This is called by the IOC when loading
 * the .dbd file.
 */
static void iocLockRegistrar() {
  iocshRegister(&iocshIocLockFileFuncDef, iocshIocLockFileFunc);
}

epicsExportRegistrar(iocLockRegistrar);
