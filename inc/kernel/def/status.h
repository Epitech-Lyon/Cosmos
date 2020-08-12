/********************************************************************************
** 
**  This file is part of the Cosmos project, and is made available under
**  the terms of the GNU General Public License version 3.
**
**  Copyright (C) 2020 - Leo Karoubi
**
\******************************************************************************/

#ifndef _KERNEL_DEF_STATUS_H_
# define _KERNEL_DEF_STATUS_H_

/*
** Cosmos kernel status
*/
typedef enum status {
    OK = 0,

    ERR_PMM_OUT_OF_MEMORY,
    ERR_VMM_ALREADY_MAPPED,

    ERR_FATAL,
} status_t;

/*
** A bad result implementation.
*/
typedef uint64 any64_t;

typedef struct result
{
    status_t status;
    any64_t  result;
} __packed result_t;

/*
** Is the result an ERR
*/
# define RESULT_IS_ERR(x)       (x.status != OK)

/*
** Is the result an OK
*/
# define RESULT_IS_OK(x)        !(STATUS_IS_ERR(x))

/*
** Unwrap the result
*/
# define UNWRAP(res, type)           \
            (type)(res.result)

/*
** Propagate if ERR
*/
# define RAISE_IF(tgt, res, type)    \
            if (RESULT_IS_ERR(res))  \
                return (res)

/*
** Unwrap if OK or propagate if ERR
*/
# define UNWRAP_OR_RAISE(tgt, res, type)    \
            if (RESULT_IS_ERR(res))         \
                return (res);               \
            type tgt = UNWRAP(res, type);

/*
** Return OK as a result
*/
# define OK_PROPAGATION(res)            \
            return (                    \
                (result_t) {            \
                    OK,                 \
                    res                 \
                }                       \
            )

/*
** Return the given ERR as a result 
*/
# define ERR_PROPAGATION(err)           \
            return (                    \
                (result_t) {            \
                    err,                \
                    err                 \
                }                       \
            )

#endif /* _KERNEL_DEF_STATUS_H_ */
