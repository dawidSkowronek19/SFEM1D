import pytest
import numpy as np
import logging

logger = logging.getLogger(__name__)


def test_boundary_rule(shape_function):
    p = shape_function.get_p()
    left_side_idx=0
    right_side_idx=0

    for shape_function_idx in range(p+1):
        if np.abs(shape_function.compute_phi(0, shape_function_idx))>1e-12:
            logger.info(f"Sh_func {shape_function_idx} != 0 at ksi=0")
            left_side_idx+=1
        if np.abs(shape_function.compute_phi(1, shape_function_idx))>1e-12:
            logger.info(f"Sh_func {shape_function_idx} != 0 at ksi=1")
            right_side_idx+=1
    try:
        assert left_side_idx==1 and right_side_idx==1
    except AssertionError:
        logger.error("Buble sh functions !=0 at boundaries")