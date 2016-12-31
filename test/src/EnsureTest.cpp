/*
 Copyright (C) 2016 Eric Wasylishen
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtest/gtest.h>

#include "Macros.h"
#include "StringUtils.h"

namespace TrenchBroom {
    namespace Ensure {
        TEST(EnsureTest, successfulEnsure) {
            EXPECT_NO_THROW(ensure(true, "this shouldn't fail"));
        }
        
        // Disable a clang warning when using ASSERT_DEATH
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wcovered-switch-default"
#endif

        TEST(EnsureTest, failingEnsure) {
            ASSERT_DEATH(ensure(false, "this should fail"), "");
        }

#ifdef __clang__
#pragma clang diagnostic pop
#endif
    }
}
