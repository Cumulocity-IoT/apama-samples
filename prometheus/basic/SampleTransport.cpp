/*
 * Title:        SampleTransport.cpp
 * Description:  Prometheus Sample Transport
 * $Copyright (c) 2018 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 */

#include <sag_connectivity_plugins.hpp>


using namespace com::softwareag::connectivity;

namespace com {
namespace apama {
namespace samples {

/**
 *	This simple transport will merely create a user defined status and increment it whenever it receives a message from EPL
 */
class SampleTransport : public AbstractSimpleTransport
{
public:
	SampleTransport(const TransportConstructorParameters &params)
		: AbstractSimpleTransport(params), 
		// create a user status item - this will automatically be exposed to Prometheus
		messages_received(getStatusReporter().createStatusItem(pluginName + "_messages_received", 0)) 
	{}

	virtual void deliverMessageTowardsTransport(Message &m)
	{
		// increment our user status item
		messages_received->increment();
	}

private:
	// our user status item
	StatusReporter::item_ptr messages_received;
};

/** Export this transport */
SAG_DECLARE_CONNECTIVITY_TRANSPORT_CLASS(SampleTransport)

}}}