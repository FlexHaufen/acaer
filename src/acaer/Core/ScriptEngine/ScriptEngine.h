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

//*** DEFINES ***

//*** NAMESPACE ***
namespace Acaer {

    namespace Native {

        static auto Lua_ConsolePrint = [](lua_State* L) -> int {
            if (lua_isstring(L, 1)) {
                AC_CORE_INFO("[LUA] {0}", lua_tostring(L, 1));
            }
            else {
                AC_CORE_WARN("Wrong parameters for Lua_Print - parameter mus be string");
                return 1;
            }
            return 0;
        };
    }


    class ScriptEngine {
    public:
        ScriptEngine() {
            AC_CORE_INFO("Setting up ScriptEngine");
            
            // Load Lua VM
            m_L = luaL_newstate();

            if (!m_L) {
                AC_CORE_ERROR("LuaSate was NULL");
                return;
            }

            luaL_openlibs(m_L);  // Load Lua std lib

            // ** Bind functions **
            lua_register(m_L, "ConsolePrint", Native::Lua_ConsolePrint);

            if (luaL_loadfile(m_L, "./src/acaer/Scripts/Lua/testScript.lua") != LUA_OK) {
                AC_CORE_WARN("Could Not Load Script");
                return;
            }

            if (lua_pcall(m_L, 0, LUA_MULTRET, 0) == LUA_OK) {   // Run plain script (non function code)
                m_L_Ok = true;   // Lua init successfull
            }
        }

        ~ScriptEngine() {
            // Destroy Lua VM
            if (!m_L) {
                AC_CORE_WARN("ScriptEngine was closed but lua wasn't");
                return;
            }
            AC_CORE_INFO("Shutting down ScriptEngine");
            lua_close(m_L);
        }

        /**
         * @brief Calls all Lua Scripts OnStart functions
         * 
         */
        void OnStart() {
            if (!m_L_Ok) {
                return;
            }
            // Call lua [0 arguments, 0 returns]
            if (lua_getglobal(m_L, "OnStart") == LUA_TFUNCTION) {
                if (!lua_pcall(m_L, 0, 0, 0) == LUA_OK) {
                    AC_CORE_WARN("Faild to call OnStart");
                }
            }
        }

        /**
         * @brief Calls all Lua Scripts OnUpdate functions
         * 
         */
        void OnUpdate() {
            if (!m_L_Ok) {
                return;
            }
            // Call lua [0 arguments, 0 returns]
            if (lua_getglobal(m_L, "OnUpdate") == LUA_TFUNCTION) {
                if (!lua_pcall(m_L, 0, 0, 0) == LUA_OK) {
                    AC_CORE_WARN("Faild to call OnUpdate");
                }
            }
        }

    private:
        // ** Members **
        
        lua_State* m_L = nullptr;   // ptr to current lua state
        b8 m_L_Ok = false;          // bool to check if lua initialization is ok
    };
}