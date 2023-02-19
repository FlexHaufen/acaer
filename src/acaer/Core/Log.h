/**
 * @file Log.h
 * @author flexhaufen
 * @brief logger
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

// *** INCLUDES ***


// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
    #include <spdlog/spdlog.h>
    #include <spdlog/fmt/ostr.h>
#pragma warning(pop)

//*** DEFINES ***
#ifndef NLY_DEBUG
    #define AC_LOGGER_ENABLE
#else
    // TODO: This will change later -> maby logger is only file logging is enabled
    #define AC_LOGGER_ENABLE
#endif


//*** NAMESPACE ***
namespace Acaer {
    /**
     * @brief Logger Class
     * 
     */
	class Log {
	public:
        /**
         * @brief initializes the logger
         * 
         */
		static void Init();

        /**
         * @brief Get the Core Logger object
         * 
         * @return std::shared_ptr<spdlog::logger>& 
         */
		static inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

        /**
         * @brief Get the Client Logger object
         * 
         * @return std::shared_ptr<spdlog::logger>& 
         */
		static inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:

        //*** MEMBERS ***
		static inline std::shared_ptr<spdlog::logger> s_CoreLogger;     // Core Logger
		static inline std::shared_ptr<spdlog::logger> s_ClientLogger;   // Client Logger
	};

}

#ifdef AC_LOGGER_ENABLE
    // Core log macros
    #define AC_CORE_TRACE(...)    ::Acaer::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define AC_CORE_INFO(...)     ::Acaer::Log::GetCoreLogger()->info(__VA_ARGS__)
    #define AC_CORE_WARN(...)     ::Acaer::Log::GetCoreLogger()->warn(__VA_ARGS__)
    #define AC_CORE_ERROR(...)    ::Acaer::Log::GetCoreLogger()->error(__VA_ARGS__)
    #define AC_CORE_CRITICAL(...) ::Acaer::Log::GetCoreLogger()->critical(__VA_ARGS__)

    // Client log macros
    #define AC_TRACE(...)         ::Acaer::Log::GetClientLogger()->trace(__VA_ARGS__)
    #define AC_INFO(...)          ::Acaer::Log::GetClientLogger()->info(__VA_ARGS__)
    #define AC_WARN(...)          ::Acaer::Log::GetClientLogger()->warn(__VA_ARGS__)
    #define AC_ERROR(...)         ::Acaer::Log::GetClientLogger()->error(__VA_ARGS__)
    #define AC_CRITICAL(...)      ::Acaer::Log::GetClientLogger()->critical(__VA_ARGS__)

#else
    // Core log macros
    #define AC_CORE_TRACE(...)   
    #define AC_CORE_INFO(...)     
    #define AC_CORE_WARN(...)     
    #define AC_CORE_ERROR(...)    
    #define AC_CORE_CRITICAL(...) 

    // Client log macros
    #define AC_TRACE(...)         
    #define AC_INFO(...)          
    #define AC_WARN(...)          
    #define AC_ERROR(...)         
    #define AC_CRITICAL(...)      
#endif