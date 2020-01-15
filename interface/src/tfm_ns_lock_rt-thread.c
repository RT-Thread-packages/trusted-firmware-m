/*
 * Copyright (c) 2019-2020, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdint.h>
#include <stdbool.h>

#include <rtthread.h>

#include "tfm_api.h"
#include "tfm_ns_lock.h"

/**
 * \brief struct ns_lock_state type
 */
struct ns_lock_state
{
    bool        init;
    rt_mutex_t  mutex;
};

/**
 * \brief ns_lock status
 */
static struct ns_lock_state ns_lock = {.init=false, .mutex=NULL};

/**
 * \brief NS world, NS lock based dispatcher
 */
uint32_t tfm_ns_lock_dispatch(veneer_fn fn,
                              uint32_t arg0, uint32_t arg1,
                              uint32_t arg2, uint32_t arg3)
{
    uint32_t result;

    /* Check the NS lock has been initialized */
    if (ns_lock.init == false) {
        return TFM_ERROR_GENERIC;
    }

    /* TFM request protected by NS lock */
    rt_mutex_take(ns_lock.mutex, RT_WAITING_FOREVER);

    result = fn(arg0, arg1, arg2, arg3);

    rt_mutex_release(ns_lock.mutex);

    return result;
}

/**
 * \brief NS world, Init NS lock
 */
uint32_t tfm_ns_lock_init()
{
    if (ns_lock.init == false) {
        ns_lock.mutex= rt_mutex_create("ns_lock", RT_IPC_FLAG_PRIO);
        if (ns_lock.mutex != RT_NULL) {
            ns_lock.init = true;
            return TFM_SUCCESS;
        }
    }

    return TFM_ERROR_GENERIC;
}

bool tfm_ns_lock_get_init_state()
{
    return ns_lock.init;
}

