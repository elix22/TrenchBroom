/*
 Copyright (C) 2010-2016 Kristian Duske
 
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

#include "EL.h"

#include <limits>

namespace TrenchBroom {
    namespace EL {
        TEST(ELTest, constructValues) {
            ASSERT_EQ(Type_Boolean, Value(true).type());
            ASSERT_EQ(Type_Boolean, Value(false).type());
            ASSERT_EQ(Type_String,  Value("test").type());
            ASSERT_EQ(Type_Number,  Value(1.0).type());
            ASSERT_EQ(Type_Array,   Value(ArrayType()).type());
            ASSERT_EQ(Type_Map,     Value(MapType()).type());
            ASSERT_EQ(Type_Null,    Value().type());
        }
        
        TEST(ELTest, subscriptOperator) {
            ASSERT_THROW(Value(true)[Value(0)], EvaluationError);
            ASSERT_THROW(Value("test")[Value(0)], EvaluationError);
            ASSERT_THROW(Value(1.0)[Value(0)], EvaluationError);
            ASSERT_THROW(Value()[Value(0)], EvaluationError);
            
            ArrayType array;
            array.push_back(Value(1.0));
            array.push_back(Value("test"));
            
            const Value arrayValue(array);
            
            ASSERT_EQ(Value(1.0), arrayValue[Value(0)]);
            ASSERT_EQ(Value("test"), arrayValue[Value(1)]);
            ASSERT_EQ(Value("test"), arrayValue[Value(-1)]);
            ASSERT_EQ(Value(1.0), arrayValue[Value(-2)]);
            
            ASSERT_THROW(arrayValue[Value(2)], EvaluationError);
            ASSERT_THROW(arrayValue[Value(-3)], EvaluationError);
            ASSERT_THROW(arrayValue[Value("asdf")], EvaluationError);
            ASSERT_THROW(arrayValue[Value("")], EvaluationError);
            
            
            MapType map;
            map["test"] = Value(1.0);
            map["huhu"] = Value("yeah");
            
            const Value mapValue(map);
            
            ASSERT_EQ(Value(1.0), mapValue[Value("test")]);
            ASSERT_EQ(Value("yeah"), mapValue[Value("huhu")]);
            ASSERT_EQ(Value::Null, mapValue[Value("huu")]);
            ASSERT_EQ(Value::Null, mapValue[Value("")]);
            ASSERT_EQ(Value::Null, mapValue[Value("huu")]);
        }
        
        TEST(ELTest, unaryPlusOperator) {
            ASSERT_THROW(+Value("test"), EvaluationError);
            ASSERT_THROW(+Value(ArrayType()), EvaluationError);
            ASSERT_THROW(+Value(MapType()), EvaluationError);
            
            ASSERT_EQ(Value(1.0), +Value(1.0));
            ASSERT_EQ(Value(1.0), +Value(true));
            ASSERT_EQ(Value(0.0), +Value(false));
        }
        
        TEST(ELTest, unaryMinusOperator) {
            ASSERT_THROW(-Value("test"), EvaluationError);
            ASSERT_THROW(-Value(ArrayType()), EvaluationError);
            ASSERT_THROW(-Value(MapType()), EvaluationError);
            
            ASSERT_EQ(Value(-1.0), -Value(1.0));
            ASSERT_EQ(Value(-1.0), -Value(true));
            ASSERT_EQ(Value( 0.0), -Value(false));
        }
        
        TEST(ELTest, binaryPlusOperator) {
            ASSERT_EQ(Value(2.0),           Value(true)     + Value(true));
            ASSERT_EQ(Value(3.0),           Value(false)    + Value(3.0));
            ASSERT_THROW(Value(true) + Value("test"),       EvaluationError);
            ASSERT_THROW(Value(true) + Value::Null,         EvaluationError);
            ASSERT_THROW(Value(true) + Value(ArrayType()),  EvaluationError);
            ASSERT_THROW(Value(true) + Value(MapType()),    EvaluationError);

            ASSERT_EQ(Value(2.0),           Value(1.0)      + Value(true));
            ASSERT_EQ(Value(2.0),           Value(3.0)      + Value(-1.0));
            ASSERT_THROW(Value(1.0) + Value("test"),        EvaluationError);
            ASSERT_THROW(Value(1.0) + Value::Null,          EvaluationError);
            ASSERT_THROW(Value(1.0) + Value(ArrayType()),   EvaluationError);
            ASSERT_THROW(Value(1.0) + Value(MapType()),     EvaluationError);
            
            ASSERT_THROW(Value("tst") + Value(true),        EvaluationError);
            ASSERT_THROW(Value("tst") + Value(2.0),         EvaluationError);
            ASSERT_EQ(Value("tsttest"),     Value("tst")    + Value("test"));
            ASSERT_THROW(Value("tst") + Value::Null,        EvaluationError);
            ASSERT_THROW(Value("tst") + Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value("tst") + Value(MapType()),   EvaluationError);

            ASSERT_THROW(Value(ArrayType()) + Value(true),          EvaluationError);
            ASSERT_THROW(Value(ArrayType()) + Value(1.0),           EvaluationError);
            ASSERT_THROW(Value(ArrayType()) + Value("test"),        EvaluationError);
            ASSERT_THROW(Value(ArrayType()) + Value::Null,          EvaluationError);
            ASSERT_THROW(Value(ArrayType()) + Value(ArrayType()),   EvaluationError);
            ASSERT_THROW(Value(ArrayType()) + Value(MapType()),     EvaluationError);
            
            ASSERT_THROW(Value(MapType()) + Value(true),          EvaluationError);
            ASSERT_THROW(Value(MapType()) + Value(1.0),           EvaluationError);
            ASSERT_THROW(Value(MapType()) + Value("test"),        EvaluationError);
            ASSERT_THROW(Value(MapType()) + Value::Null,          EvaluationError);
            ASSERT_THROW(Value(MapType()) + Value(ArrayType()),   EvaluationError);
            ASSERT_THROW(Value(MapType()) + Value(MapType()),     EvaluationError);
        }
        
        TEST(ELTest, binaryMinusOperator) {
            ASSERT_EQ(Value(0.0),           Value(true)     - Value(true));
            ASSERT_EQ(Value(-3.0),          Value(false)    - Value(3.0));
            ASSERT_THROW(Value(true) - Value("test"),       EvaluationError);
            ASSERT_THROW(Value(true) - Value::Null,         EvaluationError);
            ASSERT_THROW(Value(true) - Value(ArrayType()),  EvaluationError);
            ASSERT_THROW(Value(true) - Value(MapType()),    EvaluationError);
            
            ASSERT_EQ(Value(1.0),           Value(2.0)      - Value(true));
            ASSERT_EQ(Value(-1.0),          Value(2.0)      - Value(3.0));
            ASSERT_THROW(Value(1.0) - Value("test"),        EvaluationError);
            ASSERT_THROW(Value(1.0) - Value::Null,          EvaluationError);
            ASSERT_THROW(Value(1.0) - Value(ArrayType()),   EvaluationError);
            ASSERT_THROW(Value(1.0) - Value(MapType()),     EvaluationError);
            
            ASSERT_THROW(Value("test") - Value(true),           EvaluationError);
            ASSERT_THROW(Value("test") - Value(1.0),            EvaluationError);
            ASSERT_THROW(Value("test") - Value("test"),         EvaluationError);
            ASSERT_THROW(Value("test") - Value::Null,           EvaluationError);
            ASSERT_THROW(Value("test") - Value(ArrayType()),    EvaluationError);
            ASSERT_THROW(Value("test") - Value(MapType()),      EvaluationError);
            
            ASSERT_THROW(Value(ArrayType()) - Value(true),           EvaluationError);
            ASSERT_THROW(Value(ArrayType()) - Value(1.0),            EvaluationError);
            ASSERT_THROW(Value(ArrayType()) - Value("test"),         EvaluationError);
            ASSERT_THROW(Value(ArrayType()) - Value::Null,           EvaluationError);
            ASSERT_THROW(Value(ArrayType()) - Value(ArrayType()),    EvaluationError);
            ASSERT_THROW(Value(ArrayType()) - Value(MapType()),      EvaluationError);
            
            ASSERT_THROW(Value(MapType()) - Value(true),           EvaluationError);
            ASSERT_THROW(Value(MapType()) - Value(1.0),            EvaluationError);
            ASSERT_THROW(Value(MapType()) - Value("test"),         EvaluationError);
            ASSERT_THROW(Value(MapType()) - Value::Null,           EvaluationError);
            ASSERT_THROW(Value(MapType()) - Value(ArrayType()),    EvaluationError);
            ASSERT_THROW(Value(MapType()) - Value(MapType()),      EvaluationError);
        }
        
        TEST(ELTest, binaryTimesOperator) {
            ASSERT_EQ(Value(0.0), Value(true) * Value(false));
            ASSERT_EQ(Value(1.0), Value(true) * Value(true));
            ASSERT_EQ(Value(-2.0), Value(true) * Value(-2.0));
            ASSERT_THROW(Value(true) * Value("test"), EvaluationError);
            ASSERT_THROW(Value(true) * Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(true) * Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(true) * Value::Null, EvaluationError);
            
            ASSERT_EQ(Value(0.0), Value(2.0) * Value(false));
            ASSERT_EQ(Value(2.0), Value(2.0) * Value(true));
            ASSERT_EQ(Value(-6.0), Value(3.0) * Value(-2.0));
            ASSERT_THROW(Value(1.0) * Value("test"), EvaluationError);
            ASSERT_THROW(Value(1.0) * Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(1.0) * Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(1.0) * Value::Null, EvaluationError);

            ASSERT_THROW(Value("test") * Value(true), EvaluationError);
            ASSERT_THROW(Value("test") * Value(1.0), EvaluationError);
            ASSERT_THROW(Value("test") * Value("test"), EvaluationError);
            ASSERT_THROW(Value("test") * Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value("test") * Value(MapType()), EvaluationError);
            ASSERT_THROW(Value("test") * Value::Null, EvaluationError);
            
            ASSERT_THROW(Value(ArrayType()) * Value(true), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) * Value(1.0), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) * Value("test"), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) * Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) * Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) * Value::Null, EvaluationError);
            
            ASSERT_THROW(Value(MapType()) * Value(true), EvaluationError);
            ASSERT_THROW(Value(MapType()) * Value(1.0), EvaluationError);
            ASSERT_THROW(Value(MapType()) * Value("test"), EvaluationError);
            ASSERT_THROW(Value(MapType()) * Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(MapType()) * Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(MapType()) * Value::Null, EvaluationError);
        }
        
        TEST(ELTest, binaryOverOperator) {
            ASSERT_EQ(Value(std::numeric_limits<NumberType>::infinity()), Value(true) / Value(false));
            ASSERT_EQ(Value(1.0), Value(true) / Value(true));
            ASSERT_EQ(Value(-0.5), Value(true) / Value(-2.0));
            ASSERT_THROW(Value(true) / Value("test"), EvaluationError);
            ASSERT_THROW(Value(true) / Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(true) / Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(true) / Value::Null, EvaluationError);
            
            ASSERT_EQ(Value(-std::numeric_limits<NumberType>::infinity()), Value(-2.0) / Value(false));
            ASSERT_EQ(Value(2.0), Value(2.0) / Value(true));
            ASSERT_EQ(Value(-1.5), Value(3.0) / Value(-2.0));
            ASSERT_THROW(Value(1.0) / Value("test"), EvaluationError);
            ASSERT_THROW(Value(1.0) / Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(1.0) / Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(1.0) / Value::Null, EvaluationError);
            
            ASSERT_THROW(Value("test") / Value(true), EvaluationError);
            ASSERT_THROW(Value("test") / Value(1.0), EvaluationError);
            ASSERT_THROW(Value("test") / Value("test"), EvaluationError);
            ASSERT_THROW(Value("test") / Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value("test") / Value(MapType()), EvaluationError);
            ASSERT_THROW(Value("test") / Value::Null, EvaluationError);
            
            ASSERT_THROW(Value(ArrayType()) / Value(true), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) / Value(1.0), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) / Value("test"), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) / Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) / Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) / Value::Null, EvaluationError);
            
            ASSERT_THROW(Value(MapType()) / Value(true), EvaluationError);
            ASSERT_THROW(Value(MapType()) / Value(1.0), EvaluationError);
            ASSERT_THROW(Value(MapType()) / Value("test"), EvaluationError);
            ASSERT_THROW(Value(MapType()) / Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(MapType()) / Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(MapType()) / Value::Null, EvaluationError);
        }
        
        TEST(ELTest, binaryModulusOperator) {
            ASSERT_TRUE(std::isnan((Value(true) % Value(false)).numberValue()));
            ASSERT_EQ(Value(0.0), Value(true) % Value(true));
            ASSERT_EQ(Value(1.0), Value(true) % Value(-2.0));
            ASSERT_THROW(Value(true) % Value("test"), EvaluationError);
            ASSERT_THROW(Value(true) % Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(true) % Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(true) % Value::Null, EvaluationError);
            
            ASSERT_TRUE(std::isnan((Value(-2.0) % Value(false)).numberValue()));
            ASSERT_EQ(Value(0.0), Value(2.0) % Value(true));
            ASSERT_EQ(Value(1.0), Value(3.0) % Value(-2.0));
            ASSERT_THROW(Value(1.0) % Value("test"), EvaluationError);
            ASSERT_THROW(Value(1.0) % Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(1.0) % Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(1.0) % Value::Null, EvaluationError);
            
            ASSERT_THROW(Value("test") % Value(true), EvaluationError);
            ASSERT_THROW(Value("test") % Value(1.0), EvaluationError);
            ASSERT_THROW(Value("test") % Value("test"), EvaluationError);
            ASSERT_THROW(Value("test") % Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value("test") % Value(MapType()), EvaluationError);
            ASSERT_THROW(Value("test") % Value::Null, EvaluationError);
            
            ASSERT_THROW(Value(ArrayType()) % Value(true), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) % Value(1.0), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) % Value("test"), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) % Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) % Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(ArrayType()) % Value::Null, EvaluationError);
            
            ASSERT_THROW(Value(MapType()) % Value(true), EvaluationError);
            ASSERT_THROW(Value(MapType()) % Value(1.0), EvaluationError);
            ASSERT_THROW(Value(MapType()) % Value("test"), EvaluationError);
            ASSERT_THROW(Value(MapType()) % Value(ArrayType()), EvaluationError);
            ASSERT_THROW(Value(MapType()) % Value(MapType()), EvaluationError);
            ASSERT_THROW(Value(MapType()) % Value::Null, EvaluationError);
        }
    }
}
