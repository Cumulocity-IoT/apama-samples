/*
 * $Copyright (c) 2016-2017, 2022 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 */

package com.apama.samples;

import com.sun.net.httpserver.*;
import java.net.InetSocketAddress;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.util.List; 
import java.util.Scanner;
import java.util.Arrays;
import java.util.Collections; 
import com.softwareag.connectivity.AbstractTransport;
import com.softwareag.connectivity.Message;
import com.softwareag.connectivity.PluginConstructorParameters.TransportConstructorParameters;
import com.softwareag.connectivity.StatusReporter;
import com.softwareag.connectivity.StatusReporter.StatusItem;
import com.softwareag.connectivity.util.MapExtractor;

/**
 * A minimalist HTTP server. Primarily accepts 'PUT' requests and turns them into hostwards messages. HTTP headers are put into the
 * metadata, and the request body is put into the payload as a string.
 *
 * Uses the configuration key 'port' to find out what port to bind to.
 *
 * It responds to 'GET' requests in a hardcoded fashion, serving up 'index.html' from this package in the JAR file if it exists - this is
 * only really used for demo purposes.
 *
 * UTF-8 character encoding is assumed throughout for simplicity, regardless of what the client wants.
 *
 * Demonstrates how status and KPIs (key performance indicator statistics) can be reported to the host. 
 * 
 * This is a sample that is not suitable for production use; it has not been stress-tested or made standards-compliant.
 */
public class HTTPServer extends AbstractTransport {
	/** The port that the server listens on, dictated by the 'port' element of configuration */
	private int port;
	private HttpServer srv;

	/**
	 * The HTML content served up in response to a GET request.
	 */
	private final String index_body;

	// Status monitoring and KPIs. See naming conventions described in documentation.  
	final StatusItem transportStatus = getStatusReporter().createStatusItem(chainId+"."+pluginName
			+".status", StatusReporter.STATUS_STARTING);
	
	final StatusItem messagesTowardsHost = getStatusReporter().createStatusItem(chainId+"."+pluginName
			+".messagesTowardsHost", 0);
	
	final StatusItem badRequests = getStatusReporter().createStatusItem(chainId+"."+pluginName
			+".badRequests", 0);
	
	final StatusItem kpis = getStatusReporter().createStatusItem(chainId+"."+pluginName
			+".KPIs", String.join(",", Arrays.asList(messagesTowardsHost.getKey(), badRequests.getKey())));
	
	/*
	 * Constructor
	 */
	public HTTPServer(org.slf4j.Logger logger, TransportConstructorParameters params) throws IllegalArgumentException, Exception {
		super(logger, params);

		// extract values from configurations
		MapExtractor config = new MapExtractor(params.getConfig(), "config");
		
		port = config.get("port", Integer.class);

		config.checkNoItemsRemaining();

		InputStream index_stream = this.getClass().getResourceAsStream("index.html");
		if(index_stream != null) {
			index_body = new Scanner(index_stream).useDelimiter("\\A").next();
			try { index_stream.close(); } catch(IOException e) {}
		} else {
			index_body = "";
		}
	}

	/* Bind the port and start listening for HTTP requests - we don't want the port to be open until the host application is initialised */
	@Override
	public void hostReady() throws IOException {
		srv = HttpServer.create(new InetSocketAddress(port), 0);
		srv.createContext("/", new HttpHandler() {
			@Override
			public void handle(HttpExchange e) {
				try {
					handleRequest(e);
				} catch(IOException bad) {
					logger.warn("Failed to handle a request: ", bad);
					badRequests.increment();
				} finally {
					e.close();
				}
			}
		});
		srv.start();
		logger.info("Listening on port " + port);
		transportStatus.setStatus(StatusReporter.STATUS_ONLINE);
	}
	
	/* Stop the httpserver by closing the listening socket and disallowing any new exchanges from being processed. */
	@Override
	public void shutdown() throws Exception {
		if (srv != null) {
			logger.info("Shutting down HttpServer on port " + port);
			srv.removeContext("/");
			//Close server immediately without any delay
			srv.stop(0);
			logger.info("HttpServer shutdown complete on port " + port);
		}
	}

	/*
	 * This transport is uni-directional, it does not accept messages from the host. If modified to support other request types that require
	 * a response from the host, one could write this method to actually do something
	 */
	@Override
	public void sendBatchTowardsTransport(List<Message> m) {
	}

	/** Turns a 'PUT' request into a message and sends it towards the host, or responds immediately to a 'GET' request */
	private void handleRequest(HttpExchange r) throws IOException {
		int responseCode = 200;
		String responseBody = "";

		if(r.getRequestMethod().equals("GET")) {
			// Return our built-in 'index.html' regardless of the URL - demo purposes only
			responseBody = index_body;
		} else if(!(r.getRequestMethod().equals("PUT"))) {
			// Send a 'Method not allowed' response - we only support 'PUT' and 'GET'
			responseCode = 405;
			badRequests.increment();
		} else {
			// Send a message towards the host for this 'PUT' request

			// Put the request of the body into the message payload
			String put_body;
			try (Scanner scanner = new Scanner(r.getRequestBody(), "UTF-8")) {
				put_body = scanner.useDelimiter("\\A").next();
			}
			Message m = new Message(put_body);
			// Set metadata based on HTTP metadata ...
			m.putMetadataValue("http.url", r.getRequestURI().toString()).putMetadataValue("http.method", "PUT");
			// ... and all of the other HTTP headers
			Headers hdrs = r.getRequestHeaders();
			for(String key : hdrs.keySet()) {
				m.putMetadataValue("http.header." + key, hdrs.get(key).get(0));
			}

			// Send the message hostwards
			// Has to be synchronized because the HttpServer implementation is multi-threaded, and the connectivity API asks that you do not
			// send messages simultaneously on multiple threads
			synchronized(hostSide) {
				hostSide.sendBatchTowardsHost(Collections.singletonList(m));
			}
			messagesTowardsHost.increment();
		}

		// Respond to the HTTP client
		r.sendResponseHeaders(responseCode, responseBody.length());
		OutputStreamWriter bodyWriter = new OutputStreamWriter(r.getResponseBody(), "UTF-8");
		bodyWriter.write(responseBody);
		bodyWriter.close();

		// 'access.log'-style logging of all requests and their outcome
		if (logger.isDebugEnabled())
			logger.debug(r.getRemoteAddress().getHostString() + " " + r.getRequestMethod() + " " + r.getRequestURI() + " " + responseCode);
	}
	
	/** Identifies the version of the API this plug-in was built against. */
	public static final String CONNECTIVITY_API_VERSION = com.softwareag.connectivity.ConnectivityPlugin.CONNECTIVITY_API_VERSION;

}
