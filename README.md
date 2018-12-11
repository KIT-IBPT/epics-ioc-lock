IOC Lock Support
================

This is a very simple EPICS support module for ensuring that an IOC is not
started twice. For this purpose it introduces an new IOC shell command that
tries to acquire a lock on a file and fails if another process already holds
that lock.

Usage
-----

Add the dependency to this device support to `configure/RELEASE`. For example:

```
IOC_LOCK = /opt/epics/modules/iocLock
```

Add a reference to the support's `.dbd` file and library to the IOC's
`Makefile`. For example, in `xxxApp/src/Makefile`, add the following two lines:

```
xxx_DBD += iocLock.dbd
xxx_LIBS += iocLock
```

Finally, add the command for acquiring the lock file to `st.cmd`. For example:

```
iocLockFile("$(TOP)/iocBoot/$(IOC)/ioc.lock")
```

Copyright / License
-------------------

This EPICS support is licensed under the terms of the
[GNU Lesser General Public License version 3](LICENSE-LGPL.md). It has been
developed by [aquenos GmbH](https://www.aquenos.com/) on behalf of the
[Karlsruhe Institute of Technology's Institute of Beam Physics and Technology](https://www.ibpt.kit.edu/).
