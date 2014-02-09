#include "fontLoader.h"

#include "logging.h"
#include "assetLoader.h"
#include "tinyXml\tinyxml.h"

int extractIntValue( char* const name,  TiXmlElement * root ) {
    if ( !root ) 
        return 0;

    TiXmlElement * const element = root->FirstChildElement( name );
    if ( !element ) 
        return 0;

    return atoi(element->GetText());
}

void loadFontData(std::string path, std::vector<const Glyph> &out, unsigned int &width, unsigned int &height) {
    TiXmlDocument file( path.c_str() );
    
    if (!file.LoadFile()) {
        LOG( "Unable to load font meta file: [%s]", path);
        return;
    }

    TiXmlElement * const root = file.RootElement();
    const char * textureName = root->Attribute( "file" );

    if ( !textureName || !loadDDS(textureName, width, height)) {
        LOG ( "Unable to load texture file: [%s]", textureName );
        return;
    }

    for (TiXmlElement* current = root->FirstChildElement("character"); current; current = current->NextSiblingElement("character")){
        Glyph gl;
        current->Attribute("key", &gl.key);
        gl.height = extractIntValue( "height",current ); 
        gl.width = extractIntValue( "width",current ); 
        gl.x = extractIntValue( "x",current ); 
        gl.y = extractIntValue( "y",current ); 

        out.push_back(gl);
    }
}