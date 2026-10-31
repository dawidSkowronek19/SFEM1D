import pytest
import numpy as np
import logging

logger = logging.getLogger(__name__)

def test_sum_rule(shape_function):
    ksi_points = np.linspace(0, 1, 100)
    p = shape_function.get_p()
    shape_function_numb = p+1
    for ksi in ksi_points:
        sum_phi=0
        sum_dphi=0
        for shape_function_idx in range(shape_function_numb):
            sum_phi+=shape_function.compute_phi(ksi, shape_function_idx)
            sum_dphi+=shape_function.compute_dphi(ksi, shape_function_idx)

        try:
            assert sum_phi == pytest.approx(1.0, rel=1e-12)
            assert sum_dphi == pytest.approx(0.0, abs=1e-12)
        except AssertionError as e:
            logger.error(f"ksi={ksi} (p={p}): sum(phi) = {sum_phi}, sum(dphi) = {sum_dphi}")

    logger.info(f"Success for sum test {p}")