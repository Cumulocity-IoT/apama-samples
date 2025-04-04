/**
* $Copyright (c) 2017, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
* Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
*/

#include <sag_connectivity_plugins.hpp>
#include <sag_connectivity_chain_managers.hpp>
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <map>

using namespace com::softwareag::connectivity;
using namespace com::softwareag::connectivity::chainmanagers;

namespace my_namespace {

class MyTransportChainManager; // forward declaration

/** Sample transport connectivity plug-in class, demonstrating use of chain manager API. 
 */
class MyTransport : public AbstractSimpleTransport 
{
public:
	MyTransport(const PluginConstructorParameters::TransportConstructorParameters &params, 
		Direction direction
	) : AbstractSimpleTransport(params), direction(direction) 
	{
	}

	virtual void start() 
	{
		logger.debug("Transport instance started.");
	}

	virtual void shutdown() 
	{
		logger.debug("Transport instance shutdown.");
	}

	virtual void hostReady() 
	{
		logger.debug("Transport instance hostReady.");

		// Simulate a message sent towards the host for demonstration purposes
		// a real transport would not do this				
		if (direction == Direction::TOWARDS_HOST) { 
			hostSide->sendBatchTowardsHost(
				Message{map_t{ std::make_pair("s", "Hello World") }}
			); 
		}
	}

	virtual void deliverMessageTowardsTransport(Message &m) 
	{
		map_t &payload = get<map_t>(m.getPayload());
		const char* &str = get<const char*>(payload["s"]);
		logger.info("Sending message towards transport: %s", str );
	}

private:
	Direction direction;
};

/** Sample transport chain manager connectivity plug-in class, demonstrating use of chain manager API. 
 */
class MyTransportChainManager : public AbstractChainManager<MyTransport>, public ChannelLifecycleListener 
{
public:
		
	MyTransportChainManager(const ChainManagerConstructorParameters &params)
		: AbstractChainManager(params) 
	{
		// TODO: read chain manager configuration
		// TODO: optionally add some status reporter items to indicate whether connection is online and provide some statistics
		MapExtractor configEx(config, "config");
		channelPrefix = configEx.getStringDisallowEmpty("channelPrefix");
		configEx.checkNoItemsRemaining();

	}		

	virtual void onChannelCreated(const std::string &channel, Direction direction) 
	{
		// TODO: some transports may wish to block here (for a limited time period) 
		// until a channel with the specified name exists on the external system, 
		// in which case they may wish to pass a reference to the 
		// object representing that channel into the new transport instance when calling createChain below. 
			
		std::unique_lock<std::mutex> lock(mutex);

		// Need to check if a chain already exists for this channel and direction
		auto it = chains.find(std::make_pair(channel, direction));
		if (it != chains.end())
		{
			// chain for this channel and direction already exists, so no need to create a new one

			// TODO: some transports may wish to re-create the chain at this point if the configuration 
			// of the channel on the external system has changed since the chain was first created
			return;
		}

		// Select a chain definition. 
		// TODO: select desired chain definition. 
		// Some transports only permit a singleton chain definition, others use the dynamic chain 
		// definition identifier or something in the transport's configuration to select the right 
		// chain
		ChainDefinition chainDefinition = getChainDefinition(); // get singleton chain definition

		// Create a new chain, with a dedicated transport instance for each (channel, direction) pair. 
		std::string chainInstanceId = ((direction == Direction::TOWARDS_TRANSPORT) ? std::string("ToTransport") : std::string("FromTransport")) + "[" + channel + "]";
		map_t substitutions;		// use this to provide @{...} substitution variables for use in the dyanmic chain's configuration
		list_t subscribeChannels;

		if (direction == Direction::TOWARDS_TRANSPORT) {
			subscribeChannels.push_back(channel);
		}

		chain_t chain = host.createChain(chainInstanceId,
			channel,
			subscribeChannels,
			chainDefinition,
			substitutions,
			// any following arguments are passed to the transport constructor 
			// (whose signature must contain items with the same types), 
			direction
		);
		chain->start();
		chains.insert(std::make_pair(std::make_pair(channel, direction), chain));

	}

	virtual void onChannelDestroyed(const std::string &channel, Direction direction) 
	{
		std::unique_lock<std::mutex> lock(mutex);

		auto it = chains.find(std::make_pair(channel, direction));
		if (it == chains.end()) return;

		chain_t chain = it->second;
		chains.erase(it);
		lock.unlock();
	}

	void start() 
	{
		logger.debug("Starting MyTransportChainManager");
		host.addChannelLifecycleListener(this, channelPrefix);
		// TODO: this is typically where a connection to the external transport would be established, optionally on a background thread
		logger.debug("MyTransportChainManager started");
	}

	void shutdown() 
	{
		// best practice is to explicitly destroy all chains as part of the host shutdown
		std::unique_lock<std::mutex> lock(mutex);
		std::map<std::pair<std::string, Direction>, chain_t> localChains{std::move(chains)};
        lock.unlock(); // don't hold the lock while calling chain->destroy
		for (auto it = localChains.begin(); it != localChains.end(); it++) {
			chain_t chain = it->second;
			chain->destroy();
		}
		chains.clear();

		// TODO: after the chains have been destroyed is a good place to disconnect from the external system
	}

private:
	std::map<std::pair<std::string, Direction>, chain_t> chains;
	std::mutex mutex;
	std::string channelPrefix;
}; // end of chain manager

SAG_DECLARE_CONNECTIVITY_TRANSPORT_CHAIN_MANAGER_CLASS(MyTransportChainManager)

} // end of namespace

