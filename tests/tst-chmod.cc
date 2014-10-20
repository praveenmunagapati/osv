/*
 * Copyright (C) 2013 Cloudius Systems, Ltd.
 *
 * This work is open source software, licensed under the terms of the
 * BSD license as described in the LICENSE file in the top-level directory.
 */

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>

static int tests = 0, fails = 0;

#define expect(actual, expected) do_expect(actual, expected, #actual, #expected, __FILE__, __LINE__)
template<typename T>
bool do_expect(T actual, T expected, const char *actuals, const char *expecteds, const char *file, int line)
{
    ++tests;
    if (actual != expected) {
        fails++;
        std::cout << "FAIL: " << file << ":" << line << ": For " << actuals <<
                ", expected " << expecteds << ", saw " << actual << ".\n";
        return false;
    }
    return true;
}

#define expect_errno(call, experrno) ( \
        do_expect(call, -1, #call, "-1", __FILE__, __LINE__) && \
        do_expect(errno, experrno, #call " errno",  #experrno, __FILE__, __LINE__) )

int main(int argc, char **argv)
{
    remove("/tmp/f1");
    remove("/tmp/f2");
    expect(mknod("/tmp/f1", 0777|S_IFREG, 0), 0);
    expect(chmod("/tmp/f1", 0123), 0);
    struct stat st;
    expect(stat("/tmp/f1", &st), 0);
    expect((int)(st.st_mode & 0777), 0123);
    expect(chmod("/tmp/f1", 0234), 0);
    expect(stat("/tmp/f1", &st), 0);
    expect((int)(st.st_mode & 0777), 0234);

    expect_errno(chmod("/tmp/f2", 0123), ENOENT);

    remove("/tmp/f1");

    std::cout << "SUMMARY: " << tests << " tests, " << fails << " failures\n";
    return fails == 0 ? 0 : 1;
}
