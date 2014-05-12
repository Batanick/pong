#include "AssetManager.h"

#include <GL/glew.h>

#include "assetLoader.h"
#include "fontLoader.h"
#include "logging.h"

GLuint AssetManager::loadTexture( std::string fileName ) {
	GLuint textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_2D, textureId );
	glPixelStorei( GL_UNPACK_ALIGNMENT,1 );	

    unsigned int width, height;
    if ( !loadDDS( fileName, width, height ) ) {
		LOG( "Unable to load texture [%s]", fileName.c_str() );
		return GL_INVALID_VALUE;
	}

    textures.insert( std::make_pair( fileName, TextureInfo(textureId, width, height) ) );
	
	return textureId;
}

GLuint AssetManager::getTextureId( std::string fileName ) {
	const auto texture = textures.find( fileName );
	if ( texture == textures.end() ) {
		LOG( "Unable to find texture id for: [%s]", fileName.c_str() );
		return GL_INVALID_VALUE;
	}

    return texture->second.textureId;
}

std::shared_ptr<Font> AssetManager::loadFont( std::string metaFileName ) {
    GLuint textureId;
    glGenTextures( 1, &textureId );
    glBindTexture( GL_TEXTURE_2D, textureId );
    glPixelStorei( GL_UNPACK_ALIGNMENT,1 );	
    
    std::vector<const Glyph> glyphs;
    unsigned int width, height;
    loadFontData( metaFileName, glyphs, width, height );

    if (glyphs.size() <= 0) {
        LOG ("Unable to load font: [%s]", metaFileName);
        return std::shared_ptr<Font>();
    }
        
    std::shared_ptr<Font> font = std::shared_ptr<Font>( new Font(TextureInfo(textureId, width, height)) );
    font->init(glyphs);
    return font;
}

void AssetManager::shutdown() {
	for ( const auto textureEntry : textures ) {
        glDeleteTextures( 1, &textureEntry.second.textureId );
	}
    glDeleteTextures(1, &defaultFont->getTextureInfo().textureId);
}

bool AssetManager::init() {
    defaultFont = loadFont( "../textures/arial.xml" );
    return true;
}

std::shared_ptr<Font> AssetManager::getDefaultFont() {
    return defaultFont;
}

