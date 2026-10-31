import numpy as np
import pytest
import logging

logger = logging.getLogger(__name__)

def test_get_ksi_s_weight_s(quadrature):
    cached_points = quadrature.get_points()
    cached_weights = quadrature.get_weights()

    for idx, (cached_point, cached_weight,) in enumerate(zip(cached_points, cached_weights)):
        explicit_point = quadrature.get_ksi(idx)
        explicit_weight = quadrature.get_weight(idx)


        try:
            assert cached_point == pytest.approx(explicit_point)
        except AssertionError:
            logger.error(f"Cached point: {cached_point} != explicit point {explicit_point}")
            raise

        try:
            assert cached_weight == pytest.approx(explicit_weight)
        except AssertionError:
            logger.error(f"Cached weight: {cached_weight} != explicit weight {explicit_weight}")
            raise

def test_get_point_nb(quadrature):
    expected_num = len(quadrature.get_points())
    assert quadrature.get_point_numb() == expected_num