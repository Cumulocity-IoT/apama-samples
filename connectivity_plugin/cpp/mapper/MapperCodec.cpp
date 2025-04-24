/**
 * Title:        MapperCodec.cpp
 * Description:  Mapper Codec
 * $Copyright (c) 2016-2018, 2020-2023 Cumulocity GmbH, Duesseldorf, Germany and/or its affiliates and/or their licensors.$
 * Use, reproduction, transfer, publication or disclosure is prohibited except as specifically provided for in your License Agreement with Cumulocity GmbH.
 */

#include <sag_connectivity_plugins.hpp>
#include <vector>
#include <map>
#include <cstdint>
#include <memory>

using namespace com::softwareag::connectivity;

namespace com {
namespace apama {
namespace samples {

/**
 * A codec which can move fields around within and between the payload and the metadata,
 * depending on configuration from the yaml file.
 */
class MapperCodec: public AbstractSimpleCodec
{
public:
	/// Construct a Mapper codec, parsing the instructions from the config file
	MapperCodec(const CodecConstructorParameters &params);

	/// Transform a message going from the host to the transport
	void transformMessageDirection(Message &m, bool direction)
	{
		metadata_field_t currentFieldCount;
		map_t::iterator mit;
		fields_t::iterator fit;

		// apply rules with fieldCount of particular type if the type exists.
		// type in map fields contains total (including "*") count of the target fields.
		if ((mit = m.getMetadataMap().find("sag.control.type")) != m.getMetadataMap().end() && ignoreControlMessages) {
			return; // ignore control messages
		} else if ((mit = m.getMetadataMap().find("sag.type")) != m.getMetadataMap().end() && (fit = fields.find(get<const char*>(mit.value()))) != fields.end()) {
			applyRules(get<const char*>(mit.value()), direction, m, fit->second[direction], currentFieldCount);
		}
		applyRules("*", direction, m, fields["*"][direction], currentFieldCount);
	}

	virtual bool transformMessageTowardsTransport(Message &m)
	{
		transformMessageDirection(m, true);
		return true; // always keep the message
	}
	/// Transform a message going from the transport to the host
	virtual bool transformMessageTowardsHost(Message &m)
	{
		transformMessageDirection(m, false);
		return true; // alwaps keep the message
	}
	/// Also need to handle null payloads the same as normal payloads
	virtual bool transformNullPayloadTowardsTransport(Message &m)
	{
		try {
			return transformMessageTowardsTransport(m);
		} catch(const std::exception &e) {
			logger.debug("Ignoring exception from null-payload message: %s", e.what());
			// null payload message may fail to map, in which case just pass them through as-is
			return true;
		}
	}
	/// Also need to handle null payloads the same as normal payloads
	virtual bool transformNullPayloadTowardsHost(Message &m)
	{
		try {
			return transformMessageTowardsHost(m);
		} catch(const std::exception &e) {
			logger.debug("Ignoring exception from null-payload message: %s", e.what());
			// null payload message may fail to map, in which case just pass them through as-is
			return true;
		}
	}
private:

	/* New Map is defined which will store the total count of target field. */
	typedef std::map<std::string, std::int64_t> field_count_t;
	typedef std::map<bool, field_count_t> metadata_field_t;
	typedef std::map<bool, metadata_field_t> command_field_t;
	typedef std::map<std::string, command_field_t> fields_t;

	class Rule;
	/// The different operations we support
	enum command_mode {
		MAP_FROM,
		DEFAULT_VALUE,
		COPY_FROM,
		FOR_EACH,
		SET
	};
	// convenience typedefs
	typedef std::vector<std::shared_ptr<Rule> > rule_list_t;
	typedef std::map<command_mode, rule_list_t> rules_t;
	typedef std::map<bool, rules_t > command_t;
	typedef std::map<std::string, command_t > types_t;

