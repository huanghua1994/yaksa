/*
 * Copyright (C) by Argonne National Laboratory
 *     See COPYRIGHT in top-level directory
 */

#include "yaksi.h"
#include "yaksu.h"
#include <assert.h>

int yaksa_request_test(yaksa_request_t request, int *completed)
{
    int rc = YAKSA_SUCCESS;

    assert(yaksi_global.is_initialized);

    yaksi_request_s *yaksi_request;
    rc = yaksi_request_get(request, &yaksi_request);
    YAKSU_ERR_CHECK(rc, fn_fail);

    if (yaksu_atomic_load(&yaksi_request->cc)) {
        rc = yaksur_request_test(yaksi_request);
        YAKSU_ERR_CHECK(rc, fn_fail);
    }

    *completed = !yaksu_atomic_load(&yaksi_request->cc);

    if (*completed) {
        rc = yaksi_request_free(yaksi_request);
        YAKSU_ERR_CHECK(rc, fn_fail);
    }

  fn_exit:
    return rc;
  fn_fail:
    goto fn_exit;
}

int yaksa_request_wait(yaksa_request_t request)
{
    int rc = YAKSA_SUCCESS;

    assert(yaksi_global.is_initialized);

    yaksi_request_s *yaksi_request;
    rc = yaksi_request_get(request, &yaksi_request);
    YAKSU_ERR_CHECK(rc, fn_fail);

    if (yaksu_atomic_load(&yaksi_request->cc)) {
        rc = yaksur_request_wait(yaksi_request);
        YAKSU_ERR_CHECK(rc, fn_fail);
    }

    assert(!yaksu_atomic_load(&yaksi_request->cc));
    rc = yaksi_request_free(yaksi_request);
    YAKSU_ERR_CHECK(rc, fn_fail);

  fn_exit:
    return rc;
  fn_fail:
    goto fn_exit;
}
