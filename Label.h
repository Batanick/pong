#include <memory>

#include "RenderCommon.h"
#include "Font.h"
#include <glm.hpp>

class Label final : public Renderable {

public:
    Label( std::shared_ptr<Font> font, std::string str, int x, int y, glm::vec3 color):
		font( font ),
		str( str ),
        x(x), y(y),
        color(color),
        loaded( false ){
	}

	virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;

    void setText(std::string text);
private :
	const std::shared_ptr<Font> font;
    std::string str;
    const int x,y;
    const glm::vec3 color;

    GLuint vertexBuffer;
    GLuint uvBuffer;

    bool loaded;

    void init( const RenderContext &context );
};