	/// Return the string form of the mode
	static std::string getModeName(command_mode mode)
	{
		switch(mode) {
			case MAP_FROM: return "mapFrom";
			case COPY_FROM: return "copyFrom";
			case DEFAULT_VALUE: return "defaultValue";
			case FOR_EACH: return "forEach";
			case SET: return "set";
			default: assert(false); return "unknown";
		}
	}

	/// Apply all of the rules for a particular type in the given direction to m
	void applyRules(const char *typeName, bool transportwards, Message &m, metadata_field_t &totalFieldCount, metadata_field_t &currentFieldCount)
	{
		// find the rules for this type and direction
		rules_t &rules = types[typeName][transportwards];
		applyRulesImpl(rules, [this, &m, &totalFieldCount, &currentFieldCount](std::shared_ptr<Rule> &rule) {
				rule->apply(m, totalFieldCount, currentFieldCount, allowMissing);
			});
	}
	template<typename T>
	static void applyRulesImpl(rules_t &rules, const T &applicator)
	{
		// first do all the COPY_FROM rules
		for (auto &rule: rules[COPY_FROM]) {
			applicator(rule);
		}

		// then do all the MAP_FROM rules
		for (auto &rule: rules[MAP_FROM]) {
			applicator(rule);
		}

		// then do all FOR_EACH rules
		for (auto &rule: rules[FOR_EACH]) {
			applicator(rule);
		}

		// then do all the SET rules
		for (auto &rule: rules[SET]) {
			applicator(rule);
		}

		// then do all the DEFAULT_VALUE rules last
		for (auto &rule: rules[DEFAULT_VALUE]) {
			applicator(rule);
		}
	}

	/** Iterating through the "*" typeName and adding the "*" values to those of other types.
	 *
	 *(we walk the type-specific rules, then the "*" rules). It will be done once while constructing mapper
	 * codec. */
	void computingTotalFieldCount() {
		command_field_t &fieldstoApply = fields["*"];
		// Looping over the fields map with "*" type and adding the values in other types.
		for (command_field_t::iterator init_it = fieldstoApply.begin(); init_it != fieldstoApply.end(); ++init_it) {
			for (metadata_field_t::iterator it = init_it->second.begin(); it != init_it->second.end(); ++it){
				for (field_count_t::iterator second_it = it->second.begin(); second_it != it->second.end(); ++second_it){
					for (fields_t::iterator third_it = fields.begin(); third_it != fields.end(); ++third_it) {
						if (third_it->first != "*"){
							fields[third_it->first][init_it->first][it->first][second_it->first] += second_it->second;
						}
					}
				}
			}
		}
	}
	/// Holds a single rule to apply to a message
	class Rule
	{
	public:
		/// Construct a rule, parsing the actions from the mode and names.
		/// typeName is just provided for useful error messages
		Rule(const std::string &typeName, command_mode mode, const std::string &fieldName, data_t&& resultData, const std::string &parentFieldName, metadata_field_t &fieldData);
		Rule(const std::string &typeName, command_mode mode, const std::string &fieldName, data_t&& resultData, const std::string &parentFieldName, metadata_field_t &fieldData, rules_t &&childrules, metadata_field_t &&childfieldCounts);

		~Rule() = default;
		Rule(const Rule &) = delete;
		Rule &operator=(const Rule &) = delete;
		Rule(Rule &&other)
			: mode(other.mode),
			tometadata(other.tometadata),
			frommetadata(other.frommetadata),
			targetField(std::move(other.targetField)),
			sourceField(std::move(other.sourceField)),
			content(std::move(other.content)),
			childRules(std::move(other.childRules)),
			childFieldCounts(std::move(other.childFieldCounts))
		{
		}
		Rule &operator=(Rule &&other)
		{
			mode = other.mode;
			tometadata = other.tometadata;
			frommetadata = other.frommetadata;
			targetField = std::move(other.targetField);
			sourceField = std::move(other.sourceField);
			content = std::move(other.content);
			childRules = std::move(other.childRules);
			childFieldCounts = std::move(other.childFieldCounts);
			return *this;
		}

