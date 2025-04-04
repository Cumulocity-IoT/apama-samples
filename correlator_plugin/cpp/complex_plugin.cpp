/**
 * complex_plugin.cpp 
 *
 * A example plugin that demonstrates how to hande sequences and chunks in plugins
 * Various operations are performed over the lists:
 * - average of integer values
 * - standard deviation of float values
 * - logic AND operation of boolean values
 * - concatenation of string values
 *
 * The plugin defines a ComplexNumber as a chunk and:
 * - has setters and getters for the real and imaginary parts
 * - has the ability to add a second number to itself
 *
 * $Copyright (c) 2016-2017, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 * 
 */
#include <epl_plugin.hpp>
#include <string.h>
#include <math.h>

using namespace com::apama::epl;

class ComplexNumber
{
	public:
		ComplexNumber(): real(0), imag(0) {}
		ComplexNumber(double r, double i): real(r), imag(i) {}

		void addNumber(const ComplexNumber *rhs)
		{
			real += rhs->real;
			imag += rhs->imag;
		}

		double getReal() const {return real;}
		double getImag() const {return imag;}

		void setReal(const double r) {real = r;}
		void setImag(const double i) {imag = i;}

	private:
		double real;
		double imag;
};

/**
 * The plugin must be inherited from the EPLPlugin 
 * which needs to be templated over the derived class.
 */
class ComplexPlugin: public EPLPlugin<ComplexPlugin>
{
	public:
		// The plugin must implement zero-args constructor.
		ComplexPlugin(): base_plugin_t("ComplexPlugin"){} 
		~ComplexPlugin(){}

		/**
		 * The plugin class must provide the static initialize method that registers
		 * all the methods needed to be visible to the EPL.
		 */
		static void initialize(base_plugin_t::method_data_t &md)
		{
			/**
			 * Use custom signature if the arguments or the return types contain
			 * list_t
			 * map_t 
			 * int64_t(for passing context objects)
			 */
			md.registerMethod<decltype(&ComplexPlugin::getIntAvg), &ComplexPlugin::getIntAvg>("getIntAvg", "action<sequence<integer> > returns integer");
			md.registerMethod<decltype(&ComplexPlugin::getFloatStd), &ComplexPlugin::getFloatStd>("getFloatStd", "action<sequence<float> > returns float");
			md.registerMethod<decltype(&ComplexPlugin::getBoolAnd), &ComplexPlugin::getBoolAnd>("getBoolAnd", "action<sequence<boolean> > returns boolean");
			md.registerMethod<decltype(&ComplexPlugin::getStrCat), &ComplexPlugin::getStrCat>("getStrCat", "action<sequence<string> > returns string");
			md.registerMethod<decltype(&ComplexPlugin::getTypes), &ComplexPlugin::getTypes>("getTypes", "action<any> returns string");

			/*
			 * Register functions that handle the ComplexNumber chunks.
			 * Plugin methods can be overloaded but EPL does not allow function overloading.
			 */
			md.registerMethod<custom_t<ComplexNumber>(ComplexPlugin::*)(double, double), &ComplexPlugin::makeComplexNumber>("makeComplexNumberFull");
			md.registerMethod<custom_t<ComplexNumber>(ComplexPlugin::*)(), &ComplexPlugin::makeComplexNumber>("makeComplexNumberEmpty");
			md.registerMethod<decltype(&ComplexPlugin::getRealVal), &ComplexPlugin::getRealVal>("getRealVal");
			md.registerMethod<decltype(&ComplexPlugin::getImagVal), &ComplexPlugin::getImagVal>("getImagVal");
			md.registerMethod<decltype(&ComplexPlugin::setRealVal), &ComplexPlugin::setRealVal>("setRealVal"); 
			md.registerMethod<decltype(&ComplexPlugin::setImagVal), &ComplexPlugin::setImagVal>("setImagVal"); 
			md.registerMethod<decltype(&ComplexPlugin::addComplexNumber), &ComplexPlugin::addComplexNumber>("addComplexNumber");
		}

