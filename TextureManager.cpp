#include "TextureManager.h"

#include <GL/glew.h>

#include "assetLoader.h"
#include "logging.h"

GLuint TextureManager::loadTexture( std::string fileName ) {
	GLuint textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_2D, textureId );
	glPixelStorei( GL_UNPACK_ALIGNMENT,1 );	

	if ( !loadDDS( fileName ) ) {
		LOG( "Unable to load texture [%s]", fileName );
		return GL_INVALID_VALUE;
	}

	textures.insert( std::make_pair( fileName, textureId ) );
	
	return textureId;
}

GLuint TextureManager::getTextureId( std::string fileName ) {
	const auto texture = textures.find( fileName );
	if ( texture == textures.end() ) {
		LOG( "Unable to find texture id for: [%s]", fileName );
		return GL_INVALID_VALUE;
	}

	return texture->second;
}

void TextureManager::shutdown() {
	for ( const auto textureEntry : textures ) {
		glDeleteTextures( 1, &textureEntry.second );
	}
}

