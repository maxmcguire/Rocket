/*
 * RocketVM
 * Copyright (c) 2011 Max McGuire
 *
 * See copyright notice in COPYRIGHT
 */

#include "Test.h"
#include "LuaTest.h"

TEST_FIXTURE(TableNegativeZero, LuaFixture)
{

    // Check that -0 and 0 are equivalent when using as a table key.

    lua_newtable(L);
    int table = lua_gettop(L);

    lua_pushnumber(L, -0.0);
    lua_pushstring(L, "zero");
    lua_rawset(L, table);

    lua_pushnumber(L, 0.0);
    lua_rawget(L, table);

    CHECK_EQ( lua_tostring(L, -1), "zero" );

}

TEST_FIXTURE(ArrayAppend, LuaFixture)
{

    lua_newtable(L);
    int table = lua_gettop(L);

    lua_pushstring(L, "one");
    lua_rawseti(L, table, 1);

    lua_pushstring(L, "two");
    lua_rawseti(L, table, 2);

    lua_pushstring(L, "three");
    lua_rawseti(L, table, 3);

    lua_pushstring(L, "four");
    lua_rawseti(L, table, 4);

    lua_rawgeti(L, table, 1);
    CHECK_EQ( lua_tostring(L, -1), "one" );

    lua_rawgeti(L, table, 2);
    CHECK_EQ( lua_tostring(L, -1), "two" );

    lua_rawgeti(L, table, 3);
    CHECK_EQ( lua_tostring(L, -1), "three" );

    lua_rawgeti(L, table, 4);
    CHECK_EQ( lua_tostring(L, -1), "four" );

}

TEST_FIXTURE(ArrayConvertFromHash, LuaFixture)
{

    lua_newtable(L);
    int table = lua_gettop(L);

    lua_pushstring(L, "four");
    lua_rawseti(L, table, 4);

    lua_pushstring(L, "five");
    lua_rawseti(L, table, 5);

    lua_pushstring(L, "six");
    lua_rawseti(L, table, 6);

    lua_pushstring(L, "seven");
    lua_rawseti(L, table, 7);

    lua_pushstring(L, "one");
    lua_rawseti(L, table, 1);

    lua_pushstring(L, "two");
    lua_rawseti(L, table, 2);

    lua_pushstring(L, "three");
    lua_rawseti(L, table, 3);

    lua_rawgeti(L, table, 1);
    CHECK_EQ( lua_tostring(L, -1), "one" );

    lua_rawgeti(L, table, 2);
    CHECK_EQ( lua_tostring(L, -1), "two" );

    lua_rawgeti(L, table, 3);
    CHECK_EQ( lua_tostring(L, -1), "three" );

    lua_rawgeti(L, table, 4);
    CHECK_EQ( lua_tostring(L, -1), "four" );

    lua_rawgeti(L, table, 5);
    CHECK_EQ( lua_tostring(L, -1), "five" );

    lua_rawgeti(L, table, 6);
    CHECK_EQ( lua_tostring(L, -1), "six" );

    lua_rawgeti(L, table, 7);
    CHECK_EQ( lua_tostring(L, -1), "seven" );

    lua_rawgeti(L, table, 8);
    CHECK( lua_isnil(L, -1) );

}

TEST_FIXTURE(ArrayRemove, LuaFixture)
{

    const char* code =
        "t = { 'one', 'two', 'three', 'four', 'five' }\n"
        "t[4] = nil";

    CHECK( DoString(L, code) );

    lua_getglobal(L, "t");
    int table = lua_gettop(L);

    lua_rawgeti(L, table, 1);
    CHECK_EQ( lua_tostring(L, -1), "one" );

    lua_rawgeti(L, table, 2);
    CHECK_EQ( lua_tostring(L, -1), "two" );

    lua_rawgeti(L, table, 3);
    CHECK_EQ( lua_tostring(L, -1), "three" );

    lua_rawgeti(L, table, 4);
    CHECK( lua_isnil(L, -1) );

    lua_rawgeti(L, table, 5);
    CHECK_EQ( lua_tostring(L, -1), "five" );

}

TEST_FIXTURE(ArrayNext, LuaFixture)
{

    lua_newtable(L);
    int table = lua_gettop(L);

    lua_pushinteger(L, 1);
    lua_rawseti(L, table, 1);

    lua_pushinteger(L, 2);
    lua_rawseti(L, table, 2);

    lua_pushinteger(L, 3);
    lua_rawseti(L, table, 3);

    lua_pushinteger(L, 4);
    lua_rawseti(L, table, 4);

    int count[4] = { 0 };

    lua_pushnil(L);
    while (lua_next(L, table))
    {
        lua_Integer key   = lua_tointeger(L, -2);
        lua_Integer value = lua_tointeger(L, -1);
        CHECK_EQ( key, value );
        CHECK( key >= 1 && key <= 4 );
        ++count[key - 1];
        lua_pop(L, 1);
    }

    for (int i = 0; i < 4; ++i)
    {
        CHECK_EQ( count[i], 1 );
    }

}

TEST_FIXTURE(ArrayLarge, LuaFixture)
{

    const char* code = 
        "t = {\n"
        "0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,\n"
        "0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,\n"
        "0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,\n"
        "0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,\n"
        "0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,\n"
        "0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,\n"
        "0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,\n"
        "0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,\n"
        "0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,\n"
        "0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,\n"
        "0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,\n"
        "0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,\n"
        "0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,\n"
        "0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,\n"
        "0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,\n"
        "0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,\n"
        "0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe,0xff\n"
        "}";

    CHECK( DoString(L, code) );
    
    lua_getglobal(L, "t");
    CHECK( lua_istable(L, -1) );

    for (int i = 1; i < 256; ++i)
    {
        lua_rawgeti(L, -1, i);
        CHECK( lua_tonumber(L, -1) == i );
        lua_pop(L, 1);
    }

}

TEST_FIXTURE(LocalTable, LuaFixture)
{

    const char* code =
        "local _t = { 'one', 'two', 'three' }\n"
        "t = _t";
    
    CHECK( DoString(L, code) );

    lua_getglobal(L, "t");
    CHECK( lua_istable(L, -1) );
    
    lua_rawgeti(L, -1, 1);
    CHECK_EQ( lua_tostring(L, -1), "one" );
    lua_pop(L, 1);

    lua_rawgeti(L, -1, 2);
    CHECK_EQ( lua_tostring(L, -1), "two" );
    lua_pop(L, 1);

    lua_rawgeti(L, -1, 3);
    CHECK_EQ( lua_tostring(L, -1), "three" );
    lua_pop(L, 1);

}

/*
TEST_FIXTURE(WeakKeys, LuaFixture)
{

    lua_newtable(L);
    int table = lua_gettop(L);

    lua_newtable(L);
    lua_pushstring(L, "k");
    lua_setfield(L, -2, "__mode");

    lua_setmetatable(L, table);

    void* p = lua_newuserdata(L, 10);
    lua_pushstring(L, "value");
    lua_settable(L, table);

    lua_gc(L, LUA_GCCOLLECT, 0);

    // There shouldn't be anything left in the table after GC.
    lua_pushnil(L);
    CHECK( lua_next(L, table) == 0 );

}
*/