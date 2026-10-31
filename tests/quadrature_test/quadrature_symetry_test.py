import logging
import pytest


logger = logging.getLogger(__name__)


def test_len(quadrature):
    assert len(quadrature.get_weights()) == len(quadrature.get_points())

def test_symetry(quadrature):
    points = quadrature.get_points()
    weights = quadrature.get_weights()

    for idx in range(len(points)//2):
        sum = points[idx] + points[-1-idx]
        weight_1 = weights[idx]
        weight_2 = weights[-1-idx]
        try:
            assert pytest.approx(sum) == 1
        except AssertionError:
            logger.error(f"Quadrature sum rule for points {idx} is broken: {sum}")
            raise
        try:
            assert weight_1 == pytest.approx(weight_2)
        except AssertionError:
            logger.error(f"Quadrature weight rule is broken for weights {idx}: {weight_1}!={weight_2}")
            raise