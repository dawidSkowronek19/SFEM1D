import pytest
import numpy as np
import logging

logger = logging.getLogger(__name__)


def finite_difference(function, ksi, dksi):
    return (function(ksi+dksi)-function(ksi-dksi))/(2.0*dksi)

def test_finite_method(shape_function):
    test_points_nb=100
    dksi = 1e-4
    ksi_points = np.linspace(0,1, test_points_nb)
    p=shape_function.get_p()
    for sh_func_idx in range(p+1):
        sh_func = lambda ksi : shape_function.compute_phi(ksi, sh_func_idx)
        for ksi in ksi_points:
            try:
                assert finite_difference(sh_func, ksi, dksi) == pytest.approx(
                    shape_function.compute_dphi(ksi, sh_func_idx), 
                    rel=1e-4, abs=1e-4
                    )
            except AssertionError:
                logger.error("Shape function div is not equal to diff method")
                raise 