#include "RenderCommon.h"

class Bush final : public Renderable {
public:
    virtual void render( const RenderContext &context ) override;
    virtual void shutdown() override;
};