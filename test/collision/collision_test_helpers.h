#pragma once

#include <gtest/gtest.h>

#include "collision.h"

void assert_path_eq(const Path& lhs, const Path& rhs);

void assert_ball_path(const Ball& b, const Ball& expected);
