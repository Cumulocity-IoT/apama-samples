/**
 * complex_plugin.cpp 
 *
 * An example plugin that demonstrates:
 * - sending events from the plugin to the correlator by specifying the context or channel name
 * - receiving events from the correlator by registering an event handler
 *
 * $Copyright (c) 2016, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 * 
 */
#include <epl_plugin.hpp>
#include <string>

using namespace com::apama::epl;

/**
 * The plugin must be inherited from the EPLPlugin 
 * which needs to be templated over the derived class.
 */
class EventsPlugin: public EPLPlugin<EventsPlugin>
{
	public:
		// The plugin must implement zero-args constructor.
		EventsPlugin(): base_plugin_t("EventsPlugin"){} 
		~EventsPlugin(){}

		/**
		 * The plugin class must provide the static initialize method that registers
		 * all the methods needed to be visible to the EPL.
		 * Use custom signature if the arguments or the return type contain 
		 * int64_t(for passing context objects)
		 */
		static void initialize(base_plugin_t::method_data_t &md)
		{
			md.registerMethod<decltype(&EventsPlugin::sendDoneEvent), &EventsPlugin::sendDoneEvent>("sendDoneEvent", "action<context>");
			md.registerMethod<decltype(&EventsPlugin::sendSimpleEvent), &EventsPlugin::sendSimpleEvent>("sendSimpleEvent");
			md.registerMethod<decltype(&EventsPlugin::sendComplexEvent), &EventsPlugin::sendComplexEvent>("sendComplexEvent");
			md.registerMethod<decltype(&EventsPlugin::subscribe), &EventsPlugin::subscribe>("subscribe");
		}

		// Definition of the plugin event handler 
		class Handler: public EventHandler
		{
			public:
				Handler(EventsPlugin &self): self(self) {}
				void handleEvent(const char *type, data_t &&event, const char *channel)
				{
					self.logger.crit("EventPlugin received event %s on channel %s.", type, channel);
				}
				EventsPlugin &self;
		};

		/**
		 * The EPLPlugin base class provides registerEventHandler() function 
		 * which can be used to register an event handler
		 * on a given channel and a given mode (STRING or MAP).
		 */
		void subscribe(const char *source)
		{
			getCorrelator().registerEventHandler(Handler::ptr_t(new Handler(*this)), source, CorrelatorInterface::MAP_MODE);
		}

		void sendDoneEvent(int64_t ctx)
		{
			/**
			 * The EPLPlugin base class provides access to the correlator 
			 * by using getCorrelator(). 
			 */

			// Send the event by using sendEventTo
			getCorrelator().sendEventTo("Done()", ctx);
		}

		void sendSimpleEvent(const char *channel)
		{
			std::string evt = "Simple(\"Simple event sent from EventsPlugin\")";
			getCorrelator().sendEventTo(evt, channel);
		}

		void sendComplexEvent(const char *channel)
		{
			
			// Complex events can be sent in map form.
			map_t evt;
			evt["i"] = int64_t(4);
			evt["f"] = double(5.43);
			// Booleans must use an explicit data_t constructor
			evt["b"] = data_t(false);
			evt["s"] = "Complex event sent from EventsPlugin";

			// Send the complex event
			getCorrelator().sendEventTo("Complex", std::move(evt), channel);
		}
};
// The following macro must be used to export the plugin. 
APAMA_DECLARE_EPL_PLUGIN(EventsPlugin)