		/// Apply this rule to a message. May throw if trying to read from a non-existant field.
		/// May overwrite the payload if trying to write to a map-value in a non-map payload
		void apply(Message &m, metadata_field_t &totalFieldCount, metadata_field_t &fieldToCompare, bool &allowMissing);
	private:

		/// handle forEach operation
		void handleForEach(map_t &data, bool &allowMissing, const std::string& contentType);
		/// Apply this rule. May throw if trying to read from a non-existant field.
		void apply(map_t &data, metadata_field_t &totalFieldCount, metadata_field_t &currentFieldCount, bool &allowMissing, const std::string& contentType);

		/** Find a field entry.
		 * If insert is false and the field cannot be found, returns (data_, data_.end()). Splits the field by
		 * '.' to look up sub-maps.
		 */
		std::pair<map_t*, map_t::iterator> findField(map_t &data_, const std::string &field, bool insert, bool metadata);

		command_mode mode;
		bool tometadata;
		bool frommetadata;
		std::string targetField;
		std::string sourceField;
		data_t content;

		rules_t childRules;
		metadata_field_t childFieldCounts;
	};

	/// Holds the parsed config which will be used to map messages
	types_t types;
	/* Holds the target field count - for each type/direction/metadata, a count of how many rules target each
	 * field.  If we reach the last mapping rule for a target without it being filled (and allowMissing is not
	 * true), we have an error*/
	fields_t fields;
	/* Required to check if allowMissing is set by user */
	bool allowMissing;
	/* Required to check if ignoreControlMessages is set by the user */
	bool ignoreControlMessages; 

