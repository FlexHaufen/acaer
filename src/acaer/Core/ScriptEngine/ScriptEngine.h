/**
 * @file ScriptEngine.h
 * @author flexhaufen
 * @brief  ScriptEngine for LUA
 * @version 0.1
 * @date 2023-08-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once

// *** INCLUDES ***
#include "acaer/ac_pch.h"

#include "acaer/Core/ScriptEngine/LuaBind.h"

//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

    class ScriptBindFunctions {
        /**
         * @brief Lua_Print Function
         * 
         * @note param1 String
         * 
         * @param ptrState  lua state
         * @return s32      
         */
        static s32 Lua_ConsolePrint(lua_State* state) {
            if (lua_isstring(state, 1)) {
                AC_CORE_INFO("[LUA] {0}", lua_tostring(state, 1));
            }
            else {
                AC_CORE_WARN("Wrong parameters for Lua_Print - parameter mus be string");
                return 1;
            }
            return 0;
        }
    }


    class ScriptEngine {
    public:
        ScriptEngine() {
            AC_CORE_INFO("Setting up ScriptEngine");
            
            // Load Lua VM
            m_LuaState = luaL_newstate();
            if (m_LuaState) {
                luaL_openlibs(m_LuaState);  // Load Lua std lib

                lua_register(m_LuaState, "ConsolePrint", &ScriptBindFunctions::Lua_ConsolePrint);

                if (luaL_loadfile(m_LuaState, "./src/acaer/Scripts/Lua/testScript.lua") == LUA_OK) {
                    if (lua_pcall(m_LuaState, 0, LUA_MULTRET, 0) == LUA_OK) {   // Run plain script (non function code)
                        m_isLuaOk = true;   // Lua init successfull
                    }
                }
                else {
                    AC_CORE_WARN("Could Not Load Script");
                }
            }
            else {
                AC_CORE_ERROR("LuaSate was NULL");
            }
        }

        ~ScriptEngine() {
            // Destroy Lua VM
            if (m_LuaState) {
                lua_close(m_LuaState);
            }
            else {
                AC_CORE_WARN("ScriptEngine was closed but lua wasn't");
            }
        }

        /**
         * @brief Calls all Lua Scripts OnStart functions
         * 
         */
        void OnStart() {
            if (m_isLuaOk) {
                // Call lua [0 arguments, 0 returns]
                if (lua_getglobal(m_LuaState, "OnStart") == LUA_TFUNCTION) {
                    if (!lua_pcall(m_LuaState, 0, 0, 0) == LUA_OK) {
                        AC_CORE_WARN("Faild to call OnStart");
                    }
                }
            }
        }

        /**
         * @brief Calls all Lua Scripts OnUpdate functions
         * 
         */
        void OnUpdate() {
            if (m_isLuaOk) {
                // Call lua [0 arguments, 0 returns]
                if (lua_getglobal(m_LuaState, "OnUpdate") == LUA_TFUNCTION) {
                    if (!lua_pcall(m_LuaState, 0, 0, 0) == LUA_OK) {
                        AC_CORE_WARN("Faild to call OnUpdate");
                    }
                }
            }
        }

    private:
        // ** Members **
        
        lua_State* m_LuaState = nullptr;    // ptr to current lua state
        b8 m_isLuaOk = false;               // bool to check if lua initialization is ok
    };
}