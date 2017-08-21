/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 *all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#include "argcv/c/sys/daemon.h"

// #include <assert.h>
#include <fcntl.h>     // open
#include <signal.h>    // signal
#include <sys/stat.h>  // open O_RDONL
#include <syslog.h>    // syslog
#include <unistd.h>    // setsid

int kDaemonStatus = 0;  // for err_XXX() functions
char kDaemonWorkDir[1024] = "/";
int kDaemonMaxFD = 64;

int daemon_init(const char* ident) {
  openlog(ident, LOG_PID, LOG_USER);
  // openlog(ident,LOG_CONS|LOG_PID,LOG_USER);
  syslog(LOG_INFO, "daemon starting");
  int i;
  pid_t pid;

  if ((pid = fork()) < 0) {
    return -1;
  } else if (pid) {
    _exit(0);  // parent terminates
  }

  // child 1 continues ...
  if (setsid() < 0) {  // become session leader
    return -1;
  }

  signal(SIGHUP, SIG_IGN);

  if ((pid = fork()) < 0) {
    return -1;
  } else if (pid) {
    _exit(0);
  }
  // child 2 continues ...
  kDaemonStatus = 1;  // for err_XXX() functions

  syslog(LOG_INFO, "chdir: %s\n", kDaemonWorkDir);

  int status_chdir = chdir(kDaemonWorkDir);
  // assert(status_chdir == 0);
  if (0 != status_chdir) {
    return status_chdir;
  }

  // close off file descriptors
  for (i = 0; i < kDaemonMaxFD; i++) {
    close(i);
  }

  open("/dev/null", O_RDONLY);
  open("/dev/null", O_RDWR);
  open("/dev/null", O_RDWR);

  syslog(LOG_INFO, "daemon started");
  return 0;
}

void daemon_destroy() {
  syslog(LOG_INFO, "daemon stopped");
  closelog();
  kDaemonStatus = 0;
}
