# tests/conftest.py
import pytest
import sfem1d
import logging


logger = logging.getLogger(__name__)

@pytest.fixture(params=[1, 2, 3, 4])
def quadrature(request):
    return sfem1d.QuadratureFactory.ref_line(request.param)