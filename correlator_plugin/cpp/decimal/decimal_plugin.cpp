/**
 * complex_plugin.cpp 
 *
 * An example plugin that demonstrates:
 * - the use of decimals in CPP plugins 
 * - the use of sequence of decimals for computing the average value 
 *
 * $Copyright (c) 2016, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 * 
 */
#include <epl_plugin.hpp>

using namespace com::apama::epl;

/** This file requires the user downloads the Intel Decimal FP library
 * from - https://software.intel.com/en-us/articles/intel-decimal-floating-point-math-library
 * and setup paths correctly
 */
#include <bid_conf.h>
#include <bid_functions.h>

/**
 * The plugin must be inherited from the EPLPlugin 
 * which needs to be templated over the derived class.
 */
class DecimalPlugin: public EPLPlugin<DecimalPlugin>
{
	public:
		// The plugin must implement zero-args constructor.
		DecimalPlugin(): base_plugin_t("DecimalPlugin"){} 
		~DecimalPlugin(){}

		/**
		 * The plugin class must provide the static initialize method that registers
		 * all the methods needed to be visible to the EPL.
		 */
		static void initialize(base_plugin_t::method_data_t &md)
		{
			md.registerMethod<decltype(&DecimalPlugin::SetDecimalVal), &DecimalPlugin::SetDecimalVal>("SetDecimalVal");
			md.registerMethod<decltype(&DecimalPlugin::GetDecAvg), &DecimalPlugin::GetDecAvg>("GetDecAvg", "action<sequence<decimal> > returns decimal");
		}

		bool SetDecimalVal(decimal_t val)
		{
			/*
			 * Use the Intel Decimal FP Library bid64_to_string
			 * to convert the decimal to string.
			 */
			_IDEC_flags status = BID_EXACT_STATUS;
			char fbuf[100];
			bid64_to_string(fbuf, val.d, &status);
			logger.crit("decimal value = %s", fbuf);
			return true;
		}
	
		decimal_t GetDecAvg(const list_t &l)
		{
			_IDEC_flags status = BID_EXACT_STATUS;
			if(!l.size()) {
				decimal_t rv{bid64_from_int64(0, BID_ROUNDING_TO_NEAREST, &status)};
				return rv;
			}

			decimal_t sum{bid64_from_int64(0, BID_ROUNDING_TO_NEAREST, &status)};
			for(uint32_t i = 0; i < l.size(); ++i)
			{
				decimal_t dl = get<decimal_t>(l[i]);
				sum.d = bid64_add(sum.d, dl.d, BID_ROUNDING_TO_NEAREST, &status);
			}

			decimal_t avg{bid64_div(sum.d, bid64_from_int64(l.size(), BID_ROUNDING_TO_NEAREST, &status), BID_ROUNDING_TO_NEAREST, &status)};
			return avg;
		}
};
// The following macro must be used to export the plugin. 
APAMA_DECLARE_EPL_PLUGIN(DecimalPlugin)
