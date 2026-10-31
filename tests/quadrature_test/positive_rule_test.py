import logging
import pytest

def test_positive_weight(quadrature):
    weights = quadrature.get_weights()

    for idx, weight in enumerate(weights):
        try:
            assert weight > 0
        except AssertionError:
            logging.error(f"weight for {idx} is not positive: {weight}")