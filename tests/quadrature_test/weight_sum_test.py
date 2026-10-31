import pytest
import logging

logger = logging.getLogger(__name__)

def test_weight_sum(quadrature):
    len_ksi = 1
    weights = quadrature.get_weights()
    assert len_ksi == pytest.approx(sum(weights))