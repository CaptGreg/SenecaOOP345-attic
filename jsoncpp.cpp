// file:///usr/share/doc/libjsoncpp-dev/jsoncpp-api-html/index.html
// https://github.com/open-source-parsers/jsoncpp

#include <iostream>
#include <string>
#include <jsoncpp/json/json.h>

std::string config_doc (
R"config(
// Configuration options
{
    // Default encoding for text
    "encoding" : "UTF-8",
    
    // Plug-ins loaded at start-up
    "plug-ins" : [
        "python",
        "c++",
        "ruby"
        ],
        
    // Tab indent size
    "indent" : { "length" : 3, "use_space": true }
}
)config" 
);

// GB code references these functions
void loadPlugIn( std::string& name )      { std::cout << "need to load &"  << name << " plugin\n"; }
void loadPlugIn( std::string&& name )     { std::cout << "need to load &&" << name << " plugin\n"; }

void setIndentLength( const int len )     { std::cout << "setting indent len=" << len << "\n"; }
void setIndentUseSpace( const bool flag ) { std::cout << "setting indent use space flag=" << flag << "\n"; }
std::string& getCurrentEncoding()         { static std::string s("UTF-99");  return s; }
int  getCurrentIndentLength()             { static int len = 3; std::cout << "getCurrentIndentLength: returning " << len << "\n"; return len; }
bool getCurrentIndentUseSpace()           { static bool indentUseSpace = true; std::cout << "getCurrentIndentUseSpace: " << indentUseSpace << "\n"; return indentUseSpace; }


int main()
{

	Json::Value root; // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( config_doc, root );
	if ( !parsingSuccessful )
	{
		// report to the user the failure and their locations in the document.
		std::cout << "Failed to parse configuration\n"
		<< reader.getFormattedErrorMessages();
		return 1;
	}
	else
	{
		std::cout << "**** configuration parse successful\n";
	}

	// Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
	// such member.
	std::string encoding = root.get("encoding", "UTF-8" ).asString();
 
	std::cout << "**** encoding=" << encoding << "\n";

	// Get the value of the member of root named 'encoding', return a 'null' value if
	// there is no such member.
	const Json::Value plugins = root["plug-ins"];
	for ( int index = 0; index < plugins.size(); ++index ) // Iterates over the sequence elements.
		loadPlugIn( plugins[index].asString() );

	setIndentLength( root["indent"].get("length", 3).asInt() );

	setIndentUseSpace( root["indent"].get("use_space", true).asBool() );

	// ...
	// At application shutdown to make the new configuration document:
	// Since Json::Value has implicit constructor for all value types, it is not
	// necessary to explicitly construct the Json::Value object:

	root["encoding"] = getCurrentEncoding();
	root["indent"]["length"] = getCurrentIndentLength();
	root["indent"]["use_space"] = getCurrentIndentUseSpace();

	Json::StyledWriter writer;
	// Make a new JSON document for the configuration. Preserve original comments.
	std::string outputConfig = writer.write( root );

	// You can also use streams. This will put the contents of any JSON
	// stream at a particular sub-value, if you'd like.
	std::cin >> root["subtree"];

	// And you can write to a stream, using the StyledWriter automatically.
	std::cout << root;

	return 0;
}