		int64_t getIntAvg(const list_t &list)
		{
			if(!list.size()) return 0;

			int64_t sum = 0;
			for(uint32_t i = 0; i < list.size(); ++i)
			{
				sum += get<int64_t>(list[i]);	
			}
			return sum/list.size();
		}

		double getFloatStd(const list_t &list)
		{
			if(!list.size()) return 0;

			double sum = 0.0;
			for(uint32_t i = 0; i < list.size(); ++i)
			{
				sum += get<double>(list[i]);
			}
			double mean = sum/list.size();
			double sum_std = 0.0;
			for(uint32_t i = 0; i < list.size(); ++i)
			{
				double diff = get<double>(list[i]) - mean;
				sum_std += diff * diff;
			}
			return sqrt(sum_std/list.size());
		}

		bool getBoolAnd(const list_t &list)
		{
			if(!list.size()) return false;

			bool andVal = get<bool>(list[0]);	
			for(uint32_t i = 1; i < list.size(); ++i)
			{
				andVal = andVal && get<bool>(list[i]);
			}
			return andVal;
		}

		std::string getStrCat(const list_t &list)
		{
			if(!list.size()) return "";

			std::string strCat = "";
			for(uint32_t i = 0; i < list.size(); ++i)
			{
				strCat += get<const char*>(list[i]); 
			}
			return strCat;
		}

		custom_t<ComplexNumber> makeComplexNumber(double real, double imag)
		{
			return custom_t<ComplexNumber>(new ComplexNumber(real, imag));
		}

		custom_t<ComplexNumber> makeComplexNumber()
		{
			return custom_t<ComplexNumber>(new ComplexNumber());
		}

		double getRealVal(const custom_t<ComplexNumber> &number)
		{
			return number->getReal();
		}

		double getImagVal(const custom_t<ComplexNumber> &number)
		{
			return number->getImag();
		}

		void setRealVal(const custom_t<ComplexNumber> &number, const double real)
		{
			number->setReal(real);
		}

		void setImagVal(const custom_t<ComplexNumber> &number, const double imag)
		{
			number->setImag(imag);
		}

		void addComplexNumber(const custom_t<ComplexNumber> &number, const custom_t<ComplexNumber> &nSrc)
		{
			number->addNumber(nSrc.get());
		}

		class GetDataTType : public com::softwareag::connectivity::const_visitor<GetDataTType, std::string>
		{
			public:
				std::string visitEmpty() const { return "(null)"; }
				std::string visitBoolean(bool &) const { return "boolean"; }
				std::string visitDouble(double &) const { return "double"; }
				std::string visitInteger(int64_t &) const { return "integer"; }
				std::string visitDecimal(const decimal_t &) const { return "decimal"; }
				std::string visitString(const char *&) const { return "string"; }
				std::string visitBuffer(const com::softwareag::connectivity::buffer_t &) const { return "buffer"; }
				std::string visitCustom(const sag_underlying_custom_t &) const { return "custom"; }
				
				std::string visitList(const com::softwareag::connectivity::list_t &l) const {
					std::ostringstream rv;
					rv << "[";
					for (auto it = l.begin(); it != l.end(); ++it) {
						if(it != l.begin()) rv << ",";
						rv << apply_visitor(*this, *it);
					}
					rv << "]";
					return rv.str();
				}
				
				std::string visitMap(const com::softwareag::connectivity::map_t &m) const  {
					std::ostringstream rv;
					const char* name = m.getName();
					if (0 != name) {
						rv << name;
						rv << ":";
					}
					rv << "{";
					for (auto it = m.begin(); it != m.end(); ++it) {
						if(it != m.begin()) rv << ",";
						rv << apply_visitor(*this, it.key());
						rv << ":";
						rv << apply_visitor(*this, it.value());
					}
					rv << "}";
					return rv.str();
				}
		};

		std::string getTypes(const data_t &data)
		{
			return apply_visitor(GetDataTType(), data);
		}
};
// The following macro must be used to export the plugin. 
APAMA_DECLARE_EPL_PLUGIN(ComplexPlugin)
