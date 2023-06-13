/**
 * @file Log.cpp
 * @author flexhaufen
 * @brief logger
 * @version 0.1
 * @date 2022-09-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */

// *** INCLUDES ***
#include "acaer/Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// *** DEFINE ***


// *** NAMESPACE ***
namespace Acaer {

	//inline std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	//inline std::shared_ptr<spdlog::logger> Log::s_ClientLogger;


	void Log::Init() {
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Acaer.log", true));

		// ** Console Log **
		//							"[DD.MM.YY HH:MM:SS.ms][pid][tid] [CORE]: msg]"
		logSinks[0]->set_pattern("%^[%d.%m.%C %T.%e][%P][%t] %n: %v%$");
		s_CoreLogger = std::make_shared<spdlog::logger>("[CORE]", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_CoreLogger);
		s_CoreLogger->set_level(spdlog::level::trace);
		s_CoreLogger->flush_on(spdlog::level::trace);

		// ** File Log **
		//							"[DD.MM.YY HH:MM:SS.ms][pid][tid][level]  [APP]: msg]"
		logSinks[1]->set_pattern("%^[%d.%m.%C %T.%e][%P][%t][%l]\t %n: %v");
		s_ClientLogger = std::make_shared<spdlog::logger>("[APP]", begin(logSinks), end(logSinks));
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::info);
		s_ClientLogger->flush_on(spdlog::level::info);

		AC_CORE_INFO("-------------------------------------");
		AC_CORE_INFO("Acaer v{0}", AC_VERSION);
		AC_CORE_INFO("-------------------------------------");
		AC_CORE_INFO("Logger Initialized");
	}
}