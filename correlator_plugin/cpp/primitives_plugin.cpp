/**
 * primitives_plugin.cpp 
 * 
 * An example plugin that demonstrates the use of primitives:
 * - integer
 * - float
 * - boolean
 * - string 
 *
 *$Copyright (c) 2016, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 *Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 *
 */
#include <epl_plugin.hpp>
#include <string.h>

using namespace com::apama::epl;

/**
 * The plugin must be inherited from the EPLPlugin 
 * which needs to be templated over the derived class.
 */
class PrimitivesPlugin: public EPLPlugin<PrimitivesPlugin>
{
	public:
		// The plugin must implement zero-args constructor.
		PrimitivesPlugin(): base_plugin_t("PrimitivesPlugin"){} 
		~PrimitivesPlugin(){}

		/**
		 * The plugin class must provide the static initialize method that registers
		 * all the methods needed to be visible to the EPL.
		 */
		static void initialize(base_plugin_t::method_data_t &md)
		{
			md.registerMethod<decltype(&PrimitivesPlugin::setPrimitives), &PrimitivesPlugin::setPrimitives>("setPrimitives");
		}

		bool setPrimitives(int64_t valInt, double valFloat, bool valBool, const char *valStr)
		{
			/**
			 * The EPLPlugin base class provides access to the logger which 
			 * can be used to log messages to the system log file.
			 */
			logger.crit("setPrimitives %ld %.2lf %d %s", valInt, valFloat, valBool, valStr);
			return true;
		}
};
// The following macro must be used to export the plugin. 
APAMA_DECLARE_EPL_PLUGIN(PrimitivesPlugin)
