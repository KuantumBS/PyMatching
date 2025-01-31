# Copyright 2022 PyMatching Contributors

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#      http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import pytest

from pymatching.matching import Matching


def test_load_from_dem():
    stim = pytest.importorskip("stim")
    c = stim.Circuit.generated("surface_code:rotated_memory_x", distance=5, rounds=5,
                               after_clifford_depolarization=0.01,
                               before_measure_flip_probability=0.01,
                               after_reset_flip_probability=0.01,
                               before_round_data_depolarization=0.01)
    dem = c.detector_error_model(decompose_errors=True)
    m = Matching.from_detector_error_model(dem)
    assert m.num_detectors == dem.num_detectors
    assert m.num_fault_ids == dem.num_observables
    assert m.num_edges == 502
    m2 = Matching(dem)
    assert m2.num_detectors == dem.num_detectors
    assert m2.num_fault_ids == dem.num_observables
    assert m2.num_edges == 502


def test_load_from_dem_wrong_type_raises_type_error():
    stim = pytest.importorskip("stim")
    c = stim.Circuit.generated("surface_code:rotated_memory_x", distance=3, rounds=1,
                               after_clifford_depolarization=0.01)
    with pytest.raises(TypeError):
        Matching.from_detector_error_model(c)


def test_load_from_dem_without_stim_raises_type_error():
    try:
        import stim  # noqa
    except ImportError:
        with pytest.raises(TypeError):
            Matching.from_detector_error_model("test")
