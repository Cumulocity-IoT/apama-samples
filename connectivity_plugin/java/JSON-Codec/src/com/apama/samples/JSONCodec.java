/**
 * $Copyright (c) 2016-2017, 2019, 2020, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 */

package com.apama.samples;

import java.io.IOException;
import java.util.Map;
import java.util.HashMap;

import com.softwareag.connectivity.AbstractSimpleCodec;
import com.softwareag.connectivity.Message;
import com.softwareag.connectivity.PluginConstructorParameters.CodecConstructorParameters;
import com.softwareag.connectivity.util.MapExtractor;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;


/**
 * Codec that converts in both directions between JSON documents on the transport side and Object representations of the JSON on the host
 * side.
 *
 * The JSON document will be a String in the payload of the Message from/to the transport direction. The Object representation will be in
 * the payload of the Message from/to the host direction. Typically that Object will be a Map that contains other Maps, Lists, and
 * primitives such as Integer and String.
 *
 * Any parsing or generation errors will come out of the transform methods as an exception, resulting in the host writing the exception to
 * the log, and the Message being dropped.
 */
public class JSONCodec extends AbstractSimpleCodec
{
	private final ObjectMapper mapper = new ObjectMapper();
	private boolean filterOnContentType = false;
	
	/** The metadata key for content type value.*/
	private static final String CONTENT_TYPE_KEY = "contentType";
	/** The content type Value for JSON.*/
	private static final String CONTENT_TYPE_JSON = "application/json";
	
	public JSONCodec(org.slf4j.Logger logger, CodecConstructorParameters params) throws Exception, IllegalArgumentException
	{
		super(logger, params);
		MapExtractor config = new MapExtractor(params.getConfig(), "configuration");
		filterOnContentType = config.get("filterOnContentType", false);
		config.checkNoItemsRemaining();
	}
	
	@Override
	public Message transformMessageTowardsHost(Message message) throws JsonProcessingException, IOException
	{
		if (filterOnContentType) {
			Map<String, Object> metadata = message.getMetadataMap();
			if (!CONTENT_TYPE_JSON.equals(metadata.getOrDefault(CONTENT_TYPE_KEY, "")))
			{
				logger.debug("Skipping message towards host because it's not application/json");
				return message;
			}
		}
		
		// Take the String payload and pass it into the JSON parser, turning it into a java Object
		if (!(message.getPayload() instanceof String)) {
			throw new RuntimeException("Hostward message to JSONCodec must be a string, not a " + message.getPayload().getClass().getName());
		}
		String payload = (String)message.getPayload();
		if(payload.isEmpty()) { 
			message.setPayload(new HashMap<>());
		}
		else {
			message.setPayload(mapper.readValue(payload, new TypeReference<Object>(){}));
		}
		return message;
	}

	@Override
	public Message transformMessageTowardsTransport(Message message) throws JsonProcessingException
	{
		if (filterOnContentType) {
			Map<String, Object> metadata = message.getMetadataMap();
			if (!CONTENT_TYPE_JSON.equals(metadata.getOrDefault(CONTENT_TYPE_KEY, "")))
			{
				logger.debug("Skipping message towards transport because it's not application/json");
				return message;
			}
		}

		// Take the Object payload and pass it into the JSON parser to turn it into a JSON string
		String ret = mapper.writeValueAsString(message.getPayload());
		message.putMetadataValue(CONTENT_TYPE_KEY, CONTENT_TYPE_JSON);
		message.setPayload(ret);
		return message;
	}
	
	/** Identifies the version of the API this plug-in was built against. */
	public static final String CONNECTIVITY_API_VERSION = com.softwareag.connectivity.ConnectivityPlugin.CONNECTIVITY_API_VERSION;

}