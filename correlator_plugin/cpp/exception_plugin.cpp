/**
 * complex_plugin.cpp 
 *
 * An example plugin that demonstrates:
 * - throwing exceptions from plugin
 * - catching exceptions in monitor
 * - not catching exceptions in monitor
 *
 * $Copyright (c) 2016, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 * 
 */
#include <epl_plugin.hpp>
#include <math.h>

using namespace com::apama::epl;

/**
 * The plugin must be inherited from the EPLPlugin 
 * which needs to be templated over the derived class.
 */
class ExceptionPlugin: public EPLPlugin<ExceptionPlugin>
{
	public:
		// The plugin must implement zero-args constructor.
		ExceptionPlugin(): base_plugin_t("ExceptionPlugin"){} 
		~ExceptionPlugin(){}

		/**
		 * The plugin class must provide the static initialize method that registers
		 * all the methods needed to be visible to the EPL.
		 */
		static void initialize(base_plugin_t::method_data_t &md)
		{
			md.registerMethod<decltype(&ExceptionPlugin::sqrt), &ExceptionPlugin::sqrt>("sqrt");
		}

		double sqrt(double val, const char *exMsg)
		{
			if(val < 0) {
				// Throw exception with the provided exception message
				throw std::runtime_error(exMsg);
			} 
			else {
				return ::sqrt(val);
			}
		}
};
// The following macro must be used to export the plugin. 
APAMA_DECLARE_EPL_PLUGIN(ExceptionPlugin)
