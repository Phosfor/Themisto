#pragma once

uint16 get_fixture_category_bits(const b2Fixture *fixture);
void set_fixture_category_bits(b2Fixture *fixture, uint16 category_bits);
uint16 get_fixture_mask_bits(const b2Fixture *fixture);
void set_fixture_mask_bits(b2Fixture *fixture, uint16 mask_bits);
uint16 get_fixture_group_index(const b2Fixture *fixture);
void set_fixture_group_index(b2Fixture *fixture, uint16 group_index);
void wrap_fixture();
