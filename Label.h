#include <memory>

#include "RenderCommon.h"
#include "Font.h"

class Label final : public Renderable {

public:
	Label( std::shared_ptr<Font> font):
		font( font ),
		x(0), y(0){
	}

	virtual void render( const RenderContext &context );
	virtual void shutdown();

private :
	const std::shared_ptr<Font> font;
	
	float x;
	float y;
};