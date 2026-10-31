# tests/conftest.py
import pytest
import sfem1d
import logging


logger = logging.getLogger(__name__)

@pytest.fixture(params=[1, 2, 3])
def shape_function(request):
    logger.info(f'Testing for p = {request.param}')
    return sfem1d.ShapeFunction(request.param)