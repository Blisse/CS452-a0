#pragma once

#define SUCCESS     0
#define E_FAIL      1
#define E_NOCHANGE  2

static inline int SUCCEEDED(int status) {
    return (status == SUCCESS);
}

static inline int FAILED(int status) {
    return (status != SUCCESS);
}
