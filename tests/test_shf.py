import pytest
import numpy as np
import shape_module
import logging

logger = logging.getLogger(__name__)

# ==========================================
# 1. PARAMETRYZOWANE FIXTURE
# ==========================================
# Pytest sam wykona ten fixture i test 3 razy: dla p = 1, 2 oraz 3
@pytest.fixture(params=[1, 2, 3])
def shape_function(request):
    logger.info(f'Testing for {request.param}')
    return shape_module.ShapeFunction(request.param)

def test_cache_and_explicit_sh_fc_evaluation(shape_function):
    integ_points = np.linspace(-1.0, 1.0, 20)
    shape_function.set_cache_val(integ_points)
    p = shape_function.get_p()
    shfunc_nb = p + 1 

    for point_idx, ksi in enumerate(integ_points):
        for sh_idx in range(shfunc_nb):

            cache_phi = shape_function.get_cached_phi(point_idx, sh_idx)
            cache_dphi = shape_function.get_cached_dphi(point_idx, sh_idx)

            explicit_val_phi = shape_function.compute_phi(ksi, sh_idx)
            explicit_val_dphi = shape_function.compute_dphi(ksi, sh_idx)

            assert cache_phi == pytest.approx(explicit_val_phi, rel=1e-9)
            assert cache_dphi == pytest.approx(explicit_val_dphi, rel=1e-9)