	/// Helper method for populating rules
	void populateRules(rules_t &rules, metadata_field_t &fieldCounts, const std::string &typeName, const std::string &key, const data_t &configMap, const std::string &parentFieldName="");
};

/// Export this codec to the container
SAG_DECLARE_CONNECTIVITY_CODEC_CLASS(MapperCodec)

/// Construct a Mapper codec, parsing the instructions from the config file
MapperCodec::MapperCodec(const CodecConstructorParameters &params)
	: AbstractSimpleCodec(params)
{
	// iterate over all the config items
	// whether to apply mapping rules to control messages or not.
	ignoreControlMessages = false;
	// allowMissing to decide if non-existing soure field is allowed or not.
	allowMissing = false;
	for (auto &item: config) {
		// get the type name or *
		std::string typeName = get<const char*>(item.first);
		
		if ("ignoreControlMessages" == typeName) {
			std::string ignoreControlMessagesValue = get<const char*>(item.second);
			std::transform(ignoreControlMessagesValue.begin(), ignoreControlMessagesValue.end(), ignoreControlMessagesValue.begin(), ::tolower);
			if ("true" == ignoreControlMessagesValue) {
				ignoreControlMessages = true;
			} else if ("false" != ignoreControlMessagesValue){
				throw std::runtime_error("Unknown value in map for ignoreControlMessages. It should be either 'true' or 'false'");
			}
			continue;
		}

		if ("allowMissing" == typeName) {
			std::string allowMissingValue = get<const char*>(item.second);
			std::transform(allowMissingValue.begin(), allowMissingValue.end(), allowMissingValue.begin(), ::tolower);

			if ("true" == allowMissingValue) {
				allowMissing = true;
			} else if ("false" != allowMissingValue){
				throw std::runtime_error("Unknown value in map for allowMissing. It should be either 'true' or 'false'");
			}
			continue;
		}

		// must be a map
		if (SAG_DATA_MAP != item.second.type_tag()) throw std::runtime_error("Config for type "+typeName+" is not a map");
		const map_t &typeConfig = get<map_t>(item.second);

		// iterate over the direction rules for this type
		for (auto &type: typeConfig) {

			// it must be towardsHost or towardsTransport
			std::string key = get<const char*>(type.first);
			bool transportwards;
			if ("towardsHost" == key) {
				transportwards = false;
			} else if ("towardsTransport" == key) {
				transportwards = true;
			} else {
				throw std::runtime_error("Unknown key in map for "+typeName+" should only contain 'towardsHost' or 'towardsTransport' (not "+key+")");
			}
			populateRules(types[typeName][transportwards], fields[typeName][transportwards], typeName, key, type.second);
		}
	}
	//Merging the '*' with all the other field Names.
	computingTotalFieldCount();
}

void MapperCodec::populateRules(rules_t &rules, metadata_field_t &fieldCounts, const std::string& typeName, const std::string& key, const data_t& mapperConfig, const std::string& parentFieldName) {

	// must be a map
	if (SAG_DATA_MAP != mapperConfig.type_tag()) throw std::runtime_error("Config "+key+" for type "+typeName+" is not a map");
	const map_t &directionConfig = get<map_t>(mapperConfig);

	// iterate over each set of commands in this direction
	for (auto &dir: directionConfig) {

		// it must be mapFrom or defaultValue
		std::string command = get<const char*>(dir.first);
		command_mode cmode;
		if ("copyFrom" == command) {
			cmode = COPY_FROM;
		} else if ("mapFrom" == command) {
			cmode = MAP_FROM;
		} else if ("set" == command) {
			cmode = SET;
		} else if ("defaultValue" == command) {
			cmode = DEFAULT_VALUE;
		} else if ("forEach" == command) {
			cmode = FOR_EACH;
		} else {
			throw std::runtime_error("Unknown key in map for "+typeName+" ("+key+") should only contain 'copyFrom', 'mapFrom', 'set', 'forEach' or 'defaultValue' (not "+command+")");
		}

		// must be a list
		if (SAG_DATA_LIST != dir.second.type_tag()) throw std::runtime_error("Config "+key+"."+command+" for type "+typeName+" is not a list");
		const list_t &actions = get<list_t>(dir.second);

		// iterate over all the actions
		for (auto &act: actions) {

			// it must be a key-value pair
			if (SAG_DATA_MAP != act.type_tag()) throw std::runtime_error("Actions in "+key+"."+command+" for type "+typeName+" must be a single-entry map");
			const map_t &action = get<map_t>(act);
			if (1 != action.size()) throw std::runtime_error("Actions in "+key+"."+command+" for type "+typeName+" must be a single-entry map");

			// the value must be a string
			std::string fieldName = get<const char*>(action.begin()->first);
			if (cmode != FOR_EACH) {
				// add the rule to our table of rules
				fieldName = (parentFieldName.size() == 0) ? fieldName : (parentFieldName + "." + fieldName);
				rules[cmode].push_back(std::make_shared<Rule>(typeName, cmode, fieldName, action.begin()->second.copy(), parentFieldName, fieldCounts));
			} else {
				std::string childTypeName = typeName + "." + fieldName;
				std::string childFieldName = (parentFieldName.size() == 0) ? fieldName : (parentFieldName + "." + fieldName);
				rules_t cRules;
				metadata_field_t cFieldCounts;
				populateRules(cRules, cFieldCounts, childTypeName, key, action.begin()->second, childFieldName);
				/// Add a temporary Rule, applyRules will use <parent>.<child> sub rule for mapping forEach
				rules[cmode].push_back(std::make_shared<Rule>(typeName, cmode, childFieldName, action.begin()->first.copy(), parentFieldName, fieldCounts, std::move(cRules), std::move(cFieldCounts)));
			}
		}
	}
}

std::pair<map_t*, map_t::iterator> MapperCodec::Rule::findField(map_t &data, const std::string &field, bool insert, bool metadata)
{
	map_t::iterator it;
	if (metadata) {
		it = data.find(field);
		if (it != data.end()) {
			return std::pair<map_t*, map_t::iterator>(&data, it);
		}
	}
	if (metadata && field.substr(0, 4) == "sag.") {
		if (insert) {
			return std::pair<map_t*, map_t::iterator>(&data, data.insert(std::make_pair(field, data_t())).first);
		} else {
			return std::pair<map_t*, map_t::iterator>(&data, data.end());
		}
	}
	map_t *map = &data;
	size_t prevOffs = 0;
	size_t offs = 0;
	do {
		offs = field.find_first_of('.', prevOffs);
		std::string fieldName = field.substr(prevOffs, offs>0?offs-prevOffs:offs);
		it = map->find(fieldName);
		if (it == map->end()) {
			if (insert) {
				if (offs != std::string::npos) it = map->insert(std::make_pair(fieldName, map_t())).first;
				else it = map->insert(std::make_pair(fieldName, data_t())).first;
			} else {
				return std::pair<map_t*, map_t::iterator>(&data, data.end());
			}
		}
		if (offs != std::string::npos) {
			if (it->second.type_tag() != SAG_DATA_MAP) {
				if (insert) {
					it->second = map_t();
				} else {
					return std::pair<map_t*, map_t::iterator>(&data, data.end());
				}
			}
			map = &get<map_t>(it->second);
		}
		prevOffs = offs+1;
	} while (offs != std::string::npos);
	return std::pair<map_t*, map_t::iterator>(map, it);
}

/// Construct a rule, parsing the actions from the mode and names.
/// typeName is just provided for useful error messages
MapperCodec::Rule::Rule(const std::string &typeName, command_mode mode, const std::string &fieldName, data_t &&resultData, const std::string &parentFieldName, metadata_field_t &fieldData)
	: mode(mode)
{
	switch (mode) {

		// map-from-only , copy-from-only and for-each-only configuration
		case MAP_FROM:
		case COPY_FROM:
		case FOR_EACH: {
			if (SAG_DATA_STRING != resultData.type_tag()) throw std::runtime_error("Action " + fieldName + " in " + getModeName(mode) + " for type " + typeName + " is not a scalar");
			const std::string &result = (parentFieldName.size() == 0) ? get<const char*>(resultData) : (parentFieldName + "." + get<const char *>(resultData));

			// parse the source location
			// must be in the metadata or payload, parse out the actual field name.
			if (0 == strncmp(result.c_str(), "metadata.", 9)) { // metadata
				frommetadata = true;
				if (result.length() == 9) { // invalid
					throw std::runtime_error("metadata field name missing (you wrote: " + result + ") for " + typeName);
				}
				sourceField = result.substr(9);
			} else if (0 == strncmp(result.c_str(), "payload", 7)) { // payload
				frommetadata = false;
				if (result.length() == 7) { // reading whole payload
					sourceField = "";
				} else if (result.length() == 8) { // invalid
					throw std::runtime_error("payload field name missing (you wrote: " + result + ") for " + typeName);
				} else { // reading a field from the payload
					sourceField = result.substr(8);
				}
			} else { // invalid
				throw std::runtime_error("rules must start with 'metadata.' or 'payload' (not: " + result + ") for " + typeName);
			}
			// no break;
		}
			// shared configuration (fallthrough)
		case SET:
		case DEFAULT_VALUE: {
			// parse the target location
			// must be in the metadata or payload, parse out the actual field name.
			if (0 == strncmp(fieldName.c_str(), "metadata.", 9)) { // metadata
				tometadata = true;
				if (fieldName.length() == 9) { // invalid
					throw std::runtime_error("metadata field name missing (you wrote: " + fieldName + ") for " + typeName);
				}
				targetField = fieldName.substr(9);
			} else if (0 == strncmp(fieldName.c_str(), "payload", 7)) { // payload
				tometadata = false;
				if (fieldName.length() == 7) { // set whole payload
					targetField = "";
					if (SAG_DATA_STRING != resultData.type_tag() && SAG_DATA_MAP != resultData.type_tag()) {
						throw std::runtime_error("Action " + fieldName + " in " + getModeName(mode) + " for type " + typeName + " is not a string or map");
					}
				} else if (fieldName.length() == 8) { // invaled
					throw std::runtime_error("payload field name missing (you wrote: " + fieldName + ") for " + typeName);
				} else if (fieldName.at(7) != '.') { // invaled
					throw std::runtime_error("payload delimiter is '.' (you wrote: " + fieldName + ") for " + typeName);
				} else { // set a field in the payload
					targetField = fieldName.substr(8);
				}
			} else { // invalid
				throw std::runtime_error("rules must start with 'metadata.' or 'payload' (not: " + fieldName + ") for " + typeName);
			}

			// applicable only in forEach or nested rules in a forEach expression, remove the prefix from source and targetFields
			if (parentFieldName.size() > 0)
			{
				size_t offsetLength = parentFieldName.size() - ( tometadata ? 8 : 7 );
				// sourceField will not be populated when its a defaultValue ... instead resultData will be used directly
				if (mode != DEFAULT_VALUE && mode != SET) {
					sourceField = sourceField.substr(offsetLength);
				}
				targetField = targetField.substr(offsetLength);
			}

			// We don't want to maintain fieldCount when processing forEach
			if (mode != FOR_EACH) {
				// Keeping the count of target fields.
				// Will use it later to check if the field is defined multiples times or not.
				field_count_t field_it;
				field_it = fieldData[tometadata];
				field_count_t::iterator targetField_it = field_it.find(targetField);

				if (targetField_it == field_it.end()) {
					fieldData[tometadata][targetField] = 1;
				} else{
					fieldData[tometadata][targetField] ++;
				}
			}
			content = std::move(resultData); // for defaultValue, the value to set it to
			break;
		}

		// it's an enum, really shouldn't happen
		default: throw std::runtime_error("Internal Error: unknown rule type");
	}
}

/// Construct a rule, parsing the actions from the mode and names.
/// typeName is just provided for useful error messages
MapperCodec::Rule::Rule(const std::string &typeName, command_mode mode, const std::string &fieldName, data_t &&resultData, const std::string &parentFieldName, metadata_field_t &fieldData, rules_t &&childrules, metadata_field_t &&childfieldCounts)
	: Rule(typeName, mode, fieldName, std::move(resultData), parentFieldName, fieldData)
{
	childRules = std::move(childrules);
	childFieldCounts = std::move(childfieldCounts);
}

/// Apply this rule to a message. May throw if trying to read from a non-existant field.
/// May overwrite the payload if trying to write to a map-value in a non-map payload
void MapperCodec::Rule::apply(Message &m, metadata_field_t &totalFieldCount, metadata_field_t &currentFieldCount, bool &allowMissing)
{
	switch (mode) {
		// apply a mapFrom rule
		case MAP_FROM:
		case COPY_FROM: {
			// first get the value (erasing it from the source if MAP_FROM)
			data_t value;
			if(!frommetadata && sourceField.empty()) {
				if (mode == MAP_FROM) {
					value = std::move(m.getPayload());
				} else if (mode == COPY_FROM) {
					value = m.getPayload().copy();
				} else {
					assert(false);
				}
			} else {
				std::string contentType = frommetadata ? "metadata" : "payload";

				// payload might not be a map, in which case there's no point looking in it:
				if (!frommetadata && (SAG_DATA_MAP != m.getPayload().type_tag())) {
					if (allowMissing) return;
					else throw std::runtime_error("Could not find "+contentType+"."+sourceField+" in message");
				}

				map_t &data = frommetadata ? m.getMetadataMap() : get<map_t>(m.getPayload());
				map_t::iterator it;
				map_t *itmap;
				std::tie(itmap, it) = findField(data, sourceField, false, frommetadata);

				if (it == itmap->end())
				{
					if (allowMissing == true){
						return;
					}

					field_count_t fieldCount_it = currentFieldCount[tometadata];
					field_count_t::iterator targetField_it = fieldCount_it.find(targetField);

					// Initially updating the currentFieldCount as soon as we get
					if (targetField_it == fieldCount_it.end()) {
						currentFieldCount[tometadata][targetField] = 1;
					} else  if (targetField_it->second != -1){
						currentFieldCount[tometadata][targetField]++;
					} else if (targetField_it->second == -1){
						// -1 symbolised that this target field has been previously set, so
						// not an error if we can't find a source to map from.
						return;
					}

					// Checking if it exceeded the totalFieldCount.
					if (currentFieldCount[tometadata][targetField] < totalFieldCount[tometadata][targetField]){
						// this rule has not found a value for this targetField, but maybe a
						// later rule will, so don't raise an error (yet)
						return;
					} else {
						// we have reached the last rule that could provide a value for the
						// targetField and none have, and allowMissing is false, so we have an
						// error.
						throw std::runtime_error("Could not find " + contentType + "." + sourceField + " in message");
					}
				}

				// if mode is MAP_FROM, erase the source
				if (mode == MAP_FROM) {
					// move the value out of the source
					value = std::move(it->second);
					itmap->erase(it);
				} else if (mode == COPY_FROM) {
					// just copy, don't remove it. Also no need to erase
					value = it->second.copy();
				} else {
					assert(false);
				}
			}

			// second, put it in the target
			if (tometadata) { // into the metadata
				findField(m.getMetadataMap(), targetField, true, tometadata).second->second = std::move(value);
			} else { // into the payload
				if (targetField.empty()) { // set the whole payload
					m.setPayload(std::move(value));
				} else { // set a payload field
					if (SAG_DATA_MAP != m.getPayload().type_tag()) { // need to make the payload into a map first
						map_t map;
						m.setPayload(std::move(map));
					}
					findField(get<map_t>(m.getPayload()), targetField, true, tometadata).second->second = std::move(value);
				}
			}
			// Marking that the current field value is applied once.
			currentFieldCount[tometadata][targetField] = -1;
			break;
		}
		// apply a defaultValue or set rule
		case SET:
		case DEFAULT_VALUE: {
			if (tometadata) { // set a metadata field
				auto rv = findField(m.getMetadataMap(), targetField, true, tometadata);
				// but only if it hasn't already been set or is the empty string, or it's a set rule
				if (SET == mode || rv.second->second.empty() || (rv.second->second.type_tag() == SAG_DATA_STRING && std::string(get<const char*>(rv.second->second)).empty())) {
					rv.second->second = content.copy();
				}
			} else { // set the payload
				if (targetField.empty()) { // set the whole payload
					// but only if it's empty or it's a set rule
					if (SET == mode || m.getPayload().empty()) m.setPayload(content.copy());
				} else { // set a value in the payload
					if (SAG_DATA_MAP != m.getPayload().type_tag()) { // need to make it a map first
						map_t map;
						m.setPayload(std::move(map));
					}
					// if the key exists in the map but is null, set it to the default val
					// else add the key and default to the map (insert does not clobber)
					auto rv = findField(get<map_t>(m.getPayload()), targetField, true, tometadata);
					if (SET == mode || rv.second->second.type_tag() == SAG_DATA_EMPTY) {
						rv.second->second = content.copy();
					}
				}
			}
			// Marking that the targetField value is applied once.
			currentFieldCount[tometadata][targetField] = -1;
			break;
		}
		// apply forEach rule
		case FOR_EACH: {
				std::string contentType = frommetadata ? "metadata" : "payload";
				map_t &data = frommetadata ? m.getMetadataMap() : get<map_t>(m.getPayload());
				handleForEach(data, allowMissing, contentType);
			break;
		}
		// it's an enum, really shouldn't happen
		default: throw std::runtime_error("Internal Error: unknown rule type");
	}
}

void MapperCodec::Rule::handleForEach(map_t &data, bool &allowMissing, const std::string& contentType)
{

	/// only consider targetField when iterating through a forEach rule
	/// forEach:
	///   - payload.listA:
	///       mapFrom:
	///         - ...
	map_t::iterator it;
	map_t *itmap;
	std::tie(itmap, it) = findField(data, targetField, false, tometadata);

	std::string fieldName = contentType + "." + targetField;

	if (it == itmap->end()) {
		//Unable to find source list field, don't do anything
		if (allowMissing)
			return;
		else
			throw std::runtime_error("Could not find " + fieldName + " in message");
	}

	if (SAG_DATA_LIST != it->second.type_tag()) throw std::runtime_error("Action " + fieldName +" is not a list");

	list_t& items = get<list_t>(it->second);

	for (auto jt = items.begin(); jt != items.end(); ++jt) {

		if (SAG_DATA_MAP != jt->type_tag()) throw std::runtime_error("Actions in " + fieldName + "[] must be a single-entry map");

		metadata_field_t currentFieldCount;
		applyRulesImpl(childRules, [this, &jt, &currentFieldCount, &allowMissing, &fieldName](std::shared_ptr<Rule> &rule) {
				rule->apply(get<map_t>(*jt), childFieldCounts, currentFieldCount, allowMissing, fieldName);
			});
	}
}

void MapperCodec::Rule::apply(map_t &data, metadata_field_t &totalFieldCount, metadata_field_t &currentFieldCount, bool &allowMissing, const std::string& contentType)
{
	switch (mode) {
		// apply a mapFrom rule
		case MAP_FROM:
		case COPY_FROM: {
			map_t::iterator it;
			map_t *itmap;
			std::tie(itmap, it) = findField(data, sourceField, false, tometadata);

			if (it == itmap->end())
			{
				if (allowMissing == true) {
					return;
				}

				field_count_t fieldCount_it = currentFieldCount[tometadata];
				field_count_t::iterator targetField_it = fieldCount_it.find(targetField);

				// Initially updating the currentFieldCount as soon as we get
				if (targetField_it == fieldCount_it.end()) {
					currentFieldCount[tometadata][targetField] = 1;
				} else  if (targetField_it->second != -1){
					currentFieldCount[tometadata][targetField]++;
				} else if (targetField_it->second == -1){
					// -1 symbolised that this target field has been previously set, so
					// not an error if we can't find a source to map from.
					return;
				}

				// Checking if it exceeded the totalFieldCount.
				if (currentFieldCount[tometadata][targetField] < totalFieldCount[tometadata][targetField]){
					// this rule has not found a value for this targetField, but maybe a
					// later rule will, so don't raise an error (yet)
					return;
				} else {
					// we have reached the last rule that could provide a value for the
					// targetField and none have, and allowMissing is false, so we have an
					// error.
					throw std::runtime_error("Could not find " + contentType + "." + sourceField + " in message");
				}
			}

			data_t value;
			// if mode is MAP_FROM, erase the source
			if (mode == MAP_FROM) {
				// move the value out of the source
				value = std::move(it->second);
				itmap->erase(it);
			} else if (mode == COPY_FROM) {
				// just copy, don't remove it. Also no need to erase
				value = it->second.copy();
			} else {
				assert(false);
			}

			// second, put it in the target
			findField(data, targetField, true, tometadata).second->second = std::move(value);
			// Marking that the current field value is applied once.
			currentFieldCount[tometadata][targetField] = -1;
			break;
		}
		// apply a defaultValue or set rule
		case SET:
		case DEFAULT_VALUE: {
			// if the key exists in the map but is null, set it to the default val
			// else add the key and default to the map (insert does not clobber)
			auto rv = findField(data, targetField, true, tometadata);
			if (mode == SET || rv.second->second.type_tag() == SAG_DATA_EMPTY) {
				rv.second->second = content.copy();
			}
			// Marking that the  targetField value is applied once.
			currentFieldCount[tometadata][targetField] = -1;
			break;
		}
		// apply forEach rule
		case FOR_EACH: {
			handleForEach(data, allowMissing, contentType + "." + targetField);
			break;
		}
		// it's an enum, really shouldn't happen
		default: throw std::runtime_error("Internal Error: unknown rule type");
	}
}

}}} // com.apama.samples


